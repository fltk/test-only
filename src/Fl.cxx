//
// "$Id: Fl.cxx,v 1.144 2002/05/07 04:58:19 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/x.h>
#include <fltk/Fl_Style.h>
#include <fltk/Fl_Tooltip.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>

double Fl::version() {return FL_VERSION;}

//
// Globals...
//

Fl_Widget	*Fl::belowmouse_,
		*Fl::pushed_,
		*Fl::focus_,
		*Fl::modal_;
int		Fl::damage_,
		Fl::e_type,
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
bool		Fl::grab_,
		Fl::exit_modal_;

static Fl_Window *xfocus;	// which window X thinks has focus
static Fl_Window *xmousewin;// which window X thinks has FL_ENTER

void fl_fix_focus();

static void nothing(Fl_Widget *) {}
FL_API void (*Fl_Tooltip::enter)(Fl_Widget *) = nothing;
FL_API void (*Fl_Tooltip::exit)(Fl_Widget *) = nothing;

#ifdef _WIN32
#include "Fl_win32.cxx"
#else
#include "Fl_x.cxx"
#endif

////////////////////////////////////////////////////////////////
// Timeouts are stored in a sorted list, so only the first one needs
// to be checked to see if any should be called.

struct Timeout {
  double time;
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
  double elapsed = (newclock-prevclock)/1000.0;
  prevclock = newclock;
#else
  static struct timeval prevclock;
  struct timeval newclock;
  gettimeofday(&newclock, NULL);
  double elapsed = newclock.tv_sec - prevclock.tv_sec +
    (newclock.tv_usec - prevclock.tv_usec)/1000000.0;
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
static double missed_timeout_by;

void Fl::add_timeout(double time, Fl_Timeout_Handler cb, void *arg) {
  elapse_timeouts();
  repeat_timeout(time, cb, arg);
}

void Fl::repeat_timeout(double time, Fl_Timeout_Handler cb, void *arg) {
  time += missed_timeout_by; if (time < -.05) time = 0;
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

bool Fl::has_timeout(Fl_Timeout_Handler cb, void *arg) {
  for (Timeout* t = first_timeout; t; t = t->next)
    if (t->cb == cb && t->arg == arg) return true;
  return false;
}

void Fl::remove_timeout(Fl_Timeout_Handler cb, void *arg) {
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
// call to Fl::wait().

struct Check {
  void (*cb)(void*);
  void* arg;
  Check* next;
};
static Check* first_check, *next_check, *free_check;

void Fl::add_check(Fl_Timeout_Handler cb, void *arg) {
  Check* t = free_check;
  if (t) free_check = t->next;
  else t = new Check;
  t->cb = cb;
  t->arg = arg;
  t->next = first_check;
  if (next_check == first_check) next_check = t;
  first_check = t;
}

void Fl::remove_check(Fl_Timeout_Handler cb, void *arg) {
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

void (*Fl::idle)(); // see Fl_add_idle.cxx for the add/remove functions

static char in_idle;

#define FOREVER 1e20

int Fl::run() {
  while (first_window()) wait(FOREVER);
  return(0);
// WAS: This was tried for fltk 2.0, and the callback for closing the last
// window in Fl_Window.C called exit(). This proved to be unpopular:
//  for (;;) wait(FOREVER);
}

int Fl::wait() {
  return wait(FOREVER);
}

int Fl::wait(double time_to_wait) {
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
    if (!in_idle) {in_idle = 1; idle(); in_idle = 0;}
    // the idle function may turn off idle, we can then wait:
    if (idle && time_to_wait > 0) {time_to_wait = 0.0; ret = 1;}
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
      time_to_wait = 0.0; ret = 1;
    }
  } else {
    reset_clock = 1; // remember that elapse_timeouts was not called
  }
  // update display after the callbacks but before waiting:
  flush();
  // run the system-specific part that waits for sockets & events:
  if (time_to_wait <= 0.0) time_to_wait = 0.0;
  if (fl_wait(time_to_wait)) ret = 1;
  return ret;
}

int Fl::check() {
  return wait(0.0);
}

int Fl::ready() {
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

Fl_X* Fl_X::first;

Fl_Window* fl_find(Window xid) {
  Fl_X *x;
  for (Fl_X **pp = &Fl_X::first; (x = *pp); pp = &x->next)
    if (x->xid == xid) {
      if (x != Fl_X::first) {
	// make this window be first to speed up searches
	*pp = x->next;
	x->next = Fl_X::first;
	Fl_X::first = x;
      }
      return x->window;
    }
  return 0;
}

Fl_Window* Fl::first_window() {
  for (Fl_X* x = Fl_X::first;; x = x->next) {
    if (!x) return 0;
    Fl_Window* window = x->window;
    if (window->visible() && !window->parent() && !window->override())
      return window;
  }
}

Fl_Window* Fl::next_window(const Fl_Window* w) {
  for (Fl_X* x = Fl_X::i(w)->next;; x = x->next) {
    if (!x) return 0;
    Fl_Window* window = x->window;
    if (window->visible() && !window->parent() && !window->override())
      return window;
  }
}

void Fl::first_window(Fl_Window* window) {
  if (!window || !window->shown()) return;
  fl_find(fl_xid(window));
}

void Fl::redraw() {
  for (Fl_X* x = Fl_X::first; x; x = x->next) x->window->redraw();
}

void Fl::flush() {
  if (damage()) {
    damage_ = 0;
    for (Fl_X* x = Fl_X::first; x; x = x->next) {
      if (x->wait_for_expose) {damage_ = 1; continue;}
      Fl_Window* window = x->window;
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
#else
  if (fl_display) XFlush(fl_display);
#endif
}

////////////////////////////////////////////////////////////////
// Event handling:

bool Fl::event_inside(int x,int y,int w,int h) /*const*/ {
  int mx = e_x - x;
  int my = e_y - y;
  return (mx >= 0 && mx < w && my >= 0 && my < h);
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

static char dnd_flag = 0; // makes belowmouse send DND_LEAVE instead of LEAVE

void Fl::belowmouse(Fl_Widget *o) {
  Fl_Widget *p = belowmouse_;
  if (o != p) {
    belowmouse_ = o;
    if (!dnd_flag) Fl_Tooltip::enter(o);
    for (; p && !p->contains(o); p = p->parent())
      p->handle(dnd_flag ? FL_DND_LEAVE : FL_LEAVE);
  }
}

void Fl::pushed(Fl_Widget *o) {
  pushed_ = o;
}

// Update focus() in response to anything that might change it.
// This is called whenever a window is added or hidden, and whenever
// X says the focus window has changed.

void fl_fix_focus() {
  Fl_Widget* w = xfocus;
  // Modal overrides whatever the system says the focus is:
  if (Fl::grab_ || w && Fl::modal_) w = Fl::modal_;
  if (w) {
    if (w->contains(Fl::focus())) return; // already has it
    Fl::e_keysym = 0; // make widgets not think a keystroke moved focus
    if (w->take_focus()) return;
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
#ifndef _WIN32
  if (contains(fl_selection_requestor)) fl_selection_requestor = 0;
#endif
  if (contains(Fl::belowmouse())) {Fl::belowmouse_ = 0; Fl::e_is_click = 0;}
  if (contains(Fl::focus())) Fl::focus_ = 0;
  if (this == xfocus) xfocus = 0;
  if (this == xmousewin) xmousewin = 0;
  Fl_Tooltip::exit(this);
  if (this == Fl::modal_) {Fl::modal_ = 0; Fl::exit_modal();}
}

////////////////////////////////////////////////////////////////

void Fl::modal(Fl_Widget* widget, bool grab) {

  // release the old grab:
  if (grab_) {
    grab_ = false;
#ifdef _WIN32
    ReleaseCapture();
    // if (event() == FL_PUSH) repost_the_push_event(); NYI
#else
    XUngrabKeyboard(fl_display, fl_event_time);
    Fl::event_is_click(0); // avoid double click
    XAllowEvents(fl_display, event()==FL_PUSH ? ReplayPointer : AsyncPointer, CurrentTime);
    XUngrabPointer(fl_display, fl_event_time); // Qt did not do this...
    XFlush(fl_display); // make sure we are out of danger before continuing...
#endif
    // because we "pushed back" the FL_PUSH, make it think no buttons are down:
    e_state &= 0xffffff;
    e_keysym = 0;
  }

  // start the new grab:
  // Both X and Win32 have the annoying requirement that a visible window
  // be used as a target for the events, and it cannot disappear while the
  // grab is running. I just grab fltk's first window:
  if (grab && widget) {
#ifdef _WIN32
    Fl_Window* window = first_window();
    if (window) {
      SetActiveWindow(fl_xid(window));
      SetCapture(fl_xid(window));
      grab_ = true;
    }
#else
    Fl_Window* window = first_window();
    if (window &&
	XGrabKeyboard(fl_display,
		      fl_xid(window),
		      true, // owner_events
		      GrabModeAsync, // pointer_mode
		      GrabModeAsync, // keyboard_mode
		      fl_event_time) == GrabSuccess) {
      //XAllowEvents(fl_display, SyncKeyboard, CurrentTime);
      if (XGrabPointer(fl_display,
		       fl_xid(window),
		       true, // owner_events
		       ButtonPressMask|ButtonReleaseMask|
		       ButtonMotionMask|PointerMotionMask,
		       GrabModeSync, // pointer_mode
		       GrabModeAsync, // keyboard_mode
		       None, // confine_to
		       None, // cursor
		       fl_event_time) == GrabSuccess) {
	grab_ = true;
	XAllowEvents(fl_display, SyncPointer, CurrentTime);
      } else {
	//printf("XGrabPointer failed\n");
	XUngrabKeyboard(fl_display, fl_event_time);
      }
    } else {
      //printf("XGrabKeyboard failed\n");
    }
#endif
  }

  if (widget != modal_) {
    modal_ = widget;
    pushed_ = belowmouse_ = 0;
    fl_fix_focus();
    // generate a dummy move event so the highlights are correct for
    // the new modal widget:
    if (xmousewin) {
      Fl::e_x = Fl::e_x_root-xmousewin->x();
      Fl::e_y = Fl::e_y_root-xmousewin->y();
      Fl::handle(FL_MOVE, xmousewin);
    }
  }

  exit_modal_ = false;
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

bool (*fl_local_grab)(int); // used by fl_dnd_x.cxx

bool Fl::handle(int event, Fl_Window* window)
{
  e_type = event;
  if (fl_local_grab) return fl_local_grab(event);

  Fl_Widget* to = window;

  switch (event) {

  case FL_PUSH:
    Fl_Tooltip::enter((Fl_Widget*)0);
    if (pushed()) to = pushed();
    break;

  case FL_ENTER:
  case FL_MOVE:
//case FL_DRAG: // does not happen
    if (pushed()) {to = pushed_; event = FL_DRAG;}
    break;

  case FL_LEAVE:
    if (!pushed_) belowmouse(0);
    return true;

  case FL_RELEASE:
    to = pushed();
    if (!event_state(FL_BUTTONS)) pushed_=0;
    break;

  case FL_DND_ENTER:
  case FL_DND_DRAG:
    dnd_flag = 1;
    break;

  case FL_DND_LEAVE:
    dnd_flag = 1;
    belowmouse(0);
    dnd_flag = 0;
    return true;

  case FL_DND_RELEASE:
    to = belowmouse();
    break;

  case FL_KEY:
    Fl_Tooltip::enter((Fl_Widget*)0);
    xfocus = window; // this should already be set, but just in case.
    // try sending keystroke to the focus:
    to = focus();
    if (modal_ && !modal_->contains(to)) to = modal_;
    if (to && to->send(event)) return true;
    // try sending a shortcut to the window:
    if (handle(FL_SHORTCUT, window)) return true;
    // Try using upper-case instead of lower-case for letter shortcuts:
    if (islower(Fl::e_text[0])) {
      Fl::e_text[0] ^= 0x20;
      return handle(FL_SHORTCUT, window);
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
      if (h->handle(event)) {ret = true; break;}
  }
  dnd_flag = 0;

  if (event == FL_RELEASE && !pushed_) {
    // send a dummy move event when the user releases the mouse:
    if (xmousewin) handle(FL_MOVE, xmousewin);
    else belowmouse(0);
  }

  return ret;
}

//
// End of "$Id: Fl.cxx,v 1.144 2002/05/07 04:58:19 spitzak Exp $".
//
