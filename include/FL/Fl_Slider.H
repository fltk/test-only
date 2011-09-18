//
// "$Id$"
//
// Slider header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Slider_H
#define Fl_Slider_H

#include <fltk3/Slider.h>
#include "Fl_Valuator.H"

#define FL_VERT_SLIDER		0
#define FL_HOR_SLIDER		1
#define FL_VERT_FILL_SLIDER	2
#define FL_HOR_FILL_SLIDER	3
#define FL_VERT_NICE_SLIDER	4
#define FL_HOR_NICE_SLIDER	5


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Slider, Slider)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Slider, Slider)
  void draw(int x, int y, int w, int h) { Slider::draw(x, y, w, h); }
  int handle(int e, int x, int y, int w, int h) { return Slider::handle(e, x, y, w, h); }
  Slider_I(uchar t, int X,int Y,int W,int H, const char *L = 0) : Slider(t, X, Y, W, H, L) {}
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Slider : public Fl_Valuator {

protected:

  void draw(int x, int y, int w, int h) {
    ((fltk3::Slider_I*)_p)->draw(x, y, w, h);
  }
  
  int handle(int e, int x, int y, int w, int h) {
    return ((fltk3::Slider_I*)_p)->handle(e, x, y, w, h);
  }
  
public:
  
  FLTK3_WIDGET_VCALLS(Slider)

  Fl_Slider() { /* empty */ }
  
  Fl_Slider(int X,int Y,int W,int H, const char *L = 0) {
    _p = new fltk3::Slider_I(X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  Fl_Slider(uchar t, int X,int Y,int W,int H, const char *L = 0) {
    _p = new fltk3::Slider_I(t, X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  void bounds(double a, double b) {
    ((fltk3::Slider*)_p)->bounds(a, b);
  }
  
  int scrollvalue(int pos,int size,int first,int total) {
    return ((fltk3::Slider*)_p)->scrollvalue(pos, size, first, total);
  }
  
  float slider_size() const {
    return ((fltk3::Slider*)_p)->slider_size();
  }
  
  void slider_size(double v) {
    ((fltk3::Slider*)_p)->slider_size(v);
  }
  
  Fl_Boxtype slider() const {
    return fltk3::_3to1_boxtype( ((fltk3::Slider*)_p)->slider() );
  }
  
  void slider(Fl_Boxtype c) {
    ((fltk3::Slider*)_p)->slider( fltk3::_1to3_boxtype(c) );
  }

};

#endif

//
// End of "$Id$".
//
