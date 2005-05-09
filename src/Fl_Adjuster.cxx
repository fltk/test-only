//
// "$Id$"
//
// Adjuster widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Bitmap.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Style_List.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Symbol.H>

#include "fastarrow.h"
static Fl_Bitmap fastarrow(fastarrow_bits, fastarrow_width, fastarrow_height);
#include "mediumarrow.h"
static Fl_Bitmap mediumarrow(mediumarrow_bits, mediumarrow_width, mediumarrow_height);
#include "slowarrow.h"
static Fl_Bitmap slowarrow(slowarrow_bits, slowarrow_width, slowarrow_height);


Fl_Symbol ADJUSTER_ARROWS[] ={
  Fl_Symbol(&slowarrow),
  Fl_Symbol(&mediumarrow),
  Fl_Symbol(&fastarrow)
};



// changing the value does not change the appearance:
void Fl_Adjuster::value_damage() {}

void Fl_Adjuster::draw() {

  int dx, dy, W, H;
  if (w()>=h()) {
    dx = W = w()/3;
    dy = 0; H = h();
  } else {
    dx = 0; W = w();
    dy = H = h()/3;
  }

  
  normal_();
  Fl_Color col, col_highl, c_norm, c_highl;
  Fl_Boxtype box_ = box();
  Fl_Boxtype box_highl_ = box_;
  if (active_r()){
    col = col_highl = color();
    c_norm = c_highl =selection_color();
    if(highlight__){
      highlight_();
      col_highl = color();
      c_highl = selection_color();
      box_highl_ = box();
    }
  }else{
    col = col_highl = fl_inactive(color());
    c_norm = c_highl =  fl_inactive(selection_color());

  }
  draw_box(drag==1?fl_down(highlight__ == 1? box_highl_ : box_):highlight__ == 1? box_highl_ : box_, x(),  y()+2*dy, W, H, highlight__ == 1? col_highl : col);
  draw_box(drag==2?fl_down(highlight__ == 2? box_highl_ : box_):highlight__ == 2? box_highl_ : box_, x()+dx, y()+dy, W, H, highlight__ == 2? col_highl : col);
  draw_box(drag==3?fl_down(highlight__ == 3? box_highl_ : box_):highlight__ == 3? box_highl_ : box_, x()+2*dx,  y(), W, H, highlight__ == 3? col_highl : col);

  if(highlight__ ==1)
    (down_box()+2)->draw(x(), y()+2*dy, W, H, c_highl);
  else
    (down_box()+2)->draw(x(), y()+2*dy, W, H, c_norm);

  if(highlight__ ==2)
    (down_box()+1)->draw(x()+dx, y()+ dy, W, H, c_highl);
  else
    (down_box()+1)->draw(x()+dx, y()+ dy, W, H, c_norm);

  if(highlight__ ==3)
    down_box()->draw(x() + 2*dx, y(), W, H, c_highl);
  else
    down_box()->draw(x()+2*dx, y(), W, H, c_norm);

  if(highlight__) highlight_(); else normal_();
  if (Fl::focus() == this) draw_focus();
}

int Fl_Adjuster::handle(int event) {
  double v;
  int old_highlight_ = highlight__;
  int delta;
  int mx = Fl::event_x();
  int my = Fl::event_y();
  switch (event) {


    case FL_LEAVE:
      normal();
      highlight__ = 0;
      return 1;

    case FL_ENTER:
      highlight();
      highlight__ = 4;
      //normal_();
    case FL_MOVE:

      if (! highlight__) return 0;
      if (w()>=h())
      	highlight__ = 3*(mx-x())/w() + 1;
      else
        highlight__ = 3-3*(my-y()-1)/h();
      if(old_highlight_ != highlight__) redraw();
      return 1;



    case FL_PUSH:
      if (Fl::visible_focus()) Fl::focus(this);
      ix = mx;
      if (w()>=h())
      	drag = 3*(mx-x())/w() + 1;
      else
	      drag = 3-3*(Fl::event_y()-y()-1)/h();
      handle_push();
      redraw();
      return 1;
    case FL_DRAG:
      if (w() >= h()) {
	      delta = x()+(drag-1)*w()/3;	// left edge of button
	      if (mx < delta)
	        delta = mx-delta;
	      else if (mx > (delta+w()/3)) // right edge of button
	        delta = mx-delta-w()/3;
	      else
	        delta = 0;
      } else {
	      if (mx < x())
	        delta = mx-x();
	      else if (mx > (x()+w()))
	        delta = mx-x()-w();
	      else
	        delta = 0;
      }
      switch (drag) {
      case 3: v = increment(previous_value(), delta); break;
      case 2: v = increment(previous_value(), delta*10); break;
      default:v = increment(previous_value(), delta*100); break;
      }
      handle_drag(soft() ? softclamp(v) : clamp(v));
      return 1;
    case FL_RELEASE:
      if (Fl::event_is_click()) { // detect click but no drag
	      if (Fl::event_state()&0xF0000) delta = -10;
	      else delta = 10;
	      switch (drag) {
	        case 3: v = increment(previous_value(), delta); break;
	        case 2: v = increment(previous_value(), delta*10); break;
	        default:v = increment(previous_value(), delta*100); break;
	      }
	      handle_drag(soft() ? softclamp(v) : clamp(v));
      }
      drag = 0;
      redraw();
      handle_release();
      return 1;
    case FL_KEYBOARD :
      switch (Fl::event_key()) {
	      case FL_Up:
          if (w() > h()) return 0;
	        handle_drag(clamp(increment(value(),-1)));
	        return 1;
	      case FL_Down:
          if (w() > h()) return 0;
	        handle_drag(clamp(increment(value(),1)));
	        return 1;
	      case FL_Left:
          if (w() < h()) return 0;
	        handle_drag(clamp(increment(value(),-1)));
	        return 1;
	      case FL_Right:
          if (w() < h()) return 0;
	        handle_drag(clamp(increment(value(),1)));
	        return 1;
	      default:
          return 0;
      }
      // break not required because of switch...

    case FL_FOCUS:
    case FL_UNFOCUS:
      if (Fl::visible_focus()) {
        redraw();
        return 1;
      } else return 0;

  }
  return 0;
}



Fl_Adjuster::Fl_Adjuster(int X, int Y, int W, int H, const char* l)
  : Fl_Valuator(X, Y, W, H, l) {
  highlight__ = 0;
  style_ = default_style();
  //box(FL_UP_BOX);
  step(1, 10000);
  //selection_color(FL_BLACK);
  drag = 0;
  soft_ = 1;
}


Fl_Adjuster::Style * Fl_Adjuster::default_style(){
  static Fl_Adjuster::Style * s = 0;
  if(!s){ // not yet initialized
    s = new Fl_Adjuster::Style(Fl_Button::default_style(), Style::ALL & ~(Style::SELECTION_COLOR | Style::DOWN_BOX));
    s->selection_color(FL_FOREGROUND_COLOR);
    s->down_box(ADJUSTER_ARROWS);
  }
  return s;
}




//
// End of "$Id$".
//
