//
// "$Id: Fl_Tooltip.H,v 1.4 1999/04/11 01:18:32 carl Exp $"
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

extern void (*fl_tooltip_enter)(Fl_Widget *);
extern void (*fl_tooltip_exit)(Fl_Widget *);

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>

class Fl_TooltipBox;
class Fl_Menu_Window;

class Fl_Tooltip {
public:
  // These functions are user-called functions
  static float delay() { return default_style.delay_; }
  static void delay(float f) { default_style.delay_ = (f < 0.1) ? 0.1 : f; }
  static int font() { return default_style.textfont_; }
  static int size() { return default_style.textsize_; }
  static void font(int i) { default_style.textfont_ = i; }
  static void size(int s) { default_style.textsize_ = s; }
  static void color(uchar c) { default_style.color_ = c; }
  static uchar color() { return default_style.color_; }
  static void textcolor(uchar c) { default_style.textcolor_ = c; }
  static uchar textcolor() { return default_style.textcolor_; }
  static void boxtype(uchar b) { default_style.box_ = b; }
  static uchar boxtype() { return default_style.box_; }
  static void enable(int b = 1) { enabled = b; }
  static void disable() { enable(0); }

  // This function is called by widgets
  // when the pointer enters them
  static void enter(Fl_Widget *w) {fl_tooltip_enter(w);}

  // This function must be called when
  // an event != FL_MOVE has occured in
  // the widget
  static inline void exit(Fl_Widget *w) {fl_tooltip_exit(w);}

  static void loadstyle();

  friend class Fl_TooltipBox;
  friend class Fl_Widget;
  friend class Fl_Group;
  friend class Fl;

  struct Style {
    float delay_;
    uchar color_;
    uchar box_;
    uchar textfont_;
    uchar textsize_;
    uchar textcolor_;
  };

  static Style default_style;

private:
  static Fl_TooltipBox *box;
  static Fl_Menu_Window *window;
  static Fl_Widget *widget;
  static int shown;
  static int enabled;

  static void tooltip_timeout(Fl_Widget *);
  static void tooltip_enter(Fl_Widget *);
  static void tooltip_exit(Fl_Widget *);

};


#endif

//
// End of "$Id: Fl_Tooltip.H,v 1.4 1999/04/11 01:18:32 carl Exp $".
//
