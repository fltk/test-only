//
// "$Id: Fl_Choice.cxx,v 1.38 2000/01/23 01:38:20 bill Exp $"
//
// Choice widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Choice.H>
#include <FL/fl_draw.H>

extern char fl_draw_shortcut;

// The dimensions for the glyph in this and the Fl_Menu_Button are exactly
// the same, so that glyphs may be shared between them.

void Fl_Choice::draw() {
  draw_box();
  int X=x(); int Y=y(); int W=w(); int H=h(); box()->inset(X,Y,W,H);
  int w1 = H*4/5;
  if (mvalue()) {
    Fl_Menu_Item m = *mvalue();
    if (active_r()) m.activate(); else m.deactivate();
    fl_clip(X+2, Y, W-w1-2, H);
    fl_draw_shortcut = 2; // hack value to make '&' disappear
    m.draw(X, Y+2, W-w1-2, H-4, Fl::focus()==this ? 1 : 5);
    fl_draw_shortcut = 0;
    fl_pop_clip();
  }
  // draw the little mark at the right:
  draw_glyph(FL_GLYPH_DOWN, X+W-w1, Y, w1, H, Fl::belowmouse()==this?FL_HIGHLIGHT:0);
}

int Fl_Choice::value(int v) {
  if (!Fl_Menu_::value(v)) return 0;
  redraw();
  return 1;
}

int Fl_Choice::handle(int e) {
  if (!menu() || !menu()->text) return 0;
  const Fl_Menu_Item* v;
  switch (e) {

  case FL_FOCUS:
  case FL_UNFOCUS:
    damage(FL_DAMAGE_HIGHLIGHT);
    return 1;

  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;

  case FL_PUSH:
    // If you uncomment this line (or make a subclass that does this) then
    // a mouse click picks the current item, and the menu goes away.  The
    // user must drag the mouse to select a different item.  Depending on
    // the size and usage of the menu, this may be more user-friendly:
    // Fl::event_is_click(0);
    if (Fl::focus() != this && Fl::event_x() < x()+w()-h()*4/5) {
      take_focus();
      return 1;
    }
    take_focus();
  J1:
    v = menu()->pulldown(x(), y(), w(), h(), mvalue(), this);
    if (!v || v->submenu()) return 1;
    if (v != mvalue()) redraw();
    picked(v);
    return 1;

  case FL_SHORTCUT:
    if (Fl_Widget::test_shortcut()) goto J1;
    v = menu()->test_shortcut();
    if (!v) return 0;
  J2:
    if (v != mvalue()) redraw();
    picked(v);
    return 1;

  case FL_KEYBOARD:
    if (Fl::event_key() == ' ') goto J1;
    if (Fl::event_key() == FL_Up) {
      v = mvalue(); if (!v) v = menu()+size();
      while (v > menu()) {v--; if (v->text && !v->submenu()) goto J2;}
      return 1;
    }
    if (Fl::event_key() == FL_Down) {
      v = mvalue(); if (!v) {v = menu(); goto J2;}
      while (v->text) {v++; if (v->text && !v->submenu()) goto J2;}
      return 1;
    }
    return 0;

  default:
    return 0;
  }
}

static void revert(Fl_Style* s) {
  s->box = FL_DOWN_BOX;
  s->color = FL_WHITE;
  s->selection_color = FL_BLUE_SELECTION_COLOR;
}

static Fl_Named_Style* style = new Fl_Named_Style("Choice", revert, &style);

Fl_Choice::Fl_Choice(int x,int y,int w,int h, const char *l) : Fl_Menu_(x,y,w,h,l) {
  style(::style);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT);
  when(FL_WHEN_RELEASE);
}

//
// End of "$Id: Fl_Choice.cxx,v 1.38 2000/01/23 01:38:20 bill Exp $".
//
