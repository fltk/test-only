//
// "$Id$"
//
// Forms chart header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Chart_H
#define Fl_Chart_H

#include <fltk3/Chart.h>
#include "Fl_Widget.H"

#define FL_BAR_CHART		0
#define FL_HORBAR_CHART		1
#define FL_LINE_CHART		2
#define FL_FILL_CHART		3
#define FL_SPIKE_CHART		4
#define FL_PIE_CHART		5
#define FL_SPECIALPIE_CHART	6

#define FL_FILLED_CHART  FL_FILL_CHART

#define FL_CHART_MAX		128
#define FL_CHART_LABEL_MAX	18


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Chart, Chart)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Chart, Chart)
FLTK3_WRAPPER_INTERFACE_END()


struct FL_CHART_ENTRY {
   float val;
   unsigned col;
   char str[FL_CHART_LABEL_MAX+1];
};


class FL_EXPORT Fl_Chart : public Fl_Widget {

public:
  
  FLTK3_WIDGET_VCALLS(Fl_Chart, Chart)

  Fl_chart() { /* empty */ }
  
  Fl_Chart(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::Chart_I(_ctor_dx(X), _ctor_dy(Y), W, H, L);
    _p->wrapper(this);
  }

  void clear() {
    ((fltk3::Chart*)_p)->clear();
  }

  void add(double val, const char *str = 0, unsigned col = 0) {
    ((fltk3::Chart*)_p)->add(val, str, col);
  }

  void insert(int ind, double val, const char *str = 0, unsigned col = 0) {
    ((fltk3::Chart*)_p)->insert(ind, val, str, col);
  }

  void replace(int ind, double val, const char *str = 0, unsigned col = 0) {
    ((fltk3::Chart*)_p)->replace(ind, val, str, col);
  }

  void bounds(double *a,double *b) const {
    ((fltk3::Chart*)_p)->bounds(a, b);
  }

  void bounds(double a,double b) {
    ((fltk3::Chart*)_p)->bounds(a, b);
  }

  int size() const {
    return ((fltk3::Chart*)_p)->size();
  }

  void size(int W, int H) {
    ((fltk3::Chart*)_p)->size(W, H);
  }

  int maxsize() const {
    return ((fltk3::Chart*)_p)->maxsize();
  }

  void maxsize(int m) {
    ((fltk3::Chart*)_p)->maxsize();
  }

  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::Chart*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::Chart*)_p)->font( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::Chart*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::Chart*)_p)->textsize( fltk3::_1to3_fontsize(size); )
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::Chart*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::Chart*)_p)->textcolor( fltk3::_1to3_color(col) );
  }

  uchar autosize() const {
    return ((fltk3::Chart*)_p)->autosize();
  }

  void autosize(uchar n) {
    ((fltk3::Chart*)_p)->autosize(n);
  }
};

#endif

//
// End of "$Id$".
//
