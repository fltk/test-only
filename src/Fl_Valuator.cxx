//
// "$Id: Fl_Valuator.cxx,v 1.25 2002/07/01 15:28:19 spitzak Exp $"
//
// Valuator widget for the Fast Light Tool Kit (FLTK).
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

// Base class for sliders and all other one-value "knobs"

#include <fltk/Fl.h>
#include <fltk/Fl_Valuator.h>
#include <fltk/math.h>
#include <stdio.h>
#include <stdlib.h>

Fl_Valuator::Fl_Valuator(int X, int Y, int W, int H, const char* L)
  : Fl_Widget(X,Y,W,H,L) {
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_BOTTOM);
  when(FL_WHEN_CHANGED);
  value_ = 0.0;
  step_ = 0;
  minimum_ = 0;
  maximum_ = 1;
  linesize_ = 1;
  pagesize_ = 10;
}

#ifndef FLTK_2
void Fl_Valuator::precision(int p) {
  int B = 1;
  for (int i=0; i<p; i++) B *= 10;
  step_ = 1.0f/B;
}
#endif

void Fl_Valuator::value_damage() {
  redraw(FL_DAMAGE_VALUE); // default version does partial-redraw
}

int Fl_Valuator::value(double v) {
  clear_changed();
  if (v == value_) return 0;
  value_ = v;
  value_damage();
  return 1;
}

double Fl_Valuator::previous_value_;

// inline void Fl_Valuator::handle_push() {previous_value_ = value_;}

void Fl_Valuator::handle_drag(double v) {
  // round to nearest multiple of step:
  if (step_ >= 1) {
    double is = rint(step_); v = rint(v/is)*is;
  } else if (step_ > 0) {
    double is = rint(1/step_); v = rint(v*is)/is;
  }
  // If original value was in-range, clamp the new value:
  double A = minimum_; double B = maximum_;
  if (A > B) {A = B; B = minimum_;}
  if (v < A && previous_value_ >= A) v = A;
  else if (v > B && previous_value_ <= B) v = B;
  // store the value, redraw the widget, and do callback:
  if (v != value_) {
    value_ = v;
    value_damage();
    if (when() & FL_WHEN_CHANGED || !Fl::pushed()) do_callback();
    else set_changed();
  }
}

void Fl_Valuator::handle_release() {
  if (when()&FL_WHEN_RELEASE && !Fl::pushed()) {
    // insure changed() is off even if no callback is done.  It may have
    // been turned on by the drag, and then the slider returned to it's
    // initial position:
    clear_changed();
    // now do the callback only if slider in new position or always is on:
    if (value_ != previous_value_ || when() & FL_WHEN_NOT_CHANGED)
      do_callback();
  }
}

#if 0
double Fl_Valuator::increment(double v, int n) const {
  if (minimum_ > maximum_) n = -n;
  if (step_ >= 1) {
    double is = rint(step_);
    return rint(v/is+n)*is;
  } else if (step_ > 0) {
    double is = rint(1/step_);
    return rint(v*is+n)/is;
  } else {
    int is = int(100/fabs(maximum_-minimum_));
    if (!is) is = 1;
    return rint(v*is+n)/is;
  }
}
#endif

int Fl_Valuator::format(char* buffer) {
  double v = value();
  if (step_ >= 1) return sprintf(buffer, "%ld", long(v));
  else if (step_ <= 0) return sprintf(buffer, "%g", v);
  int i, x;
  int istep_ = int(rint(1/step_));
  for (x = 10, i = 2; x < istep_; x *= 10) i++;
  if (x == istep_) i--;
  return sprintf(buffer, "%.*f", i, v);
}

int Fl_Valuator::handle(int event) {
  switch(event) {
    case FL_ENTER:
    case FL_LEAVE:
      if (highlight_color() && takesevents()) redraw(FL_DAMAGE_HIGHLIGHT);
    case FL_MOVE:
      return 1;
    case FL_FOCUS:
    case FL_UNFOCUS:
      redraw(FL_DAMAGE_HIGHLIGHT);
      return 1;
    case FL_KEY: {
      float i = linesize();
      if (Fl::event_state()&(FL_SHIFT|FL_CTRL|FL_ALT)) i = pagesize();
      switch (Fl::event_key()) {
        case FL_Page_Up: i = pagesize(); goto MOVE_BY_i;
        case FL_Page_Down: i = -pagesize(); goto MOVE_BY_i;
        case FL_Down:
        case FL_Left:
          i = -i;
        case FL_Up:
        case FL_Right:
          MOVE_BY_i:
	  if (maximum() < minimum()) i = -i;
          handle_drag(value()+i);
          return 1;
        case FL_Home:
          handle_drag(minimum());
          return 1;
        case FL_End:
          handle_drag(maximum());
          return 1;
      }
      return 0;
    }
    case FL_MOUSEWHEEL: {
      // For normal valuators, each click is linesize(), wheel_scroll_lines
      // is ignored. However Fl_Scrollbar does use wheel_scroll_lines.
      handle_drag(value()+Fl::event_dy()*linesize());
      return 1;
    }
  }
  return 0;
}

//
// End of "$Id: Fl_Valuator.cxx,v 1.25 2002/07/01 15:28:19 spitzak Exp $".
//
