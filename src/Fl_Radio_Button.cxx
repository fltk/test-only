//
// "$Id: Fl_Radio_Button.cxx,v 1.1 2002/02/10 22:57:48 spitzak Exp $"
//
// Radio button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Radio_Button.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Group.h>

static void default_glyph(const Fl_Widget* widget, int glyph,
			  int x,int y,int w,int h, Fl_Flags flags)
{
  // h = (h+1)&(~1); // even only
  Fl_Boxtype box = widget->button_box();
  box->draw(x, y, w, h, widget->button_color(), flags);
  box->inset(x, y, w, h);
  if (flags & FL_VALUE) {
    Fl_Color color = (box == FL_NO_BOX && (flags&FL_SELECTED)) ?
      widget->selection_text_color() : widget->text_color();
    fl_color(fl_inactive(color, flags));
    int d = h/6;
    fl_ellipse(x+d, y+d, h-d-d-1, h-d-d-1);
    fl_fill();
  }
}

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
  s->color = FL_GRAY;
  s->button_box = FL_ROUND_DOWN_BOX;
  s->button_color = FL_WHITE;
  s->glyph = ::default_glyph;
}
static Fl_Named_Style style("Radio_Button", revert, &Fl_Radio_Button::default_style);
Fl_Named_Style* Fl_Radio_Button::default_style = &::style;

Fl_Radio_Button::Fl_Radio_Button(int x, int y, int w, int h, const char *l)
  : Fl_Check_Button(x, y, w, h, l)
{
  style(default_style);
  type(RADIO);
}
