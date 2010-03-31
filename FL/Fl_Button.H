//
// "$Id$"
//
// Button header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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
   Fl_Button widget . */

#ifndef Fltk1_Button_H
#define Fltk1_Button_H

#include "Fl_Widget.H"

#include <fltk3/Button.h>

// values for type()
#define FL_NORMAL_BUTTON	0   /**< value() will be set to 1 during the press of the button and 
                                         reverts back to 0 when the button is released */
#define FL_TOGGLE_BUTTON	1   ///< value() toggles between 0 and 1 at every click of the button
#define FL_RADIO_BUTTON		(FL_RESERVED_TYPE+2) /**< is set to 1 at button press, and all other
				         buttons in the same group with <tt>type() == FL_RADIO_BUTTON</tt>
				         are set to zero.*/
#define FL_HIDDEN_BUTTON	3   ///< for Forms compatibility

#ifndef FL_DOXYGEN
extern FL_EXPORT Fl_Shortcut fl_old_shortcut(const char*);
#endif

class FL_EXPORT Fl_Button : public Fl_Widget {
  
#if 0
  int shortcut_;
  char value_;
  char oldval;
  uchar down_box_;

protected:
  virtual void draw();
#endif
  
public:
  
#if 0
  virtual int handle(int);
#endif
  
  Fl_Button(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::Button(x, y, w, h, label); 
    _p->wrapper(this);
  }
  
#if 0
  int value(int v);
  char value() const {return value_;}
  int set() {return value(1);}
  int clear() {return value(0);}
  void setonly(); // this should only be called on FL_RADIO_BUTTONs
  int shortcut() const {return shortcut_;}
  void shortcut(int s) {shortcut_ = s;}
  Fl_Boxtype down_box() const {return (Fl_Boxtype)down_box_;}
  void down_box(Fl_Boxtype b) {down_box_ = b;}
  void shortcut(const char *s) {shortcut(fl_old_shortcut(s));}
  Fl_Color down_color() const {return selection_color();}
  void down_color(unsigned c) {selection_color(c);}
#endif
  
};

#endif

//
// End of "$Id$".
//
