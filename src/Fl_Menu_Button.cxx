//
// "$Id: Fl_Menu_Button.cxx,v 1.43 2001/07/24 07:48:23 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Menu_Button.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Group.h>
#include <fltk/Fl_Item.h>
#include <config.h>

void Fl_Menu_Button::draw() {
  Fl_Flags flags = draw_button(0);
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  draw_inside_label(X,Y,W,H,flags);
  // draw the little mark at the right:
  int w1 = H*4/5;
  draw_glyph(FL_GLYPH_DOWN, X+W-w1, Y, w1, H, flags);
}

void Fl_Menu_Button::draw_n_clip() {
  if (!(type()&7)) Fl_Widget::draw_n_clip();
  // else do nothing for POPUP types
}

int Fl_Menu_Button::popup() {
  if (box() == FL_NO_BOX) type(POPUP3); // back compatibility hack
  if (type()&7) {
    if (label()) {
      Fl_Item title(label());
      return Fl_Menu_::popup(Fl::event_x(), Fl::event_y(), 0,0, &title);
    } else {
      return Fl_Menu_::popup(Fl::event_x(), Fl::event_y());
    }
  } else {
    return Fl_Menu_::popup(0, 0, w(), h());
  }
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
#if CLICK_MOVES_FOCUS
      take_focus();
#endif
    }
  EXECUTE:
    if (!(type()&7)) value(-1); // make it pull down below the button...
    popup();
    return 1;

  case FL_SHORTCUT:
    if (test_shortcut()) goto EXECUTE;
    return handle_shortcut();

  case FL_KEYBOARD:
    if (Fl::event_key() == FL_Enter || Fl::event_key() == ' ') goto EXECUTE;
    return 0;

  default:
    return 0;
  }
}

static void revert(Fl_Style* s) {
  s->text_box = FL_UP_BOX; // box drawn around title
  s->text_background = FL_GRAY; // controls color of the menu
  s->leading = 4;
}
static Fl_Named_Style style("Menu_Button", revert, &Fl_Menu_Button::default_style);
Fl_Named_Style* Fl_Menu_Button::default_style = &::style;

Fl_Menu_Button::Fl_Menu_Button(int X,int Y,int W,int H,const char *l)
  : Fl_Menu_(X,Y,W,H,l)
{
  style(default_style);
  align(FL_ALIGN_CENTER);
}

//
// End of "$Id: Fl_Menu_Button.cxx,v 1.43 2001/07/24 07:48:23 spitzak Exp $".
//
