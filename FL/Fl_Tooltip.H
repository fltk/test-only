//
// "$Id: Fl_Tooltip.H,v 1.6 1999/08/16 07:30:53 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>

FL_EXPORT extern void (*fl_tooltip_enter)(Fl_Widget *);
FL_EXPORT extern void (*fl_tooltip_exit)(Fl_Widget *);

FL_EXPORT class Fl_Tooltip {
public:
  static float delay() { return delay_; }
  static void delay(float f) { delay_ = (f < 0.1) ? 0.1 : f; }
  static bool enabled() { return enabled_; }
  static void enable(bool b = true) { enabled_ = b; }
  static void disable() { enable(false); }

  // This function is called by widgets
  // when the pointer enters them
  static void enter(Fl_Widget *w) {fl_tooltip_enter(w);}

  // This function must be called when
  // an event != FL_MOVE has occured in
  // the widget
  static inline void exit(Fl_Widget *w) {fl_tooltip_exit(w);}

  static void tooltip_enter(Fl_Widget *);
  static void tooltip_exit(Fl_Widget *);

  static Fl_Style default_style;
  static Fl_Font font() { return default_style.label_font; }
  static unsigned size() { return default_style.label_size; }
  static void font(Fl_Font i) { default_style.label_font = i; }
  static void size(unsigned s) { default_style.label_size = s; }
  static void color(Fl_Color c) { default_style.color = c; }
  static Fl_Color color() { return default_style.color; }
  static void textcolor(Fl_Color c) { default_style.label_color = c; }
  static Fl_Color textcolor() { return default_style.label_color; }
  static void boxtype(Fl_Boxtype b) { default_style.box = b; }
  static Fl_Boxtype boxtype() { return default_style.box; }

private:
  static float delay_;
  static bool enabled_;
};

#endif

//
// End of "$Id: Fl_Tooltip.H,v 1.6 1999/08/16 07:30:53 bill Exp $".
//
