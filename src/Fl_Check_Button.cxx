//
// "$Id: Fl_Check_Button.cxx,v 1.36 2002/01/28 08:02:59 spitzak Exp $"
//
// Check button widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Fl_Check_Button.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Group.h>

// This is used to draw both check boxes and radio buttons. Radio buttons
// are drawn if the radio type() is set.
void Fl_Check_Button::default_glyph(const Fl_Widget* widget, int glyph,
				    int x,int y,int w,int h, Fl_Flags flags)
{
  bool radio = (widget->type() == Fl_Button::RADIO);
  Fl_Boxtype box = widget->button_box();
  if (radio && box == FL_DOWN_BOX) {
    h = (h+1)&(~1); // even only
    box = FL_ROUND_DOWN_BOX;
  }
  box->draw(x, y, w, h, widget->button_color(), flags);
  box->inset(x, y, w, h);
  if (flags & FL_VALUE) {
    Fl_Color color = (box == FL_NO_BOX && (flags&FL_SELECTED)) ?
      widget->selection_text_color() : widget->text_color();
    fl_color(fl_inactive(color, flags));
    if (!radio) { // checkmark
      x += 1;
      w = h - 2;
      int d1 = w/3;
      int d2 = w-d1;
      y += (h+d2)/2-d1-2;
      for (int n = 0; n < 3; n++, y++) {
	fl_line(x, y, x+d1, y+d1);
	fl_line(x+d1, y+d1, x+w-1, y+d1-d2+1);
      }
    } else { // radio, draw a circular blob
      int d = h/6;
      fl_ellipse(x+d, y+d, h-d-d-1, h-d-d-1);
      fl_fill();
    }
  }
}

void Fl_Check_Button::draw() {
  Fl_Button::draw(0, text_size()+2);
}

int Fl_Check_Button::handle(int event) {
  if (event == FL_RELEASE && !Fl::pushed()) redraw();
  return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
  s->color = FL_GRAY;
  s->button_box = FL_DOWN_BOX;
  s->button_color = FL_WHITE;
  s->glyph = Fl_Check_Button::default_glyph;
}
static Fl_Named_Style style("Check_Button", revert, &Fl_Check_Button::default_style);
Fl_Named_Style* Fl_Check_Button::default_style = &::style;

Fl_Check_Button::Fl_Check_Button(int x, int y, int w, int h, const char *l)
  : Fl_Button(x, y, w, h, l)
{
  style(default_style);
  type(TOGGLE);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
}
