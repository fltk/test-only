//
// "$Id: Fl.cxx,v 1.123 2001/03/08 23:49:29 robertk Exp $"
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
		*Fl::focus_;
int		(*Fl::grab_)(int, void*);
Fl_Window       *Fl::grabbed_ = 0;
static void* grab_data;
Fl_Window	*Fl::modal_;	// topmost modal() window
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

const char *Fl_Shared_Image::fl_shared_image_root=0;

Fl_Shared_Image  *Fl_Shared_Image::first_image = 0;

int Fl_Shared_Image::image_used=0;
size_t Fl_Shared_Image::mem_usage_limit=0;

size_t Fl_Shared_Image::mem_used=0;
int Fl_Shared_Image::forbid_delete = 1;

static Fl_Window *xfocus;	// which window X thinks has focus
static Fl_Window *xmousewin;// which window X thinks has FL_ENTER

void fl_fix_focus();

static void nothing(Fl_Widget *) {}
static void nothing2(Fl_Widget *, int, int, int, int, const char *) {}

FL_API void (*Fl_Tooltip::enter)(Fl_Widget *) = nothing;
FL_API void (*Fl_Tooltip::enter_area)(Fl_Widget *, int, int, int, int, const char *) = nothing2;
FL_API void (*Fl_Tooltip::exit)(Fl_Widget *) = nothing;

#ifdef WIN32
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

#ifndef WIN32
#include <sys/time.h>
#endif

// I avoid the overhead of getting the current time when we have no
// timeouts by setting this flag instead of getting the time.
// In this case calling elapse_timeouts() does nothing, but records
// the current time, and the next call will actualy elapse time.
static char reset_clock = 1;

static void elapse_timeouts() {
#ifdef WIN32
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

int Fl::has_timeout(Fl_Timeout_Handler cb, void *arg) {
  for (Timeout* t = first_timeout; t; t = t->next)
    if (t->cb == cb && t->arg == arg) return 1;
  return 0;
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

extern int fl_wait(double time); // in Fl_x.cxx or Fl_win32.cxx

static char in_idle;

int Fl::wait(double time_to_wait) {
  if (first_timeout) {
    elapse_timeouts();
    Timeout *t;
    while ((t = first_timeout)) {
      if (t->time > 0) break;
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
    }
  } else {
    reset_clock = 1; // we are not going to check the clock
  }
  // checks are a bit messy so that add/remove and wait may be called
  // from inside them without causing an infinite loop:
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
    if (idle) time_to_wait = 0.0;
  }
  if (first_timeout && first_timeout->time < time_to_wait)
    time_to_wait = first_timeout->time;
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
	while(Fl_X::first)
		wait(0);
	return(0);
//  for (;;) wait(FOREVER);
}

extern int fl_ready();

int Fl::ready() {
  if (first_timeout) {
    elapse_timeouts();
    if (first_timeout->time <= 0) return 1;
  } else {
    reset_clock = 1;
  }
  return fl_ready();
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
  for (Fl_X* x = Fl_X::first;; x = x->next) {
    if (!x) return 0;
    Fl_Window* w = x->w;
    if (w->visible() && !w->parent()) return w;
  }
}

Fl_Window* Fl::next_window(const Fl_Window* w) {
  for (Fl_X* x = Fl_X::i(w)->next;; x = x->next) {
    if (!x) return 0;
    Fl_Window* w = x->w;
    if (w->visible() && !w->parent()) return w;
  }
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
#ifdef WIN32
  GdiFlush();
#else
  if (fl_display) XFlush(fl_display);
#endif
}

////////////////////////////////////////////////////////////////
// Event handling:

int Fl::event_inside(int x,int y,int w,int h) /*const*/ {
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
  if (!dnd_flag) Fl_Tooltip::enter(o);
  Fl_Widget *p = belowmouse_;
  if (o != p) {
    belowmouse_ = o;
    for (; p && !p->contains(o); p = p->parent())
      p->handle(dnd_flag ? FL_DND_LEAVE : FL_LEAVE);
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

  if (Fl::grab() || Fl::grabbed()) return; // don't do anything while grab is on.

  Fl_Widget* w = xfocus;

  if (w) {
    Fl::e_keysym = 0; // make sure widgets don't think a keystroke moved focus
    // System says this program has focus.
    // Modal overrides anything the system says:
    if (Fl::modal()) w = Fl::modal();
    // Move to outermost window (WAS: system code should do this)
    // while (w->parent()) w = w->parent();
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

////////////////////////////////////////////////////////////////

// local_grab() causes all windows to be frozen (as though a modal window
// was up) and all events are passed to the provided grab function.
// Use release() to undo this. You can also temporarily undo it by
// setting the function to zero.

void Fl::local_grab(int (*cb)(int, void*), void* user_data) {
  grab_ = cb;
  grab_data = user_data;
}

// grab() does the same thing but also messes with the window system
// in an attempt to get events from the entire screen. This is used
// when menus are up so they can be dismissed with a click anywhere.
// Use release() to undo this.

// On X this is dangerous if your program goes into an infinite loop
// because the server will be locked up!

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
  Window w = fl_xid(first_window());
  XGrabPointer(fl_display,
	       w,
	       1,
	       ButtonPressMask|ButtonReleaseMask|
	       ButtonMotionMask|PointerMotionMask,
	       GrabModeAsync,
	       GrabModeAsync,
	       None,
	       0,
	       fl_event_time);
  XGrabKeyboard(fl_display,
		w,
		1,
		GrabModeAsync,
		GrabModeAsync,
		fl_event_time);
#endif
}

void Fl::release() {
  grab_ = 0;
  grabbed_ = 0;
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

// Returns true if the current key event can be altered to a similar
// key event that the user may have intended as a shortcut. This will
// flip the case for shortcut letter matches, and substitute function
// keys for Emacs control characters:
static bool alternate_key() {
  // try flipping the case of letter shortcuts:
  if (isalpha(Fl::e_text[0])) {Fl::e_text[0] ^= 0x20; return true;}
  // try translating Emacs control keys:
#define ctrl(x) (x^0x40)
  int key = 0;
  switch (Fl::e_text[0]) {
  case ctrl('A') : key = FL_Home; goto K;
  case ctrl('B') : key = FL_Left; goto K;
  case ctrl('D') : key = FL_Delete; goto K;
  case ctrl('E') : key = FL_End; goto K;
  case ctrl('F') : key = FL_Right; goto K;
  case ctrl('H') : key = FL_BackSpace; goto K;
  case ctrl('K') : key = FL_Clear; goto K;
  case ctrl('N') : key = FL_Down; goto K;
  case ctrl('P') : key = FL_Up; goto K;
  K: Fl::e_keysym = key; Fl::e_state &= ~FL_CTRL; Fl::e_text[0] = 0;
  return true;
  }
  return false;
}

int Fl::handle(int event, Fl_Window* window)
{
  e_type = event;
  if (grabbed_) window = grabbed_;
  if (grab_) {
    if (grab_(event, grab_data)) return true;
    if (event == FL_KEY && alternate_key()) return grab_(event, grab_data);
    return false;
  }

  Fl_Widget* to = window;

  switch (event) {

  case FL_PUSH:
    if (!pushed()) {
      Fl_Tooltip::enter((Fl_Widget*)0);
      if (modal() && window != modal()) return false;
      pushed_ = window;
    }
    to = pushed();
    break;

  case FL_DND_ENTER:
  case FL_DND_DRAG:
    if (modal() && window != modal()) return true;
    dnd_flag = 1;
    break;

  case FL_ENTER:
  case FL_MOVE:
//case FL_DRAG: // does not happen
    xmousewin = window; // this should already be set, but just in case.
    if (pushed()) {to = pushed_; event = FL_DRAG;}
    else if (modal() && window != modal()) return true;
    break;

  case FL_DND_LEAVE:
    dnd_flag = 1;
    belowmouse(0);
    dnd_flag = 0;
    return true;

  case FL_LEAVE:
    if (!pushed_) belowmouse(0);
    return true;

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
    break;

  default:
    if (modal()) to = modal();
    break;

  case 0: // events from system that fltk does not understand
    to = 0;
    break;
  }

  int ret = false;
  if (to && window) {
    int wx = 0, wy = 0;
    for (Fl_Widget *w = to; w; w = w->parent())
      { wx += w->x(); wy += w->y(); }
    int save_x = Fl::e_x; int save_y = Fl::e_y;
    Fl::e_x = Fl::e_x_root-wx;Fl::e_y = Fl::e_y_root-wy;
    ret = to->handle(event);
    Fl::e_x = save_x; Fl::e_y = save_y;
  }

  if (!ret) {
    switch (event) {
    case FL_KEY:
      // if keyboard is ignored, try shortcut events:
      if (handle(FL_SHORTCUT, window)) return true;
      if (alternate_key())
	return handle(e_text[0] ? FL_SHORTCUT : FL_KEY, window);
      return false;

      // rejected mouse events produce FL_LEAVE events:
    case FL_DND_ENTER:
    case FL_DND_DRAG:
    case FL_ENTER:
    case FL_MOVE:
      belowmouse(window);
      break;
    }

    // try the chain of global event handlers:
    for (const handler_link *h = handlers; h; h = h->next)
      if (h->handle(event)) {ret = true; break;}
  }
  dnd_flag = 0;

  if (event == FL_RELEASE && !pushed_ && xmousewin) {
    // send a dummy move event when the user releases the mouse:
    handle(FL_MOVE, xmousewin);
  }

  return ret;
}

//
// End of "$Id: Fl.cxx,v 1.123 2001/03/08 23:49:29 robertk Exp $".
//
