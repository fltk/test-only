//
// "$Id: Fl_Value_Input.cxx,v 1.44 2004/01/06 06:43:02 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/ValueInput.h>
#include <fltk/Group.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/math.h>
#include <stdlib.h>
using namespace fltk;

/*! \class fltk::ValueInput

  Controls a single floating point value through a combination of a
  FloatInput and two buttons. Other toolkits call this a "Spin
  Box".

  \image html Fl_Value_Input.gif

  Clicking the buttons increments/decrements by the
  linesize(). Holding down any shift key and clicking
  increments/decrements by the pagesize().

  If step() is greater or equal to 1.0 an IntInput is used
  instead. This prevents the user from typing anything other than
  digits. If step() is less than one then the user can type floating
  point values with decimal points and exponents.

  If you change when() to fltk::WHEN_ENTER_KEY the callback is only
  done when the user hits the up/down arrow keys or when the user
  types the Enter key. This may be more useful than the default
  setting of fltk::WHEN_CHANGED which can make the callback happen
  when partially-edited numbers are in the field.

  You can get at the input field by using the public "input" instance
  variable. For instance you can clobber the text to a word with
  value_input->input.static_value("word").

*/

/* IMPLEMENTATION NOTE:

  This is *not* an Group even though it has a "child" widget, this
  appears to be a good idea and fltk should support it.  However
  Widget::parent() returns an Group, not an Widget like it did in fltk
  1.1 so this may not be proper C++.
*/

void ValueInput::input_cb(Widget*, void* v) {
  ValueInput& t = *(ValueInput*)v;
  double nv;
  if (t.step()>=1.0) nv = strtol(t.input.value(), 0, 0);
  else nv = strtod(t.input.value(), 0);
  if (nv != t.value() || t.when() & WHEN_NOT_CHANGED) {
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
// For the pushed() widget, which button is pushed:
static char which_pushed = 0;

void ValueInput::draw() {
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  const int bw = H/2; W -= bw;
  if (damage() & DAMAGE_ALL) {
    draw_frame();
    input.set_damage(DAMAGE_ALL);
  }
  if (damage() & (DAMAGE_ALL | DAMAGE_HIGHLIGHT)) {
    Flags ff = current_flags_highlight();
    Flags f[2];
    f[0] = f[1] = ff & ~HIGHLIGHT;
    if (which_highlight)
      f[which_highlight-1] = ff;
    if (which_pushed && pushed())
      f[which_pushed-1] = VALUE | HIGHLIGHT;
    draw_glyph(GLYPH_UP_BUTTON, X+W, Y, bw, bw, f[0]);
    draw_glyph(GLYPH_DOWN_BUTTON, X+W, Y+bw, bw, H-bw, f[1]);
  }
  input.label(label());
  input.align(align());
  input.copy_style(style());
  input.draw(X, Y, W, H);
  input.set_damage(0);
}

void ValueInput::increment_cb() {
  double i = linesize();
  if (event_state()&(SHIFT|CTRL|ALT)) i *= 10;
  if (which_pushed == 2) i = -i;
  handle_drag(value()+i);
}

#define INITIALREPEAT .5f
#define REPEAT .1f

int ValueInput::handle(int event) {
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  const int bw = H/2; W -= bw;
  int n;
  switch (event) {
  case FOCUS:
    focus(&input);
    break;
  case ENTER:
  case MOVE:
    if (!highlight_color()) return 1;
    if (event_inside(X+W, 0, w()-(X+W), Y+bw)) n = 1;
    else if (event_inside(X+W, Y+bw, w()-(X+W), h()-(Y+bw))) n = 2;
    else n = 0;
    if (n != which_highlight) {
      which_highlight = n;
      redraw_highlight();
    }
    return 1;
  case LEAVE:
    if (which_highlight) {
      which_highlight = 0;
      redraw_highlight();
    }
    return 1;
  case PUSH:
  case DRAG:
    // figure out what button is pushed:
    if (event_inside(X+W, 0, w()-(X+W), Y+bw)) n = 1;
    else if (event_inside(X+W, Y+bw, w()-(X+W), h()-(Y+bw))) n = 2;
    else n = 0;
    if (event == PUSH) {
      if (!n) break; // send mouse event to the input field
      handle_push();
    }
    if (n != which_pushed) {
      remove_timeout();
      which_highlight = which_pushed = n;
      redraw(DAMAGE_HIGHLIGHT);
      if (n) {
	add_timeout(INITIALREPEAT);
	increment_cb();
      }
    }
    return 1;
  case TIMEOUT:
    if (which_pushed) {
      repeat_timeout(REPEAT);
      increment_cb();
    }
    return 1;
  case RELEASE:
    if (pushed()) return 1; // ignore multiple buttons being released
    if (which_pushed) {
      remove_timeout();
      which_pushed = 0;
      redraw(DAMAGE_HIGHLIGHT);
    }
    handle_release();
    return 1;
  // make any drop replace all the text:
  case DND_ENTER:
  case DND_DRAG:
  case DND_LEAVE:
    // we return false if input has focus so drag-out works:
    return (!input.focused());
  case DND_RELEASE:
    take_focus();
    return 1;
  case PASTE:
    // dropped text produces this, replace all input text with it:
    input.position(0, input.size());
    break;
  }
  input.type(step()>=1.0 ? FloatInput::INT : FloatInput::FLOAT);
  input.when(when());
  int r = input.send(event);
  if (!r) r = Valuator::handle(event);
  return r;
}

void ValueInput::layout() {
  Valuator::layout();
  // this is needed so events sent directly to the input get correct
  // coordinates:
  input.resize(0, 0, w(), h());
  input.layout();
  // I'm not sure why this is here, may be a mistake:
  value_damage();
}

void ValueInput::value_damage() {
  // Only redraw the text if the numeric value is different..
  if (input.value()[0]) {
    if (step() >= 1) {
      if (strtol(input.value(), 0, 0) == long(value())) return;
    } else {
      // parse the existing text:
      double oldv = strtod(input.value(), 0);
      if (!oldv) {
	if (!value()) return;
      } else {
	if (fabs(fabs(value()/oldv)-1) < 1.2e-7) return;
      }
    }
  }
  char buf[128];
  format(buf);
  input.value(buf);
  //input.position(0, input.size()); // highlight it all
}

static int nogroup(int x) {Group::current(0); return x;}

ValueInput::ValueInput(int x, int y, int w, int h, const char* l)
: Valuator(x, y, w, h, l), input(nogroup(x), y, w, h, 0) {
  input.parent((Group*)this); // kludge!
  input.callback(input_cb, this);
  clear_flag(ALIGN_MASK);
  set_flag(ALIGN_LEFT);
  set_click_to_focus();
  Group::current(parent());
}

ValueInput::~ValueInput() {
  input.parent(0); // keep it from calling Group::remove(&input) on this
}

//
// End of "$Id: Fl_Value_Input.cxx,v 1.44 2004/01/06 06:43:02 spitzak Exp $".
//
