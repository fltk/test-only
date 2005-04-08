//
// "$Id: GSave.cxx,v 1.1 2005/04/07 05:27:04 spitzak Exp $"
//
// Window widget class for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <fltk/Window.h>
#include <fltk/x.h>
using namespace fltk;

/*! \class fltk::GSave

  The constructor does a "gsave" and the destructor does a "grestore",
  and this object provides the memory to store the current graphics
  state. You must surround any calls to make_current() with this.

  To improve speed on some backends this may not preserve the font,
  color, path, or line settings, since these are usually changed
  afterwards anyway. The matrix, clip, and what surface is being drawn
  to are preserved. Also on some surfaces gsave() will clear the clip,
  to avoid expensive duplicating of it.

  \code
  if (!image.drawn()) {
    GSave gsave;
    image->make_current();
    draw_graphics_for_image();
    // destructor of the GSave happens here
  }
  // we can now draw our image into the window:
  image.draw(x,y,w,h);
  \endcode

  Warning: I ran into a C++ misfeature. If you write "GSave gsave()"
  it will compile into nothing, with no warning (on both GCC and VisualStudio)
  and your graphics state will end up destroyed.
*/

#ifdef _WIN32
extern HDC fl_bitmap_dc;
#endif

GSave::GSave() {
  push_matrix();
  push_no_clip();
  data[0] = (void*)Window::drawing_window_;
#if USE_X11
  data[1] = (void*)(xwindow);
#elif defined(_WIN32)
  // make it not destroy the previous dc:
  data[1] = (void*)dc;
  data[2] = (void*)(fl_bitmap_dc); fl_bitmap_dc = 0;
#elif defined(__APPLE__)
  data[1] = (void*)quartz_window;
  data[2] = (void*)quartz_gc;
#else
# error
#endif
}

GSave::~GSave() {
  Window::drawing_window_ = (Window*)(data[0]);
#if USE_X11
  xwindow = (XWindow)(data[1]);
#elif defined(_WIN32)
  dc = (HDC)(data[1]);
  DeleteDC(fl_bitmap_dc);
  fl_bitmap_dc = (HDC)(data[2]);
#elif defined(__APPLE__)
  quartz_window = data[1];
  quartz_gc = data[2];
#endif
  pop_clip();
  pop_matrix();
}

//
// End of "$Id: GSave.cxx,v 1.1 2005/04/07 05:27:04 spitzak Exp $".
//
