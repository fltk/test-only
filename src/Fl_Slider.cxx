//
// "$Id: Fl_Slider.cxx,v 1.16 1999/05/06 05:52:20 carl Exp $"
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

#define DEFAULT_STYLE ((Style*)default_style())

Fl_Widget::Style* Fl_Slider::_default_style = 0;

Fl_Slider::Style::Style() : Fl_Widget::Style() {
  sbf = 0;

  widget(BOX) = FL_MEDIUM_DOWN_BOX;
  slider(FLY_COLOR) = 51;
  slider(SLIDER_BOX) = FL_MEDIUM_UP_BOX2;
}

void Fl_Slider::loadstyle() const {
  if (!Fl::s_slider) {
    Fl::s_slider = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "color2", COLOR2 },
      { "color3", COLOR3 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("slider", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute slider_attributes[] = {
     { "highlight color", FLY_COLOR },
     { "slider box", SLIDER_BOX },
     { 0 }
    };
    Fl::load_attributes("scroll bar", DEFAULT_STYLE->slider_, slider_attributes);
  }
}

void Fl_Slider::_Fl_Slider() {
  slider_size_ = 0;
}

Fl_Slider::Fl_Slider(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l) {
  _Fl_Slider();
}

Fl_Slider::Fl_Slider(uchar t, int x, int y, int w, int h, const char* l)
  : Fl_Valuator(x, y, w, h, l) {
  type(t);
  box(t==FL_HOR_NICE_SLIDER || t==FL_VERT_NICE_SLIDER ?
      FL_FLAT_BOX : FL_DOWN_BOX);
  _Fl_Slider();
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
  draw_box(box(), x, y, w, h, color());
  int BW = Fl::box_dx(box());
  Fl_Color black = active_r() ? FL_BLACK : FL_INACTIVE_COLOR;
  if (type() == FL_VERT_NICE_SLIDER) {
    draw_box(FL_THIN_DOWN_BOX, x+w/2-2, y+BW, 4, h-2*BW, black);
  } else if (type() == FL_HOR_NICE_SLIDER) {
    draw_box(FL_THIN_DOWN_BOX, x+BW, y+h/2-2, w-2*BW, 4, black);
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

  int BW = Fl::box_dx(box());

  // CET - I think this looks better...
  if (type() != FL_HOR_FILL_SLIDER && type() != FL_VERT_FILL_SLIDER &&
      Fl::widget_style() != FL_MOTIF_STYLE)
    BW = 0;
  int W = (horizontal() ? w : h) - 2*BW;
  int X, S;
  if (type()==FL_HOR_FILL_SLIDER || type() == FL_VERT_FILL_SLIDER) {
    S = int(val*W+.5);
    if (minimum()>maximum()) {S = W-S; X = W-S+BW;}
    else X = BW;
  } else {
    S = int(slider_size_*W+.5);
    int T = (horizontal() ? h : w)/2-BW+1;
    if (type()==FL_VERT_NICE_SLIDER || type()==FL_HOR_NICE_SLIDER) T += 4;
    if (S < T) S = T;
    X = BW+int(val*(W-S)+.5);
  }
  int xsl, ysl, wsl, hsl;
  if (horizontal()) {
    xsl = x+X;
    wsl = S;
    ysl = y+BW;
    hsl = h-2*BW;
  } else {
    ysl = y+X;
    hsl = S;
    xsl = x+BW;
    wsl = w-2*BW;
  }

  if (damage()&FL_DAMAGE_ALL) { // complete redraw
    draw_bg(x, y, w, h);
  } else { // partial redraw, clip off new position of slider
    if (X > BW) {
      if (horizontal()) fl_clip(x, ysl, X, hsl);
      else fl_clip(xsl, y, wsl, X);
      draw_bg(x, y, w, h);
      fl_pop_clip();
    }
    if (X+S < W+BW) {
      if (horizontal()) fl_clip(xsl+wsl, ysl, x+w-BW-xsl-wsl, hsl);
      else fl_clip(xsl, ysl+hsl, wsl, y+h-BW-ysl-hsl);
      draw_bg(x, y, w, h);
      fl_pop_clip();
    }
  }

  Fl_Boxtype box1 = slider();

  Fl_Color col = // Used for regular sliders
    (Fl::belowmouse() == this && color2() == DEFAULT_STYLE->widget(COLOR2))
    ? fly_color() : color2();
  col = active_r() ? col : inactive(col);

  if (!box1) {box1 = (Fl_Boxtype)(box()&-2); if (!box1) box1 = FL_MEDIUM_UP_BOX;}
  Fl_Color col3 = // Used for "nice" sliders
    (Fl::belowmouse() == this && color3() == DEFAULT_STYLE->widget(COLOR3))
    ? fly_color() : color3();
  col3 = active_r() ? col3 : inactive(col3);
  Fl_Color selcol = active_r() ? selection_color() : inactive(selection_color());
  if (type() == FL_VERT_NICE_SLIDER) {
    draw_box(box1, xsl, ysl, wsl, hsl, col3);
    int d = (hsl-4)/2;
    draw_box(FL_THIN_DOWN_BOX, xsl+2, ysl+d, wsl-4, hsl-2*d, selcol);
  } else if (type() == FL_HOR_NICE_SLIDER) {
    draw_box(box1, xsl, ysl, wsl, hsl, col3);
    int d = (wsl-4)/2;
    draw_box(FL_THIN_DOWN_BOX, xsl+d, ysl+2, wsl-2*d, hsl-4, selcol);
  } else {
    if (wsl>0 && hsl>0) draw_box(box1, xsl, ysl, wsl, hsl, col);

// CET - Draws the little stripes on the scroll thumb.
// CET - This code could be smaller...
    if (Fl::widget_style() == FL_SGI_STYLE) {
      int X1 = xsl + Fl::box_dx(box1);
      int X2 = xsl + wsl - Fl::box_dx(box1) - 1;
      int Y1 = ysl + Fl::box_dy(box1);
      int Y2 = ysl + hsl - Fl::box_dy(box1) - 1;
      fl_clip(X1, Y1, X2 - X1 + 1, Y2 - Y1 + 1);
      if (type() == FL_HOR_SLIDER && wsl > 24) {
        int XM = xsl + wsl/2;
        int XF = XM - 4;
        int XL = XM + 4;
        for (int X = XF; X <= XL; X += 4) {
          fl_color(active_r() ? FL_LIGHT3 : inactive(FL_LIGHT3));
          fl_yxline(X - 1, Y1, Y2);
          fl_color(active_r() ? FL_BLACK : inactive(FL_BLACK));
          fl_yxline(X, Y1, Y2);
        }
      } else if(type() == FL_VERT_SLIDER && hsl > 24) {
        int YM = ysl + hsl/2;
        int YF = YM - 4;
        int YL = YM + 4;
        for (int Y = YF; Y <= YL; Y += 4) {
          fl_color(active_r() ? FL_LIGHT3 : inactive(FL_LIGHT3));
          fl_xyline(X1, Y - 1, X2);
          fl_color(active_r() ? FL_BLACK : inactive(FL_BLACK));
          fl_xyline(X1, Y, X2);
        }
      }
      fl_pop_clip();
    }

  }
  draw_label(xsl, ysl, wsl, hsl, labelcolor());
}

void Fl_Slider::draw() {
  draw(x(), y(), w(), h());
}

int Fl_Slider::handle(int event, int x, int y, int w, int h) {
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (takesevents() && type()!=FL_HOR_FILL_SLIDER && type()!=FL_VERT_FILL_SLIDER)
      redraw();
    return 1;
  case FL_PUSH:
    if (!Fl::event_inside(x, y, w, h)) return 0;
    handle_push();
  case FL_DRAG: {
    if (slider_size() >= 1 || minimum()==maximum()) return 1;
    int BW = Fl::box_dx(box());
    int W = (horizontal() ? w : h) - 2*BW;
    int X = (horizontal() ? Fl::event_x()-x : Fl::event_y()-y) - BW;
    int S = int(slider_size_*W+.5);
    int T = (horizontal() ? h : w)/2-BW+1;
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
    if (!Fl::pushed()) handle_release();
    return 1;
  default:
    return 0;
  }
}

int Fl_Slider::handle(int event) {
  return handle(event, x(), y(), w(), h());
}

uchar Fl_Slider::attr(Attribute a) const {
  loadstyle();
  if (!_style || !(SLIDER_STYLE->sbf & bf(a)))
    return DEFAULT_STYLE->slider(a);
  return SLIDER_STYLE->slider(a);
}

Fl_Boxtype Fl_Slider::slider() const { return (Fl_Boxtype)attr(SLIDER_BOX); }
Fl_Color Fl_Slider::fly_color() const { return (Fl_Color)attr(FLY_COLOR); }

//
// End of "$Id: Fl_Slider.cxx,v 1.16 1999/05/06 05:52:20 carl Exp $".
//
