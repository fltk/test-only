//
// "$Id: Fl_Check_Button.H,v 1.10 1999/09/19 04:58:52 vincent Exp $"
//
// Check button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Check_Button_H
#define Fl_Check_Button_H

#include "Fl_Button.H"

class Fl_Check_Button : public Fl_Button {
public:
  FL_IMPORT static Fl_Style default_style;
  virtual int handle(int);
  Fl_Check_Button(int x,int y,int w,int h,const char *l = 0);
protected:
  virtual void draw();
};

#endif

//
// End of "$Id: Fl_Check_Button.H,v 1.10 1999/09/19 04:58:52 vincent Exp $".
//
