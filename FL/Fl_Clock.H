//
// "$Id$"
//
// Clock header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
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

#ifndef Fl_Clock_H
#define Fl_Clock_H

#include <fltk3/Clock.h>
#include "Fl_Widget.H"

#define FL_SQUARE_CLOCK		0
#define FL_ROUND_CLOCK		1
#define FL_ANALOG_CLOCK FL_SQUARE_CLOCK
#define FL_DIGITAL_CLOCK FL_SQUARE_CLOCK


class FL_EXPORT Fl_Clock_Output : public Fl_Widget {

protected:
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Widget, draw(), draw(), Draw)

  void draw(int X, int Y, int W, int H) {
    ((fltk3::ClockOutput*)_p)->draw(X, Y, W, H);
  }
  
public:

  Fl_Clock_Output() { /* empty */ }
  
  Fl_Clock_Output(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::ClockOutput(X, Y, W, H, L);
    _p->wrapper(this);
  }

  void value(ulong v) {
    ((fltk3::ClockOutput*)_p)->value(v);
  }
  
  void value(int H, int m, int s) {
    ((fltk3::ClockOutput*)_p)->value(H, m ,s);
  }
  
  ulong value() const {
    return ((fltk3::ClockOutput*)_p)->value();
  }

  int hour() const {
    return ((fltk3::ClockOutput*)_p)->hour();
  }

  int minute() const {
    return ((fltk3::ClockOutput*)_p)->minute();
  }

  int second() const {
    return ((fltk3::ClockOutput*)_p)->second();
  }
};


class FL_EXPORT Fl_Clock : public Fl_Clock_Output {

public:

  FLTK3_WRAPPER_VCALLS_OBJECT_INT(Widget, handle(int event), handle(event), Handle)

  Fl_Clock() { /* empty */ }
  
  Fl_Clock(int X, int Y, int W, int H,  const char *L = 0) {
    _p = new fltk3::Clock(X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  Fl_Clock(uchar t, int X, int Y, int W, int H, const char *L) {
    _p = new fltk3::Clock(t, X, Y, W, H, L);
    _p->wrapper(this);
  }
  
};

#endif

//
// End of "$Id$".
//
