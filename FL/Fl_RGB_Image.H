//
// "$Id: Fl_RGB_Image.H,v 1.3 2000/03/19 01:38:30 bill Exp $"
//
// RGB_Image header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_RGB_Image_H
#define Fl_RGB_Image_H

#include "Fl_Image.H"

class FL_API Fl_RGB_Image : public Fl_Image {
  int depth;
  const uchar* data;
public:
  Fl_RGB_Image(const uchar* d, int W, int H, int D=3)
    : data(d) {w = W; h = H; depth = D;}
  void draw(int, int, int, int, int cx, int cy);
  void draw(int X, int Y) {draw(X,Y,w,h,0,0);}
};

#endif

//
// End of "$Id: Fl_RGB_Image.H,v 1.3 2000/03/19 01:38:30 bill Exp $".
//
