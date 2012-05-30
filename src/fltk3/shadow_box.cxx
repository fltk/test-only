//
// "$Id$"
//
// Shadow box drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/draw.h>

#define BW 3

void fl_shadow_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
  fltk3::color(fltk3::DARK3);
  fltk3::rectf(x+BW, y+h-BW,  w - BW, BW);
  fltk3::rectf(x+w-BW,  y+BW, BW,  h - BW);
  fltk3::color(c);
  fltk3::rect(x,y,w-BW,h-BW);
}

void fl_shadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  fltk3::color(c);
  fltk3::rectf(x+1,y+1,w-2-BW,h-2-BW);
  fl_shadow_frame(x,y,w,h,fltk3::GRAY0,t);
}


//
// End of "$Id$".
//
