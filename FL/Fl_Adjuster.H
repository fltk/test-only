//
// "$Id$"
//
// Adjuster widget header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Adjuster_H
#define Fl_Adjuster_H

#include <fltk3/Adjuster.h>
#include "Fl_Valuator.H"


class FL_EXPORT Fl_Adjuster : public Fl_Valuator {
  
protected:
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Adjuster, draw(), draw(), Draw)
  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Adjuster, handle(int event), handle(event), Handle)
    
  void value_damage() {
    ((fltk3::Adjuster*)_p)->value_damage();
  }
  
public:
  
  Fl_Adjuster() { /* empty */ }
  
  Fl_Adjuster(int x, int y, int w, int h, const char *label=0) {
    _p = new fltk3::Adjuster(x, y, w, h, label);
    _p->wrapper(this);
  }

  void soft(int s) {
    ((fltk3::Adjuster*)_p)->soft(s);
  }
  
  int soft() const {
    return ((fltk3::Adjuster*)_p)->soft();
  }
};

#endif

//
// End of "$Id$".
//
