//
// "$Id: Fl_Button.h,v 1.4 2002/01/20 07:37:15 spitzak Exp $"
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

class FL_API Fl_Button : public Fl_Widget {
public:
  enum { // values for type(), should match Fl_Button
    NORMAL = 0,
    TOGGLE = RESERVED_TYPE+1,
    RADIO  = RESERVED_TYPE+2,
    HIDDEN = 3
  };
  int   value() const {return Fl_Widget::value();}
  int	value(int);
  int	set();
  int	clear();
  void	setonly();
  virtual int handle(int);
  Fl_Button(int,int,int,int,const char * = 0);
  static Fl_Named_Style* default_style;

protected:
  virtual void draw();
  Fl_Flags draw_box(Fl_Flags);
};

#ifndef FLTK_2 // for back compatability:
#define FL_TOGGLE_BUTTON	Fl_Button::TOGGLE
#define FL_RADIO_BUTTON		Fl_Button::RADIO
#define FL_HIDDEN_BUTTON	Fl_Button::HIDDEN
#endif

#endif

//
// End of "$Id: Fl_Button.h,v 1.4 2002/01/20 07:37:15 spitzak Exp $".
//
