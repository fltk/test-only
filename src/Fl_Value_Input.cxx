//
// "$Id: Fl_Value_Input.cxx,v 1.33 2002/07/01 15:28:19 spitzak Exp $"
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

// Fltk widget for input of a floating-point value. Contains an inner
// Fl_Input widget and two buttons to increment/decrement the value.
//
// This is *not* an Fl_Group even though it has a "child"
// widget, this appears to be a good idea and fltk should support it.
// However Fl_Widget::parent() returns an Fl_Group, not an Fl_Widget
// like it did in fltk 1.1 so this may not be proper C++.

#include <fltk/Fl.h>
#include <fltk/Fl_Value_Input.h>
#include <fltk/Fl_Group.h>
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

// For the belowmouse() widget, which button is highlighted:
static char which_highlight = 0;
static char which_pushed = 0;

void Fl_Value_Input::draw() {
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  const int bw = H/2; W -= bw;
  if (damage() & FL_DAMAGE_ALL) {
    draw_frame();
    input.set_damage(FL_DAMAGE_ALL);
  }
  if (damage() & (FL_DAMAGE_ALL | FL_DAMAGE_HIGHLIGHT)) {
    Fl_Flags f[2]; f[0] = f[1] = 0;
    if (which_highlight && this==Fl::belowmouse())
      f[which_highlight-1] = FL_HIGHLIGHT;
    if (which_pushed && this==Fl::pushed())
      f[which_pushed-1] = FL_VALUE | FL_HIGHLIGHT;
    draw_glyph(FL_GLYPH_UP_BUTTON, X+W, Y, bw, bw, f[0]);
    draw_glyph(FL_GLYPH_DOWN_BUTTON, X+W, Y+bw, bw, H-bw, f[1]);
  }
  input.label(label());
  input.align(align());
  input.copy_style(style());
  input.draw(X, Y, W, H);
  input.set_damage(0);
}

void Fl_Value_Input::increment_cb() {
  double i = linesize();
  if (Fl::event_state()&(FL_SHIFT|FL_CTRL|FL_ALT)) i = pagesize();
  if (which_pushed == 2) i = -i;
  handle_drag(value()+i);
}

#define INITIALREPEAT .5
#define REPEAT .1

void Fl_Value_Input::repeat_callback(void* v) {
  Fl_Value_Input* b = (Fl_Value_Input*)v;
  if (which_pushed) {
    Fl::add_timeout(REPEAT, repeat_callback, b);
    b->increment_cb();
  }
}

int Fl_Value_Input::handle(int event) {
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  const int bw = H/2; W -= bw;
  int n;
  switch (event) {
  case FL_FOCUS:
    Fl::focus(&input);
    break;
  case FL_ENTER:
  case FL_MOVE:
    if (!highlight_color()) return 1;
    if (Fl::event_inside(X+W, 0, w()-(X+W), Y+bw)) n = 1;
    else if (Fl::event_inside(X+W, Y+bw, w()-(X+W), h()-(Y+bw))) n = 2;
    else n = 0;
    if (n != which_highlight) {
      which_highlight = n;
      redraw(FL_DAMAGE_HIGHLIGHT);
    }
    return 1;
  case FL_LEAVE:
    if (which_highlight) {
      which_highlight = 0;
      redraw(FL_DAMAGE_HIGHLIGHT);
    }
    return 1;
  case FL_PUSH:
  case FL_DRAG:
    // figure out what button is pushed:
    if (Fl::event_inside(X+W, 0, w()-(X+W), Y+bw)) n = 1;
    else if (Fl::event_inside(X+W, Y+bw, w()-(X+W), h()-(Y+bw))) n = 2;
    else n = 0;
    if (event == FL_PUSH) {
      if (!n) break; // send mouse event to the input field
      handle_push();
    }
    if (n != which_pushed) {
      Fl::remove_timeout(repeat_callback, this);
      which_highlight = which_pushed = n;
      redraw(FL_DAMAGE_HIGHLIGHT);
      if (n) {
	Fl::add_timeout(INITIALREPEAT, repeat_callback, this);
	increment_cb();
      }
    }
    return 1;
  case FL_RELEASE:
    if (Fl::pushed()) return 1; // ignore multiple buttons being released
    if (which_pushed) {
      Fl::remove_timeout((Fl_Timeout_Handler)repeat_callback, this);
      which_pushed = 0;
      redraw(FL_DAMAGE_HIGHLIGHT);
    }
    handle_release();
    return 1;
  // make any drop replace all the text:
  case FL_DND_ENTER:
  case FL_DND_DRAG:
  case FL_DND_LEAVE:
    // we return false if input has focus so drag-out works:
    return (!input.focused());
  case FL_DND_RELEASE:
    take_focus();
    return 1;
  case FL_PASTE:
    // dropped text produces this, replace all input text with it:
    input.position(0, input.size());
    break;
  }
  input.type(step()>=1.0 ? Fl_Float_Input::INT : Fl_Float_Input::FLOAT);
  input.when(when());
  int r = input.send(event);
  if (!r) r = Fl_Valuator::handle(event);
  return r;
}

void Fl_Value_Input::layout() {
  Fl_Valuator::layout();
  // this is needed so events sent directly to the input get correct
  // coordinates:
  input.resize(0, 0, w(), h());
  input.layout();
  // I'm not sure why this is here, may be a mistake:
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
  input.position(0, input.size()); // highlight it all
}

Fl_Value_Input::Fl_Value_Input(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l), input(x, y, w, h, 0) {
  //soft_ = 0;
  if (input.parent())  // defeat automatic-add
    input.parent()->remove(input);
  input.parent((Fl_Group*)this); // kludge!
  input.callback(input_cb, this);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT);
  set_click_to_focus();
}

Fl_Value_Input::~Fl_Value_Input() {
  input.parent(0); // keep it from calling Fl_Group::remove(&input) on this
}

//
// End of "$Id: Fl_Value_Input.cxx,v 1.33 2002/07/01 15:28:19 spitzak Exp $".
//
