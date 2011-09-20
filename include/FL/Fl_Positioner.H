//
// "$Id$"
//
// Positioner header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef Fl_Positioner_H
#define Fl_Positioner_H

#include <fltk3/Positioner.h>
#include "Fl_Widget.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Positioner, Positioner)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Positioner, Positioner)
  void draw(int x, int y, int w, int h) { Positioner::draw(x, y, w, h); }
  int handle(int e, int x, int y, int w, int h) { return Positioner::handle(e, x, y, w, h); }
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Positioner : public Fl_Widget {

protected:

  void draw(int x, int y, int w, int h) {
    ((fltk3::Positioner_I*)_p)->draw(x, y, w, h);
  }
  
  int handle(int e, int x, int y, int w, int h) {
    ((fltk3::Positioner_I*)_p)->handle(e, x, y, w, h);
  }

public:

  FLTK3_WIDGET_VCALLS(Fl_Positioner, Positioner)
  
  Fl_Positioner() { }
  
  Fl_Positioner(int x,int y,int w,int h, const char *l=0) {
    _p = new fltk3::Positioner_I(x, y, w, h, l);
    _p->wrapper(this);
  }
  
  double xvalue() const {
    return ((fltk3::Positioner*)_p)->xvalue();
  }
  
  double yvalue() const {
    return ((fltk3::Positioner*)_p)->yvalue();
  }
  
  int xvalue(double v) {
    return ((fltk3::Positioner*)_p)->xvalue(v);
  }
  
  int yvalue(double a) {
    return ((fltk3::Positioner*)_p)->yvalue(a);
  }
  
  int value(double a, double b) {
    return ((fltk3::Positioner*)_p)->value(a, b);
  }
  
  void xbounds(double a, double b) {
    ((fltk3::Positioner*)_p)->xbounds(a, b);
  }
  
  double xminimum() const {
    return ((fltk3::Positioner*)_p)->xminimum();
  }
  
  void xminimum(double a) {
    ((fltk3::Positioner*)_p)->xminimum(a);
  }
  
  double xmaximum() const {
    return ((fltk3::Positioner*)_p)->xmaximum();
  }
  
  void xmaximum(double a) {
    ((fltk3::Positioner*)_p)->xmaximum(a);
  }
  
  void ybounds(double a, double b) {
    ((fltk3::Positioner*)_p)->ybounds(a, b);
  }
  
  double yminimum() const {
    return ((fltk3::Positioner*)_p)->yminimum();
  }
  
  void yminimum(double a) {
    ((fltk3::Positioner*)_p)->yminimum(a);
  }
  
  double ymaximum() const {
    return ((fltk3::Positioner*)_p)->ymaximum();
  }
  
  void ymaximum(double a) {
    ((fltk3::Positioner*)_p)->ymaximum(a);
  }
  
  void xstep(double a) {
    ((fltk3::Positioner*)_p)->xstep(a);
  }
  
  void ystep(double a) {
    ((fltk3::Positioner*)_p)->ystep(a);
  }
  
};

#endif

//
// End of "$Id$".
//
