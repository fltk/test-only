//
// "$Id$"
//
// Bitmap header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
 fltk3::Bitmap widget . */

#ifndef Fltk3_Bitmap_H
#define Fltk3_Bitmap_H

#include "Image.h"

namespace fltk3 {
  class Widget; 
  struct MenuItem;
  class Image;
}

namespace fltk3 {

  class QuartzGraphicsDriver;
  class GDIGraphicsDriver;
  class XlibGraphicsDriver;

  /**
   \brief STore and draw single color images.
   
   The fltk3::Bitmap class supports caching and drawing of mono-color
   (bitmap) images. Images are drawn using the current color.
   */
  class FLTK3_EXPORT Bitmap : public Image 
  {
    friend class QuartzGraphicsDriver;
    friend class GDIGraphicsDriver;
    friend class XlibGraphicsDriver;
    
  public:
    
    /** pointer to raw bitmap data */
    const uchar *array;
    /** Non-zero if array points to bitmap data allocated internally */
    int alloc_array;
    
  private:
    
#if defined(__APPLE__) || defined(WIN32)
    /** for internal use */
    void *id_;
#else
    /** for internal use */
    unsigned id_;
#endif // __APPLE__ || WIN32
    
  public:
    
    /** The constructors create a new bitmap from the specified bitmap data */
    Bitmap(const uchar *bits, int W, int H);

    /** The constructors create a new bitmap from the specified bitmap data */
    Bitmap(const char *bits, int W, int H);

    virtual ~Bitmap();
    
    virtual Image *copy(int W, int H);
    
    Image *copy() { return copy(w(), h()); }
    
    virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0);
    
    void draw(int X, int Y) {draw(X, Y, w(), h(), 0, 0);}
    
    virtual void label(Widget*w);
    
    virtual void label(MenuItem*m);
    
    virtual void uncache();
    
  };
  
} // namespace

#endif

//
// End of "$Id$".
//
