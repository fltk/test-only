//
// "$Id: xpmImage.h,v 1.3 2003/02/02 10:39:22 spitzak Exp $"
//
// Image subclass that draws the data from an xpm format file.
// XPM is a file format designed for small icons in X, it can
// be convienently #include'd to inline the image into a program.
// Just pass the pointer defined by the file to the constructor.
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

#ifndef fltk_xpmImage_h
#define fltk_xpmImage_h

#include "Image.h"

namespace fltk {

class FL_API xpmImage : public Image {
public:
  const char * const * data;
  xpmImage(const char * const * d) : data(d) {w = -1;}
  xpmImage(const unsigned char* const * d) : data((char**)d) {w = -1;}
  // some compilers with bugs may need this one: (?)
  xpmImage(char ** d) : data(d) {w = -1;}

  void measure(int&,int&);
  void draw(int, int, int, int, Color=0, Flags = 0);
  void draw(int x, int y, Flags f = 0) {draw(x,y,w,h,0,f);}

};

}

#endif

//
// End of "$Id: xpmImage.h,v 1.3 2003/02/02 10:39:22 spitzak Exp $".
//
