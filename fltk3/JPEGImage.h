//
// "$Id: JPEGImage.h 7903 2010-11-28 21:06:39Z matt $"
//
// JPEG image header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::JPEGImage class . */

#ifndef Fltk3_JPEG_Image_H
#define Fltk3_JPEG_Image_H
#  include "Image.h"


namespace fltk3 {
  
  /**
   The fltk3::JPEGImage class supports loading, caching,
   and drawing of Joint Photographic Experts Group (JPEG) File
   Interchange Format (JFIF) images. The class supports grayscale
   and color (RGB) JPEG image files.
   */
  class FLTK3_EXPORT JPEGImage : public fltk3::RGBImage {
    
  public:
    
    JPEGImage(const char *filename);
    JPEGImage(const char *name, const unsigned char *data);
  };
  
}

#endif

//
// End of "$Id: JPEGImage.h 7903 2010-11-28 21:06:39Z matt $".
//
