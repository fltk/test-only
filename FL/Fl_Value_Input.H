//
// "$Id: Fl_Value_Input.H,v 1.12 1999/08/16 07:30:53 bill Exp $"
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

#include "Fl_Valuator.H"
#include "Fl_Input.H"

class Fl_Value_Input : public Fl_Valuator {
public:
  Fl_Input input;

  int handle(int);
  void draw();
  Fl_Value_Input(int x,int y,int w,int h,const char *l=0);
  ~Fl_Value_Input();

  void soft(char x) {soft_ = x;}
  char soft() const {return soft_;}

protected:
  void layout();

private:
  char soft_;
  static void input_cb(Fl_Widget*,void*);
  virtual void value_damage(); // cause damage() due to value() changing
};

#endif

//
// End of "$Id: Fl_Value_Input.H,v 1.12 1999/08/16 07:30:53 bill Exp $".
//
