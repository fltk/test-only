//
// "$Id: Fl_Slider.cxx,v 1.39 2000/03/17 09:50:08 bill Exp $"
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

// Draw the background behind the slider, draw() calls this more than
// once with different clipping so the slider does not blink:
void Fl_Slider::draw_bg(int x, int y, int w, int h, Fl_Flags f) {
  fl_color(color());
  fl_rectf(x, y, w, h);
  if (type() == FL_VERT_NICE_SLIDER) {
    FL_THIN_DOWN_BOX->draw(x+w/2-2, y, 4, h, fl_inactive(FL_BLACK, f), f);
  } else if (type() == FL_HOR_NICE_SLIDER) {
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

  if (!active_r()) f |= FL_INACTIVE;
  if (X > 0) {
    if (horizontal()) draw_bg(x, y, X, h, f);
    else draw_bg(x, y, w, X, f);
  }
  if (X+S < W) {
    if (horizontal()) draw_bg(x+X+S, y, w-X-S, h, f);
    else draw_bg(x, y+X+S, w, h-X-S, f);
  }

  // For compatability reasons the color is used for the background under
  // the slider and the selection_color is used for the slider itself.
  // It would be much better to use other arrangements, such as the
  // color for the slider, the off_color for the background, and the
  // selection_color for the handle of the nice-slider.

  Fl_Color bc = selection_color();
  if (!bc || type() == FL_VERT_NICE_SLIDER || type() == FL_HOR_NICE_SLIDER)
    bc = off_color();
  Fl_Color fc = selection_color();

  if (!active_r()) {
    f |= FL_INACTIVE;
  } else if ((f&FL_HIGHLIGHT) && highlight_color()) {
    bc = highlight_color();
  }
  if (horizontal()) {
    int g;
    if (type() == FL_HOR_NICE_SLIDER) g = FL_GLYPH_HNSLIDER;
    else if (type() == FL_HOR_FILL_SLIDER) g = 0;
    else g = FL_GLYPH_HSLIDER;
    glyph()(g, x+X, y, S, h, bc, fc, f, glyph_box());
    draw_button_label(x+X, y, S, h, label_color());
  } else {
    int g;
    if (type() == FL_VERT_NICE_SLIDER) g = FL_GLYPH_VNSLIDER;
    else if (type() == FL_VERT_FILL_SLIDER) g = 0;
    else g = FL_GLYPH_VSLIDER;
    glyph()(g, x, y+X, w, S, bc, fc, f, glyph_box());
    draw_button_label(x, y+X, w, S, label_color());
  }
}

void Fl_Slider::draw() {
  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) draw_frame();
  int X=x(); int Y=y(); int W=w(); int H=h(); box()->inset(X,Y,W,H);
  Fl_Flags f = 0;
  if (belowmouse()) f = FL_HIGHLIGHT;
  draw(X,Y,W,H, f);
  if (damage()&FL_DAMAGE_ALL) draw_label();
}

int Fl_Slider::handle(int event, int x, int y, int w, int h) {
  switch (event) {
  case FL_PUSH:
    if (!Fl::event_inside(x, y, w, h)) return 0;
    handle_push();
  case FL_DRAG: {
    int W = (horizontal() ? w : h);
    int H = (horizontal() ? h : w);
    int mx = (horizontal() ? Fl::event_x()-x : Fl::event_y()-y);
    int S = slider_size(W,H);
    if (W <= S) return 1;
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
  case FL_KEYBOARD: {
    // Only arrows in the correct direction are used.  Also the up/down
    // keystrokes are reversed from the default for Fl_Valuator.
    // This is due to back-compatability with scrollbars.
    int i = linesize();
    if (Fl::event_state()&(FL_SHIFT|FL_CTRL|FL_ALT)) i = pagesize();
    switch (Fl::event_key()) {
    case FL_Down: if (!horizontal()) goto UP; else return 0;
    case FL_Left: if (horizontal()) goto DOWN; else return 0;
    case FL_Up: if (!horizontal()) goto DOWN; else return 0;
    case FL_Right: if (horizontal()) goto UP; else return 0;
    case FL_BackSpace:
    DOWN:
      i = -i;
    case ' ':
    UP:
      handle_drag(clamp(increment(value(), i)));
      return 1;
    }}
    // else fall through to the default case:
  default:
    return Fl_Valuator::handle(event);
  }
}

int Fl_Slider::handle(int event) {
  int X=x(); int Y=y(); int W=w(); int H=h(); box()->inset(X,Y,W,H);
  if (event == FL_PUSH) take_focus();
  return handle(event,X,Y,W,H);
}

static void revert(Fl_Style *s) {
  s->box = FL_THIN_DOWN_BOX;
  s->color = FL_DARK2;
}

static Fl_Named_Style* style = new Fl_Named_Style("Slider", revert, &style);

Fl_Slider::Fl_Slider(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l) {
  style(::style);
  slider_size_ = 0;
}

Fl_Slider::Fl_Slider(uchar t, int x, int y, int w, int h, const char* l)
  : Fl_Valuator(x, y, w, h, l) {
  style(::style);
  slider_size_ = 0;
  type(t);
// fltk 1.0 (and XForms) set the box to FL_FLAT_BOX if the type was
// FL_HOR/VERT_NICE_SLIDER.
}

//
// End of "$Id: Fl_Slider.cxx,v 1.39 2000/03/17 09:50:08 bill Exp $".
//
