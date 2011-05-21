//
// "$Id$"
//
// Window widget class for the Fast Light Tool Kit (FLTK).
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

// The fltk3::Window is a window in the fltk library.
// This is the system-independent portions.  The huge amount of 
// crap you need to do to communicate with X is in Fl_x.cxx, the
// equivalent (but totally different) crap for MSWindows is in Fl_win32.cxx
#include <config.h>
#include <fltk3/run.h>
#include <fltk3/x.h>
#include <fltk3/Window.h>
#include <stdlib.h>
#include "flstring.h"

#ifdef __APPLE_QUARTZ__
#include <fltk3/draw.h>
#endif

char *fltk3::Window::default_xclass_ = 0L;

void fltk3::Window::_Fl_Window() {
  type(fltk3::WINDOW);
  box(fltk3::FLAT_BOX);
  if (fltk3::scheme_bg_) {
    labeltype(fltk3::NORMAL_LABEL);
    align(fltk3::ALIGN_CENTER | fltk3::ALIGN_INSIDE | fltk3::ALIGN_CLIP);
    image(fltk3::scheme_bg_);
  } else {
    labeltype(fltk3::NO_LABEL);
  }
  i = 0;
  xclass_ = 0;
  icon_ = 0;
  iconlabel_ = 0;
  resizable(0);
  size_range_set = 0;
  minw = maxw = minh = maxh = 0;
  callback((fltk3::Callback*)default_callback);
}

fltk3::Window::Window(int X,int Y,int W, int H, const char *l)
: fltk3::Group(X, Y, W, H, l) {
  cursor_default = fltk3::CURSOR_DEFAULT;
  cursor_fg      = fltk3::BLACK;
  cursor_bg      = fltk3::WHITE;

  _Fl_Window();
  set_flag(FORCE_POSITION);
}

fltk3::Window::Window(int W, int H, const char *l)
// fix common user error of a missing end() with current(0):
  : fltk3::Group((fltk3::Group::current(0),0), 0, W, H, l) {
  cursor_default = fltk3::CURSOR_DEFAULT;
  cursor_fg      = fltk3::BLACK;
  cursor_bg      = fltk3::WHITE;

  _Fl_Window();
  clear_visible();
}

fltk3::Window *fltk3::Widget::window() const {
  for (fltk3::Widget *o = parent(); o; o = o->parent())
    if (o->type() >= fltk3::WINDOW) return (fltk3::Window*)o;
  return 0;
}
/** Gets the x position of the window on the screen */
int fltk3::Window::x_root() const {
  fltk3::Window *p = window();
  if (p) return p->x_root() + x();
  return x();
}
/** Gets the y position of the window on the screen */
int fltk3::Window::y_root() const {
  fltk3::Window *p = window();
  if (p) return p->y_root() + y();
  return y();
}

void fltk3::Window::draw() {

  // The following is similar to fltk3::Group::draw(), but ...
  //  - we draw the box with x=0 and y=0 instead of x() and y()
  //  - we don't draw a label

  if (damage() & ~fltk3::DAMAGE_CHILD) {	 // draw the entire thing
    draw_box(box(),0,0,w(),h(),color()); // draw box with x/y = 0
  }
  draw_children();

#ifdef __APPLE_QUARTZ__
  // on OS X, windows have no frame. To resize a window, we drag the lower right
  // corner. This code draws a little ribbed triangle for dragging.
  extern CGContextRef fl_gc;
  if (fl_gc && !parent() && resizable() && (!size_range_set || minh!=maxh || minw!=maxw)) {
    int dx = fltk3::box_dw(box())-fltk3::box_dx(box());
    int dy = fltk3::box_dh(box())-fltk3::box_dy(box());
    if (dx<=0) dx = 1;
    if (dy<=0) dy = 1;
    int x1 = w()-dx-1, x2 = x1, y1 = h()-dx-1, y2 = y1;
    fltk3::Color c[4] = {
      color(),
      fltk3::color_average(color(), fltk3::WHITE, 0.7f),
      fltk3::color_average(color(), fltk3::BLACK, 0.6f),
      fltk3::color_average(color(), fltk3::BLACK, 0.8f),
    };
    int i;
    for (i=dx; i<12; i++) {
      fltk3::color(c[i&3]);
      fltk3::line(x1--, y1, x2, y2--);
    }
  }
#endif

# if defined(FLTK_USE_CAIRO)
  fltk3::cairo_make_current(this); // checkout if an update is necessary
# endif
}

void fltk3::Window::label(const char *name) {
  label(name, iconlabel());
}

void fltk3::Window::copy_label(const char *a) {
  if (flags() & COPIED_LABEL) {
    free((void *)label());
    clear_flag(COPIED_LABEL);
  }
  if (a) a = strdup(a);
  label(a, iconlabel());
  set_flag(COPIED_LABEL);
}


void fltk3::Window::iconlabel(const char *iname) {
  label(label(), iname);
}

// the fltk3::atclose pointer is provided for back compatibility.  You
// can now just change the callback for the window instead.

/** Default callback for window widgets. It hides the window and then calls the default widget callback. */
void fltk3::default_atclose(fltk3::Window* window, void* v) {
  window->hide();
  fltk3::Widget::default_callback(window, v); // put on fltk3::read_queue()
}
/** Back compatibility: default window callback handler \see fltk3::set_atclose() */
void (*fltk3::atclose)(fltk3::Window*, void*) = default_atclose;
/** Back compatibility: Sets the default callback v for win to call on close event */
void fltk3::Window::default_callback(fltk3::Window* win, void* v) {
  fltk3::atclose(win, v);
}

/**  Returns the last window that was made current. \see fltk3::Window::make_current() */
fltk3::Window *fltk3::Window::current() {
  return current_;
}

/** Returns the default xclass.

  \see fltk3::Window::default_xclass(const char *)

 */
const char *fltk3::Window::default_xclass()
{
  if (default_xclass_) {
    return default_xclass_;
  } else {
    return "FLTK";
  }
}

/** Sets the default window xclass.

  The default xclass is used for all windows that don't have their
  own xclass set before show() is called. You can change the default
  xclass whenever you want, but this only affects windows that are
  created (and shown) after this call.

  The given string \p xc is copied. You can use a local variable or
  free the string immediately after this call.

  If you don't call this, the default xclass for all windows will be "FLTK".
  You can reset the default xclass by specifying NULL for \p xc.

  If you call fltk3::Window::xclass(const char *) for any window, then
  this also sets the default xclass, unless it has been set before.

  \param[in] xc default xclass for all windows subsequently created

  \see fltk3::Window::xclass(const char *)
*/
void fltk3::Window::default_xclass(const char *xc)
{
  if (default_xclass_) {
    free(default_xclass_);
    default_xclass_ = 0L;
  }
  if (xc) {
    default_xclass_ = strdup(xc);
  }
}

/** Sets the xclass for this window.

  A string used to tell the system what type of window this is. Mostly
  this identifies the picture to draw in the icon. This only works if
  called \e before calling show().

  <I>Under X</I>, this is turned into a XA_WM_CLASS pair by truncating at
  the first non-alphanumeric character and capitalizing the first character,
  and the second one if the first is 'x'.  Thus "foo" turns into "foo, Foo",
  and "xprog.1" turns into "xprog, XProg".

  <I>Under Microsoft Windows</I>, this string is used as the name of the
  WNDCLASS structure, though it is not clear if this can have any
  visible effect.

  \since FLTK 1.3 the passed string is copied. You can use a local
  variable or free the string immediately after this call. Note that
  FLTK 1.1 stores the \e pointer without copying the string.

  If the default xclass has not yet been set, this also sets the
  default xclass for all windows created subsequently.

  \see fltk3::Window::default_xclass(const char *)
*/
void fltk3::Window::xclass(const char *xc) 
{
  if (xclass_) {
    free(xclass_);
    xclass_ = 0L;
  }
  if (xc) {
    xclass_ = strdup(xc);
    if (!default_xclass_) {
      default_xclass(xc);
    }
  }
}

/** Returns the xclass for this window, or a default.

  \see fltk3::Window::default_xclass(const char *)
  \see fltk3::Window::xclass(const char *)
*/
const char *fltk3::Window::xclass() const
{
  if (xclass_) {
    return xclass_;
  } else {
    return default_xclass();
  }
}

/** Gets the current icon window target dependent data. */
const void *fltk3::Window::icon() const {
  return icon_;
}

/** Sets the current icon window target dependent data. */
void fltk3::Window::icon(const void * ic) {
  icon_ = ic;
}


//
// End of "$Id$".
//
