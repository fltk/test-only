//
// "$Id: rgbImage.h,v 1.8 2003/08/04 06:55:33 spitzak Exp $"
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
  int depth;
  const uchar* data;
public:
  rgbImage(const uchar* d, int W, int H, int D=3)
    : data(d) {w_ = W; h_ = H; depth = D;}
  void draw(float, float, float, float, Flags = 0) const;
  void draw(float x, float y, Flags f = 0) const {draw(x,y,w_,h_,f);}
  bool write_jpeg(const char *filename, int quality=75, int dpi=150);
};

}

#endif

//
// End of "$Id: rgbImage.h,v 1.8 2003/08/04 06:55:33 spitzak Exp $".
//
