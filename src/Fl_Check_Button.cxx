//
// "$Id: Fl_Check_Button.cxx,v 1.35 2002/01/20 07:37:15 spitzak Exp $"
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

void Fl_Check_Button::draw() {
  // Draw the outer box as though it were a button:
  int x,y,w,h;
  Fl_Flags flags = draw_as_button(pushed()?FL_VALUE:0, x,y,w,h);
  flags = (flags&~FL_VALUE)|(this->flags()&FL_VALUE);
  // Draw the check box:
  int W = h;
  int d = h/6;
  draw_inside_label(x+W-d, y, w-W+d, h, flags);
  int X = d; int Y = d+1; W = W-2*d-2;
  if (X+W > w) X = 0;
  if (flags&FL_VALUE) flags &= ~FL_HIGHLIGHT; // don't highlight selected buttons
  draw_glyph(shape, x+X, y+Y, W, W, flags);
}

int Fl_Check_Button::handle(int event) {
  if (event == FL_RELEASE && !Fl::pushed()) redraw();
  return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
}
static Fl_Named_Style style("Check_Button", revert, &Fl_Check_Button::default_style);
Fl_Named_Style* Fl_Check_Button::default_style = &::style;

Fl_Check_Button::Fl_Check_Button(int x, int y, int w, int h, const char *l)
  : Fl_Button(x, y, w, h, l)
{
  style(default_style);
  type(TOGGLE);
  shape = FL_GLYPH_CHECK;
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
}
