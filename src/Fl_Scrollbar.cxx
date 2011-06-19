//
// "$Id$"
//
// Scroll bar widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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


#include <fltk3/run.h>
#include <fltk3/Scrollbar.h>
#include <fltk3/draw.h>
#include <math.h>
#include "flstring.h"

#define INITIALREPEAT .5
#define REPEAT .05

void fltk3::Scrollbar::increment_cb() {
  char inv = maximum()<minimum();
  int ls = inv ? -linesize_ : linesize_;
  int i;
  switch (pushed_) {
    case 1: // clicked on arrow left
      i = -ls;
      break;
    default: // clicked on arrow right
      i =  ls;
      break;
    case 5: // clicked into the box next to the slider on the left
      i = -(int((maximum()-minimum())*slider_size()/(1.0-slider_size())));
      if (inv) {
        if (i<-ls) i = -ls;
      } else {
        if (i>-ls) i = -ls; // err
      }
      break;
    case 6: // clicked into the box next to the slider on the right
      i = (int((maximum()-minimum())*slider_size()/(1.0-slider_size())));
      if (inv) {
        if (i>ls) i = ls;
      } else {
        if (i<ls) i = ls; // err
      }
      break;
  }
  handle_drag(clamp(value() + i));
}

void fltk3::Scrollbar::timeout_cb(void* v) {
  fltk3::Scrollbar* s = (fltk3::Scrollbar*)v;
  s->increment_cb();
  fltk3::add_timeout(REPEAT, timeout_cb, s);
}

int fltk3::Scrollbar::handle(int event) {
  // area of scrollbar:
  int area;
  int X=x(); int Y=y(); int W=w(); int H=h();

  // adjust slider area to be inside the arrow buttons:
  if (horizontal()) {
    if (W >= 3*H) {X += H; W -= 2*H;}
  } else {
    if (H >= 3*W) {Y += W; H -= 2*W;}
  }

  // which widget part is highlighted?
  int relx;
  int ww;
  if (horizontal()) {
    relx = fltk3::event_x()-X;
    ww = W;
  } else {
    relx = fltk3::event_y()-Y;
    ww = H;
  }
  if (relx < 0) area = 1;
  else if (relx >= ww) area = 2;
  else {
    int S = int(slider_size()*ww+.5);
    int T = (horizontal() ? H : W)/2+1;
    if (type()==fltk3::VERT_NICE_SLIDER || type()==fltk3::HOR_NICE_SLIDER) T += 4;
    if (S < T) S = T;
    double val =
      (maximum()-minimum()) ? (value()-minimum())/(maximum()-minimum()) : 0.5;
    int sliderx;
    if (val >= 1.0) sliderx = ww-S;
    else if (val <= 0.0) sliderx = 0;
    else sliderx = int(val*(ww-S)+.5);
    if (fltk3::event_button() == fltk3::MIDDLE_MOUSE) area = 8;
    else if (relx < sliderx) area = 5;
    else if (relx >= sliderx+S) area = 6;
    else area = 8;
  }

  switch (event) {
  case fltk3::ENTER:
  case fltk3::LEAVE:
    return 1;
  case fltk3::RELEASE:
      damage(fltk3::DAMAGE_ALL);
    if (pushed_) {
      fltk3::remove_timeout(timeout_cb, this);
      pushed_ = 0;
    }
    handle_release();
    return 1;
  case fltk3::PUSH:
    if (pushed_) return 1;
    if (area != 8) pushed_ = area;
    if (pushed_) {
      handle_push();
      fltk3::add_timeout(INITIALREPEAT, timeout_cb, this);
      increment_cb();
      damage(fltk3::DAMAGE_ALL);
      return 1;
    }
    return fltk3::Slider::handle(event, X,Y,W,H);
  case fltk3::DRAG:
    if (pushed_) return 1;
    return fltk3::Slider::handle(event, X,Y,W,H);
  case fltk3::MOUSEWHEEL :
    if (horizontal()) {
      if (fltk3::e_dx==0) return 0;
      int ls = maximum()>=minimum() ? linesize_ : -linesize_;
      handle_drag(clamp(value() + ls * fltk3::e_dx));
      return 1;
    } else {
      if (fltk3::e_dy==0) return 0;
      int ls = maximum()>=minimum() ? linesize_ : -linesize_;
      handle_drag(clamp(value() + ls * fltk3::e_dy));
      return 1;
    }
  case fltk3::SHORTCUT:
  case fltk3::KEYBOARD: {
    int v = value();
    int ls = maximum()>=minimum() ? linesize_ : -linesize_;
    if (horizontal()) {
      switch (fltk3::event_key()) {
      case fltk3::LeftKey:
	v -= ls;
	break;
      case fltk3::RightKey:
	v += ls;
	break;
      default:
	return 0;
      }
    } else { // vertical
      switch (fltk3::event_key()) {
      case fltk3::UpKey:
	v -= ls;
	break;
      case fltk3::DownKey:
	v += ls;
	break;
      case fltk3::PageUpKey:
	if (slider_size() >= 1.0) return 0;
	v -= int((maximum()-minimum())*slider_size()/(1.0-slider_size()));
	v += ls;
	break;
      case fltk3::PageDownKey:
	if (slider_size() >= 1.0) return 0;
	v += int((maximum()-minimum())*slider_size()/(1.0-slider_size()));
	v -= ls;
	break;
      case fltk3::HomeKey:
	v = int(minimum());
	break;
      case fltk3::EndKey:
	v = int(maximum());
	break;
      default:
	return 0;
      }
    }
    v = int(clamp(v));
    if (v != value()) {
      fltk3::Slider::value(v);
      value_damage();
      set_changed();
      do_callback();
    }
    return 1;}
  }
  return 0;
}

void fltk3::Scrollbar::draw() {
  FLTK3_OBJECT_VCALLS_WRAPPER(draw(), Draw)
  if (damage()&fltk3::DAMAGE_ALL) draw_box();
  int X = x()+fltk3::box_dx(box());
  int Y = y()+fltk3::box_dy(box());
  int W = w()-fltk3::box_dw(box());
  int H = h()-fltk3::box_dh(box());
  if (horizontal()) {
    if (W < 3*H) {fltk3::Slider::draw(X,Y,W,H); return;}
    fltk3::Slider::draw(X+H,Y,W-2*H,H);
    if (damage()&fltk3::DAMAGE_ALL) {
      draw_box((pushed_==1) ? fltk3::down(slider()) : slider(),
	       X, Y, H, H, selection_color());
      draw_box((pushed_==2) ? fltk3::down(slider()) : slider(),
	       X+W-H, Y, H, H, selection_color());
      if (active_r())
        fltk3::color(labelcolor());
      else
        fltk3::color(fltk3::inactive(labelcolor()));
      int w1 = (H-4)/3; if (w1 < 1) w1 = 1;
      int x1 = X+(H-w1-1)/2;
      int yy1 = Y+(H-2*w1-1)/2;
      if (fltk3::scheme_ && !strcmp(fltk3::scheme_, "gtk+")) {
	fltk3::polygon(x1, yy1+w1, x1+w1, yy1+2*w1, x1+w1-1, yy1+w1, x1+w1, yy1);
	x1 += (W-H);
	fltk3::polygon(x1, yy1, x1+1, yy1+w1, x1, yy1+2*w1, x1+w1, yy1+w1);
      } else {
	fltk3::polygon(x1, yy1+w1, x1+w1, yy1+2*w1, x1+w1, yy1);
	x1 += (W-H);
	fltk3::polygon(x1, yy1, x1, yy1+2*w1, x1+w1, yy1+w1);
      }
    }
  } else { // vertical
    if (H < 3*W) {fltk3::Slider::draw(X,Y,W,H); return;}
    fltk3::Slider::draw(X,Y+W,W,H-2*W);
    if (damage()&fltk3::DAMAGE_ALL) {
      draw_box((pushed_==1) ? fltk3::down(slider()) : slider(),
	       X, Y, W, W, selection_color());
      draw_box((pushed_==2) ? fltk3::down(slider()) : slider(),
	       X, Y+H-W, W, W, selection_color());
      if (active_r())
        fltk3::color(labelcolor());
      else
        fltk3::color(fltk3::inactive(labelcolor()));
      int w1 = (W-4)/3; if (w1 < 1) w1 = 1;
      int x1 = X+(W-2*w1-1)/2;
      int yy1 = Y+(W-w1-1)/2;
      if (fltk3::scheme_ && !strcmp(fltk3::scheme_, "gtk+")) {
	fltk3::polygon(x1, yy1+w1, x1+w1, yy1+w1-1, x1+2*w1, yy1+w1, x1+w1, yy1);
	yy1 += H-W;
	fltk3::polygon(x1, yy1, x1+w1, yy1+1, x1+w1, yy1+w1);
	fltk3::polygon(x1+w1, yy1+1, x1+2*w1, yy1, x1+w1, yy1+w1);
      } else {
	fltk3::polygon(x1, yy1+w1, x1+2*w1, yy1+w1, x1+w1, yy1);
	yy1 += H-W;
	fltk3::polygon(x1, yy1, x1+w1, yy1+w1, x1+2*w1, yy1);
      }
    }
  }
}

/**
  Creates a new fltk3::Scrollbar widget with given position, size, and label.
  You need to do type(fltk3::HORIZONTAL) if you want a horizontal scrollbar.
*/
fltk3::Scrollbar::Scrollbar(int X, int Y, int W, int H, const char* L)
  : fltk3::Slider(X, Y, W, H, L) {
  box(fltk3::FLAT_BOX);
  color(fltk3::DARK2);
  slider(fltk3::UP_BOX);
  linesize_ = 16;
  pushed_ = 0;
  step(1);
}

/**  Destroys the Scrollbar. */
fltk3::Scrollbar::~Scrollbar() {
  if (pushed_)
    fltk3::remove_timeout(timeout_cb, this);
}


//
// End of "$Id$".
//
