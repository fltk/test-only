//
// "$Id: Fl_Image.h,v 1.6 2002/10/29 00:37:23 easysw Exp $"
//
// Image header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Image_H
#define Fl_Image_H

#include "Fl_Export.h"
#include "Fl_Flags.h"

class FL_API Fl_Widget;

/**
   This class holds an image, normally used to label a widget. 
   The subclasses define how the data is interpreted, and usually
   store server-side cached versions of the image. All the current
   types define pixel arrays, but other types of images, such as 
   vector graphics, can be defined. 
*/
class FL_API Fl_Image {
protected:
  /*@{*/
  /** 
   Subclasses that draw a fixed-size and unchanging image can use these 
   protected methods that use system-specific code to cache images in a 
   form that is ready to be drawn on the screen quickly. In the first 
   call to draw() they can set w and h to the size of the image, id and
   mask to the color and transparency offscreen windows, using 
   system-specific code. Then they can call _draw() to draw them, and all
   subsequent calls they can just call _draw(). To test if this is the 
   first call to draw() a subclass can check if id is zero. 
   */
  int w, h;
  void* id, * mask;
  void _draw(int x, int y, Fl_Flags);
  /*@}*/
public:
  Fl_Image() : id(0), mask(0) {}
  /**
   The two parameters are set to the size the image will occupy when drawn.
   For image types that can scale these are also input parameters: they must
   be preset to the rectangle the caller intends to pass to draw(). This is 
   for scaling and tiling image types. To find the "natural" size of a scaling 
   image, preset w and h to zero before calling this. 
  */
  virtual void measure(int& W, int& H);
  /**
   Draw the image with the upper-left corner at x,y. If the image can scale
   or tile or otherwise uses a size, the w,h describe the size of the box it
   is wanted to fill. The flags can be used by subclasses to draw differently 
   if FL_INACTIVE or any other flags are set. 
  */
  virtual void draw(int x, int y, int w, int h, Fl_Flags = 0) = 0;
  /**
   Draws the image with the upper-left corner at x,y. This is the same as
   doing draw(x, y, this->w, this->h, flags). 

   The base class destructor will destroy anything created for id or mask. 
  */
  void draw(int x, int y, Fl_Flags f = 0) {draw(x,y,w,h,f);}
  /**
   The destructor throws away temporary data created to draw the image, 
   but in most cases does not destroy the local data passed to a constructor. 
  */
  virtual ~Fl_Image();
  // for back compatability only:
  void label(Fl_Widget* o);
};

#endif

//
// End of "$Id: Fl_Image.h,v 1.6 2002/10/29 00:37:23 easysw Exp $".
//
