//
// "$Id: Fl.cxx,v 1.158 2002/12/15 10:42:53 spitzak Exp $"
//
// Main event handling code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/run.h>
#include <fltk/events.h>
#include <fltk/error.h>
#include <fltk/damage.h>
#include <fltk/layout.h>
#include <fltk/Window.h>
#include <fltk/Style.h>
#include <fltk/Tooltip.h>
#include <fltk/x.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>
#include <assert.h>
// things that should be in different source files:
#include <fltk/FL_VERSION.h>
#include <fltk/ScreenInfo.h>

using namespace fltk;

float fltk::version() {return FL_VERSION;}

//
// Globals...
//

Widget	*fltk::belowmouse_,
	*fltk::pushed_,
	*fltk::focus_,
	*fltk::modal_;
int	fltk::e_type,
	fltk::e_x,
	fltk::e_y,
	fltk::e_dx,
	fltk::e_dy,
	fltk::e_x_root,
	fltk::e_y_root,
	fltk::e_state,
	fltk::e_clicks,
	fltk::e_is_click,
	fltk::e_keysym;
char	*fltk::e_text = "";
int	fltk::e_length;
bool	fltk::grab_,
	fltk::exit_modal_;

static Window *xfocus;	// which window X thinks has focus
static Window *xmousewin; // which window X thinks has ENTER

// Update focus() in response to anything that might change it.
// This is called whenever a window is added or hidden, and whenever
// X says the focus window has changed.
static void fix_focus() {
  Widget* w = xfocus;
  // Modal overrides whatever the system says the focus is:
  if (grab_ || w && modal_) w = modal_;
  if (w) {
    if (w->contains(focus())) return; // already has it
    e_keysym = 0; // make widgets not think a keystroke moved focus
    if (w->take_focus()) return;
  }
  // give nothing the focus:
  focus(0);
}

#ifdef _WIN32
# include "Fl_win32.cxx"
#elif (defined(__APPLE__) && !USE_X11)
# include "Fl_mac.cxx"
#else
# include "Fl_x.cxx"
#endif

////////////////////////////////////////////////////////////////
// Timeouts are stored in a sorted list, so only the first one needs
// to be checked to see if any should be called.

struct Timeout {
  float time;
  void (*cb)(void*);
  void* arg;
  Timeout* next;
};
static Timeout* first_timeout, *free_timeout;

#ifndef _WIN32
#include <sys/time.h>
#endif

// I avoid the overhead of getting the current time when we have no
// timeouts by setting this flag instead of getting the time.
// In this case calling elapse_timeouts() does nothing, but records
// the current time, and the next call will actualy elapse time.
static char reset_clock = 1;

static void elapse_timeouts() {
#ifdef _WIN32
  unsigned long newclock = GetTickCount();
  static unsigned long prevclock;
  float elapsed = (newclock-prevclock)/1000.0f;
  prevclock = newclock;
#else
  static struct timeval prevclock;
  struct timeval newclock;
  gettimeofday(&newclock, NULL);
  float elapsed = float(newclock.tv_sec - prevclock.tv_sec) +
    float(newclock.tv_usec - prevclock.tv_usec)/1000000.0f;
  prevclock.tv_sec = newclock.tv_sec;
  prevclock.tv_usec = newclock.tv_usec;
#endif
  if (reset_clock) {
    reset_clock = 0;
  } else if (elapsed > 0) {
    for (Timeout* t = first_timeout; t; t = t->next) t->time -= elapsed;
  }
}

// Continuously-adjusted error value, this is a number <= 0 for how late
// we were at calling the last timeout. This appears to make repeat_timeout
// very accurate even when processing takes a significant portion of the
// time interval:
static float missed_timeout_by;

void fltk::add_timeout(float time, TimeoutHandler cb, void *arg) {
  elapse_timeouts();
  repeat_timeout(time, cb, arg);
}

void fltk::repeat_timeout(float time, TimeoutHandler cb, void *arg) {
  time += missed_timeout_by; if (time < -.05f) time = 0;
  Timeout* t = free_timeout;
  if (t) free_timeout = t->next;
  else t = new Timeout;
  t->time = time;
  t->cb = cb;
  t->arg = arg;
  // insert-sort the new timeout:
  Timeout** p = &first_timeout;
  while (*p && (*p)->time <= time) p = &((*p)->next);
  t->next = *p;
  *p = t;
}

bool fltk::has_timeout(TimeoutHandler cb, void *arg) {
  for (Timeout* t = first_timeout; t; t = t->next)
    if (t->cb == cb && t->arg == arg) return true;
  return false;
}

void fltk::remove_timeout(TimeoutHandler cb, void *arg) {
  // This version removes all matching timeouts, not just the first one.
  // This may change in the future.
  for (Timeout** p = &first_timeout; *p;) {
    Timeout* t = *p;
    if (t->cb == cb && t->arg == arg) {
      *p = t->next;
      t->next = free_timeout;
      free_timeout = t;
    } else {
      p = &(t->next);
    }
  }
}

////////////////////////////////////////////////////////////////
// Checks are just stored in a list. They are called in the reverse
// order that they were added (this may change in the future).
// This is a bit messy because I want to allow checks to be added,
// removed, and have wait() called from inside them, to do this
// next_check points at the next unprocessed one for the outermost
// call to wait().

struct Check {
  void (*cb)(void*);
  void* arg;
  Check* next;
};
static Check* first_check, *next_check, *free_check;

void fltk::add_check(TimeoutHandler cb, void *arg) {
  Check* t = free_check;
  if (t) free_check = t->next;
  else t = new Check;
  t->cb = cb;
  t->arg = arg;
  t->next = first_check;
  if (next_check == first_check) next_check = t;
  first_check = t;
}

void fltk::remove_check(TimeoutHandler cb, void *arg) {
  for (Check** p = &first_check; *p;) {
    Check* t = *p;
    if (t->cb == cb && t->arg == arg) {
      if (next_check == t) next_check = t->next;
      *p = t->next;
      t->next = free_check;
      free_check = t;
    } else {
      p = &(t->next);
    }
  }
}

////////////////////////////////////////////////////////////////
// wait/run/check/ready:

void (*fltk::idle)(); // see add_idle.cxx for the add/remove functions

static bool in_idle;

#define FOREVER 1e20f

int fltk::run() {
  while (Window::first()) wait(FOREVER);
  return(0);
// WAS: This was tried for fltk 2.0, and the callback for closing the last
// window in Window.C called exit(). This proved to be unpopular:
//  for (;;) wait(FOREVER);
}

int fltk::wait() {
  return wait(FOREVER);
}

int fltk::wait(float time_to_wait) {
  int ret = 0;
  // checks are a bit messy so that add/remove and wait may be called
  // from inside them without causing an infinite loop. We must also
  // do them first so that they can install an idle or timeout function:
  if (next_check == first_check) {
    while (next_check) {
      Check* check = next_check;
      next_check = check->next;
      (check->cb)(check->arg);
    }
    next_check = first_check;
  }
  if (idle) {
    if (!in_idle) {in_idle = true; idle(); in_idle = false;}
    // the idle function may turn off idle, we can then wait:
    if (idle && time_to_wait > 0) {time_to_wait = 0; ret = 1;}
  }
  if (first_timeout) {
    elapse_timeouts();
    Timeout *t;
    while ((t = first_timeout)) {
      if (t->time > 0) {
	if (t->time < time_to_wait) {time_to_wait = t->time; ret = 1;}
	break;
      }
      // The first timeout in the array has expired.
      missed_timeout_by = t->time;
      // We must remove timeout from array before doing the callback:
      void (*cb)(void*) = t->cb;
      void *arg = t->arg;
      first_timeout = t->next;
      t->next = free_timeout;
      free_timeout = t;
      // Now it is safe for the callback to do add_timeout:
      cb(arg);
      // return immediately afterwards because timeout was done:
      time_to_wait = 0; ret = 1;
    }
  } else {
    reset_clock = 1; // remember that elapse_timeouts was not called
  }
  // run the system-specific part that waits for sockets & events:
  if (time_to_wait <= 0) time_to_wait = 0;
  else flush();
  if (fl_wait(time_to_wait)) ret = 1;
  if (!time_to_wait) flush();
  return ret;
}

int fltk::check() {
  return wait(0.0);
}

int fltk::ready() {
  if (first_timeout) {
    elapse_timeouts();
    if (first_timeout->time <= 0) return 1;
  } else {
    reset_clock = 1;
  }
  // run the system-specific part:
  return fl_ready();
}

////////////////////////////////////////////////////////////////
// Window list management:

CreatedWindow* CreatedWindow::first;

Window* fltk::find(XWindow xid) {
  CreatedWindow *x;
  for (CreatedWindow **pp = &CreatedWindow::first; (x = *pp); pp = &x->next)
    if (x->xid == xid) {
      if (x != CreatedWindow::first) {
	// make this window be first to speed up searches
	*pp = x->next;
	x->next = CreatedWindow::first;
	CreatedWindow::first = x;
      }
      return x->window;
    }
  return 0;
}

Window* Window::first() {
  for (CreatedWindow* x = CreatedWindow::first;; x = x->next) {
    if (!x) return 0;
    Window* window = x->window;
    if (window->visible() && !window->parent() && !window->override())
      return window;
  }
}

Window* Window::next() {
  for (CreatedWindow* x = CreatedWindow::find(this)->next;; x = x->next) {
    if (!x) return 0;
    Window* window = x->window;
    if (window->visible() && !window->parent() && !window->override())
      return window;
  }
}

void Window::first(Window* window) {
  if (!window || !window->shown()) return;
  fltk::find(xid(window));
}

void fltk::redraw() {
  for (CreatedWindow* x = CreatedWindow::first; x; x = x->next)
    x->window->redraw();
}

bool fl_windows_damaged;

void fltk::flush() {
  if (fl_windows_damaged) {
    fl_windows_damaged = false;
    for (CreatedWindow* x = CreatedWindow::first; x; x = x->next) {
      if (x->wait_for_expose) {fl_windows_damaged = true; continue;}
      Window* window = x->window;
      if (window->visible_r() && window->w()>0 && window->h()>0) {
	if (window->layout_damage()) window->layout();
	if (window->damage() || x->region) {
	  window->flush();
	  window->set_damage(0);
	}
      }
      // destroy damage regions for windows that don't use them:
      if (x->region) {XDestroyRegion(x->region); x->region = 0;}
    }
  }
#ifdef _WIN32
  GdiFlush();
#elif (defined(__APPLE__) && !USE_X11)
  //QDFlushPortBuffer( GetWindowPort(xid), 0 ); // \todo do we need this?
#else
  if (xmousewin) {
    CreatedWindow* i = CreatedWindow::find(xmousewin);
    if (i->cursor != None && !i->cursor_for->contains(belowmouse_)) {
      i->cursor = None;
      XDefineCursor(xdisplay, i->xid, None);
    }
  }
  XFlush(xdisplay);
#endif
}

////////////////////////////////////////////////////////////////
// Event handling:

bool fltk::event_inside(int x,int y,int w,int h) /*const*/ {
  int mx = e_x - x;
  int my = e_y - y;
  return (mx >= 0 && mx < w && my >= 0 && my < h);
}

void fltk::focus(Widget *o) {
  Widget *p = focus_;
  if (o != p) {
    compose_reset();
    focus_ = o;
    for (; p && !p->contains(o); p = p->parent()) p->handle(UNFOCUS);
    if (o) {
      o->handle(FOCUS);
      for (; (o = o->parent()); ) o->handle(FOCUS_CHANGE);
    }
  }
}

static bool dnd_flag; // makes belowmouse send DND_LEAVE instead of LEAVE

void fltk::belowmouse(Widget *o) {
  Widget *p = belowmouse_;
  if (o != p) {
    belowmouse_ = o;
    for (; p && !p->contains(o); p = p->parent())
      p->handle(dnd_flag ? DND_LEAVE : LEAVE);
  }
}

void fltk::pushed(Widget *o) {
  pushed_ = o;
}

// This function is called by ~Widget() and by Widget::deactivate
// and by Widget::hide().  It indicates that the widget does not want
// to receive any more events, and also removes all global variables that
// point at the widget.
// I changed this from the 1.0.1 behavior, the older version could send
// LEAVE or UNFOCUS events to the widget.  This appears to not be
// desirable behavior and caused flwm to crash.

void Widget::throw_focus() {
  if (contains(fltk::pushed())) pushed_ = 0;
#if !defined(_WIN32) && !(defined(__APPLE__) && !USE_X11)
  if (contains(selection_requestor)) selection_requestor = 0;
#endif
  if (contains(fltk::belowmouse())) {belowmouse_ = 0; e_is_click = 0;}
  if (this == xmousewin) xmousewin = Window::first();
  if (contains(focus())) focus_ = 0;
  if (this == xfocus) xfocus = 0;
  if (this == Tooltip::current()) Tooltip::current(0);
  if (this == modal_) {modal_ = 0; exit_modal();}
}

////////////////////////////////////////////////////////////////

void fltk::modal(Widget* widget, bool grab) {

  // release the old grab:
  if (grab_) {
    grab_ = false;
#ifdef _WIN32
    ReleaseCapture();
    // if (event() == PUSH) repost_the_push_event(); NYI
#elif (defined(__APPLE__) && !USE_X11)
    // dunno what to do here
#else
    XUngrabKeyboard(xdisplay, event_time);
    event_is_click(0); // avoid double click
    XAllowEvents(xdisplay, event()==PUSH ? ReplayPointer : AsyncPointer, CurrentTime);
    XUngrabPointer(xdisplay, event_time); // Qt did not do this...
    XFlush(xdisplay); // make sure we are out of danger before continuing...
#endif
    // because we "pushed back" the PUSH, make it think no buttons are down:
    e_state &= 0xffffff;
    e_keysym = 0;
  }

  // start the new grab:
  // Both X and Win32 have the annoying requirement that a visible window
  // be used as a target for the events, and it cannot disappear while the
  // grab is running. I just grab fltk's first window:
  if (grab && widget) {
#ifdef _WIN32
    Window* window = Window::first();
    if (window) {
      SetActiveWindow(xid(window));
      SetCapture(xid(window));
      grab_ = true;
    }
#elif (defined(__APPLE__) && !USE_X11)
    // dunno what to do here
#else
    Window* window = Window::first();
    if (window &&
	XGrabKeyboard(xdisplay,
		      xid(window),
		      true, // owner_events
		      GrabModeAsync, // pointer_mode
		      GrabModeAsync, // keyboard_mode
		      event_time) == GrabSuccess) {
      //XAllowEvents(xdisplay, SyncKeyboard, CurrentTime);
      if (XGrabPointer(xdisplay,
		       xid(window),
		       true, // owner_events
		       ButtonPressMask|ButtonReleaseMask|
		       ButtonMotionMask|PointerMotionMask,
		       GrabModeSync, // pointer_mode
		       GrabModeAsync, // keyboard_mode
		       None, // confine_to
		       None, // cursor
		       event_time) == GrabSuccess) {
	grab_ = true;
	XAllowEvents(xdisplay, SyncPointer, CurrentTime);
      } else {
	//printf("XGrabPointer failed\n");
	XUngrabKeyboard(xdisplay, event_time);
      }
    } else {
      //printf("XGrabKeyboard failed\n");
    }
#endif
  }

  modal_ = widget;
  fix_focus();
  // Generate a dummy move event so the highlights are correct for
  // the new modal widget. This is mostly useful when exiting modal
  // state because it will correct the highlights in the new top
  // window. This also stops tooltips from appearing in for the
  // widget the mouse ends up pointing at. For this to work you should
  // hide any modal windows or widgets before calling this to turn
  // modal state off.
  if (xmousewin) {
    handle(MOVE, xmousewin);
    // Don't pop up the tooltip for whatever they are pointing at:
    Tooltip::current(belowmouse_);
  }
//    printf("modal %s, xmousewin %s, belowmouse %s\n",
//  	 modal_ ? modal_->label() : "NULL",
//  	 xmousewin ? xmousewin->label() : "NULL",
//  	 belowmouse_ ? belowmouse_->label() : "NULL");
  exit_modal_ = false;
}

////////////////////////////////////////////////////////////////

struct handler_link {
  int (*handle)(int, Window*);
  const handler_link *next;
};

static const handler_link *handlers = 0;

void fltk::add_event_handler(int (*h)(int, Window*)) {
  handler_link *l = new handler_link;
  l->handle = h;
  l->next = handlers;
  handlers = l;
}

bool (*fl_local_grab)(int); // used by fl_dnd_x.cxx

bool fltk::handle(int event, Window* window)
{
  e_type = event;
  if (fl_local_grab) return fl_local_grab(event);

  Widget* to = window;

  switch (event) {

  case PUSH:
    if (pushed()) to = pushed();
    Tooltip::current(to);
    break;

  case ENTER:
    // this is a fix for DnD where the parent of a widget accepts the
    // DND_DRAG but the widget accepts DRAG. When the mouse is
    // released X generated a drag event which would cause the belowmouse
    // widget to change before the release event happens.
    if (to->contains(belowmouse())) return 0;
  case MOVE:
//case DRAG: // does not happen
    if (pushed()) {to = pushed_; event = DRAG; break;}
    {Widget* pbm = belowmouse();
    if (modal_ && !modal_->contains(to)) to = modal_;
    bool ret = to && to->send(MOVE);
    if (pbm != belowmouse()) Tooltip::enter(belowmouse());
    return ret;}

  case LEAVE:
    if (!pushed_) {belowmouse(0); Tooltip::exit();}
    return true;

  case RELEASE:
    to = pushed();
    if (!event_state(ANY_BUTTON)) pushed_=0;
    break;

  case DND_ENTER:
  case DND_DRAG:
    dnd_flag = true;
    break;

  case DND_LEAVE:
    dnd_flag = true;
    belowmouse(0);
    dnd_flag = false;
    return true;

  case DND_RELEASE:
    to = belowmouse();
    break;

  case KEY:
    Tooltip::exit();
    xfocus = window; // this should already be set, but just in case.
    // try sending keystroke to the focus:
    to = focus();
    if (modal_ && !modal_->contains(to)) to = modal_;
    while (to) {
      if (to->send(event)) return true;
      to = to->parent();
    }
    // try sending a shortcut to the window:
    if (handle(SHORTCUT, window)) return true;
    // Try using upper-case instead of lower-case for letter shortcuts:
    if (islower(e_text[0])) {
      e_text[0] ^= 0x20;
      return handle(SHORTCUT, window);
    }
    return false;

  case 0: // events from system that fltk does not understand
    to = 0;
    break;
  }

  // restrict to modal widgets:
  if (modal_ && !modal_->contains(to)) to = modal_;

  bool ret = to && to->send(event);
  if (!ret) {
    // try the chain of global event handlers:
    for (const handler_link *h = handlers; h; h = h->next)
      if (h->handle(event, window)) {ret = true; break;}
  }
  dnd_flag = false;

  if (event == RELEASE && !pushed_) {
    // send a dummy move event when the user releases the mouse:
    if (xmousewin) handle(MOVE, xmousewin);
    else belowmouse(0);
    // Don't pop up the tooltip for whatever they are pointing at:
    Tooltip::current(belowmouse_);
  }

  return ret;
}

//
// End of "$Id: Fl.cxx,v 1.158 2002/12/15 10:42:53 spitzak Exp $".
//
