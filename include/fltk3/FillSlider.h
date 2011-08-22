//
// "$Id: FillSlider.h 8022 2010-12-12 23:21:03Z AlbrechtS $"
//
// Filled slider header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::FillSlider widget . */

#ifndef Fltk3_Fill_Slider_H
#define Fltk3_Fill_Slider_H

#include "Slider.h"

namespace fltk3 {
  
  /** Widget that draws a filled horizontal  slider,  useful as a progress or value meter*/
  class FLTK3_EXPORT FillSlider : public fltk3::Slider {
  public:
    /** Creates the slider from its position,size and optional title. */
    FillSlider(int x,int y,int w,int h,const char *l=0)
    : fltk3::Slider(x,y,w,h,l) {type(fltk3::VERT_FILL_SLIDER);}
  };
  
}

#endif

//
// End of "$Id: FillSlider.h 8022 2010-12-12 23:21:03Z AlbrechtS $".
//
