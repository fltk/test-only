//
// "$Id: ValueSlider.h 4886 2006-03-30 09:55:32Z fabien $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fltk2_Value_Slider_h
#define fltk2_Value_Slider_h

#include <fltk3/ValueSlider.h>

#include "Slider.h"

namespace fltk {

class FL_API ValueSlider : public Slider {
protected:
  ValueSlider() {}
public:
  // fltk123: FloatInput input;
  // fltk123: int handle(int);
  // fltk123: void draw();
  ValueSlider(int x,int y,int w,int h, const char *l = 0) {
    _p = new fltk3::ValueSlider(x, y, w, h, l);
    _p->wrapper(this);
    _p->type(FL_HOR_SLIDER); // defaults to horizontal
  }
  // fltk123: ~ValueSlider();
  // fltk123: void layout();
  // fltk123: virtual void value_damage(); // cause damage() due to value() changing
};

}
#endif

//
// End of "$Id: ValueSlider.h 4886 2006-03-30 09:55:32Z fabien $".
//
