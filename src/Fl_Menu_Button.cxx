// "$Id: Fl_Menu_Button.cxx,v 1.61 2004/08/02 07:35:18 spitzak Exp $"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

/*! \class fltk::PopupMenu

This subclass pops up a menu in response to a user click. The menu is
popped up positioned so that the mouse is pointing at the last-selected
item, even if it in a nested submenu (To turn off this behaivor do
value(-1) after each item is selected)

\image html menu_button.gif

Normally any mouse button will pop up a menu and it is lined up above
the button, or below it when there is no previous selected value as
shown in the picture.

However a PopupMenu can also have type() set to POPUP1, POPUP2,
POPUP12, POPUP3, POPUP13, POPUP23, or POPUP123. It then becomes invisible
and ignores all mouse buttons other than the ones named in the popup
type. You can then resize it to cover another widget (or many widgets)
so that pressing that button pops up the menu.

The menu will also pop up in response to shortcuts indicated by
the shortcut() or by putting '&x' in the label().

Typing the fltk::Widget::shortcut() of any menu items will cause it
to be picked. The callback will be done but there will be no visible
effect to the widget.

*/

#include <fltk/PopupMenu.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/Item.h>

using namespace fltk;

extern Widget* fl_did_clipping;

/*! The little down-arrow indicator can be replaced by setting a new
  glyph() function and making it draw whatever you want.
  If you don't want any glyph at all it is probably easiest to
  subclass and replace draw() with your own function.
*/
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
  Flags flags = current_flags_highlight();
  box->draw(0, 0, this->w(), this->h(), style(), flags);
  int x,y,w,h;
  x = y = 0; w = this->w(); h = this->h(); box->inset(x,y,w,h);
  draw_label(x,y,w,h, style(), flags);
  // draw the little mark at the right:
  int w1 = int(textsize());
  draw_glyph(GLYPH_DOWN, x+w-w1, y, w1, h, flags);
  focusbox()->draw(x+1, y+1, w-2, h-2, style(), flags);
}

/*! Wrapper for Menu::popup(). For NORMAL PopupMenu this places the
  menu over the widget. For POPUP ones it uses the mouse position
  and sets the "title" to the label() if it is not null.
*/
int PopupMenu::popup() {
  if (type()&7) {
    if (label()) {
      return Menu::popup(event_x(), event_y(), 0,0, label());
    } else {
      return Menu::popup(event_x(), event_y());
    }
  } else {
    return Menu::popup(0, 0, w(), h());
  }
}

int PopupMenu::handle(int e) {
  if (box() == NO_BOX) type(POPUP3); // back compatibility hack
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
    redraw_highlight();
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
    if (event_key() == ReturnKey ||
	event_key() == KeypadEnter ||
	event_key() == SpaceKey) goto EXECUTE;
    return 0;

  default:
    return 0;
  }
}

static void revert(Style* s) {
  s->color_ = GRAY75;
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
// End of "$Id: Fl_Menu_Button.cxx,v 1.61 2004/08/02 07:35:18 spitzak Exp $".
//
