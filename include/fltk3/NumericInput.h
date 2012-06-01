//
// "$Id$"
//
// One-line text input field, which handles up/down arrows to
// change the digit to the right of the cursor. This still allows
// arbitrary text such as a math expression to be typed, if you
// want to restrict the user to a number use FloatInput or IntInput.
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
//    http://www.fltk.org/str.php
//

/* \file
 fltk3::NumericInput widget . */


#ifndef Fltk3_Numeric_Input_H
#define Fltk3_Numeric_Input_H

#include "Input.h"

namespace fltk3 {
  
  /**
    One-line text input field, which handles up/down arrows to
    change the digit to the right of the cursor. This still allows
    arbitrary text such as a math expression to be typed, if you
    want to restrict the user to a number use FloatInput or IntInput.
   */
  class FLTK3_EXPORT NumericInput : public fltk3::Input {
  public:
    /**
     Creates a new fltk3::NumericInput widget using the given position,
     size, and label string. The default boxtype is fltk3::DOWN_BOX.
     
     Inherited destructor destroys the widget and any value associated with it
     FIXME: this is not yet implemented! Please copy code from FLTK2!
     */
    NumericInput(int X,int Y,int W,int H,const char *l = 0)
    : fltk3::Input(X,Y,W,H,l) {type(fltk3::FLOAT_INPUT);}
  };
  
}

#endif

//
// End of "$Id$".
//
