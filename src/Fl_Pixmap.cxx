//
// "$Id: Fl_Pixmap.cxx,v 1.24 2003/11/04 08:11:01 spitzak Exp $"
//
// Pixmap drawing code for the Fast Light Tool Kit (FLTK).
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

// An Image that draws inline XPM data.

// See draw_pixmap.C for code used to get the actual data into pixmap.
// Implemented without using the xpm library (which I can't use because
// it interferes with the color cube used by drawimage).

#include <fltk/xpmImage.h>
#include <fltk/draw.h>
#include <fltk/x.h>
using namespace fltk;

void xpmImage::_measure(float& W, float& H) const {
  if (w() < 0) measure_xpm(data,
			   const_cast<xpmImage*>(this)->w_,
			   const_cast<xpmImage*>(this)->h_);
  W = w_; 
  H = h_;
}

void xpmImage::_draw(int x, int y, int w, int h, const Style* style, Flags flags) const
{
  if (w_ < 0) measure_xpm(data,
			   const_cast<xpmImage*>(this)->w_,
			   const_cast<xpmImage*>(this)->h_);
  if (!w_) return; // ignore empty or bad pixmap data
  if (!id) {
    Pixmap pixmap = fl_create_offscreen(w_, h_);
    const_cast<xpmImage*>(this)->id = (void*)pixmap;
    fl_begin_offscreen(pixmap);
    uchar *bitmap = 0;
    set_mask_bitmap(&bitmap);
    draw_xpm(data, 0, 0, NO_COLOR);
    set_mask_bitmap(0);
    if (bitmap) {
      (const_cast<xpmImage*>(this))->create_bitmap_mask(bitmap, w_, h_);
      delete[] bitmap;
    }
    fl_end_offscreen();
  }
  draw_cache(x, y, w, h, style, flags);
}

//
// End of "$Id: Fl_Pixmap.cxx,v 1.24 2003/11/04 08:11:01 spitzak Exp $".
//
