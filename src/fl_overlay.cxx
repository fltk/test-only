//
// "$Id: fl_overlay.cxx,v 1.9 2002/12/09 04:52:30 spitzak Exp $"
//
// Overlay support for the Fast Light Tool Kit (FLTK).
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

// Extremely limited "overlay" support.  You can use this to drag out
// a rectangle in response to mouse events.  It is your responsibility
// to erase the overlay before drawing anything that might intersect
// it.

#include <fltk/x.h>
#include <fltk/draw.h>
using namespace fltk;

static int px,py,pw,ph;

static void draw_current_rect() {
#ifdef _WIN32
  int old = SetROP2(gc, R2_NOT);
  strokerect(px, py, pw, ph);
  SetROP2(gc, old);
#else
  XSetFunction(xdisplay, gc, GXxor);
  XSetForeground(xdisplay, gc, 0xffffffff);
  XDrawRectangle(xdisplay, xwindow, gc, px, py, pw, ph);
  XSetFunction(xdisplay, gc, GXcopy);
#endif
}

void fltk::overlay_clear() {
  if (pw > 0) {draw_current_rect(); pw = 0;}
}

void fltk::overlay_rect(int x, int y, int w, int h) {
  transform(x,y);
  if (w < 0) {x += w; w = -w;} else if (!w) w = 1;
  if (h < 0) {y += h; h = -h;} else if (!h) h = 1;
  if (pw > 0) {
    if (x==px && y==py && w==pw && h==ph) return;
    draw_current_rect();
  }
  px = x; py = y; pw = w; ph = h;
  draw_current_rect();
}

//
// End of "$Id: fl_overlay.cxx,v 1.9 2002/12/09 04:52:30 spitzak Exp $".
//
