//
// "$Id$"
//
// Tooltip header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef Fl_Tooltip_H
#define Fl_Tooltip_H

#include <fltk3/Tooltip.h>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>


class FL_EXPORT Fl_Tooltip : public fltk3::Tooltip {
  
public:

  // FIXME: 123-1 - static void (*enter)(Fl_Widget* w);
  
  static void enter_area(Fl_Widget* w, int X, int Y, int W, int H, const char* tip) {
    fltk3::Tooltip::enter_area(fltk3::_1to3_widget(w), X, Y, W, H, tip);
  }
  
  // FIXME: 123-1 - static void (*exit)(Fl_Widget *w);
  
  static Fl_Widget* current() {
    return fltk3::_3to1_widget( fltk3::Tooltip::current() );
  }
  
  static void current(Fl_Widget *w) {
    fltk3::Tooltip::current(fltk3::_1to3_widget(w));
  }

  static Fl_Font font() {
    return fltk3::_3to1_font( fltk3::Tooltip::font() );
  }

  static void font(Fl_Font i) {
    fltk3::Tooltip::font(fltk3::_1to3_font(i));
  }

  static Fl_Fontsize size() { 
    return fltk3::_3to1_fontsize( fltk3::Tooltip::size() );
  }

  static void size(Fl_Fontsize s) {
    fltk3::Tooltip::size(fltk3::_1to3_fontsize(s));
  }

  static Fl_Color color() { 
    return fltk3::_3to1_color( fltk3::Tooltip::color() );
  }

  static void color(Fl_Color c) { 
    fltk3::Tooltip::color(fltk3::_1to3_color(c));
  }

  static Fl_Color textcolor() { 
    return fltk3::_3to1_color( fltk3::Tooltip::textcolor() );
  }

  static void textcolor(Fl_Color c) { 
    fltk3::Tooltip::textcolor(fltk3::_1to3_color(c));
  }
  
#ifdef __APPLE__
  static Fl_Window* current_window(void) {
    return fltk3::_3to1_window( fltk3::Tooltip::current_window() );
  }
#endif

};

#endif

//
// End of "$Id$".
//
