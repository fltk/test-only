/
// "$Id$
/
// Round button for the Fast Light Tool Kit (FLTK)
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

// A subclass of Fl_Button that always draws as a round circle.  Thi
// circle is smaller than the widget size and can be surrounded b
// another box type, for compatability with Forms

#include <FL/Fl.H
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Symbol.H>




Fl_Round_Button::Fl_Round_Button(int X,int Y,int W,int H, const char *l
: Fl_Light_Button(X,Y,W,H,l) 
  style_ = default_style()
}


Fl_Button::Style * Fl_Round_Button::default_style(){
  static Style * s = 0;
  if(!s){ // not yet initialized
    s = new Style(Fl_Button::default_style(), Style::ALL & ~(Style::SELECTION_COLOR|Style::DOWN_BOX|Style::BOX));
    s->selection_color(FL_FOREGROUND_COLOR);
    s->down_box(FL_ROUND_DOWN_BOX);
    s->box(FL_NO_BOX);
  }
  return s;
}


void Fl_Round_Button::draw() {
  
  if (box()) draw_box(this==Fl::pushed() ? fl_down(box()) : box(), color());
  Fl_Color col = value() ? (active_r() ? selection_color() :
                            fl_inactive(selection_color())) : color();
  int W;
  int dx, dy;

  W  = labelsize();
  dx = Fl::box_dx(box()) + 2;
  dy = (h() - W) / 2;
  // if (dy < 0) dy = 0;         // neg. offset o.k. for vertical centering

  draw_box(down_box(), x()+dx, y()+dy, W, W, FL_BACKGROUND2_COLOR);
  if (value()) {
    fl_color(col);
    int tW = (W - Fl::box_dw(down_box())) / 2 + 1;
    if ((W - tW) & 1) tW++; // Make sure difference is even to center
    int tdx = dx + (W - tW) / 2;
    int tdy = dy + (W - tW) / 2;
    switch (tW) {
    // Larger circles draw fine...
      default :
         fl_pie(x() + tdx, y() + tdy, tW, tW, 0.0, 360.0);
        break;
        // Small circles don't draw well on many systems...
      case 6 :
        fl_rectf(x() + tdx + 2, y() + tdy, tW - 4, tW);
        fl_rectf(x() + tdx + 1, y() + tdy + 1, tW - 2, tW - 2);
        fl_rectf(x() + tdx, y() + tdy + 2, tW, tW - 4);
        break;
       case 5 :
      case 4 :
      case 3 :
        fl_rectf(x() + tdx + 1, y() + tdy, tW - 2, tW);
        fl_rectf(x() + tdx, y() + tdy + 1, tW, tW - 2);
        break;
 	    case 2 :
      case 1 :
        fl_rectf(x() + tdx, y() + tdy, tW, tW);
        break;
    }
  }

  draw_label(x()+W+2*dx, y(), w()-W-2*dx, h());
  if (Fl::focus() == this) draw_focus();
}



/
// End of "$Id$"
/
