//
// "$Id: Fl_Button.cxx,v 1.9 1999/04/04 03:45:24 gustavo Exp $"
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

#define DEFAULT_STYLE ((Style*)default_style())

void Fl_Button::loadstyle() const {
  if (!Fl::s_button) {
    Fl::s_button = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "down color", COLOR2 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("button", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute button_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { "down box", DOWN_BOX },
      { "down label color", DOWN_LABELCOLOR },
      { 0 }
    };
    Fl::load_attributes("button", DEFAULT_STYLE->button_, button_attributes);
  }
}

#include <FL/Fl_Tooltip.H>

// There are a lot of subclasses, named Fl_*_Button.  Some of
// them are implemented by setting the type() value and testing it
// here.  This includes Fl_Radio_Button and Fl_Toggle_Button

int Fl_Button::value(int v) {
  v = v ? 1 : 0;
  oldval = v;
  clear_changed();
  if (value_ != v) {value_ = v; redraw(); return 1;}
  else return 0;
}

void Fl_Button::setonly() { // set this radio button on, turn others off
  value(1);
  Fl_Group* g = (Fl_Group*)parent();
  Fl_Widget*const* a = g->array();
  for (int i = g->children(); i--;) {
    Fl_Widget* o = *a++;
    if (o != this && o->type()==FL_RADIO_BUTTON) ((Fl_Button*)o)->value(0);
  }
}

void Fl_Button::draw() {
  if (type() == FL_HIDDEN_BUTTON) return;
  Fl_Color col = value() ? selection_color() : color();
  Fl_Boxtype bt = value() ? (down_box()?down_box():down(box())) : box();
  if (fly_box() && Fl::belowmouse() == this && !value())
    { bt = fly_box(); col = fly_color(); }

  draw_box(bt, col);

  Fl_Color lc = value() ? down_labelcolor() : labelcolor();
  draw_label(lc);
}

int Fl_Button::handle(int event) {
  int newval;
  switch (event) {
  case FL_ENTER:
    Fl_Tooltip::enter(this);
    if (fly_box()) redraw();
    return 1;
  case FL_LEAVE:
    Fl_Tooltip::exit(this);
    if (fly_box()) redraw();
    return 1;
  case FL_PUSH:
  case FL_DRAG:
    Fl_Tooltip::exit(this);
    if (Fl::event_inside(this)) {
      if (type() == FL_RADIO_BUTTON) newval = 1;
      else newval = !oldval;
    } else
      newval = oldval;
    if (newval != value_) {
      value_ = newval;
      redraw();
      if (when() & FL_WHEN_CHANGED) do_callback();
    }
    return 1;
  case FL_RELEASE:
    if (!Fl::pushed()) {
      Fl_Tooltip::exit(this);
      if (value_ == oldval) return 1;
      if (type() == FL_RADIO_BUTTON)
        setonly();
      else if (type() == FL_TOGGLE_BUTTON)
        oldval = value_;
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
    if (type() == FL_RADIO_BUTTON && !value_) {
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

Fl_Widget::Style* Fl_Button::_default_style = 0;

Fl_Button::Style::Style() : Fl_Widget::Style() {
  widget(BOX) = FL_MEDIUM_UP_BOX;
  sbf = 0;

  button(FLY_COLOR) = 51;
  button(DOWN_BOX) = FL_MEDIUM_DOWN_BOX;
  button(FLY_BOX) = FL_MEDIUM_UP_BOX;
  button(DOWN_LABELCOLOR) = FL_BLACK;
}

Fl_Button::Fl_Button(int x,int y,int w,int h, const char *l) : Fl_Widget(x,y,w,h,l) {
  value_ = oldval = 0;
  shortcut_ = 0;
  set_flag(SHORTCUT_LABEL);
}

Fl_Boxtype Fl_Button::fly_box() const {
  loadstyle();
  if (_style && (WIDGET_STYLE->sbf & bf(BOX)) && !(BUTTON_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)WIDGET_STYLE->widget(BOX);
  if (!_style || !(BUTTON_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->button(FLY_BOX);
  return (Fl_Boxtype)BUTTON_STYLE->button(FLY_BOX);
}

Fl_Color Fl_Button::fly_color() const {
  loadstyle();
  if (!_style || !(BUTTON_STYLE->sbf & bf(FLY_COLOR)))
    return (Fl_Color)DEFAULT_STYLE->button(FLY_COLOR);
  return (Fl_Color)BUTTON_STYLE->button(FLY_COLOR);
}

Fl_Boxtype Fl_Button::down_box() const {
  loadstyle();
  if (!_style || !(BUTTON_STYLE->sbf & bf(DOWN_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->button(DOWN_BOX);
  return (Fl_Boxtype)BUTTON_STYLE->button(DOWN_BOX);
}

Fl_Color Fl_Button::down_labelcolor() const {
  loadstyle();
  if (!_style || !(BUTTON_STYLE->sbf & bf(DOWN_LABELCOLOR)))
    return (Fl_Color)DEFAULT_STYLE->button(DOWN_LABELCOLOR);
  return (Fl_Color)BUTTON_STYLE->button(DOWN_LABELCOLOR);
}

Fl_Color Fl_Button::down_color() const {return selection_color();}

//
// End of "$Id: Fl_Button.cxx,v 1.9 1999/04/04 03:45:24 gustavo Exp $".
//
