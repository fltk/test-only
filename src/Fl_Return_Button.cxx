//
// "$Id$"
//
// Return button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <FL/Fl.H>
#include <FL/Fl_Return_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Style_List.H>


int fl_return_arrow(int x, int y, int w, int h) {
  int size = w; if (h<size) size = h;
  int d = (size+2)/4; if (d<3) d = 3;
  int t = (size+9)/12; if (t<1) t = 1;
  int x0 = x+(w-2*d-2*t-1)/2;
  int x1 = x0+d;
  int y0 = y+h/2;
  fl_color(FL_LIGHT3);
  fl_line(x0, y0, x1, y0+d);
  fl_yxline(x1, y0+d, y0+t, x1+d+2*t, y0-d);
  fl_yxline(x1, y0-t, y0-d);
  fl_color(fl_gray_ramp(0));
  fl_line(x0, y0, x1, y0-d);
  fl_color(FL_DARK3);
  fl_xyline(x1+1, y0-t, x1+d, y0-d, x1+d+2*t);
  return 1;
}

static void fl_return_arrow_(int x, int y, int w, int h, Fl_Color c){fl_return_arrow(x,y,w,h);}


Fl_Symbol FL_RETURN_ARROW_(fl_return_arrow_);
FL_EXPORT Fl_Boxtype FL_RETURN_ARROW = &FL_RETURN_ARROW_;

void Fl_Cancel_Button::draw() {
  if (type() == FL_HIDDEN_BUTTON) return;
  draw_box(value() ? fl_down(box()) : box(),
	   value() ? selection_color() : color());

  uchar align_ = symbol_align();
  int W = 0;
  int xl_ =x()+2;
  int x_;
  if(down_box()){
    if(align_ & (FL_ALIGN_LEFT|FL_ALIGN_RIGHT)){
      W = h();
      if (w()/3 < W) W = w()/3;


      if(align_ & FL_ALIGN_RIGHT){
        x_ = x()+w() - W - 4;
        xl_ = x();
      }else{
        x_ =x()+4;
        xl_ =x()+W+4;
      }
      draw_box(down_box(), x_, y(), W, h(),selection_color());
      draw_label(xl_, y(), w()-W - 4, h());
    }else{
      int w_ = w()  - Fl::box_dw(box())-4;
      int h_ = 0;
      if(align() & FL_ALIGN_INSIDE)
        fl_measure(label(), w_, h_);
      W = h() - Fl::box_dh(box()) - h_ - 2;
      if((h() - 6) < W) W = h() - 6;
      if(align_ & FL_ALIGN_TOP){
        draw_box(down_box(), x() + w()/2 - W/2, y() + Fl::box_dy(box()) + 2, W, W,selection_color());
        draw_label(x(),y()+h() + Fl::box_dy(box()) - Fl::box_dh(box()) - h_ - 2, w(), h_);
      }else{
        draw_label(x(),y() + Fl::box_dy(box()) +2, w(), h_);
        int offset = (h() - Fl::box_dh(box()) - 2 - h_) / 2;
        draw_box(down_box(), x() + w()/2 - W/2, y() + h_ + 2 + offset - W/2, W, W, selection_color());
      }
    }
    
  }else
    draw_label(xl_, y(), w()-W - 4, h());
  if (Fl::focus() == this) draw_focus();

}

int Fl_Return_Button::handle(int event) {
  if (event == FL_SHORTCUT &&
      (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)) {
    do_callback();
    return 1;
  } else
    return Fl_Button::handle(event);
}










uchar Fl_Cancel_Button::symbol_align() const{
  return ((Fl_Cancel_Button::Style *)style_)->symbol_align();
}
void Fl_Cancel_Button::symbol_align(uchar a){
  dynamic_style();
  ((Fl_Cancel_Button::Style *)style_)->symbol_align_ = a;
  ((Fl_Cancel_Button::Style *)style_)->set_flag(Style::SYMBOL_ALIGN);
}


Fl_Cancel_Button::Style::Style(Fl_Widget::Style * parent, unsigned mode):Fl_Button::Style(parent,BASE){
  symbol_align_ = FL_ALIGN_RIGHT;
  clear_flag(SYMBOL_ALIGN);
  init(parent,mode);
}



void Fl_Cancel_Button::Style::cancel_button_update(Fl_Widget::Style * s, unsigned what){
  if(!s) return;
  Fl_Button_Style::update_(this, s, what);
  if(SYMBOL_ALIGN & ~(s->flags()) & what) ((Fl_Cancel_Button::Style *)s)->symbol_align_ = symbol_align_;
};

void Fl_Cancel_Button::Style::symbol_align(uchar a) {
  symbol_align_= a;
  set_flag (SYMBOL_ALIGN);
  fl_update_styles(symbol_align,SYMBOL_ALIGN,a);
}

Fl_Cancel_Button::Style * Fl_Cancel_Button::default_style(){
  static Fl_Cancel_Button::Style * s = 0;
  if(!s){ // not yet initialized
    s = new Fl_Cancel_Button::Style(Fl_Button::default_style(), Style::ALL & ~Style::DOWN_BOX);
    s->symbol_align(FL_ALIGN_RIGHT);
  }
  return s;
}


Fl_Cancel_Button::Style * Fl_Return_Button::default_style(){
  static Fl_Cancel_Button::Style * s = 0;
  if(!s){ // not yet initialized
    s = new Fl_Cancel_Button::Style(Fl_Cancel_Button::default_style(), Style::ALL & ~Style::DOWN_BOX);
    s->down_box(FL_RETURN_ARROW);
  }
  return s;
}






//
// End of "$Id$".
//
