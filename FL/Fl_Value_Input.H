//
// "$Id: Fl_Value_Input.H,v 1.10 1999/06/15 17:02:26 gustavo Exp $"
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

  virtual void loadstyle() const { input.loadstyle(); }
  int handle(int);
  void draw();
  Fl_Value_Input(int x,int y,int w,int h,const char *l=0);
  ~Fl_Value_Input() { _style = 0; } // so that it doesn't get free()ed twice!

  void soft(char x) {soft_ = x;}
  char soft() const {return soft_;}

  int is_style_forced(Fl_Widget::Attribute a) const
    { return Fl_Widget::is_style_forced(a); }
  int is_style_forced(Fl_Input_::Attribute a) const
    { return input.is_style_forced(a); }

  Fl_Font textfont() const {return input.textfont();}
  void textfont(uchar s) {input.textfont(s);}

  uchar textsize() const {return input.textsize();}
  void textsize(uchar s) {input.textsize(s);}

  Fl_Color textcolor() const {return input.textcolor();}
  void textcolor(uchar n) {input.textcolor(n);}

  Fl_Color cursor_color() const {return input.cursor_color();}
  void cursor_color(uchar n) {input.cursor_color(n);}

protected:
  void layout();

private:
  char soft_;
  static void input_cb(Fl_Widget*,void*);
  virtual void value_damage(); // cause damage() due to value() changing
};

#endif

//
// End of "$Id: Fl_Value_Input.H,v 1.10 1999/06/15 17:02:26 gustavo Exp $".
//
