//
// "$Id$"
//
// Value input header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Input_H
#define Fl_Value_Input_H

#include <fltk3/ValueInput.h>
#include "Fl_Valuator.H"
#include "Fl_Input.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Value_Input, ValueInput)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Value_Input, ValueInput)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Value_Input : public Fl_Valuator {
  
public:

  FLTK3_WIDGET_VCALLS(Fl_Value_Input, ValueInput)

  Fl_Value_Input() {}
  
  Fl_Value_Input(int x,int y,int w,int h,const char *l=0) {
    _p = new fltk3::ValueInput_I(_ctor_dx(x), _ctor_dy(y), w, h, l);
    _p->wrapper(this);
  }

  void soft(char s) {
    ((fltk3::ValueInput*)_p)->soft(s);
  }

  char soft() const {
    return ((fltk3::ValueInput*)_p)->soft();
  }
  
  int shortcut() const {
    return ((fltk3::ValueInput*)_p)->shortcut();
  }
  
  void shortcut(int s) {
    ((fltk3::ValueInput*)_p)->shortcut(s);
  }

  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::ValueInput*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::ValueInput*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::ValueInput*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::ValueInput*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::ValueInput*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::ValueInput*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  
  Fl_Color cursor_color() const {
    return fltk3::_3to1_color( ((fltk3::ValueInput*)_p)->cursor_color() );
  }
  
  void cursor_color(Fl_Color n) {
    ((fltk3::ValueInput*)_p)->cursor_color( fltk3::_1to3_color(n) );
  }

};

#endif

//
// End of "$Id$".
//
