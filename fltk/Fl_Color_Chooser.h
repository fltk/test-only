//
// "$Id: Fl_Color_Chooser.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Color chooser header file for the Fast Light Tool Kit (FLTK).
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

// The color chooser object and the color chooser popup.  The popup
// is just a window containing a single color chooser and some boxes
// to indicate the current and cancelled color.

#ifndef Fl_Color_Chooser_H
#define Fl_Color_Chooser_H

#include <fltk/Fl_Group.h>
#include <fltk/Fl_Box.h>
#include <fltk/Fl_Return_Button.h>
#include <fltk/Fl_Choice.h>
#include <fltk/Fl_Value_Input.h>

class FL_API Flcc_HueBox : public Fl_Widget {
  int px, py;
protected:
  void draw();
public:
  int handle(int);
  Flcc_HueBox(int X, int Y, int W, int H) : Fl_Widget(X,Y,W,H) {
  px = py = 0;}
};

class FL_API Flcc_ValueBox : public Fl_Widget {
  int py;
protected:
  void draw();
public:
  int handle(int);
  Flcc_ValueBox(int X, int Y, int W, int H) : Fl_Widget(X,Y,W,H) {
  py = 0;}
};

class FL_API Flcc_Value_Input : public Fl_Value_Input {
public:
  int format(char*);
  Flcc_Value_Input(int X, int Y, int W, int H) : Fl_Value_Input(X,Y,W,H) {}
};

/**
   The Fl_Color_Chooser widget provides a standard RGB color chooser.
   You can place any number of these into a panel of your own design. 
   This widget contains the hue box, value slider, and rgb input fields 
   from the above diagram (it does not have the color chips or the Cancel 
   or OK buttons). The callback is done every time the user changes the rgb 
   value. It is not done if they move the hue control in a way that produces 
   the same rgb value, such as when saturation or value is zero.
*/
class FL_API Fl_Color_Chooser : public Fl_Group {
  Flcc_HueBox huebox;
  Flcc_ValueBox valuebox;
  Fl_Group nrgroup; // no resize group
  Fl_Choice choice;
  Flcc_Value_Input rvalue;
  Flcc_Value_Input gvalue;
  Flcc_Value_Input bvalue;
  float hue_, saturation_, value_;
  float r_, g_, b_;
  void set_valuators();
  static void rgb_cb(Fl_Widget*, void*);
  static void mode_cb(Fl_Widget*, void*);
public:
  int mode() {return choice.value();}
  /** 
   Return the current hue. 0 <= hue < 6. Zero is red, one is yellow, two is green, etc.
   This value is convienent for the internal calculations - some other systems
   consider hue to run from zero to one, or from 0 to 360. 
  */
  float hue() const {return hue_;}
  /**
   Returns the saturation. 0 <= saturation <= 1.
  */
  float s() const {return saturation_;}
  /** turns the value/brightness. 0 <= value <= 1. */
  float v() const {return value_;}
  /** Returns the current red value. 0 <= r <= 1. */
  float r() const {return r_;}
  /** Returns the current green value. 0 <= g <= 1. */
  float g() const {return g_;}
  /** Returns the current blue value. 0 <= b <= 1. */
  float b() const {return b_;}
  Fl_Color value() const;
  void value(Fl_Color);
  /**
   Set the hsv values. The passed values are clamped (or for
   hue, modulus 6 is used) to get legal values. Does not do the callback.
  */
  int hsv(float,float,float);
  /**
   Sets the current rgb color values. Does not do the callback. 
   Does not clamp (but out of range values will produce psychedelic
   effects in the hue selector). 
  */
  int rgb(float,float,float);
  /** This static method converts HSV colors to RGB colorspace. */
  static void hsv2rgb(float, float, float,float&,float&,float&);
  /** This static method converts RGB colors to HSV colorspace. */
  static void rgb2hsv(float, float, float,float&,float&,float&);
  /**
   Creates a new Fl_Color_Chooser widget using the given position, 
   size, and label string. The recommended dimensions are 200x95. 
   The color is initialized to black. 
  */
  Fl_Color_Chooser(int x,int y,int w,int h,const char* label= 0);
};

/*@{*/
/**
   Shown here is the output of the function fl_color_chooser(), 
   which you may want to use. This window contains an Fl_Color_Chooser 
   and also two color chips below it and the ok/cancel buttons. 
   
   \image fl_color_chooser.jpg
*/
FL_API int fl_color_chooser(const char*name, float& r, float& g, float& b);
FL_API int fl_color_chooser(const char*name, uchar& r, uchar& g, uchar& b);
FL_API int fl_color_chooser(const char*name, Fl_Color& c);
/*@}*/

#endif

//
// End of "$Id: Fl_Color_Chooser.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
