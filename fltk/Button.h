//
// "$Id: Button.h 5433 2006-09-16 03:00:02Z spitzak $"
//
// Push button widget
//
// Copyright 2002 by Bill Spitzak and others.
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

#ifndef fltk2_Button_h
#define fltk2_Button_h

#include "Widget.h"
#include <fltk3/Button.h>

namespace fltk {

class FL_API Button : public Widget {
protected:
  Button() {}
  
public:
  enum {HIDDEN=3}; // back-comptability value to hide the button

  bool	value() const { return ((fltk3::Button*)_p)->value(); }
  bool	value(bool v) { ((fltk3::Button*)_p)->clear_changed(); if (v==value()) return false; value(v); return true; }

#if 0
  int handle(int);
  int handle(int event, const Rectangle&);
#endif
  
  Button(int x, int y, int w, int h, const char *label=0) {
    _p = new fltk3::Button(x, y, w, h, label); 
    _p->wrapper(this);
  }

#if 0
  static NamedStyle* default_style;

  virtual void draw();
  void draw(int glyph_width) const;
#endif
  
};

}

#endif

//
// End of "$Id: Button.h 5433 2006-09-16 03:00:02Z spitzak $".
//
