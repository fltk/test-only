//
// "$Id: Fl_Menu_Button.cxx,v 1.55 2003/09/03 06:08:06 spitzak Exp $"
//
// Menu button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/PopupMenu.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/Item.h>

using namespace fltk;

extern Widget* fl_did_clipping;

void PopupMenu::draw() {
  if (type()&7) { // draw nothing for the popup types
    fl_did_clipping = this;
    return;
  }
  Box* box /*= style()->box; if (!box) box*/ = this->buttonbox();
  // We need to erase the focus rectangle on DAMAGE_HIGHTLIGHT for
  // NO_BOX buttons such as checkmarks:
  if (damage()&DAMAGE_EXPOSE && !box->fills_rectangle()
      || box == NO_BOX && damage()&DAMAGE_HIGHLIGHT && !focused()) {
    draw_background();
  }
  Flags flags = this->flags();
  Color color = this->buttoncolor();
  Color labelcolor = this->labelcolor();
  if (!active_r()) {
    flags |= INACTIVE;
  } else if (belowmouse()) {
    flags |= HIGHLIGHT;
    Color h = highlight_color();
    if (h) {color = h; labelcolor = highlight_textcolor();}
  }
  box->draw(0, 0, this->w(), this->h(), color, flags);
  int x,y,w,h;
  x = y = 0; w = this->w(); h = this->h(); box->inset(x,y,w,h);
  if (!(flags&15) || (flags & ALIGN_INSIDE)) {
    if (w > 11 && (flags&(ALIGN_LEFT|ALIGN_RIGHT))) {x += 3; w -= 6;}
    draw_label(x,y,w,h, labelcolor, flags);
  }
  if (focused())
    focusbox()->draw(x+1, y+1, w-2, h-2, labelcolor, INVISIBLE);
  // draw the little mark at the right:
  int w1 = int(textsize());
  draw_glyph(GLYPH_DOWN, x+w-w1, y, w1, h, flags);
}

int PopupMenu::popup() {
  if (box() == NO_BOX) type(POPUP3); // back compatibility hack
  if (type()&7) {
    if (label()) {
      Item title(label());
      return Menu::popup(event_x(), event_y(), 0,0, &title);
    } else {
      return Menu::popup(event_x(), event_y());
    }
  } else {
    return Menu::popup(0, 0, w(), h());
  }
}

int PopupMenu::handle(int e) {
  if (!children()) return 0;
  switch (e) {

  case FOCUS:
  case UNFOCUS:
    if (type()&7) return 0;
    redraw(DAMAGE_HIGHLIGHT);
    return 1;

  case ENTER:
  case LEAVE:
    if (type()&7) return 0;
    if (highlight_color() && takesevents()) redraw(DAMAGE_HIGHLIGHT);
  case MOVE:
    return 1;

  case PUSH:
    // If you uncomment this line (or make a subclass that does this) then
    // a mouse click picks the current item, and the menu goes away.  The
    // user must drag the mouse to select a different item.  Depending on
    // the size and usage of the menu, this may be more user-friendly:
    // event_is_click(0);
    if (type()&7) {
      if (!(type() & (1 << (event_button()-1)))) return 0;
    } else {
      if (click_to_focus()) take_focus();
    }
  EXECUTE:
    //if (!(type()&7)) value(-1); // make it pull down below the button...
    popup();
    return 1;

  case SHORTCUT:
    if (test_shortcut()) goto EXECUTE;
    return handle_shortcut();

  case KEY:
    if (event_key() == ReturnKey || event_key() == SpaceKey) goto EXECUTE;
    return 0;

  default:
    return 0;
  }
}

static void revert(Style* s) {
  s->color = GRAY75;
//    s->box = UP_BOX;
}
static NamedStyle style("PopupMenu", revert, &PopupMenu::default_style);
NamedStyle* PopupMenu::default_style = &::style;

PopupMenu::PopupMenu(int X,int Y,int W,int H,const char *l)
  : Menu(X,Y,W,H,l)
{
  style(default_style);
  align(ALIGN_CENTER);
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Menu_Button.cxx,v 1.55 2003/09/03 06:08:06 spitzak Exp $".
//
