//
// "$Id$"
//
// Portable drawing routines for the Fast Light Tool Kit (FLTK).
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

// Portable drawing code for drawing arbitrary shapes with
// simple 2D transformations.  See also fl_arc.cxx

#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/math.h>
#include <stdlib.h>
#include "Fl_Win_Display.H"

static XPoint *p = (XPoint *)0;
// typedef what the x,y fields in a point are:

typedef int COORD_T;


static int p_size;
static int n;
int fl_what_loop;
enum {LINE, LOOP, POLYGON, POINT_};

void Fl_Win_Display::begin_points() {n = 0; fl_what_loop = POINT_;}

void Fl_Win_Display::begin_line() {n = 0; fl_what_loop = LINE;}

void Fl_Win_Display::begin_loop() {n = 0; fl_what_loop = LOOP;}

void Fl_Win_Display::begin_polygon() {n = 0; fl_what_loop = POLYGON;}



static void fl_transformed_vertex(COORD_T x, COORD_T y) {
  if (!n || x != p[n-1].x || y != p[n-1].y) {
    if (n >= p_size) {
      p_size = p ? 2*p_size : 16;
      p = (XPoint *)realloc((void*)p, p_size*sizeof(*p));
    }
    p[n].x = x;
    p[n].y = y;
    n++;
  }
}

void Fl_Win_Display::transformed_vertex(double xf, double yf) {
  fl_transformed_vertex(COORD_T(rint(xf)), COORD_T(rint(yf)));
}

void Fl_Win_Display::vertex(double x,double y) {
  Fl_Win_Display::transformed_vertex(x*m.a + y*m.c + m.x, x*m.b + y*m.d + m.y);
}

void Fl_Win_Display::end_points() {
  for (int i=0; i<n; i++) SetPixel(fl_gc, p[i].x, p[i].y, fl_RGB());
}

void Fl_Win_Display::end_line() {
  if (n < 2) {
    Fl_Win_Display::end_points();
    return;
  }
  if (n>1) {
    fl_pen();
    Polyline(fl_gc, p, n);
  };
}

static void fixloop() {  // remove equal points from closed path
  while (n>2 && p[n-1].x == p[0].x && p[n-1].y == p[0].y) n--;
}

void Fl_Win_Display::end_loop() {
  fixloop();
  if (n>2) fl_transformed_vertex((COORD_T)p[0].x, (COORD_T)p[0].y);
  Fl_Win_Display::end_line();
}

void Fl_Win_Display::end_polygon() {
  fixloop();
  if (n < 3) {
    Fl_Win_Display::end_line();
    return;
  }

  if (n>2) {
    fl_brush();
    //SelectObject(fl_gc, fl_brush());
    Polygon(fl_gc, p, n);
  }
}

static int gap_;

static int counts[20];
static int numcount;


void Fl_Win_Display::begin_complex_polygon() {
  Fl_Win_Display::begin_polygon();
  gap_ = 0;
  numcount = 0;
}

void Fl_Win_Display::gap() {
  while (n>gap_+2 && p[n-1].x == p[gap_].x && p[n-1].y == p[gap_].y) n--;
  if (n > gap_+2) {
    fl_transformed_vertex((COORD_T)p[gap_].x, (COORD_T)p[gap_].y);
    counts[numcount++] = n-gap_;
    gap_ = n;
  } else {
    n = gap_;
  }
}

void Fl_Win_Display::end_complex_polygon() {
  fl_gap();
  if (n < 3) {
    Fl_Win_Display::end_line();
    return;
  }

  if (n>2) {
    //SelectObject(fl_gc, fl_brush());
    fl_brush();
    PolyPolygon(fl_gc, p, counts, numcount);
  }

}

// shortcut the closed circles so they use XDrawArc:
// warning: these do not draw rotated ellipses correctly!
// See fl_arc.c for portable version.

void Fl_Win_Display::circle(double x, double y,double r) {
  double xt = fl_transform_x(x,y);
  double yt = fl_transform_y(x,y);
  double rx = r * (m.c ? sqrt(m.a*m.a+m.c*m.c) : fabs(m.a));
  double ry = r * (m.b ? sqrt(m.b*m.b+m.d*m.d) : fabs(m.d));
  int llx = (int)rint(xt-rx);
  int w = (int)rint(xt+rx)-llx;
  int lly = (int)rint(yt-ry);
  int h = (int)rint(yt+ry)-lly;

  if (fl_what_loop==POLYGON) {
    fl_brush();
    //SelectObject(fl_gc, fl_brush());
    Pie(fl_gc, llx, lly, llx+w, lly+h, 0,0, 0,0); 
  } else{
    fl_pen();
    Arc(fl_gc, llx, lly, llx+w, lly+h, 0,0, 0,0); 
  };
}

//
// End of "$Id$".
//
