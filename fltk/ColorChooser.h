//
// "$Id: ColorChooser.h,v 1.2 2002/12/10 01:46:15 easysw Exp $"
//
// Color chooser header file for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// The color chooser object and the color chooser popup.  The popup
// is just a window containing a single color chooser and some boxes
// to indicate the current and cancelled color.

#ifndef fltk_ColorChooser_h
#define fltk_ColorChooser_h

#include <fltk/Group.h>
#include <fltk/Choice.h>
#include <fltk/ValueInput.h>

namespace fltk {

class FL_API ccHueBox : public Widget {
  int px, py;
protected:
  void draw();
public:
  int handle(int);
  ccHueBox(int X, int Y, int W, int H) : Widget(X,Y,W,H) {
  px = py = 0;}
};

class FL_API ccValueBox : public Widget {
  int py;
protected:
  void draw();
public:
  int handle(int);
  ccValueBox(int X, int Y, int W, int H) : Widget(X,Y,W,H) {
  py = 0;}
};

class FL_API ccValueInput : public ValueInput {
public:
  int format(char*);
  ccValueInput(int X, int Y, int W, int H) : ValueInput(X,Y,W,H) {}
};

class FL_API ColorChooser : public Group {
  ccHueBox huebox;
  ccValueBox valuebox;
  Group nrgroup; // no resize group
  Choice choice;
  ccValueInput rvalue;
  ccValueInput gvalue;
  ccValueInput bvalue;
  float hue_, saturation_, value_;
  float r_, g_, b_;
  void set_valuators();
  static void rgb_cb(Widget*, void*);
  static void mode_cb(Widget*, void*);
public:
  int mode() {return choice.value();}
  float h() const {return hue_;}
  float s() const {return saturation_;}
  float v() const {return value_;}
  float r() const {return r_;}
  float g() const {return g_;}
  float b() const {return b_;}
  Color value() const;
  void value(Color);
  int hsv(float,float,float);
  int rgb(float,float,float);
  static void hsv2rgb(float, float, float,float&,float&,float&);
  static void rgb2hsv(float, float, float,float&,float&,float&);
  ColorChooser(int,int,int,int,const char* = 0);
};

FL_API int color_chooser(const char* name, float& r, float& g, float& b);
FL_API int color_chooser(const char* name, uchar& r, uchar& g, uchar& b);
FL_API int color_chooser(const char* name, Color& c);

}

#endif

//
// End of "$Id: ColorChooser.h,v 1.2 2002/12/10 01:46:15 easysw Exp $".
//
