//
// "$Id: Fl_RGB_Image.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
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

#include "Fl_Image.h"

/**

   This subclass of Fl_Image encapsulates a full-color RGB image, 
   and allows you to make an Fl_Widget use an image as a label, or 
   to just draw the image directly. This is more efficient than 
   using fl_draw_image() if you want to draw the same image more than once. 

*/
class FL_API Fl_RGB_Image : public Fl_Image {
  int depth;
  const uchar* data;
public:
  /**
   Construct using a pointer to RGB data. W and H are the size of 
   the image in pixels. D is the delta between pixels (it may be
   more than 3 to skip alpha or other data, or negative to flip 
   the image left/right). LD is the delta between lines (it may 
   be more than D * W to crop images, or negative to flip the 
   image vertically). The data pointer is simply copied to the 
   object, so it must point at persistent storage. 
  */
  Fl_RGB_Image(const uchar* d, int W, int H, int D=3)
    : data(d) {w = W; h = H; depth = D;}
  /**
   The image is drawn with the top-left corner at x,y. 
   The w and h are ignored. The flags are ignored
   (although really it should use FL_INACTIVE to gray out the image). 
  */
  void draw(int x, int y, int w, int h, Fl_Flags = 0);
  /**
   Draws the image with the upper-left corner at x,y. 
   This is the same as doing draw(x, y, this->w, this->h, flags). 
  */
  void draw(int x, int y, Fl_Flags f = 0) {draw(x,y,w,h,f);}
};

#endif

//
// End of "$Id: Fl_RGB_Image.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
