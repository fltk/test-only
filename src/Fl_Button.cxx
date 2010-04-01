//
// "$Id$"
//
// Button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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
#include <fltk3/Button.h>
#include <fltk3/Group.h>
#include <fltk3/Window.h>

// There are a lot of subclasses, named Fl_*_Button.  Some of
// them are implemented by setting the type() value and testing it
// here.  This includes fltk3::RadioButton and Fl_Toggle_Button

/**
  Sets the current value of the button.
  A non-zero value sets the button to 1 (ON), and zero sets it to 0 (OFF).
  \param[in] v button value.
  \see set(), clear()
 */
int fltk3::Button::value(int v) {
  v = v ? 1 : 0;
  oldval = v;
  clear_changed();
  if (value_ != v) {
    value_ = v;
    if (box()) redraw();
    else redraw_label();
    return 1;
  } else {
    return 0;
  }
}

/**
  Turns on this button and turns off all other radio buttons in the group
  (calling \c value(1) or \c set() does not do this).
 */
void fltk3::Button::setonly() { // set this radio button on, turn others off
  value(1);
  fltk3::Group* g = parent();
  fltk3::Widget*const* a = g->array();
  for (int i = g->children(); i--;) {
    fltk3::Widget* o = *a++;
    if (o != this && o->type()==FL_RADIO_BUTTON) ((fltk3::Button*)o)->value(0);
  }
}

void fltk3::Button::draw() {
  if (type() == FL_HIDDEN_BUTTON) return;
  Fl_Color col = value() ? selection_color() : color();
  draw_box(value() ? (down_box()?down_box():fl_down(box())) : box(), col);
  if (labeltype() == fltk3::NORMAL_LABEL && value()) {
    Fl_Color c = labelcolor();
    labelcolor(fl_contrast(c, col));
    draw_label();
    labelcolor(c);
  } else draw_label();
  if (fltk3::focus() == this) draw_focus();
}

int fltk3::Button::handle(int event) {
  int newval;
  switch (event) {
  case fltk3::ENTER: /* FALLTHROUGH */
  case fltk3::LEAVE:
//  if ((value_?selection_color():color())==FL_GRAY) redraw();
    return 1;
  case fltk3::PUSH:
    if (fltk3::visible_focus() && handle(fltk3::FOCUS)) fltk3::focus(this);
  case fltk3::DRAG:
    if (fltk3::event_inside(this)) {
      if (type() == FL_RADIO_BUTTON) newval = 1;
      else newval = !oldval;
    } else
    {
      clear_changed();
      newval = oldval;
    }
    if (newval != value_) {
      value_ = newval;
      set_changed();
      redraw();
      if (when() & FL_WHEN_CHANGED) do_callback();
    }
    return 1;
  case fltk3::RELEASE:
    if (value_ == oldval) {
      if (when() & FL_WHEN_NOT_CHANGED) do_callback();
      return 1;
    }
    set_changed();
    if (type() == FL_RADIO_BUTTON) setonly();
    else if (type() == FL_TOGGLE_BUTTON) oldval = value_;
    else {
      value(oldval);
      set_changed();
      if (when() & FL_WHEN_CHANGED) {
	Fl_Widget_Tracker wp(this);
        do_callback();
        if (wp.deleted()) return 1;
      }
    }
    if (when() & FL_WHEN_RELEASE) do_callback();
    return 1;
  case fltk3::SHORTCUT:
    if (!(shortcut() ?
	  fltk3::test_shortcut(shortcut()) : test_shortcut())) return 0;
    
    if (fltk3::visible_focus() && handle(fltk3::FOCUS)) fltk3::focus(this);

    if (type() == FL_RADIO_BUTTON && !value_) {
      setonly();
      set_changed();
      if (when() & (FL_WHEN_CHANGED|FL_WHEN_RELEASE) ) 
	  do_callback();
    } else if (type() == FL_TOGGLE_BUTTON) {
      value(!value());
      set_changed();
      if (when() & (FL_WHEN_CHANGED|FL_WHEN_RELEASE)) 
	  do_callback();
    } else if (when() & FL_WHEN_RELEASE) do_callback();
    return 1;
  case fltk3::FOCUS : /* FALLTHROUGH */
  case fltk3::UNFOCUS :
    if (fltk3::visible_focus()) {
      if (box() == fltk3::NO_BOX) {
	// Widgets with the fltk3::NO_BOX boxtype need a parent to
	// redraw, since it is responsible for redrawing the
	// background...
	int X = x() > 0 ? x() - 1 : 0;
	int Y = y() > 0 ? y() - 1 : 0;
	if (window()) window()->damage(FL_DAMAGE_ALL, X, Y, w() + 2, h() + 2);
      } else redraw();
      return 1;
    } else return 0;
  case fltk3::KEY :
    if (fltk3::focus() == this && fltk3::event_key() == ' ' &&
        !(fltk3::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META))) {
      set_changed();
      Fl_Widget_Tracker wp(this);
      if (type() == FL_RADIO_BUTTON && !value_) {
	setonly();
	if (when() & FL_WHEN_CHANGED) do_callback();
      } else if (type() == FL_TOGGLE_BUTTON) {
	value(!value());
	if (when() & FL_WHEN_CHANGED) do_callback();
      }
      if (wp.deleted()) return 1;
      if (when() & FL_WHEN_RELEASE) do_callback();
      return 1;
    }
  default:
    return 0;
  }
}

/**
  The constructor creates the button using the given position, size and label.
  \param[in] X, Y, W, H position and size of the widget
  \param[in] L widget label, default is no label
 */
fltk3::Button::Button(int X, int Y, int W, int H, const char *L)
: fltk3::Widget(X,Y,W,H,L) {
  box(fltk3::UP_BOX);
  down_box(fltk3::NO_BOX);
  value_ = oldval = 0;
  shortcut_ = 0;
  set_flag(SHORTCUT_LABEL);
}

//
// End of "$Id$".
//
