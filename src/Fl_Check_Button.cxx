//
// "$Id: Fl_Check_Button.cxx,v 1.24 2000/02/14 11:32:47 bill Exp $"
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
  // Draw the outer box as though it was a button:
  Fl_Flags f = flags();
  Fl_Color c = color();
  Fl_Color lc = label_color();
  if (!active_r()) {
    f |= FL_INACTIVE;
  } else if (belowmouse() && highlight_color()) {
    c = highlight_color();
    lc = highlight_label_color();
    f |= FL_HIGHLIGHT;
  }
  if (focused()) f |= FL_FOCUSED;
  // We need to erase the focus rectangle for FL_NO_BOX buttons, such
  // as checkmarks:
  if (!(f&FL_FOCUSED) && box()==FL_NO_BOX && (damage()&FL_DAMAGE_HIGHLIGHT)) {
    fl_clip(x(), y(), w(), h());
    parent()->draw_group_box();
    fl_pop_clip();
  }
  box()->draw(x(), y(), w(), h(), c, pushed() ? (f|FL_VALUE) : (f&~FL_VALUE));

  // Draw the check box:
  int d = h()/6;
  int W = (w()<h() ? w() : h()) - 2*d - 2;
  glyph()(type()==FL_RADIO_BUTTON ? FL_GLYPH_RADIO : FL_GLYPH_CHECK,
	  x()+d, y()+d+1, W, W,
	  off_color(), (f&FL_VALUE) ? selection_color() : off_color(),
	  f, glyph_box());

  draw_button_label(x()+W+d, y(), w()-W-d, h(), lc);
}

int Fl_Check_Button::handle(int event) {
  if (event == FL_RELEASE && !Fl::pushed()) redraw();
  return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
  s->glyph_box = FL_DOWN_BOX;
  s->selection_color = FL_BLACK;
  s->off_color = FL_WHITE;
  s->glyph = fl_glyph;
}

static Fl_Named_Style* style = new Fl_Named_Style("Check_Button", revert, &style);

Fl_Check_Button::Fl_Check_Button(int x, int y, int w, int h, const char *l)
  : Fl_Button(x, y, w, h, l)
{
  style(::style);
  type(FL_TOGGLE_BUTTON);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
}
