//
// "$Id: Fl_Menu_Button.cxx,v 1.36 2000/05/30 07:42:14 bill Exp $"
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
  Fl_Color lc = draw_button(flags());
  draw_label(lc);
  // draw the little mark at the right:
  int X=x(); int Y=y(); int W=w(); int H=h(); box()->inset(X,Y,W,H);
  int w1 = H*4/5;
  Fl_Flags f = 0;
  Fl_Color gc = (text_box() == FL_NO_BOX) ? lc : text_color();
  if (!active_r()) f |= FL_INACTIVE;
  glyph()(FL_GLYPH_DOWN, X+W-w1, Y, w1, H, text_background(), gc, f, text_box());
}

void Fl_Menu_Button::draw_n_clip() {
  if (!(type()&7))
    Fl_Widget::draw_n_clip();
  else
    box(FL_NO_BOX); 
}


int Fl_Menu_Button::popup() {
  if (box() == FL_NO_BOX) type(POPUP3); // back compatibility hack
  if (type()&7)
    return Fl_Menu_::popup(Fl::event_x(), Fl::event_y(), label());
  else
    return pulldown(x(), y(), w(), h());
}

int Fl_Menu_Button::handle(int e) {
  if (!children()) return 0;
  switch (e) {

  case FL_FOCUS:
  case FL_UNFOCUS:
    if (type()&7) return 0;
    damage(FL_DAMAGE_HIGHLIGHT);
    return 1;

  case FL_ENTER:
  case FL_LEAVE:
    if (type()&7) return 0;
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
  case FL_MOVE:
    return 1;

  case FL_PUSH:
    // If you uncomment this line (or make a subclass that does this) then
    // a mouse click picks the current item, and the menu goes away.  The
    // user must drag the mouse to select a different item.  Depending on
    // the size and usage of the menu, this may be more user-friendly:
    // Fl::event_is_click(0);
    if (type()&7) {
      if (!(type() & (1 << (Fl::event_button()-1)))) return 0;
    } else {
      take_focus();
    }
  EXECUTE:
    if (!(type()&7)) value(-1); // make it pull down below the button...
    popup();
    return 1;

  case FL_SHORTCUT:
    if (Fl_Widget::test_shortcut()) goto EXECUTE;
    return handle_shortcut();

  case FL_KEYBOARD:
    if (Fl::event_key() == FL_Enter || Fl::event_key() == ' ') goto EXECUTE;
    return 0;

  default:
    return 0;
  }
}

static void revert(Fl_Style* s) {
  s->text_box = FL_NO_BOX;
  s->selection_color = FL_GRAY;
  s->selection_text_color = FL_BLACK;
}

static Fl_Named_Style* style = new Fl_Named_Style("Menu_Button", revert, &style);

Fl_Menu_Button::Fl_Menu_Button(int X,int Y,int W,int H,const char *l)
  : Fl_Menu_(X,Y,W,H,l)
{
  style(::style);
  align(FL_ALIGN_CENTER);
}

//
// End of "$Id: Fl_Menu_Button.cxx,v 1.36 2000/05/30 07:42:14 bill Exp $".
//
