//
// "$Id: Fl_Scrollbar.cxx,v 1.62 2002/10/04 07:48:14 spitzak Exp $"
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

#define vertical() (!(type()&1))

int Fl_Scrollbar::value(int p, int w, int t, int l) {
//	p = position, first line displayed
//	w = window, number of lines displayed
//	t = top, number of first line
//	l = length, total number of lines
  if (p+w > t+l) l = p+w-t;
  if (l <= 0) l = 1;
  int b = l-w+t;
  int X=0; int Y=0; int W=this->w(); int H=h(); box()->inset(X,Y,W,H);
  if (vertical()) {int T = W; W = H; H = T; T = b; b = t; t = T;}
  if (W >= 3*H) W -= 2*H;
  int S = W*w/l; if (S < H) S = H; if (S > W) S = W;
  if (S != slider_size() || t != minimum() || b != maximum()) {
    slider_size(S); minimum(t); maximum(b); redraw();
  }
  int ls = int(linesize());
  pagesize(w>2*ls ? w-ls : ls);
  return Fl_Slider::value(p);
}

enum {NOTHING = 0, UP_ARROW, DOWN_ARROW, ABOVE_SLIDER, BELOW_SLIDER, SLIDER};

// For the belowmouse() widget, which button is highlighted:
static char which_highlight = 0;
// For the pushed() widget, which button is pushed:
static char which_pushed = 0;
// How belowmouse() widget was last drawn:
static char last_;

void Fl_Scrollbar::increment_cb() {
  double i;
  switch (which_pushed) {
  case UP_ARROW: i = -linesize(); break;
  default:i =  linesize(); break;
  case ABOVE_SLIDER: i = -pagesize(); break;
  case BELOW_SLIDER: i =  pagesize(); break;
  }
  handle_drag(value()+i);
}

#define INITIALREPEAT .5f
#define REPEAT .05f

void Fl_Scrollbar::timeout_cb(void* v) {
  Fl_Scrollbar* s = (Fl_Scrollbar*)v;
  if (which_pushed) {
    Fl::add_timeout(REPEAT, timeout_cb, s);
    s->increment_cb();
  }
}

int Fl_Scrollbar::handle(int event) {
  // area of scrollbar:
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);

  // adjust slider area to be inside the arrow buttons:
  if (vertical()) {
    if (H >= 3*W) {Y += W; H -= 2*W;}
  } else {
    if (W >= 3*H) {X += H; W -= 2*H;}
  }

  // which widget part is highlighted?
  int mx = Fl::event_x();
  int my = Fl::event_y();
  int which_part;
  if (!Fl::event_inside(0, 0, w(), h())) which_part = NOTHING;
  else if (vertical()) {
    if (my < Y) which_part = UP_ARROW;
    else if (my >= Y+H) which_part = DOWN_ARROW;
    else {
      int slidery = slider_position(value(), H);
      if (my < Y+slidery) which_part = ABOVE_SLIDER;
      else if (my >= Y+slidery+slider_size()) which_part = BELOW_SLIDER;
      else which_part = SLIDER;
    }
  } else { // horizontal
    if (mx < X) which_part = UP_ARROW;
    else if (mx >= X+W) which_part = DOWN_ARROW;
    else {
      int sliderx = slider_position(value(), W);
      if (mx < X+sliderx) which_part = ABOVE_SLIDER;
      else if (mx >= X+sliderx+slider_size()) which_part = BELOW_SLIDER;
      else which_part = SLIDER;
    }
  }
  switch (event) {
  case FL_FOCUS:
    return 0;
  case FL_ENTER:
  case FL_MOVE:
    if (!highlight_color()) return 1;
    if (which_part != which_highlight) {
      which_highlight = which_part;
      redraw(FL_DAMAGE_HIGHLIGHT);
    }
    return 1;
  case FL_LEAVE:
    if (which_highlight) {
      which_highlight = 0;
      redraw(FL_DAMAGE_HIGHLIGHT);
    }
    return 1;
  case FL_PUSH:
    // Clicking on the slider or middle or right click on the trough
    // gives us normal slider behavior:
    if (which_part == SLIDER ||
	Fl::event_button() > 1 && which_part > DOWN_ARROW) {
      which_pushed = SLIDER;
      return Fl_Slider::handle(event, X,Y,W,H);
    }
    handle_push();
    // middle/right click on arrows jumps to that end:
    if (Fl::event_button()>1) {
      if (which_part==UP_ARROW) handle_drag(vertical()?maximum():minimum());
      else if (which_part==DOWN_ARROW) handle_drag(vertical()?minimum():maximum());
    }
    goto J1;
  case FL_DRAG:
    if (which_pushed==SLIDER) return Fl_Slider::handle(event, X,Y,W,H);
    if (which_part == SLIDER) which_part = NOTHING;
    // it is okay to switch between arrows and nothing, but no other
    // changes are allowed:
    if (!which_pushed && which_part <= DOWN_ARROW) ;
    else if (!which_part && which_pushed <= DOWN_ARROW) ;
    else which_part = which_pushed;
  J1:
    if (which_part != which_pushed) {
      Fl::remove_timeout(timeout_cb, this);
      which_highlight = which_pushed = which_part;
      redraw(FL_DAMAGE_HIGHLIGHT);
      if (which_part) {
	Fl::add_timeout(INITIALREPEAT, timeout_cb, this);
	increment_cb();
      }
    }
    return 1;
  case FL_RELEASE:
    if (which_pushed == SLIDER) {
      Fl_Slider::handle(event, X,Y,W,H);
    } else if (which_pushed) {
      Fl::remove_timeout(timeout_cb, this);
      handle_release();
      redraw(FL_DAMAGE_HIGHLIGHT);
    }
    which_pushed = NOTHING;
    return 1;
  case FL_MOUSEWHEEL: {
    float n = (vertical() ? -Fl::event_dy() : Fl::event_dx())
      * Fl_Style::wheel_scroll_lines * linesize();
    if (fabsf(n) > pagesize()) n = (n<0)?-pagesize():pagesize();
    handle_drag(value()+n);
    return 1;
  }
  case FL_KEY:
    if (vertical()) switch(Fl::event_key()) {
    case FL_Home: handle_drag(maximum()); return 1;
    case FL_End:  handle_drag(minimum()); return 1;
    case FL_Page_Up: handle_drag(value()-pagesize()); return 1;
    case FL_Page_Down: handle_drag(value()+pagesize()); return 1;
    } // else fall through...
  default:
    return Fl_Slider::handle(event);
  }
}

void Fl_Scrollbar::draw() {
  if (damage()&FL_DAMAGE_ALL) draw_frame();

  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  int ix = X; int iy = Y; int iw = W; int ih = H;

  char pushed_ = this==Fl::pushed() ? which_pushed : NOTHING;
  char highlight_ = this==Fl::belowmouse() ? which_highlight : NOTHING;

  // 1 = left/top   2 = right/bottom   5 = slider button
  Fl_Flags f1 = 0, f2 = 0, f5 = 0;
  if (!active_r()) {
    f1 = f2 = f5 = FL_INACTIVE;
  } else {
    if (pushed_ == UP_ARROW) f1 = FL_VALUE|FL_HIGHLIGHT;
    else if (highlight_ == UP_ARROW) f1 = FL_HIGHLIGHT;
    if (pushed_ == DOWN_ARROW) f2 = FL_VALUE|FL_HIGHLIGHT;
    else if (highlight_ == DOWN_ARROW) f2 = FL_HIGHLIGHT;
    if (pushed_ == SLIDER) f5 = FL_VALUE|FL_HIGHLIGHT;
    else if (highlight_ == SLIDER) f5 = FL_HIGHLIGHT;
  }

  if (vertical() && H >= 3*W) {
    if (damage()&FL_DAMAGE_ALL || last_ == UP_ARROW || highlight_ == UP_ARROW)
      draw_glyph(FL_GLYPH_UP_BUTTON, X, Y, W, W, f1);
    if (damage()&FL_DAMAGE_ALL || last_ ==DOWN_ARROW|| highlight_ ==DOWN_ARROW)
      draw_glyph(FL_GLYPH_DOWN_BUTTON, X, Y+H-W, W, W, f2);
    iy += W; ih -= 2*W;

  } else if (W >= 3*H) { // horizontal:
    if (damage()&FL_DAMAGE_ALL || last_ == UP_ARROW || highlight_ == UP_ARROW)
      draw_glyph(FL_GLYPH_LEFT_BUTTON, X, Y, H, H, f1);
    if (damage()&FL_DAMAGE_ALL || last_ ==DOWN_ARROW|| highlight_ ==DOWN_ARROW)
      draw_glyph(FL_GLYPH_RIGHT_BUTTON, X+W-H, Y, H, H, f2);
    ix += H; iw -= 2*H;
  }
  last_ = highlight_;

  if (Fl_Slider::draw(ix, iy, iw, ih, f5, false)) {
    fl_color(color());
    fl_rectf(ix, iy, iw, ih);
    fl_pop_clip();
  }

}

static void glyph(const Fl_Widget* widget, int glyph,
		  int x,int y,int w,int h, Fl_Flags flags)
{
  if (!glyph) flags &= ~FL_VALUE;
  Fl_Widget::default_glyph(widget, glyph, x, y, w, h, flags);
}

static void revert(Fl_Style* s) {
  //s->highlight_color = FL_WHITE;
  s->box = FL_FLAT_BOX;
  s->color = FL_DARK2;
  s->glyph = ::glyph;
}
static Fl_Named_Style style("Scrollbar", revert, &Fl_Scrollbar::default_style);
Fl_Named_Style* Fl_Scrollbar::default_style = &::style;

Fl_Scrollbar::Fl_Scrollbar(int X, int Y, int W, int H, const char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  style(default_style);
  step(1);
  pagesize_ = 1;
}

//
// End of "$Id: Fl_Scrollbar.cxx,v 1.62 2002/10/04 07:48:14 spitzak Exp $".
//
