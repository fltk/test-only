//
// "$Id: Fl_Positioner.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Positioner header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Positioner_H
#define Fl_Positioner_H

#ifndef Fl_Widget_H
#include <fltk/Fl_Widget.h>
#endif

/**

  This class is provided for Forms compatibility. It provides 2D input. 
  It would be useful if this could be put atop another widget so that 
  the crosshairs are on top, but this is not implemented. The color of
  the crosshairs is selection_color(). 

   \image positioner.gif

*/
class FL_FORMS_API Fl_Positioner : public Fl_Widget {

  double xmin, ymin;
  double xmax, ymax;
  double xvalue_, yvalue_;
  double xstep_, ystep_;

protected:

  // these allow subclasses to put the dial in a smaller area:
  void draw(int, int, int, int);
  int handle(int, int, int, int, int);
  void draw();

public:

  int handle(int);
  /**
   Creates a new Fl_Positioner widget using the given position,
   size, and label string. The default boxtype is FL_NO_BOX. 
  */
  Fl_Positioner(int x,int y,int w,int h, const char *l=0);
  /*@{*/
  /** Gets or sets the X axis coordinate. */
  double xvalue() const {return xvalue_;}
  int xvalue(double);
  /*@}*/
  /*@{*/
  /** Gets or sets the Y axis coordinate. */
  double yvalue() const {return yvalue_;}
  int yvalue(double);
  /*@}*/
  int value(double,double);
  /** Sets the X axis bounds. */
  void xbounds(double, double);
  double xminimum() const {return xmin;}
  void xminimum(double a) {xbounds(a,xmax);}
  double xmaximum() const {return xmax;}
  void xmaximum(double a) {xbounds(xmin,a);}
  /** Sets the Y axis bounds. */
  void ybounds(double, double);
  double yminimum() const {return ymin;}
  void yminimum(double a) {ybounds(a,ymax);}
  double ymaximum() const {return ymax;}
  void ymaximum(double a) {ybounds(ymin,a);}
  /** Sets the stepping value for the X axis. */
  void xstep(double a) {xstep_ = a;}
  /** Sets the stepping value for the Y axis. */
  void ystep(double a) {ystep_ = a;}

};

#endif

//
// End of "$Id: Fl_Positioner.h,v 1.2 2002/10/29 00:37:23 easysw Exp $".
//
