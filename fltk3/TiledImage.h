//
// "$Id: TiledImage.h 7903 2010-11-28 21:06:39Z matt $"
//
// Tiled image header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::TiledImage widget . */

#ifndef Fltk3_Tiled_Image_H
#  define Fltk3_Tiled_Image_H

#  include "Image.h"

namespace fltk3 {
  
  /**
   This class supports tiling of images
   over a specified area. The source (tile) image is <B>not</B>
   copied unless you call the color_average(),
   desaturate(),
   or inactive()
   methods.
   */
  class FLTK3_EXPORT TiledImage : public fltk3::Image {
  protected:
    
    fltk3::Image	*image_;		// The image that is shared
    int		alloc_image_;		// Did we allocate this image?
    
  public:
    
    TiledImage(fltk3::Image *i, int W = 0, int H = 0);
    virtual ~TiledImage();
    
    virtual fltk3::Image *copy(int W, int H);
    fltk3::Image *copy() { return copy(w(), h()); }
    virtual void color_average(fltk3::Color c, float i);
    virtual void desaturate();
    virtual void draw(int X, int Y, int W, int H, int cx, int cy);
    void draw(int X, int Y) { draw(X, Y, w(), h(), 0, 0); }
    /** Gets The image that is shared */ 
    fltk3::Image *image() { return image_; }
  };
  
}

#endif // !Fl_Tiled_Image_H

//
// End of "$Id: TiledImage.h 7903 2010-11-28 21:06:39Z matt $"
//
