//
// "$Id: fl_arci.cxx,v 1.19 2004/01/25 06:55:05 spitzak Exp $"
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

#include <fltk/draw.h>
#include <fltk/math.h>
#include <fltk/x.h>
using namespace fltk;

/*!
  These functions match the rather limited circle drawing code
  provided by X and WIN32. The advantage over using addarc() is that
  they are faster because they often use the hardware, and that small
  ones draw quite nicely (probably due to stored hand-drawn bitmaps of
  small circles!).

  This is merged into a single function to make writing a dispatch
  table easier. The arc is inscribed in the x,y,w,h rectangle, and
  goes from angle \a a1 to \a a2, measured in degrees counter-clockwise
  from 3'oclock.

  The default if \a what is not specified is to fill a pie-slice shape
  with the current color. \a what is used to implement the other
  functions and can be one of fltk::FILLPIE, fltk::FILLARC, fltk::STROKEPIE,
  fltk::STROKEARC.
*/
void fltk::fillpie(int x,int y,int w,int h,float a1,float a2, int what) {
  if (w <= 0 || h <= 0) return;
  transform(x,y);
#ifdef _WIN32
  if (a1 == a2) return;
  if (a2 < a1) {float t = a2; a2 = a1; a1 = t;}
  if (a2 >= a1+360) a1 = a2 = 0;
  //w++; h++; // is this needed to match X?
  int xa = x+w/2+int(w*cosf(a1*float(M_PI/180.0)));
  int ya = y+h/2-int(h*sinf(a1*float(M_PI/180.0)));
  int xb = x+w/2+int(w*cosf(a2*float(M_PI/180.0)));
  int yb = y+h/2-int(h*sinf(a2*float(M_PI/180.0)));
  switch (what) {
  case FILLPIE:
    setbrush();
    setpen();
    Pie(gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  case FILLARC:
    setbrush();
    setpen();
    Chord(gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  case STROKEPIE: // not correct, should draw lines to center
  case STROKEARC:
    setpen();
    Arc(gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  }
#elif (defined(__APPLE__) && !USE_X11)
  Rect r; r.left=x; r.right=x+w; r.top=y; r.bottom=y+h;
  a1 = a2-a1; a2 = 450-a2;
  switch (what) {
  case FILLPIE:
  case FILLARC: // not correct, should fill chord
    PaintArc(&r, (short int)a2, (short int)a1);
    break;
  case STROKEPIE: // not correct, should draw lines to center
  case STROKEARC:
    FrameArc(&r, (short int)a2, (short int)a1);
    break;
  }
#else
  int A = int(a1*64);
  int B = int(a2*64)-A;
  switch(what) {
  case FILLPIE:
    XSetArcMode(xdisplay, gc, ArcPieSlice);
    goto J1;
  case FILLARC:
    XSetArcMode(xdisplay, gc, ArcChord);
  J1:
    XFillArc(xdisplay, xwindow, gc, x-1, y-1, w+1, h+1, A, B);
    break;
  case STROKEPIE: // not correct, should draw lines to center
  case STROKEARC:
    XDrawArc(xdisplay, xwindow, gc, x, y, w, h, A, B);
    break;
  }
#endif
}

/*! \fn void fltk::strokepie(int x,int y,int w,int h,float a,float a2);
  Draw a line along a closed path around the pie slice, including two
  straight lines from the ends of the arc to the center. (nyi, right
  now it acts like strokearc().
*/

/*! \fn void fltk::fillarc(int x,int y,int w,int h,float a,float a2);
  Fill a "chord" shape, with an arc and one straight line joining the
  ends.
*/

/*! \fn void fltk::strokearc(int x,int y,int w,int h,float a,float a2);
  Draw a line along the arc. The path is not closed, this just draws
  the curved edge.
*/

//
// End of "$Id: fl_arci.cxx,v 1.19 2004/01/25 06:55:05 spitzak Exp $".
//
