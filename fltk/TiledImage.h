//
// "$Id: TiledImage.h,v 1.2 2003/02/02 10:39:22 spitzak Exp $"
//
// A tiled image completely fills the bounding box passed to it with
// replications of the internal Image passed to it.
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#ifndef fltk_TiledImage_h
#define fltk_TiledImage_h

#include "Image.h"

namespace fltk {

class FL_API TiledImage : public Image {
protected:
  Image* image_;
public:
  TiledImage(Image *i) : Image(), image_(i) {}
  Image* image() const {return image_;}
  void image(Image* i) {image_ = i;}
  void measure(int& w, int& h);
  void draw(int x, int y, int w, int h, Color = 0, Flags = 0);
};

}
#endif

//
// End of "$Id: TiledImage.h,v 1.2 2003/02/02 10:39:22 spitzak Exp $"
//
