//
// "$Id: Fl_Light_Button.cxx,v 1.32 2002/09/09 01:39:57 spitzak Exp $"
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

// The Fl_Light_Button draws the indicator as a rectangle in the
// style of Flame.  This is done by replacing the glyph drawing function
// in the default style.

#include <fltk/Fl.h>
#include <fltk/Fl_Light_Button.h>

static void glyph(const Fl_Widget* widget, int/*t*/,
		  int x,int y,int w,int h, Fl_Flags f)
{
  int ww = w/2+1;
  Fl_Color color;
  if (f & FL_VALUE) {
    f = f&FL_INACTIVE;
    color = widget->selection_color();
  } else if ((color = widget->style()->button_color)) {
    // if user set the color of the button draw that color
    f = f&FL_INACTIVE;
  } else {
    f = f&FL_INACTIVE | FL_INVISIBLE; // draw frame only
    color = widget->color();
  }
  FL_THIN_DOWN_BOX->draw(x+((w-ww)>>1), y, ww, h, color, f);
}

static void revert(Fl_Style* s) {
  s->selection_color = FL_YELLOW;
  s->glyph = glyph;
}
static Fl_Named_Style style("Light_Button", revert, &Fl_Light_Button::default_style);
Fl_Named_Style* Fl_Light_Button::default_style = &::style;

Fl_Light_Button::Fl_Light_Button(int x, int y, int w, int h, const char *l)
  : Fl_Check_Button(x, y, w, h, l)
{
  default_style->parent = Fl_Button::default_style;
  style(default_style);
}

//
// End of "$Id: Fl_Light_Button.cxx,v 1.32 2002/09/09 01:39:57 spitzak Exp $".
//
