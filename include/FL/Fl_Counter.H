//
// "$Id$"
//
// Counter header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Counter_H
#define Fl_Counter_H

#include <fltk3/Counter.h>
#include "Fl_Valuator.H"

#define FL_NORMAL_COUNTER	0
#define FL_SIMPLE_COUNTER	1


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Counter, Counter)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Counter, Counter)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Counter : public Fl_Valuator {

public:

  FLTK3_WIDGET_VCALLS(Counter)

  Fl_Counter() { /* empty */ }
  
  Fl_Counter(int X, int Y, int W, int H, const char* L = 0) {
    _p = new fltk3::Counter(X, Y, W, H, L);
    _p->wrapper(this);
  }

  void lstep(double a) {
    ((fltk3::Counter*)_p)->lstep(a);
  }

  void step(double a,double b) {
    ((fltk3::Counter*)_p)->step(a, b);
  }

  void step(double a) {
    ((fltk3::Counter*)_p)->step(a);
  }

  double step() const {
    return ((fltk3::Counter*)_p)->step();
  }
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::Counter*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::Counter*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::Counter*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::Counter*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::Counter*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::Counter*)_p)->textcolor( fltk3::_1to3_color(col) );
  }

};

#endif

//
// End of "$Id$".
//
