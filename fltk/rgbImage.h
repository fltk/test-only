//
// "$Id$"
//
// Image subclass that draws uncompressed 8-bit rgb data from memory.
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#ifndef fltk_rgbImage_h
#define fltk_rgbImage_h

#include "Image.h"

namespace fltk {

class FL_API rgbImage : public Image {
  const uchar* pixels_;
  PixelType pixeltype_;
  int depth_;
  int linedelta_;
public:
  PixelType pixeltype() const {return pixeltype_;}
  int depth() const {return depth_;}
  int linedelta() const {return linedelta_;}
  const uchar* pixels() const {return pixels_;}

  void _draw(const Rectangle&, const Style*, Flags) const;
  //bool write_jpeg(const char *filename, int quality=75, int dpi=150);

  rgbImage(const uchar* d, PixelType p, int W, int H) :
    Image(W,H), pixels_(d), pixeltype_(p), depth_(fltk::depth(p)) {linedelta_=W*depth_;}

  rgbImage(const uchar* d, PixelType p, int W, int H, int delta) :
    Image(W,H), pixels_(d), pixeltype_(p), depth_(delta), linedelta_(W*delta) {}

  rgbImage(const uchar* d, PixelType p, int W, int H, int delta, int ld) :
    Image(W,H), pixels_(d), pixeltype_(p), depth_(delta), linedelta_(ld) {}

  rgbImage(const uchar* d, int W, int H, int D = 3) :
    Image(W,H), pixels_(d), pixeltype_(RGB), depth_(D), linedelta_(W*D) {}

};

}

#endif

//
// End of "$Id$".
//
