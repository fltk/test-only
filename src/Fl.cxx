//
// "$Id: Fl.cxx,v 1.86 2000/04/11 17:14:11 bill Exp $"
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
Fl_Window	*Fl::grab_,	// most recent Fl::grab()
		*Fl::modal_;	// topmost modal() window
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

static Fl_Window *fl_xfocus;	// which window X thinks has focus
static Fl_Window *fl_xmousewin;// which window X thinks has FL_ENTER

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
  if (!Fl_X::first) return 0; // no windows
  if ((idle && !in_idle) || expired) {
    fl_wait(1,0.0);
  } else if (numtimeouts) {
    fl_wait(1, timeout[0].time);
  } else {
    initclock = 0;
    fl_wait(0,0);
  }
  return 1;
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

void Fl::redraw() {
  for (Fl_X* x = Fl_X::first; x; x = x->next) x->w->redraw();
}

Fl_Window* Fl::first_window() {Fl_X* x = Fl_X::first; return x ? x->w : 0;}

Fl_Window* Fl::next_window(const Fl_Window* w) {
  Fl_X* x = Fl_X::i(w)->next; return x ? x->w : 0;}

////////////////////////////////////////////////////////////////

int Fl::event_inside(int x,int y,int w,int h) /*const*/ {
  int mx = event_x() - x;
  int my = event_y() - y;
  return (mx >= 0 && mx < w && my >= 0 && my < h);
}

int Fl::event_inside(const Fl_Widget& o) /*const*/ {
  return event_inside(o.x(), o.y(), o.w(), o.h());
}

void Fl::focus(Fl_Widget *o) {
  if (grab()) return; // don't do anything while grab is on
  Fl_Widget *p = focus_;
  if (o != p) {
    focus_ = o;
    for (; p && !p->contains(o); p = p->parent()) p->handle(FL_UNFOCUS);
    for (; o; o = o->parent()) o->handle(FL_FOCUS);
  }
}

void Fl::belowmouse(Fl_Widget *o) {
  if (grab()) return; // don't do anything while grab is on
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

// Update focus() and belowmouse() in response to anything that might
// change them.  If the argument is non-zero then a move event is sent
// to wake up the widget that is below the mouse.

// This is called whenever a window is added or hidden, and whenever
// X says the focus or mouse window have changed.

void fl_fix_focus(int sendmove) {

  if (Fl::grab()) return; // don't do anything while grab is on.

  // set focus based on Fl::modal() and fl_xfocus
  Fl_Widget* w = fl_xfocus;
  if (w) {
    while (w->parent()) w = w->parent();
    if (Fl::modal()) w = Fl::modal();
    if (!w->contains(Fl::focus())) {
      Fl::e_keysym = 0; // make sure it does not look like navigation keystroke
      if (w->takesevents() &&
	  w->handle(FL_FOCUS) &&
	  w->contains(Fl::focus()))
	;
      else Fl::focus(w); // give it focus even if it does not want it
    }
  } else
    Fl::focus(0);

//  if (!Fl::pushed()) {
    // set belowmouse based on Fl::modal() and fl_xmousewin:
    w = fl_xmousewin;
    if (w) {
      if (Fl::modal()) w = Fl::modal();
      if (!w->contains(Fl::belowmouse())) {
	Fl::belowmouse(w);
	w->handle(FL_ENTER);
      } else if (sendmove) {
	// send a FL_MOVE event so the enter/leave state is up to date
	Fl::e_x = Fl::e_x_root-fl_xmousewin->x();
	Fl::e_y = Fl::e_y_root-fl_xmousewin->y();
	w->handle(FL_MOVE);
      }
    } else {
      Fl::belowmouse(0);
    }
//  }
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
  if (this == fl_xfocus) fl_xfocus = 0;
  if (this == fl_xmousewin) fl_xmousewin = 0;
  Fl_Tooltip::exit(this);
  fl_fix_focus(0);
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
  Fl_Widget* w = window;

  switch (event) {

  case FL_CLOSE:
    if (grab() || modal() && window != modal()) return 0;
    w->do_callback();
    return 1;

  case FL_SHOW:
    ((Fl_Widget*)w)->show();
    return 1;

  case FL_HIDE:
    ((Fl_Widget*)w)->hide();
    return 1;

  case FL_PUSH:
    Fl_Tooltip::enter((Fl_Widget*)0);
    if (pushed()) w = pushed();
    else if (grab()) w = grab();
    else if (modal() && w != modal()) return 0;
    pushed_ = w;
    if (send(event, w, window)) return 1;
    pushed_ = 0; // stops drag+release from being sent
    // raise windows that are clicked on:
    window->show();
    return 1;

  case FL_MOVE:
//  case FL_DRAG: // does not happen
    fl_xmousewin = window; // this should already be set, but just in case.
    if (grab()) {
      w = grab();
//     } else if (belowmouse()) {
//       if (event_inside(belowmouse())) w = belowmouse();
//       else belowmouse(0);
    }
    if (w != pushed()) send(FL_MOVE, w, window);
    if (pushed()) send(FL_DRAG, pushed(), window);
    return 1;

  case FL_RELEASE: {
    w = pushed();
    if (!(event_state()&(FL_BUTTON1|FL_BUTTON2|FL_BUTTON3))) pushed_=0;
    int r = w ? send(event, w, window) : 0;
    fl_fix_focus(1);
    return r;}

  case FL_UNFOCUS:
    window = 0;
  case FL_FOCUS:
    fl_xfocus = window;
    fl_fix_focus(0);
    return 1;

  case FL_ENTER:
    fl_xmousewin = window;
    fl_fix_focus(0);
    return 1;

  case FL_LEAVE:
    if (window == fl_xmousewin) {fl_xmousewin = 0; fl_fix_focus(0);}
    return 1;

  case FL_KEYBOARD:

    Fl_Tooltip::enter((Fl_Widget*)0);
    fl_xfocus = window; // this should already be set, but just in case.
    // fl_fix_focus(0); clobbers e_keysym, so do not call this, or save it

    // Try sending keystroke to the focus, if any:
    w = grab(); if (!w) w = focus();
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
    // All other events (including FL_SHORTCUT, FL_MOUSEWHEEL,
    // FL_KEYUP, and all new events that we may add) are sent first to
    // the focus, and then to each window around the focus and
    // up, until something accepts it.  Part of this search is done by
    // Fl_Group::handle(), which sends the events to every child.  This
    // causes every widget in the window to be tried eventually, so
    // if something is interested in the event it should see it.

    if (grab()) return send(event, grab(), window);

    w = focus();
    if (!w) {w = modal();
    if (!w) {w = window;}}
    for (; w; w = w->parent()) if (send(event, w, window)) return 1;
    // otherwise fall through to the unknown case:

  case 0:
    // try the chain of global event handlers:
    {for (const handler_link *h = handlers; h; h = h->next)
      if (h->handle(event)) return 1;}
    return 0;

  }
}

////////////////////////////////////////////////////////////////

void Fl_Window::destroy() {

  Fl_X* x = i;
  if (!x) return;

  // remove from the list of windows:
  Fl_X** pp = &Fl_X::first;
  for (; *pp != x; pp = &(*pp)->next) if (!*pp) return;
  *pp = x->next;
  i = 0;

  // recursively remove any subwindows:
  for (Fl_X *w = Fl_X::first; w;) {
    Fl_Window* W = w->w;
    if (W->window() == this) {
      W->destroy();
      w = Fl_X::first;
    } else w = w->next;
  }

  if (this == Fl::modal_) { // we are closing the modal window, find next one:
    Fl_Window* w;
    for (w = Fl::first_window(); w; w = Fl::next_window(w))
      if (w->modal()) break;
    Fl::modal_ = w;
  }

  // Make sure no events are sent to this window:
  throw_focus();
  handle(FL_HIDE);

#ifdef WIN32
  if (x->private_dc) ReleaseDC(x->xid,x->private_dc);
  if (x->xid == fl_window && fl_gc) {
    ReleaseDC(fl_window, fl_gc);
    fl_window = (HWND)-1;
    fl_gc = 0;
  }
#else
  if (x->region) XDestroyRegion(x->region);
#endif
  XDestroyWindow(fl_display, x->xid);

  delete x;
}

Fl_Window::~Fl_Window() {
  destroy();
}

// Fl_Window::show()/hide() are different than Fl_Widget::show()/hide(),
// even though they are not virtual functions.  This was necessary for
// compatability with fltk 1.0.

// Fl_Window::show()/hide() actually create and destroy the X window.
// Fl_Window::show() also has the effect of raising an outer window if
// it is already shown.  The method Fl_Window::shown() is true between
// Fl_Window::show() and hide().

// Fl_Widget::show()/hide() are ignored for outer windows, it is assummed
// these are called only in response to the user iconizing/deiconizing
// the window and thus the window is already in the right state.  For
// child windows these do unmap/map the X window, this was so that
// child windows inside Fl_Tabs work.

// The purpose is to save server resources, and to avoid the need to
// have a fourth state for the window:
//
// visible:		window->shown();
// iconized:		window->shown() && !window->visible();
// not on screen:	!window->shown();
// not on screen but known by system: does not happen in fltk

extern void fl_startup();

void Fl_Window::show() {
  if (parent()) {
    set_visible();
    handle(FL_SHOW);
  } else if (!i) {
    Fl_Group::current(0); // get rid of very common user bug: forgot end()

    // this is the secret place where the world is initialized:
#ifndef WIN32
    fl_open_display();
#endif
    // one-time startup stuff for schemes & config
    fl_startup();
// CET - FIXME    Fl::loadtheme();

    layout();

    // back compatability with older modal() and non_modal() flags:
    if (non_modal() && !fl_modal_for) {
      fl_modal_for = Fl::first_window();
      while (fl_modal_for && fl_modal_for->parent())
	fl_modal_for = fl_modal_for->window();
    }
    // back-compatability automatic size_range() based on resizable():
    if (!size_range_set) {
      if (resizable()) {
	Fl_Widget *o = resizable();
	int minw = o->w(); if (minw > 100) minw = 100;
	int minh = o->h(); if (minh > 100) minh = 100;
	size_range(w() - o->w() + minw, h() - o->h() + minh, 0, 0);
      } else {
	size_range(w(), h(), w(), h());
      }
    }
    create();
    fl_modal_for = 0;
    set_visible();
    handle(FL_SHOW);
    if (modal()) {Fl::modal_ = this; fl_fix_focus(0);}
  } else {
#ifdef WIN32
    if (IsIconic(i->xid)) OpenIcon(i->xid);
    if (!Fl::grab()) // we would lose the capture if we activated the window
      BringWindowToTop(i->xid);
#else
    XMapRaised(fl_display, i->xid);
#endif
  }
}

void Fl_Window::show(const Fl_Window* modal_for) {
  // find the outermost window and make sure it has been shown():
  while (modal_for && modal_for->parent()) modal_for = modal_for->window();
  if (modal_for && modal_for->shown()) fl_modal_for = modal_for;
  show();
  fl_modal_for = 0;
}

int Fl_Window::exec(const Fl_Window* modal_for) {
  clear(); // clear the value()
  set_modal();
  show(modal_for);
  while (shown()) Fl::wait();
  return value();
}

void Fl_Window::show_inside(const Fl_Window* w) {
#ifdef WIN32
  fl_mdi_window = w;
  show();
  fl_mdi_window = 0;
#else
  show(w);
#endif
}

void Fl_Window::hide() {
  destroy();
  clear_visible();
}

// Fl_Widget::show()/hide() call this:
int Fl_Window::handle(int event) {
  if (parent()) switch (event) {
  case FL_SHOW:
    if (!i) create();
    else XMapWindow(fl_display, i->xid);
    break;
  case FL_HIDE:
    if (i && !visible()) XUnmapWindow(fl_display, i->xid);
    break;
  }
  if (Fl_Group::handle(event)) return 1;
  // Make the Escape key close windows:
  if (event == FL_SHORTCUT && !parent() && test_shortcut()) {
    do_callback();
    return 1;
  }
  return 0;
}

// call this to free a selection (or change the owner):
void Fl::selection_owner(Fl_Widget *owner) {
  if (selection_owner_ && owner != selection_owner_)
    selection_owner_->handle(FL_SELECTIONCLEAR);
  if (focus_ && owner != focus_ && focus_ != selection_owner_)
    focus_->handle(FL_SELECTIONCLEAR); // clear non-X-selection highlight
  selection_owner_ = owner;
}

void Fl_Window::flush() {
  make_current();
//if (damage() == FL_DAMAGE_EXPOSE && can_boxcheat(box())) fl_boxcheat = this;
  fl_clip_region(i->region); i->region = 0;
  draw();
}

//
// End of "$Id: Fl.cxx,v 1.86 2000/04/11 17:14:11 bill Exp $".
//
