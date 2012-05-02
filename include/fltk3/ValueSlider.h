//
// "$Id$"
//
// Value slider header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/* \file
 fltk3::ValueSlider widget . */

#ifndef Fltk3_Value_Slider_H
#define Fltk3_Value_Slider_H


#include "Slider.h"


namespace fltk3 {
  
  /**
   The fltk3::ValueSlider widget is a fltk3::Slider widget
   with a box displaying the current value.
   <P ALIGN=CENTER>\image html value_slider.png 
   \image latex  value_slider.png "fltk3::ValueSlider" width=4cm
   */
  class FLTK3_EXPORT ValueSlider : public fltk3::Slider {
    
  protected:
    void draw();
  public:
    int handle(int);
    ValueSlider(int x,int y,int w,int h, const char *l = 0);
  };
  
}

#endif

//
// End of "$Id$".
//
