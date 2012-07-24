//
// "$Id$"
//
// Button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/Button.h>
#include <fltk3/Group.h>
#include <fltk3/Window.h>
#include <fltk3/draw.h>

#include <fltk3/RadioButton.h>
#include <fltk3/ToggleButton.h>
#include <fltk3/ToggleLightButton.h>
#include <fltk3/ToggleRoundButton.h>


fltk3::WidgetTracker *fltk3::Button::key_release_tracker = 0;
fltk3::Style* fltk3::Button::default_button_style_ = 0L;


// There are a lot of subclasses, named Fl_*_Button.  Some of
// them are implemented by setting the type() value and testing it
// here.  This includes fltk3::RadioButton and fltk3::ToggleButton


/**
 Sets the current value of the button.
 A non-zero value sets the button to 1 (ON), and zero sets it to 0 (OFF).
 \param[in] v button value.
 \see set(), clear()
 */
int fltk3::Button::value(int v) 
{
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
void fltk3::Button::setonly() 
{
  value(1);
  fltk3::Group* g = parent();
  fltk3::Widget*const* a = g->array();
  for (int i = g->children(); i--;) {
    fltk3::Widget* o = *a++;
    if (o != this && o->type()==fltk3::RADIO_BUTTON) ((fltk3::Button*)o)->value(0);
  }
}


void fltk3::Button::draw() 
{
  Box *b = value() ? ( down_box() ? down_box() : fltk3::down(box()) ) : box();
  draw(b, 0, 0, w(), h(), color(), 0);
}

void fltk3::Button::draw(Box* b, int x, int y, int w, int h, Color c, Box::Flags f)
{
  if (type() == fltk3::HIDDEN_BUTTON) return;
  fltk3::Color col;
  if (value()) {
    if (selection_color()==fltk3::GRAY) 
      col = fltk3::color_average(fltk3::BLACK, c, 0.2f);
    else
      col = selection_color();
  } else {
    col = c;
  }
  fltk3::draw_box(b, x, y, w, h, col, f);
  draw_backdrop();
  if (labeltype() == fltk3::NORMAL_LABEL && value()) {
    fltk3::Color c = labelcolor();
    labelcolor(fltk3::contrast(c, col));
    draw_label();
    labelcolor(c);
  } else draw_label();
  if (fltk3::focus() == this) draw_focus();
}


int fltk3::Button::handle(int event) 
{
  int newval;
  switch (event) {
    case fltk3::ENTER: /* FALLTHROUGH */
    case fltk3::LEAVE:
      //  if ((value_?selection_color():color())==fltk3::GRAY) redraw();
      return 1;
    case fltk3::PUSH:
      if (fltk3::visible_focus() && handle(fltk3::FOCUS)) fltk3::focus(this);
    case fltk3::DRAG:
      if (fltk3::event_inside(0, 0, w(), h())) {
        if (type() == fltk3::RADIO_BUTTON) {
          newval = 1;
        } else {
          newval = !oldval;
        }
      } else {
        clear_changed();
        newval = oldval;
      }
      if (newval != value_) {
        value_ = newval;
        set_changed();
        redraw();
        if (when() & fltk3::WHEN_CHANGED) do_callback();
      }
      return 1;
    case fltk3::RELEASE:
      if (value_ == oldval) {
        if (when() & fltk3::WHEN_NOT_CHANGED) do_callback();
        return 1;
      }
      set_changed();
      if (type() == fltk3::RADIO_BUTTON) setonly();
      else if (type() == fltk3::TOGGLE_BUTTON) oldval = value_;
      else {
        value(oldval);
        set_changed();
        if (when() & fltk3::WHEN_CHANGED) {
          fltk3::WidgetTracker wp(this);
          do_callback();
          if (wp.deleted()) return 1;
        }
      }
      if (when() & fltk3::WHEN_RELEASE) do_callback();
      return 1;
    case fltk3::SHORTCUT:
      if (!(shortcut() ?
            fltk3::test_shortcut(shortcut()) : test_shortcut())) return 0;    
      if (fltk3::visible_focus() && handle(fltk3::FOCUS)) fltk3::focus(this);
      goto triggered_by_keyboard;
    case fltk3::FOCUS : /* FALLTHROUGH */
    case fltk3::UNFOCUS :
      if (fltk3::visible_focus()) {
        if (box() == fltk3::NO_BOX) {
          // Widgets with the fltk3::NO_BOX boxtype need a parent to
          // redraw, since it is responsible for redrawing the
          // background...
          if (parent()) parent()->damage(fltk3::DAMAGE_ALL, -1, -1, w() + 2, h() + 2);
        } else redraw();
        return 1;
      } else return 0;
    case fltk3::KEYBOARD :
      if (fltk3::focus() == this && fltk3::event_key() == ' ' &&
          !(fltk3::event_state() & (fltk3::SHIFT | fltk3::CTRL | fltk3::ALT | fltk3::META))) {
        set_changed();
      triggered_by_keyboard:
        fltk3::WidgetTracker wp(this);
        if (type() == fltk3::RADIO_BUTTON) {
          if (!value_) {
            setonly();
            if (when() & fltk3::WHEN_CHANGED) do_callback();
          }
        } else if (type() == fltk3::TOGGLE_BUTTON) {
          value(!value());
          if (when() & fltk3::WHEN_CHANGED) do_callback();
        } else {
          simulate_key_action();
        }
        if (wp.deleted()) return 1;
        if (when() & fltk3::WHEN_RELEASE) do_callback();
        return 1;
      }
    default:
      return 0;
  }
}


void fltk3::Button::simulate_key_action()
{
  if (key_release_tracker) {
    fltk3::remove_timeout(key_release_timeout, key_release_tracker);
    key_release_timeout(key_release_tracker);
  }
  value(1); 
  redraw();
  key_release_tracker = new fltk3::WidgetTracker(this);
  fltk3::add_timeout(0.15, key_release_timeout, key_release_tracker);
}


void fltk3::Button::key_release_timeout(void *d)
{
  fltk3::WidgetTracker *wt = (fltk3::WidgetTracker*)d;
  if (!wt)
    return;
  if (wt==key_release_tracker) 
    key_release_tracker = 0L;
  fltk3::Button *btn = (fltk3::Button*)wt->widget();
  if (btn) {
    btn->value(0);
    btn->redraw();
  }
  delete wt;
}


/**
 The constructor creates the button using the given position, size and label.
 \param[in] X, Y, W, H position and size of the widget
 \param[in] L widget label, default is no label
 */
fltk3::Button::Button(int X, int Y, int W, int H, const char *L)
: fltk3::Widget(X,Y,W,H,L) 
{
  if (!default_button_style_) {
    default_button_style_ = new Style(fltk3::default_style);
    default_button_style_->box(fltk3::UP_BOX);
  }
  style(default_button_style_);
  down_box(fltk3::NO_BOX);
  value_ = oldval = 0;
  shortcut_ = 0;
  set_flag(SHORTCUT_LABEL);
}


fltk3::RadioButton::RadioButton(int x,int y,int w,int h,const char *l)
: fltk3::Button(x,y,w,h,l) 
{
  type(fltk3::RADIO_BUTTON);
}


fltk3::ToggleButton::ToggleButton(int X,int Y,int W,int H,const char *l)
: Button(X,Y,W,H,l) 
{
  type(TOGGLE_BUTTON);
}


fltk3::ToggleLightButton::ToggleLightButton(int x,int y,int w,int h,const char *l)
: LightButton(x, y, w, h, l) 
{
}


fltk3::ToggleRoundButton::ToggleRoundButton(int x,int y,int w,int h,const char *l)
: RoundButton(x, y, w, h, l) 
{
}


//
// End of "$Id$".
//
