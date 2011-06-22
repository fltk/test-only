//
// "$Id$"
//
// Value input widget for the Fast Light Tool Kit (FLTK).
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

// FLTK widget for drag-adjusting a floating point value.
// Warning: this works by making a child fltk3::Input object, even
// though this object is *not* an fltk3::Group.  May be a kludge?

#include <fltk3/run.h>
#include <fltk3/ValueInput.h>
#include <fltk3/Group.h>
#include <stdlib.h>
#include <fltk3/math.h>


void fltk3::ValueInput::input_cb(fltk3::Widget*, void* v) {
  fltk3::ValueInput& t = *(fltk3::ValueInput*)v;
  double nv;
  if ((t.step() - floor(t.step()))>0.0 || t.step() == 0.0) nv = strtod(t.input.value(), 0);
  else nv = strtol(t.input.value(), 0, 0);
  if (nv != t.value() || t.when() & fltk3::WHEN_NOT_CHANGED) {
    t.set_value(nv);
    t.set_changed();
    if (t.when()) t.do_callback();
  }
}

void fltk3::ValueInput::draw() {
  FLTK3_OBJECT_VCALLS_WRAPPER(draw(), Draw)
  if (damage()&~fltk3::DAMAGE_CHILD) input.clear_damage(fltk3::DAMAGE_ALL);
  input.box(box());
  input.color(color(), selection_color());
  fltk3::Widget *i = &input; i->draw(); // calls protected input.draw()
  input.clear_damage();
}

void fltk3::ValueInput::resize(int X, int Y, int W, int H) {
  fltk3::Valuator::resize(X, Y, W, H);
  input.resize(X, Y, W, H);
}

void fltk3::ValueInput::value_damage() {
  char buf[128];
  format(buf);
  input.value(buf);
  input.mark(input.position()); // turn off selection highlight
}

int fltk3::ValueInput::handle(int event) {
  FLTK3_OBJECT_VCALLS_WRAPPER_RET(int, handle(event), Handle)
  double v;
  int delta;
  int mx = fltk3::event_x_root();
  static int ix, drag;
  input.when(when());
  switch (event) {
  case fltk3::PUSH:
    if (!step()) goto DEFAULT;
    ix = mx;
    drag = fltk3::event_button();
    handle_push();
    return 1;
  case fltk3::DRAG:
    if (!step()) goto DEFAULT;
    delta = mx-ix;
    if (delta > 5) delta -= 5;
    else if (delta < -5) delta += 5;
    else delta = 0;
    switch (drag) {
    case 3: v = increment(previous_value(), delta*100); break;
    case 2: v = increment(previous_value(), delta*10); break;
    default:v = increment(previous_value(), delta); break;
    }
    v = round(v);
    handle_drag(soft()?softclamp(v):clamp(v));;
    return 1;
  case fltk3::RELEASE:
    if (!step()) goto DEFAULT;
    if (value() != previous_value() || !fltk3::event_is_click())
      handle_release();
    else {
      fltk3::WidgetTracker wp(&input);
      input.handle(fltk3::PUSH);
      if (wp.exists())
	input.handle(fltk3::RELEASE);
    }
    return 1;
  case fltk3::FOCUS:
    return input.take_focus();
  case fltk3::SHORTCUT:
    return input.handle(event);
  default:
  DEFAULT:
    input.type(((step() - floor(step()))>0.0 || step() == 0.0) ? fltk3::FLOAT_INPUT : fltk3::INT_INPUT);
    return input.handle(event);
  }
}

/**
  Creates a new fltk3::ValueInput widget using the given
  position, size, and label string. The default boxtype is
  fltk3::DOWN_BOX.
*/
fltk3::ValueInput::ValueInput(int X, int Y, int W, int H, const char* l)
: fltk3::Valuator(X, Y, W, H, l), input(X, Y, W, H, 0) {
  soft_ = 0;
  if (input.parent())  // defeat automatic-add
    input.parent()->remove(input);
  input.parent((fltk3::Group *)this); // kludge!
  input.callback(input_cb, this);
  input.when(fltk3::WHEN_CHANGED);
  box(input.box());
  color(input.color());
  selection_color(input.selection_color());
  align(fltk3::ALIGN_LEFT);
  value_damage();
  set_flag(SHORTCUT_LABEL);
}

fltk3::ValueInput::~ValueInput() {

  if (input.parent() == (fltk3::Group *)this)
    input.parent(0);   // *revert* ctor kludge!
}

//
// End of "$Id$".
//
