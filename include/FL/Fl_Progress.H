//
// "$Id$"
//
// Progress bar widget definitions.
// FLTK 123 wrapper finished
//
// Copyright 2000-2011 by Michael Sweet.
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

#ifndef _Fl_Progress_H_
#define _Fl_Progress_H_

#include <fltk3/Progress.h>
#include "Fl_Widget.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Progress, Progress)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Progress, Progress)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Progress : public Fl_Widget {

public:
  
  FLTK3_WIDGET_VCALLS(Fl_Progress, Progress)

  Fl_Progress() { }
  
  Fl_Progress(int x, int y, int w, int h, const char *l = 0) {
    _p = new fltk3::Progress(_ctor_dx(x), _ctor_dy(y), w, h, l);
    _p->wrapper(this);
  }

  void	maximum(float v) { 
    ((fltk3::Progress*)_p)->maximum(v);
  }

  float	maximum() const { 
    return ((fltk3::Progress*)_p)->maximum();
  }

  void minimum(float v) {
    ((fltk3::Progress*)_p)->minimum(v);
  }

  float	minimum() const { 
    return ((fltk3::Progress*)_p)->minimum();
  }

  void value(float v) {
    ((fltk3::Progress*)_p)->value(v);
  }

  float	value() const { 
    return ((fltk3::Progress*)_p)->value();
  }
  
};

#endif // !_Fl_Progress_H_

//
// End of "$Id$".
//
