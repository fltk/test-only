//
// "$Id: Fl_Slider.cxx,v 1.54 2002/01/28 08:03:00 spitzak Exp $"
//
// Slider widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Fl_Slider.h>
#include <fltk/fl_draw.h>
#include <math.h>
#include <config.h>

#define vertical() (!(type()&1))

// Draw the background behind the slider, draw() calls this more than
// once with different clipping so the slider does not blink:
void Fl_Slider::draw_bg(int x, int y, int w, int h, Fl_Flags f) {
  fl_color(color());
  fl_rectf(x, y, w, h);
  if (type() == VERTICAL_NICE) {
    FL_THIN_DOWN_BOX->draw(x+w/2-2, y, 4, h, fl_inactive(FL_BLACK, f), f);
  } else if (type() == HORIZONTAL_NICE) {
    FL_THIN_DOWN_BOX->draw(x, y+h/2-2, w, 4, fl_inactive(FL_BLACK, f), f);
  }
  if (focused()) {
    fl_color(FL_BLACK);
    fl_line_style(FL_DOT);
    fl_rect(x+1,y+1,w-2,h-2);
    fl_line_style(0);
  }
}

// Return how big the slider should be if area is of size WxH:
int Fl_Slider::slider_size(int W, int H) {
  int S = slider_size_;
  if (S <= 0) {
    S = H/2+1;
    if (type()==VERTICAL_NICE || type()==HORIZONTAL_NICE) S += 4;
  }
  if (S < W) return S;
  if (minimum() == maximum()) return W;
  return W-1;
}

// Return the location of the edge of the slider if it is S pixels
// wide and moves in an area of size W:
int Fl_Slider::slider_position(int W, int S) {
  if (minimum() == maximum()) return 0;
  double val = (value()-minimum())/(maximum()-minimum());
  if (vertical()) val = 1-val;
  if (val >= 1.0) return W-S;
  else if (val <= 0.0) return 0;
  else return int(val*(W-S)+.5);
}

// Fltk 2.0 is incompatable with the use of color:
// color area:                          1.0:                    2.0:
// background behind slider             color                   color
// normal slider                        selection_color         selection_color
// normal slider symbol                 N/A                     text_color
// nice slider slider                   FL_GRAY                 button_color
// nice slider color mark               selection_color         selection_color

// Draw a normal rectangular slider in the passed region:
void Fl_Slider::draw(int x, int y, int w, int h, Fl_Flags f) {

  if (w <= 0 || h <= 0) return;
  int W = (vertical() ? h : w);
  int X,S;
  if (type()==HORIZONTAL_FILL || type() == VERTICAL_FILL) {
    S = slider_position(W, 0);
    if (!(minimum()<=maximum()) == !vertical()) {S = W-S; X = W-S;}
    else X = 0;
  } else {
    S = slider_size(W, vertical() ? w : h);
    X = slider_position(W, S);
  }

  if (X > 0) {
    if (vertical()) draw_bg(x, y, w, X, f);
    else draw_bg(x, y, X, h, f);
  }
  if (X+S < W) {
    if (vertical()) draw_bg(x, y+X+S, w, h-X-S, f);
    else draw_bg(x+X+S, y, w-X-S, h, f);
  }

  // if user directly set selected_color we use it:
  if (style()->selection_color && type()<VERTICAL_NICE) f |= FL_SELECTED;

  if (vertical()) {
    draw_glyph(0, x, y+X, w, S, f);
    draw_inside_label(x, y+X, w, S, f);
    if (type()==VERTICAL_NICE) {
      int y1 = (S-4)/2;
      FL_THIN_DOWN_BOX->draw(x+1, y+X+y1, w-2, S-2*y1, selection_color());
    }
  } else {
    draw_glyph(0, x+X, y, S, h, f);
    draw_inside_label(x+X, y, S, h, f);
    if (type()==HORIZONTAL_NICE) {
      int x1 = (S-4)/2;
      FL_THIN_DOWN_BOX->draw(x+X+x1, y+1, S-2*x1, h-2, selection_color());
    }
  }
}

void Fl_Slider::draw() {
  if (damage()&FL_DAMAGE_ALL) draw_frame();
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  Fl_Flags f = 0;
  if (!active_r()) {
    f |= FL_INACTIVE;
  } else {
    //if (Fl::pushed() == this) f |= FL_VALUE;
    if (belowmouse()) f |= FL_HIGHLIGHT;
  }
  draw(X,Y,W,H, f);
}

int Fl_Slider::handle(int event, int x, int y, int w, int h) {
  switch (event) {
  case FL_PUSH:
    handle_push();
    redraw(FL_DAMAGE_VALUE);
  case FL_DRAG: {
    int H = (vertical() ? w : h);
    int W = (vertical() ? h : w);
    int mx = (vertical() ? Fl::event_y()-y : Fl::event_x()-x);
    int S = slider_size(W,H);
    if (W <= S) return 1;
    int X;
    static int offcenter;
    if (type() == HORIZONTAL_FILL || type() == VERTICAL_FILL) {
      X = slider_position(W, 0);
      if (event == FL_PUSH) {
	offcenter = mx-X;
	if (offcenter < -S/2) offcenter = 0;
	else if (offcenter > S/2) offcenter = 0;
	else return 1;
      }
      S = 0;
    } else {
      X = slider_position(W, S);
      if (event == FL_PUSH) {
	offcenter = mx-X;
	if (offcenter < 0) offcenter = 0;
	else if (offcenter > S) offcenter = S;
	else return 1;
      }
    }
    X = mx-offcenter;
    double v;
  RETRY:
    if (X < 0) {
      X = 0;
      offcenter = mx; if (offcenter < 0) offcenter = 0;
    } else if (X > (W-S)) {
      X = W-S;
      offcenter = mx-X; if (offcenter > S) offcenter = S;
    }
    if (vertical()) X = (W-S)-X;
    v = round(X*(maximum()-minimum())/(W-S) + minimum());
    // make sure a click outside the sliderbar moves it:
    if (event == FL_PUSH && v == value()) {
      offcenter = S/2;
      event = FL_DRAG;
      goto RETRY;
    }
    handle_drag(clamp(v));
    } return 1;
  case FL_RELEASE:
    redraw(FL_DAMAGE_VALUE);
    handle_release();
    return 1;
  case FL_KEY:
    // Only arrows in the correct direction are used.  This allows the
    // opposite arrows to be used to navigate between a set of parellel
    // sliders.
    switch (Fl::event_key()) {
    case FL_Up:
    case FL_Down:
      if (!vertical()) return 0;
      break;
    case FL_Left:
    case FL_Right:
      if (vertical()) return 0;
    }
  default:
    return Fl_Valuator::handle(event);
  }
}

int Fl_Slider::handle(int event) {
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
#if CLICK_MOVES_FOCUS
  if (event == FL_PUSH) take_focus();
#endif
  return handle(event,X,Y,W,H);
}

/*
// The only difference in the glyph is that we turn of FL_VALUE for the
// central box, so it does not draw pushed in:
static void glyph(const Fl_Widget* widget, int glyph,
		  int x,int y,int w,int h, Fl_Flags flags)
{
  if (!glyph) flags &= ~FL_VALUE;
  Fl_Widget::default_glyph(widget, glyph, x, y, w, h, flags);
}
*/

static void revert(Fl_Style *s) {
  s->color = FL_DARK2;
  //  s->glyph = ::glyph;
}
static Fl_Named_Style style("Slider", revert, &Fl_Slider::default_style);
Fl_Named_Style* Fl_Slider::default_style = &::style;

Fl_Slider::Fl_Slider(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l) {
  style(default_style);
  slider_size_ = 0;
}

Fl_Slider::Fl_Slider(uchar t, int x, int y, int w, int h, const char* l)
  : Fl_Valuator(x, y, w, h, l) {
  style(default_style);
  slider_size_ = 0;
  type(t);
// fltk 1.0 (and XForms) set the box to FL_FLAT_BOX if the type was
// HORIZONTAL_NICE or VERTICAL_NICE
}

//
// End of "$Id: Fl_Slider.cxx,v 1.54 2002/01/28 08:03:00 spitzak Exp $".
//
