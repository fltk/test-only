//
// "$Id: Fl_Radio_Button.cxx,v 1.5 2003/02/02 10:39:23 spitzak Exp $"
//
// Radio button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/RadioButton.h>
#include <fltk/draw.h>
#include <fltk/Box.h>
using namespace fltk;

static void default_glyph(const Widget* widget, int glyph,
			  int x,int y,int w,int h, Flags flags)
{
  // h = (h+1)&(~1); // even only
  Box* box = widget->box();
  box->draw(x, y, w, h, widget->color(), flags);
  box->inset(x, y, w, h);
  if (flags & VALUE) {
    Color color = (box == NO_BOX && (flags&SELECTED)) ?
      widget->selection_textcolor() : widget->textcolor();
    setcolor(inactive(color, flags));
    int d = h/6;
    addellipse(x+d, y+d, h-d-d-1, h-d-d-1);
    fillpath();
  }
}

static void revert(Style* s) {
  s->buttonbox = NO_BOX;
  s->box = ROUND_DOWN_BOX;
  s->glyph = ::default_glyph;
}
static NamedStyle style("Radio_Button", revert, &RadioButton::default_style);
NamedStyle* RadioButton::default_style = &::style;

RadioButton::RadioButton(int x, int y, int w, int h, const char *l)
  : CheckButton(x, y, w, h, l)
{
  style(default_style);
  type(RADIO);
}
