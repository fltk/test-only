//
// "$Id$"
//
// NumericInput modified to only allow floating point to by
// typed. Currently this is implemented by the base class by checking
// type() but this may change in the future.
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

#ifndef fltk_FloatInput_h
#define fltk_FloatInput_h

#include <fltk3/FloatInput.h>
#include "NumericInput.h"


FLTK2_WRAPPER_INTERFACE_BEGIN(FloatInput, FloatInput)
FLTK2_WRAPPER_INTERFACE_WIDGET(FloatInput, FloatInput)
FLTK2_WRAPPER_INTERFACE_END()


namespace fltk {
  
  class FloatInput : public NumericInput {
    
  public:

    FLTK2_WIDGET_VCALLS(FloatInput, FloatInput)
    
    FloatInput() {}
    
    FloatInput(int x,int y,int w,int h, const char *l = 0) {
      _p = new fltk3::FloatInput_I(x, y, w, h, l);
      _p->wrapper(this);
    }

#if 0
    virtual bool replace(int, int, const char*, int);
  public:
    enum { FLOAT = 1, INT = 2 };
    FloatInput(int x,int y,int w,int h,const char *l = 0)
    : NumericInput(x,y,w,h,l) { type(FLOAT); }
    long lvalue() const;
    int ivalue() const { return int(lvalue()); }
    double fvalue() const;
#endif
    
  };
  
}

#endif

//
// End of "$Id$".
//
