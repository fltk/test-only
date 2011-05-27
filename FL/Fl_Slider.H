//
// "$Id$"
//
// Slider header file for the Fast Light Tool Kit (FLTK).
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


class FL_EXPORT Fl_Slider : public Fl_Valuator {

#if 0 // FIXME: 123
  
protected:

  // these allow subclasses to put the slider in a smaller area:
  void draw(int, int, int, int);
  int handle(int, int, int, int, int);
  void draw();
  
#endif
  
public:

  Fl_Slider() { /* empty */ }
  
  Fl_Slider(int X,int Y,int W,int H, const char *L = 0) {
    _p = new fltk3::Slider(X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  Fl_Slider(uchar t, int X,int Y,int W,int H, const char *L = 0) {
    _p = new fltk3::Slider(t, X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  void bounds(double a, double b) {
    ((fltk3::Slider*)_p)->bounds(a, b);
  }
  
#if 0 // FIXME: 123

  ivirtual nt handle(int);
  int scrollvalue(int pos,int size,int first,int total);
  float slider_size() const {return slider_size_;}
  void slider_size(double v);
  Fl_Boxtype slider() const {return (Fl_Boxtype)slider_;}
  void slider(Fl_Boxtype c) {slider_ = c;}

#endif

};

#endif

//
// End of "$Id$".
//
