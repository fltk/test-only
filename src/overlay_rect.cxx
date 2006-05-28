//
// "$Id$"
//
// Overlay support for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

static fltk::Rectangle pr(0,0,0,0);

static void draw_current_rect() {
  if (pr.empty()) return;
  pen_mode(PEN_OVERLAY);
  strokerect(pr);
  pen_mode(PEN_NORMAL);
}

/** sets the pen mode (normal, xor for now) before drawing 
*/
void fltk::pen_mode(fltk::PenMode mode) {
#if USE_X11
    XSetFunction(xdisplay, gc, mode==PEN_OVERLAY ? GXxor : GXcopy);
    if (mode==PEN_OVERLAY ) XSetForeground(xdisplay, gc, 0xffffffff);
    return 0;
#elif defined(_WIN32)
    SetROP2(dc, mode==PEN_OVERLAY ? R2_NOT : R2_COPYPEN );
#elif defined(__APPLE__)
    PenMode( mode==PEN_OVERLAY ? patXor : patCopy);
#endif
}

void fltk::overlay_clear() {
  if (!pr.empty()) {
      draw_current_rect(); 
      pr.w(0);
  }
}

void fltk::overlay_rect(int x, int y, int w, int h) {
  if (w < 0) {x += w; w = -w;} else if (!w) w = 1;
  if (h < 0) {y += h; h = -h;} else if (!h) h = 1;
  if (!pr.empty()) {
    if (x==pr.x() && y==pr.y() && w==pr.w() && h==pr.h()) 
	return;
    draw_current_rect();
  }
  pr.set(x,y,w,h);
  draw_current_rect();
}

//
// End of "$Id$".
//
