//
// "$Id: Fl_Tooltip.H,v 1.8 1999/09/19 04:58:56 vincent Exp $"
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

FL_EXPORT FL_IMPORT extern void (*fl_tooltip_enter)(Fl_Widget *);
FL_EXPORT FL_IMPORT extern void (*fl_tooltip_exit)(Fl_Widget *);

FL_EXPORT class Fl_Tooltip {
public:
  FL_IMPORT static float delay() { return delay_; }
  FL_IMPORT static void delay(float f) { delay_ = (f < 0.1) ? 0.1 : f; }
  FL_IMPORT static int enabled() { return enabled_; }
  FL_IMPORT static void enable(int b = 1) { enabled_ = b; }
  FL_IMPORT static void disable() { enable(0); }

  // This function is called by widgets
  // when the pointer enters them
  FL_IMPORT static void enter(Fl_Widget *w) {fl_tooltip_enter(w);}

  // This function must be called when
  // an event != FL_MOVE has occured in
  // the widget
  FL_IMPORT static inline void exit(Fl_Widget *w) {fl_tooltip_exit(w);}

  FL_IMPORT static void tooltip_enter(Fl_Widget *);
  FL_IMPORT static void tooltip_exit(Fl_Widget *);

  FL_IMPORT static Fl_Style default_style;
  FL_IMPORT static Fl_Font font() { return default_style.label_font; }
  FL_IMPORT static unsigned size() { return default_style.label_size; }
  FL_IMPORT static void font(Fl_Font i) { default_style.label_font = i; }
  FL_IMPORT static void size(unsigned s) { default_style.label_size = s; }
  FL_IMPORT static void color(Fl_Color c) { default_style.color = c; }
  FL_IMPORT static Fl_Color color() { return default_style.color; }
  FL_IMPORT static void textcolor(Fl_Color c) { default_style.label_color = c; }
  FL_IMPORT static Fl_Color textcolor() { return default_style.label_color; }
  FL_IMPORT static void boxtype(Fl_Boxtype b) { default_style.box = b; }
  FL_IMPORT static Fl_Boxtype boxtype() { return default_style.box; }

private:
  FL_IMPORT static float delay_;
  FL_IMPORT static int enabled_;
};

#endif

//
// End of "$Id: Fl_Tooltip.H,v 1.8 1999/09/19 04:58:56 vincent Exp $".
//
