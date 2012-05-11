//
// "$Id$"
//
// Pixmap header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Pixmap_H
#define Fl_Pixmap_H

#include <fltk3/Pixmap.h>
#include "Fl_Image.H"

FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Pixmap, Pixmap)
FLTK3_WRAPPER_INTERFACE_PIXMAP(Fl_Pixmap, Pixmap)
FLTK3_WRAPPER_INTERFACE_END()

class FL_EXPORT Fl_Pixmap : public Fl_Image {
  
protected:

  void measure() {
    ((fltk3::Pixmap_I*)_p)->measure();
  }
  
public:
  
  FLTK3_IMAGE_VCALLS(Fl_Pixmap, Pixmap)
  
  Fl_Pixmap() {}

  explicit Fl_Pixmap(char * const * D) {
    _p = new fltk3::Pixmap_I((const uchar * const *)D);
    _p->wrapper(this);
  }
  
  explicit Fl_Pixmap(uchar * const * D) {
    _p = new fltk3::Pixmap_I((const uchar * const *)D);
    _p->wrapper(this);
  }
  
  explicit Fl_Pixmap(const char * const * D) {
    _p = new fltk3::Pixmap_I((const uchar * const *)D);
    _p->wrapper(this);
  }
  
  explicit Fl_Pixmap(const uchar * const * D) {
    _p = new fltk3::Pixmap_I(D);
    _p->wrapper(this);
  }
  
  void draw(int x, int y) { 
    ((fltk3::Pixmap*)_p)->draw(x, y);
  }
};


#endif

//
// End of "$Id$".
//
