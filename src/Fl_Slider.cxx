//
// "$Id: Fl_Slider.cxx,v 1.28 1999/11/10 18:06:06 carl Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_draw.H>
#include <math.h>

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
// FL_HOR/VERT_NICE_SLIDER.
}

// Draw the background behind the slider, draw() calls this more than
// once with different clipping so the slider does not blink:
void Fl_Slider::draw_bg(int x, int y, int w, int h) {
  fl_color(color());
  fl_rectf(x, y, w, h);
  if (type() == FL_VERT_NICE_SLIDER) {
    FL_THIN_DOWN_BOX->draw(x+w/2-2, y, 4, h, FL_BLACK, 0);
  } else if (type() == FL_HOR_NICE_SLIDER) {
    FL_THIN_DOWN_BOX->draw(x, y+h/2-2, w, 4, FL_BLACK, 0);
  }
}

// Return how big the slider should be if area is of size WxH:
int Fl_Slider::slider_size(int W, int H) {
  int S = slider_size_;
  if (S <= 0) {
    S = H/2+1;
    if (type()==FL_VERT_NICE_SLIDER || type()==FL_HOR_NICE_SLIDER) S += 4;
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
  if (val >= 1.0) return W-S;
  else if (val <= 0.0) return 0;
  else return int(val*(W-S)+.5);
}

// Draw a normal rectangular slider in the passed region:
void Fl_Slider::draw(int x, int y, int w, int h, Fl_Flags f) {
  if (w <= 0 || h <= 0) return;
  int W = (horizontal() ? w : h);
  int X,S;
  if (type()==FL_HOR_FILL_SLIDER || type() == FL_VERT_FILL_SLIDER) {
    S = slider_position(W, 0);
    if (minimum()>maximum()) {S = W-S; X = W-S;}
    else X = 0;
  } else {
    S = slider_size(W, horizontal() ? h : w);
    X = slider_position(W, S);
  }

  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) {
    if (X > 0) {
      if (horizontal()) draw_bg(x, y, X, h);
      else draw_bg(x, y, w, X);
    }
    if (X+S < W) {
      if (horizontal()) draw_bg(x+X+S, y, w-X-S, h);
        else draw_bg(x, y+X+S, w, h-X-S);
    }
  }

  if (type() == FL_VERT_NICE_SLIDER)
    draw_glyph(FL_GLYPH_VNSLIDER, x, y+X, w, S, f);
  else if (type() == FL_HOR_NICE_SLIDER)
    draw_glyph(FL_GLYPH_HNSLIDER, x+X, y, S, h, f);
  else if (horizontal()) {
    draw_glyph(type() == FL_HOR_FILL_SLIDER ? 0 : FL_GLYPH_HSLIDER,
	       x+X, y, S, h, f);
    draw_button_label(x+X, y, S, h, label_color());
  } else {
    draw_glyph(type() == FL_VERT_FILL_SLIDER ? 0 : FL_GLYPH_VSLIDER,
	       x, y+X, w, S, f);
    draw_button_label(x, y+X, w, S, label_color());
  }
}

void Fl_Slider::draw() {
  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) draw_frame();
  Fl_Flags f;
  if (!active_r()) f = FL_INACTIVE;
  else if (Fl::belowmouse() == this) f = FL_HIGHLIGHT;
  else f = 0;
  draw(x()+box()->dx(),
       y()+box()->dy(),
       w()-box()->dw(),
       h()-box()->dh(),
       f);
  if (damage()&FL_DAMAGE_ALL) draw_label();
}

int Fl_Slider::handle(int event, int x, int y, int w, int h) {
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_PUSH:
    if (!Fl::event_inside(x, y, w, h)) return 0;
    damage(FL_DAMAGE_EXPOSE);
    handle_push();
  case FL_DRAG: {
    int W = (horizontal() ? w : h);
    int H = (horizontal() ? h : w);
    int mx = (horizontal() ? Fl::event_x()-x : Fl::event_y()-y);
    int S = slider_size(W,H);
    int X;
    static int offcenter;
    if (type() == FL_HOR_FILL_SLIDER || type() == FL_VERT_FILL_SLIDER) {
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
  TRY_AGAIN:
    if (X < 0) {
      X = 0;
      offcenter = mx; if (offcenter < 0) offcenter = 0;
    } else if (X > (W-S)) {
      X = W-S;
      offcenter = mx-X; if (offcenter > S) offcenter = S;
    }
    v = round(X*(maximum()-minimum())/(W-S) + minimum());
    // make sure a click outside the sliderbar moves it:
    if (event == FL_PUSH && v == value()) {
      offcenter = S/2;
      event = FL_DRAG;
      goto TRY_AGAIN;
    }
    handle_drag(clamp(v));
    } return 1;
  case FL_RELEASE:
    damage(FL_DAMAGE_EXPOSE);
    handle_release();
    return 1;
  default:
    return 0;
  }
}

int Fl_Slider::handle(int event) {
  return handle(event,
		x()+box()->dx(),
		y()+box()->dy(),
		w()-box()->dw(),
		h()-box()->dh());
}

static void revert(Fl_Style *s) {
  s->box = FL_THIN_DOWN_BOX;
  s->color = FL_DARK2;
}

Fl_Style Fl_Slider::default_style("Slider", revert);

//
// End of "$Id: Fl_Slider.cxx,v 1.28 1999/11/10 18:06:06 carl Exp $".
//
