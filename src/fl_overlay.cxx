//
// "$Id: fl_overlay.cxx,v 1.15 2004/07/31 11:01:41 laza2000 Exp $"
//
// Overlay support for the Fast Light Tool Kit (FLTK).
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

// Extremely limited "overlay" support.  You can use this to drag out
// a rectangle in response to mouse events.  It is your responsibility
// to erase the overlay before drawing anything that might intersect
// it.

#include <config.h>
#include <fltk/x.h>
#include <fltk/draw.h>
using namespace fltk;

static int px,py,pw,ph;

static void draw_current_rect() {
#if USE_X11
  XSetFunction(xdisplay, gc, GXxor);
  XSetForeground(xdisplay, gc, 0xffffffff);
  strokerect(px, py, pw, ph);
  XSetFunction(xdisplay, gc, GXcopy);
#elif defined(_WIN32)
  int old = SetROP2(dc, R2_NOT);
  strokerect(px, py, pw, ph);
  SetROP2(dc, old);
#elif defined(__APPLE__)
  PenMode( patXor );
  strokerect(px, py, pw, ph);
  PenMode( patCopy );
#endif
}

void fltk::overlay_clear() {
  if (pw > 0) {draw_current_rect(); pw = 0;}
}

void fltk::overlay_rect(int x, int y, int w, int h) {
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
// End of "$Id: fl_overlay.cxx,v 1.15 2004/07/31 11:01:41 laza2000 Exp $".
//
