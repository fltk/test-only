//
// "$Id: Fl_Valuator.h,v 1.6 2002/05/07 04:58:19 spitzak Exp $"
//
// Valuator header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Valuator_H
#define Fl_Valuator_H

#ifndef Fl_Widget_H
#include "Fl_Widget.h"
#endif

class FL_API Fl_Valuator : public Fl_Widget {

public:

  double value() const {return value_;}
  int value(double);

  float step() const {return step_;}
  void step(double a) {step_ = float(a);}

  double minimum() const {return minimum_;}
  void minimum(double a) {minimum_ = a;}

  double maximum() const {return maximum_;}
  void maximum(double a) {maximum_ = a;}

  void range(double a, double b) {minimum_ = a; maximum_ = b;}

  short unsigned linesize() const {return linesize_;}
  void linesize(short unsigned i) {linesize_ = i;}

  short unsigned pagesize() const {return pagesize_;}
  void pagesize(short unsigned i) {pagesize_ = i;}

  virtual int format(char*);

  int handle(int);

  double increment(double v, int steps) const;
  double clamp(double) const;
  double round(double) const;
  double softclamp(double) const;

#ifndef FLTK_2
  void step(double a, int b) {step(a/b);}
  void bounds(double a, double b) {minimum_=a; maximum_=b;}
  void precision(int);
#endif

protected:

  Fl_Valuator(int X, int Y, int W, int H, const char* L);
  double previous_value() const {return previous_value_;}
  void handle_push() {previous_value_ = value_;}
  void handle_drag(double newvalue);
  void handle_release();

  virtual void value_damage(); // callback whenever value changes
  void set_value(double v) {value_ = v;} // change w/o doing value_damage

private:

  double value_;
  static double previous_value_;
  double minimum_;
  double maximum_;
  float step_;
  short unsigned linesize_;
  short unsigned pagesize_;

};

#endif

//
// End of "$Id: Fl_Valuator.h,v 1.6 2002/05/07 04:58:19 spitzak Exp $".
//
