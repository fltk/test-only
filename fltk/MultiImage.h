//
// "$Id: MultiImage.h,v 1.5 2003/04/20 03:17:48 easysw Exp $"
//
// Image type that draws a different image depending on the flags,
// for instace VALUE or SELECTED or HIGHLIGHT.
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

#ifndef fltk_MultiImage_h
#define fltk_MultiImage_h

#include "Image.h"

namespace fltk {

class FL_API MultiImage : public Image
{
  enum {MAXIMAGES = 8};
  Image* images[MAXIMAGES];
  Flags flags[MAXIMAGES];
public:
  void measure(int&,int&);
  void draw(int, int, int, int, Flags = 0);
  void draw(int x, int y, Flags f = 0) {draw(x,y,w,h,f);}
  MultiImage(Image& image0,
		Flags flags1, Image& image1) {
    images[0] = &image0;
    flags[1] = flags1; images[1] = &image1;
    images[2] = 0;
  }
  MultiImage(Image& image0,
		Flags flags1, Image& image1,
		Flags flags2, Image& image2) {
    images[0] = &image0;
    flags[1] = flags1; images[1] = &image1;
    flags[2] = flags2; images[2] = &image2;
    images[3] = 0;
  }
  MultiImage(Image& image0,
		Flags flags1, Image& image1,
		Flags flags2, Image& image2,
		Flags flags3, Image& image3) {
    images[0] = &image0;
    flags[1] = flags1; images[1] = &image1;
    flags[2] = flags2; images[2] = &image2;
    flags[3] = flags3; images[3] = &image3;
    images[4] = 0;
  }
  MultiImage(Image& image0,
		Flags flags1, Image& image1,
		Flags flags2, Image& image2,
		Flags flags3, Image& image3,
		Flags flags4, Image& image4) {
    images[0] = &image0;
    flags[1] = flags1; images[1] = &image1;
    flags[2] = flags2; images[2] = &image2;
    flags[3] = flags3; images[3] = &image3;
    flags[4] = flags4; images[4] = &image4;
    images[5] = 0;
  }
  MultiImage(Image& image0,
		Flags flags1, Image& image1,
		Flags flags2, Image& image2,
		Flags flags3, Image& image3,
		Flags flags4, Image& image4,
		Flags flags5, Image& image5) {
    images[0] = &image0;
    flags[1] = flags1; images[1] = &image1;
    flags[2] = flags2; images[2] = &image2;
    flags[3] = flags3; images[3] = &image3;
    flags[4] = flags4; images[4] = &image4;
    flags[5] = flags5; images[5] = &image5;
    images[6] = 0;
  }
  MultiImage(Image& image0,
		Flags flags1, Image& image1,
		Flags flags2, Image& image2,
		Flags flags3, Image& image3,
		Flags flags4, Image& image4,
		Flags flags5, Image& image5,
		Flags flags6, Image& image6) {
    images[0] = &image0;
    flags[1] = flags1; images[1] = &image1;
    flags[2] = flags2; images[2] = &image2;
    flags[3] = flags3; images[3] = &image3;
    flags[4] = flags4; images[4] = &image4;
    flags[5] = flags5; images[5] = &image5;
    flags[6] = flags6; images[6] = &image6;
    images[7] = 0;
  }
  MultiImage(Image& image0,
		Flags flags1, Image& image1,
		Flags flags2, Image& image2,
		Flags flags3, Image& image3,
		Flags flags4, Image& image4,
		Flags flags5, Image& image5,
		Flags flags6, Image& image6,
		Flags flags7, Image& image7) {
    images[0] = &image0;
    flags[1] = flags1; images[1] = &image1;
    flags[2] = flags2; images[2] = &image2;
    flags[3] = flags3; images[3] = &image3;
    flags[4] = flags4; images[4] = &image4;
    flags[5] = flags5; images[5] = &image5;
    flags[6] = flags6; images[6] = &image6;
    flags[7] = flags7; images[7] = &image7;
  }
};

}

#endif

//
// End of "$Id: MultiImage.h,v 1.5 2003/04/20 03:17:48 easysw Exp $".
//
