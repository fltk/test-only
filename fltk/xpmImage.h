//
// "$Id: xpmImage.h,v 1.8 2003/11/04 08:10:57 spitzak Exp $"
//
// Image subclass that draws the data from an xpm format file.
// XPM is a file format designed for small icons in X, it can
// be convienently #include'd to inline the image into a program.
// Just pass the pointer defined by the file to the constructor.
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

#ifndef fltk_xpmImage_h
#define fltk_xpmImage_h

#include "Image.h"

namespace fltk {

class FL_API xpmImage : public Image {
public:
  const char * const * data;
  xpmImage(const char * const * d) : data(d) {w_ = -1;}
  xpmImage(const unsigned char* const * d) : data((char**)d) {w_ = -1;}
  // some compilers with bugs may need this one: (?)
  xpmImage(char ** d) : data(d) {w_ = -1;}

  void _measure(float&, float&) const;
  void _draw(int, int, int, int, const Style*, Flags = 0) const;

};

}

#endif

//
// End of "$Id: xpmImage.h,v 1.8 2003/11/04 08:10:57 spitzak Exp $".
//
