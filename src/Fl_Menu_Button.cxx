//
// "$Id: Fl_Menu_Button.cxx,v 1.23 1999/11/19 10:06:48 bill Exp $"
//
// Menu button widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>

void Fl_Menu_Button::draw() {
  draw_button();
  draw_label();
  // draw the little mark at the right:
  int X=x(); int Y=y(); int W=w(); int H=h(); box()->inset(X,Y,W,H);
  H = H-4;
  X = X+W-H-2;
  Y = Y+2;
  Fl_Flags f = active_r() ? Fl::belowmouse() == this ? FL_HIGHLIGHT : 0
                            : FL_INACTIVE;
  draw_glyph(FL_GLYPH_DOWN, X, Y, H, H, f);
}

void Fl_Menu_Button::draw_n_clip() {
  if (!type())
    Fl_Widget::draw_n_clip();
  else
    box(FL_NO_BOX); 
}


const Fl_Menu_Item* Fl_Menu_Button::popup() {
  const Fl_Menu_Item* m;

// back compatibility hack
  if (box() == FL_NO_BOX) type(POPUP3);

  if (type()) {
    m = menu()->popup(Fl::event_x(), Fl::event_y(), label(), mvalue(), this);
  } else {
    m = menu()->pulldown(x(), y(), w(), h(), 0, this);
  }
  picked(m);
  return m;
}

int Fl_Menu_Button::handle(int e) {
  if (!menu() || !menu()->text) return 0;
  switch (e) {
  case FL_ENTER:
  case FL_LEAVE:
    if (box() && !type()) {
      if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
      return 1;
    }
    return 0;
  case FL_PUSH:
    if (box() == FL_NO_BOX) {
      if (Fl::event_button() != 3) return 0;
    } else if (type()) {
      if (!(type() & (1 << (Fl::event_button()-1)))) return 0;
    }
    popup();
    return 1;
  case FL_SHORTCUT:
    if (Fl_Widget::test_shortcut()) {popup(); return 1;}
    return test_shortcut() != 0;
  default:
    return 0;
  }
}

Fl_Menu_Button::Fl_Menu_Button(int X,int Y,int W,int H,const char *l)
  : Fl_Menu_(X,Y,W,H,l)
{
  style(default_style);
}

static void revert(Fl_Style* s) {
  s->glyph_box = FL_NO_BOX;
}

Fl_Style Fl_Menu_Button::default_style("Menu_Button", revert);

//
// End of "$Id: Fl_Menu_Button.cxx,v 1.23 1999/11/19 10:06:48 bill Exp $".
//
