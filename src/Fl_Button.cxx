//
// "$Id: Fl_Button.cxx,v 1.39 2001/07/24 07:48:23 spitzak Exp $"
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
    if (o != this && o->type()==FL_RADIO_BUTTON)
      ((Fl_Button*)o)->clear();
  }
}

void Fl_Button::draw() {
  if (type() == FL_HIDDEN_BUTTON) {box(FL_NO_BOX); return;}
  Fl_Flags f = draw_button();
  draw_inside_label(0,0,w(),h(),f);
}

// Call the draw method, handle the clip out
void Fl_Button::draw_n_clip()
{
  if (type() != FL_HIDDEN_BUTTON) Fl_Widget::draw_n_clip();
}

int Fl_Button::handle(int event) {
  static int oldval;
  int newval;
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
  case FL_MOVE:
    return 1;
  case FL_PUSH:
#if CLICK_MOVES_FOCUS
    take_focus();
#endif
    oldval = value();
  case FL_DRAG:
    if (Fl::event_inside(0,0,w(),h())) {
      if (type()==FL_RADIO_BUTTON) newval = 1;
      else newval = !oldval;
    } else
      newval = oldval;
    if (value(newval) && when()&FL_WHEN_CHANGED) do_callback();
    return 1;
  case FL_RELEASE:
    if (!Fl::pushed()) {
      if (value() == oldval) return 1;
      if (type()==FL_RADIO_BUTTON)
	setonly();
      else if (type() == FL_TOGGLE_BUTTON)
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
    damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_KEYBOARD:
    if (Fl::event_key() == FL_Enter || Fl::event_key() == ' ') goto EXECUTE;
    return 0;
  case FL_SHORTCUT:
    if (!test_shortcut()) return 0;
  EXECUTE:
    if (type()==FL_RADIO_BUTTON && !value()) {
      setonly();
      if (when() & FL_WHEN_CHANGED) do_callback();
    } else if (type() == FL_TOGGLE_BUTTON) {
      value(!value());
      if (when() & FL_WHEN_CHANGED) do_callback();
    }
    if (when() & FL_WHEN_RELEASE) do_callback(); else set_changed();
    return 1;
  default:
    return 0;
  }
}

static void revert(Fl_Style*) {}

static Fl_Named_Style style("Button", revert, &Fl_Button::default_style);
Fl_Named_Style* Fl_Button::default_style = &::style;

Fl_Button::Fl_Button(int x,int y,int w,int h, const char *l) : Fl_Widget(x,y,w,h,l) {
  style(default_style);
}


#include <fltk/Fl_Round_Button.h>
Fl_Round_Button::Fl_Round_Button(int x,int y,int w,int h,const char *l)
	: Fl_Check_Button(x,y,w,h,l) { 
	shape = FL_GLYPH_ROUND; 
}

//
// End of "$Id: Fl_Button.cxx,v 1.39 2001/07/24 07:48:23 spitzak Exp $".
//
