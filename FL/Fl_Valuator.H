//
// "$Id$"
//
// Valuator header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/* \file
   Fl_Valuator widget . */

#ifndef Fl_Valuator_H
#define Fl_Valuator_H

#include <fltk3/Valuator.h>
#include "Fl_Widget.H"

#define FL_VERTICAL		0
#define FL_HORIZONTAL		1


class FL_EXPORT Fl_Valuator : public Fl_Widget {

protected:

  Fl_Valuator() { /* empty */ }
  
  Fl_Valuator(int X, int Y, int W, int H, const char* L) {
    _p = new fltk3::Valuator(X, Y, W, H, L);
    _p->wrapper(this);
  }

#if 0 // FIXME: 123
  
  int horizontal() const {return type()& FL_HORIZONTAL;}
  Fl_Valuator(int X, int Y, int W, int H, const char* L);
  double previous_value() const {return previous_value_;}
  void handle_push() {previous_value_ = value_;}
  double softclamp(double);
  void handle_drag(double newvalue);
  void handle_release();
  virtual void value_damage();
  void set_value(double v) {value_ = v;}

#endif
  
public:

#if 0 // FIXME: 123

  void bounds(double a, double b) {min=a; max=b;}
  double minimum() const {return min;}
  void minimum(double a) {min = a;}
  double maximum() const {return max;}
  void maximum(double a) {max = a;}
  void range(double a, double b) {min = a; max = b;}
  void step(int a) {A = a; B = 1;}
  void step(double a, int b) {A = a; B = b;}
  void step(double s);
  double step() const {return A/B;}
  void precision(int);

#endif

  double value() const {
    return ((fltk3::Valuator*)_p)->value();
  }
  
  int value(double v) {
    return ((fltk3::Valuator*)_p)->value(v);
  }

#if 0 // FIXME: 123
  
  virtual int format(char*);
  double round(double);
  double clamp(double);
  double increment(double, int);

#endif

};

#endif

//
// End of "$Id$".
//
