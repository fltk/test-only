//
// "$Id: FloatInput.h 8132 2010-12-28 18:33:46Z yuri $"
//
// Floating point input header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::FloatInput widget . */


#ifndef Fltk3_Float_Input_H
#define Fltk3_Float_Input_H

#include "Input.h"

namespace fltk3 {
  
  /**
   The fltk3::FloatInput class is a subclass of fltk3::Input
   that only allows the user to type floating point numbers (sign,
   digits, decimal point, more digits, 'E' or 'e', sign, digits).
   */
  class FloatInput : public fltk3::Input { // don't use FLTK3_EXPORT here !
  public:
    /**
     Creates a new fltk3::FloatInput widget using the given position,
     size, and label string. The default boxtype is fltk3::DOWN_BOX.
     <P> Inherited destructor destroys the widget and any value associated with it
     */
    FloatInput(int X,int Y,int W,int H,const char *l = 0)
    : fltk3::Input(X,Y,W,H,l) {type(fltk3::FLOAT_INPUT);}
  };
  
}

#endif

//
// End of "$Id: FloatInput.h 8132 2010-12-28 18:33:46Z yuri $".
//
