//
// "$Id$"
//
// Value output header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Output_H
#define Fl_Value_Output_H

#include <fltk3/ValueOutput.h>
#include "Fl_Valuator.H"


class FL_EXPORT Fl_Value_Output : public Fl_Valuator {

protected:

  FLTK3_WRAPPER_VCALLS_OBJECT(ValueOutput, draw(), draw(), Draw)

public:
  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, ValueOutput, handle(int event), handle(event), Handle)
  
  Fl_Value_Output(int x,int y,int w,int h,const char *l=0) {
    _p = new fltk3::ValueOutput(x, y, w, h, l);
    _p->wrapper(this);
  }

  void soft(char s) {
    ((fltk3::ValueOutput*)_p)->soft(s);
  }
  
  char soft() const {
    return ((fltk3::ValueOutput*)_p)->soft();
  }
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::ValueOutput*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::ValueOutput*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::ValueOutput*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::ValueOutput*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::ValueOutput*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::ValueOutput*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  
};

#endif

//
// End of "$Id$".
//
