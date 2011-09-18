//
// "$Id$"
//
// Color chooser header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//  - three widgets marked "internal" were not wrapped!
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

#ifndef Fl_Color_Chooser_H
#define Fl_Color_Chooser_H

#include <fltk3/ColorChooser.h>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Input.H>


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Color_Chooser, ColorChooser)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Color_Chooser, ColorChooser)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Color_Chooser : public Fl_Group {

public:
  
  FLTK3_WIDGET_VCALLS(ColorChooser)

  int mode() {
    return ((fltk3::ColorChooser*)_p)->mode();
  }
            
  void mode(int newMode) {
    ((fltk3::ColorChooser*)_p)->mode(newMode);
  }
  
  double hue() const {
    return ((fltk3::ColorChooser*)_p)->hue();
  }
  
  double saturation() const {
    return ((fltk3::ColorChooser*)_p)->saturation();
  }
  
  double value() const {
    return ((fltk3::ColorChooser*)_p)->value();
  }
  
  double r() const {
    return ((fltk3::ColorChooser*)_p)->r();
  }
  
  double g() const {
    return ((fltk3::ColorChooser*)_p)->g();
  }
  
  double b() const {
    return ((fltk3::ColorChooser*)_p)->b();
  }
  
  int hsv(double H, double S, double V) {
    return ((fltk3::ColorChooser*)_p)->hsv(H, S, V);
  }
  
  int rgb(double R, double G, double B) {
    return ((fltk3::ColorChooser*)_p)->rgb(R, G, B);
  }
  
  static void hsv2rgb(double H, double S, double V, double& R, double& G, double& B) {
    fltk3::ColorChooser::hsv2rgb(H, S, V, R, G, B);
  }

  static void rgb2hsv(double R, double G, double B, double& H, double& S, double& V) {
    fltk3::ColorChooser::rgb2hsv(R, G, B, H, S, V);
  }
  
  Fl_Color_Chooser() { /* empty */ }
  
  Fl_Color_Chooser(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::ColorChooser_I(X, Y, W, H, L);
    _p->wrapper(this);
  }
  
};


inline int fl_color_chooser(const char* name, double& r, double& g, double& b, int m=-1) {
  return fltk3::color_chooser(name, r, g, b, m);
}

inline int fl_color_chooser(const char* name, uchar& r, uchar& g, uchar& b, int m=-1) {
  return fltk3::color_chooser(name, r, g, b, m);
}


#endif

//
// End of "$Id$".
//
