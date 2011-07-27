//
// "$Id: IntInput.h 8022 2010-12-12 23:21:03Z AlbrechtS $"
//
// Integer input header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::IntInput widget . */

#ifndef Fltk3_Int_Input_H
#define Fltk3_Int_Input_H

#include "Input.h"

namespace fltk3 {
  
  /**
   The fltk3::IntInput class is a subclass of fltk3::Input that only allows
   the user to type decimal digits (or hex numbers of the form 0xaef).
   */
  class FLTK3_EXPORT IntInput : public fltk3::Input {
  public:
    /**
     Creates a new fltk3::IntInput widget using the given position,
     size, and label string. The default boxtype is fltk3::DOWN_BOX.
     
     Inherited destructor Destroys the widget and any value associated with it.
     */
#if defined(FL_DLL)	// implementation in src/Fl_Input.cxx
    IntInput(int X,int Y,int W,int H,const char *l = 0);
#else
    IntInput(int X,int Y,int W,int H,const char *l = 0)
    : fltk3::Input(X,Y,W,H,l) {type(fltk3::INT_INPUT);}
#endif
  };
  
}

#endif

//
// End of "$Id: IntInput.h 8022 2010-12-12 23:21:03Z AlbrechtS $".
//
