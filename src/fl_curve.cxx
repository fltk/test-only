//
// "$Id: fl_curve.cxx,v 1.7 2002/07/01 15:28:19 spitzak Exp $"
//
// Bezier curve functions for the Fast Light Tool Kit (FLTK).
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

// Utility for drawing Bezier curves, adding the points to
// the current fl_begin/fl_vertex/fl_end path.
// Incremental math implementation:
// I very much doubt this is optimal!  From Foley/vanDam page 511.
// If anybody has a better algorithim, please send it!

#include <fltk/fl_draw.h>
#include <math.h>

void fl_curve(double x0, double y0,
	      double x1, double y1,
	      double x2, double y2,
	      double x3, double y3) {

  fl_transform(x0,y0);
  fl_transform(x1,y1);
  fl_transform(x2,y2);
  fl_transform(x3,y3);
  double x = x0; double y = y0;

#define MAXPOINTS 100
  float points[MAXPOINTS][2];
  float* p = points[0];
  *p++ = float(x); *p++ = float(y);

  // find the area:
  double a = fabs((x-x2)*(y3-y1)-(y-y2)*(x3-x1));
  double b = fabs((x-x3)*(y2-y1)-(y-y3)*(x2-x1));
  if (b > a) a = b;

  // use that to guess at the number of segments:
  int n = int(sqrt(a)/4);
  if (n > 1) {
    if (n > MAXPOINTS-1) n = MAXPOINTS-1;

    double e = 1.0/n;

    // calculate the coefficients of 3rd order equation:
    double xa = (x3-3*x2+3*x1-x);
    double xb = 3*(x2-2*x1+x);
    double xc = 3*(x1-x);
    // calculate the forward differences:
    double dx1 = ((xa*e+xb)*e+xc)*e;
    double dx3 = 6*xa*e*e*e;
    double dx2 = dx3 + 2*xb*e*e;

    // calculate the coefficients of 3rd order equation:
    double ya = (y3-3*y2+3*y1-y);
    double yb = 3*(y2-2*y1+y);
    double yc = 3*(y1-y);
    // calculate the forward differences:
    double dy1 = ((ya*e+yb)*e+yc)*e;
    double dy3 = 6*ya*e*e*e;
    double dy2 = dy3 + 2*yb*e*e;

    // draw points 1 .. n-2:
    for (int m=2; m<n; m++) {
      x += dx1;
      *p++ = float(x);
      dx1 += dx2;
      dx2 += dx3;
      y += dy1;
      *p++ = float(y);
      dy1 += dy2;
      dy2 += dy3;
    }

    // draw point n-1:
    *p++ = float(x+dx1);
    *p++ = float(y+dy1);
  }

  // draw point n:
  *p++ = float(x3);
  *p++ = float(y3);
  fl_transformed_vertices((p-points[0])/2, points);
}

//
// End of "$Id: fl_curve.cxx,v 1.7 2002/07/01 15:28:19 spitzak Exp $".
//
