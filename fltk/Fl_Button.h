//
// "$Id: Fl_Button.h,v 1.7 2002/10/29 00:37:22 easysw Exp $"
//
// Button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Button_H
#define Fl_Button_H

#ifndef Fl_Widget_H
#include "Fl_Widget.h"
#endif

/**
   Buttons generate callbacks when they are clicked by the user. 
   You control exactly when and how by changing the values for type() 
   and when(). 

   Buttons can also generate callbacks in response to FL_SHORTCUT events.
   The button can either have an explicit shortcut() value or a letter 
   shortcut can be indicated in the label() with an '&' character before it.
   For the label shortcut it does not matter if Alt is held down, but if you 
   have an input field in the same window, the user will have to hold down 
   the Alt key so that the input field does not eat the event first as 
   an FL_KEY event. 
*/
class FL_API Fl_Button : public Fl_Widget {
public:
   /**
    Values for type().
    - 0: The value is unchanged. 
    - FL_TOGGLE_BUTTON: The value is inverted. 
    - FL_RADIO_BUTTON: The value is set to 1, and all 
       other buttons in the current group with type() == FL_RADIO_BUTTON 
       are set to zero. 
   */
  enum { // , should match Fl_Button
    NORMAL = 0,
    TOGGLE = RESERVED_TYPE+1,
    RADIO  = RESERVED_TYPE+2,
    HIDDEN = 3
  };
  /*@{*/
  /**
   The first form returns the current value (0 or 1). 
   The second form sets the current value.
  */
  bool  value() const {return Fl_Widget::value();}
  bool	value(bool);
  /*@}*/
  /**
   Same as value(1). 
  */
  bool	set();
  /** Same as value(0). */
  bool	clear();
  /**
   Turns on this button and turns off all other radio buttons in 
   the group (calling value(1) or set() does not do this). 
  */
  void	setonly();
  virtual int handle(int);

  /**
   The constructor creates the button using the position, size, and label.
  */
  Fl_Button(int x,int y,int w,int h,const char * label= 0);
  static Fl_Named_Style* default_style;

protected:
  virtual void draw();
  void draw(int glyph, int glyph_width) const;
};

#ifndef FLTK_2 // for back compatability:
#define FL_TOGGLE_BUTTON	Fl_Button::TOGGLE
#define FL_RADIO_BUTTON		Fl_Button::RADIO
#define FL_HIDDEN_BUTTON	Fl_Button::HIDDEN
#endif

#endif

//
// End of "$Id: Fl_Button.h,v 1.7 2002/10/29 00:37:22 easysw Exp $".
//
