//
// "$Id: Image.h,v 1.2 2002/12/10 02:00:29 easysw Exp $"
//
// Image object used to label widgets. This caches the image in a
// server pixmap. Subclasses are used to decide how to change data
// into an image.
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

#ifndef fltk_Image_h
#define fltk_Image_h

#include "FL_API.h"
#include "Flags.h"

namespace fltk {

class FL_API Widget;

class FL_API Image {
protected:
  int w, h;
  void* id, * mask;
  void _draw(int x, int y, Flags);
public:
  Image() : id(0), mask(0) {}
  virtual void measure(int& W, int& H);
  virtual void draw(int x, int y, int w, int h, Flags = 0) = 0;
  void draw(int x, int y, Flags f = 0) {draw(x,y,w,h,f);}
  virtual ~Image();
  // for back compatability only:
  void label(Widget* o);
};

}

#endif

//
// End of "$Id: Image.h,v 1.2 2002/12/10 02:00:29 easysw Exp $".
//
