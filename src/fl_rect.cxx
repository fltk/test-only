//
// "$Id: fl_rect.cxx,v 1.24 2001/02/20 06:59:50 spitzak Exp $"
//
// Non-path routines from fl_draw.H that are used by the standard boxtypes
// and thus are always linked into an fltk program.
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

#include <FL/fl_draw.H>
#include <FL/x.H>

void fl_rect(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
#ifdef WIN32
  x += fl_x_; y += fl_y_;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x+w-1, y);
  LineTo(fl_gc, x+w-1, y+h-1);
  LineTo(fl_gc, x, y+h-1);
  LineTo(fl_gc, x, y);
#else
  XDrawRectangle(fl_display, fl_window, fl_gc, fl_x_+x, fl_y_+y, w-1, h-1);
#endif
}

void fl_rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
#ifdef WIN32
  RECT rect;
  x += fl_x_; y += fl_y_;
  rect.left = x; rect.top = y;  
  rect.right = x + w; rect.bottom = y + h;
  FillRect(fl_gc, &rect, fl_brush);
#else
  XFillRectangle(fl_display, fl_window, fl_gc, fl_x_+x, fl_y_+y, w, h);
#endif
}

void fl_line(int x, int y, int x1, int y1) {
#ifdef WIN32
  MoveToEx(fl_gc, fl_x_+x, fl_y_+y, 0L); 
  LineTo(fl_gc, fl_x_+x1, fl_y_+y1);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  SetPixel(fl_gc, fl_x_+x1, fl_y_+y1, fl_colorref);
#else
  XDrawLine(fl_display, fl_window, fl_gc, fl_x_+x, fl_y_+y, fl_x_+x1,fl_y_+y1);
#endif
}

void fl_point(int x, int y) {
#ifdef WIN32
  SetPixel(fl_gc, fl_x_+x, fl_y_+y, fl_colorref);
#else
  XDrawPoint(fl_display, fl_window, fl_gc, fl_x_+x, fl_y_+y);
#endif
}

//
// End of "$Id: fl_rect.cxx,v 1.24 2001/02/20 06:59:50 spitzak Exp $".
//
