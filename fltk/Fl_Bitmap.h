//
// "$Id: Fl_Bitmap.h,v 1.5 2002/10/29 00:37:22 easysw Exp $"
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

/**
   This subclass of Fl_Image encapsulates the width, height, and bits 
   of an X bitmap (XBM), and allows you to make an Fl_Widget use a 
   bitmap as a label, or to just draw the bitmap directly. 
*/
class FL_API Fl_Bitmap : public Fl_Image {
public:
  const unsigned char *array;
  /**
   Construct using an X bitmap. The bits pointer is simply copied 
   to the object, so it must point at persistent storage. 
   The two constructors are provided because various X implementations 
   disagree about the type of bitmap data. 
   To use an XBM file use: 
   
   \code
   #include "foo.xbm"
   ...
   Fl_Bitmap bitmap = new Fl_Bitmap(foo_bits, foo_width, foo_height);
   \endcode
  */
  Fl_Bitmap(const unsigned char *bits, int W, int H) : array(bits) {w = W; h = H;}
  Fl_Bitmap(const char *bits, int W, int H) :
    array((const unsigned char *)bits) {w = W; h = H;}
  /**
   The image is drawn with the top-left corner at x,y. 
   The w and h are ignored. 1 bits are drawn with the current color, 
   0 bits are unchanged. The flags are ignored (FL_INACTIVE can be 
   done by selecting a gray version of the color). 
  */
  void draw(int x, int y, int w, int h, Fl_Flags = 0);
  /**
   Draws the image with the upper-left corner at x,y. 
   This is the same as doing draw(x, y, this->w, this->h, flags). 
  */
  void draw(int x, int y, Fl_Flags f = 0) {draw(x,y,w,h,f);}
  int width() const {return w;}
  int height() const {return h;}
};

#endif

//
// End of "$Id: Fl_Bitmap.h,v 1.5 2002/10/29 00:37:22 easysw Exp $".
//
