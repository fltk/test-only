//
// "$Id: fl_scroll_area.cxx,v 1.6 2001/01/23 18:47:55 spitzak Exp $"
//
// Scrolling routines for the Fast Light Tool Kit (FLTK).
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

// Drawing function to move the contents of a rectangle.  This is passed
// a "callback" which is called to draw rectangular areas that are moved
// into the drawing area.

// This is not yet fixed to use the origin in fl_x_ and fl_y_!

#include <FL/x.H>
#include <FL/fl_draw.H>

// scroll a rectangle and redraw the newly exposed portions:
void fl_scroll(int X, int Y, int W, int H, int dx, int dy,
	       void (*draw_area)(void*, int,int,int,int), void* data)
{
  if (!dx && !dy) return;
  if (dx <= -W || dx >= W || dy <= -H || dy >= H) {
    // no intersection of old an new scroll
    draw_area(data,X,Y,W,H);
    return;
  }
  int src_x, src_w, dest_x, clip_x, clip_w;
  if (dx > 0) {
    src_x = X;
    dest_x = X+dx;
    src_w = W-dx;
    clip_x = X;
    clip_w = dx;
  } else {
    src_x = X-dx;
    dest_x = X;
    src_w = W+dx;
    clip_x = X+src_w;
    clip_w = W-src_w;
  }
  int src_y, src_h, dest_y, clip_y, clip_h;
  if (dy > 0) {
    src_y = Y;
    dest_y = Y+dy;
    src_h = H-dy;
    clip_y = Y;
    clip_h = dy;
  } else {
    src_y = Y-dy;
    dest_y = Y;
    src_h = H+dy;
    clip_y = Y+src_h;
    clip_h = H-src_h;
  }
#ifdef WIN32
  BitBlt(fl_gc, dest_x+fl_x_, dest_y+fl_y_, src_w, src_h,
	 fl_gc, src_x+fl_x_, src_y+fl_y_, SRCCOPY);
  // NYI: need to redraw areas that the source of BitBlt was bad due to
  // overlapped windows, somehow similar to what X does.
#else
  XCopyArea(fl_display, fl_window, fl_window, fl_gc,
	    src_x+fl_x_, src_y+fl_y_, src_w, src_h,
	    dest_x+fl_x_, dest_y+fl_y_);
// Synchronous update by waiting for graphics expose events:
  for (;;) {
    XEvent e; XWindowEvent(fl_display, fl_window, ExposureMask, &e);
    if (e.type == NoExpose) break;
    // otherwise assumme it is a GraphicsExpose event:
    draw_area(data, e.xexpose.x-fl_x_, e.xexpose.y-fl_y_,
	      e.xexpose.width, e.xexpose.height);
    if (!e.xgraphicsexpose.count) break;
  }
#endif
  if (dx) draw_area(data, clip_x, dest_y, clip_w, src_h);
  if (dy) draw_area(data, X, clip_y, W, clip_h);
}

//
// End of "$Id: fl_scroll_area.cxx,v 1.6 2001/01/23 18:47:55 spitzak Exp $".
//
