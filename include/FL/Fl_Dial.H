//
// "$Id$"
//
// Dial header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Dial_H
#define Fl_Dial_H

#include <fltk3/Dial.h>
#include "Fl_Valuator.H"

#define FL_NORMAL_DIAL	0
#define FL_LINE_DIAL	1
#define FL_FILL_DIAL	2


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Dial, Dial)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Dial, Dial)
void draw(int X, int Y, int W, int H) { Dial::draw(X, Y, W, H); }
int handle(int event, int X, int Y, int W, int H) { return Dial::handle(event, X, Y, W, H); }
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Dial : public Fl_Valuator {

protected:

  void draw(int X, int Y, int W, int H) {
    ((fltk3::Dial_I*)_p)->draw(X, Y, W, H);
  }
  
  int handle(int event, int X, int Y, int W, int H) {
    return ((fltk3::Dial_I*)_p)->handle(event, X, Y, W, H);
  }
  
public:

  FLTK3_WIDGET_VCALLS(Dial)

  Fl_Dial() { /* empty */ }
  
  Fl_Dial(int x,int y,int w,int h, const char *l = 0) {
    _p = new fltk3::Dial_I(x, y, w, h, l);
    _p->wrapper(this);
  }
  
  short angle1() const {
    return ((fltk3::Dial*)_p)->angle1();
  }
  
  void angle1(short a) {
    ((fltk3::Dial*)_p)->angle1(a);
  }
  
  short angle2() const {
    return ((fltk3::Dial*)_p)->angle2();
  }
  
  void angle2(short a) {
    ((fltk3::Dial*)_p)->angle2(a);
  }
  
  void angles(short a, short b) {
    ((fltk3::Dial*)_p)->angles(a, b);
  }

};

#endif

//
// End of "$Id$".
//
