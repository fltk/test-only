//
// "$Id$"
//
// Diamond box code for the Fast Light Tool Kit (FLTK).
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

// Box drawing code for an obscure box type.
// These box types are in separate files so they are not linked
// in if not used.

// The diamond box draws best if the area is square!

#include <fltk3/run.h>
#include <fltk3/draw.h>

namespace fltk3 {
  extern uchar* gray_ramp();
}

void fl_diamond_up_box(int x,int y,int w,int h,fltk3::Color bgcolor, fltk3::Boxtype) {
  w &= -2;
  h &= -2;
  int x1 = x+w/2;
  int y1 = y+h/2;
  fltk3::color(bgcolor); fltk3::polygon(x+3, y1, x1,y+3, x+w-3,y1, x1,y+h-3);
  uchar *g = fltk3::gray_ramp();
  fltk3::color(g[(uchar)'W']); fltk3::line(x+1, y1, x1, y+1, x+w-1, y1);
  fltk3::color(g[(uchar)'U']); fltk3::line(x+2, y1, x1, y+2, x+w-2, y1);
  fltk3::color(g[(uchar)'S']); fltk3::line(x+3, y1, x1, y+3, x+w-3, y1);
  fltk3::color(g[(uchar)'P']); fltk3::line(x+3, y1, x1, y+h-3, x+w-3, y1);
  fltk3::color(g[(uchar)'N']); fltk3::line(x+2, y1, x1, y+h-2, x+w-2, y1);
  fltk3::color(g[(uchar)'H']); fltk3::line(x+1, y1, x1, y+h-1, x+w-1, y1);
  fltk3::color(g[(uchar)'A']); fltk3::loop(x, y1, x1, y, x+w, y1, x1, y+h);
}

void fl_diamond_down_box(int x,int y,int w,int h,fltk3::Color bgcolor, fltk3::Boxtype) {
  w &= -2;
  h &= -2;
  int x1 = x+w/2;
  int y1 = y+h/2;
  uchar *g = fltk3::gray_ramp();
  fltk3::color(g[(uchar)'P']); fltk3::line(x+0, y1, x1, y+0, x+w-0, y1);
  fltk3::color(g[(uchar)'N']); fltk3::line(x+1, y1, x1, y+1, x+w-1, y1);
  fltk3::color(g[(uchar)'H']); fltk3::line(x+2, y1, x1, y+2, x+w-2, y1);
  fltk3::color(g[(uchar)'W']); fltk3::line(x+2, y1, x1, y+h-2, x+w-2, y1);
  fltk3::color(g[(uchar)'U']); fltk3::line(x+1, y1, x1, y+h-1, x+w-1, y1);
  fltk3::color(g[(uchar)'S']); fltk3::line(x+0, y1, x1, y+h-0, x+w-0, y1);
  fltk3::color(bgcolor); fltk3::polygon(x+3, y1, x1,y+3, x+w-3,y1, x1,y+h-3);
  fltk3::color(g[(uchar)'A']); fltk3::loop(x+3, y1, x1, y+3, x+w-3, y1, x1, y+h-3);
}


//
// End of "$Id$".
//
