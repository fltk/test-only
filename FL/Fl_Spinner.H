//
// "$Id$"
//
// Spinner widget for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Spinner_H
#define Fl_Spinner_H

#include <fltk3/Spinner.h>
#include <FL/Fl_Group.H>


class FL_EXPORT Fl_Spinner : public Fl_Group {
  
public:
  
  Fl_Spinner() {}
  
  Fl_Spinner(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::Spinner(X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT_INT(Spinner, handle(int event), handle(event), Handle)

  const char *format() { 
    return ((fltk3::Spinner*)_p)->format();
  }

  void format(const char *f) {
    ((fltk3::Spinner*)_p)->format(f);
  }
  
  double maximum() const { 
    return ((fltk3::Spinner*)_p)->maximum();
  }
  
  void maximum(double m) { 
    ((fltk3::Spinner*)_p)->maximum(m);
  }
  
  double minimum() const {
    return ((fltk3::Spinner*)_p)->minimum();
  }
  
  void minimum(double m) {
    ((fltk3::Spinner*)_p)->minimum(m);
  }
  
  void range(double a, double b) { 
    ((fltk3::Spinner*)_p)->range(a, b);
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Spinner, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)
    
  double step() const {
    return ((fltk3::Spinner*)_p)->step();
  }
  
  void step(double s) {
    ((fltk3::Spinner*)_p)->step(s);
  }
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::Spinner*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::Spinner*)_p)->font( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::Spinner*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::Spinner*)_p)->textsize( fltk3::_1to3_fontsize(size); )
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::Spinner*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::Spinner*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
    
  uchar type() const { 
    return ((fltk3::Spinner*)_p)->type();
  }
    
  void type(uchar v) { 
    ((fltk3::Spinner*)_p)->type(v);
  }
    
  double value() const {
    return ((fltk3::Spinner*)_p)->value();
  }
    
  void value(double v) {
    ((fltk3::Spinner*)_p)->value(v);
  }
    
};


#endif // !Fl_Spinner_H

//
// End of "$Id$".
//
