//
// "$Id: Fl_RGB_Image.cxx,v 1.3 2001/07/23 09:50:05 spitzak Exp $"
//
// RGB_Image drawing code for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/fl_draw.h>
#include <fltk/x.h>
#include <fltk/Fl_RGB_Image.h>

#include <stdio.h>

void Fl_RGB_Image::draw(int X, int Y, int W, int H, int cx, int cy)
{
  if (!id) {
    id = fl_create_offscreen(w, h);
    fl_begin_offscreen(id);
    fl_draw_image(data, 0, 0, w, h, depth);
    fl_end_offscreen();
  }
  _draw(X, Y, W, H, cx, cy);
}

//
// End of "$Id: Fl_RGB_Image.cxx,v 1.3 2001/07/23 09:50:05 spitzak Exp $".
//
