//
// "$Id$"
//
// Secret input header file for the Fast Light Tool Kit (FLTK).
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

/* \file
 fltk3::SecretInput widget . */

#ifndef Fltk3_Secret_Input_H
#define Fltk3_Secret_Input_H

#include "Input.h"

namespace fltk3 {
  
  /**
   The fltk3::SecretInput class is a subclass of fltk3::Input that displays its
   input as a string of placeholders. Depending on the platform this
   placeholder is either the asterisk ('*') or the Unicode bullet
   character (U+2022).
   
   This subclass is usually used to receive passwords and other "secret" information.
   */
  class FLTK3_EXPORT SecretInput : public fltk3::Input {
  public:
    /**
     Creates a new fltk3::SecretInput widget using the given
     position, size, and label string. The default boxtype is fltk3::DOWN_BOX.
     
     Inherited destructor destroys the widget and any value associated with it.
     */
#if defined(FL_DLL)	// implementation in src/Fl_Input.cxx
    SecretInput(int X,int Y,int W,int H,const char *l = 0);
#else
    SecretInput(int X,int Y,int W,int H,const char *l = 0)
    : fltk3::Input(X,Y,W,H,l) {type(fltk3::SECRET_INPUT);}
#endif
  };
  
}

#endif

//
// End of "$Id$".
//
