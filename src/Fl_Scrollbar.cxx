//
// "$Id: Fl_Scrollbar.cxx,v 1.64 2002/12/10 01:46:17 easysw Exp $"
//
// Scroll bar widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/Scrollbar.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <math.h>
#include <stdlib.h>
using namespace fltk;

#define vertical() (!(type()&1))

int Scrollbar::value(int p, int w, int t, int l) {
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
  return Slider::value(p);
}

enum {NOTHING = 0, UP_ARROW, DOWN_ARROW, ABOVE_SLIDER, BELOW_SLIDER, SLIDER};

// For the belowmouse() widget, which button is highlighted:
static char which_highlight = 0;
// For the pushed() widget, which button is pushed:
static char which_pushed = 0;
// How belowmouse() widget was last drawn:
static char last_;

void Scrollbar::increment_cb() {
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

int Scrollbar::handle(int event) {
  // area of scrollbar:
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);

  // adjust slider area to be inside the arrow buttons:
  if (vertical()) {
    if (H >= 3*W) {Y += W; H -= 2*W;}
  } else {
    if (W >= 3*H) {X += H; W -= 2*H;}
  }

  // which widget part is highlighted?
  int mx = event_x();
  int my = event_y();
  int which_part;
  if (!event_inside(0, 0, w(), h())) which_part = NOTHING;
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
  case FOCUS:
    return 0;
  case ENTER:
  case MOVE:
    if (!highlight_color()) return 1;
    if (which_part != which_highlight) {
      which_highlight = which_part;
      redraw(DAMAGE_HIGHLIGHT);
    }
    return 1;
  case LEAVE:
    if (which_highlight) {
      which_highlight = 0;
      redraw(DAMAGE_HIGHLIGHT);
    }
    return 1;
  case PUSH:
    // Clicking on the slider or middle or right click on the trough
    // gives us normal slider behavior:
    if (which_part == SLIDER ||
	event_button() > 1 && which_part > DOWN_ARROW) {
      which_pushed = SLIDER;
      return Slider::handle(event, X,Y,W,H);
    }
    handle_push();
    // middle/right click on arrows jumps to that end:
    if (event_button()>1) {
      if (which_part==UP_ARROW) handle_drag(vertical()?maximum():minimum());
      else if (which_part==DOWN_ARROW) handle_drag(vertical()?minimum():maximum());
    }
    goto J1;
  case DRAG:
    if (which_pushed==SLIDER) return Slider::handle(event, X,Y,W,H);
    if (which_part == SLIDER) which_part = NOTHING;
    // it is okay to switch between arrows and nothing, but no other
    // changes are allowed:
    if (!which_pushed && which_part <= DOWN_ARROW) ;
    else if (!which_part && which_pushed <= DOWN_ARROW) ;
    else which_part = which_pushed;
  J1:
    if (which_part != which_pushed) {
      remove_timeout();
      which_highlight = which_pushed = which_part;
      redraw(DAMAGE_HIGHLIGHT);
      if (which_part) {
	add_timeout(INITIALREPEAT);
	increment_cb();
      }
    }
    return 1;
  case TIMEOUT:
    if (which_pushed) {
      repeat_timeout(REPEAT);
      increment_cb();
    }
    return 1;
  case RELEASE:
    if (which_pushed == SLIDER) {
      Slider::handle(event, X,Y,W,H);
    } else if (which_pushed) {
      remove_timeout();
      handle_release();
      redraw(DAMAGE_HIGHLIGHT);
    }
    which_pushed = NOTHING;
    return 1;
  case MOUSEWHEEL: {
    float n = (vertical() ? -event_dy() : event_dx())
      * Style::wheel_scroll_lines * linesize();
    if (fabsf(n) > pagesize()) n = (n<0)?-pagesize():pagesize();
    handle_drag(value()+n);
    return 1;
  }
  case KEY:
    if (vertical()) switch(event_key()) {
    case HomeKey: handle_drag(maximum()); return 1;
    case EndKey:  handle_drag(minimum()); return 1;
    case PageUpKey: handle_drag(value()-pagesize()); return 1;
    case PageDownKey: handle_drag(value()+pagesize()); return 1;
    } // else fall through...
  default:
    return Slider::handle(event);
  }
}

void Scrollbar::draw() {
  if (damage()&DAMAGE_ALL) draw_frame();

  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  int ix = X; int iy = Y; int iw = W; int ih = H;

  char pushed_ = pushed() ? which_pushed : NOTHING;
  char highlight_ = belowmouse() ? which_highlight : NOTHING;

  // 1 = left/top   2 = right/bottom   5 = slider button
  Flags f1 = 0, f2 = 0, f5 = 0;
  if (!active_r()) {
    f1 = f2 = f5 = INACTIVE;
  } else {
    if (pushed_ == UP_ARROW) f1 = VALUE|HIGHLIGHT;
    else if (highlight_ == UP_ARROW) f1 = HIGHLIGHT;
    if (pushed_ == DOWN_ARROW) f2 = VALUE|HIGHLIGHT;
    else if (highlight_ == DOWN_ARROW) f2 = HIGHLIGHT;
    if (pushed_ == SLIDER) f5 = VALUE|HIGHLIGHT;
    else if (highlight_ == SLIDER) f5 = HIGHLIGHT;
  }

  if (vertical() && H >= 3*W) {
    if (damage()&DAMAGE_ALL || last_ == UP_ARROW || highlight_ == UP_ARROW)
      draw_glyph(GLYPH_UP_BUTTON, X, Y, W, W, f1);
    if (damage()&DAMAGE_ALL || last_ ==DOWN_ARROW|| highlight_ ==DOWN_ARROW)
      draw_glyph(GLYPH_DOWN_BUTTON, X, Y+H-W, W, W, f2);
    iy += W; ih -= 2*W;

  } else if (W >= 3*H) { // horizontal:
    if (damage()&DAMAGE_ALL || last_ == UP_ARROW || highlight_ == UP_ARROW)
      draw_glyph(GLYPH_LEFT_BUTTON, X, Y, H, H, f1);
    if (damage()&DAMAGE_ALL || last_ ==DOWN_ARROW|| highlight_ ==DOWN_ARROW)
      draw_glyph(GLYPH_RIGHT_BUTTON, X+W-H, Y, H, H, f2);
    ix += H; iw -= 2*H;
  }
  last_ = highlight_;

  if (Slider::draw(ix, iy, iw, ih, f5, false)) {
    setcolor(color());
    fillrect(ix, iy, iw, ih);
    pop_clip();
  }

}

static void glyph(const Widget* widget, int glyph,
		  int x,int y,int w,int h, Flags flags)
{
  if (!glyph) flags &= ~VALUE;
  Widget::default_glyph(widget, glyph, x, y, w, h, flags);
}

static void revert(Style* s) {
  //s->highlight_color = WHITE;
  s->box = FLAT_BOX;
  s->color = GRAY60;
  s->glyph = ::glyph;
}
static NamedStyle style("Scrollbar", revert, &Scrollbar::default_style);
NamedStyle* Scrollbar::default_style = &::style;

Scrollbar::Scrollbar(int X, int Y, int W, int H, const char* L)
  : Slider(X, Y, W, H, L)
{
  style(default_style);
  step(1);
  pagesize_ = 1;
}

//
// End of "$Id: Fl_Scrollbar.cxx,v 1.64 2002/12/10 01:46:17 easysw Exp $".
//
