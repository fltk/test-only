//
// "$Id: Fl_Valuator.cxx,v 1.7 1999/10/04 09:12:48 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Valuator.H>
#include <FL/math.h>
#include <stdio.h>

Fl_Valuator::Fl_Valuator(int X, int Y, int W, int H, const char* L)
  : Fl_Widget(X,Y,W,H,L) {
  align(FL_ALIGN_BOTTOM);
  when(FL_WHEN_CHANGED);
  value_ = 0.0;
  step_ = 0.0;
  minimum_ = 0.0;
  maximum_ = 1.0;
}

void Fl_Valuator::precision(int p) {
  double B = 1.0;
  for (int i=0; i<p; i++) B *= 10;
  step_ = 1.0/B;
}

void Fl_Valuator::value_damage() {
  damage(FL_DAMAGE_EXPOSE); // default version does partial-redraw
}

bool Fl_Valuator::value(double v) {
  clear_changed();
  if (v == value_) return false;
  value_ = v;
  value_damage();
  return true;
}

double Fl_Valuator::softclamp(double v) {
  int which = (minimum_ <= maximum_);
  double p = previous_value_;
  if ((v<minimum_)==which && p!=minimum_ && (p<minimum_)!=which)
    return minimum_;
  else if ((v>maximum_)==which && p!=maximum_ && (p>maximum_)!=which)
    return maximum_;
  else
    return v;
}

// inline void Fl_Valuator::handle_push() {previous_value_ = value_;}

void Fl_Valuator::handle_drag(double v) {
  if (v != value_) {
    value_ = v;
    value_damage();
    if (when() & FL_WHEN_CHANGED) do_callback();
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

double Fl_Valuator::round(double v) {
  if (!step_) return v;
  // this is necessary so that rounding errors do not cause steps like .1
  // to produce inaccurate results when v is very large:
  return rint(v*rint(1.0/step_))*step_;
}

double Fl_Valuator::clamp(double v) {
  if ((v<minimum_) == (minimum_<=maximum_)) return minimum_;
  else if ((v>maximum_) == (minimum_<=maximum_)) return maximum_;
  else return v;
}

double Fl_Valuator::increment(double v, int n) {
  if (!step_) return v+n*(maximum_-minimum_)/100;
  if (minimum_ > maximum_) n = -n;
  return rint(v*rint(1.0/step_)+n)*step_;
}

int Fl_Valuator::format(char* buffer) {
  double v = value();
  if (!step_) return sprintf(buffer, "%g", v);
  int i, x;
  double b = rint(1.0/step_);
  for (x = 10, i = 2; x < b; x *= 10) i++;
  if (x == b) i--;
  return sprintf(buffer, "%.*f", i, v);
}

//
// End of "$Id: Fl_Valuator.cxx,v 1.7 1999/10/04 09:12:48 bill Exp $".
//
