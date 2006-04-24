//
// "$Id$"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Button.h>
#include <fltk/Group.h>
#include <fltk/Box.h>
#include <stdlib.h>
using namespace fltk;

/*! \class fltk::Button

  Buttons generate callbacks when they are clicked by the user. You
  control exactly when and how by changing the values for when():
  - fltk::WHEN_NEVER: The callback is not done, instead changed() is turned on.
  - fltk::WHEN_RELEASE: This is the default, the callback is done
    after the user successfully clicks the button (i.e. they let it go
    with the mouse still pointing at it), or when a shortcut is typed.
  - fltk::WHEN_CHANGED : The callback is done each time the value()
    changes (when the user pushes and releases the button, and as the
    mouse is dragged around in and out of the button).

  Buttons can also generate callbacks in response to fltk::SHORTCUT
  events. The button can either have an explicit shortcut() value or a
  letter shortcut can be indicated in the label() with an '&'
  character before it. For the label shortcut it does not matter if
  Alt is held down, but if you have an input field in the same window,
  the user will have to hold down the Alt key so that the input field
  does not eat the event first as an fltk::KEY event.

  \image html buttons.gif

*/

/*! Same as value(1). */
bool Button::set() {
  clear_changed();
  if (!value()) {set_flag(VALUE); redraw(); return true;}
  return false;
}

/*! Same as value(0). */
bool Button::clear() {
  clear_changed();
  if (value()) {clear_flag(VALUE); redraw(); return true;}
  return false;
}

/*! The current value. True means it is pushed down, false means it is
  not pushed down. Only with the ToggleButton subclass can the user change
  this permanelty, but you can change it for any button. */
bool Button::value(bool v) {
  return v ? set() : clear();
}

/*!  Turns on this button and turns off all other RadioButton objects in the
  parent Group (calling value(1) or set() does not do this).
*/
void Button::setonly() {
  set();
  for (int i = parent()->children(); i--;) {
    Widget* o = parent()->child(i);
    if (o != this && o->type() == RADIO)
      ((Button*)o)->clear();
  }
}

static Button* pushed_button;
static bool initial_value;

int Button::handle(int event) {
  return handle(event, Rectangle(w(),h()));
}
#include <stdio.h>
int Button::handle(int event, const Rectangle& rectangle) {
  switch (event) {
  case ENTER:
  case LEAVE:
    redraw_highlight();
  case MOVE:
    return 1;
  case PUSH:
    if (pushed()) return 1; /* this is pushed already, don't change value */
    initial_value = value();
  case DRAG: {
    bool new_value;
    if (event_inside(rectangle)) {
      pushed_button = this;
      // if (type() == RADIO) new_value = true; else
      new_value = !initial_value;
    } else {
      pushed_button = 0;
      new_value = initial_value;
    }
    if (value(new_value) && (when()&WHEN_CHANGED)) do_callback();
    return 1;}
  case RELEASE:
    if (value() == initial_value) return 1;
    redraw(DAMAGE_VALUE);
    if (type() == RADIO)
      setonly();
    else if (type()) // TOGGLE
      ; // leave it as set
    else {
      value(initial_value);
      if (when() & WHEN_CHANGED) {do_callback(); return 1;}
    }
    if (when() & WHEN_RELEASE) do_callback(); else set_changed();
    return 1;
  case FOCUS:
    redraw(1); // minimal redraw to just add the focus box
    // grab initial focus if we are an ReturnButton:
    return shortcut()==ReturnKey ? 2 : 1;
  case UNFOCUS:
    redraw_highlight(); //(DAMAGE_HIGHLIGHT);
    return 1;
  case KEY:
    if (event_key() == ' ' || event_key() == ReturnKey
	|| event_key() == KeypadEnter) goto EXECUTE;
    return 0;
  case SHORTCUT:
    if (!test_shortcut()) return 0;
  EXECUTE:
    if (type() == RADIO && !value()) {
      setonly();
      if (when() & WHEN_CHANGED) do_callback();
    } else if (type()) { // TOGGLE
      value(!value());
      if (when() & WHEN_CHANGED) do_callback();
    }
    if (when() & WHEN_RELEASE) do_callback(); else set_changed();
    return 1;
  default:
    return 0;
  }
}

////////////////////////////////////////////////////////////////

#include <fltk/draw.h>

extern Widget* fl_did_clipping;

/*!
  This function provides a mess of back-compatabilty and Windows
  emulation to subclasses of Button to draw with. It will draw the
  button according to the current state of being pushed and it's
  value. If non-zero is passed for \a glyph_width then the glyph()
  is drawn in that space on the left (or on the right if negative),
  and it assummes the glyph indicates the value, so the box is only
  used to indicate the pushed state.
*/
void Button::draw(int glyph_width) const
{
  // For back-compatability, setting color() or box() directly on a plain
  // button will cause it to act like buttoncolor() or buttonbox() are
  // set:
  Style localstyle;
  const Style* style = this->style();
  if (!glyph_width) {
    localstyle = *style;
    if (localstyle.color_) localstyle.buttoncolor_ = localstyle.color_;
    if (localstyle.box_) localstyle.buttonbox_ = localstyle.box_;
    if (localstyle.labelcolor_) localstyle.textcolor_ = localstyle.labelcolor_;
    style = &localstyle;
  }

  Box* box = style->buttonbox();

  Flags box_flags = flags() & ~(VALUE|PUSHED) | OUTPUT;
  Flags glyph_flags = box_flags & ~(HIGHLIGHT|OUTPUT);
  if (glyph_width) {
    if (value()) glyph_flags |= VALUE;
    if (this == pushed_button && pushed()) {
      box_flags |= VALUE|PUSHED;
      if (box == NO_BOX) glyph_flags |= PUSHED;
      else if (!(when()&WHEN_CHANGED)) glyph_flags ^= VALUE;
    }
  } else {
    if (value()) box_flags |= VALUE;
    if (this == pushed_button && pushed()) box_flags |= PUSHED;
  }

  // only draw "inside" labels:
  bool draw_label = true;
  Rectangle r(0,0,w(),h());

  if (box == NO_BOX) {
    Color bg;
    if (box_flags & HIGHLIGHT && (bg = style->highlight_color())) {
      setcolor(bg);
      fillrect(r);
    } else if ((damage()&(DAMAGE_EXPOSE|DAMAGE_HIGHLIGHT))) {
      // erase the background so we can redraw the label in the new color:
      draw_background();
    } else if (!label()) {
      // Assumme this is a button with an animated image label.
      // we must redraw the image so it changes depending on state.
      // Partially-transparent pixels will not draw right!
    } else if (context_image()) {
	draw_background();
    } else {
      // Don't draw the label unnecessarily:
      draw_label = false;
    }
    // this allows these buttons to be put into browser/menus:
    //fg = fl_item_labelcolor(this);
  } else {
    if ((damage()&(DAMAGE_EXPOSE|DAMAGE_HIGHLIGHT)) && !box->fills_rectangle()) {
      // Erase the area behind non-square boxes
      draw_background();
    }
  }

  // Draw the box:
  drawstyle(style,box_flags);
  // For back-compatability we use any directly-set selection_color()
  // to color the box:
  if (!glyph_width && value() && style->selection_color_) {
    setbgcolor(style->selection_color_);
    setcolor(contrast(style->selection_textcolor(),style->selection_color_));
  }
  box->draw(r);
  box->inset(r);

  if (draw_label) {
    Rectangle lr(r);
    if (glyph_width) {
      if (glyph_width < 0)
	lr.w(lr.w()+glyph_width-3);
      else
	lr.set_x(glyph_width+3);
    }
    this->draw_label(lr, box_flags);
  }

  if (glyph_width) {
    int g = abs(glyph_width);
    Rectangle gr(r, g, g);
    if (glyph_width < 0)
      gr.x(r.r()-g-3);
    else
      gr.x(r.x()+3);
    drawstyle(style,glyph_flags);
    this->glyph()->draw(gr);
    drawstyle(style,box_flags);
  }
  focusbox()->draw(r);
}

void Button::draw() {
  if (type() == HIDDEN) {
    fl_did_clipping = this;
    return;
  }
  draw(0);
}

////////////////////////////////////////////////////////////////

static NamedStyle style("Button", 0, &Button::default_style);
NamedStyle* Button::default_style = &::style;

Button::Button(int x,int y,int w,int h, const char *l) : Widget(x,y,w,h,l) {
  style(default_style);
  //set_click_to_focus();
}

////////////////////////////////////////////////////////////////

/*! \class fltk::ToggleButton
  This button turns the value() on and off each release of a click
  inside of it.

  You can also convert a regular button into this by doing
  type(Button::TOGGLE) to it.
*/

//
// End of "$Id$".
//
