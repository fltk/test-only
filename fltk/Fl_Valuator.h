//
// "$Id: Fl_Valuator.h,v 1.9 2002/10/29 00:37:23 easysw Exp $"
//
// Valuator header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Valuator_H
#define Fl_Valuator_H

#ifndef Fl_Widget_H
#include "Fl_Widget.h"
#endif

/**

   The Fl_Valuator class controls a single floating-point value and provides 
   a consistent interface to set the value(), range(), and step(), and 
   insures that callbacks are done the same for every object. 
   
   There are probably more of these classes in FLTK than any others: 

   \image valuators.gif

   In the above diagram each box surrounds an actual subclass. These are 
   further differentiated by setting the type() of the widget to the symbolic
   value labeling the widget. The ones labelled "0" are the default versions
   with a type(0). For consistency the symbol FL_VERTICAL is defined as zero.

*/
class FL_API Fl_Valuator : public Fl_Widget {

public:

  /*@{*/
  /**
   Get or set the current value. The new value is stored unchanged,
   even if it is outside the range or not a multiple of step(). 
  */
  double value() const {return value_;}
  int value(double);
  /*@}*/

  /*@{*/
  /**
   Sets the minimum and maximum values for the valuator. For most subclasses 
   the user cannot move the value outside this range if it starts inside 
   this range.

   These values should be multiples of the step() to avoid ambiguity and
   possible implementation changes.

   For most subclasses, the minimum may be greater than the maximum. This 
   has the effect of "reversing" the object so the larger values are in 
   the opposite direction. This also switches which end of the filled 
   sliders is filled.

   You probably need to redraw() the widget after changing the range.
  */
  float minimum() const {return minimum_;}
  void minimum(double a) {minimum_ = float(a);}

  float maximum() const {return maximum_;}
  void maximum(double a) {maximum_ = float(a);}

  void range(double a, double b) {minimum_ = float(a); maximum_ = float(b);}
  /*@}*/

  /*@{*/
  /**
   Get or set the step value. As the user moves the mouse the value is rounded
   to a multiple of this. Fl_Slider is very intelligent and tries to round to 
   a power of 10 times 1, 2, or 5 times this, so you may want to try setting a
   very small step like .0001. 

   If this is zero (the default) then all rounding is disabled. This results 
   in the smoothest controller movement but this is not recommended if you want
   to present the resulting numbers to the user as text, because they will have
   useless extra digits of precision. 

   For some widgets like Fl_Roller this is also the distance the value moves 
   when the user drags the mouse 1 pixel. In these cases if step() is zero 
   then it acts like it is .01. 

   Negative values produce undocumented results. 
  */
  float step() const {return step_;}
  void step(double a) {step_ = float(a);}
  /*@}*/

  /*@{*/
  /**
   The linesize is the amount the valuator moves in response to an 
   arrow key, or the user clicking an up/down button. The default 
   value is 1. Negative values and values that are not a multiple 
   of step() produce undocumented results.
  */
  float linesize() const {return linesize_;}
  void linesize(double a) {linesize_ = float(a);}
  /*@}*/

  /**
   Format the passed value to show enough digits for the current 
   step value. If the step has been set to zero then it does a %g
   format. If step is greater or equal to 1 it does %d format. The 
   characters are written into the passed buffer (which must be long
   enough, 40 characters is safe). 
  */
  virtual int format(char*);

  /**
   The default handler turns arrow and page up/down keystrokes, 
   and movements of the mouse wheel, into movements of the value.
   It is up to the subclass to filter out events it does not want
   to respond to before calling this. 
  */
  int handle(int);

#ifndef FLTK_2
  void step(double a, int b) {step(a/b);}
  void bounds(double a, double b) {minimum_=float(a); maximum_=float(b);}
  void precision(int);
#endif

protected:

  /**
   Creates a new Fl_Valuator widget using the given position, size,
   and label string. The default boxtype is FL_NO_BOX. 
  */
  Fl_Valuator(int X, int Y, int W, int H, const char* L);
  /** Value saved when handle_push() was last called. */
  double previous_value() const {return previous_value_;}
  /** Subclasses should call this when the user starts to change the value. */
  void handle_push() {previous_value_ = value_;}
  /**
   Subclasses should call this as the user moves the value. The passed 
   value is clamped to the range if previous_value() is inside the range,
   and then it is rounded to the nearest multiple of step(), and then stored.
   This may call the callback if the new value is different than the old one. 
  */
  void handle_drag(double newvalue);
  /**
   Subclasses should call this when the user stops moving the value.
   It may call the callback. 
  */
  void handle_release();

  /** Callback whenever value changes. */
  virtual void value_damage(); // callback whenever value changes
  /** Change the value() without calling value_damage(). */
  void set_value(double v) {value_ = v;} // change w/o doing value_damage

private:

  double value_;
  static double previous_value_;
  float minimum_;
  float maximum_;
  float step_;
  float linesize_;

};

#endif

//
// End of "$Id: Fl_Valuator.h,v 1.9 2002/10/29 00:37:23 easysw Exp $".
//
