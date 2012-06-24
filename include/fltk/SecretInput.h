//
// "$Id$"
//
// One-line text input field that draws asterisks instead of the
// letters. It also prevents the user from cutting or copying the
// text and then pasting it somewhere.
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#ifndef fltk_SecretInput_h
#define fltk_SecretInput_h

#include <fltk3/SecretInput.h>
#include "Input.h"


FLTK2_WRAPPER_INTERFACE_BEGIN(SecretInput, SecretInput)
FLTK2_WRAPPER_INTERFACE_WIDGET(SecretInput, SecretInput)
FLTK2_WRAPPER_INTERFACE_END()


namespace fltk {
  
  class SecretInput : public Input {
    
  public:
    
    FLTK2_WIDGET_VCALLS(SecretInput, SecretInput)
    
    SecretInput() {}
    
    SecretInput(int x,int y,int w,int h, const char *l = 0) {
      _p = new fltk3::SecretInput_I(x, y, w, h, l);
      _p->wrapper(this);
    }
    
  };
  
}

#endif

//
// End of "$Id$".
//
