//
// "$Id: fl_arc.cxx,v 1.7 2002/01/29 08:05:56 spitzak Exp $"
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

/*

Algorithim is a modification of the Minsky scheme for incremental
circle drawing. I made the modifications by accident trying to improve
the output, but here is a mathematical description of what is happening,
courtesy of Jim Wilson Gainesville, FL (my notes in parensthesis)

I looked a Graphics Gems 1-5.  Volume 1 was colored in, but volume
2 referred to the original scheme -- no half-Y-step.  I am willing to
credit you for this innovative change.

As you may have already analyzed, the exact scheme has a state
transition matrix (to get from [x, y]T to [x", y"]T) of:

        [ cos(e)  sin(e) ]
        [-sin(e)  cos(e) ]

(ie you multiply an x,y by the above matrix to rotate it by an angle e
about the origin).

The original (Minsky) scheme uses the approximant:

        [ 1    e   ]
        [-e  1-e^2 ]

(this is the result of expanding the x += e*y, y -= e*x of the original
algorithim)

Both these transition matrices have the required determinant, 1.0.

Obviously, the determinant is also 1.0 for your scheme (if I've done my
sums correctly):

        [  1-e^2/2     e    ]
        [ -e+e^3/4  1-e^2/2 ]

(Above, "^", exponentiation, has a higher precedence than "/", division,
have a higher precedence than addition/subtraction/negation.)

(this is what happens when you expand y -= e/2*x, x += e*y, y -= e/2*x)

Minsky's scheme uses the first term of the Maclaurin series for cos(e)
in the diagonal elements; you use the first *two* terms.  That's as far
as my math-addled brain can carry me.  It is a neat innovation, and it
sure draws damn fine circles.

(Maclurin series for cos(x) is 1 - x^2/2! + x^4/4! - x^6/6! + x^8/8! - ...)
*/

#include <fltk/fl_draw.h>
#include <fltk/math.h>

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
// End of "$Id: fl_arc.cxx,v 1.7 2002/01/29 08:05:56 spitzak Exp $".
//
