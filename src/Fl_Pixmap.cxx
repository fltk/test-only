//
// "$Id: Fl_Pixmap.cxx,v 1.26 2004/05/04 07:30:43 spitzak Exp $"
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

#include <config.h>
#include <fltk/xpmImage.h>
#include <fltk/draw.h>
#include <fltk/x.h>
using namespace fltk;

void xpmImage::_measure(float& W, float& H) const {
  if (w() < 0) {
    int w,h;
    measure_xpm(data,w,h);
    const_cast<xpmImage*>(this)->setsize(w,h);
  }
  W = w(); 
  H = h();
}

void xpmImage::_draw(int x, int y, int w, int h, const Style* style, Flags flags) const
{
  if (!drawn()) {
    if (this->w() < 0) {
      int W,H;
      measure_xpm(data,W,H);
      const_cast<xpmImage*>(this)->setsize(W,H);
    }
    if (this->w() <= 0 || this->h() <= 0) return;
    ImageDraw idraw(const_cast<xpmImage*>(this));
    uchar *bitmap = 0;
    set_mask_bitmap(&bitmap);
    draw_xpm(data, 0, 0, NO_COLOR);
    set_mask_bitmap(0);
    if (bitmap) {
      (const_cast<xpmImage*>(this))->set_alpha_bitmap(bitmap, this->w(), this->h());
      delete[] bitmap;
    }
  }
  Image::_draw(x, y, w, h, style, flags);
}

//
// End of "$Id: Fl_Pixmap.cxx,v 1.26 2004/05/04 07:30:43 spitzak Exp $".
//
