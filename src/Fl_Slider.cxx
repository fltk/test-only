//
// "$Id$"
//
// Slider widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/run.h>
#include <fltk3/Slider.h>
#include <fltk3/draw.h>
#include <math.h>
#include "flstring.h"

void Fl_Slider::_Fl_Slider() {
  slider_size_ = 0;
  slider_ = 0; // fltk3::UP_BOX;
}

/**
  Creates a new Fl_Slider widget using the given position,
  size, and label string. The default boxtype is fltk3::DOWN_BOX.
*/
Fl_Slider::Fl_Slider(int X, int Y, int W, int H, const char* L)
: Fl_Valuator(X, Y, W, H, L) {
  box(fltk3::DOWN_BOX);
  _Fl_Slider();
}

/**
  Creates a new Fl_Slider widget using the given box type, position,
  size, and label string.
*/
Fl_Slider::Fl_Slider(uchar t, int X, int Y, int W, int H, const char* L)
  : Fl_Valuator(X, Y, W, H, L) {
  type(t);
  box(t==fltk3::HOR_NICE_SLIDER || t==fltk3::VERT_NICE_SLIDER ?
      fltk3::FLAT_BOX : fltk3::DOWN_BOX);
  _Fl_Slider();
}

void Fl_Slider::slider_size(double v) {
  if (v <  0) v = 0;
  if (v > 1) v = 1;
  if (slider_size_ != float(v)) {
    slider_size_ = float(v); 
    damage(fltk3::DAMAGE_EXPOSE);
  }
}

/** 
  Sets the minimum (a) and maximum (b) values for the valuator widget. 
  if at least one of the values is changed, a partial redraw is asked.
*/
void Fl_Slider::bounds(double a, double b) {
  if (minimum() != a || maximum() != b) {
    Fl_Valuator::bounds(a, b); 
    damage(fltk3::DAMAGE_EXPOSE);
  }
}

/**
  Sets the size and position of the sliding knob in the box.
  \param[in] pos position of first line displayed
  \param[in] size size of window in lines
  \param[in] first number of first line
  \param[in] total total number of lines
  Returns Fl_Valuator::value(p)
 */
int Fl_Slider::scrollvalue(int pos, int size, int first, int total) {
  step(1, 1);
  if (pos+size > first+total) total = pos+size-first;
  slider_size(size >= total ? 1.0 : double(size)/double(total));
  bounds(first, total-size+first);
  return value(pos);
}

// All slider interaction is done as though the slider ranges from
// zero to one, and the left (bottom) edge of the slider is at the
// given position.  Since when the slider is all the way to the
// right (top) the left (bottom) edge is not all the way over, a
// position on the widget itself covers a wider range than 0-1,
// actually it ranges from 0 to 1/(1-size).

void Fl_Slider::draw_bg(int X, int Y, int W, int H) {
  fltk3::push_clip(X, Y, W, H);
  draw_box();
  fltk3::pop_clip();

  fltk3::Color black = active_r() ? fltk3::FOREGROUND_COLOR : fltk3::INACTIVE_COLOR;
  if (type() == fltk3::VERT_NICE_SLIDER) {
    draw_box(fltk3::THIN_DOWN_BOX, X+W/2-2, Y, 4, H, black);
  } else if (type() == fltk3::HOR_NICE_SLIDER) {
    draw_box(fltk3::THIN_DOWN_BOX, X, Y+H/2-2, W, 4, black);
  }
}

void Fl_Slider::draw(int X, int Y, int W, int H) {

  double val;
  if (minimum() == maximum())
    val = 0.5;
  else {
    val = (value()-minimum())/(maximum()-minimum());
    if (val > 1.0) val = 1.0;
    else if (val < 0.0) val = 0.0;
  }

  int ww = (horizontal() ? W : H);
  int xx, S;
  if (type()==fltk3::HOR_FILL_SLIDER || type() == fltk3::VERT_FILL_SLIDER) {
    S = int(val*ww+.5);
    if (minimum()>maximum()) {S = ww-S; xx = ww-S;}
    else xx = 0;
  } else {
    S = int(slider_size_*ww+.5);
    int T = (horizontal() ? H : W)/2+1;
    if (type()==fltk3::VERT_NICE_SLIDER || type()==fltk3::HOR_NICE_SLIDER) T += 4;
    if (S < T) S = T;
    xx = int(val*(ww-S)+.5);
  }
  int xsl, ysl, wsl, hsl;
  if (horizontal()) {
    xsl = X+xx;
    wsl = S;
    ysl = Y;
    hsl = H;
  } else {
    ysl = Y+xx;
    hsl = S;
    xsl = X;
    wsl = W;
  }

  draw_bg(X, Y, W, H);

  fltk3::Boxtype box1 = slider();
  if (!box1) {box1 = (fltk3::Boxtype)(box()&-2); if (!box1) box1 = fltk3::UP_BOX;}
  if (type() == fltk3::VERT_NICE_SLIDER) {
    draw_box(box1, xsl, ysl, wsl, hsl, fltk3::GRAY);
    int d = (hsl-4)/2;
    draw_box(fltk3::THIN_DOWN_BOX, xsl+2, ysl+d, wsl-4, hsl-2*d,selection_color());
  } else if (type() == fltk3::HOR_NICE_SLIDER) {
    draw_box(box1, xsl, ysl, wsl, hsl, fltk3::GRAY);
    int d = (wsl-4)/2;
    draw_box(fltk3::THIN_DOWN_BOX, xsl+d, ysl+2, wsl-2*d, hsl-4,selection_color());
  } else {
    if (wsl>0 && hsl>0) draw_box(box1, xsl, ysl, wsl, hsl, selection_color());

    if (type()!=fltk3::HOR_FILL_SLIDER && type() != fltk3::VERT_FILL_SLIDER &&
        fltk3::scheme_ && !strcmp(fltk3::scheme_, "gtk+")) {
      if (W>H && wsl>(hsl+8)) {
        // Draw horizontal grippers
	int yy, hh;
	hh = hsl-8;
	xx = xsl+(wsl-hsl-4)/2;
	yy = ysl+3;

	fltk3::color(fltk3::darker(selection_color()));
	fltk3::line(xx, yy+hh, xx+hh, yy);
	fltk3::line(xx+6, yy+hh, xx+hh+6, yy);
	fltk3::line(xx+12, yy+hh, xx+hh+12, yy);

        xx++;
	fltk3::color(fltk3::lighter(selection_color()));
	fltk3::line(xx, yy+hh, xx+hh, yy);
	fltk3::line(xx+6, yy+hh, xx+hh+6, yy);
	fltk3::line(xx+12, yy+hh, xx+hh+12, yy);
      } else if (H>W && hsl>(wsl+8)) {
        // Draw vertical grippers
	int yy;
	xx = xsl+4;
	ww = wsl-8;
	yy = ysl+(hsl-wsl-4)/2;

	fltk3::color(fltk3::darker(selection_color()));
	fltk3::line(xx, yy+ww, xx+ww, yy);
	fltk3::line(xx, yy+ww+6, xx+ww, yy+6);
	fltk3::line(xx, yy+ww+12, xx+ww, yy+12);

        yy++;
	fltk3::color(fltk3::lighter(selection_color()));
	fltk3::line(xx, yy+ww, xx+ww, yy);
	fltk3::line(xx, yy+ww+6, xx+ww, yy+6);
	fltk3::line(xx, yy+ww+12, xx+ww, yy+12);
      }
    }
  }

  draw_label(xsl, ysl, wsl, hsl);
  if (fltk3::focus() == this) {
    if (type() == fltk3::HOR_FILL_SLIDER || type() == fltk3::VERT_FILL_SLIDER) draw_focus();
    else draw_focus(box1, xsl, ysl, wsl, hsl);
  }
}

void Fl_Slider::draw() {
  if (damage()&fltk3::DAMAGE_ALL) draw_box();
  draw(x()+fltk3::box_dx(box()),
       y()+fltk3::box_dy(box()),
       w()-fltk3::box_dw(box()),
       h()-fltk3::box_dh(box()));
}

int Fl_Slider::handle(int event, int X, int Y, int W, int H) {
  // fltk3::WidgetTracker wp(this);
  switch (event) {
  case fltk3::PUSH: {
    fltk3::WidgetTracker wp(this);
    if (!fltk3::event_inside(X, Y, W, H)) return 0;
    handle_push();
    if (wp.deleted()) return 1; }
    // fall through ...
  case fltk3::DRAG: {

    double val;
    if (minimum() == maximum())
      val = 0.5;
    else {
      val = (value()-minimum())/(maximum()-minimum());
      if (val > 1.0) val = 1.0;
      else if (val < 0.0) val = 0.0;
    }

    int ww = (horizontal() ? W : H);
    int mx = (horizontal() ? fltk3::event_x()-X : fltk3::event_y()-Y);
    int S;
    static int offcenter;

    if (type() == fltk3::HOR_FILL_SLIDER || type() == fltk3::VERT_FILL_SLIDER) {

      S = 0;
      if (event == fltk3::PUSH) {
	int xx = int(val*ww+.5);
	offcenter = mx-xx;
	if (offcenter < -10 || offcenter > 10) offcenter = 0;
	else return 1;
      }

    } else {

      S = int(slider_size_*ww+.5); if (S >= ww) return 0;
      int T = (horizontal() ? H : W)/2+1;
      if (type()==fltk3::VERT_NICE_SLIDER || type()==fltk3::HOR_NICE_SLIDER) T += 4;
      if (S < T) S = T;
      if (event == fltk3::PUSH) {
	int xx = int(val*(ww-S)+.5);
	offcenter = mx-xx;
	if (offcenter < 0) offcenter = 0;
	else if (offcenter > S) offcenter = S;
	else return 1;
      }
    }

    int xx = mx-offcenter;
    double v;
    char tryAgain = 1;
    while (tryAgain)
    {
      tryAgain = 0;
      if (xx < 0) {
        xx = 0;
        offcenter = mx; if (offcenter < 0) offcenter = 0;
      } else if (xx > (ww-S)) {
        xx = ww-S;
        offcenter = mx-xx; if (offcenter > S) offcenter = S;
      }
      v = round(xx*(maximum()-minimum())/(ww-S) + minimum());
      // make sure a click outside the sliderbar moves it:
      if (event == fltk3::PUSH && v == value()) {
        offcenter = S/2;
        event = fltk3::DRAG;
        tryAgain = 1;
      }
    }
    handle_drag(clamp(v));
    } return 1;
  case fltk3::RELEASE:
    handle_release();
    return 1;
  case fltk3::KEYBOARD:
    { fltk3::WidgetTracker wp(this);
      switch (fltk3::event_key()) {
	case fltk3::UpKey:
	  if (horizontal()) return 0;
	  handle_push();
	  if (wp.deleted()) return 1;
	  handle_drag(clamp(increment(value(),-1)));
	  if (wp.deleted()) return 1;
	  handle_release();
	  return 1;
	case fltk3::DownKey:
	  if (horizontal()) return 0;
	  handle_push();
	  if (wp.deleted()) return 1;
	  handle_drag(clamp(increment(value(),1)));
	  if (wp.deleted()) return 1;
	  handle_release();
	  return 1;
	case fltk3::LeftKey:
	  if (!horizontal()) return 0;
	  handle_push();
	  if (wp.deleted()) return 1;
	  handle_drag(clamp(increment(value(),-1)));
	  if (wp.deleted()) return 1;
	  handle_release();
	  return 1;
	case fltk3::RightKey:
	  if (!horizontal()) return 0;
	  handle_push();
	  if (wp.deleted()) return 1;
	  handle_drag(clamp(increment(value(),1)));
	  if (wp.deleted()) return 1;
	  handle_release();
	  return 1;
	default:
	  return 0;
      }
    }
    // break not required because of switch...
  case fltk3::FOCUS :
  case fltk3::UNFOCUS :
    if (fltk3::visible_focus()) {
      redraw();
      return 1;
    } else return 0;
  case fltk3::ENTER :
  case fltk3::LEAVE :
    return 1;
  default:
    return 0;
  }
}

int Fl_Slider::handle(int event) {
  if (event == fltk3::PUSH && fltk3::visible_focus()) {
    fltk3::focus(this);
    redraw();
  }

  return handle(event,
		x()+fltk3::box_dx(box()),
		y()+fltk3::box_dy(box()),
		w()-fltk3::box_dw(box()),
		h()-fltk3::box_dh(box()));
}

//
// End of "$Id$".
//
