//
// "$Id: Fl_Pixmap.cxx,v 1.15 2001/08/05 21:12:15 spitzak Exp $"
//
// Pixmap drawing code for the Fast Light Tool Kit (FLTK).
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

// An Fl_Image that draws inline XPM data.

// See fl_draw_pixmap.C for code used to get the actual data into pixmap.
// Implemented without using the xpm library (which I can't use because
// it interferes with the color cube used by fl_draw_image).

#include <fltk/Fl.h>
#include <fltk/fl_draw.h>
#include <fltk/x.h>
#include <fltk/Fl_Pixmap.h>

void Fl_Pixmap::measure(int& W,int& H) {
  if (w < 0) fl_measure_pixmap(data, w, h);
  W=w; 
  H=h;
}

void Fl_Pixmap::draw(int X, int Y, Fl_Flags flags)
{
  if (w < 0) fl_measure_pixmap(data, w, h);
  if (!w) return; // ignore empty or bad pixmap data
  if (!id) {
    id = fl_create_offscreen(w, h);
    fl_begin_offscreen(id);
    uchar *bitmap = 0;
    fl_set_mask_bitmap(&bitmap);
    fl_draw_pixmap(data, 0, 0, FL_NO_COLOR);
    fl_set_mask_bitmap(0);
    if (bitmap) {
      mask = fl_create_bitmap(bitmap, w, h);
      delete[] bitmap;
    }
    fl_end_offscreen();
  }
  _draw(X, Y, flags);
}

//
// End of "$Id: Fl_Pixmap.cxx,v 1.15 2001/08/05 21:12:15 spitzak Exp $".
//
