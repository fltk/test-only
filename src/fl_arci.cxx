//
// "$Id: fl_arci.cxx,v 1.26 2005/01/27 08:50:40 spitzak Exp $"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#if 0 // THIS FILE IS NO LONGER USED BY FLTK!
#include <config.h>
#include <fltk/draw.h>
#include <fltk/math.h>
#include <fltk/x.h>
using namespace fltk;

/*! Innards of fillpie(), strokepie(), fillarc(), strokearc(),
  fillstrokepie(), and fillstrokearc().

  These functions match the rather limited circle drawing code
  provided by X and WIN32. The advantage over using addarc() is that
  they are faster because they often use the hardware, and that small
  ones draw quite nicely (probably due to stored hand-drawn bitmaps of
  small circles!).

  This is merged into a single function to make writing a dispatch
  table easier. The arc is inscribed in the x,y,w,h rectangle, and
  goes from angle \a a1 to \a a2, measured in degrees counter-clockwise
  from 3'oclock.

  The result if the transformation is rotated is undefined and will
  differ depending on the backend.
*/
void fltk::arci(const Rectangle& r1, float a1, float a2, int what, Color c) {
  Rectangle r(r1); transform(r);
  if (r.empty()) return;

#if USE_X11

  int A = int(a1*64);
  int B = int(a2*64)-A;
  if (what != STROKEARC && r.w()>2 && r.h()>2) {
    XSetArcMode(xdisplay, gc, what == FILLPIE ? ArcPieSlice : ArcChord);
    XFillArc(xdisplay, xwindow, gc, r.x(), r.y(), r.w()-1, r.h()-1, A, B);
  }
  if (what == FILLSTROKEARC) setcolor(c);
  XDrawArc(xdisplay, xwindow, gc, r.x(), r.y(), r.w()-1, r.h()-1, A, B);

#elif defined(_WIN32)
  if (a1 == a2) return;
  if (a2 < a1) {float t = a2; a2 = a1; a1 = t;}
  if (a2 >= a1+360) a1 = a2 = 0;
  //w++; h++; // is this needed to match X?
  int xa = r.center_x()+int(r.w()*cosf(a1*float(M_PI/180.0)));
  int ya = r.center_y()-int(r.h()*sinf(a1*float(M_PI/180.0)));
  int xb = r.center_x()+int(r.w()*cosf(a2*float(M_PI/180.0)));
  int yb = r.center_y()-int(r.h()*sinf(a2*float(M_PI/180.0)));
  switch (what) {
  case FILLPIE:
    setbrush();
    setpen();
    Pie(dc, r.x(), r.y(), r.r(), r.b(), xa, ya, xb, yb);
    break;
  case FILLARC:
    setbrush();
    setpen();
    Chord(dc, r.x(), r.y(), r.r(), r.b(), xa, ya, xb, yb);
    break;
  case STROKEARC:
    setpen();
    Arc(dc, r.x(), r.y(), r.r(), r.b(), xa, ya, xb, yb);
    break;
  case FILLSTROKEARC:
    setbrush();
    setcolor(c);
    setpen();
    Chord(dc, r.x(), r.y(), r.r(), r.b(), xa, ya, xb, yb);
    break;
  }
#elif USE_QUARTZ
//+++ port me!
  a1 = (-a1)/180.0f*M_PI; a2 = (-a2)/180.0f*M_PI;
  float cx = r.center_x()-0.5f, cy = r.center_y()-0.5f;
  float w = r.w(), h = r.h();
  if (w!=h) {
    CGContextSaveGState(quartz_gc);
    CGContextTranslateCTM(quartz_gc, cx, cy);
    CGContextScaleCTM(quartz_gc, w-1.0f, h-1.0f);
    CGContextAddArc(quartz_gc, 0, 0, 0.5, a1, a2, 1);
    CGContextRestoreGState(quartz_gc);
  } else {
    float r = (w+h)*0.25f-0.5f;
    CGContextAddArc(quartz_gc, cx, cy, r, a1, a2, 1);
  }
  switch (what) {
  case FILLPIE:
    CGContextAddLineToPoint(quartz_gc, cx, cy);
    // fall through
  case FILLARC:
    CGContextFillPath(quartz_gc);
    break;
  case STROKEARC:
    CGContextStrokePath(quartz_gc);
    break;
  case FILLSTROKEARC: {
    uchar r, g, b; 
    split_color(c, r, g, b);
    CGContextSetRGBStrokeColor(quartz_gc, r/255.0f, g/255.0f, b/255.0f, 1.0);
    CGContextDrawPath(quartz_gc, kCGPathFillStroke);
    break; }
  }
/*
  Rect rect;
  rect.left=r.x(); rect.right=r.r(); rect.top=r.y(); rect.bottom=r.b();
  a1 = a2-a1; a2 = 450-a2;
  switch (what) {
  case FILLPIE:
  case FILLARC: // not correct, should fill chord
    PaintArc(&rect, (short int)a2, (short int)a1);
    break;
  case STROKEARC:
    FrameArc(&rect, (short int)a2, (short int)a1);
    break;
  case FILLSTROKEARC:
    PaintArc(&rect, (short int)a2, (short int)a1);
    setcolor(c);
    FrameArc(&rect, (short int)a2, (short int)a1);
  }
*/
#else
# error
#endif
}

/*! \fn void fltk::fillpie(const Rectangle&, float a, float a2);
  Draw a pie-slice shape. If the angles are 360 or more degrees apart
  a full circle is drawn. Such a circle will just touch each side
  of the rectangle.
*/

/*! \fn void fltk::fillarc(const Rectangle&, float a, float a2);
  Fill a "chord" shape, with an arc and one straight line joining the
  ends. If the angles are more than 360 degrees apart a fill circle is
  drawn. Such a circle will just touch each side of the rectangle. In
  addition the line drawn by strokearc() if the line_style() is zero,
  will just hide this edge.
*/

/*! \fn void fltk::strokearc(const Rectangle&, float a, float a2);
  Draw a line along the arc. The path is not closed, this just draws
  the curved edge.
*/

/*! \fn void fltk::fillstrokearc(const Rectangle&, float a, float a2, Color);
  Does both fillarc and strokearc, using the Color argument as the
  stroke color. In most implementations fillpie() draws a line anyway
  so this saves time.
*/

#endif

//
// End of "$Id: fl_arci.cxx,v 1.26 2005/01/27 08:50:40 spitzak Exp $".
//
