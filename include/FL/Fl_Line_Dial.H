//
// "$Id$"
//
// Line dial header file for the Fast Light Tool Kit (FLTK).
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


#ifndef Fl_Line_Dial_H
#define Fl_Line_Dial_H

#include <fltk3/LineDial.h>
#include "Fl_Dial.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Line_Dial, LineDial)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Line_Dial, LineDial)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Line_Dial : public Fl_Dial {
  
public:
  
  FLTK3_WIDGET_VCALLS(Fl_Line_Dial, LineDial)

  Fl_Line_Dial() { /* empty */ }
  
  Fl_Line_Dial(int x,int y,int w,int h, const char *l = 0) {
    _p = new fltk3::LineDial_I(_ctor_dx(x), _ctor_dy(y), w, h, l);
    _p->wrapper(this);
  }
  
};


#endif

//
// End of "$Id$".
//
