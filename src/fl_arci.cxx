//
// "$Id: fl_arci.cxx,v 1.15 2003/02/21 18:16:53 spitzak Exp $"
//
// Arc (integer) drawing functions for the Fast Light Tool Kit (FLTK).
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

// Draw pie slices, chord shapes, and curved lines, using the facilities
// of X or Windows instead of the line segment drawing that arc(...)
// uses.  These draw the limited
// circle types provided by X and NT graphics.  The advantage of
// these is that small ones draw quite nicely (probably due to stored
// hand-drawn bitmaps of small circles!) and may be implemented by
// hardware and thus are fast.
// This is merged into a single function to make writing a dispatch
// table easier.

#include <fltk/draw.h>
#include <fltk/math.h>
#include <fltk/x.h>
using namespace fltk;

void fltk::fillpie(int x,int y,int w,int h,float a1,float a2, int what) {
  if (w <= 0 || h <= 0) return;
  transform(x,y);
#ifdef _WIN32
  if (a1 == a2) return;
  w++; h++; // is this needed to match X?
  int xa = x+w/2+int(w*cosf(a1*float(M_PI/180.0)));
  int ya = y+h/2-int(h*sinf(a1*float(M_PI/180.0)));
  int xb = x+w/2+int(w*cosf(a2*float(M_PI/180.0)));
  int yb = y+h/2-int(h*sinf(a2*float(M_PI/180.0)));
  switch (what) {
  case FILLPIE:
    Pie(gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  case FILLARC:
    Chord(gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  case STROKEPIE: // not correct, should draw lines to center
  case STROKEARC:
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
    XFillArc(xdisplay, xwindow, gc, x, y, w, h, A, B);
    break;
  case STROKEPIE: // not correct, should draw lines to center
  case STROKEARC:
    XDrawArc(xdisplay, xwindow, gc, x, y, w, h, A, B);
    break;
  }
#endif
}

//
// End of "$Id: fl_arci.cxx,v 1.15 2003/02/21 18:16:53 spitzak Exp $".
//
