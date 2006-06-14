//
// "$Id$"
//
// Image subclass that draws uncompressed 8-bit rgb data from memory.
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
  int linedelta_;
public:
    rgbImage(const uchar * d, PixelType p, int W, int H, const char* name=0) 
	: Image(W,H,name, (const char* const*) d) {p_=p;  linedelta_=W*this->depth();}

    rgbImage(const uchar * d, PixelType p, int W, int H, int linedelta)
	: Image(W,H, 0, (const char* const*) d),  linedelta_(linedelta) {p_=p;}

    int linedelta() const {return linedelta_;}
    void update();
  //bool write_jpeg(const char *filename, int quality=75, int dpi=150);
};

}

#endif

//
// End of "$Id$".
//
