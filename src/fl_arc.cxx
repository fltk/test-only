//
// "$Id: fl_arc.cxx,v 1.5 2001/02/20 06:59:50 spitzak Exp $"
//
// Arc functions for the Fast Light Tool Kit (FLTK).
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

// Utility for drawing arcs and circles.  They are added to
// the current fl_begin/fl_vertex/fl_end path.
// Incremental math implementation:

#include <FL/fl_draw.H>
#include <FL/math.h>

void fl_arc(double l, double b, double w, double h, double start, double end) {

  const double x = l+w/2;
  const double y = b+h/2;
  const double A = start*(M_PI/180);
  const double E = end*(M_PI/180);

  // draw start point accurately:
  double X = w/2*cos(A);
  double Y = -h/2*sin(A);
  fl_vertex(x+X,y+Y);

  // number of segments per radian:
  double x1 = fl_transform_dx(w,0);
  double y1 = fl_transform_dy(w,0);
  double x2 = fl_transform_dx(0,h);
  double y2 = fl_transform_dy(0,h);
  int n = int(sqrt(fabs(x1*y2-x2*y1))*(.5 * .841471));

  int i = int((E-A)*n);
  if (i) {
    if (i < 0) {i = -i; n = -n;}
    const double epsilon = w/h/n;
    const double epsilon2 = h/w/n/2;
    while (--i) {
      X += epsilon*Y;
      Y -= epsilon2*X;
      fl_vertex(x+X,y+Y);
      Y -= epsilon2*X;
    }
  }

  // draw the end point accurately:
  fl_vertex(x+w/2*cos(E), y-h/2*sin(E));
}

//
// End of "$Id: fl_arc.cxx,v 1.5 2001/02/20 06:59:50 spitzak Exp $".
//
