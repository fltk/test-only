//
// "$Id$"
//
// Box header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper complete
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

#ifndef Fl_Box_H
#define Fl_Box_H

#include <fltk3/Box.h>
#include "Fl_Widget.H"

class FL_EXPORT Fl_Box : public Fl_Widget {

protected:
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Box, draw(), draw(), Draw)

public:
  Fl_Box(int x, int y, int w, int h, const char *label=0) {
    _p = new fltk3::Box(x, y, w, h, label);
    _p->wrapper(this);
  }
  
  Fl_Box(Fl_Boxtype b, int x, int y, int w, int h, const char *label) {
    _p = new fltk3::Box(fltk3::_1to3_boxtype(b), x, y, w, h, label);
    _p->wrapper(this);
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Box, handle(int event), handle(event), Handle)
};

#endif

//
// End of "$Id$".
//
