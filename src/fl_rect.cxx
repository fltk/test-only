//
// "$Id: fl_rect.cxx,v 1.37 2004/03/25 18:13:18 spitzak Exp $"
//
// Non-path routines from draw.h that are used by the standard boxtypes
// and thus are always linked into an fltk program.
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

#include <config.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <fltk/math.h>
using namespace fltk;

/** \defgroup rectangle Simple Drawing
    \ingroup drawing

    These functions bypass the path construction mechanism to draw
    some common shapes more quickly with calls to Xlib or GDI32.  Only
    the x,y coordinates are transformed and in most cases they are
    rounded to the nearest integer.

  \{
*/

/*! 
  Draw a line \e inside this bounding box (currently correct only for
  0-thickness lines).
*/
void fltk::strokerect(int x, int y, int w, int h) {
  if (w <= 0 || h <= 0) return;
  transform(x,y);
#if USE_X11
  XDrawRectangle(xdisplay, xwindow, gc, x, y, w-1, h-1);
#elif defined(_WIN32)
  setpen();
  MoveToEx(gc, x, y, 0L); 
  LineTo(gc, x+w-1, y);
  LineTo(gc, x+w-1, y+h-1);
  LineTo(gc, x, y+h-1);
  LineTo(gc, x, y);
#elif defined(__APPLE__)
  Rect rect;
  SetRect(&rect, x, y, x+w, y+h);
  FrameRect(&rect);
#else
#error
#endif
}

/*! Fill the rectangle with the current color. */
void fltk::fillrect(int x, int y, int w, int h) {
  if (w <= 0 || h <= 0) return;
  transform(x,y);
#if USE_X11
  XFillRectangle(xdisplay, xwindow, gc, x, y, w, h);
#elif defined(_WIN32)
  RECT rect;
  rect.left = x; rect.top = y;  
  rect.right = x + w; rect.bottom = y + h;
  SetBkColor(gc, current_xpixel);
  ExtTextOut(gc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
#elif defined(__APPLE__)
  Rect rect;
  SetRect(&rect, x, y, x+w, y+h);
  PaintRect(&rect);
#else
#error
#endif
}

/*! Draw a straight line between the two points. */
void fltk::drawline(int x, int y, int x1, int y1) {
  transform(x,y);
  transform(x1,y1);
#if USE_X11
  XDrawLine(xdisplay, xwindow, gc, x, y, x1, y1);
#elif defined(_WIN32)
  setpen();
  MoveToEx(gc, x, y, 0L); 
  LineTo(gc, x1, y1);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  // fltk is supposed to act like there is a 1-pixel pen.
  SetPixel(gc, x1, y1, current_xpixel);
#elif defined(__APPLE__)
  MoveTo(x, y); 
  LineTo(x1, y1);
#else
#error
#endif
}

/*! Draw a straight line between the two points. */
void fltk::drawline(float X, float Y, float X1, float Y1) {
  transform(X,Y); int x = int(floorf(X)+.5); int y = int(floorf(Y)+.5);
  transform(X1,Y1);int x1 = int(floorf(X1)+.5); int y1 = int(floorf(Y1)+.5);
#if USE_X11
  XDrawLine(xdisplay, xwindow, gc, x, y, x1, y1);
#elif defined(_WIN32)
  setpen();
  MoveToEx(gc, x, y, 0L); 
  LineTo(gc, x1, y1);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  // fltk is supposed to act like there is a 1-pixel pen.
#elif defined(__APPLE__)
  MoveTo(x, y); 
  LineTo(x1, y1);
#else
#error
#endif
}

/*! Draw a dot at the given point. */
void fltk::drawpoint(int x, int y) {
  transform(x,y);
#if USE_X11
  XDrawPoint(xdisplay, xwindow, gc, x, y);
#elif defined(_WIN32)
  SetPixel(gc, x, y, current_xpixel);
#elif defined(__APPLE__)
  MoveTo(x, y); Line(0, 0);
#else
#error
#endif
}

/*! Draw a dot at the given point. */
void fltk::drawpoint(float X, float Y) {
  transform(X,Y); int x = int(floorf(X)); int y = int(floorf(Y));
#if USE_X11
  XDrawPoint(xdisplay, xwindow, gc, x, y);
#elif defined(_WIN32)
  SetPixel(gc, x, y, current_xpixel);
#elif defined(__APPLE__)
  MoveTo(x, y); Line(0, 0);
#else
#error
#endif
}

/** \} */

//
// End of "$Id: fl_rect.cxx,v 1.37 2004/03/25 18:13:18 spitzak Exp $".
//
