//
// "$Id: Fl_Scrollbar.cxx,v 1.18 1999/10/04 09:12:48 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/fl_draw.H>
#include <math.h>

bool Fl_Scrollbar::value(int p, int w, int t, int l) {
//	p = position, first line displayed
//	w = window, number of lines displayed
//	t = top, number of first line
//	l = length, total number of lines
  if (p+w > t+l) l = p+w-t;
  int b = l-w+t;
  int W = this->w()-box()->dw();
  int H = h()-box()->dh();
  if (!horizontal()) {int T = W; W = H; H = T;}
  if (W >= 3*H) W -= 2*H;
  int S = W*w/l; if (S < H) S = H; if (S > W) S = W;
  if (S != slider_size() || t != minimum() || b != maximum()) {
    slider_size(S); minimum(t); maximum(b); redraw();
  }
  return Fl_Slider::value(p);
}

#define INITIALREPEAT .5
#define REPEAT .05

void Fl_Scrollbar::increment_cb() {
  int i;
  switch (pushed_) {
  case 1: i = -linesize_; break;
  default:i =  linesize_; break;
  case 3: i = -pagesize_; break;
  case 4: i =  pagesize_; break;
  }
  if (maximum() < minimum()) i = -i;
  handle_drag(clamp(value() + i));
}

void Fl_Scrollbar::timeout_cb(void* v) {
  Fl_Scrollbar* s = (Fl_Scrollbar*)v;
  s->increment_cb();
  Fl::add_timeout(REPEAT, timeout_cb, s);
}

int Fl_Scrollbar::handle(int event) {
  // area of slider:
  int X = x()+box()->dx();
  int Y = y()+box()->dy();
  int W = w()-box()->dw();
  int H = h()-box()->dh();

  // adjust slider area to be inside the arrow buttons:
  if (horizontal()) {
    if (W >= 3*H) {X += H; W -= 2*H;}
  } else {
    if (H >= 3*W) {Y += W; H -= 2*W;}
  }

  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && active_r()) redraw();
    return 1;
  case FL_RELEASE:
    if (pushed_) {
      Fl::remove_timeout(timeout_cb, this);
      pushed_ = 0;
      redraw();
    }
    handle_release();
    return 1;
  case FL_PUSH:
    if (pushed_) return 1;
    if (horizontal()) {
      int mx = Fl::event_x();
      if (mx < X) pushed_ = 1;
      else if (mx >= X+W) pushed_ = 2;
      else {
	int sliderx = slider_position(W, slider_size());
	if (mx < X+sliderx) pushed_ = 3;
	else if (mx >= X+sliderx+slider_size()) pushed_ = 4;
      }
    } else {
      int my = Fl::event_y();
      if (my < Y) pushed_ = 1;
      else if (my >= Y+H) pushed_ = 2;
      else {
	int slidery = slider_position(H, slider_size());
	if (my < Y+slidery) pushed_ = 3;
	else if (my >= Y+slidery+slider_size()) pushed_ = 4;
      }
    }
    if (pushed_) {
      handle_push();
      Fl::add_timeout(INITIALREPEAT, timeout_cb, this);
      increment_cb();
      redraw();
      return 1;
    }
    return Fl_Slider::handle(event, X,Y,W,H);
  case FL_DRAG:
    if (pushed_) return 1;
    return Fl_Slider::handle(event, X,Y,W,H);
  case FL_SHORTCUT: {
    int v = value();
    int ls = maximum()>=minimum() ? linesize_ : -linesize_;
    if (horizontal()) {
      switch (Fl::event_key()) {
      case FL_Left:
	v -= ls;
	break;
      case FL_Right:
	v += ls;
	break;
      default:
	return 0;
      }
    } else { // vertical
      switch (Fl::event_key()) {
      case FL_Up:
	v -= ls;
	break;
      case FL_Down:
	v += ls;
	break;
      case FL_Page_Up:
	if (slider_size() >= 1.0) return 0;
	v -= int((maximum()-minimum())*slider_size()/(1.0-slider_size()));
	v += ls;
	break;
      case FL_Page_Down:
	if (slider_size() >= 1.0) return 0;
	v += int((maximum()-minimum())*slider_size()/(1.0-slider_size()));
	v -= ls;
	break;
      case FL_Home:
	v = int(minimum());
	break;
      case FL_End:
	v = int(maximum());
	break;
      default:
	return 0;
      }
    }
    v = int(clamp(v));
    if (v != value()) {
      Fl_Slider::value(v);
      value_damage();
      do_callback();
    }
    return 1;}
  }
  return 0;
}

void Fl_Scrollbar::draw() {
  if (damage()&FL_DAMAGE_ALL) draw_box();
  int X = x()+box()->dx();
  int Y = y()+box()->dy();
  int W = w()-box()->dw();
  int H = h()-box()->dh();
  Fl_Color c = color2(); // not color()!  Different from all other widgets...
  Fl_Flags f = 0;
  if (!active_r()) {
    f = FL_INACTIVE;
  } else if (Fl::belowmouse() == this && highlight_color()) {
    f = FL_HIGHLIGHT;
    c = highlight_color();
  }
  if (horizontal()) {
    if (W < 3*H) {Fl_Slider::draw(X,Y,W,H); return;}
    Fl_Slider::draw(X+H,Y,W-2*H,H);
    if (damage()&FL_DAMAGE_ALL) {
      glyph()(FL_GLYPH_LEFT, X, Y, H, H, c, f|((pushed_==1)?FL_VALUE:0));
      glyph()(FL_GLYPH_RIGHT, X+W-H, Y, H, H, c, f|((pushed_==2)?FL_VALUE:0));
    }
  } else { // vertical
    if (H < 3*W) {Fl_Slider::draw(X,Y,W,H); return;}
    Fl_Slider::draw(X,Y+W,W,H-2*W);
    if (damage()&FL_DAMAGE_ALL) {
      glyph()(FL_GLYPH_UP, X, Y, W, W, c, f|((pushed_==1)?FL_VALUE :0));
      glyph()(FL_GLYPH_DOWN, X, Y+H-W, W, W, c, f|((pushed_==2)?FL_VALUE:0));
    }
  }
}

Fl_Style Fl_Scrollbar::default_style = {
  FL_FLAT_BOX,	// box
  0,		// glyphs
  0,		// label_font
  0,		// text_font
  0,		// label_type
  FL_DARK2,	// color
  0,		// label_color
  FL_GRAY	// color2
  // rest is zero
};

Fl_Style_Definer fl_scrollbar_style_definer("scrollbar", Fl_Scrollbar::default_style);

Fl_Scrollbar::Fl_Scrollbar(int X, int Y, int W, int H, const char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  style(default_style);
  linesize_ = 16;
  pagesize_ = 100;
  pushed_ = 0;
  step(1);
}

//
// End of "$Id: Fl_Scrollbar.cxx,v 1.18 1999/10/04 09:12:48 bill Exp $".
//
