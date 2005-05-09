//
// "$Id$"
//
// Check button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

#include <FL/Fl.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>

// A subclass of Fl_Button that always draws as a diamond box.  This
// diamond is smaller than the widget size and can be surchecked by
// another box type, for compatability with Forms.

Fl_Check_Button::Fl_Check_Button(int X, int Y, int W, int H, const char *l)
: Fl_Round_Button(X, Y, W, H, l) {
  style_ = default_style();
}



static inline void fl_draw_check(int x, int y, int w,int h){
      int tx = x + 1;
      int tw = w - 2;
      int d1 = tw/3;
      int d2 = tw-d1;
      int ty = y + (w+d2)/2-d1-2;
      for (int n = 0; n < 3; n++, ty++) {
        fl_line(tx, ty, tx+d1, ty+d1);
        fl_line(tx+d1, ty+d1, tx+tw-1, ty+d1-d2+1);
      }
}

static void fl_check_box(int x, int y, int w, int h, Fl_Color c){
  FL_DOWN_BOX->draw(x,y,w,h,FL_BACKGROUND2_COLOR);
  if(Fl::draw_box_value()){
    fl_color( Fl::draw_box_active() ? c : fl_inactive(c));
    fl_draw_check(x + FL_DOWN_BOX->dx(),y + FL_DOWN_BOX->dy(),w - FL_DOWN_BOX->dw(),h - FL_DOWN_BOX->dh());
  }
}

Fl_Symbol FL_CHECK_BOX_(&fl_check_box, 3, 3, 6, 6);
FL_EXPORT Fl_Boxtype FL_CHECK_BOX = &FL_CHECK_BOX_;



Fl_Button::Style * Fl_Check_Button::default_style(){
  static Style * s = 0;
  if(!s){ // not yet initialized
    s = new Style(Fl_Round_Button::default_style(), Style::ALL & ~Style::DOWN_BOX);
    s->down_box(FL_CHECK_BOX);
  }
  return s;
}

