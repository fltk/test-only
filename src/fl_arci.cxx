//
// "$Id: fl_arci.cxx,v 1.8 2001/07/23 09:50:05 spitzak Exp $"
//
// Arc (integer) drawing functions for the Fast Light Tool Kit (FLTK).
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

// Draw pie slices, chord shapes, and curved lines, using the facilities
// of X or Windows instead of the line segment drawing that fl_arc(...)
// uses.  These draw the limited
// circle types provided by X and NT graphics.  The advantage of
// these is that small ones draw quite nicely (probably due to stored
// hand-drawn bitmaps of small circles!) and may be implemented by
// hardware and thus are fast.
// This is merged into a single function to make writing a dispatch
// table easier.

#include <fltk/fl_draw.h>
#include <fltk/x.h>
#ifdef WIN32
#include <fltk/math.h>
#endif

void fl_pie(int x,int y,int w,int h,double a1,double a2, int what) {
  if (w <= 0 || h <= 0) return;
  x += fl_x_; y += fl_y_;
#ifdef WIN32
  if (a1 == a2) return;
  w++; h++; // is this needed to match X?
  int xa = x+w/2+int(w*cos(a1/180.0*M_PI));
  int ya = y+h/2-int(h*sin(a1/180.0*M_PI));
  int xb = x+w/2+int(w*cos(a2/180.0*M_PI));
  int yb = y+h/2-int(h*sin(a2/180.0*M_PI));
  switch (what) {
  case FL_PIE:
    Pie(fl_gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  case FL_CHORD:
    Chord(fl_gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  case FL_ARC:
    Arc(fl_gc, x, y, x+w, y+h, xa, ya, xb, yb); 
    break;
  }
#else
  int A = int(a1*64);
  int B = int(a2*64)-A;
  switch(what) {
  case FL_PIE:
    XSetArcMode(fl_display, fl_gc, ArcPieSlice);
    goto J1;
  case FL_CHORD:
    XSetArcMode(fl_display, fl_gc, ArcChord);
  J1:
    XFillArc(fl_display, fl_window, fl_gc, x, y, w, h, A, B);
    break;
  case FL_ARC:
    XDrawArc(fl_display, fl_window, fl_gc, x, y, w, h, A, B);
    break;
  }
#endif
}

//
// End of "$Id: fl_arci.cxx,v 1.8 2001/07/23 09:50:05 spitzak Exp $".
//
