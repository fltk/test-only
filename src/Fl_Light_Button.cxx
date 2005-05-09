//
// "$Id$"
//
// Lighted button widget for the Fast Light Tool Kit (FLTK).
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

// Subclass of Fl_Button where the "box" indicates whether it is
// pushed or not, and the "down box" is drawn small and square on
// the left to indicate the current state.

// The default down_box of zero draws a rectangle designed to look
// just like Flame's buttons.

#include <FL/Fl.H>
#include <FL/Fl_Light_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Style.H>






void Fl_Light_Button::draw() {
  
  if (box()) draw_box(this==Fl::pushed() ? fl_down(box()) : box(), color());

  int W  = labelsize();
  int align_ = align();
  int dy = (h() - W) / 2 ;

  Fl_Boxtype b = box();
  int dx = Fl::box_dx(b);
  int dw = Fl::box_dw(b);
  
  if(align_ & FL_ALIGN_RIGHT){
    draw_box(down_box(), x()+w() - dw + dx - W - 2, y()+dy, W, W, selection_color());
    draw_label(x(),y(),w()-  W - 4 - 2*(dw-dx), h());
  }else{
    draw_box(down_box(), x()+dx+2, y()+dy, W, W, selection_color());
    draw_label(x()+W+2*dx +4, y(), w()-W-2*dx -4, h());
  }

  if (Fl::focus() == this) draw_focus();
}


int Fl_Light_Button::handle(int event) {
  switch (event) {
  case FL_RELEASE:
    if (box()) redraw();
  default:
    return Fl_Button::handle(event);
  }
}



Fl_Light_Button::Fl_Light_Button(int X, int Y, int W, int H, const char* l)
: Fl_Button(X, Y, W, H, l){
  type(FL_TOGGLE_BUTTON);
  style_ = default_style(); 
  align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
}


static void fl_light_box(int x, int y, int w, int h, Fl_Color c){
  if(h<3) h = 3;
  int w1 = h * 2 / 3;
  if(w1 < 3) w = 3;
  x = x + (w-w1) / 2;
  Fl_Color col = Fl::draw_box_value() ? c : FL_BACKGROUND_COLOR;
  if(!Fl::draw_box_active()) col = fl_inactive(col);
 
  FL_THIN_DOWN_BOX->draw(x,y,w1,h,col);
}



static Fl_Symbol FL_LIGHT_BOX_(fl_light_box, 0, 0, 0, 0);
FL_EXPORT Fl_Boxtype FL_LIGHT_BOX = & FL_LIGHT_BOX_;




Fl_Button::Style * Fl_Light_Button::default_style(){
  static Style * s = 0;
  if(!s){ // not yet initialized
    s = new Style(Fl_Button::default_style(), Style::ALL & ~Style::SELECTION_COLOR & ~Style::DOWN_BOX);
    s->selection_color(FL_RED);
    s->down_box(FL_LIGHT_BOX);
  }
  return s;
}






//
// End of "$Id$".
//
