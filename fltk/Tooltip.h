//
// "$Id: Tooltip.h,v 1.5 2003/11/04 08:10:57 spitzak Exp $"
//
// Code used to control the appearance of tooltips, and to pop them
// up in response to events other than the mouse entering and exiting
// a widget.
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

#ifndef fltk_Tooltip_h
#define fltk_Tooltip_h

#include "Widget.h"

namespace fltk {

class FL_API Tooltip {
  static Widget* widget;
public:
  static float delay() { return delay_; }
  static void delay(float f) { delay_ = f; }
  static bool enabled() { return enabled_; }
  static void enable(bool b = true) { enabled_ = b; }
  static void disable() { enabled_ = false; }

  typedef const char* (*Generator)(Widget*, void*);
  static void enter(Widget* w, int X, int Y, int W, int H, Generator, void* = 0);
  static void enter(Widget* w, int X, int Y, int W, int H, const char* t)
    { enter(w, X, Y, W, H, 0, (void*)t); }
  static void enter(Widget* w);
  static void exit();
  static Widget* current()	{ return widget; }
  static void current(Widget*);

  static NamedStyle* default_style;
  static Font* font()		{ return default_style->labelfont(); }
  static void font(Font* i)	{ default_style->labelfont(i); }
  static float size()		{ return default_style->labelsize(); }
  static void size(float s)	{ default_style->labelsize(s); }
  static void color(Color c)	{ default_style->color(c); }
  static Color color()		{ return default_style->color(); }
  static void textcolor(Color c){ default_style->textcolor(c); }
  static Color textcolor()	{ return default_style->textcolor(); }
  static void box(Box* b)	{ default_style->box(b); }
  static Box* box()		{ return default_style->box(); }

private:
  static float delay_;
  static bool enabled_;
};

}

#endif

//
// End of "$Id: Tooltip.h,v 1.5 2003/11/04 08:10:57 spitzak Exp $".
//
