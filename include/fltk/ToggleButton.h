//
// "$Id$"
//
// Button that clicks on and off. You get the state with value().
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef fltk_ToggleButton_h
#define fltk_ToggleButton_h


#include <fltk3/ToggleButton.h>
#include "Button.h"


FLTK2_WRAPPER_INTERFACE_BEGIN(ToggleButton, ToggleButton)
FLTK2_WRAPPER_INTERFACE_WIDGET(ToggleButton, ToggleButton)
FLTK2_WRAPPER_INTERFACE_END()


namespace fltk {
  
  class ToggleButton : public Button {
  public:
    FLTK2_WIDGET_VCALLS(ToggleButton, ToggleButton)
    
    ToggleButton() { /* empty */ }
    
    ToggleButton(int x, int y, int w, int h, const char *label=0) {
      _p = new fltk3::ToggleButton_I(x, y, w, h, label);
      _p->wrapper(this);
    }
  };
  
}

#endif

//
// End of "$Id$".
//
