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

  int horizontal() const {
    return ((fltk3::Valuator*)_p)->horizontal();
  }
  
  double previous_value() const {
    return ((fltk3::Valuator*)_p)->previous_value();
  }
  
  void handle_push() {
    ((fltk3::Valuator*)_p)->handle_push();
  }
  
  double softclamp(double v) {
    return ((fltk3::Valuator*)_p)->softclamp(v);
  }
  
  void handle_drag(double newvalue) {
    ((fltk3::Valuator*)_p)->handle_drag(newvalue);
  }
  
  void handle_release() {
    ((fltk3::Valuator*)_p)->handle_release();
  }
  
  // FIXME: 123 - this method was virtual 
  void value_damage() {
    ((fltk3::Valuator*)_p)->value_damage();
  }
  
  void set_value(double v) {
    ((fltk3::Valuator*)_p)->set_value(v);
  }

public:

  void bounds(double a, double b) {
    ((fltk3::Valuator*)_p)->bounds(a, b);
  }
  
  double minimum() const {
    return ((fltk3::Valuator*)_p)->minimum();
  }
  
  void minimum(double a) {
    ((fltk3::Valuator*)_p)->minimum(a);
  }
  
  double maximum() const {
    return ((fltk3::Valuator*)_p)->maximum();
  }
  
  void maximum(double a) {
    ((fltk3::Valuator*)_p)->maximum(a);
  }
  
  void range(double a, double b) {
    ((fltk3::Valuator*)_p)->range(a, b);
  }
  
  void step(int a) {
    ((fltk3::Valuator*)_p)->step(a);
  }
  
  void step(double a, int b) {
    ((fltk3::Valuator*)_p)->step(a, b);
  }
  
  void step(double s) {
    ((fltk3::Valuator*)_p)->step(s);
  }
  
  double step() const {
    return ((fltk3::Valuator*)_p)->step();
  }
  
  void precision(int x) {
    ((fltk3::Valuator*)_p)->precision(x);
  }

  double value() const {
    return ((fltk3::Valuator*)_p)->value();
  }
  
  int value(double v) {
    return ((fltk3::Valuator*)_p)->value(v);
  }

  // FIXME: 123 - this method was virtual 
  int format(char *f) {
    return ((fltk3::Valuator*)_p)->format(f);
  }
  
  double round(double v) {
    return ((fltk3::Valuator*)_p)->round(v);
  }
  
  double clamp(double v) {
    return ((fltk3::Valuator*)_p)->clamp(v);
  }
  
  double increment(double v, int x) {
    return ((fltk3::Valuator*)_p)->increment(v, x);
  }

};

#endif

//
// End of "$Id$".
//
