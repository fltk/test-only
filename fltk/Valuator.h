//
// "$Id$"
//
// Base class for all the widgets that control a single floating-point
// value.
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fltk_Valuator_h
#define fltk_Valuator_h

#ifndef fltk_Widget_h
#include "Widget.h"
#endif

namespace fltk {

class FL_API Valuator : public Widget {

public:

  double value() const {return value_;}
  int value(double);

  float minimum() const {return minimum_;}
  void minimum(double a) {minimum_ = float(a);}

  float maximum() const {return maximum_;}
  void maximum(double a) {maximum_ = float(a);}

  void range(double a, double b) {minimum_ = float(a); maximum_ = float(b);}

  float step() const {return step_;}
  void step(double a) {step_ = float(a);}

  float linesize() const {return linesize_;}
  void linesize(double a) {linesize_ = float(a);}

  virtual int format(char*);

  int handle(int);

#ifdef FLTK_1_SLIDER
  void step(double a, int b) {step(a/b);}
  void bounds(double a, double b) {minimum_=float(a); maximum_=float(b);}
  void precision(int p) {
    int B = 1;
    for (int i=0; i<p; i++) B *= 10;
    step_ = 1.0f/B;
  }
#endif

//protected:

  Valuator(int X, int Y, int W, int H, const char* L);
  double previous_value() const {return previous_value_;}
  void handle_push() {previous_value_ = value_;}
  void handle_drag(double newvalue);
  void handle_release();

  virtual void value_damage(); // callback whenever value changes
  void set_value(double v) {value_ = v;} // change w/o doing value_damage

private:

  double value_;
  static double previous_value_;
  float minimum_;
  float maximum_;
  float step_;
  float linesize_;

};

}

#endif

//
// End of "$Id$".
//
