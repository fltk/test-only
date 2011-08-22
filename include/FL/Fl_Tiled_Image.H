//
// "$Id$"
//
// Tiled image header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//  - TODO: 123 - virtual setup of images not yet done
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

#ifndef Fl_Tiled_Image_H
#define Fl_Tiled_Image_H

#include <fltk3/TiledImage.h>
#include "Fl_Image.H"

class FL_EXPORT Fl_Tiled_Image : public Fl_Image {

public:

  Fl_Tiled_Image(Fl_Image *i, int W = 0, int H = 0) {
    _p = new fltk3::TiledImage(fltk3::_1to3_image(i), W, H);
    _p->wrapper(this);
  }
  
#if 0
  virtual Fl_Image *copy(int W, int H);
  Fl_Image *copy() { return copy(w(), h()); }
  virtual void color_average(Fl_Color c, float i);
  virtual void desaturate();
  virtual void draw(int X, int Y, int W, int H, int cx, int cy);
  void draw(int X, int Y) { draw(X, Y, w(), h(), 0, 0); }
  /** Gets The image that is shared */ 
  Fl_Image *image() { return image_; }
#endif
  
};

#endif // !Fl_Tiled_Image_H

//
// End of "$Id$"
//
