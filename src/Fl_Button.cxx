//
// "$Id: Fl_Button.cxx,v 1.65 2003/09/03 06:08:06 spitzak Exp $"
//
// Button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Button.h>
#include <fltk/Group.h>
#include <fltk/Box.h>
using namespace fltk;

bool Button::set() {
  clear_changed();
  if (!value()) {set_value(); redraw(); return true;}
  return false;
}

bool Button::clear() {
  clear_changed();
  if (value()) {clear_value(); redraw(); return true;}
  return false;
}

bool Button::value(bool v) {
  return v ? set() : clear();
}

void Button::setonly() { // set this radio button on, turn others off
  set();
  for (int i = parent()->children(); i--;) {
    Widget* o = parent()->child(i);
    if (o != this && o->type() == RADIO)
      ((Button*)o)->clear();
  }
}

static Button* held_down;

int Button::handle(int event) {
  static bool oldval;
  bool newval;
  switch (event) {
  case ENTER:
  case LEAVE:
    if (highlight_color() && takesevents()) redraw(DAMAGE_HIGHLIGHT);
  case MOVE:
    return 1;
  case PUSH:
    oldval = value();
  case DRAG:
    if (event_inside(0,0,w(),h())) {
      held_down = this;
      if (type() == RADIO) newval = 1;
      else newval = !oldval;
    } else {
      held_down = 0;
      newval = oldval;
    }
    if (value(newval) && when()&WHEN_CHANGED) do_callback();
    return 1;
  case RELEASE:
    redraw(DAMAGE_VALUE);
    held_down = 0;
    if (value() == oldval) return 1;
    if (type() == RADIO)
      setonly();
    else if (type()) // TOGGLE
      ; // leave it as set
    else {
      value(oldval);
      if (when() & WHEN_CHANGED) do_callback();
    }
    if (when() & WHEN_RELEASE) do_callback(); else set_changed();
    return 1;
  case FOCUS:
    redraw(1); // minimal redraw to just add the focus box
    // grab initial focus if we are an ReturnButton:
    return shortcut()=='\r' ? 2 : 1;
  case UNFOCUS:
    redraw(DAMAGE_HIGHLIGHT);
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

// Draw button-like widgets with an optional glyph. The glyph is given
// a size (negative to put it on the right)
void Button::draw(int glyph, int glyph_width) const
{
  // For back-compatability color() is used if it is directly set
  // and there is no glyph:
  Color bg;
  if (!glyph_width && style()->color) bg = style()->color;
  else bg = this->buttoncolor();

  Color fg = this->labelcolor();

  // For back-compatability box() is used if it is directly set and
  // there is no glyph:
  Box* box = style()->box;
  if (!box || glyph_width) box = this->buttonbox();

  Flags flags = this->flags(); // flags to pass to draw_label
  if (!active_r()) {
    flags |= INACTIVE;
  } else if (belowmouse()) {
    flags |= HIGHLIGHT;
    Color h = highlight_color();
    if (h) {
      bg = h;
      fg = highlight_textcolor();
      if (box == NO_BOX) box = FLAT_BOX;
    }
  }

  Flags glyph_flags = flags; // flags to pass to draw_glyph
  if (glyph_width) {
    if (this == held_down) flags |= VALUE;
    if (value()) glyph_flags |= VALUE;
  } else if (value()) {
    flags |= VALUE;
    // Use the pushed-in color if the user has explicitly set it
    // on this widget:
    if (style()->selection_color) {
      bg = style()->selection_color;
      fg = selection_textcolor();
      if (box == NO_BOX) box = FLAT_BOX;
    }
  }

  // only draw "inside" labels:
  bool draw_label = true;
  int x = 0, y = 0, w = this->w(), h = this->h();

  if (box == NO_BOX) {
    // If the box is noBox we need to avoid drawing the label so
    // that it does not blink and does not draw multiple times (which
    // will make it look bold if antialiasing is on).
    if ((damage()&(DAMAGE_EXPOSE|DAMAGE_HIGHLIGHT))) {
      // erase the background behind where the label will draw:
      draw_background();
    } else if (!label()) {
      // Assumme this is a button with an animated image label.
      // we must redraw the image so it changes depending on state.
      // Partially-transparent pixels will not draw right!
    } else {
      // Don't draw the label unnecessarily:
      draw_label = false;
    }
    // this allows these buttons to be put into browser/menus:
    //fg = fl_item_labelcolor(this);
  } else {
    if ((damage()&DAMAGE_EXPOSE) && !box->fills_rectangle()) {
      // Erase the area behind non-square boxes
      draw_background();
    }
    // Draw the box:
    box->draw(0, 0, w, h, bg, flags);
    box->inset(x,y,w,h);
  }

  int lx = x; int lw = w;
  if (glyph_width < 0) {
    int g = -glyph_width;
    draw_glyph(glyph, x+w-g-3, y+((h-g)>>1), g, g, glyph_flags);
    lw = w-g-3;
  } else if (glyph_width > 0) {
    int g = glyph_width;
    draw_glyph(glyph, x+3, y+((h-g)>>1), g, g, glyph_flags);
    lx = x+g+3; lw = w-g-3;
  }

  if (draw_label) {
    // turn on this flag that makes some images draw differently:
    if (focused()) flags |= SELECTED;
    this->draw_label(lx, y, lw, h, fg, flags);
  }

  if (focused())
    focusbox()->draw(x+1, y+1, w-2, h-2, fg, INVISIBLE);
}

void Button::draw() {
  if (type() == HIDDEN) {
    fl_did_clipping = this;
    return;
  }
  draw(0,0);
}

////////////////////////////////////////////////////////////////

static NamedStyle style("Button", 0, &Button::default_style);
NamedStyle* Button::default_style = &::style;

Button::Button(int x,int y,int w,int h, const char *l) : Widget(x,y,w,h,l) {
  style(default_style);
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Button.cxx,v 1.65 2003/09/03 06:08:06 spitzak Exp $".
//
