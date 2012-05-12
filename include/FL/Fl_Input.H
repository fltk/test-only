//
// "$Id$"
//
// Input header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
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

#ifndef Fl_Input_H
#define Fl_Input_H

#include <fltk3/Input.h>
#include "Fl_Input_.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Input, Input)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Input, Input)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Input : public Fl_Input_ {
  
public:
  
  FLTK3_WIDGET_VCALLS(Fl_Input, Input)
  
  Fl_Input() { /* empty */ }
  
  Fl_Input(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::Input_I(_ctor_dx(x), _ctor_dy(y), w, h, label);
    _p->wrapper(this);
  }
};

#endif 

//
// End of "$Id$".
//
