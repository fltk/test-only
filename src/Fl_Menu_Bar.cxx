//
// "$Id: Fl_Menu_Bar.cxx,v 1.71 2004/05/18 15:53:40 spitzak Exp $"
//
// Menu bar widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/MenuBar.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/Item.h>
#include <fltk/draw.h>
#include <fltk/Tooltip.h>
#include <config.h>
#include <fltk/Item.h> // for TOGGLE, RADIO
#define checkmark(item) (item->type()>=Item::TOGGLE && item->type()<=Item::RADIO)

using namespace fltk;
extern bool fl_hide_shortcut;

void MenuBar::draw() {
  Menu::draw(this, 0, 0, highlight_, last_);
  last_ = highlight_;
}

int MenuBar::handle(int event) {
  int children = this->children();
  if (!children) return 0;
  int i;
  switch (event) {
  case LEAVE:
    highlight_ = -1;
    goto J2;
  case MOVE:
  case ENTER:
    highlight_ = Menu::find_selected(this,0,0,event_x(), event_y());
    if (highlight_ >= 0) {
      const char* c = child(highlight_)->tooltip();
      if (c) {
	int x,y,w,h; Menu::get_location(this,0,0,highlight_,x,y,w,h);
	Tooltip::enter(this, x,y,w,h, c);
      } else {
	Tooltip::exit();
      }
    } else {
      Tooltip::exit();
    }
  J2:
    if (highlight_ == last_) return 1;
    redraw(DAMAGE_CHILD);
    return 1;
  case PUSH:
    if (highlight_ < 0) return 0;
    value(-1);
  J1:
    highlight_ = -1; redraw(DAMAGE_CHILD);
    popup(0, 0, w(), h(), 0, true);
    return 1;
  case SHORTCUT:
    // Test against the shortcut() of any item in any submenu:
    if (handle_shortcut()) return 1;
    // Check against the &x of top-level items:
    if (event_state(ALT|META)) for (i = 0; i < children; i++) {
      Widget* w = child(i);
      if (w->active() && w->test_label_shortcut()) {
	if (w->is_group()) {value(i); goto J1;} // menu title
  	execute(w); // button in the menu bar
//  	if (checkmark(w)) redraw();
  	return 1;
      }
    }
    if (style()->hide_shortcut() &&
	!event_clicks() &&
	(event_key() == LeftAltKey || event_key() == RightAltKey)) redraw();
    return 0;
  case KEYUP:
    // In the future maybe any shortcut() will work, but for now
    // only the Alt key does. Setting the shortcut to zero will disable
    // the alt key shortcut.
    if (event_key() != LeftAltKey && event_key() != RightAltKey) break;
    if (style()->hide_shortcut()) redraw();
    if (shortcut() != LeftAltKey && shortcut() != RightAltKey) break;
    // checking for event_clicks insures that the keyup matches the
    // keydown that preceeded it, so Alt was pressed & released without
    // any intermediate values.  On X it is false if Alt is held down
    // for a long time, too:
    if (!event_is_click()) break;
    // okay we got the shortcut, find first menu and pop it up:
    for (i = 0; i < children; i++) {
      Widget* w = child(i);
      if (w->active()) {value(i); goto J1;}
    }
  }
  return 0;
}

// The default style for menu bars.  The buttonbox() is used to draw
// the boxes around the popup titles, this is done by PopupMenu.cxx, and
// done here for highlight boxes.

static void revert(Style* s) {
  s->color_ = GRAY75;
  s->box_ = FLAT_BOX;
  s->leading_ = 4;
#if 0
  // NT 4.0 style
  s->buttonbox_ = FLAT_BOX;
#else
  // Windows98 style:
  s->buttonbox_ = HIGHLIGHT_UP_BOX;
#endif
}
static NamedStyle style("MenuBar", revert, &MenuBar::default_style);
NamedStyle* MenuBar::default_style = &::style;

MenuBar::MenuBar(int x,int y,int w,int h,const char *l)
  : Menu(x,y,w,h,l)
{
  style(default_style);
  shortcut(LeftAltKey);
}

//
// End of "$Id: Fl_Menu_Bar.cxx,v 1.71 2004/05/18 15:53:40 spitzak Exp $".
//
