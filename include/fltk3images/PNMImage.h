//
// "$Id$"
//
// PNM image header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
 fltk3::PNMImage class . */

#ifndef Fltk3_PNM_Image_H
#define Fltk3_PNM_Image_H
#  include <fltk3/Image.h>

namespace fltk3 {
  
  /**
   The fltk3::PNMImage class supports loading, caching,
   and drawing of Portable Anymap (PNM, PBM, PGM, PPM) image files. The class
   loads bitmap, grayscale, and full-color images in both ASCII and
   binary formats.
   */
  class FLTK3_EXPORT PNMImage : public fltk3::RGBImage {
    
  protected:
    
    PNMImage(const uchar *a, int b, int c, int d=3, int e=0) : RGBImage(a, b, c, d, e) {}

  public:
    
    PNMImage(const char* filename);
  };
  
}

#endif

//
// End of "$Id$".
//
