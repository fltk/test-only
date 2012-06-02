//
// "$Id$"
//
// Button with a circle indicator to it's left, turning it on turns
// off all other radio buttons in the same Group.
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

#ifndef fltk_RadioButton_h
#define fltk_RadioButton_h


#include <fltk3/RadioButton.h>
#include "CheckButton.h"


FLTK2_WRAPPER_INTERFACE_BEGIN(RadioButton, RadioButton)
FLTK2_WRAPPER_INTERFACE_WIDGET(RadioButton, RadioButton)
FLTK2_WRAPPER_INTERFACE_END()


namespace fltk {
  
  class FL_API RadioButton : public CheckButton {
    
  public:
    
    FLTK2_WIDGET_VCALLS(RadioButton, RadioButton)
    
    RadioButton() { /* empty */ }
    
    RadioButton(int x, int y, int w, int h, const char *label=0) {
      _p = new fltk3::RadioButton_I(x, y, w, h, label);
      _p->wrapper(this);
    }
    
#if 0
    static NamedStyle* default_style;
#endif
    
  };
  
}

#endif

//
// End of "$Id$".
//
