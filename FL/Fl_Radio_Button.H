//
// "$Id: Fl_Radio_Button.H,v 1.10 1999/11/20 04:42:30 vincent Exp $"
//
// Radio button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Radio_Button_H
#define Fl_Radio_Button_H

#include "Fl_Check_Button.H"

class FL_API Fl_Radio_Button : public Fl_Check_Button {
public:
    Fl_Radio_Button(int x,int y,int w,int h,const char *l=0)
	: Fl_Check_Button(x,y,w,h,l) {type(FL_RADIO_BUTTON);}
private:
  static Fl_Style* default_style;
};

#endif

//
// End of "$Id: Fl_Radio_Button.H,v 1.10 1999/11/20 04:42:30 vincent Exp $".
//
