//
// "$Id$"
//
// Spinner widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

/* \file
 fltk3::Spinner widget . */

#ifndef Fltk3_Spinner_H
#  define Fltk3_Spinner_H

#  include <fltk3/enumerations.h>
#  include <fltk3/Group.h>
#  include <fltk3/Input.h>
#  include <fltk3/RepeatButton.h>
#  include <stdio.h>
#  include <stdlib.h>

namespace fltk3 {
  
  /**
   This widget is a combination of the input
   widget and repeat buttons. The user can either type into the
   input area or use the buttons to change the value.
   */
  class FLTK3_EXPORT Spinner : public fltk3::Group {
    
    class UpButton : public RepeatButton {
    public:
      UpButton(int x, int y, int w, int h, const char *l=0);
      void draw();
    };
    
    class DownButton : public RepeatButton {
    public:
      DownButton(int x, int y, int w, int h, const char *l=0);
      void draw();
    };
    
    class SpInput : public Input {
    public:
      SpInput(int x, int y, int w, int h, const char *l=0);
      void draw();
    };
    
    double        value_;               // Current value
    double        minimum_;             // Minimum value
    double        maximum_;             // Maximum value
    double        step_;                // Amount to add/subtract for up/down
    const char*   format_;              // Format string
    
    SpInput       input_;               // Input field for the value
    UpButton      up_button_;           // Up button
    DownButton    down_button_;         // Down button
    
    static void	sb_cb(fltk3::Widget *w, fltk3::Spinner *sb);
    
    void update();
    
  public:
    
    /**
     Creates a new fltk3::Spinner widget using the given position, size,
     and label string.
     
     Inherited destructor Destroys the widget and any value associated with it.
     */
    Spinner(int X, int Y, int W, int H, const char *L = 0);
    
    /** Sets or returns the format string for the value. */
    const char *format() { return (format_); }
    
    /** Sets or returns the format string for the value. */
    void format(const char *f) { format_ = f; update(); }
    
    int handle(int event);
    
    /** Speling mistakes retained for source compatibility \deprecated */
    double maxinum() const { return (maximum_); }
    
    /** Gets the maximum value of the widget. */
    double maximum() const { return (maximum_); }
    
    /** Sets the maximum value of the widget. */
    void maximum(double m) { maximum_ = m; }
    
    /** Speling mistakes retained for source compatibility \deprecated */
    double mininum() const { return (minimum_); }
    
    /** Gets the minimum value of the widget. */
    double minimum() const { return (minimum_); }
    
    /** Sets the minimum value of the widget. */
    void minimum(double m) { minimum_ = m; }
    
    /** Sets the minimum and maximum values for the widget. */
    void range(double a, double b) { minimum_ = a; maximum_ = b; }
    
    void resize(int X, int Y, int W, int H);
    
    /**
     Sets or returns the amount to change the value when the user clicks a button. 
     Before setting step to a non-integer value, the spinner 
     type() should be changed to floating point. 
     */
    double step() const { return (step_); }
    
    /** See double fltk3::Spinner::step() const */
    void step(double s);
    
    /** Gets the color of the text in the input field. */
    fltk3::Color textcolor() const { return (input_.textcolor()); }
    
    /** Sets the color of the text in the input field. */
    void textcolor(fltk3::Color c) { input_.textcolor(c); }
    
    /** Gets the font of the text in the input field. */
    fltk3::Font textfont() const { return (input_.textfont()); }
    
    /** Sets the font of the text in the input field. */
    void textfont(fltk3::Font f) { input_.textfont(f); }
    
    /** Gets the size of the text in the input field. */
    fltk3::Fontsize  textsize() const { return (input_.textsize()); }
    
    /** Sets the size of the text in the input field. */
    void textsize(fltk3::Fontsize s) { input_.textsize(s); }
    
    /** Gets the numeric representation in the input field.
     \see fltk3::Spinner::type(uchar) 
     */
    uchar type() const { return (input_.type()); }
    
    /** Sets the numeric representation in the input field.
     Valid values are fltk3::INT_INPUT and fltk3::FLOAT_INPUT.
     Also changes the format() template.
     Setting a new spinner type via a superclass pointer will not work.
     \note  type is not a virtual function. 
     */
    void type(uchar v);
    
    /** Gets the current value of the widget. */
    double value() const { return (value_); }
    
    /**
     Sets the current value of the widget.
     Before setting value to a non-integer value, the spinner 
     type() should be changed to floating point. 
     */
    void value(double v) { value_ = v; update(); }
    
  };
  
}

#endif // !Fltk3_Spinner_H

//
// End of "$Id$".
//
