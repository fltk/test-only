//
// "$Id: Fl_Window.cxx,v 1.106 2003/11/11 07:36:31 spitzak Exp $"
//
// Window widget class for the Fast Light Tool Kit (FLTK).
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

/*! \class fltk::Window

This widget produces an actual window. This can either be a main
window, with a border and title and all the window management
controls, or a "subwindow" inside a window. This is controlled by
whether or not the window has a parent(). Internally there are now
significant differences between "main" windows and "subwindows" and
these really should be different classes, they are the same for
historic fltk reasons.

Once you create a window, you usually add children Widgets to it by
using add(child) or by using begin() and then constructing the
children. See fltk::Group for more information on how to add and
remove children.

There are several subclasses of fltk::Window that provide
double-buffering, overlay, menu, and OpenGL support.

The window's callback is done if the user tries to close a window
using the window manager and fltk::modal() is zero or equal to the
window. Window has a default callback that calls Window::hide() and
calls exit(0) if this is the last top-level window.

*/

// This is the system-independent portions.  The huge amount of
// crap you need to do to communicate with X is in x.C, the
// equivalent (but totally different) crap for MSWindows is in win32.C

#include <fltk/Window.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/layout.h>
#include <fltk/run.h>
#include <fltk/x.h>
#include <config.h>
using namespace fltk;

/*! Return a pointer to the fltk::Window this widget is in.
  (it will skip any and all parent widgets between this and
  the window).  Returns NULL if none.  Note: for an
  fltk::Window, this returns the \e parent window (if any),
  not \e this window.
*/
Window *Widget::window() const {
  for (Widget *o = parent(); o; o = o->parent())
    if (o->is_window()) return (Window*)o;
  return 0;
}

void Window::draw() {Group::draw();}

/*! Sets the window title, which is drawn in the titlebar by the system. */
void Window::label(const char *name) {label(name, iconlabel());}

/*! Sets the text displayed below the icon (or in the taskbar). If
  you don't set this it defaults to the label() but if that appears
  to be a filename, it defaults to the last portion after the last
  '/' character. */
void Window::iconlabel(const char *iname) {label(label(), iname);}

void Window::default_callback(Window* window, void*) {
  window->hide();
  // if there are no visible windows we exit:
  // Not anymore, it has been restored to fltk 1.0 behavior. Instead
  // run() checks after each loop to see if all windows are closed.
  // if (!first()) exit(0);
}

static void revert(fltk::Style* s) {
  s->color_ = GRAY75;
  s->box_ = FLAT_BOX;
}
static NamedStyle style("Window", revert, &Window::default_style);
/*! By default a window has box() set to \c FLAT_BOX, and the color()
  set to \c GRAY75, which is a special color cell that is altered by
  fltk::set_background().

  If you plan to turn the border() off you may want to change
  the box() to \c UP_BOX. You can also produce something that looks
  like an arbitrary shape (though really it is showing the original
  screen contents in the "outside" area, so the window had better
  be temporary and the user cannot move it) by setting the box()
  to \c NO_BOX and making draw() only draw the opaque part.
*/
NamedStyle* Window::default_style = &::style;

void Window::_Window() {
  style(default_style);
  type(WINDOW_TYPE);
  i = 0;
  icon_ = 0;
  iconlabel_ = 0;
  //resizable(0); // new default for group
  minw = minh = maxw = maxh = 0;
  size_range_set = 0;
  child_of_ = 0;
  shortcut(EscapeKey);
  callback((Callback*)default_callback);
}

/*! This constructor is for \e child windows. You should use the
  constructor with just W and H for normal parent windows. This
  constructor leaves visible() true, so the child window will appear
  when the parent window has show() called. */
Window::Window(int X,int Y,int W, int H, const char *l, bool begin)
: Group(X, Y, W, H, l, begin) {
  _Window();
}

/*! This form of the constructor should be used for a "top-level"
  window (that is, one that is not inside another window). It
  correctly sets visible() to false and parent() to NULL. By not
  specifying the position of the window, the window system will pick a
  place to show the window (some older X window managers will allow
  the user to place the window by hand). If you want to force a
  position you should call position(x,y) or hotspot() before calling
  show().
*/
Window::Window(int W, int H, const char *l)
// fix common user error of a missing end() with current(0):
  : Group((Group::current(0),USEDEFAULT), USEDEFAULT, W, H, l) {
  _Window();
  clear_visible();
}

/*! \fn void Window::size_range(int minw, int minh, int maxw, int maxh, int dw, int dh, int aspect)

  Set the allowable range the user can resize this window to. This
  only works for top-level windows.

  - minw and minh are the smallest the window can be. 
  - maxw and maxh are the largest the window can be. If either is
  equal to the minimum then you cannot resize in that direction. If
  either is zero then FLTK picks a maximum size in that direction such
  that the window will fill the screen.
  - dw and dh are size increments. The window will be constrained to
  widths of minw + N * dw, where N is any non-negative integer. If
  these are less or equal to 1 they are ignored. (this is ignored on
  WIN32)

  - aspect is a flag that indicates that the window should preserve
  it's aspect ratio. This is ignored by WIN32 and by most X window
  managers (in addition X does not describe what to do if the minimum
  and maximum sizes have different aspect ratios...)

  It is undefined what happens if the current size does not fit in the
  constraints passed to size_range().

  If this function is not called, FLTK tries to figure out the range
  from the setting of resizeable():

  - If resizeable() is NULL (this is the default) then the window
  cannot be resized.
  - If either dimension of resizeable() is less than 100, then that is
  considered the minimum size. Otherwise the resizeable() has a
  minimum size of 100.
  - If either dimension of resizeable() is zero, then that is also the
  maximum size (so the window cannot resize in that direction).

*/

/*! void Window::resize(int,int,int,int)

  Change the size and position of the window. If shown() is true,
  these changes are communicated to the window server (which may
  refuse that size and cause a further resize). If shown() is false,
  the size and position are used when show() is called. See
  fltk::Group for the effect of resizing on the child widgets.

  The special value fltk::USEDEFAULT may be used for x and y indicate
  that the system should choose the window's position. This will only
  work before show() is called.

*/

////////////////////////////////////////////////////////////////
// show() and hide()...
// SHOW events will normally create and map the window, HIDE will
// unmap.  On both X and Win32 creating a window requires a lot of ugly
// cruft, some of it is here and much of it is in the machine-specific
// code like Fl_x.cxx.  There are also static variables (!) used to
// modify how the window is created, such as to create it iconized or
// to create it with a parent.

/*! \fn bool Window::shown() const
  Returns non-zero if show() has been called, but destroy() has not
  been called. Note that this returns true if hide() was called or
  if the user has iconized the window.
*/

// This is set by arg to argv[0], or the user can set it.
// It is used by X to look up stuff in the X resource database:
const char* Window::xclass_ = "fltk";

bool fl_show_iconic; // set by iconize() or by -i arg switch

#ifdef _WIN32
static void keep_app_active() {
  // activate some other window so the active app does not change!
  if (grab()) return;
  for (CreatedWindow* x = CreatedWindow::first; x; x = x->next)
    if (!x->window->parent() && x->window->visible()) {
      //BringWindowToTop(x->xid);
      SetActiveWindow(x->xid);
      break;
    }
}
#endif

extern Window* fl_actual_window; // in Fl.cxx

extern Window* fl_actual_window; // in Fl.cxx

int Window::handle(int event) {
  switch (event) {
  case SHOW:
    // create the window, which will recursively call this:
    if (!shown()) {show(); return 1;}
    Group::handle(event); // make the child windows map first
#ifdef _WIN32
    ShowWindow(i->xid, SW_RESTORE);
#elif (defined(__APPLE__) && !USE_X11)
    if (parent()) ; // needs to update clip and redraw...
    else ShowWindow(i->xid);
#else
    XMapWindow(xdisplay, i->xid);
#endif
    return 1;

  case HIDE:
    if (flags()&MODAL) modal(0, false);
    if (i) XUnmapWindow(xdisplay, i->xid);
#ifdef _WIN32
    keep_app_active();
#endif
    break;

  case PUSH:
    // If we are in a modal state, see if the user is clicking on
    // another window. If so, just raise this (the modal) window.
    if (modal()==this && fl_actual_window != this) {
      show();
      return 0;
    }
    break;
  }

  int ret = Group::handle(event); if (ret) return ret;

  // unused events can close windows or raise them:
  if (!parent()) switch (event) {
  case KEY:
  case SHORTCUT:
    // See if the user hit Escape (or whatever the shortcut is)
    // and close the window:
    if (test_shortcut()) {
      if (!event_clicks())// make repeating key not close everything
	do_callback();
      return 1;
    }
    break;
#if USE_X11
  case PUSH:
    // unused clicks raise the window.
    if (shown()) XMapRaised(xdisplay, i->xid);
#endif
  }
  return 0;
}

/*! \fn void Window::hide()
  Remove the window from the screen. If the window is already hidden
  or show() has not been called then this does nothing and is harmless.  */
  
/*! Cause the window to become visible. It is harmless to call this
    multiple times.

    For subwindows (with a parent()) this just causes the window
    to appear. Currently no guarantee about stacking order is made.

    For a outer window (one with no parent()) this causes the window
    to appear on the screen, be de-iconized, and be raised to the top.
    Depending on child_of() settings of this window and of windows
    pointing to it, and on system and window manager settings, this
    may cause other windows to also be deiconized and raised, or if this
    window is a child_of() then this window \e may remain iconized.

    <i>Window::show() is not a virtual override of Widget::show()</i>.
    You can call either one. The \e only difference is that if an outer
    window has had show() called already, Window::show() will raise
    and deiconize it, while Widget::show() will only un-hide() it,
    making it appear in the same stacking order as before but not
    changing the iconization state (on some X window managers it will
    deiconize anyway).

    The first time this is called is when the actual "system" window
    (ie the X window) is created. Before that an fltk window is simply
    an internal data structure and is not visible outside your
    program. To return to the non-system-window state call destroy()
    or ~Window(). hide() will "unmap" the system window.

    The first time show() is called on any window is when fltk will
    call fltk::open_display() and fltk::load_theme(), unless you have
    already called them. This allows these expensive operations to be
    deferred as long as possible, and allows fltk programs to be
    written that will run without an X server as long as they don't
    actually show a window.  */
void Window::show() {
  // get rid of very common user bug: forgot end():
  Group::current(0);
  // Emulate the undocumented back-compatability modal() stuff:
  if (flags()&(MODAL|NON_MODAL)) {
    child_of(first()); // this may unmap window if it changes
    if (flags()&MODAL) modal(this, false);
  }

#if (defined(__APPLE__) && !USE_X11)
  if (!parent()) {
    // bring the application to the front
    ProcessSerialNumber psn;
    OSErr err = GetCurrentProcess( &psn );
    if ( err==noErr ) SetFrontProcess( &psn );
  }
#endif

  if (!shown()) {
    // Create an all-new system window.

    // If the window was created with the xywh constructor, the visible()
    // flag was left on, turn it off:
    clear_visible();

    // open the display:
    load_theme();
    open_display();
    layout();

    // back-compatability automatic size_range() based on resizable():
    if (!parent() && !size_range_set) {
      if (resizable()) {
	// find the innermost nested resizable():
	Widget *o = resizable();
	while (o->is_group()) {
	  Widget* p = ((Group*)o)->resizable();
	  if (!p || p == o) break;
	  o = p;
	}
	int minw = w(); if (o->w() > 72) minw -= (o->w()-72);
	int minh = h(); if (o->h() > 72) minh -= (o->h()-72);
	size_range(minw, minh, 0, 0);
      } else {
	size_range(w(), h(), w(), h());
      }
    }

    // Raise any parent windows, to get around stupid window managers
    // that think "child of" means "next to" rather than "above":
    //if (child_of() && !override()) ((Window*)child_of())->show();

    create();

    // create & map child windows:
    Group::handle(SHOW);

    // We must turn this flag on first so when silly Windows calls
    // the WinProc directly it is already on:
    set_visible();

    // map the window, making it iconized if fl_show_iconic is on:
#ifdef _WIN32
    int showtype;
    if (parent())
      showtype = SW_RESTORE;
    else if (!modal() && fl_show_iconic)
      showtype = SW_SHOWMINNOACTIVE,fl_show_iconic = false;
    // If we've captured the mouse, we don't want do activate any
    // other windows from the code, or we lose the capture.
    // Also, we don't want to activate the window for tooltips.
    else if (grab() || override())
      showtype = SW_SHOWNOACTIVATE;
    else
      showtype = SW_SHOWNORMAL;
    ShowWindow(i->xid, showtype);
#elif (defined(__APPLE__) && !USE_X11)
    if (!modal() && fl_show_iconic) {
      fl_show_iconic = 0;
      CollapseWindow( i->xid, true ); // \todo Mac ; untested
    } else {
      ShowWindow(i->xid);
    }
#else
    // for X, iconic stuff was done by create()
    XMapRaised(xdisplay, i->xid);
#endif

  } else {

    // Raise+deiconize an already-made top-level window:
    if (!parent()) {
      // Raise any parent windows, to get around stupid window managers
      // that think "child of" means "next to" rather than "above":
      //if (child_of() && !override()) ((Window*)child_of())->show();
#ifdef _WIN32
      if (IsIconic(i->xid)) OpenIcon(i->xid);
      if (!grab() && !override()) BringWindowToTop(i->xid);
#elif (defined(__APPLE__) && !USE_X11)
      ShowWindow(i->xid); // does this de-iconize?
      if (!grab() && !override()) {
	BringToFront(i->xid);
	SelectWindow(i->xid);
      }
#else
      XMapRaised(xdisplay, i->xid);
#endif
    }

    // This will call handle(SHOW) if visible is false, which will
    // cause the window to be mapped. Does nothing if visible is
    // already true:
    Widget::show();
  }
}

/*! Tell the system that this window will not have an icon, it will
  dissappear and reappear when \a parent is iconized or shown, and it
  is forced to always be above \a parent. On X this is called a
  "Transient window", and Windows calls this a "overlapping child". \a
  parent is different than the parent(), which must be zero.

  Changing this value causes destroy() to be called, due to stupid
  limitations in X and Windows.

  Win32 and some X window managers have an annoying bug where calling
  show() on this will also raise the parent window to right below
  this, making many useful user interface designs impossible!

  If you want a dialog that blocks interaction with the other windows
  of your application or with all other applications, you need to look
  at exec() (or possibly fltk::modal()). */
void Window::child_of(const Window* parent) {
  if (contains(parent)) return;
  while (parent && parent->parent()) parent = parent->window();
  if (child_of_ != parent) destroy();
  child_of_ = parent;
}

/*! Same as child_of(parent); show(). */
void Window::show(const Window* parent) {
  child_of(parent);
  show();
}

/*! Simple description: the window is popped up and this function
  waits until the user closes it, this then returns a true if the user
  hit ok, false if the user cancelled or closed the window in some
  other way. During this time events to other windows in this
  application are either thrown away or redirected to this window.

  This does child_of(parent) (using fltk::first_window() if parent is
  null). It then does show() to make this window visible and raise
  it. It then uses fltk::modal(this,grab) to make all events go to
  this window, and waits until fltk::exit_modal() is called (typically
  by the window being hidden or destroyed).

  The return value is value() of the window, which is true only if
  some callback does window->set(). To use this, make an OK button
  with a callback that does this.

  If parent is null the window that last received an event is used as
  the parent. This is convenient for popups that appear in response to
  a mouse or key click.

  See fltk::modal() for what grab does. This is useful for popup menus.
*/
bool Window::exec(const Window* parent, bool grab) {
  clear_value();
  child_of(parent ? parent : first());
  Widget* saved_modal = fltk::modal(); bool saved_grab = fltk::grab();
  fltk::modal(this, grab);
  show();
  while (modal() && !exit_modal_flag()) wait();
  hide();
  modal(saved_modal, saved_grab);
  return value();
}

/*! Make the window with a normal system border and behavior, but
  place it inside the \a frame as though that was the desktop. This is
  what Windows calls "MDI". Typically the other window (which must
  already be shown) is a child window so that space can remain around
  it for a menu/tool bar.

  Notice that parent() of the window must be zero and it will remain
  zero after this is called. Fltk uses a zero parent to indicate that
  the system is managing the window.

  On systems that don't support nested desktops (i.e. X) this does
  child_of(frame) and show(), which produces an overlapping window
  that will remain above the frame window. (IMHO this is a great
  improvement over MDI!).
*/
#if defined(_WIN32) && !USE_X11
extern const Window* fl_mdi_window;
void Window::show_inside(const Window* frame) {
  fl_mdi_window = frame;
  show();
  fl_mdi_window = 0;
}
#else
void Window::show_inside(const Window* frame) {
  show(frame);
}
#endif

////////////////////////////////////////////////////////////////

/*! Indicates that a rectangular region is damaged. draw() will be
  called later with damage() set to
  fltk::DAMAGE_ALL|fltk::DAMAGE_EXPOSE and with FLTK's clipping set to
  at least the given rectangle. Normally this is called more than once
  and the clip region will be the union of all these calls. In
  addition damage from the operating system (ie from overlapping
  windows) will increase the clipping region.

  This can be used to get speed up and improve complex displays of
  many overlapping and changing objects. Even if you do nothing else
  about it, it is usually faster to do a drawing operation that is
  clipped than one that appears, so display will be faster. You can
  also check to see if anything by testing fltk::not_clipped(x,y,w,h)
  or fltk::clip_box(...) and skipping unnecessary drawing calls
  completely. Also if your normal drawing causes blinking (due to
  overlapping objects) this can make the display look much better by
  limiting the blinking to the small area that is actually changing.  */
void Widget::redraw(int X, int Y, int W, int H) {
  // go up to the window, clipping to each widget's area, quit if empty:
  Widget* window = this;
  for (;;) {
    if (X < 0) {W += X; X = 0;}
    if (Y < 0) {H += Y; Y = 0;}
    if (W > window->w()-X) W = window->w()-X;
    if (H > window->h()-Y) H = window->h()-Y;
    if (W <= 0 || H <= 0) return;
    if (window->is_window()) break;
    X += window->x();
    Y += window->y();
    window = window->parent();
    if (!window) return;
  }
  CreatedWindow* i = CreatedWindow::find((Window*)window);
  if (!i) return; // window not mapped, so ignore it
  window->damage_ |= DAMAGE_EXPOSE;
  i->expose(X, Y, W, H);
}

// Merge a rectangle into a window's expose region. If the entire
// window is damaged we switch to a DAMAGE_ALL mode which will
// avoid drawing it twice:
void CreatedWindow::expose(int X, int Y, int W, int H) {
  // Ignore if window already marked as completely damaged:
  if (window->damage() & DAMAGE_ALL) ;
  // Detect expose events that cover the entire window:
  else if (X<=0 && Y<=0 && W>=window->w() && H>=window->h()) {
    window->set_damage(DAMAGE_ALL);
  } else if (!region) {
    // create a new region:
    region = XRectangleRegion(X,Y,W,H);
  } else {
    // merge with the region:
#ifdef WIN32
    Region R = XRectangleRegion(X, Y, W, H);
    CombineRgn(region, region, R, RGN_OR);
    XDestroyRegion(R);
#elif (defined(__APPLE__) && !USE_X11)
    Region R = NewRgn(); 
    SetRectRgn(R, X, Y, X+W, Y+H);
    UnionRgn(R, region, region);
    DisposeRgn(R);
#else
    XRectangle R;
    R.x = X; R.y = Y; R.width = W; R.height = H;
    XUnionRectWithRegion(&R, region, region);
#endif
  }
  // make flush() search for this window:
  fltk::damage(1); // make flush() do something
}

void Window::flush() {
  make_current();
  unsigned char d = damage();
  if (d & ~DAMAGE_EXPOSE) {
    set_damage(d & ~DAMAGE_EXPOSE);
    draw();
  }
  if (i->region && !(d & DAMAGE_ALL)) {
    clip_region(i->region); i->region = 0;
    set_damage(DAMAGE_EXPOSE); draw();
    clip_region(0);
  }
}

////////////////////////////////////////////////////////////////

/*! Hides the window and also deletes all window system information
  about the window, and thus returns it back to the state it was in
  before the first show(). It is harmless to call this if the window
  is already destroyed.

  Subclasses can override this, if you do this you must also override
  the destructor and make it call destroy().  */
void Window::destroy() {
  CreatedWindow* x = i;
  if (!x) return;
  i = 0;

  // remove from the list of windows:
  CreatedWindow** pp = &CreatedWindow::first;
  for (; *pp != x; pp = &(*pp)->next) if (!*pp) return;
  *pp = x->next;

  // recursively remove any subwindows:
  for (CreatedWindow *x1 = CreatedWindow::first; x1;) {
    Window* subwindow = x1->window;
    if (subwindow->window() == this || subwindow->child_of() == this) {
      bool v = subwindow->visible();
      subwindow->destroy();
      if (v) subwindow->set_visible();
      x1 = CreatedWindow::first;
    } else x1 = x1->next;
  }

  // Make sure no events are sent to this window:
  if (flags()&MODAL) modal(0, false);
  throw_focus();
  clear_visible();

  x->free_gc();
  if (x->region) XDestroyRegion(x->region);
  XDestroyWindow(xdisplay, x->xid);
  delete x;
#ifdef _WIN32
  keep_app_active();
#endif
}

/*! Calls destroy(). The destructor <i>also deletes all the
  children</i>. This allows a whole tree to be deleted at once, without
  having to keep a pointer to all the children in the user code. A
  kludge has been done so the Window and all of it's children
  can be automatic (local) variables, but you must declare the
  Window first so that it is destroyed last. */
Window::~Window() {
  destroy();
}

//
// End of "$Id: Fl_Window.cxx,v 1.106 2003/11/11 07:36:31 spitzak Exp $".
//
