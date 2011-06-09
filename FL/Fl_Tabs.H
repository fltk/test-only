//
// "$Id$"
//
// Tab header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
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

#ifndef Fl_Tabs_H
#define Fl_Tabs_H

#include <fltk3/TabGroup.h>
#include "Fl_Group.H"

class FL_EXPORT Fl_Tabs : public Fl_Group {

protected:
  
  void redraw_tabs() {
    ((fltk3::TabGroup*)_p)->redraw_tabs();
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT(TabGroup, draw(), draw(), Draw)

public:
  
  FLTK3_WRAPPER_VCALLS_OBJECT_INT(TabGroup, handle(int event), handle(event), Handle)
  
  Fl_Widget *value() {
    return fltk3::_3to1_widget( ((fltk3::TabGroup*)_p)->value() );
  }
  
  int value(Fl_Widget *w) {
    return ((fltk3::TabGroup*)_p)->value( fltk3::_1to3_widget(w) );
  }
  
  Fl_Widget *push() const {
    return fltk3::_3to1_widget( ((fltk3::TabGroup*)_p)->push() );
  }
  
  int push(Fl_Widget *w) {
    return ((fltk3::TabGroup*)_p)->push( fltk3::_1to3_widget(w) );
  }
  
  Fl_Tabs() {}
  
  Fl_Tabs(int x, int y, int w, int h, const char *label=0) {
    _p = new fltk3::TabGroup(x, y, w, h, label);
    _p->wrapper(this);
  }
  
  Fl_Widget *which(int event_x, int event_y) {
    return fltk3::_3to1_widget( ((fltk3::TabGroup*)_p)->which(event_x, event_y) );
  }
  
  void client_area(int &rx, int &ry, int &rw, int &rh, int tabh=0) {
    ((fltk3::TabGroup*)_p)->client_area(rx, ry, rw, rh, tabh);
  }
  
};


#endif

//
// End of "$Id$".
//
