//
// "$Id: Fl_Tooltip.h,v 1.5 2002/09/23 07:15:22 spitzak Exp $"
//
// Tooltip definitions for the Fast Light Tool Kit (FLTK).
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

#ifndef _FL_TOOLTIP_H_
#define _FL_TOOLTIP_H_

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>

class FL_API Fl_Tooltip {
  static Fl_Widget* widget;
public:
  static float delay() { return delay_; }
  static void delay(float f) { delay_ = f; }
  static bool enabled() { return enabled_; }
  static void enable(bool b = true) { enabled_ = b; }
  static void disable() { enabled_ = false; }

  typedef const char* (*Generator)(Fl_Widget*, void*);
  static void enter(Fl_Widget* w, int X, int Y, int W, int H, Generator, void* = 0);
  static void enter(Fl_Widget* w, int X, int Y, int W, int H, const char* t)
    { enter(w, X, Y, W, H, 0, (void*)t); }
  static void enter(Fl_Widget* w);
  static void exit();
  static Fl_Widget* current() {return widget;}
  static void current(Fl_Widget*);

  static Fl_Named_Style* default_style;
  static Fl_Font font()		{ return default_style->label_font; }
  static void font(Fl_Font i)	{ default_style->label_font = i; }
  static unsigned size()	{ return default_style->label_size; }
  static void size(unsigned s)	{ default_style->label_size = s; }
  static void color(Fl_Color c)	{ default_style->color = c; }
  static Fl_Color color()	{ return default_style->color; }
  static void textcolor(Fl_Color c) {default_style->label_color = c; }
  static Fl_Color textcolor()	{ return default_style->label_color; }
  static void boxtype(Fl_Boxtype b) {default_style->box = b; }
  static Fl_Boxtype boxtype()	{ return default_style->box; }

private:
  static float delay_;
  static bool enabled_;
};

#endif

//
// End of "$Id: Fl_Tooltip.h,v 1.5 2002/09/23 07:15:22 spitzak Exp $".
//
