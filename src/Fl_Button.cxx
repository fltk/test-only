//
// "$Id: Fl_Button.cxx,v 1.44 2002/01/20 07:37:15 spitzak Exp $"
//
// Button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Button.h>
#include <fltk/Fl_Group.h>
#include <config.h>

int Fl_Button::set() {
  clear_changed();
  if (!value()) {set_value(); redraw(); return 1;}
  return 0;
}

int Fl_Button::clear() {
  clear_changed();
  if (value()) {clear_value(); redraw(); return 1;}
  return 0;
}

int Fl_Button::value(int v) {
  v ? set() : clear();
  return value();
}

void Fl_Button::setonly() { // set this radio button on, turn others off
  set();
  for (int i = parent()->children(); i--;) {
    Fl_Widget* o = parent()->child(i);
    if (o != this && o->type() == RADIO)
      ((Fl_Button*)o)->clear();
  }
}

extern Fl_Widget* fl_did_clipping;

void Fl_Button::draw() {
  if (type() == HIDDEN) {
    fl_did_clipping = this;
    return;
  }
  Fl_Flags f;
  if (value()) {
    // only use the pushed-in color if the user has explicitly set it
    // on this widget:
    if (style()->selection_color) f = FL_VALUE|FL_SELECTED;
    else f = FL_VALUE;
  } else {
    f = 0;
  }
  int x,y,w,h; f = draw_as_button(f,x,y,w,h);
  draw_inside_label(x,y,w,h,f);
}

int Fl_Button::handle(int event) {
  static int oldval;
  int newval;
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) redraw(FL_DAMAGE_HIGHLIGHT);
  case FL_MOVE:
    return 1;
  case FL_PUSH:
#if CLICK_MOVES_FOCUS
    take_focus();
#endif
    oldval = value();
  case FL_DRAG:
    if (Fl::event_inside(0,0,w(),h())) {
      if (type() == RADIO) newval = 1;
      else newval = !oldval;
    } else
      newval = oldval;
    if (value(newval) && when()&FL_WHEN_CHANGED) do_callback();
    return 1;
  case FL_RELEASE:
    if (!Fl::pushed()) {
      if (value() == oldval) return 1;
      if (type() == RADIO)
	setonly();
      else if (type()) // TOGGLE
	  ; // leave it as set
      else {
	value(oldval);
	if (when() & FL_WHEN_CHANGED) do_callback();
      }
      if (when() & FL_WHEN_RELEASE) do_callback(); else set_changed();
    }
    return 1;
  case FL_FOCUS:
  case FL_UNFOCUS:
    redraw(FL_DAMAGE_HIGHLIGHT);
    // grab initial focus if we are an Fl_Return_Button:
    return shortcut()==FL_Enter ? 2 : 1;
  case FL_KEYBOARD:
    if (Fl::event_key() == FL_Enter || Fl::event_key() == ' ') goto EXECUTE;
    return 0;
  case FL_SHORTCUT:
    if (!test_shortcut()) return 0;
  EXECUTE:
    if (type() == RADIO && !value()) {
      setonly();
      if (when() & FL_WHEN_CHANGED) do_callback();
    } else if (type()) { // TOGGLE
      value(!value());
      if (when() & FL_WHEN_CHANGED) do_callback();
    }
    if (when() & FL_WHEN_RELEASE) do_callback(); else set_changed();
    return 1;
  default:
    return 0;
  }
}

static void revert(Fl_Style* s) {
  s->color = FL_GRAY;
  s->box = FL_UP_BOX;
}

static Fl_Named_Style style("Button", revert, &Fl_Button::default_style);
Fl_Named_Style* Fl_Button::default_style = &::style;

Fl_Button::Fl_Button(int x,int y,int w,int h, const char *l) : Fl_Widget(x,y,w,h,l) {
  style(default_style);
}

//
// End of "$Id: Fl_Button.cxx,v 1.44 2002/01/20 07:37:15 spitzak Exp $".
//
