//
// "$Id: Fl_Value_Output.H,v 1.10 1999/04/03 22:16:12 carl Exp $"
//
// Value output header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Output_H
#define Fl_Value_Output_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif
#include "Fl_Output.H"

class Fl_Value_Output : public Fl_Valuator {
public:
  Fl_Output output;

  virtual void loadstyle() const { output.loadstyle(); }

  int handle(int);
  void draw();
  void resize(int,int,int,int);
  Fl_Value_Output(int x,int y,int w,int h,const char *l=0);
  ~Fl_Value_Output() { _style = 0; } // So that it doesn't get free()ed twice!

  void soft(uchar x) {soft_ = x;}
  uchar soft() const {return soft_;}

  int is_style_forced(Fl_Widget::Attribute a) const
    { return Fl_Widget::is_style_forced(a); }
  int is_style_forced(Fl_Input_::Attribute a) const
    { return output.is_style_forced(a); }

  Fl_Font textfont() const {return output.textfont();}
  void textfont(uchar s) {output.textfont(s);}

  uchar textsize() const {return output.textsize();}
  void textsize(uchar s) {output.textsize(s);}

  Fl_Color textcolor() const {return output.textcolor();}
  void textcolor(uchar n) {output.textcolor(n);}

protected:
  uchar soft_;
  static void output_cb(Fl_Widget*,void*);
  virtual void value_damage(); // cause damage() due to value() changing
};

#endif

//
// End of "$Id: Fl_Value_Output.H,v 1.10 1999/04/03 22:16:12 carl Exp $".
//
