/
// "$Id$
/
// Check button widget for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#include <FL/Fl.H
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Style.H

// A subclass of Fl_Button that always draws as a diamond box.  Thi
// diamond is smaller than the widget size and can be surchecked b
// another box type, for compatability with Forms

Fl_Check_Button::Fl_Check_Button(int X, int Y, int W, int H, const char *l
: Fl_Light_Button(X, Y, W, H, l) {
  style_ = default_style()
  //box(FL_NO_BOX)
  //down_box(FL_DOWN_BOX)
  //selection_color(FL_FOREGROUND_COLOR); //was:FL_BLAC




Fl_Button::Style * Fl_Check_Button::default_style(){
  static Style * s = 0;
  if(!s){ // not yet initialized
    s = new Style(Fl_Round_Button::default_style(), Style::ALL & ~Style::DOWN_BOX);
    s->down_box(FL_DOWN_BOX);
  }
  return s;
}

