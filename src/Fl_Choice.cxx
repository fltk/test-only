//
// "$Id: Fl_Choice.cxx,v 1.35 2000/01/10 06:31:18 bill Exp $"
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

// This is almost exactly the same as an Fl_Menu_Button.  The only
// difference is the appearance of the button: it draws the text of
// the current pick and a motif-like box.

static void glyph(int/*t*/, int x,int y,int w,int h, Fl_Color bc, Fl_Color,
		  Fl_Flags f, Fl_Boxtype box)
{
  int H = h/2;
  int Y = y + (h-H)/2;
  box->draw(x,Y,w,H, bc, f);
}

extern char fl_draw_shortcut;

void Fl_Choice::draw() {
  draw_button();
  int X=x(); int Y=y(); int W=w(); int H=h(); box()->inset(X,Y,W,H);
  if (mvalue()) {
    Fl_Menu_Item m = *mvalue();
    if (active_r()) m.activate(); else m.deactivate();
    fl_clip(X, Y, W-H-2, H);
    fl_draw_shortcut = 2; // hack value to make '&' disappear
    m.draw(X, Y, W-H-2, H, 5);
    fl_draw_shortcut = 0;
    fl_pop_clip();
  }
  // draw the little mark at the right:
  H = H-4;
  X = X+W-H-2;
  Y = Y+2;
  draw_glyph(FL_GLYPH_DOWN, X, Y, H, H, Fl::belowmouse()==this?FL_HIGHLIGHT:0);
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
    //Fl::event_is_click(0);
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
    if (v != mvalue()) redraw();
    picked(v);
    return 1;

  case FL_KEYBOARD:
    if (Fl::event_key() == ' ') goto J1;
    return 0;

  default:
    return 0;
  }
}


static void revert(Fl_Style* s) {
  //s->glyph_box = FL_FLAT_BOX;
  s->glyph = glyph;
}

static Fl_Named_Style* style = new Fl_Named_Style("Choice", revert, &style);

Fl_Choice::Fl_Choice(int x,int y,int w,int h, const char *l) : Fl_Menu_(x,y,w,h,l) {
  style(::style);
  align(FL_ALIGN_LEFT);
  when(FL_WHEN_RELEASE);
}

//
// End of "$Id: Fl_Choice.cxx,v 1.35 2000/01/10 06:31:18 bill Exp $".
//
