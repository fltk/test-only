//
// "$Id: Fl_Scrollbar.cxx,v 1.54 2001/12/16 22:32:03 spitzak Exp $"
//
// Scroll bar widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Scrollbar.h>
#include <fltk/fl_draw.h>
#include <math.h>
#include <stdlib.h>

int Fl_Scrollbar::value(int p, int w, int t, int l) {
//	p = position, first line displayed
//	w = window, number of lines displayed
//	t = top, number of first line
//	l = length, total number of lines
  if (p+w > t+l) l = p+w-t;
  if (l <= 0) l = 1;
  int b = l-w+t;
  int X=0; int Y=0; int W=this->w(); int H=h(); text_box()->inset(X,Y,W,H);
  if (!horizontal()) {int T = W; W = H; H = T; T = b; b = t; t = T;}
  if (W >= 3*H) W -= 2*H;
  int S = W*w/l; if (S < H) S = H; if (S > W) S = W;
  if (S != slider_size() || t != minimum() || b != maximum()) {
    slider_size(S); minimum(t); maximum(b); redraw();
  }
  pagesize(w>1 ? w-1 : 1);
  return Fl_Slider::value(p);
}

#define INITIALREPEAT .5
#define REPEAT .05

void Fl_Scrollbar::increment_cb() {
  int i;
  switch (pushed_) {
  case 1: i = -linesize(); break;
  default:i =  linesize(); break;
  case 3: i = -pagesize(); break;
  case 4: i =  pagesize(); break;
  }
  handle_drag(clamp(value()+i));
}

void Fl_Scrollbar::timeout_cb(void* v) {
  Fl_Scrollbar* s = (Fl_Scrollbar*)v;
  s->increment_cb();
  Fl::add_timeout(REPEAT, (Fl_Timeout_Handler)timeout_cb, s);
}

int Fl_Scrollbar::handle(int event) {
  // area of scrollbar:
  int X=0; int Y=0; int W=w(); int H=h(); text_box()->inset(X,Y,W,H);

  // adjust slider area to be inside the arrow buttons:
  if (horizontal()) {
    if (W >= 3*H) {X += H; W -= 2*H;}
  } else {
    if (H >= 3*W) {Y += W; H -= 2*W;}
  }

  // which widget part is highlighted?
  int mx = Fl::event_x();
  int my = Fl::event_y();
  int which_part;
  if (!Fl::event_inside(0, 0, w(), h())) which_part = 0;
  else if (horizontal()) {
    if (mx < X) which_part = 1;
    else if (mx >= X+W) which_part = 2;
    else {
      int sliderx = slider_position(W, slider_size(W, H));
      if (mx < X+sliderx) which_part = 3;
      else if (mx >= X+sliderx+slider_size(W, H)) which_part = 4;
      else which_part = 5;
    }
  } else {
    if (my < Y) which_part = 1;
    else if (my >= Y+H) which_part = 2;
    else {
      int slidery = slider_position(H, slider_size(H, W));
      if (my < Y+slidery) which_part = 3;
      else if (my >= Y+slidery+slider_size(H, W)) which_part = 4;
      else which_part = 5;
    }
  }
  switch (event) {
  case FL_LEAVE:
  case FL_ENTER:
  case FL_MOVE:
    if (Fl::pushed() != this) highlight_ = which_part;
    if (last_ != highlight_) redraw(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_PUSH:
    if (pushed_) return 1;
      pushed_ = which_part;
    if (pushed_ && pushed_ != 5) {
      handle_push();
      Fl::add_timeout(INITIALREPEAT, (Fl_Timeout_Handler)timeout_cb, this);
      increment_cb();
      redraw(FL_DAMAGE_HIGHLIGHT);
      return 1;
    }
    return Fl_Slider::handle(event, X,Y,W,H);
  case FL_DRAG:
    if (pushed_ != 5) return 1;
    return Fl_Slider::handle(event, X,Y,W,H);
  case FL_RELEASE:
    if (pushed_) Fl::remove_timeout((Fl_Timeout_Handler)timeout_cb, this);
    highlight_ = which_part;
    last_ = pushed_; // so that it will unpush without redrawing everything
    pushed_ = 0;
    redraw(FL_DAMAGE_HIGHLIGHT);
    handle_release();
    return 1;
  case FL_MOUSEWHEEL: {
    int n = (horizontal() ? Fl::event_dx() : Fl::event_dy())
      * Fl_Style::wheel_scroll_lines;
    int sign = 1; if (n < 0) {n = -n; sign = -1;}
    n *= linesize();
    if (n > pagesize()) n = pagesize();
    handle_drag(clamp(increment(value(), n * sign)));
    return 1;
  }
  case FL_KEYBOARD:
    if (!horizontal()) switch(Fl::event_key()) {
    case FL_Home: handle_drag(maximum()); return 1;
    case FL_End:  handle_drag(minimum()); return 1;
    } // else fall through...
  default:
    return Fl_Slider::handle(event);
  }
}

// Fltk 2.0 is incompatable with the use of color:
// color area:                                  1.0:            2.0:
// background behind scrollbar                  color           text_background
// scrollbar slider & buttons                   FL_GRAY         color
// scrollbar slider & buttons when selected     FL_GRAY         selection_color
// scrollbar button symbols                     selection_color text_color
// scrollbar button symbols when selected       selection_color selection_text_color

void Fl_Scrollbar::draw() {
  if (damage()&FL_DAMAGE_ALL) draw_text_frame();

  int X=0; int Y=0; int W=w(); int H=h(); text_box()->inset(X,Y,W,H);

  // 1 = left/top   2 = right/bottom   5 = slider button
  Fl_Flags f1 = 0, f2 = 0, f5 = 0;
  if (!active_r()) {
    f1 |= FL_INACTIVE; f2 |= FL_INACTIVE; f5 |= FL_INACTIVE;
  } else {
    if (pushed_ == 1) f1 = FL_VALUE|FL_SELECTED;
    else if (highlight_ == 1) f1 = FL_HIGHLIGHT;
    if (pushed_ == 2) f2 = FL_VALUE|FL_SELECTED;
    else if (highlight_ == 2) f2 = FL_HIGHLIGHT;
    if (pushed_ == 5) f5 = FL_VALUE|FL_SELECTED;
    else if (highlight_ == 5) f5 = FL_HIGHLIGHT;
  }
  if (horizontal()) {
    if (W < 3*H) {Fl_Slider::draw(X,Y,W,H,f5); last_ = highlight_; return; }
    Fl_Slider::draw(X+H,Y,W-2*H,H,f5);
    if (damage()&FL_DAMAGE_ALL || last_ == 1 || highlight_ == 1)
      draw_glyph(FL_GLYPH_LEFT_BUTTON, X, Y, H, H, f1);
    if (damage()&FL_DAMAGE_ALL || last_ == 2 || highlight_ == 2)
      draw_glyph(FL_GLYPH_RIGHT_BUTTON, X+W-H, Y, H, H, f2);
  } else { // vertical
    if (H < 3*W) {Fl_Slider::draw(X,Y,W,H,f5); last_ = highlight_; return; }
    Fl_Slider::draw(X,Y+W,W,H-2*W,f5);
    if (damage()&FL_DAMAGE_ALL || last_ == 1 || highlight_ == 1)
      draw_glyph(FL_GLYPH_UP_BUTTON, X, Y, W, W, f1);
    if (damage()&FL_DAMAGE_ALL || last_ == 2 || highlight_ == 2)
      draw_glyph(FL_GLYPH_DOWN_BUTTON, X, Y+H-W, W, W, f2);
  }
  last_ = highlight_;
}

static void revert(Fl_Style* s) {
  s->selection_color = FL_GRAY;
  s->selection_text_color = FL_BLACK;
  s->text_box = FL_FLAT_BOX;
  s->text_background = FL_DARK2;
}
static Fl_Named_Style style("Scrollbar", revert, &Fl_Scrollbar::default_style);
Fl_Named_Style* Fl_Scrollbar::default_style = &::style;

Fl_Scrollbar::Fl_Scrollbar(int X, int Y, int W, int H, const char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  style(default_style);
  pushed_ = highlight_ = 0;
  step(1);
}

//
// End of "$Id: Fl_Scrollbar.cxx,v 1.54 2001/12/16 22:32:03 spitzak Exp $".
//
