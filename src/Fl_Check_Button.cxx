//
// "$Id: Fl_Check_Button.cxx,v 1.28 2000/07/12 01:39:07 clip Exp $"
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
  Fl_Flags f = flags();
  // Draw the outer box as though it were a button:
  Fl_Color lc = draw_button(pushed() ? (f|FL_VALUE) : (f&~FL_VALUE));

  // Draw the check box:
  int d = h()/6;
  int W = (w()<h() ? w() : h()) - 2*d - 2;
  Fl_Color gc = (text_box() == FL_NO_BOX) ? lc : text_color();
  if (!active_r()) f |= FL_INACTIVE;
  glyph()(/* type() == FL_RADIO_BUTTON ? FL_GLYPH_ROUND : */ shape,
          x()+d, y()+d+1, W, W, text_background(), gc, f, text_box());

  draw_button_label(x()+W+d, y(), w()-W-d, h(), lc);
}

int Fl_Check_Button::handle(int event) {
  if (event == FL_RELEASE && !Fl::pushed()) redraw();
  return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
  s->selection_color = FL_GRAY;
  s->selection_text_color = FL_BLACK;
  s->text_color = FL_BLACK;
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
