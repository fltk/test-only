//
// "$Id: Fl_Value_Input.cxx,v 1.24 2000/08/20 04:31:38 spitzak Exp $"
//
// Value input widget for the Fast Light Tool Kit (FLTK).
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

// Fltk widget for drag-adjusting a floating point value.
// Warning: this works by making a child Fl_Input object, even
// though this object is *not* an Fl_Group.  May be a kludge?

#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Group.H>
#include <stdlib.h>

void Fl_Value_Input::input_cb(Fl_Widget*, void* v) {
  Fl_Value_Input& t = *(Fl_Value_Input*)v;
  double nv;
  if (t.step()>=1.0) nv = strtol(t.input.value(), 0, 0);
  else nv = strtod(t.input.value(), 0);
  if (nv != t.value() || t.when() & FL_WHEN_NOT_CHANGED) {
    t.set_value(nv);
    if (t.when()) {
      t.clear_changed();
      t.do_callback();
    } else {
      t.set_changed();
    }
  }
}

void Fl_Value_Input::draw() {
  if (damage()&~FL_DAMAGE_CHILD) input.set_damage(FL_DAMAGE_ALL);
  input.copy_style(style()); // force it to use this style
  input.draw();
  input.clear_damage();
}

void Fl_Value_Input::layout() {
  Fl_Valuator::layout();
  input.resize(x(), y(), w(), h());
  input.layout();
  value_damage();
}

void Fl_Value_Input::value_damage() {
  // Only redraw the text if the numeric value is different..
  if (input.value()[0]) {
    double nv;
    if (step()>=1.0) nv = strtol(input.value(), 0, 0);
    else nv = strtod(input.value(), 0);
    if (nv == value()) return;
  }
  char buf[128];
  format(buf);
  input.value(buf);
  input.mark(input.position()); // turn off selection highlight
}

int Fl_Value_Input::handle(int event) {
  double v;
  int delta;
  int mx = Fl::event_x()-Fl::event_y();
  static int ix, drag;
  switch (event) {
  case FL_PUSH:
    if (!step()) goto DEFAULT;
    take_focus();
    ix = mx;
    drag = Fl::event_button();
    handle_push();
    return 1;
  case FL_DRAG:
    if (!step()) goto DEFAULT;
    delta = mx-ix;
    if (delta > 5) delta -= 5;
    else if (delta < -5) delta += 5;
    else delta = 0;
    if (drag == 3) delta *= 100; else if (drag == 2) delta *= 10;
    v = round(increment(previous_value(), delta));
    handle_drag(soft()?softclamp(v):clamp(v));;
    input.position(input.size(),0);
    return 1;
  case FL_RELEASE:
    if (!step()) goto DEFAULT;
    if (!Fl::pushed()) {
      if (value() != previous_value() || !Fl::event_is_click())
        handle_release();
      else {
        input.handle(FL_PUSH);
        input.handle(FL_RELEASE);
      }
    }
    return 1;
  case FL_FOCUS:
    Fl::focus(&input);
  default:
  DEFAULT:
    input.type(step()>=1.0 ? FL_INT_INPUT : FL_FLOAT_INPUT);
    if (input.handle(event)) return 1;
    return Fl_Valuator::handle(event);
  }
}

Fl_Value_Input::Fl_Value_Input(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l), input(x, y, w, h, 0) {
  soft_ = 0;
  if (input.parent())  // defeat automatic-add
    input.parent()->remove(input);
  input.parent((Fl_Group*)this); // kludge!
  input.callback(input_cb, this);
  input.when(FL_WHEN_CHANGED);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT);
}

Fl_Value_Input::~Fl_Value_Input() {
  input.parent(0); // keep it from calling Fl_Group::remove(&input) on this
}

//
// End of "$Id: Fl_Value_Input.cxx,v 1.24 2000/08/20 04:31:38 spitzak Exp $".
//
