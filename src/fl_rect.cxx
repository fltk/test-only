//
// "$Id: fl_rect.cxx,v 1.18 2000/05/17 07:08:11 bill Exp $"
//
// These routines from fl_draw.H are used by the standard boxtypes
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
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x+w-1, y);
  LineTo(fl_gc, x+w-1, y+h-1);
  LineTo(fl_gc, x, y+h-1);
  LineTo(fl_gc, x, y);
#else
  XDrawRectangle(fl_display, fl_window, fl_gc, x, y, w-1, h-1);
#endif
}

void fl_rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
#ifdef WIN32
  RECT rect;
  rect.left = x; rect.top = y;  
  rect.right = x + w; rect.bottom = y + h;
  FillRect(fl_gc, &rect, fl_brush());
#else
  if (w && h) XFillRectangle(fl_display, fl_window, fl_gc, x, y, w, h);
#endif
}

void fl_xyline(int x, int y, int x1) {
#ifdef WIN32
  MoveToEx(fl_gc, x, y, 0L); LineTo(fl_gc, x1+1, y);
#else
  XDrawLine(fl_display, fl_window, fl_gc, x, y, x1, y);
#endif
}

void fl_xyline(int x, int y, int x1, int y2) {
#ifdef WIN32
  if (y2 < y) y2--;
  else y2++;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y);
  LineTo(fl_gc, x1, y2);
#else
  XPoint p[3];
  p[0].x = x;  p[0].y = p[1].y = y;
  p[1].x = p[2].x = x1; p[2].y = y2;
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
#endif
}

#if 0
void fl_xyline(int x, int y, int x1, int y2, int x3) {
#ifdef WIN32
  if(x3 < x1) x3--;
  else x3++;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y);
  LineTo(fl_gc, x1, y2);
  LineTo(fl_gc, x3, y2);
#else
  XPoint p[4];
  p[0].x = x;  p[0].y = p[1].y = y;
  p[1].x = p[2].x = x1; p[2].y = p[3].y = y2;
  p[3].x = x3;
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
#endif
}
#endif

void fl_yxline(int x, int y, int y1) {
#ifdef WIN32
  if (y1 < y) y1--;
  else y1++;
  MoveToEx(fl_gc, x, y, 0L); LineTo(fl_gc, x, y1);
#else
  XDrawLine(fl_display, fl_window, fl_gc, x, y, x, y1);
#endif
}

void fl_yxline(int x, int y, int y1, int x2) {
#ifdef WIN32
  if (x2 > x) x2++;
  else x2--;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x, y1);
  LineTo(fl_gc, x2, y1);
#else
  XPoint p[3];
  p[0].x = p[1].x = x;  p[0].y = y;
  p[1].y = p[2].y = y1; p[2].x = x2;
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
#endif
}

#if 0
void fl_yxline(int x, int y, int y1, int x2, int y3) {
#ifdef WIN32
  if(y3<y1) y3--;
  else y3++;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x, y1);
  LineTo(fl_gc, x2, y1);
  LineTo(fl_gc, x2, y3);
#else
  XPoint p[4];
  p[0].x = p[1].x = x;  p[0].y = y;
  p[1].y = p[2].y = y1; p[2].x = p[3].x = x2;
  p[3].y = y3;
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
#endif
}
#endif

void fl_line(int x, int y, int x1, int y1) {
#ifdef WIN32
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  SetPixel(fl_gc, x1, y1, fl_RGB());
#else
  XDrawLine(fl_display, fl_window, fl_gc, x, y, x1, y1);
#endif
}

void fl_line(int x, int y, int x1, int y1, int x2, int y2) {
#ifdef WIN32
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  LineTo(fl_gc, x2, y2);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  SetPixel(fl_gc, x2, y2, fl_RGB());
#else
  XPoint p[3];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
#endif
}

#if 0
void fl_loop(int x, int y, int x1, int y1, int x2, int y2) {
#ifdef WIN32
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  LineTo(fl_gc, x2, y2);
  LineTo(fl_gc, x, y);
#else
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x;  p[3].y = y;
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
#endif
}

void fl_loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
#ifdef WIN32
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  LineTo(fl_gc, x2, y2);
  LineTo(fl_gc, x3, y3);
  LineTo(fl_gc, x, y);
#else
  XPoint p[5];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;
  p[4].x = x;  p[4].y = y;
  XDrawLines(fl_display, fl_window, fl_gc, p, 5, 0);
#endif
}
#endif

void fl_polygon(int x, int y, int x1, int y1, int x2, int y2) {
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
#ifdef WIN32
  SelectObject(fl_gc, fl_brush());
  Polygon(fl_gc, p, 3);
#else
  p[3].x = x;  p[3].y = y;
  XFillPolygon(fl_display, fl_window, fl_gc, p, 3, Convex, 0);
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
#endif
}

#if 0
void fl_polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  XPoint p[5];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;
#ifdef WIN32
  SelectObject(fl_gc, fl_brush());
  Polygon(fl_gc, p, 4);
#else
  p[4].x = x;  p[4].y = y;
  XFillPolygon(fl_display, fl_window, fl_gc, p, 4, Convex, 0);
  XDrawLines(fl_display, fl_window, fl_gc, p, 5, 0);
#endif
}
#endif

void fl_point(int x, int y) {
#ifdef WIN32
  SetPixel(fl_gc, x, y, fl_RGB());
#else
  XDrawPoint(fl_display, fl_window, fl_gc, x, y);
#endif
}

//
// End of "$Id: fl_rect.cxx,v 1.18 2000/05/17 07:08:11 bill Exp $".
//
