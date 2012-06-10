//
// "$Id$"
//
// GIF image header file for the Fast Light Tool Kit (FLTK).
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
//     http://www.fltk.org/str.php
//

/* \file
   Fl_GIF_Image widget . */

#ifndef Fl_GIF_Image_H
#define Fl_GIF_Image_H

#include "Fl_Pixmap.H"
#include <fltk3/GIFImage.h>

FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_GIF_Image, GIFImage)
FLTK3_WRAPPER_INTERFACE_IMAGE(Fl_GIF_Image, GIFImage)
FLTK3_WRAPPER_INTERFACE_END()

/**
  The Fl_GIF_Image class supports loading, caching,
  and drawing of Compuserve GIF<SUP>SM</SUP> images. The class
  loads the first image and supports transparency.
*/
class FL_EXPORT Fl_GIF_Image : public Fl_Pixmap {

public:
  
  FLTK3_IMAGE_VCALLS(Fl_GIF_Image, GIFImage)

  Fl_GIF_Image(const char* filename) {
    _p = new fltk3::GIFImage_I(filename);
    _p->wrapper(this);
  }
  
};

#endif

//
// End of "$Id$".
//
