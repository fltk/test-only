//
// "$Id: Fl_Valuator.cxx,v 1.29 2002/12/10 01:46:17 easysw Exp $"
//
// Valuator widget for the Fast Light Tool Kit (FLTK).
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

// Base class for sliders and all other one-value "knobs"

#include <fltk/Valuator.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace fltk;

Valuator::Valuator(int X, int Y, int W, int H, const char* L)
  : Widget(X,Y,W,H,L) {
  clear_flag(ALIGN_MASK);
  set_flag(ALIGN_BOTTOM);
  when(WHEN_CHANGED);
  value_ = 0.0;
  step_ = 0;
  minimum_ = 0;
  maximum_ = 1;
  linesize_ = 1;
}

void Valuator::value_damage() {
  redraw(DAMAGE_VALUE); // default version does partial-redraw
}

int Valuator::value(double v) {
  clear_changed();
  if (v == value_) return 0;
  value_ = v;
  value_damage();
  return 1;
}

double Valuator::previous_value_;

// inline void Valuator::handle_push() {previous_value_ = value_;}

void Valuator::handle_drag(double v) {
  // round to nearest multiple of step:
  if (step_ >= 1) {
    v = rint(v/step_)*step_;
  } else if (step_ > 0) {
    // Try to detect fractions like .1 which are actually stored as
    // .9999999 and thus would round to unexpected values. This is done
    // by seeing if 1/N is close to an integer:
    double is = rint(1/step_);
    if (fabs(is*step_-1) < .001) v = rint(v*is)/is;
    else v = rint(v/step_)*step_;
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
    if (when() & WHEN_CHANGED || !pushed()) do_callback();
    else set_changed();
  }
}

void Valuator::handle_release() {
  if (when()&WHEN_RELEASE && !pushed()) {
    // insure changed() is off even if no callback is done.  It may have
    // been turned on by the drag, and then the slider returned to it's
    // initial position:
    clear_changed();
    // now do the callback only if slider in new position or always is on:
    if (value_ != previous_value_ || when() & WHEN_NOT_CHANGED)
      do_callback();
  }
}

int Valuator::format(char* buffer) {
  double v = value();
  if (step_ <= 0) return sprintf(buffer, "%g", v);
  else if (rint(step_) == step_) return sprintf(buffer, "%ld", long(v));
  int i, x;
  int istep_ = int(1/(step_-floor(step_))+.5);
  for (x = 10, i = 2; x < istep_; x *= 10) i++;
  if (x == istep_) i--;
  return sprintf(buffer, "%.*f", i, v);
}

int Valuator::handle(int event) {
  switch(event) {
    case ENTER:
    case LEAVE:
      if (highlight_color() && takesevents()) redraw(DAMAGE_HIGHLIGHT);
    case MOVE:
      return 1;
    case FOCUS:
    case UNFOCUS:
      redraw(DAMAGE_HIGHLIGHT);
      return 1;
    case KEY: {
      float i;
      switch (event_key()) {
        case DownKey:
        case LeftKey:
          i = -linesize();
	  goto J1;
        case UpKey:
        case RightKey:
          i = linesize();
      J1:
	  if (event_state()&(SHIFT|CTRL|ALT)) i *= 10;
	  if (maximum() < minimum()) i = -i;
          handle_drag(value()+i);
          return 1;
        case HomeKey:
          handle_drag(minimum());
          return 1;
        case EndKey:
          handle_drag(maximum());
          return 1;
      }
      return 0;
    }
    case MOUSEWHEEL: {
      // For normal valuators, each click is linesize(), wheel_scroll_lines
      // is ignored. However Scrollbar does use wheel_scroll_lines.
      handle_drag(value()+event_dy()*linesize());
      return 1;
    }
  }
  return 0;
}

//
// End of "$Id: Fl_Valuator.cxx,v 1.29 2002/12/10 01:46:17 easysw Exp $".
//
