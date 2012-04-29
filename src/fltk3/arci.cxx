//
// "$Id$"
//
// Arc (integer) drawing functions for the Fast Light Tool Kit (FLTK).
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
  \file fl_arci.cxx
  \brief Utility functions for drawing circles using integers
*/

// "integer" circle drawing functions.  These draw the limited
// circle types provided by X and NT graphics.  The advantage of
// these is that small ones draw quite nicely (probably due to stored
// hand-drawn bitmaps of small circles!) and may be implemented by
// hardware and thus are fast.

// Probably should add fltk3::chord.

// 3/10/98: created

#include <fltk3/draw.h>
#include <fltk3/x.h>
#ifdef WIN32
#  include <fltk3/math.h>
#endif
#include <config.h>

#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::arc(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  x += origin_x(); y += origin_y();
  a1 = (-a1)/180.0f*M_PI; a2 = (-a2)/180.0f*M_PI;
  float cx = x + 0.5f*w - 0.5f, cy = y + 0.5f*h - 0.5f;
  CGContextSetShouldAntialias(fl_gc, true);
  if (w!=h) {
    CGContextSaveGState(fl_gc);
    CGContextTranslateCTM(fl_gc, cx, cy);
    CGContextScaleCTM(fl_gc, w-1.0f, h-1.0f);
    CGContextAddArc(fl_gc, 0, 0, 0.5, a1, a2, 1);
    CGContextRestoreGState(fl_gc);
  } else {
    float r = (w+h)*0.25f-0.5f;
    CGContextAddArc(fl_gc, cx, cy, r, a1, a2, 1);
  }
  CGContextStrokePath(fl_gc);
  CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::arc(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  x += origin_x(); y += origin_y();
  int xa = x+w/2+int(w*cos(a1/180.0*M_PI));
  int ya = y+h/2-int(h*sin(a1/180.0*M_PI));
  int xb = x+w/2+int(w*cos(a2/180.0*M_PI));
  int yb = y+h/2-int(h*sin(a2/180.0*M_PI));
  if (fabs(a1 - a2) < 90) {
    if (xa == xb && ya == yb) SetPixel(fl_gc, xa, ya, fl_RGB());
    else Arc(fl_gc, x, y, x+w, y+h, xa, ya, xb, yb);
  } else Arc(fl_gc, x, y, x+w, y+h, xa, ya, xb, yb);
}
#else
void fltk3::XlibGraphicsDriver::arc(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  x += origin_x(); y += origin_y();
  XDrawArc(fl_display, fl_window, fl_gc, x,y,w-1,h-1, int(a1*64),int((a2-a1)*64));
}
#endif


#if defined(__APPLE_QUARTZ__)
void fltk3::QuartzGraphicsDriver::pie(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  x += origin_x(); y += origin_y();
  a1 = (-a1)/180.0f*M_PI; a2 = (-a2)/180.0f*M_PI;
  float cx = x + 0.5f*w - 0.5f, cy = y + 0.5f*h - 0.5f;
  CGContextSetShouldAntialias(fl_gc, true);
  if (w!=h) {
    CGContextSaveGState(fl_gc);
    CGContextTranslateCTM(fl_gc, cx, cy);
    CGContextScaleCTM(fl_gc, w, h);
    CGContextAddArc(fl_gc, 0, 0, 0.5, a1, a2, 1);
    CGContextAddLineToPoint(fl_gc, 0, 0);
    CGContextClosePath(fl_gc);
    CGContextRestoreGState(fl_gc);
  } else {
    float r = (w+h)*0.25f;
    CGContextAddArc(fl_gc, cx, cy, r, a1, a2, 1);
    CGContextAddLineToPoint(fl_gc, cx, cy);
    CGContextClosePath(fl_gc);
  }
  CGContextFillPath(fl_gc);
  CGContextSetShouldAntialias(fl_gc, false);
}
#elif defined(WIN32)
void fltk3::GDIGraphicsDriver::pie(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  x += origin_x(); y += origin_y();
  if (a1 == a2) return;
  int xa = x+w/2+int(w*cos(a1/180.0*M_PI));
  int ya = y+h/2-int(h*sin(a1/180.0*M_PI));
  int xb = x+w/2+int(w*cos(a2/180.0*M_PI));
  int yb = y+h/2-int(h*sin(a2/180.0*M_PI));
  SelectObject(fl_gc, fl_brush());
  if (fabs(a1 - a2) < 90) {
    if (xa == xb && ya == yb) {
      MoveToEx(fl_gc, x+w/2, y+h/2, 0L); 
      LineTo(fl_gc, xa, ya);
      SetPixel(fl_gc, xa, ya, fl_RGB());
    } else Pie(fl_gc, x, y, x+w, y+h, xa, ya, xb, yb);
  } else Pie(fl_gc, x, y, x+w, y+h, xa, ya, xb, yb); 
}
#else
void fltk3::XlibGraphicsDriver::pie(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  x += origin_x(); y += origin_y();
  XDrawArc(fl_display, fl_window, fl_gc, x,y,w-1,h-1, int(a1*64),int((a2-a1)*64));
  XFillArc(fl_display, fl_window, fl_gc, x,y,w-1,h-1, int(a1*64),int((a2-a1)*64));
}
#endif
//
// End of "$Id$".
//
