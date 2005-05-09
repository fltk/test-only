//
// "$Id$"
//
// Portable drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

// Portable drawing code for drawing arbitrary shapes with
// simple 2D transformations.  See also fl_arc.cxx

#include <FL/fl_draw.H>
//#include <FL/x.H>
#include <FL/math.h>
//#include <stdlib.h>

struct matrix {double a, b, c, d, x, y;};

static matrix m = {1, 0, 0, 1, 0, 0};

static matrix stack[10];
static int sptr = 0;

void fl_push_matrix() {stack[sptr++] = m;}

void fl_pop_matrix() {m = stack[--sptr];}

FL_EXPORT void fl_mult_matrix(double a, double b, double c, double d, double x, double y) {
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


matrix * fl_matrix = &m;

#ifdef WIN32
#  include "win/vertex.cxx"
#elif defined(__APPLE__)
#  include "carbon/vertex.cxx"
#else
#  include "xlib/vertex.cxx"
#endif


//
// End of "$Id$".
//
