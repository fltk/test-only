//
// "$Id: Fl_Slider.cxx,v 1.19 1999/08/16 07:31:21 bill Exp $"
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

Fl_Style Fl_Slider::default_style = {
  FL_THIN_DOWN_BOX,	// used to be a thick down box
  0,		// glyphs
  0,		// label_font
  0,		// text_font
  0,		// label_type
  FL_DARK2,	// color
  0,		// label_color
  FL_GRAY	// selection_color
  // rest is zero
};

Fl_Slider::Fl_Slider(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l) {
  style(default_style);
  slider_size_ = 0;
}

Fl_Slider::Fl_Slider(uchar t, int x, int y, int w, int h, const char* l)
  : Fl_Valuator(x, y, w, h, l) {
  style(default_style);
  type(t);
//   box(t==FL_HOR_NICE_SLIDER || t==FL_VERT_NICE_SLIDER ?
//       FL_FLAT_BOX : FL_DOWN_BOX);
  slider_size_ = 0;
}

void Fl_Slider::slider_size(double v) {
  if (v <  0) v = 0;
  if (v > 1) v = 1;
  if (slider_size_ != float(v)) {
    slider_size_ = float(v); 
    damage(FL_DAMAGE_EXPOSE);
  }
}

void Fl_Slider::bounds(double a, double b) {
  if (minimum() != a || maximum() != b) {
    Fl_Valuator::bounds(a, b); 
    damage(FL_DAMAGE_EXPOSE);
  }
}

int Fl_Slider::scrollvalue(int p, int w, int t, int l) {
//	p = position, first line displayed
//	w = window, number of lines displayed
//	t = top, number of first line
//	l = length, total number of lines
  step(1, 1);
  if (p+w > t+l) l = p+w-t;
  slider_size(w >= l ? 1.0 : double(w)/double(l));
  bounds(t, l-w+t);
  return value(p);
}

// All slider interaction is done as though the slider ranges from
// zero to one, and the left (bottom) edge of the slider is at the
// given position.  Since when the slider is all the way to the
// right (top) the left (bottom) edge is not all the way over, a
// position on the widget itself covers a wider range than 0-1,
// actually it ranges from 0 to 1/(1-size).

void Fl_Slider::draw_bg(int x, int y, int w, int h) {
  fl_color(color());
  fl_rectf(x, y, w, h);
  if (type() == FL_VERT_NICE_SLIDER) {
    FL_THIN_DOWN_BOX->draw(x+w/2-2, y, 4, h, FL_BLACK, 0);
  } else if (type() == FL_HOR_NICE_SLIDER) {
    FL_THIN_DOWN_BOX->draw(x, y+h/2-2, w, 4, FL_BLACK, 0);
  }
}

void Fl_Slider::draw(int x, int y, int w, int h) {
  double val;

  if (minimum() == maximum())
    val = 0.5;
  else {
    val = (value()-minimum())/(maximum()-minimum());
    if (val > 1.0) val = 1.0;
    else if (val < 0.0) val = 0.0;
  }

  int W = (horizontal() ? w : h);
  int X, S;
  if (type()==FL_HOR_FILL_SLIDER || type() == FL_VERT_FILL_SLIDER) {
    S = int(val*W+.5);
    if (minimum()>maximum()) {S = W-S; X = W-S;}
    else X = 0;
  } else {
    S = int(slider_size_*W+.5);
    int T = (horizontal() ? h : w)/2+1;
    if (type()==FL_VERT_NICE_SLIDER || type()==FL_HOR_NICE_SLIDER) T += 4;
    if (S < T) S = T;
    X = int(val*(W-S)+.5);
  }
  int xsl, ysl, wsl, hsl;
  if (horizontal()) {
    xsl = x+X;
    wsl = S;
    ysl = y;
    hsl = h;
  } else {
    ysl = y+X;
    hsl = S;
    xsl = x;
    wsl = w;
  }

  if (damage()&FL_DAMAGE_ALL) { // complete redraw
    draw_bg(x, y, w, h);
  } else { // partial redraw, clip off new position of slider
    if (X > 0) {
      if (horizontal()) fl_clip(x, ysl, X, hsl);
      else fl_clip(xsl, y, wsl, X);
      draw_bg(x, y, w, h);
      fl_pop_clip();
    }
    if (X+S < W) {
      if (horizontal()) fl_clip(xsl+wsl, ysl, x+w-xsl-wsl, hsl);
      else fl_clip(xsl, ysl+hsl, wsl, y+h-ysl-hsl);
      draw_bg(x, y, w, h);
      fl_pop_clip();
    }
  }

  if (wsl>0 && hsl>0) {
    Fl_Color c = color2(); // not color()!  Different from all other widgets...
    Fl_Flags f = 0;
    if (!active_r()) {
      f = FL_INACTIVE;
    } else if (Fl::belowmouse() == this && highlight_color()) {
      f = FL_HIGHLIGHT;
      c = highlight_color();
    }
    if (type() == FL_VERT_NICE_SLIDER) {
      glyph()(0, xsl, ysl, wsl, hsl, FL_GRAY, 0);
      int d = (hsl-4)/2;
      FL_THIN_DOWN_BOX->draw(xsl+2, ysl+d, wsl-4, hsl-2*d, c);
    } else if (type() == FL_HOR_NICE_SLIDER) {
      glyph()(0, xsl, ysl, wsl, hsl, FL_GRAY, 0);
      int d = (wsl-4)/2;
      FL_THIN_DOWN_BOX->draw(xsl+d, ysl+2, wsl-2*d, hsl-4, c);
    } else {
      glyph()(horizontal() ? FL_GLYPH_HSLIDER : FL_GLYPH_VSLIDER,
	      xsl, ysl, wsl, hsl, c, 0);
    }
  }

  draw_button_label(xsl, ysl, wsl, hsl, label_color());
}

void Fl_Slider::draw() {
  if (damage()&FL_DAMAGE_ALL) draw_frame();
  draw(x()+box()->dx(),
       y()+box()->dy(),
       w()-box()->dw(),
       h()-box()->dh());
}

int Fl_Slider::handle(int event, int x, int y, int w, int h) {
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && active_r()) redraw();
    return 1;
  case FL_PUSH:
    if (!Fl::event_inside(x, y, w, h)) return 0;
    handle_push();
  case FL_DRAG: {
    if (slider_size() >= 1 || minimum()==maximum()) return 1;
    int W = (horizontal() ? w : h);
    int X = (horizontal() ? Fl::event_x()-x : Fl::event_y()-y);
    int S = int(slider_size_*W+.5);
    int T = (horizontal() ? h : w)/2+1;
    if (type()==FL_VERT_NICE_SLIDER || type()==FL_HOR_NICE_SLIDER) T += 4;
    if (type()!=FL_HOR_FILL_SLIDER && type()!=FL_VERT_FILL_SLIDER) {
      if (S < T) S = T;
    }
    double v = double(X)/(W-S);
    double sliderwidth = double(S)/(W-S);
    double val = (value()-minimum())/(maximum()-minimum());
    static double offcenter;
    if (event == FL_PUSH) {
      offcenter = v-val;
      if (offcenter < 0) offcenter = 0;
      else if (offcenter > sliderwidth) offcenter = sliderwidth;
      else return 1;
    }
  TRY_AGAIN:
    v -= offcenter;
    if (v < 0) {
      offcenter = v+offcenter;
      if (offcenter<0) offcenter=0;
      v = 0;
    } else if (v > 1) {
      offcenter =  v+offcenter-1;
      if (offcenter > sliderwidth) offcenter = sliderwidth;
      v = 1;
    }
    // if (Fl::event_state(FL_SHIFT)) v = val+(v-val)*.05;
    v = round(v*(maximum()-minimum())+minimum());
    // make sure a click outside the sliderbar moves it:
    if (event == FL_PUSH && v == value()) {
      offcenter = sliderwidth/2;
      v = double(X)/(W-S);
      event = FL_DRAG;
      goto TRY_AGAIN;
    }
    handle_drag(clamp(v));
    } return 1;
  case FL_RELEASE:
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

//
// End of "$Id: Fl_Slider.cxx,v 1.19 1999/08/16 07:31:21 bill Exp $".
//
