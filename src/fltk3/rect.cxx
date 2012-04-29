//
// "$Id$"
//
// Rectangle drawing routines for the Fast Light Tool Kit (FLTK).
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

/**
  \file fl_rect.cxx
  \brief Drawing and clipping routines for rectangles.
*/

// These routines from draw.h are used by the standard boxtypes
// and thus are always linked into an fltk program.
// Also all fl_clip routines, since they are always linked in so
// that minimal update works.

#include <config.h>
#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/Printer.h>
#include <fltk3/draw.h>
#include <fltk3/x.h>

// fl_line_width_ must contain the absolute value of the current
// line width to be used for X11 clipping (see below).
// This is defined in src/fltk3::line_style.cxx
extern int fl_line_width_;

#ifdef __APPLE_QUARTZ__
extern float fl_quartz_line_width_;
#endif

#ifdef USE_X11

#ifndef SHRT_MAX
#define SHRT_MAX (32767)
#endif

/*
  We need to check some coordinates for areas for clipping before we
  use X functions, because X can't handle coordinates outside the 16-bit
  range. Since all windows use relative coordinates > 0, we do also
  check for negative values. X11 only, see also STR #2304.
  
  Note that this is only necessary for large objects, where only a
  part of the object is visible. The draw() functions (e.g. box
  drawing) must be clipped correctly. This is usually only a matter
  for large container widgets. The individual child widgets will be
  clipped completely.

  We define the usable X coordinate space as [ -LW : SHRT_MAX - LW ]
  where LW = current line width for drawing. This is done so that
  horizontal and vertical line drawing works correctly, even in real
  border cases, e.g. drawing a rectangle slightly outside the top left
  window corner, but with a line width so that a part of the line should
  be visible (in this case 2 of 5 pixels):

    fltk3::line_style (fltk3::SOLID,5);	// line width = 5
    fltk3::rect (-1,-1,100,100);	// top/left: 2 pixels visible
  
  In this example case, no clipping would be done, because X can
  handle it and clip unneeded pixels.
  
  Note that we must also take care of the case where fl_line_width_
  is zero (maybe unitialized). If this is the case, we assume a line
  width of 1.

  Todo: Arbitrary line drawings (e.g. polygons) and clip regions
  are not yet done.

  Note:

  We could use max. screen coordinates instead of SHRT_MAX, but that
  would need more work and would probably be slower. We assume that
  all window coordinates are >= 0 and that no window extends up to
  32767 - LW (where LW = current line width). Thus it is safe to clip
  all coordinates to this range before calling X functions. If this
  is not true, then clip_to_short() and clip_x() must be redefined.

  It would be somewhat easier if we had fl_clip_w and fl_clip_h, as
  defined in FLTK 2.0 (for the upper clipping bounds)...
*/

/*
  clip_to_short() returns 1, if the area is invisible (clipped),
  because ...

    (a) w or h are <= 0		i.e. nothing is visible
    (b) x+w or y+h are < kmin	i.e. left of or above visible area
    (c) x or y are > kmax	i.e. right of or below visible area

  kmin and kmax are the minimal and maximal X coordinate values,
  as defined above. In this case x, y, w, and h are not changed.

  It returns 0, if the area is potentially visible and X can handle
  clipping. x, y, w, and h may have been adjusted to fit into the
  X coordinate space.

  Use this for clipping rectangles, as used in fltk3::rect() and
  fltk3::rectf().
*/

static int clip_to_short(int &x, int &y, int &w, int &h) {

  int lw = (fl_line_width_ > 0) ? fl_line_width_ : 1;
  int kmin = -lw;
  int kmax = SHRT_MAX - lw;

  if (w <= 0 || h <= 0) return 1;		// (a)
  if (x+w < kmin || y+h < kmin) return 1;	// (b)
  if (x > kmax || y > kmax) return 1;		// (c)

  if (x < kmin) { w -= (kmin-x); x = kmin; }
  if (y < kmin) { h -= (kmin-y); y = kmin; }
  if (x+w > kmax) w = kmax - x;
  if (y+h > kmax) h = kmax - y;

  return 0;
}

/*
  clip_x() returns a coordinate value clipped to the 16-bit coordinate
  space (see above). This can be used to draw horizontal and vertical
  lines that can be handled by X11. Each single coordinate value can
  be clipped individually, and the result can be used directly, e.g.
  in fltk3::xyline() and fltk3::yxline(). Note that this can't be used for
  arbitrary lines (not horizontal or vertical).
*/
static int clip_x (int x) {

  int lw = (fl_line_width_ > 0) ? fl_line_width_ : 1;
  int kmin = -lw;
  int kmax = SHRT_MAX - lw;

  if (x < kmin)
    x = kmin;
  else if (x > kmax)
    x = kmax;
  return x;
}

#endif	// USE_X11


void fltk3::GraphicsDriver::push_origin() {
  if (optr==origin_stack_size)
    fltk3::error("fltk3::push_origin(): origin stack overflow.");
  else
    ostack[optr++] = o;
}

void fltk3::GraphicsDriver::pop_origin() {
  if (optr==0)
    fltk3::error("fltk3::pop_origin(): origin stack underflow.");
  else 
    o = ostack[--optr];
}

void fltk3::GraphicsDriver::translate_origin(int dx, int dy) {
  o.x += dx;
  o.y += dy;
}

void fltk3::GraphicsDriver::origin(int x, int y) {
  o.x = x;
  o.y = y;
}


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::rect(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  x += origin_x(); y += origin_y();
  if ( fltk3::SurfaceDevice::surface() == fltk3::DisplayDevice::display_device() && fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  CGRect rect = CGRectMake(x, y, w-1, h-1);
  CGContextStrokeRect(fl_gc, rect);
  if ( fltk3::SurfaceDevice::surface() == fltk3::DisplayDevice::display_device() && fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::rect(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  x += origin_x(); y += origin_y();
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x+w-1, y);
  LineTo(fl_gc, x+w-1, y+h-1);
  LineTo(fl_gc, x, y+h-1);
  LineTo(fl_gc, x, y);
}
#else
void fltk3::XlibGraphicsDriver::rect(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  x += origin_x(); y += origin_y();
  if (!clip_to_short(x, y, w, h))
    XDrawRectangle(fl_display, fl_window, fl_gc, x, y, w-1, h-1);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  x += origin_x(); y += origin_y();
  CGRect rect = CGRectMake(x, y, w-0.9, h-0.9);
  CGContextFillRect(fl_gc, rect);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  x += origin_x(); y += origin_y();
  RECT rect;
  rect.left = x; rect.top = y;  
  rect.right = x + w; rect.bottom = y + h;
  FillRect(fl_gc, &rect, fl_brush());
}
#else
void fltk3::XlibGraphicsDriver::rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  x += origin_x(); y += origin_y();
  if (!clip_to_short(x, y, w, h))
    XFillRectangle(fl_display, fl_window, fl_gc, x, y, w, h);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::xyline(int x, int y, int x1) {
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); x1 += origin_x();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y);
  CGContextStrokePath(fl_gc);
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::xyline(int x, int y, int x1) {
  x += origin_x(); y += origin_y(); x1 += origin_x();
  MoveToEx(fl_gc, x, y, 0L); LineTo(fl_gc, x1+1, y);
}
#else
void fltk3::XlibGraphicsDriver::xyline(int x, int y, int x1) {
  x += origin_x(); y += origin_y(); x1 += origin_x();
  XDrawLine(fl_display, fl_window, fl_gc, clip_x(x), clip_x(y), clip_x(x1), clip_x(y));
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::xyline(int x, int y, int x1, int y2) {
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y);
  CGContextAddLineToPoint(fl_gc, x1, y2);
  CGContextStrokePath(fl_gc);
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::xyline(int x, int y, int x1, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y();
  if (y2 < y) y2--;
  else y2++;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y);
  LineTo(fl_gc, x1, y2);
}
#else
void fltk3::XlibGraphicsDriver::xyline(int x, int y, int x1, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y();
  XPoint p[3];
  p[0].x = clip_x(x);  p[0].y = p[1].y = clip_x(y);
  p[1].x = p[2].x = clip_x(x1); p[2].y = clip_x(y2);
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::xyline(int x, int y, int x1, int y2, int x3) {
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y(); x3 += origin_x();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y);
  CGContextAddLineToPoint(fl_gc, x1, y2);
  CGContextAddLineToPoint(fl_gc, x3, y2);
  CGContextStrokePath(fl_gc);
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::xyline(int x, int y, int x1, int y2, int x3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y(); x3 += origin_x();
  if(x3 < x1) x3--;
  else x3++;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y);
  LineTo(fl_gc, x1, y2);
  LineTo(fl_gc, x3, y2);
}
#else
void fltk3::XlibGraphicsDriver::xyline(int x, int y, int x1, int y2, int x3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y(); x3 += origin_x();
  XPoint p[4];
  p[0].x = clip_x(x);  p[0].y = p[1].y = clip_x(y);
  p[1].x = p[2].x = clip_x(x1); p[2].y = p[3].y = clip_x(y2);
  p[3].x = clip_x(x3);
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::yxline(int x, int y, int y1) {
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); y1 += origin_y();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x, y1);
  CGContextStrokePath(fl_gc);
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::yxline(int x, int y, int y1) {
  x += origin_x(); y += origin_y(); y1 += origin_y();
  if (y1 < y) y1--;
  else y1++;
  MoveToEx(fl_gc, x, y, 0L); LineTo(fl_gc, x, y1);
}
#else
void fltk3::XlibGraphicsDriver::yxline(int x, int y, int y1) {
  x += origin_x(); y += origin_y(); y1 += origin_y();
  XDrawLine(fl_display, fl_window, fl_gc, clip_x(x), clip_x(y), clip_x(x), clip_x(y1));
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::yxline(int x, int y, int y1, int x2) {
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x, y1);
  CGContextAddLineToPoint(fl_gc, x2, y1);
  CGContextStrokePath(fl_gc);
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::yxline(int x, int y, int y1, int x2) {
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x();
  if (x2 > x) x2++;
  else x2--;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x, y1);
  LineTo(fl_gc, x2, y1);
}
#else
void fltk3::XlibGraphicsDriver::yxline(int x, int y, int y1, int x2) {
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x();
  XPoint p[3];
  p[0].x = p[1].x = clip_x(x);  p[0].y = clip_x(y);
  p[1].y = p[2].y = clip_x(y1); p[2].x = clip_x(x2);
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::yxline(int x, int y, int y1, int x2, int y3) {
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x(); y3 += origin_y();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x, y1);
  CGContextAddLineToPoint(fl_gc, x2, y1);
  CGContextAddLineToPoint(fl_gc, x2, y3);
  CGContextStrokePath(fl_gc);
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device() || fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::yxline(int x, int y, int y1, int x2, int y3) {
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x(); y3 += origin_y();
  if(y3<y1) y3--;
  else y3++;
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x, y1);
  LineTo(fl_gc, x2, y1);
  LineTo(fl_gc, x2, y3);
}
#else
void fltk3::XlibGraphicsDriver::yxline(int x, int y, int y1, int x2, int y3) {
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x(); y3 += origin_y();
  XPoint p[4];
  p[0].x = p[1].x = clip_x(x);  p[0].y = clip_x(y);
  p[1].y = p[2].y = clip_x(y1); p[2].x = p[3].x = clip_x(x2);
  p[3].y = clip_x(y3);
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::line(int x, int y, int x1, int y1) {
  if (fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y1);
  CGContextStrokePath(fl_gc);
  if (fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::line(int x, int y, int x1, int y1) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y();
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  SetPixel(fl_gc, x1, y1, fl_RGB());
}
#else
void fltk3::XlibGraphicsDriver::line(int x, int y, int x1, int y1) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y();
  XDrawLine(fl_display, fl_window, fl_gc, x, y, x1, y1);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::line(int x, int y, int x1, int y1, int x2, int y2) {
  if (fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, true);
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y1);
  CGContextAddLineToPoint(fl_gc, x2, y2);
  CGContextStrokePath(fl_gc);
  if (fl_quartz_line_width_ > 1.5f) CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::line(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  LineTo(fl_gc, x2, y2);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  SetPixel(fl_gc, x2, y2, fl_RGB());
}
#else
void fltk3::XlibGraphicsDriver::line(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  XPoint p[3];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::loop(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  CGContextSetShouldAntialias(fl_gc, true);
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y1);
  CGContextAddLineToPoint(fl_gc, x2, y2);
  CGContextClosePath(fl_gc);
  CGContextStrokePath(fl_gc);
  CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::loop(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  LineTo(fl_gc, x2, y2);
  LineTo(fl_gc, x, y);
}
#else
void fltk3::XlibGraphicsDriver::loop(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x;  p[3].y = y;
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  CGContextSetShouldAntialias(fl_gc, true);
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y1);
  CGContextAddLineToPoint(fl_gc, x2, y2);
  CGContextAddLineToPoint(fl_gc, x3, y3);
  CGContextClosePath(fl_gc);
  CGContextStrokePath(fl_gc);
  CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  LineTo(fl_gc, x2, y2);
  LineTo(fl_gc, x3, y3);
  LineTo(fl_gc, x, y);
}
#else
void fltk3::XlibGraphicsDriver::loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  XPoint p[5];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;
  p[4].x = x;  p[4].y = y;
  XDrawLines(fl_display, fl_window, fl_gc, p, 5, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::polygon(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  CGContextSetShouldAntialias(fl_gc, true);
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y1);
  CGContextAddLineToPoint(fl_gc, x2, y2);
  CGContextClosePath(fl_gc);
  CGContextFillPath(fl_gc);
  CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::polygon(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  XPoint p[3];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  SelectObject(fl_gc, fl_brush());
  Polygon(fl_gc, p, 3);
}
#else
void fltk3::XlibGraphicsDriver::polygon(int x, int y, int x1, int y1, int x2, int y2) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x;  p[3].y = y;
  XFillPolygon(fl_display, fl_window, fl_gc, p, 3, Convex, 0);
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  CGContextSetShouldAntialias(fl_gc, true);
  CGContextMoveToPoint(fl_gc, x, y);
  CGContextAddLineToPoint(fl_gc, x1, y1);
  CGContextAddLineToPoint(fl_gc, x2, y2);
  CGContextAddLineToPoint(fl_gc, x3, y3);
  CGContextClosePath(fl_gc);
  CGContextFillPath(fl_gc);
  CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;
  SelectObject(fl_gc, fl_brush());
  Polygon(fl_gc, p, 4);
}
#else
void fltk3::XlibGraphicsDriver::polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  x += origin_x(); y += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  XPoint p[5];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;
  p[4].x = x;  p[4].y = y;
  XFillPolygon(fl_display, fl_window, fl_gc, p, 4, Convex, 0);
  XDrawLines(fl_display, fl_window, fl_gc, p, 5, 0);
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::point(int x, int y) {
  x += origin_x(); y += origin_y();
  CGContextFillRect(fl_gc, CGRectMake(x - 0.5, y - 0.5, 1, 1) );
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::point(int x, int y) {
  x += origin_x(); y += origin_y();
  SetPixel(fl_gc, x, y, fl_RGB());
}
#else
void fltk3::XlibGraphicsDriver::point(int x, int y) {
  x += origin_x(); y += origin_y();
  XDrawPoint(fl_display, fl_window, fl_gc, clip_x(x), clip_x(y));
}
#endif

////////////////////////////////////////////////////////////////

#if !defined(WIN32) && !defined(__APPLE__)
// Missing X call: (is this the fastest way to init a 1-rectangle region?)
// MSWindows equivalent exists, implemented inline in win32.h
fltk3::Region XRectangleRegion(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  XRectangle R;
  clip_to_short(x, y, w, h);
  R.x = x; R.y = y; R.width = w; R.height = h;
  fltk3::Region r = XCreateRegion();
  XUnionRectWithRegion(&R, r, r);
  return r;
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::restore_clip() {
  fl_clip_state_number++;
  fltk3::Region r = clip_region();
  if ( fl_window ) { // clipping for a true window
    Fl_X::q_clear_clipping();
    Fl_X::q_fill_context();//flip coords if bitmap context
    //apply program clip
    if (r) {
      CGContextClipToRects(fl_gc, r->rects, r->count);
    }
  } else if (fl_gc) { // clipping for an offscreen drawing world (CGBitmap)
    Fl_X::q_clear_clipping();
    Fl_X::q_fill_context();
    if (r) {
      CGContextClipToRects(fl_gc, r->rects, r->count);
    }
  }
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::restore_clip() {
  fl_clip_state_number++;
  fltk3::Region r = clip_region();
  SelectClipRgn(fl_gc, r); //if r is NULL, clip is automatically cleared
}
#else
void fltk3::XlibGraphicsDriver::restore_clip() {
  fl_clip_state_number++;
  fltk3::Region r = clip_region();
  if (r) XSetRegion(fl_display, fl_gc, r);
  else XSetClipMask(fl_display, fl_gc, 0);
}
#endif


void fltk3::GraphicsDriver::clip_region(fltk3::Region r) {
  fltk3::Region oldr = rstack[rstackptr];
  if (oldr) XDestroyRegion(oldr);
  rstack[rstackptr] = r;
  restore_clip();
}

void fltk3::GraphicsDriver::region_stack_push(fltk3::Region r) {
  if (rstackptr < region_stack_max) rstack[++rstackptr] = r;
  else fltk3::warning("fltk3::push_clip: clip stack overflow!\n");
}

#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::push_clip(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  fltk3::Region r;
  if (w > 0 && h > 0) {
    r = XRectangleRegion(x,y,w,h);
    fltk3::Region current = clip_region();
    if (current) {
      XDestroyRegion(r);
      r = Fl_X::intersect_region_and_rect(current, x,y,w,h);
    }
  } else { // make empty clip region:
    r = XRectangleRegion(0,0,0,0);
  }
  region_stack_push(r);
  restore_clip();
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::push_clip(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  fltk3::Region r;
  if (w > 0 && h > 0) {
    r = XRectangleRegion(x,y,w,h);
    fltk3::Region current = clip_region();
    if (current) {
      CombineRgn(r,r,current,RGN_AND);
    }
  } else { // make empty clip region:
    r = CreateRectRgn(0,0,0,0);
  }
  region_stack_push(r);
  restore_clip();
}
#else
void fltk3::XlibGraphicsDriver::push_clip(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  fltk3::Region r;
  if (w > 0 && h > 0) {
    r = XRectangleRegion(x,y,w,h);
    fltk3::Region current = clip_region();
    if (current) {
      fltk3::Region temp = XCreateRegion();
      XIntersectRegion(current, r, temp);
      XDestroyRegion(r);
      r = temp;
    }
  } else { // make empty clip region:
    r = XCreateRegion();
  }
  region_stack_push(r);
  restore_clip();
}
#endif


// make there be no clip (used by fl_begin_offscreen() only!)
void fltk3::GraphicsDriver::push_no_clip() {
  if (rstackptr < region_stack_max) rstack[++rstackptr] = 0;
  else fltk3::warning("fltk3::push_no_clip: clip stack overflow!\n");
  restore_clip();
}

// pop back to previous clip:
void fltk3::GraphicsDriver::pop_clip() {
  if (rstackptr > 0) {
    fltk3::Region oldr = rstack[rstackptr--];
    if (oldr) XDestroyRegion(oldr);
  } else fltk3::warning("fltk3::pop_clip: clip stack underflow!\n");
  restore_clip();
}


#if defined(__APPLE_QUARTZ__)
int fltk3::QuartzGraphicsDriver::not_clipped(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  if (x+w <= 0 || y+h <= 0) return 0;
  fltk3::Region r = clip_region();
  if (!r) return 1;
  CGRect arg = fl_cgrectmake_cocoa(x, y, w, h);
  for (int i = 0; i < r->count; i++) {
    CGRect test = CGRectIntersection(r->rects[i], arg);
    if (!CGRectIsEmpty(test)) return 1;
  }
  return 0;
}
#elif defined(WIN32)
int fltk3::GDIGraphicsDriver::not_clipped(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  if (x+w <= 0 || y+h <= 0) return 0;
  fltk3::Region r = clip_region();
  if (!r) return 1;
  RECT rect;
  if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device()) { // in case of print context, convert coords from logical to device
    POINT pt[2] = { {x, y}, {x + w, y + h} };
    LPtoDP(fl_gc, pt, 2);
    rect.left = pt[0].x; rect.top = pt[0].y; rect.right = pt[1].x; rect.bottom = pt[1].y;
  } else {
    rect.left = x; rect.top = y; rect.right = x+w; rect.bottom = y+h;
  }
  return RectInRegion(r,&rect);
}
#else
int fltk3::XlibGraphicsDriver::not_clipped(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  if (x+w <= 0 || y+h <= 0) return 0;
  fltk3::Region r = clip_region();
  if (!r) return 1;
  // get rid of coordinates outside the 16-bit range the X calls take.
  if (clip_to_short(x,y,w,h)) return 0;	// clipped
  return XRectInRegion(r, x, y, w, h);
}
#endif


// return rectangle surrounding intersection of this rectangle and clip:
#if defined(__APPLE_QUARTZ__)
int fltk3::QuartzGraphicsDriver::clip_box(int x, int y, int w, int h, int& X, int& Y, int& W, int& H){
  x += origin_x(); y += origin_y();
  X = x; Y = y; W = w; H = h;
  fltk3::Region r = clip_region();
  if (!r) return 0;
  CGRect arg = fl_cgrectmake_cocoa(x, y, w, h);
  CGRect u = CGRectMake(0,0,0,0);
  CGRect test;
  for(int i = 0; i < r->count; i++) {
    test = CGRectIntersection(r->rects[i], arg);
    if( ! CGRectIsEmpty(test) ) {
      if(CGRectIsEmpty(u)) u = test;
      else u = CGRectUnion(u, test);
    }
  }
  X = int(u.origin.x);
  Y = int(u.origin.y);
  W = int(u.size.width + 1);
  H = int(u.size.height + 1);
  if(CGRectIsEmpty(u)) W = H = 0;
  X -= origin_x(); Y -= origin_y();
  return ! CGRectEqualToRect(arg, u);
}
#elif defined(WIN32)
int fltk3::GDIGraphicsDriver::clip_box(int x, int y, int w, int h, int& X, int& Y, int& W, int& H){
  x += origin_x(); y += origin_y();
  X = x; Y = y; W = w; H = h;
  fltk3::Region r = clip_region();
  if (!r) return 0;
  // The win32 API makes no distinction between partial and complete
  // intersection, so we have to check for partial intersection ourselves.
  // However, given that the regions may be composite, we have to do
  // some voodoo stuff...
  fltk3::Region rr = XRectangleRegion(x,y,w,h);
  fltk3::Region temp = CreateRectRgn(0,0,0,0);
  int ret;
  if (CombineRgn(temp, rr, r, RGN_AND) == NULLREGION) { // disjoint
    W = H = 0;
    ret = 2;
  } else if (EqualRgn(temp, rr)) { // complete
    ret = 0;
  } else {	// partial intersection
    RECT rect;
    GetRgnBox(temp, &rect);
    if (fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device()) { // if print context, convert coords from device to logical
      POINT pt[2] = { {rect.left, rect.top}, {rect.right, rect.bottom} };
      DPtoLP(fl_gc, pt, 2);
      X = pt[0].x; Y = pt[0].y; W = pt[1].x - X; H = pt[1].y - Y;
    }
    else {
      X = rect.left; Y = rect.top; W = rect.right - X; H = rect.bottom - Y;
    }
    ret = 1;
  }
  DeleteObject(temp);
  DeleteObject(rr);
  X -= origin_x(); Y -= origin_y();
  return ret;
}
#else
int fltk3::XlibGraphicsDriver::clip_box(int x, int y, int w, int h, int& X, int& Y, int& W, int& H){
  x += origin_x(); y += origin_y();
  X = x; Y = y; W = w; H = h;
  fltk3::Region r = clip_region();
  if (!r) return 0;
  switch (XRectInRegion(r, x, y, w, h)) {
    case 0: // completely outside
      W = H = 0;
      return 2;
    case 1: // completely inside:
      return 0;
    default: // partial:
      break;
  }
  fltk3::Region rr = XRectangleRegion(x,y,w,h);
  fltk3::Region temp = XCreateRegion();
  XIntersectRegion(r, rr, temp);
  XRectangle rect;
  XClipBox(temp, &rect);
  X = rect.x; Y = rect.y; W = rect.width; H = rect.height;
  XDestroyRegion(temp);
  XDestroyRegion(rr);
  X -= origin_x(); Y -= origin_y();
  return 1;
}
#endif

//
// End of "$Id$".
//
