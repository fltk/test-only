//
// "$Id: Fl_Scrollbar.cxx,v 1.27 1999/11/10 12:21:53 bill Exp $"
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
  pagesize_ = w-1; if (pagesize_ <= 0) pagesize_ = 1;
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
  // area of scrollbar:
  int SX = x()+box()->dx(); int X = SX;
  int SY = y()+box()->dy(); int Y = SY;
  int SW = w()-box()->dw(); int W = SW;
  int SH = h()-box()->dh(); int H = SH;

  // adjust slider area to be inside the arrow buttons:
  if (horizontal()) {
    if (W >= 3*H) {X += H; W -= 2*H;}
  } else {
    if (H >= 3*W) {Y += W; H -= 2*W;}
  }

  // which widget part is highlighted?
  int mx = Fl::event_x();
  int my = Fl::event_y();
  if (Fl::pushed() == this) ; // don't change highlight
  else if (!Fl::event_inside(SX, SY, SW, SH)) highlight_ = 0;
  else if (horizontal()) {
    if (mx < X) highlight_ = 1;
    else if (mx >= X+W) highlight_ = 2;
    else {
      // Carl: I did this on purpose so the highlighting will work
      // when you point at the trough!  I don't understand why you
      // don't want the highlighting in this case

      // Because commenting this code out breaks the page up/down
      // functionality of clicking on the trough.  Besides, no other
      // toolkit highlights when pointing at the trough so it doesn't
      // look "right" and makes emulation themes less correct.
      int sliderx = slider_position(W, slider_size(W, H));
      if (mx < X+sliderx) highlight_ = 3;
      else if (mx >= X+sliderx+slider_size(W, H)) highlight_ = 4;
      else highlight_ = 5;
    }
  } else {
    if (mx < X || mx >= X+W) highlight_ = 0;
    else if (my < Y) highlight_ = 1;
    else if (my >= Y+H) highlight_ = 2;
    else {
      int slidery = slider_position(H, slider_size(H, W));
      if (my < Y+slidery) highlight_ = 3;
      else if (my >= Y+slidery+slider_size(H, W)) highlight_ = 4;
      else highlight_ = 5;
    }
  }
  switch (event) {
  case FL_MOVE:
    if (last_ == highlight_) return 1;
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents())
      damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_RELEASE:
      damage(FL_DAMAGE_EXPOSE);
    if (pushed_) {
      Fl::remove_timeout(timeout_cb, this);
      pushed_ = 0;
    }
    handle_release();
    return 1;
  case FL_PUSH:
    if (pushed_) return 1;
    if (highlight_ != 5) pushed_ = highlight_;
    if (pushed_) {
      handle_push();
      Fl::add_timeout(INITIALREPEAT, timeout_cb, this);
      increment_cb();
      damage(FL_DAMAGE_EXPOSE);
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

  Fl_Flags f1 = 0, f2 = 0;
  if (!active_r()) {
    f1 = f2 = FL_INACTIVE;
  } else {
    if (pushed_ == 1)
      f1 |= FL_VALUE;
    else if (highlight_ == 1)
      f1 |= FL_HIGHLIGHT;
    if (pushed_ == 2)
      f2 |= FL_VALUE;
    else if (highlight_ == 2)
      f2 |= FL_HIGHLIGHT;
  }

  Fl_Flags f;
  if (!active_r()) f = FL_INACTIVE;
  else if (highlight_ == 5) f = FL_HIGHLIGHT;
  else f = 0;

  if (horizontal()) {
    if (W < 3*H) {Fl_Slider::draw(X,Y,W,H,f); last_ = highlight_; return; }
    Fl_Slider::draw(X+H,Y,W-2*H,H,f);
    if (damage()&FL_DAMAGE_ALL || last_ == 1 || highlight_ == 1)
      draw_glyph(FL_GLYPH_LEFT, X, Y, H, H, f1);
    if (damage()&FL_DAMAGE_ALL || last_ == 2 || highlight_ == 2)
      draw_glyph(FL_GLYPH_RIGHT, X+W-H, Y, H, H, f2);
  } else { // vertical
    if (H < 3*W) {Fl_Slider::draw(X,Y,W,H,f); last_ = highlight_; return; }
    Fl_Slider::draw(X,Y+W,W,H-2*W,f);
    if (damage()&FL_DAMAGE_ALL || last_ == 1 || highlight_ == 1)
      draw_glyph(FL_GLYPH_UP, X, Y, W, W, f1);
    if (damage()&FL_DAMAGE_ALL || last_ == 2 || highlight_ == 2)
      draw_glyph(FL_GLYPH_DOWN, X, Y+H-W, W, W, f2);
  }
  last_ = highlight_;
}

static void revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX;
  s->color = FL_DARK2;
}

Fl_Named_Style Fl_Scrollbar::default_style("Scrollbar", revert);

Fl_Scrollbar::Fl_Scrollbar(int X, int Y, int W, int H, const char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  style(default_style);
  linesize_ = 16;
  pagesize_ = 100;
  pushed_ = highlight_ = 0;
  step(1);
}

//
// End of "$Id: Fl_Scrollbar.cxx,v 1.27 1999/11/10 12:21:53 bill Exp $".
//
