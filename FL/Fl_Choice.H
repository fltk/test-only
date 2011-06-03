//
// "$Id$"
//
// Choice header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//  - FIXME: 123 - how to wrap Fl_Menu_Item?
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

#ifndef Fl_Choice_H
#define Fl_Choice_H

#include <fltk3/Choice.h>
#include "Fl_Menu_.H"

class FL_EXPORT Fl_Choice : public Fl_Menu_ {
  
protected:
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Choice, draw(), draw(), Draw)
  
public:
  
  FLTK3_WRAPPER_VCALLS_OBJECT_INT(Choice, handle(int event), handle(event), Handle)

  Fl_Choice(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::Choice(X, Y, W, H, L);
    _p->wrapper(this);
  }

  int value() const {
    return ((fltk3::Choice*)_p)->value();
  }

  int value(int v) {
    return ((fltk3::Choice*)_p)->value(v);
  }

  int value(const Fl_Menu_Item* v) {
    return ((fltk3::Choice*)_p)->value(v);
  }
  
};

#endif

//
// End of "$Id$".
//
