//
// "$Id: Fl_Window.cxx,v 1.25 1999/10/27 08:41:00 bill Exp $"
//
// Window widget class for the Fast Light Tool Kit (FLTK).
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

// The Fl_Window is a window in the fltk library.
// This is the system-independent portions.  The huge amount of 
// crap you need to do to communicate with X is in Fl_x.C, the
// equivalent (but totally different) crap for MSWindows is in Fl_win32.C

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/conf.h>
#include <config.h>

Fl_Style Fl_Window::default_style = {
  FL_FLAT_BOX	// box
  // rest is zero
};

static Fl_Style_Definer x("window", Fl_Window::default_style);

void Fl_Window::_Fl_Window() {
  style(default_style);
  type(FL_WINDOW);
  i = 0;
  xclass_ = 0;
  icon_ = 0;
  iconlabel_ = 0;
  resizable(0);
  size_range_set = 0;
  callback((Fl_Callback*)default_callback);
}

Fl_Window::Fl_Window(int X,int Y,int W, int H, const char *l)
: Fl_Group(X, Y, W, H, l) {
  _Fl_Window();
  set_flag(FL_FORCE_POSITION);
}

Fl_Window::Fl_Window(int W, int H, const char *l)
// fix common user error of a missing end() with current(0):
  : Fl_Group((Fl_Group::current(0),0), 0, W, H, l) {
  _Fl_Window();
  clear_visible();
}

Fl_Window *Fl_Widget::window() const {
  for (Fl_Widget *o = parent(); o; o = o->parent())
    if (o->type() >= FL_WINDOW) return (Fl_Window*)o;
  return 0;
}

int Fl_Window::x_root() const {
  Fl_Window *p = window();
  if (p) return p->x_root() + x();
  return x();
}

int Fl_Window::y_root() const {
  Fl_Window *p = window();
  if (p) return p->y_root() + y();
  return y();
}

void Fl_Window::draw() {
  int savex = x(); x(0);
  int savey = y(); y(0);
  Fl_Group::draw();
  y(savey);
  x(savex);
}

void Fl_Window::draw_n_clip() {
}

void Fl_Window::label(const char *name) {label(name, iconlabel());}

void Fl_Window::iconlabel(const char *iname) {label(label(), iname);}

void Fl_Window::default_callback(Fl_Window* window, void* v) {
  window->hide();
  Fl_Widget::default_callback(window, v); // put on Fl::read_queue()
}

//
// End of "$Id: Fl_Window.cxx,v 1.25 1999/10/27 08:41:00 bill Exp $".
//
