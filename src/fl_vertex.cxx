//
// "$Id: fl_vertex.cxx,v 1.8 2000/09/27 16:25:52 spitzak Exp $"
//
// Portable drawing routines for the Fast Light Tool Kit (FLTK).
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

// Portable drawing code for drawing arbitrary shapes with
// simple 2D transformations.  See also fl_arc.C

#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/math.h>
#include <stdlib.h>

struct matrix {double a, b, c, d, x, y;};

static matrix m = {1, 0, 0, 1, 0, 0};

static matrix stack[10];
static int sptr = 0;

void fl_push_matrix() {stack[sptr++] = m;}

void fl_pop_matrix() {m = stack[--sptr];}

void fl_mult_matrix(double a, double b, double c, double d, double x, double y) {
  matrix o;
  o.a = a*m.a + b*m.c;
  o.b = a*m.b + b*m.d;
  o.c = c*m.a + d*m.c;
  o.d = c*m.b + d*m.d;
  o.x = x*m.a + y*m.c + m.x;
  o.y = x*m.b + y*m.d + m.y;
  m = o;
}

void fl_scale(double x,double y) {fl_mult_matrix(x,0,0,y,0,0);}

void fl_scale(double x) {fl_mult_matrix(x,0,0,x,0,0);}

void fl_translate(double x,double y) {fl_mult_matrix(1,0,0,1,x,y);}

void fl_rotate(double d) {
  if (d) {
    double s, c;
    if (d == 0) {s = 0; c = 1;}
    else if (d == 90) {s = 1; c = 0;}
    else if (d == 180) {s = 0; c = -1;}
    else if (d == 270 || d == -90) {s = -1; c = 0;}
    else {s = sin(d*M_PI/180); c = cos(d*M_PI/180);}
    fl_mult_matrix(c,-s,s,c,0,0);
  }
}

double fl_transform_x(double x, double y) {return x*m.a + y*m.c + m.x;}

double fl_transform_y(double x, double y) {return x*m.b + y*m.d + m.y;}

double fl_transform_dx(double x, double y) {return x*m.a + y*m.c;}

double fl_transform_dy(double x, double y) {return x*m.b + y*m.d;}

// typedef what the x,y fields in a point are:
#ifdef WIN32
typedef int COORD_T;
#else
typedef short COORD_T;
#endif

// Storage of the current path:
static XPoint *point; // all the points
static int point_array_size;
static int points; // number of points
static int loop_start; // point at start of current loop
static int* loop; // number of points in each loop
static int loops; // number of loops
static int loop_array_size;
// We also keep track of one circle:
static int circle_x, circle_y, circle_w, circle_h;

void fl_begin() {points = loop_start = loops = circle_w = 0;}

static void transformed_vertex(COORD_T x, COORD_T y) {
  if (!points || x != point[points-1].x || y != point[points-1].y) {
    if (points >= point_array_size) {
      point_array_size = point_array_size ? 2*point_array_size : 16;
      point = (XPoint*)realloc((void*)point, point_array_size*sizeof(XPoint*));
    }
    point[points].x = x;
    point[points].y = y;
    points++;
  }
}

void fl_transformed_vertex(double xf, double yf) {
  transformed_vertex(COORD_T(xf+.5), COORD_T(yf+.5));
}

void fl_vertex(double x,double y) {
  transformed_vertex(COORD_T(x*m.a + y*m.c + m.x + .5),
		     COORD_T(x*m.b + y*m.d + m.y + .5));
}

void fl_gap() {
  if (points > loop_start+2) {
    // close the shape by duplicating first point:
    transformed_vertex(point[loop_start].x, point[loop_start].y);
    if (loops >= loop_array_size) {
      loop_array_size = loop_array_size ? 2*loop_array_size : 16;
      loop = (int*)realloc((void*)loop, loop_array_size*sizeof(int));
    }
    loop[loops++] = points-loop_start;
    loop_start = points;
  } else {
    points = loop_start;
  }
}

// Shortcut closed circles so they use XDrawArc.
// Only 1 circle per path, ellipses do not work in rotated
// coordinate systems, not connected with any other parts of path.
// If this is a problem use fl_arc() for a correct but slower version.

void fl_circle(double x, double y,double r) {
  fl_gap(); // maybe it should do this?
  double xt = fl_transform_x(x,y);
  double yt = fl_transform_y(x,y);
  double rx = r * (m.c ? sqrt(m.a*m.a+m.c*m.c) : fabs(m.a));
  double ry = r * (m.b ? sqrt(m.b*m.b+m.d*m.d) : fabs(m.d));
  circle_w = int(xt+rx+.5)-(circle_x = int(xt-rx+.5));
  circle_h = int(yt+ry+.5)-(circle_y = int(yt-ry+.5));
}

void fl_end_points() {
#ifdef WIN32
  for (int i=0; i<points; i++)
    SetPixel(fl_gc, point[i].x, point[i].y, fl_colorref);
#else
  if (points > 0) XDrawPoints(fl_display, fl_window, fl_gc, point, points, 0);
#endif
}

void fl_end_line() {
  if (circle_w > 0) {
#ifdef WIN32
    Arc(fl_gc, circle_x, circle_y, circle_x+circle_w, circle_y+circle_h, 0,0, 0,0); 
#else
    XDrawArc(fl_display, fl_window, fl_gc, circle_x, circle_y, circle_w, circle_h, 0, 360*64);
#endif
  }
  int loop_start = 0;
  for (int n = 0; n < loops; n++) {
    int loop_size = loop[n];
#ifdef WIN32
    Polyline(fl_gc, point+loop_start, loop_size);
#else
    XDrawLines(fl_display, fl_window, fl_gc, point+loop_start, loop_size, 0);
#endif
    loop_start += loop_size;
  }
  int loop_size = points-loop_start;
#ifdef WIN32
  if (loop_size > 1) Polyline(fl_gc, point+loop_start, loop_size);
#else
  if (loop_size > 1) XDrawLines(fl_display, fl_window, fl_gc, point+loop_start, loop_size, 0);
#endif
}

void fl_end_loop() {
  // close the shape by duplicating first point:
  if (points > loop_start+2)
    transformed_vertex(point[loop_start].x, point[loop_start].y);
  fl_end_line();
}

void fl_end_polygon() {
  if (circle_w > 0) {
#ifdef WIN32
    SelectObject(fl_gc, fl_brush);
    Pie(fl_gc, circle_x, circle_y, circle_x+circle_w, circle_y+circle_h, 0, 0, 0,0); 
#else
    XFillArc(fl_display, fl_window, fl_gc, circle_x, circle_y, circle_w, circle_h, 0, 360*64);
#endif
  }
  if (loops) {
    fl_gap();
#ifdef WIN32
    SelectObject(fl_gc, fl_brush);
    PolyPolygon(fl_gc, point, loop, loops);
#else
    XFillPolygon(fl_display, fl_window, fl_gc, point, points, 0, 0);
#endif
  } else if (points > 2) {
#ifdef WIN32
    SelectObject(fl_gc, fl_brush);
    Polygon(fl_gc, point, points);
#else
    XFillPolygon(fl_display, fl_window, fl_gc, point, points, Convex, 0);
#endif
  }
}

void fl_end_complex_polygon() {
#ifndef WIN32
  // For X this stops it from sending the Convex signal:
  if (!loops) {
    if (points > 2)
      XFillPolygon(fl_display, fl_window, fl_gc, point, points, 0, 0);
    return;
  }
#endif
  fl_end_polygon();
}

// back-compatability functions:
void fl_begin_points() {fl_begin();}
void fl_begin_line() {fl_begin();}
void fl_begin_loop() {fl_begin();}
void fl_begin_polygon() {fl_begin();}
void fl_begin_complex_polygon() {fl_begin();}

//
// End of "$Id: fl_vertex.cxx,v 1.8 2000/09/27 16:25:52 spitzak Exp $".
//
