//
// "$Id: xbmImage.h,v 1.2 2002/12/10 02:00:29 easysw Exp $"
//
// Image subclass for in-memory xbm data (you #include the .xbm file
// and then construct this).
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

#ifndef fltk_xbmImage_h
#define fltk_xbmImage_h

#include "Image.h"

namespace fltk {

class FL_API xbmImage : public Image {
public:
  const unsigned char *array;
  xbmImage(const unsigned char *bits, int W, int H) : array(bits) {w = W; h = H;}
  xbmImage(const char *bits, int W, int H) :
    array((const unsigned char *)bits) {w = W; h = H;}
  void draw(int, int, int, int, Flags = 0);
  void draw(int x, int y, Flags f = 0) {draw(x,y,w,h,f);}
  int width() const {return w;}
  int height() const {return h;}
};

}

#endif

//
// End of "$Id: xbmImage.h,v 1.2 2002/12/10 02:00:29 easysw Exp $".
//
