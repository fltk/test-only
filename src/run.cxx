//
// "$Id$"
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

#include <config.h>
#include <fltk/run.h>
#include <fltk/events.h>
#include <fltk/error.h>
#include <fltk/damage.h>
#include <fltk/layout.h>
#include <fltk/Window.h>
#include <fltk/Style.h>
#include <fltk/Tooltip.h>

#if defined(_WIN32) && USE_MULTIMONITOR && WINVER<0x0500
// Why??
# undef WINVER
# define WINVER 0x0500
#endif

#include <fltk/x.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/* things that should be in different source files: */
#include <fltk/FL_VERSION.h>
#include <fltk/Monitor.h>

using namespace fltk;

/*! \namespace fltk

  \brief All classes and interface functions for fltk are in the fltk:: namespace.  This is a complete list of them, with links to the pages they are described on.

  Put "using namespace fltk;" and you will not need to put the "fltk::" in
  front of everything.

  Fltk also provides some \ref utilities that are \e not in the fltk
  namespace, as they are provided to make up for missing functions
  that should be in standard libraries.

  Fltk1.1 emulation is mostly achieved by aliasing the old class name
  "Fl" to this namespace so that the static methods in Fltk1.1 will
  work, and by typedefs from the new fltk::Widget class names to the
  older Fl_Widget class names.

*/

/*!
  Returns the version number of fltk. This can be compared to the
  value of the FL_VERSION macro to see if the shared library of
  fltk your program linked with is up to date.

  The FLTK version number is stored in a number of compile-time constants: 
  - FL_MAJOR_VERSION - The major release number, currently 2.
  - FL_MINOR_VERSION - The minor release number, currently 0.
  - FL_PATCH_VERSION - The patch release number, currently 1.
  - FL_VERSION - A combined floating-point version number of the
    form M.mmpp where M is the major number, mm is the minor number,
    and pp is the patch number, currently 2.0001.
*/
float fltk::version() {return FL_VERSION;}

//
// Globals...
//

Widget *fltk::belowmouse_,
	 *fltk::pushed_,
	 *fltk::focus_,
	 *fltk::modal_;
int	  fltk::e_type,
	  fltk::e_x,
	  fltk::e_y,
	  fltk::e_dx,
	  fltk::e_dy,
	  fltk::e_x_root,
	  fltk::e_y_root,
  	  fltk::e_clicks,
	  fltk::e_device = DEVICE_MOUSE;
unsigned  fltk::e_state,
	  fltk::e_keysym,
	  fltk::e_is_click,
	  fltk::e_length;
float     fltk::e_pressure,
          fltk::e_x_tilt,
          fltk::e_y_tilt;
const char *fltk::e_text = "";
bool      fltk::grab_,
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
    unsigned saved = e_keysym;
    e_keysym = 0; // make widgets not think a keystroke moved focus
    if (w->take_focus()) {e_keysym = saved; return;}
    e_keysym = saved;
  }
  // give nothing the focus:
  focus(0);
}

extern "C" {
// This function is here because Window::label() uses it:
/**
  Equivalent to strdup() except the C++ new[] operator is used. A
  block of memory strlen(from)+1 is allocated and the \a from
  string is copied to it. Notice that you must use delete[] to
  destroy the returned value.

  If NULL is passed, a NULL is returned.

  It is a good idea to use this instead of strdup() if you want a
  replacement new-handler to work. FLTK uses this for all strings
  that it copies internally.
*/
const char* newstring(const char *from) {
  if (!from) return 0;
  unsigned n = strlen(from)+1;
  char* ret = new char[n];
  strcpy(ret, from);
  return ret;
}
}

// This function is here because Window::label() uses it:
/**
  Returns a pointer to after the last slash in \a name. If the name
  ends with a slash then this returns a pointer to the NUL. If there
  is no slash this returns a pointer to the start of \a name.
*/
const char *filename_name(const char *name) {
  const char *p,*q;
  q = name;
#ifdef _WIN32
  if (q[0] && q[1]==':') q += 2; // skip leading drive letter
  for (p = q; *p; p++) if (*p == '/' || *p == '\\') q = p+1;
#else
  for (p = q; *p;) if (*p++ == '/') q = p;
#endif
  return q;
}

#if USE_X11
# include "x11/run.cxx"
#elif defined(_WIN32)
# include "win32/run.cxx"
#elif USE_QUARTZ
# include "osx/run.cxx"
#else
# error
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

/*!
  Add a one-shot timeout callback. The function will be called by
  fltk::wait() at t seconds after this function is called. The
  optional void* argument is passed to the callback.
*/
void fltk::add_timeout(float time, TimeoutHandler cb, void *arg) {
  elapse_timeouts();
  repeat_timeout(time, cb, arg);
}

/*!

  Inside a timeout callback you can call this to add another
  timeout. Rather than the time being measured from "now", it is
  measured from when the system call elapsed that caused this timeout
  to be called. This will result in far more accurate spacing of the
  timeout callbacks, it also has slightly less system call
  overhead. (It will also use all your machine time if your timeout
  code and fltk's overhead take more than t seconds, as the real
  timeout will be reduced to zero).

  Outside a timeout callback this acts like add_timeout().

  This code will print "TICK" each second on stdout, with a fair
  degree of accuracy:
\code
void callback(void*) {
  printf("TICK\n");
  fltk::repeat_timeout(1.0,callback);
}

main() {
  fltk::add_timeout(1.0,callback);
  for (;;) fltk::wait();
}
\endcode 
*/
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

/*!
 Returns true if the timeout exists and has not been called yet.
*/
bool fltk::has_timeout(TimeoutHandler cb, void *arg) {
  for (Timeout* t = first_timeout; t; t = t->next)
    if (t->cb == cb && t->arg == arg) return true;
  return false;
}

/*!
  Removes all pending timeout callbacks that match the function and arg.
  Does nothing if there are no matching ones that have not been
  called yet.
*/
void fltk::remove_timeout(TimeoutHandler cb, void *arg) {
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

/*!

  Fltk will call this callback just before it flushes the display and
  waits for events. This is different than add_idle() because it
  is only called once, then fltk calls the system and tells it not to
  return until an event happens. If several checks have been added fltk
  calls them all, the most recently added one first.

  This can be used by code that wants to monitor the application's
  state, such as to keep a display up to date. The advantage of using
  a check callback is that it is called only when no events are
  pending. If events are coming in quickly, whole blocks of them will
  be processed before this is called once. This can save significant
  time and avoid the application falling behind the events:
\code
bool state_changed; // anything that changes the display turns this on

void check(void*) {
  if (!state_changed) return;
  state_changed = false;
  do_expensive_calculation();
  widget->redraw();
}

main() {
  fltk::add_check(1.0,check);
  return fltk::run();
}
\endcode
*/
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

/*!
  Remove all matching check callback, if any exists. You can call this
  from inside the check callback if you want.
*/
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

/*!
  Return true if add_check() has been done with this \a cb and \a arg,
  and remove_check() has not been done.
*/
bool fltk::has_check(TimeoutHandler cb, void *arg) {
  for (Check* t = first_check; t; t = t->next)
    if (t->cb == cb && t->arg == arg) return true;
  return false;
}

////////////////////////////////////////////////////////////////
// wait/run/check/ready:

void (*fltk::idle)(); // see add_idle.cxx for the add/remove functions

static bool in_idle;

#define FOREVER 1e20f

/*!
  Calls fltk::wait() as long as any windows are not closed. When
  all the windows are hidden or destroyed (checked by seeing if
  Window::first() is null) this will return with zero. A program can
  also exit by having a callback call exit() or abort().

  Most fltk programs will end main() with return fltk::run();.
*/
int fltk::run() {
  while (Window::first()) wait(FOREVER);
  return(0);
// WAS: This was tried for fltk 2.0, and the callback for closing the last
// window in Window.C called exit(). This proved to be unpopular:
//  for (;;) wait(FOREVER);
}

/*!
  Same as fltk::wait(infinity). Call this repeatedly to "run" your
  program. You can also check what happened each time after this
  returns, which is quite useful for managing program state.
*/
int fltk::wait() {
  return wait(FOREVER);
}

static void run_checks() {
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
}

/*!
  Waits until "something happens", or the given time interval
  passes. It can return much sooner than the time if something
  happens.

  What this really does is call all idle callbacks, all elapsed
  timeouts, call fltk::flush() to get the screen to update, and then
  wait some time (zero if there are idle callbacks, the shortest of
  all pending timeouts, or the given time), for any events from the
  user or any fltk::add_fd() callbacks. It then handles the events and
  calls the callbacks and then returns.

  The return value is zero if nothing happened before the passed
  \a time_to_wait expired. It is non-zero if any events or timeouts
  came in.
*/
int fltk::wait(float time_to_wait) {

  // check functions must be run first so they can install idle or timeout
  // functions:
  run_checks();
  flush();

  if (first_timeout) {
    elapse_timeouts();
    Timeout* t = first_timeout;
    if (t->time < time_to_wait) time_to_wait = t->time;
  }

  // run the system-specific part that waits for sockets & events:
  if (time_to_wait <= 0 || idle && !in_idle) time_to_wait = 0;
  int ret = fl_wait(time_to_wait);

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
      // return true because something was done:
      ret = 1;
    }
  } else {
    reset_clock = 1; // remember that elapse_timeouts was not called
  }

  if (idle && !in_idle) {in_idle = true; idle(); in_idle = false;}

  flush();

  return ret;
}

/*!
  Same as fltk::wait(0). Calling this during a big calculation will
  keep the screen up to date and the interface responsive:
\code
while (!calculation_done()) {
  calculate();
  fltk::check();
  if (user_hit_abort_button()) break;
}
\endcode 
*/
int fltk::check() {
  return wait(0.0);
}

/*!
  Test to see if any events or callbacks are pending. This will
  return true if fltk::check() would do anything other than update
  the screen. Since this will not draw anything or call any code,
  it is safe to call this if your program is in an inconsistent
  state. This is also useful if your calculation is updating widgets but
  you do not want or need the overhead of the screen updating every time
  you check for events.
\code
while (!calculation_done()) {
  calculate();
  if (fltk::ready()) {
    do_expensive_cleanup();
    fltk::check();
    if (user_hit_abort_button()) break;
  }
}
\endcode 
*/
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

#if USE_X11
Window* fltk::find(XWindow xid)
#elif defined(_WIN32)
Window* fltk::find(HWND xid)
#elif USE_QUARTZ
Window* fltk::find(WindowPtr xid)
#endif
{
  CreatedWindow *x;
  for (CreatedWindow **pp = &CreatedWindow::first; (x = *pp); pp = &x->next)
#if USE_X11
    if (x->xid == xid || x->frontbuffer==xid)
#else
    if (x->xid == xid)
#endif
      {if (x != CreatedWindow::first) {
	// make this window be first to speed up searches
	*pp = x->next;
	x->next = CreatedWindow::first;
	CreatedWindow::first = x;
      }
      return x->window;
    }
  return 0;
}

/*!
  Returns the id of some visible() window. If there is more than
  one, the last one to receive an event is returned. This is useful
  as a default value for fltk::Window::child_of().
  fltk::Window::exec() uses it for this if no other parent is specified.
  This is also used by fltk::run() to see if any windows still exist.
*/
Window* Window::first() {
  for (CreatedWindow* x = CreatedWindow::first;; x = x->next) {
    if (!x) return 0;
    Window* window = x->window;
    if (window->visible() && !window->parent() && !window->override())
      return window;
  }
}

/*!
  Returns the next visible() top-level window, returns NULL after
  the last one. You can use this and first() to iterate through
  all the visible windows.
*/
Window* Window::next() {
  for (CreatedWindow* x = CreatedWindow::find(this)->next;; x = x->next) {
    if (!x) return 0;
    Window* window = x->window;
    if (window->visible() && !window->parent() && !window->override())
      return window;
  }
}

/*!
  If this window is visible, this removes it from wherever it is in
  the list and inserts it at the top, as though it received an
  event. This can be used to change the parent of dialog boxes run by
  fltk::Window::exec() or fltk::ask().
*/
void Window::first(Window* window) {
  if (!window || !window->shown()) return;
  fltk::find(xid(window));
}

int fltk::damage_;

/*! \fn int fltk::damage()

  True if any Widget::redraw() calls have been done since the
  last fltk::flush(). This indicates that flush() will do something.
  Currently the meaning of any bits are undefined.

  Window flush() routines can set this to indicate that flush() should
  be called again after waiting for more events. This is useful in
  some instances such as X windows that are waiting for a mapping
  event before being drawn.
*/

/*!
  Redraws all widgets. This is a good idea if you have made global
  changes to the styles.
*/
void fltk::redraw() {
  for (CreatedWindow* x = CreatedWindow::first; x; x = x->next)
    x->window->redraw();
}

#if !USE_X11 && defined(_WIN32)
extern void fl_do_deferred_calls(); // in Fl_Window.cxx:
#endif

/*!
  Get the display up to date. This is done by calling layout() on
  all Window objects with layout_damage() and then calling draw()
  on all Window objects with damage(). (actually it calls Window::flush()
  and that calls draw(), but normally you can ignore this). This
  will also flush the X i/o buffer, update the cursor shape, update
  Windows window sizes, and other operations to get the display
  up to date.

  wait() calls this before it waits for events.
*/
void fltk::flush() {
#if USE_X11
  if (!xdisplay) return; // ignore if no windows created yet
#elif defined(_WIN32)
  fl_do_deferred_calls();
#endif
  if (damage_) {
    damage_ = false; // turn it off so Window::flush() can turn it back on
    for (CreatedWindow* x = CreatedWindow::first; x; x = x->next) {
      if (x->wait_for_expose) {damage_ = true; continue;}
      Window* window = x->window;
      if (window->visible_r() && window->w()>0 && window->h()>0) {
	if (window->layout_damage()) window->layout();
	if (window->damage() || x->region) {
	  window->flush();
	  window->set_damage(0);
	}
      }
      // destroy damage regions for windows that don't use them:
      if (x->region) {
#if USE_X11
	XDestroyRegion(x->region);
#elif defined(_WIN32)
	DeleteObject(x->region);
#elif USE_QUARTZ
	DisposeRgn(x->region);
#endif
	x->region = 0;
      }
    }
  }
#if USE_X11
  if (xmousewin && !pushed() && !grab()) {
    CreatedWindow* i = CreatedWindow::find(xmousewin);
    if (i->cursor != None && !i->cursor_for->contains(belowmouse_)) {
      i->cursor = None;
      XDefineCursor(xdisplay, i->xid, None);
    }
  }
  XFlush(xdisplay);
#elif defined(_WIN32)
  GdiFlush();
#elif USE_QUARTZ
  //+++ QDFlushPortBuffer( GetWindowPort(xid), 0 ); // \todo do we need this?
#endif
}

////////////////////////////////////////////////////////////////

/*! \class fltk::Rectangle
  Describes an integer-sized rectangle. This is the base class of
  Widget, and also used a lot to pass rectangular areas to drawing
  functions. Almost all the functions are inline.

  Negative w() or h() is supposed to mean an empty and thus
  invisible rectangle, but some code will treat the rectangle as
  reflected about x or y. Set the size to zero to make sure you
  have an empty one.
*/

/*! \fn Rectangle::Rectangle()
  The default constructor does not put anything into the fields!
  You can either call set() or just modify the x_, y_, w_, and h_
  variables directly.
*/

/** Initialize to the size w,h. The rectangle is placed inside the
    source rectangle \a r either centered or against an edge depending
    on the ALIGN values in \a flags. For centered alignment if the
    difference in sizes is odd, it always rounds up and left.
    Default value for \a flags is to center in both directions.
 */
fltk::Rectangle::Rectangle(const fltk::Rectangle& r, int w, int h, int flags) {
  if (flags & ALIGN_LEFT) {
    if (flags & ALIGN_RIGHT &&  w > r.w()) x_ = r.r()-w;
    else x_ = r.x();
  } else if (flags & ALIGN_RIGHT) {
    x_ = r.r()-w;
  } else {
    x_ = r.x()+((r.w()-w)>>1);
  }
  if (flags & ALIGN_TOP) {
    if (flags & ALIGN_BOTTOM && h > r.h()) y_ = r.b()-h;
    else y_ = r.y();
  } else if (flags & ALIGN_BOTTOM) {
    y_ = r.b()-h;
  } else {
    y_ = r.y()+((r.h()-h)>>1);
  }
  w_ = w;
  h_ = h;
}

#if 0 // I commented these out because nothing seems to be calling them
void Rectangle::intersect(const fltk::Rectangle& R) {
  if (R.x() > x()) set_x(R.x());
  if (R.r() < r()) set_r(R.r());
  if (R.y() > y()) set_y(R.y());
  if (R.b() < b()) set_b(R.b());
}

void Rectangle::merge(const fltk::Rectangle& R) {
  if (R.empty()) return;
  if (empty()) {*this = R; return;}
  if (R.x() < x()) set_x(R.x());
  if (R.r() > r()) set_r(R.r());
  if (R.y() < y()) set_y(R.y());
  if (R.b() > b()) set_b(R.b());
}
#endif

////////////////////////////////////////////////////////////////
// Event handling:

/*!
  Returns true if the current fltk::event_x() and fltk::event_y()
  put it inside the Rectangle. You should always call this rather
  than doing your own comparison so you are consistent about edge
  effects.
*/
bool fltk::event_inside(const fltk::Rectangle& r) {
  return r.contains(e_x, e_y);
}

/*! \fn Widget* fltk::focus()
  Returns the widgets that will receive fltk::KEY events. This is NULL
  if the application does not have focus now, or if no widgets
  accepted focus.
*/
/*!
  Change fltk::focus() to the given widget, the previous widget and
  all parents (that don't contain the new widget) are sent
  fltk::UNFOCUS events, the new widget is sent an fltk::FOCUS
  event, and all parents of it get fltk::FOCUS_CHANGE events.

  fltk::focus() is set whether or not the applicaton has the focus or
  if the widgets accept the focus. You may want to use
  fltk::Widget::take_focus() instead, it will test first.
*/
void fltk::focus(Widget *o) {
  Widget *p = focus_;
  if (o != p) {
    compose_reset();
    focus_ = o;
    for (; p && !p->contains(o); p = p->parent()) p->handle(UNFOCUS);
    if (o) {
      unsigned saved = e_keysym;
      e_keysym = 0; // make widgets not think a keystroke moved focus
      o->handle(FOCUS);
      for (; (o = o->parent()); ) o->handle(FOCUS_CHANGE);
      e_keysym = saved;
    }
  }
}

static bool dnd_flag; // makes belowmouse send DND_LEAVE instead of LEAVE

/*! \fn Widget* fltk::belowmouse()
  Get the widget that is below the mouse. This is the last widget to
  respond to an fltk::ENTER event as long as the mouse is still
  pointing at it. This is for highlighting buttons and bringing up
  tooltips. It is not used to send fltk::PUSH or fltk::MOVE directly,
  for several obscure reasons, but those events typically go to this
  widget.
*/

/*!
  Change the fltk::belowmouse() widget, the previous one and all
  parents (that don't contain the new widget) are sent fltk::LEAVE
  events. Changing this does not send fltk::ENTER to this or any
  widget, because sending fltk::ENTER is supposed to test if the
  widget wants the mouse (by it returning non-zero from handle()).
*/
void fltk::belowmouse(Widget *o) {
  Widget *p = belowmouse_;
  if (o != p) {
    belowmouse_ = o;
    for (; p && !p->contains(o); p = p->parent())
      p->handle(dnd_flag ? DND_LEAVE : LEAVE);
  }
}

/*! \fn Widget* fltk::pushed()
  Get the widget that is being pushed. fltk::DRAG or fltk::RELEASE
  (and any more fltk::PUSH) events will be sent to this widget. This
  is null if no mouse button is being held down, or if no widget
  responded to the fltk::PUSH event.
*/
/*!
  Change the fltk::pushed() widget. This sends no events.
*/
void fltk::pushed(Widget *o) {
  pushed_ = o;
}

/*! This function is called by ~Widget() and by deactivate() and by
  hide(). It indicates that the widget does not want to receive any
  more events, and also removes all global variables that point at the
  widget (not just the fltk::focus(), but the fltk::belowmouse(),
  fltk::modal(), and some internal pointers). Unlike older versions of
  fltk, no events (i.e. fltk::LEAVE or fltk::UNFOCUS) are sent to the
  widget.  */
void Widget::throw_focus() {
  if (contains(fltk::pushed())) pushed_ = 0;
#if USE_X11
  if (contains(selection_requestor)) selection_requestor = 0;
#endif
  if (contains(fltk::belowmouse())) {belowmouse_ = 0; e_is_click = 0;}
  if (this == xmousewin) xmousewin = Window::first();
  if (contains(focus_)) focus_ = 0;
  if (this == xfocus) xfocus = 0;
  if (this == Tooltip::current_widget()) Tooltip::current(0);
  if (this == modal_) {modal_ = 0; exit_modal();}
}

////////////////////////////////////////////////////////////////

/*! Restricts events to a certain widget.

 First thing: much of the time fltk::Window::exec() will do what you
 want, so try using that.

 This function sets the passed widget as the "modal widget". All user
 events are directed to it or a child of it, preventing the user from
 messing with other widgets. The modal widget does not have to be
 visible or even a child of an fltk::Window for this to work (but if
 it not visible, fltk::event_x() and fltk::event_y() are meaningless,
 use fltk::event_x_root() and fltk::event_y_root()).

 The calling code is responsible for saving the current value of
 modal() and grab() and restoring them by calling this after it is
 done. The code calling this should then loop calling fltk::wait()
 until fltk::exit_modal_flag() is set or you otherwise decide to get
 out of the modal state. It is the calling code's responsibility to
 monitor this flag and restore the modal widget to it's previous value
 when it turns on.

 \a grab indicates that the modal widget should get events from
 anywhere on the screen. This is done by messing with the window
 system. If fltk::exit_modal() is called in response to an fltk::PUSH
 event (rather than waiting for the drag or release event) fltk will
 "repost" the event so that it is handled after modal state is
 exited. This may also be done for keystrokes in the future. On both X
 and WIN32 grab will not work unless you have some visible window
 because the system interface needs a visible window id. On X be
 careful that your program does not enter an infinite loop while
 grab() is on, it will lock up your screen!
*/
void fltk::modal(Widget* widget, bool grab) {

  // release the old grab:
  if (grab_) {
    grab_ = false;
#if USE_X11
    XUngrabKeyboard(xdisplay, event_time);
    event_is_click(0); // avoid double click
    XAllowEvents(xdisplay, event()==PUSH ? ReplayPointer : AsyncPointer, CurrentTime);
    XUngrabPointer(xdisplay, event_time); // Qt did not do this...
    XFlush(xdisplay); // make sure we are out of danger before continuing...
#elif defined(_WIN32)
    ReleaseCapture();
    // if (event() == PUSH) repost_the_push_event(); NYI
#elif USE_QUARTZ
    // dunno what to do here
#else
# error
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
#if USE_X11
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
#elif defined(_WIN32)
    Window* window = Window::first();
    if (window) {
      SetActiveWindow(xid(window));
      SetCapture(xid(window));
      grab_ = true;
    }
#elif USE_QUARTZ
    // dunno what to do here
#else
# error
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

/*! \fn Widget* fltk::modal()
  Returns the current modal widget, or null if there isn't one.  It is
  useful to test these in timeouts and file descriptor callbacks in
  order to block actions that should not happen while the modal window
  is up. You also need these in order to save and restore the modal
  state.
*/

/*! \fn bool fltk::grab()
  returns the current value of grab (this is always false if modal()
  is null).
*/

/*! \fn void fltk::exit_modal()
  Turns on exit_modal_flag(). This may be used by user callbacks to
  cancel modal state.
*/

/*! \fn bool fltk::exit_modal_flag()
  True if exit_modal() has been called. The flag is also set by the
  destruction or hiding of the modal widget, and on Windows by other
  applications taking the focus when grab is on.
*/

////////////////////////////////////////////////////////////////

struct handler_link {
  int (*handle)(int, Window*);
  const handler_link *next;
};

static const handler_link *handlers = 0;

/*!
  Install a function to parse unrecognized events. If FLTK cannot
  figure out what to do with an event, it calls each of these
  functions (most recent first) until one of them returns non-zero. If
  none of them returns non zero then the event is ignored.

  Currently this is called for these reasons:
  - If there is a keystroke that no widgets are interested in, this is
    called with fltk::SHORTCUT. You can use this to implement global
    hotkeys.
  - Unrecognized X events cause this to be called with NO_EVENT. The
    Window parameter is set if fltk can figure out the target window
    and it is an fltk one. You can then use system specific code to
    access the event data and figure out what to do. This is not done
    on Windows due to the \e enormous number of garbage messages a
    program gets, you should instead use Windows pre-filtering
    functions to look at these.
  - Events it gets with a window id it does not recognize cause this
    to be called, with the Window parameter set to null. This can only
    happen on X, on Windows any unknown windows should have their
    own message handling function that does not call fltk.
  - This may be called with other event types when the widget fltk
    wants to send it to returns zero. However the exact rules when
    this happens may change in future versions.
*/
void fltk::add_event_handler(int (*h)(int, Window*)) {
  handler_link *l = new handler_link;
  l->handle = h;
  l->next = handlers;
  handlers = l;
}

bool (*fl_local_grab)(int); // used by fl_dnd_x.cxx

// Set event_x() and event_y() correctly for target widget and send event:
static bool send_from_root(Widget* widget, int event) {
  e_x = e_x_root;
  e_y = e_y_root;
  for (Widget *t= widget->parent(); t; t = t->parent()) {
    e_x -= t->x(); e_y -= t->y();
  }
  return widget->send(event) != 0;
}

// Similar to !modal->contains(b) but it also follows the child_of
// pointers to windows. Possibly this is what contains() should do always.
static bool outside_modal(const Widget* b) {
  if (!modal()) return false;
  if (!b) return true;
  for (;;) {
    if (b == modal()) return false;
    const Widget* c = b->parent();
    if (!c) {
      if (b->is_window() && (c = ((Window*)b)->child_of())) ;
      else return true;
    }
    b = c;
  }
}


extern int fl_pushed_dx, fl_pushed_dy;
Window* fl_actual_window;

/*!
  Try sending the current KEY event as a SHORTCUT event.

  Normally the focus() gets all keystrokes, and shortcuts are only
  tested if that widget indicates it is uninterested by returning zero
  from Widget::handle().  However in some cases the focus wants to use
  the keystroke <i>only if it is not a shortcut</i>.  The most common
  example is Emacs-style editing keystrokes in text editing widgets,
  which conflict with Microsoft-compatable menu key bindings, but we
  want the editing keys to work if there is no conflict.

  This will send a SHORTCUT event just like the focus returned zero,
  to every widget in the focus window, and to the add_handler() calls,
  if any. It will return true if any widgets were found that were
  interested in it. A handle() method can call this in a KEY event. If
  it returns true, return 1 \e immediatly, as the shortcut will have
  executed and may very well have destroyed your widget. If this
  returns false, then do what you want the key to do.

*/
bool fltk::try_shortcut() {
  static bool recursion;
  if (recursion) return false;
  recursion = true;
  bool ret = handle(SHORTCUT, fl_actual_window) != 0;
  recursion = false;
  return ret;
}

/*! This is the function called from the system-specific code for all
  events that can be passed to Widget::handle().

  You can call it directly to fake events happening to your
  widgets. Currently data other than the event number can only be
  faked by writing to the undocumented fltk::e_* variables, for
  instance to make event_x() return 5, you should do fltk::e_x =
  5. This may change in future versions.

  This will redirect events to the modal(), pushed(), belowmouse(), or
  focus() widget depending on those settings and the event type. It
  will turn MOVE into DRAG if any buttons are down. If the resulting
  widget returns 0 (or the window or widget is null) then the functions
  pointed to by add_event_handler() are called.
*/
bool fltk::handle(int event, Window* window)
{
  e_type = event;
  if (fl_local_grab) return fl_local_grab(event);

  Widget* to = fl_actual_window = window;

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
  case DRAG: // does not happen from system code, but user code may send this
    if (pushed()) {
      e_x = e_x_root+fl_pushed_dx;
      e_y = e_y_root+fl_pushed_dy;
      return pushed()->handle(DRAG) != 0;
    }
    {Widget* pbm = belowmouse();
    if (outside_modal(to)) to = modal_;
    bool ret = to && send_from_root(to,MOVE);
    if (pbm != belowmouse()) Tooltip::enter(belowmouse());
    return ret;}

  case RELEASE:
    to = pushed();
    if (!event_state(ANY_BUTTON)) pushed_=0;
    if (to) {
      e_x = e_x_root+fl_pushed_dx;
      e_y = e_y_root+fl_pushed_dy;
      return to->handle(RELEASE) != 0;
    }
    break;

  case LEAVE:
    if (!pushed_) {belowmouse(0); Tooltip::exit();}
    return true;

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
    // Send to focus widget, then each parent so that keyboard navigation
    // can be done:
    to = focus();
    if (outside_modal(to)) to = modal_;
    while (to) {
      if (send_from_root(to,event)) return true;
      to = to->parent();
    }
    // If nothing wanted the keystroke, try sending shortcut event to
    // every widget in the window:
    event = SHORTCUT;
    to = window;
    break;

  case KEYUP:
    // Shift keys are sent to all widgets (they should use SHORTCUT for down):
    if (event_key()>=0xffe1 && event_key()<0xfff0) break;
    // others go to focus (should follow hotkeys, too...):
    to = focus();
    break;

  case 0:
    goto CALL_GLOBAL_HANDLERS;
//default: break;
  }

  // restrict to modal widgets:
  if (outside_modal(to)) to = modal_;
  if (to && send_from_root(to,event)) {dnd_flag = false; return true;}
  dnd_flag = false;

 CALL_GLOBAL_HANDLERS:
  // try the chain of global event handlers:
  for (const handler_link *h = handlers; h; h = h->next)
    if (h->handle(event, window)) return true;

  return false;
}

//
// End of "$Id$".
//
