//
// "$Id: Fl.cxx,v 1.105 2000/07/31 05:52:46 spitzak Exp $"
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
#include <FL/Fl_Shared_Image.H>
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

Fl_Font* Fl_Font_::array = 0;
int Fl_Font_::num_fonts = 0;

char *Fl_Shared_Image::fl_shared_image_root=0;

Fl_Shared_Image  *Fl_Shared_Image::first_image = 0;

int Fl_Shared_Image::image_used=0;
size_t Fl_Shared_Image::mem_usage_limit=0;

size_t Fl_Shared_Image::mem_used=0; 
int Fl_Shared_Image::forbid_delete = 1;

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

// I avoid the overhead of getting the current time when we have no
// timeouts by setting this flag instead of getting the time.
// In this case calling elapse_timeouts() does nothing, but records
// the current time, and the next call will actualy elapse time.
static char reset_clock = 1;

void fl_fix_focus();

#ifdef WIN32
#include "Fl_win32.cxx"
#else
#include "Fl_x.cxx"
#endif

void (*Fl::idle)();
static char in_idle;
static char in_timeout;

int Fl::wait(double time_to_wait) {
  if (numtimeouts) {
    elapse_timeouts();
    if (timeout[0].time <= time_to_wait) time_to_wait = timeout[0].time;
    while (numtimeouts) {
      if (timeout[0].time > 0) break;
      // The first timeout in the array has expired.
      // We must remove timeout from array before doing the callback:
      void (*cb)(void*) = timeout[0].cb;
      void *arg = timeout[0].arg;
      numtimeouts--;
      if (numtimeouts)
	memmove(timeout, timeout+1, numtimeouts*sizeof(Timeout));
      // Now it is safe for the callback to do add_timeout:
      in_timeout = 1;
      cb(arg);
      in_timeout = 0;
    }
  } else {
    reset_clock = 1; // we are not going to check the clock
  }
  if (idle) {
    if (!in_idle) {in_idle = 1; idle(); in_idle = 0;}
    // the idle function may turn off idle, we can then wait:
    if (idle) time_to_wait = 0.0;
  }
  if (time_to_wait <= 0.0) {
    // do flush second so that the results of events are visible:
    int ret = fl_wait(0.0);
    flush();
    return ret;
  } else {
    // do flush first so that user sees the display:
    flush();
    return fl_wait(time_to_wait);
  }
}

int Fl::check() {
  return wait(0.0);
}

#define FOREVER 1e20

int Fl::wait() {
  return wait(FOREVER);
}

int Fl::run() {
  for (;;) {
    // if there are no visible windows we return:
    for (Fl_Window* w = first_window(); ; w = next_window(w)) {
      if (!w) return 0;
      if (w->visible() && !w->parent()) break;
    }
    wait(FOREVER);
  }
}

int Fl::ready() {
  if (numtimeouts) {
    elapse_timeouts();
    if (timeout[0].time <= 0) return 1;
  } else {
    reset_clock = 1;
  }
  return fl_ready();
}

void Fl::add_timeout(double t, Fl_Timeout_Handler cb, void *v) {
  elapse_timeouts();
  repeat_timeout(t, cb, v);
}

void Fl::repeat_timeout(double t, Fl_Timeout_Handler cb, void *v) {

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

int Fl::has_timeout(Fl_Timeout_Handler cb, void *v) {
  for (int i=0; i<numtimeouts; i++)
    if (timeout[i].cb == cb && timeout[i].arg==v) return 1;
  return 0;
}

void Fl::remove_timeout(Fl_Timeout_Handler cb, void *v) {
  int i,j;
  for (i=j=0; i<numtimeouts; i++) {
    if (timeout[i].cb == cb && timeout[i].arg==v) ;
    else {if (j<i) timeout[j]=timeout[i]; j++;}
  }
  numtimeouts = j;
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
    Fl::compose_reset();
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
      w->handle(FL_FOCUS);
      if (w->contains(Fl::focus())) return; // it took it
    }
  }
  // give nothing the focus:
  Fl::focus(0);
}

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
// effects: The window system is told to "grab" events and send all of
// them to this application, and all events we get are sent to the
// "grab handler" rather than to normal widgets.

// On both X and Win32 "this application" has to be identified by a
// window, fltk just picks the top-most displayed window, which is not
// necessarily where the events are really going!

// CET - FIXME - Would someone look at this window grab stuff and see
// whether this is done properly?
// WAS - It didn't do show(), but otherwise this was correct.  I also
// changed it so an arbitrary widget can be used, not just a window
static int widget_grab(int e, void* w) { return ((Fl_Widget*)w)->handle(e); }
void Fl::grab(Fl_Widget* w) { grab(widget_grab, w); }

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

  Fl_Widget* to = window;

  switch (event) {

  case FL_PUSH:
    if (!pushed()) {
      Fl_Tooltip::enter((Fl_Widget*)0);
      if (modal() && window != modal()) return 0;
      pushed_ = window;
    }
    to = pushed();
    break;

  case FL_ENTER:
  case FL_MOVE:
//case FL_DRAG: // does not happen
  case FL_DND_ENTER:
  case FL_DND_DRAG:
    xmousewin = window; // this should already be set, but just in case.
    if (pushed()) {to = pushed_; event = FL_DRAG;}
    else if (modal() && window != modal()) return 1;
    break;

  case FL_LEAVE:
  case FL_DND_LEAVE:
    if (!pushed_) belowmouse(0);
    if (window) return 1;
    break;

  case FL_RELEASE:
    to = pushed();
    if (!(event_pushed())) pushed_=0;
    break;

  case FL_DND_RELEASE:
    to = belowmouse();
    break;

  case FL_KEY:
    Fl_Tooltip::enter((Fl_Widget*)0);
    xfocus = window; // this should already be set, but just in case.
    to = focus();

  default:
    if (modal()) to = modal();
    break;

  case 0: // events from system that fltk does not understand
    to = 0;
    break;
  }

  int ret = 0;
  if (to && window) {
    int dx = window->x();
    int dy = window->y();
    for (const Fl_Widget* w = to; w; w = w->parent())
      if (w->is_window()) {dx -= w->x(); dy -= w->y();}
    int save_x = Fl::e_x; Fl::e_x += dx;
    int save_y = Fl::e_y; Fl::e_y += dy;
    ret = to->handle(event);
    Fl::e_y = save_y;
    Fl::e_x = save_x;
  }

  if (!ret) {
    switch (event) {
    case FL_KEY:
      // if keyboard is ignored, try shortcut events:
      if (handle(FL_SHORTCUT, window)) return 1;
      // try flipping the case of letter shortcuts:
      if (isalpha(event_text()[0])) {
	char* c = (char*)event_text(); // cast away const
	*c = isupper(*c) ? tolower(*c) : toupper(*c);
	if (handle(FL_SHORTCUT, window)) return 1;
      }
      return 0;

      // rejected mouse events produce FL_LEAVE events:
    case FL_ENTER:
    case FL_MOVE:
    case FL_DND_ENTER:
    case FL_DND_DRAG:
      belowmouse(0);
      break;
    }

    // try the chain of global event handlers:
    for (const handler_link *h = handlers; h; h = h->next)
      if (h->handle(event)) {ret = 1; break;}
  }

  if (event == FL_RELEASE && !pushed_ && xmousewin) {
    // send a dummy move event when the user releases the mouse:
    handle(FL_MOVE, xmousewin);
  }

  return ret;
}

//
// End of "$Id: Fl.cxx,v 1.105 2000/07/31 05:52:46 spitzak Exp $".
//
