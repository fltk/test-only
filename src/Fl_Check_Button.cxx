//
// "$Id: Fl_Check_Button.cxx,v 1.32 2001/01/02 00:20:28 clip Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>

void Fl_Check_Button::draw() {
  // Draw the outer box as though it were a button:
  Fl_Flags flags = draw_button(pushed()?FL_VALUE:0);
  flags = (flags&~FL_VALUE)|(this->flags()&FL_VALUE);
  // Draw the check box:
  int W = h();
  int d = h()/6;
  draw_inside_label(x()+W-d, y(), w()-W+d, h(), flags);
  int X = x()+d; int Y = y()+d+1; W = W-2*d-2;
  if (X+W > x()+w()) X = x();
  if (flags&FL_VALUE) flags &= ~FL_HIGHLIGHT; // don't highlight selected buttons
  draw_glyph(shape, X, Y, W, W, flags);
}

int Fl_Check_Button::handle(int event) {
  if (event == FL_RELEASE && !Fl::pushed()) redraw();
  return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
}

static Fl_Named_Style* style = new Fl_Named_Style("Check Button", revert, &style);

Fl_Check_Button::Fl_Check_Button(int x, int y, int w, int h, const char *l)
  : Fl_Button(x, y, w, h, l)
{
  style(::style);
  type(FL_TOGGLE_BUTTON);
  shape = FL_GLYPH_CHECK;
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
}
