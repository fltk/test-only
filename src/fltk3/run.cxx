//
// "$Id$"
//
// Main event handling code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//


// warning: the Apple Quartz version still uses some Quickdraw calls,
//          mostly to get around the single active context in QD and
//          to implement clipping. This should be changed into pure
//          Quartz calls in the near future.
#include <config.h>
#include <fltk3/Wrapper.h>

/* We require Windows 2000 features (e.g. VK definitions) */
#if defined(WIN32)
# if !defined(WINVER) || (WINVER < 0x0500)
#  ifdef WINVER
#   undef WINVER
#  endif
#  define WINVER 0x0500
# endif
# if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0500)
#  ifdef _WIN32_WINNT
#   undef _WIN32_WINNT
#  endif
#  define _WIN32_WINNT 0x0500
# endif
#endif

// recent versions of MinGW warn: "Please include winsock2.h before windows.h",
// hence we must include winsock2.h before FL/Fl.H (A.S. Dec. 2010, IMM May 2011)
#if defined(WIN32) && !defined(__CYGWIN__)
#  include <winsock2.h>
#endif

#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/Tooltip.h>
#include <fltk3/x.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "flstring.h"

#if defined(DEBUG) || defined(DEBUG_WATCH)
#  include <stdio.h>
#endif // DEBUG || DEBUG_WATCH

#ifdef WIN32
#  include <ole2.h>
void fl_free_fonts(void);
HBRUSH fl_brush_action(int action);
void fl_cleanup_pens(void);
void fl_release_dc(HWND,HDC);
void fl_cleanup_dc_list(void);
#elif defined(__APPLE__)
extern double fl_mac_flush_and_wait(double time_to_wait, char in_idle);
#endif // WIN32

//
// Globals...
//
#if defined(__APPLE__) || defined(FLTK3_DOXYGEN)
const char *Fl_Mac_App_Menu::about = "About ";
const char *Fl_Mac_App_Menu::print = "Print Front Window";
const char *Fl_Mac_App_Menu::services = "Services";
const char *Fl_Mac_App_Menu::hide = "Hide ";
const char *Fl_Mac_App_Menu::hide_others = "Hide Others";
const char *Fl_Mac_App_Menu::show = "Show All";
const char *Fl_Mac_App_Menu::quit = "Quit ";
#endif // __APPLE__
#ifndef FLTK3_DOXYGEN
fltk3::Widget	*fltk3::belowmouse_,
		*fltk3::pushed_,
		*fltk3::focus_,
		*fltk3::selection_owner_;
int		fltk3::damage_,
		fltk3::e_number,
		fltk3::e_x,
		fltk3::e_y,
		fltk3::e_x_root,
		fltk3::e_y_root,
		fltk3::e_dx,
		fltk3::e_dy,
		fltk3::e_clicks,
		fltk3::e_is_click,
                fltk3::e_original_keysym,
		fltk3::scrollbar_size_ = 16;
unsigned int fltk3::e_keysym;
unsigned int fltk3::e_state;


char		*fltk3::e_text = (char *)"";
int		fltk3::e_length;

fltk3::EventDispatch fltk3::e_dispatch = 0;

unsigned char   fltk3::options_[] = { 0, 0 };
unsigned char   fltk3::options_read_ = 0;


fltk3::Window *fl_xfocus;	// which window X thinks has focus
fltk3::Window *fl_xmousewin;// which window X thinks has fltk3::ENTER
fltk3::Window *fltk3::grab_;	// most recent fltk3::grab()
fltk3::Window *fltk3::modal_;	// topmost modal() window

#endif // FLTK3_DOXYGEN

//
// 'fltk3::version()' - Return the API version number...
//

double
/**
  Returns the compiled-in value of the FL_VERSION constant. This
  is useful for checking the version of a shared library.
*/
fltk3::version() {
  return FL_VERSION;
}

/**
  Gets the default scrollbar size used by
  fltk3::Browser_,
  fltk3::HelpView,
  fltk3::ScrollGroup, and
  fltk3::TextDisplay widgets.
  \returns The default size for widget scrollbars, in pixels.
*/
int fltk3::scrollbar_size() {
  return scrollbar_size_;
}

/**
  Sets the default scrollbar size that is used by the
  fltk3::Browser_,
  fltk3::HelpView,
  fltk3::ScrollGroup, and
  fltk3::TextDisplay widgets.
  \param[in] W The new default size for widget scrollbars, in pixels.
*/
void fltk3::scrollbar_size(int W) {
  scrollbar_size_ = W;
}


/** Returns whether or not the mouse event is inside the given rectangle.

    Returns non-zero if the current fltk3::event_x() and fltk3::event_y()
    put it inside the given arbitrary bounding box.

    You should always call this rather than doing your own comparison
    so you are consistent about edge effects.

    To find out, whether the event is inside a child widget of the
    current window, you can use fltk3::event_inside(const fltk3::Widget *).

    \param[in] xx,yy,ww,hh	bounding box
    \return			non-zero, if mouse event is inside
*/
int fltk3::event_inside(int xx,int yy,int ww,int hh) /*const*/ {
  int mx = e_x - xx;
  int my = e_y - yy;
  return (mx >= 0 && mx < ww && my >= 0 && my < hh);
}

/** Returns whether or not the mouse event is inside a given child widget.

    Returns non-zero if the current fltk3::event_x() and fltk3::event_y()
    put it inside the given child widget's bounding box.

    This method can only be used to check whether the mouse event is
    inside a \b child widget of the window that handles the event, and
    there must not be an intermediate subwindow (i.e. the widget must
    not be inside a subwindow of the current window). However, it is
    valid if the widget is inside a nested fltk3::Group.

    You must not use it with the window itself as the \p o argument
    in a window's handle() method.

    \note The mentioned restrictions are necessary, because this method
    does not transform coordinates of child widgets, and thus the given
    widget \p o must be within the \e same window that is handling the
    current event. Otherwise the results are undefined.

    You should always call this rather than doing your own comparison
    so you are consistent about edge effects.

    \see fltk3::event_inside(int, int, int, int)

    \param[in] o	child widget to be tested
    \return		non-zero, if mouse event is inside the widget
*/
int fltk3::event_inside(const fltk3::Widget *o) /*const*/ {
  int mx = e_x - o->x();
  int my = e_y - o->y();
  return (mx >= 0 && mx < o->w() && my >= 0 && my < o->h());
}

//
//
// timer support
//

#ifdef WIN32

// implementation in Fl_win32.cxx

#elif defined(__APPLE__)

// implementation in Fl_mac.cxx

#else

//
// X11 timers
//


////////////////////////////////////////////////////////////////////////
// Timeouts are stored in a sorted list (*first_timeout), so only the
// first one needs to be checked to see if any should be called.
// Allocated, but unused (free) Timeout structs are stored in another
// linked list (*free_timeout).

struct Timeout {
  double time;
  void (*cb)(void*);
  void* arg;
  Timeout* next;
};
static Timeout* first_timeout, *free_timeout;

#include <sys/time.h>

// I avoid the overhead of getting the current time when we have no
// timeouts by setting this flag instead of getting the time.
// In this case calling elapse_timeouts() does nothing, but records
// the current time, and the next call will actually elapse time.
static char reset_clock = 1;

static void elapse_timeouts() {
  static struct timeval prevclock;
  struct timeval newclock;
  gettimeofday(&newclock, NULL);
  double elapsed = newclock.tv_sec - prevclock.tv_sec +
    (newclock.tv_usec - prevclock.tv_usec)/1000000.0;
  prevclock.tv_sec = newclock.tv_sec;
  prevclock.tv_usec = newclock.tv_usec;
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

void fltk3::add_timeout(double time, fltk3::TimeoutHandler cb, void *argp) {
  elapse_timeouts();
  repeat_timeout(time, cb, argp);
}

void fltk3::repeat_timeout(double time, fltk3::TimeoutHandler cb, void *argp) {
  time += missed_timeout_by; if (time < -.05) time = 0;
  Timeout* t = free_timeout;
  if (t) {
      free_timeout = t->next;
  } else {
      t = new Timeout;
  }
  t->time = time;
  t->cb = cb;
  t->arg = argp;
  // insert-sort the new timeout:
  Timeout** p = &first_timeout;
  while (*p && (*p)->time <= time) p = &((*p)->next);
  t->next = *p;
  *p = t;
}

/**
  Returns true if the timeout exists and has not been called yet.
*/
int fltk3::has_timeout(fltk3::TimeoutHandler cb, void *argp) {
  for (Timeout* t = first_timeout; t; t = t->next)
    if (t->cb == cb && t->arg == argp) return 1;
  return 0;
}

/**
  Removes a timeout callback. It is harmless to remove a timeout
  callback that no longer exists.

  \note	This version removes all matching timeouts, not just the first one.
	This may change in the future.
*/
void fltk3::remove_timeout(fltk3::TimeoutHandler cb, void *argp) {
  for (Timeout** p = &first_timeout; *p;) {
    Timeout* t = *p;
    if (t->cb == cb && (t->arg == argp || !argp)) {
      *p = t->next;
      t->next = free_timeout;
      free_timeout = t;
    } else {
      p = &(t->next);
    }
  }
}

#endif

////////////////////////////////////////////////////////////////
// Checks are just stored in a list. They are called in the reverse
// order that they were added (this may change in the future).
// This is a bit messy because I want to allow checks to be added,
// removed, and have wait() called from inside them. To do this
// next_check points at the next unprocessed one for the outermost
// call to fltk3::wait().

struct Check {
  void (*cb)(void*);
  void* arg;
  Check* next;
};
static Check *first_check, *next_check, *free_check;

/**
  FLTK will call this callback just before it flushes the display and
  waits for events.  This is different than an idle callback because it
  is only called once, then FLTK calls the system and tells it not to
  return until an event happens.

  This can be used by code that wants to monitor the
  application's state, such as to keep a display up to date. The
  advantage of using a check callback is that it is called only when no
  events are pending. If events are coming in quickly, whole blocks of
  them will be processed before this is called once. This can save
  significant time and avoid the application falling behind the events.

  Sample code:

  \code
  bool state_changed; // anything that changes the display turns this on
  
  void callback(void*) {
   if (!state_changed) return;
   state_changed = false;
   do_expensive_calculation();
   widget-&gt;redraw();
  }
  
  main() {
   fltk3::add_check(callback);
   return fltk3::run();
  }
  \endcode
*/
void fltk3::add_check(fltk3::TimeoutHandler cb, void *argp) {
  Check* t = free_check;
  if (t) free_check = t->next;
  else t = new Check;
  t->cb = cb;
  t->arg = argp;
  t->next = first_check;
  if (next_check == first_check) next_check = t;
  first_check = t;
}

/**
  Removes a check callback. It is harmless to remove a check
  callback that no longer exists.
*/
void fltk3::remove_check(fltk3::TimeoutHandler cb, void *argp) {
  for (Check** p = &first_check; *p;) {
    Check* t = *p;
    if (t->cb == cb && t->arg == argp) {
      if (next_check == t) next_check = t->next;
      *p = t->next;
      t->next = free_check;
      free_check = t;
    } else {
      p = &(t->next);
    }
  }
}

/**
  Returns 1 if the check exists and has not been called yet, 0 otherwise.
*/
int fltk3::has_check(fltk3::TimeoutHandler cb, void *argp) {
  for (Check** p = &first_check; *p;) {
    Check* t = *p;
    if (t->cb == cb && t->arg == argp) {
      return 1;
    } else {
      p = &(t->next);
    }
  }
  return 0;
}

static void run_checks()
{
  // checks are a bit messy so that add/remove and wait may be called
  // from inside them without causing an infinite loop:
  if (next_check == first_check) {
    while (next_check) {
      Check* checkp = next_check;
      next_check = checkp->next;
      (checkp->cb)(checkp->arg);
    }
    next_check = first_check;
  }
}

#ifndef WIN32
static char in_idle;
#endif

////////////////////////////////////////////////////////////////
// wait/run/check/ready:

void (*fltk3::idle)(); // see fltk3::add_idle.cxx for the add/remove functions

extern int fl_ready(); // in Fl_<platform>.cxx
extern int fl_wait(double time); // in Fl_<platform>.cxx

/**
  See int fltk3::wait()
*/
double fltk3::wait(double time_to_wait) {
  // delete all widgets that were listed during callbacks
  do_widget_deletion();

#ifdef WIN32

  return fl_wait(time_to_wait);

#elif defined(__APPLE__)

  run_checks();
  if (idle) {
    if (!in_idle) {
      in_idle = 1;
      idle();
      in_idle = 0;
    }
    // the idle function may turn off idle, we can then wait:
    if (idle) time_to_wait = 0.0;
  }
  return fl_mac_flush_and_wait(time_to_wait, in_idle);

#else

  if (first_timeout) {
    elapse_timeouts();
    Timeout *t;
    while ((t = first_timeout)) {
      if (t->time > 0) break;
      // The first timeout in the array has expired.
      missed_timeout_by = t->time;
      // We must remove timeout from array before doing the callback:
      void (*cb)(void*) = t->cb;
      void *argp = t->arg;
      first_timeout = t->next;
      t->next = free_timeout;
      free_timeout = t;
      // Now it is safe for the callback to do add_timeout:
      cb(argp);
    }
  } else {
    reset_clock = 1; // we are not going to check the clock
  }
  run_checks();
//  if (idle && !fl_ready()) {
  if (idle) {
    if (!in_idle) {
      in_idle = 1;
      idle();
      in_idle = 0;
    }
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
    if (idle && !in_idle) // 'idle' may have been set within flush()
      time_to_wait = 0.0;
    return fl_wait(time_to_wait);
  }
#endif
}

#define FOREVER 1e20

/**
  As long as any windows are displayed this calls fltk3::wait()
  repeatedly.  When all the windows are closed it returns zero
  (supposedly it would return non-zero on any errors, but FLTK calls
  exit directly for these).  A normal program will end main()
  with return fltk3::run();.
*/
int fltk3::run() {
  while (Fl_X::first) wait(FOREVER);
  return 0;
}

#ifdef WIN32

// Function to initialize COM/OLE for usage. This must be done only once.
// We define a flag to register whether we called it:
static char oleInitialized = 0;

// This calls the Windows function OleInitialize() exactly once.
void fl_OleInitialize() {
  if (!oleInitialized) {
    OleInitialize(0L);
    oleInitialized = 1;
  }
}

// This calls the Windows function OleUninitialize() only, if
// OleInitialize has been called before.
void fl_OleUninitialize() {
  if (oleInitialized) {
    OleUninitialize();
    oleInitialized = 0;
  }
}

class Fl_Win32_At_Exit {
public:
  Fl_Win32_At_Exit() { }
  ~Fl_Win32_At_Exit() {
    fl_free_fonts();        // do some WIN32 cleanup
    fl_cleanup_pens();
    fl_OleUninitialize();
    fl_brush_action(1);
    fl_cleanup_dc_list();
  }
};
static Fl_Win32_At_Exit win32_at_exit;
#endif



/**
  Waits until "something happens" and then returns.  Call this
  repeatedly to "run" your program.  You can also check what happened
  each time after this returns, which is quite useful for managing
  program state.
  
  What this really does is call all idle callbacks, all elapsed
  timeouts, call fltk3::flush() to get the screen to update, and
  then wait some time (zero if there are idle callbacks, the shortest of
  all pending timeouts, or infinity), for any events from the user or
  any fltk3::add_fd() callbacks.  It then handles the events and
  calls the callbacks and then returns.

  The return value of fltk3::wait() is non-zero if there are any
  visible windows - this may change in future versions of FLTK.

  fltk3::wait(time) waits a maximum of \e time seconds.
  <i>It can return much sooner if something happens.</i>

  The return value is positive if an event or fd happens before the
  time elapsed.  It is zero if nothing happens (on Win32 this will only
  return zero if \e time is zero).  It is negative if an error
  occurs (this will happen on UNIX if a signal happens).
*/
int fltk3::wait() {
  if (!Fl_X::first) return 0;
  wait(FOREVER);
  return Fl_X::first != 0; // return true if there is a window
}

/**
  Same as fltk3::wait(0).  Calling this during a big calculation
  will keep the screen up to date and the interface responsive:
  
  \code
  while (!calculation_done()) {
  calculate();
  fltk3::check();
  if (user_hit_abort_button()) break;
  }
  \endcode
  
  This returns non-zero if any windows are displayed, and 0 if no
  windows are displayed (this is likely to change in future versions of
  FLTK).
*/
int fltk3::check() {
  wait(0.0);
  return Fl_X::first != 0; // return true if there is a window
}

/**
  This is similar to fltk3::check() except this does \e not
  call fltk3::flush() or any callbacks, which is useful if your
  program is in a state where such callbacks are illegal.  This returns
  true if fltk3::check() would do anything (it will continue to
  return true until you call fltk3::check() or fltk3::wait()).
  
  \code
  while (!calculation_done()) {
    calculate();
    if (fltk3::ready()) {
      do_expensive_cleanup();
      fltk3::check();
      if (user_hit_abort_button()) break;
    }
  }
  \endcode
*/
int fltk3::ready() {
#if ! defined( WIN32 )  &&  ! defined(__APPLE__)
  if (first_timeout) {
    elapse_timeouts();
    if (first_timeout->time <= 0) return 1;
  } else {
    reset_clock = 1;
  }
#endif
  return fl_ready();
}

////////////////////////////////////////////////////////////////
// Window list management:

#ifndef FLTK3_DOXYGEN
Fl_X* Fl_X::first;
#endif

fltk3::Window* fl_find(Window xid) {
  Fl_X *window;
  for (Fl_X **pp = &Fl_X::first; (window = *pp); pp = &window->next)
#if defined(WIN32) || defined(USE_X11)
   if (window->xid == xid) 
#elif defined(__APPLE_QUARTZ__)
   if (window->xid == xid && !window->w->window()) 
#else
# error unsupported platform
#endif // __APPLE__
	{
      if (window != Fl_X::first && !fltk3::modal()) {
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

/**
  Returns the first top-level window in the list of shown() windows.  If
  a modal() window is shown this is the top-most modal window, otherwise
  it is the most recent window to get an event.
*/
fltk3::Window* fltk3::first_window() {
  Fl_X* i = Fl_X::first;
  return i ? i->w : 0;
}

/**
  Returns the next top-level window in the list of shown() windows.
  You can use this call to iterate through all the windows that are shown().
  \param[in] window	must be shown and not NULL
*/
fltk3::Window* fltk3::next_window(const fltk3::Window* window) {
  Fl_X* i = Fl_X::i(window)->next;
  return i ? i->w : 0;
}

/**
 Sets the window that is returned by first_window().  
 The window is removed from wherever it is in the
 list and inserted at the top.  This is not done if fltk3::modal()
 is on or if the window is not shown(). Because the first window
 is used to set the "parent" of modal windows, this is often
 useful.
 */
void fltk3::first_window(fltk3::Window* window) {
  if (!window || !window->shown()) return;
  fl_find( Fl_X::i(window)->xid );
}

/**
  Redraws all widgets.
*/
void fltk3::redraw() {
  for (Fl_X* i = Fl_X::first; i; i = i->next) i->w->redraw();
}

/**
  Causes all the windows that need it to be redrawn and graphics forced
  out through the pipes.
  
  This is what wait() does before looking for events.

  Note: in multi-threaded applications you should only call fltk3::flush()
  from the main thread. If a child thread needs to trigger a redraw event,
  it should instead call fltk3::awake() to get the main thread to process the
  event queue.
*/
void fltk3::flush() {
  if (damage()) {
    fltk3::damage_ = 0;
    for (Fl_X* i = Fl_X::first; i; i = i->next) {
      if (i->wait_for_expose) {fltk3::damage_ = 1; continue;}
      fltk3::Window* wi = i->w;
      if (!wi->visible_r()) continue;
      if (wi->damage()) {i->flush(); wi->clear_damage();}
      // destroy damage regions for windows that don't use them:
      if (i->region) {XDestroyRegion(i->region); i->region = 0;}
    }
  }
#if defined(USE_X11)
  if (fl_display) XFlush(fl_display);
#elif defined(WIN32)
  GdiFlush();
#elif defined (__APPLE_QUARTZ__)
  if (fl_gc)
    CGContextFlush(fl_gc);
#else
# error unsupported platform
#endif
}


////////////////////////////////////////////////////////////////
// Event handlers:


struct handler_link {
  int (*handle)(int);
  handler_link *next;
};


static handler_link *handlers = 0;


/**
  Install a function to parse unrecognized events.  If FLTK cannot
  figure out what to do with an event, it calls each of these functions
  (most recent first) until one of them returns non-zero.  If none of
  them returns non-zero then the event is ignored.  Events that cause
  this to be called are:
  
  - fltk3::SHORTCUT events that are not recognized by any widget.
    This lets you provide global shortcut keys.
 -  fltk3::SCREEN_CONFIGURATION_CHANGED events.
  - System events that FLTK does not recognize.  See fl_xevent.
  - \e Some other events when the widget FLTK selected returns
    zero from its handle() method.  Exactly which ones may change
    in future versions, however.

 \see fltk3::remove_handler(fltk3::EventHandler)
 \see fltk3::event_dispatch(fltk3::EventDispatch d)
 \see fltk3::handle(int, fltk3::Window*)
*/
void fltk3::add_handler(fltk3::EventHandler ha) {
  handler_link *l = new handler_link;
  l->handle = ha;
  l->next = handlers;
  handlers = l;
}


/**
 Removes a previously added event handler.
 \see fltk3::handle(int, fltk3::Window*)
*/
void fltk3::remove_handler(fltk3::EventHandler ha) {
  handler_link *l, *p;

  // Search for the handler in the list...
  for (l = handlers, p = 0; l && l->handle != ha; p = l, l = l->next);

  if (l) {
    // Found it, so remove it from the list...
    if (p) p->next = l->next;
    else handlers = l->next;

    // And free the record...
    delete l;
  }
}

int (*fl_local_grab)(int); // used by fl_dnd.cxx

static int send_handlers(int e) {
  for (const handler_link *hl = handlers; hl; hl = hl->next)
    if (hl->handle(e)) return 1;
  return 0;
}

////////////////////////////////////////////////////////////////

fltk3::Widget* fl_oldfocus; // kludge for fltk3::Group...

/**
    Sets the widget that will receive fltk3::KEYBOARD events.
    
    If you change fltk3::focus(), the previous widget and all
    parents (that don't contain the new widget) are sent fltk3::UNFOCUS
    events.  Changing the focus does \e not send fltk3::FOCUS to
    this or any widget, because sending fltk3::FOCUS is supposed to
    \e test if the widget wants the focus (by it returning non-zero from
    handle()).
    
    \see fltk3::Widget::take_focus()
*/
void fltk3::focus(fltk3::Widget *o) {
  if (o && !o->visible_focus()) return;
  if (grab()) return; // don't do anything while grab is on
  fltk3::Widget *p = focus_;
  if (o != p) {
    fltk3::compose_reset();
    focus_ = o;
    // make sure that fl_xfocus is set to the top level window
    // of this widget, or fl_fix_focus will clear our focus again
    if (o) {
      fltk3::Window *win = 0, *w1 = o->as_window();
      if (!w1) w1 = o->window();
      while (w1) { win=w1; w1=win->window(); }
      if (win) {
#ifdef __APPLE__
	if (fl_xfocus != win) {
	  Fl_X *x = Fl_X::i(win);
	  if (x) x->set_key_window();
        }
#endif
	fl_xfocus = win;
      }
    }
    // take focus from the old focused window
    fl_oldfocus = 0;
    int old_event = e_number;
    e_number = fltk3::UNFOCUS;
    for (; p; p = p->parent()) {
      p->handle(fltk3::UNFOCUS);
      fl_oldfocus = p;
    }
    e_number = old_event;
  }
}

static char dnd_flag = 0; // make 'belowmouse' send DND_LEAVE instead of LEAVE

/**
    Sets the widget that is below the mouse.  This is for
    highlighting buttons.  It is not used to send fltk3::PUSH or 
    fltk3::MOVE directly, for several obscure reasons, but those events
    typically go to this widget.  This is also the first widget tried for 
    fltk3::SHORTCUT events.
    
    If you change the belowmouse widget, the previous one and all
    parents (that don't contain the new widget) are sent fltk3::LEAVE
    events.  Changing this does \e not send fltk3::ENTER to this
    or any widget, because sending fltk3::ENTER is supposed to \e test
    if the widget wants the mouse (by it returning non-zero from 
    handle()).
*/
void fltk3::belowmouse(fltk3::Widget *o) {
  if (grab()) return; // don't do anything while grab is on
  fltk3::Widget *p = belowmouse_;
  if (o != p) {
    belowmouse_ = o;
    int old_event = e_number;
    e_number = dnd_flag ? fltk3::DND_LEAVE : fltk3::LEAVE;
    for (; p && !p->contains(o); p = p->parent()) {
      p->handle(e_number);
    }
    e_number = old_event;
  }
}

/**
    Sets the widget that is being pushed. fltk3::DRAG or 
    fltk3::RELEASE (and any more fltk3::PUSH) events will be sent to
    this widget.
    
    If you change the pushed widget, the previous one and all parents
    (that don't contain the new widget) are sent fltk3::RELEASE
    events.  Changing this does \e not send fltk3::PUSH to this
    or any widget, because sending fltk3::PUSH is supposed to \e test
    if the widget wants the mouse (by it returning non-zero from 
    handle()).
*/
 void fltk3::pushed(fltk3::Widget *o) {
  pushed_ = o;
}

static void nothing(fltk3::Widget *) {}
void (*fltk3::Tooltip::enter)(fltk3::Widget *) = nothing;
void (*fltk3::Tooltip::exit)(fltk3::Widget *) = nothing;

// Update modal(), focus() and other state according to system state,
// and send fltk3::ENTER, fltk3::LEAVE, fltk3::FOCUS, and/or fltk3::UNFOCUS events.
// This is the only function that produces these events in response
// to system activity.
// This is called whenever a window is added or hidden, and whenever
// X says the focus or mouse window have changed.

void fl_fix_focus() {
#ifdef DEBUG
  puts("fl_fix_focus();");
#endif // DEBUG

  if (fltk3::grab()) return; // don't do anything while grab is on.

  // set focus based on fltk3::modal() and fl_xfocus
  fltk3::Widget* w = fl_xfocus;
  if (w) {
    unsigned int saved = fltk3::e_keysym;
    if (fltk3::e_keysym < (fltk3::MouseButton + fltk3::LEFT_MOUSE) ||
        fltk3::e_keysym > (fltk3::MouseButton + fltk3::RIGHT_MOUSE))
      fltk3::e_keysym = 0; // make sure widgets don't think a keystroke moved focus
    while (w->parent()) w = w->parent();
    if (fltk3::modal()) w = fltk3::modal();
    if (!w->contains(fltk3::focus()))
      if (!w->take_focus()) fltk3::focus(w);
    fltk3::e_keysym = saved;
  } else
    fltk3::focus(0);

// MRS: Originally we checked the button state, but a user reported that it
//      broke click-to-focus in FLWM?!?
//  if (!(fltk3::event_state() & 0x7f00000 /*fltk3::BUTTONS*/)) {
  if (!fltk3::pushed()) {
    // set belowmouse based on fltk3::modal() and fl_xmousewin:
    w = fl_xmousewin;
    if (w) {
      if (fltk3::modal()) w = fltk3::modal();
      if (!w->contains(fltk3::belowmouse())) {
        int old_event = fltk3::e_number;
	w->handle(fltk3::e_number = fltk3::ENTER);
	fltk3::e_number = old_event;
	if (!w->contains(fltk3::belowmouse())) fltk3::belowmouse(w);
      } else {
	// send a fltk3::MOVE event so the enter/leave state is up to date
	fltk3::e_x = fltk3::e_x_root-fl_xmousewin->x();
	fltk3::e_y = fltk3::e_y_root-fl_xmousewin->y();
        int old_event = fltk3::e_number;
	w->handle(fltk3::e_number = fltk3::MOVE);
	fltk3::e_number = old_event;
      }
    } else {
      fltk3::belowmouse(0);
      fltk3::Tooltip::enter(0);
    }
  }
}

#if !(defined(WIN32) || defined(__APPLE__))
extern fltk3::Widget *fl_selection_requestor; // from Fl_x.cxx
#endif

// This function is called by ~Fl_Widget() and by fltk3::Widget::deactivate()
// and by fltk3::Widget::hide().  It indicates that the widget does not want
// to receive any more events, and also removes all global variables that
// point at the widget.
// I changed this from the 1.0.1 behavior, the older version could send
// fltk3::LEAVE or fltk3::UNFOCUS events to the widget.  This appears to not be
// desirable behavior and caused flwm to crash.

void fl_throw_focus(fltk3::Widget *o) {
#ifdef DEBUG
  printf("fl_throw_focus(o=%p)\n", o);
#endif // DEBUG

  if (o->contains(fltk3::pushed())) fltk3::pushed_ = 0;
#if !(defined(WIN32) || defined(__APPLE__))
  if (o->contains(fl_selection_requestor)) fl_selection_requestor = 0;
#endif
  if (o->contains(fltk3::belowmouse())) fltk3::belowmouse_ = 0;
  if (o->contains(fltk3::focus())) fltk3::focus_ = 0;
  if (o == fl_xfocus) fl_xfocus = 0;
  if (o == fltk3::Tooltip::current()) fltk3::Tooltip::current(0);
  if (o == fl_xmousewin) fl_xmousewin = 0;
  fltk3::Tooltip::exit(o);
  fl_fix_focus();
}

////////////////////////////////////////////////////////////////

// Call to->handle(), but first replace the mouse x/y with the correct
// values to account for nested windows. 'window' is the outermost
// window the event was posted to by the system:
static int send(int event, fltk3::Widget* to, fltk3::Window* window) {
  int dx, dy;
  int old_event = fltk3::e_number;
  if (window) {
    dx = window->x();
    dy = window->y();
  } else {
    dx = dy = 0;
  }
  for (const fltk3::Widget* w = to; w; w = w->parent())
    if (w->type()>=fltk3::WINDOW) {dx -= w->x(); dy -= w->y();}
  int save_x = fltk3::e_x; fltk3::e_x += dx;
  int save_y = fltk3::e_y; fltk3::e_y += dy;
  int ret = to->handle(fltk3::e_number = event);
  fltk3::e_number = old_event;
  fltk3::e_y = save_y;
  fltk3::e_x = save_x;
  return ret;
}


/**
 \brief Set a new event dispatch function.

 The event dispatch function is called after native events are converted to
 FLTK events, but before they are handled by FLTK. If the dispatch function
 fltk3::EventDispatch \p d is set, it is up to the dispatch function to call
 fltk3::handle_(int, fltk3::Window*) or to ignore the event.

 The dispatch function itself must return 0 if it ignored the event,
 or non-zero if it used the event. If you call fltk3::handle_(), then
 this will return the correct value.

 The event dispatch can be used to handle exceptions in FLTK events and
 callbacks before they reach the native event handler:

 \code
 int myHandler(int e, fltk3::Window *w) {
   try {
     return fltk3::handle_(e, w);
   } catch () {
     ...
   }
 }

 main() {
   fltk3::event_dispatch(myHandler);
   ...
   fltk3::run();
 }
 \endcode

 \param d new dispatch function, or NULL 
 \see fltk3::add_handler(fltk3::EventHandler)
 \see fltk3::handle(int, fltk3::Window*)
 \see fltk3::handle_(int, fltk3::Window*)
 */
void fltk3::event_dispatch(fltk3::EventDispatch d)
{
  e_dispatch = d; 
}


/** 
 \brief Return the current event dispatch function. 
 */
fltk3::EventDispatch fltk3::event_dispatch() 
{ 
  return e_dispatch; 
}


/**
 \brief Handle events from the window system.

 This is called from the native event dispatch after native events have been
 converted to FLTK notation. This function calls fltk3::handle_(int, fltk3::Window*) 
 unless the user sets a dispatch function. If a user dispatch function is set,
 the user must make sure that fltk3::handle_() is called, or the event will be
 ignored.

 \param e the event type (fltk3::event_number() is not yet set)
 \param window the window that caused this event
 \return 0 if the event was not handled

 \see fltk3::add_handler(fltk3::EventHandler)
 \see fltk3::event_dispatch(fltk3::EventDispatch)
 */
int fltk3::handle(int e, fltk3::Window* window)
{
  if (e_dispatch) {
    return e_dispatch(e, window);
  } else {
    return handle_(e, window);
  }
}


/**
 \brief Handle events from the window system.

 This function is called from the native event dispatch, unless the user sets 
 another dispatch function. In that case, the user dispatch function must 
 decide when to call fltk3::handle_(int, fltk3::Window*)

 \param e the event type (fltk3::event_number() is not yet set)
 \param window the window that caused this event
 \return 0 if the event was not handled

 \see fltk3::event_dispatch(fltk3::EventDispatch)
 */
int fltk3::handle_(int e, fltk3::Window* window)
{  
  e_number = e;
  if (fl_local_grab) return fl_local_grab(e);

  fltk3::Widget* wi = window;

  switch (e) {

  case fltk3::CLOSE:
    if ( grab() || (modal() && window != modal()) ) return 0;
    wi->do_callback();
    return 1;

  case fltk3::SHOW:
    wi->Widget::show(); // this calls fltk3::Widget::show(), not fltk3::Window::show()
    return 1;

  case fltk3::HIDE:
    wi->Widget::hide(); // this calls fltk3::Widget::hide(), not fltk3::Window::hide()
    return 1;

  case fltk3::PUSH:
#ifdef DEBUG
    printf("fltk3::handle(e=%d, window=%p);\n", e, window);
#endif // DEBUG

    if (grab()) wi = grab();
    else if (modal() && wi != modal()) return 0;
    pushed_ = wi;
    fltk3::Tooltip::current(wi);
    if (send(e, wi, window)) return 1;
    // raise windows that are clicked on:
    window->show();
    return 1;

  case fltk3::DND_ENTER:
  case fltk3::DND_DRAG:
    dnd_flag = 1;
    break;

  case fltk3::DND_LEAVE:
    dnd_flag = 1;
    belowmouse(0);
    dnd_flag = 0;
    return 1;

  case fltk3::DND_RELEASE:
    wi = belowmouse();
    break;

  case fltk3::MOVE:
  case fltk3::DRAG:
    fl_xmousewin = window; // this should already be set, but just in case.
    if (pushed()) {
      wi = pushed();
      if (grab()) wi = grab();
      e_number = e = fltk3::DRAG;
      break;
    }
    if (modal() && wi != modal()) wi = 0;
    if (grab()) wi = grab();
    { int ret;
      fltk3::Widget* pbm = belowmouse();
#ifdef __APPLE__
      if (fl_mac_os_version < 100500) {
        // before 10.5, mouse moved events aren't sent to borderless windows such as tooltips
	fltk3::Window *tooltip = fltk3::Tooltip::current_window();
	int inside = 0;
	if (tooltip && tooltip->shown() ) { // check if a tooltip window is currently opened
	  // check if mouse is inside the tooltip
	  inside = (fltk3::event_x_root() >= tooltip->x() && fltk3::event_x_root() < tooltip->x() + tooltip->w() &&
	  fltk3::event_y_root() >= tooltip->y() && fltk3::event_y_root() < tooltip->y() + tooltip->h() );
	}
	// if inside, send event to tooltip window instead of background window
	if (inside) ret = send(e, tooltip, window);
	else ret = (wi && send(e, wi, window));
      } else
#endif
      ret = (wi && send(e, wi, window));
   if (pbm != belowmouse()) {
#ifdef DEBUG
      printf("fltk3::handle(e=%d, window=%p);\n", e, window);
#endif // DEBUG
      fltk3::Tooltip::enter(belowmouse());
    }
    return ret;}

  case fltk3::RELEASE: {
//    printf("fltk3::RELEASE: window=%p, pushed() = %p, grab() = %p, modal() = %p\n",
//           window, pushed(), grab(), modal());

    if (grab()) {
      wi = grab();
      pushed_ = 0; // must be zero before callback is done!
    } else if (pushed()) {
      wi = pushed();
      pushed_ = 0; // must be zero before callback is done!
    } else if (modal() && wi != modal()) return 0;
    int r = send(e, wi, window);
    fl_fix_focus();
    return r;}

  case fltk3::UNFOCUS:
    window = 0;
  case fltk3::FOCUS:
    fl_xfocus = window;
    fl_fix_focus();
    return 1;

  case fltk3::KEYUP:
    // Send the key-up to the current focus widget. This is not
    // always the same widget that received the corresponding
    // fltk3::KEYBOARD event because focus may have changed.
    // Sending the KEYUP to the right KEYDOWN is possible, but
    // would require that we track the KEYDOWN for every possible 
    // key stroke (users may hold down multiple keys!) and then 
    // make sure that the widget still exists before sending
    // a KEYUP there. I believe that the current solution is
    // "close enough".
    for (wi = grab() ? grab() : focus(); wi; wi = wi->parent())
      if (send(fltk3::KEYUP, wi, window)) return 1;
    return 0;

  case fltk3::KEYBOARD:
#ifdef DEBUG
    printf("fltk3::handle(e=%d, window=%p);\n", e, window);
#endif // DEBUG

    fltk3::Tooltip::enter((fltk3::Widget*)0);

    fl_xfocus = window; // this should not happen!  But maybe it does:

    // Try it as keystroke, sending it to focus and all parents:
    for (wi = grab() ? grab() : focus(); wi; wi = wi->parent())
      if (send(fltk3::KEYBOARD, wi, window)) return 1;

    // recursive call to try shortcut:
    if (handle(fltk3::SHORTCUT, window)) return 1;

    // and then try a shortcut with the case of the text swapped, by
    // changing the text and falling through to fltk3::SHORTCUT case:
    {unsigned char* c = (unsigned char*)event_text(); // cast away const
    if (!isalpha(*c)) return 0;
    *c = isupper(*c) ? tolower(*c) : toupper(*c);}
    e_number = e = fltk3::SHORTCUT;

  case fltk3::SHORTCUT:
    if (grab()) {wi = grab(); break;} // send it to grab window

    // Try it as shortcut, sending to mouse widget and all parents:
    wi = belowmouse();
    if (!wi) {
      wi = modal();
      if (!wi) wi = window;
    } else if (wi->window() != first_window()) {
      if (send(fltk3::SHORTCUT, first_window(), first_window())) return 1;
    }

    for (; wi; wi = wi->parent()) {
      if (send(fltk3::SHORTCUT, wi, wi->window())) return 1;
    }

    // try using add_handle() functions:
    if (send_handlers(fltk3::SHORTCUT)) return 1;

    // make Escape key close windows:
    if (event_key()==fltk3::EscapeKey) {
      wi = modal(); if (!wi) wi = window;
      wi->do_callback();
      return 1;
    }

    return 0;

  case fltk3::ENTER:
#ifdef DEBUG
    printf("fltk3::handle(e=%d, window=%p);\n", e, window);
#endif // DEBUG

    fl_xmousewin = window;
    fl_fix_focus();
    fltk3::Tooltip::enter(belowmouse());
    return 1;

  case fltk3::LEAVE:
#ifdef DEBUG
    printf("fltk3::handle(e=%d, window=%p);\n", e, window);
#endif // DEBUG

    if (!pushed_) {
      belowmouse(0);
      fltk3::Tooltip::enter(0);
    }
    if (window == fl_xmousewin) {fl_xmousewin = 0; fl_fix_focus();}
    return 1;

  case fltk3::MOUSEWHEEL:
    fl_xfocus = window; // this should not happen!  But maybe it does:

    // Try sending it to the "grab" first
    if (grab() && grab()!=modal() && grab()!=window) {
      if (send(fltk3::MOUSEWHEEL, grab(), window)) return 1;
    }
    // Now try sending it to the "modal" window
    if (modal()) {
      send(fltk3::MOUSEWHEEL, modal(), window);
      return 1;
    }
    // Finally try sending it to the window, the event occured in
    if (send(fltk3::MOUSEWHEEL, window, window)) return 1;
  default:
    break;
  }
  if (wi && send(e, wi, window)) {
    dnd_flag = 0;
    return 1;
  }
  dnd_flag = 0;
  return send_handlers(e);
}

////////////////////////////////////////////////////////////////
// hide() destroys the X window, it does not do unmap!

#if !defined(WIN32) && USE_XFT
extern void fl_destroy_xft_draw(Window);
#endif

void fltk3::Window::hide() {
  clear_visible();

  if (!shown()) return;

  // remove from the list of windows:
  Fl_X* ip = i;
  Fl_X** pp = &Fl_X::first;
  for (; *pp != ip; pp = &(*pp)->next) if (!*pp) return;
  *pp = ip->next;
#ifdef __APPLE__
  ip->unlink();
  // MacOS X manages a single pointer per application. Make sure that hiding
  // a toplevel window will not leave us with some random pointer shape, or
  // worst case, an invisible pointer
  if (!parent()) cursor(fltk3::CURSOR_DEFAULT);
#endif
  i = 0;

  // recursively remove any subwindows:
  for (Fl_X *wi = Fl_X::first; wi;) {
    fltk3::Window* W = wi->w;
    if (W->window() == this) {
      W->hide();
      W->set_visible();
      wi = Fl_X::first;
    } else wi = wi->next;
  }

  if (this == fltk3::modal_) { // we are closing the modal window, find next one:
    fltk3::Window* W;
    for (W = fltk3::first_window(); W; W = fltk3::next_window(W))
      if (W->modal()) break;
    fltk3::modal_ = W;
  }

  // Make sure no events are sent to this window:
  fl_throw_focus(this);
  handle(fltk3::HIDE);

#if defined(WIN32)
  // this little trick keeps the current clipboard alive, even if we are about
  // to destroy the window that owns the selection.
  if (GetClipboardOwner()==ip->xid) {
    fltk3::Window *w1 = fltk3::first_window();
    if (w1 && OpenClipboard(fl_xid(w1))) {
      EmptyClipboard();
      SetClipboardData(CF_TEXT, NULL);
      CloseClipboard();
    }
  }
  // Send a message to myself so that I'll get out of the event loop...
  PostMessage(ip->xid, WM_APP, 0, 0);
  if (ip->private_dc) fl_release_dc(ip->xid, ip->private_dc);
    if (ip->xid == fl_window && fl_gc) {
      fl_release_dc(fl_window, fl_gc);
      fl_window = (HWND)-1;
      fl_gc = 0;
# ifdef FLTK_USE_CAIRO
      if (fltk3::cairo_autolink_context()) fltk3::cairo_make_current((fltk3::Window*) 0);
# endif
    }
#elif defined(__APPLE_QUARTZ__)
  Fl_X::q_release_context(ip);
  if ( ip->xid == fl_window && !parent() )
    fl_window = 0;
#endif

  if (ip->region) XDestroyRegion(ip->region);

#if defined(USE_X11)
# if USE_XFT
  fl_destroy_xft_draw(ip->xid);
# endif
  // this test makes sure ip->xid has not been destroyed already
  if (ip->xid) XDestroyWindow(fl_display, ip->xid);
#elif defined(WIN32)
  // this little trickery seems to avoid the popup window stacking problem
  HWND p = GetForegroundWindow();
  if (p==GetParent(ip->xid)) {
    ShowWindow(ip->xid, SW_HIDE);
    ShowWindow(p, SW_SHOWNA);
  }
  XDestroyWindow(fl_display, ip->xid);
#elif defined(__APPLE_QUARTZ__)
  ip->destroy();
#else
# error unsupported platform
#endif
  
#ifdef WIN32
  // Try to stop the annoying "raise another program" behavior
  if (non_modal() && fltk3::first_window() && fltk3::first_window()->shown())
    fltk3::first_window()->show();
#endif
  delete ip;
}

fltk3::Window::~Window() {
  hide();
  if (xclass_) {
    free(xclass_);
  }
}

// fltk3::SHOW and fltk3::HIDE are called whenever the visibility of this widget
// or any parent changes.  We must correctly map/unmap the system's window.

// For top-level windows it is assumed the window has already been
// mapped or unmapped!!!  This is because this should only happen when
// fltk3::Window::show() or fltk3::Window::hide() is called, or in response to
// iconize/deiconize events from the system.

int fltk3::Window::handle(int ev)
{
  if (parent()) {
    switch (ev) {
    case fltk3::SHOW:
      if (!shown()) show();
      else {
#if defined(USE_X11) || defined(WIN32)
        XMapWindow(fl_display, fl_xid(this)); // extra map calls are harmless
#elif defined(__APPLE_QUARTZ__)
	i->map();
#else
# error unsupported platform
#endif // __APPLE__
      }
      break;
    case fltk3::HIDE:
      if (shown()) {
	// Find what really turned invisible, if it was a parent window
	// we do nothing.  We need to avoid unnecessary unmap calls
	// because they cause the display to blink when the parent is
	// remapped.  However if this or any intermediate non-window
	// widget has really had hide() called directly on it, we must
	// unmap because when the parent window is remapped we don't
	// want to reappear.
	if (visible()) {
	 fltk3::Widget* p = parent(); for (;p->visible();p = p->parent()) {}
	 if (p->type() >= fltk3::WINDOW) break; // don't do the unmap
	}
#if defined(USE_X11) || defined(WIN32)
	XUnmapWindow(fl_display, fl_xid(this));
#elif defined(__APPLE_QUARTZ__)
	i->unmap();
#else
# error platform unsupported
#endif
      }
      break;
    }
//  } else if (ev == fltk3::FOCUS || ev == fltk3::UNFOCUS) {
//    fltk3::Tooltip::exit(fltk3::Tooltip::current());
  }

  return Group::handle(ev);
}

////////////////////////////////////////////////////////////////
// Back compatibility cut & paste functions for fltk 1.1 only:

/** Back-compatibility only: The single-argument call can be used to
    move the selection to another widget or to set the owner to
    NULL, without changing the actual text of the
    selection. fltk3::SELECTIONCLEAR is sent to the previous
    selection owner, if any.
    
    <i>Copying the buffer every time the selection is changed is
    obviously wasteful, especially for large selections.  An interface will
    probably be added in a future version to allow the selection to be made
    by a callback function.  The current interface will be emulated on top
    of this.</i>
*/
void fltk3::selection_owner(fltk3::Widget *owner) {selection_owner_ = owner;}

/**
  Changes the current selection.  The block of text is
  copied to an internal buffer by FLTK (be careful if doing this in
  response to an fltk3::PASTE as this \e may be the same buffer
  returned by event_text()).  The selection_owner()
  widget is set to the passed owner.
*/
void fltk3::selection(fltk3::Widget &owner, const char* text, int len) {
  selection_owner_ = &owner;
  fltk3::copy(text, len, 0);
}

/** Backward compatibility only.
  This calls fltk3::paste(receiver, 0);
  \see fltk3::paste(fltk3::Widget &receiver, int clipboard)
*/
void fltk3::paste(fltk3::Widget &receiver) {
  fltk3::paste(receiver, 0);
}

////////////////////////////////////////////////////////////////

#include <fltk3/draw.h>

void fltk3::Widget::redraw() {
  damage(fltk3::DAMAGE_ALL);
}

void fltk3::Widget::redraw_label() {
  if (window()) {
    if (box() == fltk3::NO_BOX) {
      // Widgets with the fltk3::NO_BOX boxtype need a parent to
      // redraw, since it is responsible for redrawing the
      // background...
      int X = x() > 0 ? x() - 1 : 0;
      int Y = y() > 0 ? y() - 1 : 0;
      window()->damage(fltk3::DAMAGE_ALL, X, Y, w() + 2, h() + 2);
    }

    if (align() && !(align() & fltk3::ALIGN_INSIDE) && window()->shown()) {
      // If the label is not inside the widget, compute the location of
      // the label and redraw the window within that bounding box...
      int W = 0, H = 0;
      Label::measure(W, H);
      W += 5; // Add a little to the size of the label to cover overflow
      H += 5;

      // FIXME:
      // This assumes that measure() returns the correct outline, which it does
      // not in all possible cases of alignment combinedwith image and symbols.
      switch (align() & 0x0f) {
        case fltk3::ALIGN_TOP_LEFT:
          window()->damage(fltk3::DAMAGE_EXPOSE, x(), y()-H, W, H); break;
        case fltk3::ALIGN_TOP:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()+(w()-W)/2, y()-H, W, H); break;
        case fltk3::ALIGN_TOP_RIGHT:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()+w()-W, y()-H, W, H); break;
        case fltk3::ALIGN_LEFT_TOP:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()-W, y(), W, H); break;
        case fltk3::ALIGN_RIGHT_TOP:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()+w(), y(), W, H); break;
        case fltk3::ALIGN_LEFT:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()-W, y()+(h()-H)/2, W, H); break;
        case fltk3::ALIGN_RIGHT:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()+w(), y()+(h()-H)/2, W, H); break;
        case fltk3::ALIGN_LEFT_BOTTOM:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()-W, y()+h()-H, W, H); break;
        case fltk3::ALIGN_RIGHT_BOTTOM:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()+w(), y()+h()-H, W, H); break;
        case fltk3::ALIGN_BOTTOM_LEFT:
          window()->damage(fltk3::DAMAGE_EXPOSE, x(), y()+h(), W, H); break;
        case fltk3::ALIGN_BOTTOM:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()+(w()-W)/2, y()+h(), W, H); break;
        case fltk3::ALIGN_BOTTOM_RIGHT:
          window()->damage(fltk3::DAMAGE_EXPOSE, x()+w()-W, y()+h(), W, H); break;
        default:
          window()->damage(fltk3::DAMAGE_ALL); break;
      }
    } else {
      // The label is inside the widget, so just redraw the widget itself...
      damage(fltk3::DAMAGE_ALL);
    }
  }
}

void fltk3::Widget::damage(uchar fl) {
  if (type() < fltk3::WINDOW) {
    // damage only the rectangle covered by a child widget:
    damage(fl, x(), y(), w(), h());
  } else {
    // damage entire window by deleting the region:
    Fl_X* i = Fl_X::i((fltk3::Window*)this);
    if (!i) return; // window not mapped, so ignore it
    if (i->region) {XDestroyRegion(i->region); i->region = 0;}
    damage_ |= fl;
    fltk3::damage(fltk3::DAMAGE_CHILD);
  }
}

void fltk3::Widget::damage(uchar fl, int X, int Y, int W, int H) {
  fltk3::Widget* wi = this;
  // mark all parent widgets between this and window with fltk3::DAMAGE_CHILD:
  while (wi->type() < fltk3::WINDOW) {
    wi->damage_ |= fl;
    wi = wi->parent();
    if (!wi) return;
    fl = fltk3::DAMAGE_CHILD;
  }
  Fl_X* i = Fl_X::i((fltk3::Window*)wi);
  if (!i) return; // window not mapped, so ignore it

  // clip the damage to the window and quit if none:
  if (X < 0) {W += X; X = 0;}
  if (Y < 0) {H += Y; Y = 0;}
  if (W > wi->w()-X) W = wi->w()-X;
  if (H > wi->h()-Y) H = wi->h()-Y;
  if (W <= 0 || H <= 0) return;

  if (!X && !Y && W==wi->w() && H==wi->h()) {
    // if damage covers entire window delete region:
    wi->damage(fl);
    return;
  }

  if (wi->damage()) {
    // if we already have damage we must merge with existing region:
    if (i->region) {
#if defined(USE_X11)
      XRectangle R;
      R.x = X; R.y = Y; R.width = W; R.height = H;
      XUnionRectWithRegion(&R, i->region, i->region);
#elif defined(WIN32)
      fltk3::Region R = XRectangleRegion(X, Y, W, H);
      CombineRgn(i->region, i->region, R, RGN_OR);
      XDestroyRegion(R);
#elif defined(__APPLE_QUARTZ__)
      CGRect arg = fl_cgrectmake_cocoa(X, Y, W, H);
      int j; // don't add a rectangle totally inside the fltk3::Region
      for(j = 0; j < i->region->count; j++) {
        if(CGRectContainsRect(i->region->rects[j], arg)) break;
      }
      if( j >= i->region->count) {
        i->region->rects = (CGRect*)realloc(i->region->rects, (++(i->region->count)) * sizeof(CGRect));
        i->region->rects[i->region->count - 1] = arg;
      }
#else
# error unsupported platform
#endif
    }
    wi->damage_ |= fl;
  } else {
    // create a new region:
    if (i->region) XDestroyRegion(i->region);
    i->region = XRectangleRegion(X,Y,W,H);
    wi->damage_ = fl;
  }
  fltk3::damage(fltk3::DAMAGE_CHILD);
}
void fltk3::Window::flush() {
  make_current();
//if (damage() == fltk3::DAMAGE_EXPOSE && can_boxcheat(box())) fl_boxcheat = this;
  fltk3::clip_region(i->region); i->region = 0;
  draw();
}

#ifdef WIN32
#  include "win32.cxx"
//#elif defined(__APPLE__)
#endif

//
// The following methods allow callbacks to schedule the deletion of
// widgets at "safe" times.
//

static int		num_dwidgets = 0, alloc_dwidgets = 0;
static fltk3::Widget	**dwidgets = 0;

/** 
  Schedules a widget for deletion at the next call to the event loop.
  Use this method to delete a widget inside a callback function.

  To avoid early deletion of widgets, this function should be called
  toward the end of a callback and only after any call to the event
  loop (fltk3::wait(), fltk3::flush(), fltk3::check(), fltk3::ask(), etc.).

  When deleting groups or windows, you must only delete the group or
  window widget and not the individual child widgets.

  \since FLTK 1.3 it is not necessary to remove widgets from their parent
  groups or windows before calling this, because it will be done in the
  widget's destructor, but it is not a failure to do this nevertheless.

  \note In FLTK 1.1 you \b must remove widgets from their parent group
  (or window) before deleting them.

  \see fltk3::Widget::~Fl_Widget()
*/
void fltk3::delete_widget(fltk3::Widget *wi) {
  if (!wi) return;

  if (num_dwidgets >= alloc_dwidgets) {
    fltk3::Widget	**temp;

    temp = new fltk3::Widget *[alloc_dwidgets + 10];
    if (alloc_dwidgets) {
      memcpy(temp, dwidgets, alloc_dwidgets * sizeof(fltk3::Widget *));
      delete[] dwidgets;
    }

    dwidgets = temp;
    alloc_dwidgets += 10;
  }

  dwidgets[num_dwidgets] = wi;
  num_dwidgets ++;
}

/** 
    Deletes widgets previously scheduled for deletion.
    
    This is for internal use only. You should never call this directly.

    fltk3::do_widget_deletion() is called from the FLTK event loop or whenever
    you call fltk3::wait(). The previously scheduled widgets are deleted in the
    same order they were scheduled by calling fltk3::delete_widget().

    \see fltk3::delete_widget(fltk3::Widget *wi)
*/
void fltk3::do_widget_deletion() {
  if (!num_dwidgets) return;

  for (int i = 0; i < num_dwidgets; i ++)
    delete dwidgets[i];

  num_dwidgets = 0;
}

static fltk3::Widget ***widget_watch = 0;
static int num_widget_watch = 0;
static int max_widget_watch = 0;

/**
  Adds a widget pointer to the widget watch list.
  
  \note Internal use only, please use class fltk3::WidgetTracker instead.

  This can be used, if it is possible that a widget might be deleted during
  a callback or similar function. The widget pointer must be added to the
  watch list before calling the callback. After the callback the widget
  pointer can be queried, if it is NULL. \e If it is NULL, then the widget has been
  deleted during the callback and must not be accessed anymore. If the widget
  pointer is \e not NULL, then the widget has not been deleted and can be accessed
  safely.

  After accessing the widget, the widget pointer must be released from the
  watch list by calling fltk3::release_widget_pointer().

  Example for a button that is clicked (from its handle() method):
  \code
    fltk3::Widget *wp = this;		// save 'this' in a pointer variable
    fltk3::watch_widget_pointer(wp);	// add the pointer to the watch list
    set_changed();			// set the changed flag
    do_callback();			// call the callback
    if (!wp) {				// the widget has been deleted

      // DO NOT ACCESS THE DELETED WIDGET !

    } else {				// the widget still exists
      clear_changed();			// reset the changed flag
    }

    fltk3::release_widget_pointer(wp);	// remove the pointer from the watch list
   \endcode

   This works, because all widgets call fltk3::clear_widget_pointer() in their
   destructors.

   \see fltk3::release_widget_pointer()
   \see fltk3::clear_widget_pointer()

   An easier and more convenient method to control widget deletion during
   callbacks is to use the class fltk3::WidgetTracker with a local (automatic)
   variable.

   \see class fltk3::WidgetTracker
*/
void fltk3::watch_widget_pointer(fltk3::Widget *&w) 
{
  fltk3::Widget **wp = &w;
  int i;
  for (i=0; i<num_widget_watch; ++i) {
    if (widget_watch[i]==wp) return;
  }
  if (num_widget_watch==max_widget_watch) {
    max_widget_watch += 8;
    widget_watch = (fltk3::Widget***)realloc(widget_watch, sizeof(fltk3::Widget**)*max_widget_watch);
  }
  widget_watch[num_widget_watch++] = wp;
#ifdef DEBUG_WATCH
  printf ("\nwatch_widget_pointer:   (%d/%d) %8p => %8p\n",
    num_widget_watch,num_widget_watch,wp,*wp);
  fflush(stdout);
#endif // DEBUG_WATCH
}

/**
  Releases a widget pointer from the watch list.

  This is used to remove a widget pointer that has been added to the watch list
  with fltk3::watch_widget_pointer(), when it is not needed anymore.
  
  \note Internal use only, please use class fltk3::WidgetTracker instead.

  \see fltk3::watch_widget_pointer()
*/
void fltk3::release_widget_pointer(fltk3::Widget *&w)
{
  fltk3::Widget **wp = &w;
  int i,j=0;
  for (i=0; i<num_widget_watch; ++i) {
    if (widget_watch[i]!=wp) {
      if (j<i) widget_watch[j] = widget_watch[i]; // fill gap
      j++;
    }
#ifdef DEBUG_WATCH
    else { // found widget pointer
      printf ("release_widget_pointer: (%d/%d) %8p => %8p\n",
	i+1,num_widget_watch,wp,*wp);
    }
#endif //DEBUG_WATCH
  }
  num_widget_watch = j;
#ifdef DEBUG_WATCH
  printf ("                        num_widget_watch = %d\n\n",num_widget_watch);
  fflush(stdout);
#endif // DEBUG_WATCH
  return;
}
/**
  Clears a widget pointer \e in the watch list.

  This is called when a widget is destroyed (by its destructor). You should never
  call this directly.

  \note Internal use only !

  This method searches the widget watch list for pointers to the widget and
  clears each pointer that points to it. Widget pointers can be added to the
  widget watch list by calling fltk3::watch_widget_pointer() or by using the
  helper class fltk3::WidgetTracker (recommended).

  \see fltk3::watch_widget_pointer()
  \see class fltk3::WidgetTracker
*/
void fltk3::clear_widget_pointer(fltk3::Widget const *w) 
{
  if (w==0L) return;
  int i;
  for (i=0; i<num_widget_watch; ++i) {
    if (widget_watch[i] && *widget_watch[i]==w) {
      *widget_watch[i] = 0L;
    }
  }
}


/**
 \brief FLTK library options management.

 This function needs to be documented in more detail. It can be used for more
 optional settings, such as using a native file chooser instead of the FLTK one
 wherever possible, disabling tooltips, disabling visible focus, disabling 
 FLTK file chooser preview, etc. .

 There should be a command line option interface.

 There should be an application that manages options system wide, per user, and
 per application.

 \note As of FLTK 1.3.0, options can be managed within fluid, using the menu
 <i>Edit/Global FLTK Settings</i>.

 \param opt which option
 \return true or false
 \see enum fltk3::Option
 \see fltk3::option(fltk3::Option, bool)

 \since FLTK 1.3.0
 */ 
bool fltk3::option(fltk3::Option opt)
{
  if (!options_read_) {
    int tmp;
    { // first, read the system wide preferences
      fltk3::Preferences prefs(fltk3::Preferences::SYSTEM, "fltk.org", "fltk");
      fltk3::Preferences opt_prefs(prefs, "options");
      opt_prefs.get("ArrowFocus", tmp, 0);                      // default: off
      options_[OPTION_ARROW_FOCUS] = tmp;
      //opt_prefs.get("NativeFilechooser", tmp, 1);             // default: on
      //options_[OPTION_NATIVE_FILECHOOSER] = tmp;
      //opt_prefs.get("FilechooserPreview", tmp, 1);            // default: on
      //options_[OPTION_FILECHOOSER_PREVIEW] = tmp;
      opt_prefs.get("VisibleFocus", tmp, 1);                    // default: on
      options_[OPTION_VISIBLE_FOCUS] = tmp;
      opt_prefs.get("DNDText", tmp, 1);                         // default: on
      options_[OPTION_DND_TEXT] = tmp;
      opt_prefs.get("ShowTooltips", tmp, 1);                    // default: on
      options_[OPTION_SHOW_TOOLTIPS] = tmp;
    }
    { // next, check the user preferences
      // override system options only, if the option is set ( >= 0 )
      fltk3::Preferences prefs(fltk3::Preferences::USER, "fltk.org", "fltk");
      fltk3::Preferences opt_prefs(prefs, "options");
      opt_prefs.get("ArrowFocus", tmp, -1); 
      if (tmp >= 0) options_[OPTION_ARROW_FOCUS] = tmp;
      //opt_prefs.get("NativeFilechooser", tmp, -1); 
      //if (tmp >= 0) options_[OPTION_NATIVE_FILECHOOSER] = tmp;
      //opt_prefs.get("FilechooserPreview", tmp, -1);
      //if (tmp >= 0) options_[OPTION_FILECHOOSER_PREVIEW] = tmp;
      opt_prefs.get("VisibleFocus", tmp, -1); 
      if (tmp >= 0) options_[OPTION_VISIBLE_FOCUS] = tmp;
      opt_prefs.get("DNDText", tmp, -1); 
      if (tmp >= 0) options_[OPTION_DND_TEXT] = tmp;
      opt_prefs.get("ShowTooltips", tmp, -1); 
      if (tmp >= 0) options_[OPTION_SHOW_TOOLTIPS] = tmp;
    }
    { // now, if the developer has registered this app, we could as for per-application preferences
    }
    options_read_ = 1;
  }
  if (opt<0 || opt>=OPTION_LAST) 
    return false;
  return (bool)(options_[opt]!=0);
}

/**
 \brief Override an option while the application is running.
 
 This function does not change any system or user settings.
 
 \param opt which option
 \param val set to true or false
 \see enum fltk3::Option
 \see bool fltk3::option(fltk3::Option)
 */
void fltk3::option(fltk3::Option opt, bool val)
{
  if (opt<0 || opt>=OPTION_LAST) 
    return;
  if (!options_read_) {
    // first read this option, so we don't override our setting later
    option(opt);
  }
  options_[opt] = val;
}


// Helper class fltk3::WidgetTracker

/**
  The constructor adds a widget to the watch list.
*/
fltk3::WidgetTracker::WidgetTracker(fltk3::Widget *wi) 
{
  wp_ = wi;
  fltk3::watch_widget_pointer(wp_); // add pointer to watch list
}

/**
  The destructor removes a widget from the watch list.
*/
fltk3::WidgetTracker::~WidgetTracker() 
{
  fltk3::release_widget_pointer(wp_); // remove pointer from watch list
}

//
// End of "$Id$".
//
