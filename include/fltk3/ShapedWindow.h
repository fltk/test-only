//
// "$Id"
//
// Window header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef fltk3_ShapedWindow_H
#define fltk3_ShapedWindow_H

#include "Window.h"

namespace fltk3 {

class Image;

/**
  The fltk3::ShapedWindow is a fltk3::Window that can take an arbitrary shape (not just a rectangular region).
  It does this by taking a fltk3::Bitmap (with an alpha channel) and using the alpha channel as the transparency mask.

  The layout and widgets inside are unaware of the mask shape, and most will act as though the bounding box is available 
  to them. It is up to you to make sure they adhere to the bounds of their masking shape!

  The window borders and caption created by the window system are turned off by default for a ShapedWindow object.  They 
  can be re-enabled by calling void Window::border(bool set).
 
  A minimal usage example is found at test/shapedwindow.cxx, which embeds an image in the code (for portability).

  One day this will work for any image type, rather than just a fltk3::Bitmap.
 
  \todo Make this work for all image sets fltk3 supports, not just a fltk3::Bitmap
  \todo Currently the draw method is inefficient as it comes, and as such the window suffers from a high degree of artefacting as it is moved around (by the user). This needs to be fixed ASAP (if possible) as it is no doubt more CPU intensive than it should be.
  \todo An image() method needs to be added (or, alternatively, the shape() method needs to be changed) so that passing an image with an alpha channel does the task of both shape() and image() combined.
 */


class FLTK3_EXPORT ShapedWindow : public Window {
  public:
    ShapedWindow(int w, int h, const char* l = 0);
    ShapedWindow(int x, int y, int w, int h, const char* l = 0);
    ~ShapedWindow();
    void shape(Image* b);
    inline void shape(Image& b) { shape(&b); }

  protected:
    virtual void draw();
    Image* shape_;
    int lw, lh;
    bool changed;
};

}
#endif

//
// End of "$Id".
//
