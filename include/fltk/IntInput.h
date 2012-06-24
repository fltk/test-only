//
// "$Id$"
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

#ifndef fltk_IntInput_h
#define fltk_IntInput_h

#include <fltk3/IntInput.h>
#include "FloatInput.h"


FLTK2_WRAPPER_INTERFACE_BEGIN(IntInput, IntInput)
FLTK2_WRAPPER_INTERFACE_WIDGET(IntInput, IntInput)
FLTK2_WRAPPER_INTERFACE_END()


namespace fltk {
  
  class FL_API IntInput : public FloatInput {
    
  public:
    
    FLTK2_WIDGET_VCALLS(IntInput, IntInput)
    
    IntInput() {}
    
    IntInput(int x,int y,int w,int h, const char *l = 0) {
      _p = new fltk3::IntInput_I(x, y, w, h, l);
      _p->wrapper(this);
    }
    
  };
  
}
#endif

//
// End of "$Id$".
//
