//
// "$Id: Fl_Button.cxx,v 1.17 1999/11/01 02:21:30 carl Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

int Fl_Widget::set() {
  clear_changed();
  if (!value()) {set_value(); redraw(); return 1;}
  return 0;
}

int Fl_Widget::clear() {
  clear_changed();
  if (value()) {clear_value(); redraw(); return 1;}
  return 0;
}

int Fl_Widget::value(int v) {
  return v ? set() : clear();
}

void Fl_Widget::setonly() { // set this radio button on, turn others off
  value(1);
  Fl_Widget*const* a = parent()->array();
  for (int i = parent()->children(); i--;) {
    Fl_Widget* o = *a++;
    if (o != this && o->type()==FL_RADIO_BUTTON)
      o->value(0);
  }
}

void Fl_Button::draw() {
  if (type() == FL_HIDDEN_BUTTON) {box(FL_NO_BOX); return;}
  Fl_Color lc = draw_button();
  draw_label(lc);
}

// Call the draw method, handle the clip out
void Fl_Button::draw_n_clip()
{
  if (type() != FL_HIDDEN_BUTTON && 
      (box() != FL_NO_BOX || label() || image()))
    Fl_Widget::draw_n_clip();
}

int Fl_Button::handle(int event) {
  static int oldval;
  int newval;
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_PUSH:
    oldval = value();
  case FL_DRAG:
    if (Fl::event_inside(this)) {
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
  case FL_SHORTCUT:
    if (!(shortcut() ?
	  Fl::test_shortcut(shortcut()) : test_shortcut())) return 0;
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

Fl_Button::Fl_Button(int x,int y,int w,int h, const char *l) : Fl_Widget(x,y,w,h,l) {
  style(default_style);
  shortcut_ = 0;
  set_flag(FL_SHORTCUT_LABEL);
}

Fl_Style Fl_Button::default_style = {
  0,                    // box
  0,                    // glyph_box
  0,		        // glyphs
  0,		        // label_font
  0,		        // text_font
  0,		        // label_type
  0,		        // color
  0,		        // label_color
  0,                    // selection_color / on_color
  0,		        // selection_text_color
  0,                    // off_color
  FL_LIGHT2             // highlight color
};

static Fl_Style_Definer x("button", Fl_Button::default_style);

//
// End of "$Id: Fl_Button.cxx,v 1.17 1999/11/01 02:21:30 carl Exp $".
//
