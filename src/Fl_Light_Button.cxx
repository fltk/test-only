//
// "$Id: Fl_Light_Button.cxx,v 1.33 2002/12/09 04:52:25 spitzak Exp $"
//
// Lighted button widget for the Fast Light Tool Kit (FLTK).
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

// The LightButton draws the indicator as a rectangle in the
// style of Flame.  This is done by replacing the glyph drawing function
// in the default style.

#include <fltk/LightButton.h>
#include <fltk/Box.h>

using namespace fltk;

static void glyph(const Widget* widget, int/*t*/,
		  int x,int y,int w,int h, Flags f)
{
  int ww = w/2+1;
  Color color;
  if (f & VALUE) {
    f = f&INACTIVE;
    color = widget->selection_color();
  } else if ((color = widget->style()->buttoncolor)) {
    // if user set the color of the button draw that color
    f = f&INACTIVE;
  } else {
    f = f&INACTIVE | INVISIBLE; // draw frame only
    color = widget->color();
  }
  THIN_DOWN_BOX->draw(x+((w-ww)>>1), y, ww, h, color, f);
}

static void revert(Style* s) {
  s->selection_color = YELLOW;
  s->glyph = glyph;
}
static NamedStyle style("Light_Button", revert, &LightButton::default_style);
NamedStyle* LightButton::default_style = &::style;

LightButton::LightButton(int x, int y, int w, int h, const char *l)
  : CheckButton(x, y, w, h, l)
{
  default_style->parent = Button::default_style;
  style(default_style);
}

//
// End of "$Id: Fl_Light_Button.cxx,v 1.33 2002/12/09 04:52:25 spitzak Exp $".
//
