//
// "$Id$"
//
// An input/chooser widget.
//            ______________  ____
//           |              || __ |
//           | input area   || \/ |
//           |______________||____|
// FLTK 123 wrapper started
//  - Fl_Menu_Item not wrapped yet
//
// Copyright 1998-2011 by Bill Spitzak and others.
// Copyright 2004 by Greg Ercolano.
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

#ifndef Fl_Input_Choice_H
#define Fl_Input_Choice_H

#include <fltk3/InputChoice.h>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>
#include <string.h>


class FL_EXPORT Fl_Input_Choice : public Fl_Group {

public:

  Fl_Input_Choice() { /* empty */ }
  
  Fl_Input_Choice (int x,int y,int w,int h,const char*l=0) {
    _p = new fltk3::InputChoice(x, y, w, h, l);
    _p->wrapper(this);
  }

  void add(const char *s) {
    ((fltk3::InputChoice*)_p)->add(s);
  }
  
  int changed() const {
    return ((fltk3::InputChoice*)_p)->changed();
  }
  
  void clear_changed() {
    ((fltk3::InputChoice*)_p)->clear_changed();
  }
  
  void set_changed() { 
    ((fltk3::InputChoice*)_p)->set_changed();
  }

  void clear() { 
    ((fltk3::InputChoice*)_p)->clear();
  }
  
  Fl_Boxtype down_box() const { 
    return fltk3::_3to1_boxtype( ((fltk3::InputChoice*)_p)->down_box() );
  }

  // FIXME: 123
  void down_box(Fl_Boxtype b) {
    ((fltk3::InputChoice*)_p)->down_box( fltk3::_1to3_boxtype(b) );
  }

  // FIXME: 123
  const Fl_Menu_Item *menu() { return (menu_->menu()); }
  
  // FIXME: 123
  void menu(const Fl_Menu_Item *m) { menu_->menu(m); }
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Widget, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::InputChoice*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::InputChoice*)_p)->font( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::InputChoice*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::InputChoice*)_p)->textsize( fltk3::_1to3_fontsize(size); )
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::InputChoice*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::InputChoice*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  
  const char* value() const { 
    return ((fltk3::InputChoice*)_p)->value();
  }
  
  void value(const char *val) { 
    ((fltk3::InputChoice*)_p)->val();
  }
  
  void value(int val) {
    ((fltk3::InputChoice*)_p)->value(val);
  }
  
  // FIXME: 123
  Fl_Menu_Button *menubutton();
  
  // FIXME: 123
  Fl_Input *input();
  
};

#endif // !Fl_Input_Choice_H

//
// End of "$Id$".
//
