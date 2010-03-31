//
// "$Id$"
//
// Window widget class for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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
#include "config.h"
#include <fltk3/run.h>
#include <fltk3/x.H>
#include <fltk3/Window.h>
#include <stdlib.h>
#include "flstring.h"

#ifdef __APPLE_QUARTZ__
#include <fltk3/fl_draw.H>
#endif

void fltk3::Window::_Fl_Window() {
  type(FL_WINDOW);
  box(fltk3::FLAT_BOX);
  if (fltk3::scheme_bg_) {
    labeltype(fltk3::NORMAL_LABEL);
    align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
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
  cursor_default = FL_CURSOR_DEFAULT;
  cursor_fg      = FL_BLACK;
  cursor_bg      = FL_WHITE;

  _Fl_Window();
  set_flag(FORCE_POSITION);
}

fltk3::Window::Window(int W, int H, const char *l)
// fix common user error of a missing end() with current(0):
: fltk3::Group((fltk3::Group::current(0),0), 0, W, H, l) {
  cursor_default = FL_CURSOR_DEFAULT;
  cursor_fg      = FL_BLACK;
  cursor_bg      = FL_WHITE;

  _Fl_Window();
  clear_visible();
}

fltk3::Window *fltk3::Widget::window() const {
  for (fltk3::Widget *o = parent(); o; o = o->parent())
    if (o->type() >= FL_WINDOW) return (fltk3::Window*)o;
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

  if (damage() & ~FL_DAMAGE_CHILD) {	 // draw the entire thing
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
    Fl_Color c[4] = {
      color(),
      fl_color_average(color(), FL_WHITE, 0.7f),
      fl_color_average(color(), FL_BLACK, 0.6f),
      fl_color_average(color(), FL_BLACK, 0.8f),
    };
    int i;
    for (i=dx; i<12; i++) {
      fl_color(c[i&3]);
      fl_line(x1--, y1, x2, y2--);
    }
  }
#endif

# if defined(USE_CAIRO)
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


//
// End of "$Id$".
//
