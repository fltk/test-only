//
// "$Id: Fl_Light_Button.cxx,v 1.40 2005/01/24 08:07:25 spitzak Exp $"
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

#include <fltk/LightButton.h>
#include <fltk/Box.h>

using namespace fltk;

/*! \class fltk::LightButton

  This button turns the value() on and off each release of a click
  inside of it, and displays a light to show the user this. This style
  was designed for the original Forms library and has been copied
  into Discreet products and many other pieces of software that
  originated with SGI machines:

  \image html Fl_Light_Button.gif

  When off the light is color(), when on the light is selection_color().
*/

static void glyph(int/*t*/, const Rectangle& r, const Style* style, Flags f)
{
  if (f & VALUE) {
    f |= SELECTED;
  } else if (style->color_) {
    // if user set the color of the button draw that color
    f |= OUTPUT;
  } else {
    f |= INVISIBLE; // draw frame only
  }
  int ww = r.w()/2+1; // we will center box of this width in area
  THIN_DOWN_BOX->draw(Rectangle(r, ww, r.h()), style, f);
}

static void revert(Style* s) {
  s->selection_color_ = YELLOW;
  s->glyph_ = glyph;
}
static NamedStyle style("Light_Button", revert, &LightButton::default_style);
NamedStyle* LightButton::default_style = &::style;

LightButton::LightButton(int x, int y, int w, int h, const char *l)
  : CheckButton(x, y, w, h, l)
{
  default_style->parent_ = Button::default_style;
  style(default_style);
}

//
// End of "$Id: Fl_Light_Button.cxx,v 1.40 2005/01/24 08:07:25 spitzak Exp $".
//
