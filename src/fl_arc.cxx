/
// "$Id$
/
// Arc functions for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

// Utility for drawing arcs and circles.  They are added t
// the current fl_begin/fl_vertex/fl_end path
// Incremental math implementation

#include <FL/fl_draw.H
#include <FL/math.h
#include <FL/Fl_Display.H

void Fl_Display::arc(double x, double y, double r, double start, double end) 

  // draw start point accurately
 
  double A = start*(M_PI/180);		// Initial angle (radians
  double X =  r*cos(A);			// Initial displacement, (X,Y
  double Y = -r*sin(A);			//   from center to initial poin
  fl_vertex(x+X,y+Y);			// Insert initial poin

  // Maximum arc length to approximate with chord with error <= 0.12
 
  double epsilon; 
    double r1 = hypot(fl_transform_dx(r,0), // Horizontal "radius
		      fl_transform_dy(r,0))
    double r2 = hypot(fl_transform_dx(0,r), // Vertical "radius
		      fl_transform_dy(0,r))
		     
    if (r1 > r2) r1 = r2;		// r1 = minimum "radius
    if (r1 < 2.) r1 = 2.;		// radius for circa 9 chords/circl
   
    epsilon = 2*acos(1.0 - 0.125/r1);	// Maximum arc angl
  
  A = end*(M_PI/180) - A;		// Displacement angle (radians
  int i = int(ceil(fabs(A)/epsilon));	// Segments in approximatio
 
  if (i) 
    epsilon = A/i;			// Arc length for equal-size step
    double cos_e = cos(epsilon);	// Rotation coefficient
    double sin_e = sin(epsilon)
    do 
      double Xnew =  cos_e*X + sin_e*Y
		Y = -sin_e*X + cos_e*Y
      fl_vertex(x + (X=Xnew), y + Y)
    } while (--i)
  


#if 0 // portable version.  X-specific one in xlib/vertex.cx
void fl_circle(double x,double y,double r) 
  _fl_arc(x, y, r, r, 0, 360)

#endi

/
// End of "$Id$"
/
