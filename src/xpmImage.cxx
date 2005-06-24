// "$Id$"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

/*! \class fltk::xpmImage

  Draws inline XPM data. This is a text-based 256-color image format
  designed for X11 and still very useful for making small icons, since
  the data can be easily inserted into your source code.

  Currently fltk has a huge kludge to recognize "monochrome" XPM files
  and assume they are b&w "glyphs" that can be drawn in any
  color. This is detected by the first line reading " \tc #FFFFFF", ie
  the first color is space and is defined as white. White will be
  transparent, black opaque, and gray partially transparent.  You
  should only use gray shades in the rest of the image. Such b&w
  glyphs are very useful for making modern user interfaces.

*/

// See draw_pixmap.C for code used to get the actual data into pixmap.
// Implemented without using the xpm library (which I can't use because
// it interferes with the color cube used by drawimage).

#include <config.h>
#include <fltk/xpmImage.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <fltk/string.h>
using namespace fltk;

void xpmImage::_measure(int& w, int& h) const {
  if (this->w() < 0) {
    measure_xpm(data,w,h);
    const_cast<xpmImage*>(this)->setsize(w,h);
  } else {
    w = this->w();
    h = this->h();
  }
}

#if !USE_QUARTZ
extern bool fl_did_monochrome;
#if USE_X11
extern uchar **fl_mask_bitmap;
#endif
#endif

void xpmImage::_draw(const fltk::Rectangle& r) const
{
#if !USE_QUARTZ
  // detect our kludge for monochrome xpm images of letters:
  if (this->fg || this->bg) {
#if USE_X11
    if (getcolor() != this->fg || getbgcolor() != this->bg)
#else
    if (getcolor() != this->fg)
#endif
      (const_cast<xpmImage*>(this))->destroy_cache();
  }
#endif
  if (!drawn()) {
    xpmImage* t = const_cast<xpmImage*>(this);
    if (this->w() < 0) {
      int w,h;
      measure_xpm(data,w,h);
      t->setsize(w,h);
    }
    if (this->w() <= 0 || this->h() <= 0) return;
    GSave gsave;
    t->make_current();
#if USE_QUARTZ
    draw_xpm(data, 0, 0);
#else
    fl_did_monochrome = false;
# if USE_X11
    uchar *bitmap = 0;
    fl_mask_bitmap = &bitmap;
# endif
    draw_xpm(data, 0, 0);
    if (fl_did_monochrome) {
      t->fg = getcolor();
      t->bg = getbgcolor();
    } else {
      t->fg = t->bg = 0;
    }
# if USE_X11
    fl_mask_bitmap = 0;
    if (bitmap) {
      (const_cast<xpmImage*>(this))->set_alpha_bitmap(bitmap, this->w(), this->h());
      delete[] bitmap;
    }
# endif
#endif
  }
  Image::_draw(r);
}

//
// End of "$Id$".
//
