//
// "$Id$"
//
// Button header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper complete
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef Fl_Button_H
#define Fl_Button_H

#include <fltk3/Button.h>
#include "Fl_Widget.H"

#define FL_NORMAL_BUTTON	0
#define FL_TOGGLE_BUTTON	1
#define FL_RADIO_BUTTON		(FL_RESERVED_TYPE+2)
#define FL_HIDDEN_BUTTON	3

// FIXME: 123 extern FL_EXPORT Fl_Shortcut fl_old_shortcut(const char*);

class Fl_Widget_Tracker;


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Button, Button)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Button, Button)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Button : public Fl_Widget {

protected:
  
  void simulate_key_action() {
    ((fltk3::Button_I*)_p)->simulate_key_action();
  }
  
public:

  FLTK3_WIDGET_VCALLS(Button)
  
  Fl_Button() {}

  Fl_Button(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::Button_I(x, y, w, h, label);
    _p->wrapper(this);
  }

  int value(int v) {
    return ((fltk3::Button*)_p)->value(v);
  }
  
  char value() const {
    return ((fltk3::Button*)_p)->value();
  }
  
  int set() {
    return ((fltk3::Button*)_p)->set();
  }
  
  int clear() {
    return ((fltk3::Button*)_p)->clear();
  }
  
  void setonly() {
    ((fltk3::Button*)_p)->setonly();
  }
  
  int shortcut() const {
    return ((fltk3::Button*)_p)->shortcut();
  }
  
  void shortcut(int s) {
    ((fltk3::Button*)_p)->shortcut(s);
  }
  
  Fl_Boxtype down_box() const {
    return fltk3::_3to1_boxtype( ((fltk3::Button*)_p)->down_box() );
  }
  
  void down_box(Fl_Boxtype b) {
    ((fltk3::Button*)_p)->down_box(fltk3::_1to3_boxtype(b) );
  }
  
  void shortcut(const char *s) {
    ((fltk3::Button*)_p)->shortcut(s);
  }
  
  Fl_Color down_color() const {
    return fltk3::_3to1_color( ((fltk3::Button*)_p)->down_color() );
  }
  
  void down_color(unsigned c) {
    ((fltk3::Button*)_p)->down_color( fltk3::_1to3_color(c) );
  }
  
};

#endif

//
// End of "$Id$".
//
