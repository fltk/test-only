//
// "$Id: Fl_Button.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $"
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

// values for type()
#define FL_TOGGLE_BUTTON	(FL_RESERVED_TYPE+1)
#define FL_RADIO_BUTTON		(FL_RESERVED_TYPE+2)
#define FL_HIDDEN_BUTTON	3 // for Forms compatability

class FL_API Fl_Button : public Fl_Widget {
public:
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
  void draw_n_clip();
};

#endif

//
// End of "$Id: Fl_Button.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $".
//
