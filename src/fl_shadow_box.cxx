//
// "$Id$"
//
// Shadow box drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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

#include <fltk3/run.h>
#include <fltk3/fl_draw.H>

#define BW 3

void fl_shadow_frame(int x, int y, int w, int h, Fl_Color c) {
  fl_color(FL_DARK3);
  fl_rectf(x+BW, y+h-BW,  w - BW, BW);
  fl_rectf(x+w-BW,  y+BW, BW,  h - BW);
  fl_color(c);
  fl_rect(x,y,w-BW,h-BW);
}

void fl_shadow_box(int x, int y, int w, int h, Fl_Color c) {
  fl_color(c);
  fl_rectf(x+1,y+1,w-2-BW,h-2-BW);
  fl_shadow_frame(x,y,w,h,FL_GRAY0);
}

//
// End of "$Id$".
//
