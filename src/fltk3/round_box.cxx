//
// "$Id$"
//
// Round box drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/draw.h>

namespace fltk3 {
  extern uchar* gray_ramp();
}


// A compiler from a certain very large software company will not compile
// the function pointer assignment due to the name conflict with fltk3::arc.
// This function is to fix that:
static void arc_i(int x,int y,int w,int h,double a1,double a2) {
  fltk3::arc(x,y,w,h,a1,a2);
}

enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

static void draw(int which, int x,int y,int w,int h, int inset, fltk3::Color color)
{
  if (inset*2 >= w) inset = (w-1)/2;
  if (inset*2 >= h) inset = (h-1)/2;
  x += inset;
  y += inset;
  w -= 2*inset;
  h -= 2*inset;
  int d = w <= h ? w : h;
  if (d <= 1) return;
  fltk3::color(color);
  void (*f)(int,int,int,int,double,double);
  f = (which==FILL) ? fltk3::pie : arc_i;
  if (which >= CLOSED) {
    f(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270);
  } else if (which == UPPER_LEFT) {
    f(x+w-d, y, d, d, 45, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, 225);
  } else { // LOWER_RIGHT
    f(x, y+h-d, d, d, 225, w<=h ? 360 : 270);
    f(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45);
  }
  if (which == FILL) {
    if (w < h)
      fltk3::rectf(x, y+d/2, w, h-(d&-2));
    else if (w > h)
      fltk3::rectf(x+d/2, y, w-(d&-2), h);
  } else {
    if (w < h) {
      if (which != UPPER_LEFT) fltk3::yxline(x+w-1, y+d/2-1, y+h-d/2+1);
      if (which != LOWER_RIGHT) fltk3::yxline(x, y+d/2-1, y+h-d/2+1);
    } else if (w > h) {
      if (which != UPPER_LEFT) fltk3::xyline(x+d/2-1, y+h-1, x+w-d/2+1);
      if (which != LOWER_RIGHT) fltk3::xyline(x+d/2-1, y, x+w-d/2+1);
    }
  }
}

void fl_classic_round_down_box(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*) {
  uchar *g = fltk3::gray_ramp();
  draw(FILL,	    x,   y, w,   h, 2, bgcolor);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'N']);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'H']);
  draw(UPPER_LEFT,  x,   y, w,   h, 0, (fltk3::Color)g[(uchar)'N']);
  draw(UPPER_LEFT,  x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'H']);
  draw(LOWER_RIGHT, x,   y, w,   h, 0, (fltk3::Color)g[(uchar)'S']);
  draw(LOWER_RIGHT, x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'U']);
  draw(LOWER_RIGHT, x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'U']);
  draw(LOWER_RIGHT, x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'W']);
  draw(CLOSED,	    x,   y, w,   h, 2, (fltk3::Color)g[(uchar)'A']);
}

void fl_classic_round_up_box(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*) {
  uchar *g = fltk3::gray_ramp();
  draw(FILL,	    x,   y, w,   h, 2, bgcolor);
  draw(LOWER_RIGHT, x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'H']);
  draw(LOWER_RIGHT, x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'N']);
  draw(LOWER_RIGHT, x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'H']);
  draw(LOWER_RIGHT, x,   y, w,   h, 2, (fltk3::Color)g[(uchar)'N']);
  draw(UPPER_LEFT,  x,   y, w,   h, 2, (fltk3::Color)g[(uchar)'U']);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'S']);
  draw(UPPER_LEFT,  x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'W']);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'U']);
  draw(CLOSED,	    x,   y, w,   h, 0, (fltk3::Color)g[(uchar)'A']);
}

//
// End of "$Id$".
//
