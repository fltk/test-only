//
// "$Id: fl_vertex.cxx,v 1.10 2001/02/20 06:59:50 spitzak Exp $"
//
// Path construction and filling. I think this file is always linked
// into any fltk program, so try to keep it reasonably small.
// Also see fl_arc.cxx and fl_curve.cxx
//
// Copyright 2001 by Bill Spitzak and others.
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
#include <FL/math.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////
// Transformation:

struct Matrix {double a, b, c, d, x, y;};

static Matrix m = {1, 0, 0, 1, 0, 0};

struct Matrixi {Matrix m; int x, y;};

static Matrixi stack[10];
static int sptr = 0;

void fl_push_matrix() {
  stack[sptr].m = m;
  stack[sptr].x = fl_x_;
  stack[sptr].y = fl_y_;
  sptr++;
}

void fl_pop_matrix() {
  sptr--;
  m = stack[sptr].m;
  fl_x_ = stack[sptr].x;
  fl_y_ = stack[sptr].y;
}

void fl_mult_matrix(double a, double b, double c, double d, double x, double y) {
  Matrix o;
  o.a = a*m.a + b*m.c;
  o.b = a*m.b + b*m.d;
  o.c = c*m.a + d*m.c;
  o.d = c*m.b + d*m.d;
  o.x = x*m.a + y*m.c + m.x;
  {double t = rint(o.x); o.x = o.x-t; fl_x_ += int(t);}
  o.y = x*m.b + y*m.d + m.y;
  {double t = rint(o.y); o.y = o.y-t; fl_y_ += int(t);}
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

////////////////////////////////////////////////////////////////
// Path construction:

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

static inline void inline_newpath() {
  points = loop_start = loops = circle_w = 0;
}
void fl_newpath() {inline_newpath();}

void fl_vertex(double x,double y) {
  fl_vertex(int(x*m.a + y*m.c + m.x + .5),
	    int(x*m.b + y*m.d + m.y + .5));
}

void fl_vertex(int X, int Y) {
  COORD_T x = COORD_T(X+fl_x_);
  COORD_T y = COORD_T(Y+fl_y_);
  if (!points || x != point[points-1].x || y != point[points-1].y) {
    if (points >= point_array_size) {
      point_array_size = point_array_size ? 2*point_array_size : 16;
      point = (XPoint*)realloc((void*)point,
			       (point_array_size+1)*sizeof(XPoint*));
    }
    point[points].x = x;
    point[points].y = y;
    points++;
  }
}

void fl_transformed_vertex(double xf, double yf) {
  fl_vertex(int(xf+.5), int(yf+.5));
}

void fl_closepath() {
  if (points > loop_start+2) {
    // close the shape by duplicating first point:
    // the array always has one extra point so we don't need to check
    XPoint& p = point[points-1];
    XPoint& q = point[loop_start];
    if (p.x != q.x || p.y != q.y) point[points++] = q;
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

// Add a circle to the path. It is always a circle, irregardless of
// the transform. Currently only one per path is supported, this uses
// commands on the server to draw a nicer circle than the path mechanism
// can make.
void fl_circle(double x, double y, double r) {
  double xt = fl_transform_x(x,y);
  double yt = fl_transform_y(x,y);
  double rt = r * sqrt(fabs(m.a*m.d-m.b*m.c));
  circle_x = int(xt-rt+.5)+fl_x_;
  circle_w = int(xt+rt+.5)+fl_x_-circle_x;
  circle_y = int(yt-rt+.5)+fl_y_;
  circle_h = int(yt+rt+.5)+fl_y_-circle_y;
}

// Add an ellipse to the path. Very lame, only works right for orthogonal
// transformations.
void fl_ellipse(double x, double y, double w, double h) {
  x += w/2;
  y += h/2;
  double cx = fl_transform_x(x,y);
  double cy = fl_transform_y(x,y);
  double d1 = fl_transform_dx(w,0);
  double d2 = fl_transform_dx(0,h);
  double rx = sqrt(d1*d1+d2*d2)/2;
  d1 = fl_transform_dy(w,0);
  d2 = fl_transform_dy(0,h);
  double ry = sqrt(d1*d1+d2*d2)/2;
  circle_x = int(cx-rx+.5)+fl_x_;
  circle_w = int(cx+rx+.5)+fl_x_-circle_x;
  circle_y = int(cy-ry+.5)+fl_y_;
  circle_h = int(cy+ry+.5)+fl_y_-circle_y;
// this would work if fl_arc drew nicely:
// fl_closepath();
// fl_arc(x, y, w, h, 0, 360);
// fl_closepath();
}

////////////////////////////////////////////////////////////////
// Draw the path:

void fl_points() {
#ifdef WIN32
  for (int i=0; i<points; i++)
    SetPixel(fl_gc, point[i].x, point[i].y, fl_colorref);
#else
  if (points > 0) XDrawPoints(fl_display, fl_window, fl_gc, point, points, 0);
#endif
  inline_newpath();
}

void fl_stroke() {
#ifdef WIN32
  if (circle_w > 0)
    Arc(fl_gc, circle_x, circle_y, circle_x+circle_w, circle_y+circle_h,
	0,0, 0,0);
  int loop_start = 0;
  for (int n = 0; n < loops; n++) {
    int loop_size = loop[n];
    Polyline(fl_gc, point+loop_start, loop_size);
    loop_start += loop_size;
  }
  int loop_size = points-loop_start;
  if (loop_size > 1)
    Polyline(fl_gc, point+loop_start, loop_size);
#else
  if (circle_w > 0)
    XDrawArc(fl_display, fl_window, fl_gc,
	     circle_x, circle_y, circle_w, circle_h, 0, 360*64);
  int loop_start = 0;
  for (int n = 0; n < loops; n++) {
    int loop_size = loop[n];
    XDrawLines(fl_display, fl_window, fl_gc, point+loop_start, loop_size, 0);
    loop_start += loop_size;
  }
  int loop_size = points-loop_start;
  if (loop_size > 1)
    XDrawLines(fl_display, fl_window, fl_gc, point+loop_start, loop_size, 0);
#endif
  inline_newpath();
}

// Warning: result is different on X and Win32! Use fl_fill_stroke().
// There may be a way to tell Win32 to do what X does, perhaps by making
// the current pen invisible?
void fl_fill() {
#ifdef WIN32
  if (circle_w > 0)
    Chord(fl_gc, circle_x, circle_y, circle_x+circle_w, circle_y+circle_h,
	  0,0, 0,0);
  if (loops) {
    fl_closepath();
    PolyPolygon(fl_gc, point, loop, loops);
  } else if (points > 2) {
    Polygon(fl_gc, point, points);
  }
#else
  if (circle_w > 0)
    XFillArc(fl_display, fl_window, fl_gc,
	     circle_x, circle_y, circle_w, circle_h, 0, 64*360);
  if (loops) fl_closepath();
  if (points > 2)
    XFillPolygon(fl_display, fl_window, fl_gc, point, points, 0, 0);
#endif
  inline_newpath();
}

// This seems to produce very similar results on X and Win32. Also
// should be faster than seperate fill & stroke on Win32 and on
// PostScript/PDF style systems.
void fl_fill_stroke(Fl_Color color) {
#ifdef WIN32
  HPEN newpen = fl_create_pen();
  HPEN oldpen = (HPEN)SelectObject(fl_gc, newpen);
  if (circle_w > 0)
    Chord(fl_gc, circle_x, circle_y, circle_x+circle_w, circle_y+circle_h,
	  0,0, 0,0);
  if (loops) {
    fl_closepath();
    PolyPolygon(fl_gc, point, loop, loops);
  } else if (points > 2) {
    Polygon(fl_gc, point, points);
  }
  SelectObject(fl_gc, oldpen);
  DeleteObject(newpen);
  inline_newpath();
#else
  if (circle_w > 0)
    XFillArc(fl_display, fl_window, fl_gc,
	     circle_x, circle_y, circle_w, circle_h, 0, 64*360);
  if (loops) fl_closepath();
  if (points > 2)
    XFillPolygon(fl_display, fl_window, fl_gc, point, points, 0, 0);
  Fl_Color saved = fl_color();
  fl_color(color); fl_stroke();
  fl_color(saved);
#endif
}

//
// End of "$Id: fl_vertex.cxx,v 1.10 2001/02/20 06:59:50 spitzak Exp $".
//
