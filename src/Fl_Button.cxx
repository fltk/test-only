//
// "$Id: Fl_Button.cxx,v 1.59 2002/12/10 02:00:39 easysw Exp $"
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
  case UNFOCUS:
    redraw(DAMAGE_HIGHLIGHT);
    // grab initial focus if we are an ReturnButton:
    return shortcut()=='\r' ? 2 : 1;
  case KEY:
    if (event_key() == ' ') goto EXECUTE;
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
  // Figure out the colors to use. The flags are used by the label and
  // glyph functions to figure out their colors:
  Flags flags;
  Color color;
  if (!active_r()) {
    flags = INACTIVE;
    color = this->color();
  } else if (belowmouse()) {
    flags = HIGHLIGHT;
    color = highlight_color();
    if (!color) color = this->color();
  } else {
    flags = 0;
    color = this->color();
  }
  Flags glyph_flags = flags;
  if (glyph_width) {
    if (this == held_down) flags |= VALUE;
    if (value()) glyph_flags |= VALUE;
  } else if (value()) {
    flags |= VALUE;
    // Use the pushed-in color if the user has explicitly set it
    // on this widget:
    if (style()->selection_color) {
      color = style()->selection_color;
      flags |= SELECTED; // this makes label use selected_textcolor()
    }
  }

  bool draw_label = true;
  int x = 0, y = 0, w = this->w(), h = this->h();
  Box* box = this->box();

  if (box == NO_BOX) {
    // If the box is noBox we need to avoid drawing the label so
    // that it does not blink and does not draw multiple times (which
    // will make it look bold if antialiasing is on).
    /* if (!label()) {
      // don't do anything if no label, so buttons that are an image
      // only will redraw correctly and with minimum blinking.
      } else */
    if ((damage()&DAMAGE_EXPOSE) ||
	(damage()&DAMAGE_HIGHLIGHT) && !focused()) {
      // erase the background behind where the label will draw:
      draw_background();
    } else {
      // Don't draw the label unnecessarily:
      draw_label = false;
    }
  } else {
    if ((damage()&DAMAGE_EXPOSE) && !box->fills_rectangle()) {
      // Erase the area behind non-square boxes
      draw_background();
    }
    // Draw the box:
    box->draw(0, 0, w, h, color, flags);
    box->inset(x,y,w,h);
  }

  if (glyph_width < 0) {
    int g = -glyph_width;
    draw_glyph(glyph, x+w-g-3, y+((h-g)>>1), g, g, glyph_flags);
    if (draw_label) draw_inside_label(x, y, w-g-3, h, flags);
  } else if (glyph_width > 0) {
    int g = glyph_width;
    draw_glyph(glyph, x+3, y+((h-g)>>1), g, g, glyph_flags);
    if (draw_label) draw_inside_label(x+g+3, y, w-g-3, h, flags);
  } else {
    if (draw_label) draw_inside_label(x, y, w, h, flags);
  }

  if (focused()) {
    focusbox()->draw(x+1, y+1, w-2, h-2,
		      flags&SELECTED ? selection_textcolor():labelcolor(),
		      INVISIBLE);
  }
}

void Button::draw() {
  if (type() == HIDDEN) {
    fl_did_clipping = this;
    return;
  }
  draw(0,0);
}

////////////////////////////////////////////////////////////////

static void revert(Style* s) {
  s->color = GRAY75;
  s->box = UP_BOX;
}

static NamedStyle style("Button", revert, &Button::default_style);
NamedStyle* Button::default_style = &::style;

Button::Button(int x,int y,int w,int h, const char *l) : Widget(x,y,w,h,l) {
  style(default_style);
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Button.cxx,v 1.59 2002/12/10 02:00:39 easysw Exp $".
//
