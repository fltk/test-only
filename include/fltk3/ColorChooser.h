//
// "$Id$"
//
// Color chooser header file for the Fast Light Tool Kit (FLTK).
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

/** \file
 fltk3::ColorChooser widget . */

// The color chooser object and the color chooser popup.  The popup
// is just a window containing a single color chooser and some boxes
// to indicate the current and cancelled color.

#ifndef Fltk3_Color_Chooser_H
#define Fltk3_Color_Chooser_H

#include <fltk3/Group.h>
#include <fltk3/Box.h>
#include <fltk3/ReturnButton.h>
#include <fltk3/Choice.h>
#include <fltk3/ValueInput.h>

namespace fltk3 {
  
#ifndef FLTK3_DOXYGEN
  
  /** For internal use only */
  class FLTK3_EXPORT cc_HueBox : public fltk3::Widget {
    int px, py;
  protected:
    void draw();
    int handle_key(int);
  public:
    int handle(int);
    cc_HueBox(int X, int Y, int W, int H) : fltk3::Widget(X,Y,W,H) {
      px = py = 0;}
  };
  
  /** For internal use only */
  class FLTK3_EXPORT cc_ValueBox : public fltk3::Widget {
    int py;
  protected:
    void draw();
    int handle_key(int);
  public:
    int handle(int);
    cc_ValueBox(int X, int Y, int W, int H) : fltk3::Widget(X,Y,W,H) {
      py = 0;}
  };
  
  /** For internal use only */
  class FLTK3_EXPORT cc_Value_Input : public fltk3::ValueInput {
  public:
    int format(char*);
    cc_Value_Input(int X, int Y, int W, int H) : fltk3::ValueInput(X,Y,W,H) {}
  };
  
#endif // !FLTK3_DOXYGEN
  
  /** \addtogroup group_comdlg 
   @{ */
  
  /**
   \class fltk3::ColorChooser
   \brief The fltk3::ColorChooser widget provides a standard RGB color chooser.
   
   \image html fltk3::color_chooser.jpg
   \image latex fltk3::color_chooser.jpg "fltk3::color_chooser()" width=5cm
   
   You can place any number of the widgets into a panel of your own design.
   The diagram shows the widget as part of a color chooser dialog created by
   the fltk3::color_chooser() function. The fltk3::ColorChooser widget contains the
   hue box, value slider, and rgb input fields from the above diagram (it
   does not have the color chips or the Cancel or OK buttons).
   The callback is done every time the user changes the rgb value. It is not
   done if they move the hue control in a way that produces the \e same rgb
   value, such as when saturation or value is zero.
   
   The fltk3::color_chooser() function pops up a window to let the user pick an
   arbitrary RGB color. They can pick the hue and saturation in the "hue box"
   on the left (hold down CTRL to just change the saturation), and the
   brightness using the vertical slider. Or they can type the 8-bit numbers
   into the RGB fltk3::ValueInput fields, or drag the mouse across them to adjust
   them.  The pull-down menu lets the user set the input fields to show RGB,
   HSV, or 8-bit RGB (0 to 255).
   
   fltk3::color_chooser() returns non-zero if the user picks ok, and updates the
   RGB values.  If the user picks cancel or closes the window this returns
   zero and leaves RGB unchanged.
   
   If you use the color chooser on an 8-bit screen, it will allocate all the
   available colors, leaving you no space to exactly represent the color the
   user picks! You can however use fltk3::rectf() to fill a region with a simulated
   color using dithering.
   */
  /** @} */
  class FLTK3_EXPORT ColorChooser : public fltk3::Group {
    cc_HueBox huebox;
    cc_ValueBox valuebox;
    fltk3::Choice choice;
    cc_Value_Input rvalue;
    cc_Value_Input gvalue;
    cc_Value_Input bvalue;
    fltk3::Box resize_box;
    double hue_, saturation_, value_;
    double r_, g_, b_;
    void set_valuators();
    static void rgb_cb(fltk3::Widget*, void*);
    static void mode_cb(fltk3::Widget*, void*);
  public:
    
    /** 
     Returns which fltk3::ColorChooser variant is currently active 
     \return color modes are rgb(0), byte(1), hex(2), or hsv(3)   
     */
    int mode() {return choice.value();}
    
    /** 
     Set which fltk3::ColorChooser variant is currently active 
     \param[in] newMode color modes are rgb(0), byte(1), hex(2), or hsv(3)   
     */
    void mode(int newMode);
    
    /**
     Returns the current hue.
     0 <= hue < 6. Zero is red, one is yellow, two is green, etc.
     <em>This value is convenient for the internal calculations - some other
     systems consider hue to run from zero to one, or from 0 to 360.</em>
     */
    double hue() const {return hue_;}
    
    /**
     Returns the saturation.
     0 <= saturation <= 1.
     */
    double saturation() const {return saturation_;}
    
    /**
     Returns the value/brightness.
     0 <= value <= 1.
     */
    double value() const {return value_;}
    
    /**
     Returns the current red value.
     0 <= r <= 1.
     */
    double r() const {return r_;}
    
    /**
     Returns the current green value.
     0 <= g <= 1.
     */
    double g() const {return g_;}
    
    /**
     Returns the current blue value.
     0 <= b <= 1.
     */
    double b() const {return b_;}
    
    int hsv(double H, double S, double V);
    
    int rgb(double R, double G, double B);
    
    static void hsv2rgb(double H, double S, double V, double& R, double& G, double& B);
    
    static void rgb2hsv(double R, double G, double B, double& H, double& S, double& V);
    
    ColorChooser(int X, int Y, int W, int H, const char *L = 0);
  };
  
  FLTK3_EXPORT int color_chooser(const char* name, double& r, double& g, double& b, int m=-1);
  FLTK3_EXPORT int color_chooser(const char* name, uchar& r, uchar& g, uchar& b, int m=-1);
  
} // namespace

#endif

//
// End of "$Id$".
//
