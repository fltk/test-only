//
// "$Id: Fl_Bitmap.h,v 1.4 2002/05/06 06:31:26 spitzak Exp $"
//
// Bitmap header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Bitmap_H
#define Fl_Bitmap_H

#include "Fl_Image.h"

class FL_API Fl_Bitmap : public Fl_Image {
public:
  const unsigned char *array;
  Fl_Bitmap(const unsigned char *bits, int W, int H) : array(bits) {w = W; h = H;}
  Fl_Bitmap(const char *bits, int W, int H) :
    array((const unsigned char *)bits) {w = W; h = H;}
  void draw(int, int, int, int, Fl_Flags = 0);
  void draw(int x, int y, Fl_Flags f = 0) {draw(x,y,w,h,f);}
  int width() const {return w;}
  int height() const {return h;}
};

#endif

//
// End of "$Id: Fl_Bitmap.h,v 1.4 2002/05/06 06:31:26 spitzak Exp $".
//
