//
// "$Id: Fl.cxx,v 1.91 2000/05/15 05:52:24 bill Exp $"
//
// Main event handling code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Tooltip.H>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>

//
// Globals...
//

Fl_Widget	*Fl::belowmouse_,
		*Fl::pushed_,
		*Fl::focus_,
		*Fl::selection_owner_;
int		(*Fl::grab_)(int, void*);
static void* grab_data;
Fl_Window	*Fl::modal_;	// topmost modal() window
int		Fl::damage_,
		Fl::e_x,
		Fl::e_y,
		Fl::e_dx,
		Fl::e_dy,
		Fl::e_x_root,
		Fl::e_y_root,
		Fl::e_state,
		Fl::e_clicks,
		Fl::e_is_click,
		Fl::e_keysym;
char		*Fl::e_text = "";
int		Fl::e_length;

static Fl_Window *xfocus;	// which window X thinks has focus
static Fl_Window *xmousewin;// which window X thinks has FL_ENTER

// Timeouts are insert-sorted into order.  This works good if there
// are only a small number:

static struct Timeout {
  double time;
  void (*cb)(void*);
  void* arg;
} * timeout;
static int numtimeouts;
static int timeout_array_size;
static int initclock; // if false we didn't call fl_elapsed() last time

void fl_fix_focus();

#ifdef _WIN32
#include "Fl_win32.cxx"
#else
#include "Fl_x.cxx"
#endif

////////////////////////////////////////////////////////////////

void Fl::add_timeout(double t, Fl_Timeout_Handler cb, void *v) {

  fl_elapsed();

  if (numtimeouts >= timeout_array_size) {
    timeout_array_size = 2*timeout_array_size+1;
    timeout = (Timeout*)realloc(timeout, timeout_array_size*sizeof(Timeout));
  }

  // insert-sort the new timeout:
  int i;
  for (i=0; i<numtimeouts; i++) {
    if (timeout[i].time > t) {
      for (int j=numtimeouts; j>i; j--) timeout[j] = timeout[j-1];
      break;
    }
  }
  timeout[i].time = t;
  timeout[i].cb = cb;
  timeout[i].arg = v;

  numtimeouts++;
}

void Fl::remove_timeout(Fl_Timeout_Handler cb, void *v) {
  int i,j;
  for (i=j=0; i<numtimeouts; i++) {
    if (timeout[i].cb == cb && timeout[i].arg==v) ;
    else {if (j<i) timeout[j]=timeout[i]; j++;}
  }
  numtimeouts = j;
}

static int call_timeouts() {
  int expired = 0;
  while (numtimeouts) {
    if (timeout[0].time > 0) break;
    // we must remove timeout from array before doing the callback:
    void (*cb)(void*) = timeout[0].cb;
    void *arg = timeout[0].arg;
    numtimeouts--; expired++;
    if (numtimeouts) memmove(timeout, timeout+1, numtimeouts*sizeof(Timeout));
    // now it is safe for the callback to do add_timeout:
    cb(arg);
  }
  return expired;
}

void Fl::flush() {
  if (damage()) {
    damage_ = 0;
    for (Fl_X* x = Fl_X::first; x; x = x->next) {
      if (x->wait_for_expose) {damage_ = 1; continue;}
      Fl_Window* w = x->w;
      if (!w->visible_r()) continue;
      if (w->damage() & FL_DAMAGE_LAYOUT) w->layout();
      if (w->damage()) {w->flush(); w->clear_damage();}
    }
  }
#ifndef WIN32
  if (fl_display) XFlush(fl_display);
#endif
}

void (*Fl::idle)();
static char in_idle;
static void callidle() {
  if (!Fl::idle || in_idle) return;
  in_idle = 1;
  Fl::idle();
  in_idle = 0;
}

int Fl::wait() {
  callidle();
  int expired = 0;
  if (numtimeouts) {fl_elapsed(); expired = call_timeouts();}
  flush();
  if ((idle && !in_idle) || expired) {
    fl_wait(1,0.0);
  } else if (numtimeouts) {
    fl_wait(1, timeout[0].time);
  } else {
    initclock = 0;
    fl_wait(0,0);
  }
  return Fl_X::first != 0; // return true if there is a window
}

double Fl::wait(double time) {
  callidle();
  int expired = 0;
  if (numtimeouts) {time -= fl_elapsed(); expired = call_timeouts();}
  flush();
  double wait_time = (idle && !in_idle) || expired ? 0.0 : time;
  if (numtimeouts && timeout[0].time < wait_time) wait_time = timeout[0].time;
  fl_wait(1, wait_time);
  return time - fl_elapsed();
}

int Fl::check() {
  callidle();
  if (numtimeouts) {fl_elapsed(); call_timeouts();}
  fl_wait(1, 0.0);
  flush();
  return Fl_X::first != 0; // return true if there is a window
}

int Fl::run() {
  while (wait());
  return 0;
}

////////////////////////////////////////////////////////////////
// Window list management:

Fl_X* Fl_X::first;

Fl_Window* fl_find(Window xid) {
  Fl_X *window;
  for (Fl_X **pp = &Fl_X::first; (window = *pp); pp = &window->next)
    if (window->xid == xid) {
      if (window != Fl_X::first && !Fl::modal()) {
	// make this window be first to speed up searches
	// this is not done if modal is true to avoid messing up modal stack
	*pp = window->next;
	window->next = Fl_X::first;
	Fl_X::first = window;
      }
      return window->w;
    }
  return 0;
}

Fl_Window* Fl::first_window() {
  Fl_X* x = Fl_X::first;
  return x ? x->w : 0;
}

Fl_Window* Fl::next_window(const Fl_Window* w) {
  Fl_X* x = Fl_X::i(w)->next;
  return x ? x->w : 0;
}

void Fl::first_window(Fl_Window* window) {
  if (!window || !window->shown()) return;
  fl_find(fl_xid(window));
}

void Fl::redraw() {
  for (Fl_X* x = Fl_X::first; x; x = x->next) x->w->redraw();
}

////////////////////////////////////////////////////////////////
// Event handling:

int Fl::event_inside(int x,int y,int w,int h) /*const*/ {
  int mx = event_x() - x;
  int my = event_y() - y;
  return (mx >= 0 && mx < w && my >= 0 && my < h);
}

int Fl::event_inside(const Fl_Widget& o) /*const*/ {
  return event_inside(o.x(), o.y(), o.w(), o.h());
}

void Fl::focus(Fl_Widget *o) {
  Fl_Widget *p = focus_;
  if (o != p) {
    focus_ = o;
    for (; p && !p->contains(o); p = p->parent()) p->handle(FL_UNFOCUS);
    for (; o; o = o->parent()) o->handle(FL_FOCUS);
  }
}

void Fl::belowmouse(Fl_Widget *o) {
  Fl_Tooltip::enter(o);
  Fl_Widget *p = belowmouse_;
  if (o != p) {
    event_is_click(0);
    belowmouse_ = o;
    for (; p && !p->contains(o); p = p->parent()) p->handle(FL_LEAVE);
  }
}

void Fl::pushed(Fl_Widget *o) {
  pushed_ = o;
}

// Update focus() in response to anything that might change it.  We
// need to keep the focus pointing at modal windows, even if the window
// system thinks otherwise...

// This is called whenever a window is added or hidden, and whenever
// X says the focus window has changed.

void fl_fix_focus() {

  if (Fl::grab()) return; // don't do anything while grab is on.

  Fl_Widget* w = xfocus;

  if (w) {
    // System says this program has focus.
    // Modal overrides anything the system says:
    if (Fl::modal()) w = Fl::modal();
    // Move to outermost window:
    while (w->parent()) w = w->parent();
    if (w->contains(Fl::focus())) return; // it already has it
    // Give it the focus:
    if (w->takesevents()) {
      Fl::e_keysym = 0; // make sure it does not look like navigation keystroke
      w->handle(FL_FOCUS);
      if (w->contains(Fl::focus())) return; // it took it
    }
  }
  // give nothing the focus:
  Fl::focus(0);
}

#ifndef WIN32
Fl_Widget *fl_selection_requestor; // from Fl_cutpaste.C
#endif

// This function is called by ~Fl_Widget() and by Fl_Widget::deactivate
// and by Fl_Widget::hide().  It indicates that the widget does not want
// to receive any more events, and also removes all global variables that
// point at the widget.
// I changed this from the 1.0.1 behavior, the older version could send
// FL_LEAVE or FL_UNFOCUS events to the widget.  This appears to not be
// desirable behavior and caused flwm to crash.

void Fl_Widget::throw_focus() {
  if (contains(Fl::pushed())) Fl::pushed_ = 0;
  if (contains(Fl::selection_owner())) Fl::selection_owner_ = 0;
#ifndef WIN32
  if (contains(fl_selection_requestor)) fl_selection_requestor = 0;
#endif
  if (contains(Fl::belowmouse())) Fl::belowmouse_ = 0;
  if (contains(Fl::focus())) Fl::focus_ = 0;
  if (this == xfocus) xfocus = 0;
  if (this == xmousewin) xmousewin = 0;
  Fl_Tooltip::exit(this);
  fl_fix_focus();
}

// call this to free a selection (or change the owner):
void Fl::selection_owner(Fl_Widget *owner) {
  if (selection_owner_ && owner != selection_owner_)
    selection_owner_->handle(FL_SELECTIONCLEAR);
  if (focus_ && owner != focus_ && focus_ != selection_owner_)
    focus_->handle(FL_SELECTIONCLEAR); // clear non-X-selection highlight
  selection_owner_ = owner;
}

////////////////////////////////////////////////////////////////

// "Grab" is done while menu systems are up.  This has two
// effects: The window system is told to "grab" events and sed all of
// them to this application, and all events we get are sent to the
// "grab handler" rather than to normal widgest.

// On both X and Win32 "this application" has to be identified by a
// window, fltk just picks the top-most displayed window, which is not
// necessarily where the events are really going!

void Fl::grab(int (*cb)(int, void*), void* user_data) {
  grab_ = cb;
  grab_data = user_data;
#ifdef WIN32
  HWND w = fl_xid(first_window());
  SetActiveWindow(w); // is this necessary?
  SetCapture(w);
#else
  XGrabPointer(fl_display,
	       fl_xid(first_window()),
	       1,
	       ButtonPressMask|ButtonReleaseMask|
	       ButtonMotionMask|PointerMotionMask,
	       GrabModeAsync,
	       GrabModeAsync, 
	       None,
	       0,
	       fl_event_time);
  XGrabKeyboard(fl_display,
		fl_xid(first_window()),
		1,
		GrabModeAsync,
		GrabModeAsync, 
		fl_event_time);
#endif
}

void Fl::release() {
  grab_ = 0;
#ifdef WIN32
  ReleaseCapture();
#else
  XUngrabKeyboard(fl_display, fl_event_time);
  XUngrabPointer(fl_display, fl_event_time);
  // this flush is done in case the picked menu item goes into
  // an infinite loop, so we don't leave the X server locked up:
  XFlush(fl_display);
#endif
  if (xmousewin) {
    // send a FL_MOVE event so the enter/leave state is up to date
    Fl::e_x = Fl::e_x_root-xmousewin->x();
    Fl::e_y = Fl::e_y_root-xmousewin->y();
    xmousewin->handle(FL_MOVE);
  } else {
    Fl::belowmouse(0);
  }
}

////////////////////////////////////////////////////////////////

// Call to->handle but first replace the mouse x/y with the correct
// values to account for nested X windows. 'window' is the outermost
// window the event was posted to by X:
static int send(int event, Fl_Widget* to, Fl_Window* window) {
  int dx = window->x();
  int dy = window->y();
  for (const Fl_Widget* w = to; w; w = w->parent())
    if (w->is_window()) {dx -= w->x(); dy -= w->y();}
  int save_x = Fl::e_x; Fl::e_x += dx;
  int save_y = Fl::e_y; Fl::e_y += dy;
  int ret = to->handle(event);
  Fl::e_y = save_y;
  Fl::e_x = save_x;
  return ret;
}

struct handler_link {
  int (*handle)(int);
  const handler_link *next;
};

static const handler_link *handlers = 0;

void Fl::add_handler(int (*h)(int)) {
  handler_link *l = new handler_link;
  l->handle = h;
  l->next = handlers;
  handlers = l;
}

int Fl::handle(int event, Fl_Window* window)
{
  if (grab_) return grab_(event, grab_data);

  Fl_Widget* w = window;

  switch (event) {

  case FL_PUSH:
    Fl_Tooltip::enter((Fl_Widget*)0);
    if (pushed()) w = pushed();
    else if (modal() && w != modal()) return 0;
    pushed_ = w;
    if (send(event, w, window)) return 1;
    pushed_ = 0; // stops drag+release from being sent
    // raise windows that are clicked on:
    window->show();
    return 1;

  case FL_ENTER:
  case FL_MOVE:
//case FL_DRAG: // does not happen
    xmousewin = window; // this should already be set, but just in case.
    if (w != pushed() && (!modal() || w == modal())) send(event, w, window);
    if (pushed()) send(FL_DRAG, pushed(), window);
    return 1;

  case FL_LEAVE:
    belowmouse(0);
    return 1;

  case FL_RELEASE:
    w = pushed();
    if (!(event_pushed())) pushed_=0;
    if (w) return send(event, w, window);
    return 0;

  case FL_KEYBOARD:

    Fl_Tooltip::enter((Fl_Widget*)0);
    xfocus = window; // this should already be set, but just in case.

    // Try sending keystroke to the focus, if any:
    w = focus();
    if (w && send(FL_KEYBOARD, w, window)) return 1;

    // try flipping the case of letter shortcuts:
    if (isalpha(event_text()[0])) {
      if (handle(FL_SHORTCUT, window)) return 1;
      char* c = (char*)event_text(); // cast away const
      *c = isupper(*c) ? tolower(*c) : toupper(*c);
    }

    event = FL_SHORTCUT;
    // fall through to the shortcut handling case:

  default:
    // This includes FL_SHORTCUT, FL_MOUSEWHEEL, FL_KEYUP, FL_ENTER,
    // and many other events of interest.
#if 1
    // nice simple sending of the event to outermost windows:
    if (send(event, modal() ? modal() : window, window)) return 1;
#else
    // Old code that preferred to send these near the focus first:
    w = window;
    if (w->contains(focus())) w = focus();
    if (modal() && !modal()->contains(w)) w = modal();
    for (; w; w = w->parent()) if (send(event, w, window)) return 1;
#endif
    // otherwise fall through to the unknown case:

  case 0:
    // try the chain of global event handlers:
    {for (const handler_link *h = handlers; h; h = h->next)
      if (h->handle(event)) return 1;}
    return 0;

  }
}

//
// End of "$Id: Fl.cxx,v 1.91 2000/05/15 05:52:24 bill Exp $".
//
