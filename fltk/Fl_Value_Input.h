//
// "$Id: Fl_Value_Input.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Value input header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Input_H
#define Fl_Value_Input_H

#include "Fl_Valuator.h"
#include "Fl_Float_Input.h"

/**

   Controls a single floating point value through a combination of a 
   Fl_Float_Input and two buttons. Other toolkits call this a "Spin Box". 

   \image Fl_Value_Input.gif

   Clicking the buttons increments/decrements by the linesize(). Holding
   down shift (or ctrl or alt) and clicking increments/decrements by the
   pagesize(). 

   If step() is greater or equal to 1.0 an Fl_Int_Input is used instead. 
   This prevents the user from typing anything other than digits. 
   If step() is less than one then the user can type floating point
   values with decimal points and exponents. 

   If you change when() to FL_WHEN_ENTER_KEY the callback is only done 
   when the user hits the up/down arrow keys or when the user types the
   Enter key. This may be more useful than the default setting of 
   FL_WHEN_CHANGED which can make the callback happen when partially-edited
   numbers are in the field. 

   You can get at the input field by using the public "input" 
   instance variable. For instance you can clobber the text to a 
   word with value_input->input.static_value("word"). 

*/
class FL_API Fl_Value_Input : public Fl_Valuator {
public:
  Fl_Float_Input input;

  int handle(int);
  void draw();
  /**
   Creates a new Fl_Value_Input widget using the given position, size, 
   and label string. The default boxtype is FL_DOWN_BOX.
  */
  Fl_Value_Input(int x,int y,int w,int h,const char *l=0);
  /** Destroys the valuator. */
  ~Fl_Value_Input();

protected:
  void layout();

private:
  virtual void value_damage(); // cause damage() due to value() changing
  static void input_cb(Fl_Widget*,void*);
  void increment_cb();
  static void repeat_callback(void* v);
};

#endif

//
// End of "$Id: Fl_Value_Input.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
