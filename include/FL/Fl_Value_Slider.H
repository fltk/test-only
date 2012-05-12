//
// "$Id$"
//
// Value slider header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Slider_H
#define Fl_Value_Slider_H

#include <fltk3/ValueSlider.h>
#include "Fl_Slider.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Value_Slider, ValueSlider)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Value_Slider, ValueSlider)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Value_Slider : public Fl_Slider {

public:
  
  FLTK3_WIDGET_VCALLS(Fl_Value_Slider, ValueSlider)

  Fl_Value_Slider() {}
  
  Fl_Value_Slider(int x,int y,int w,int h, const char *l = 0) {
    _p = new fltk3::ValueSlider_I(_ctor_dx(x), _ctor_dy(y), w, h, l);
    _p->wrapper(this);
  }
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::ValueSlider*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::ValueSlider*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::ValueSlider*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::ValueSlider*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::ValueSlider*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::ValueSlider*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  
};


#endif

//
// End of "$Id$".
//
