//
// "$Id: Fl_Menu_Bar.cxx,v 1.58 2002/12/18 08:34:22 spitzak Exp $"
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
#include <fltk/draw.h>
#include <fltk/Tooltip.h>
#include <config.h>
#include <fltk/Item.h> // for TOGGLE, RADIO
#define checkmark(item) (item->type()>=Item::TOGGLE && item->type()<=Item::RADIO)

using namespace fltk;

void MenuBar::draw() {
  if (damage()&(~DAMAGE_HIGHLIGHT)) draw_box();
  if (!children()) { last_ = -1; return; }
  int x1 = 0; int y1 = 0; int w1 = w(); int h1 = this->h();
  box()->inset(x1,y1,w1,h1);
  int X = 3;
  for (int i = 0; i < children(); i++) {
    Widget* widget = child(i);
    if (!widget->visible()) continue;
    int W = widget->width() + 10;
    if (damage()&(~DAMAGE_HIGHLIGHT) || last_ == i || highlight_ == i) {
      // If you change how the items are drawn, you probably need to
      // change MenuTitle::draw and the functions find_selected and
      // titlex in PopupMenu.cxx.
      if (i == highlight_ && takesevents() && widget->active_r())
	widget->set_flag(HIGHLIGHT);
      else
	widget->clear_flag(HIGHLIGHT);
      widget->clear_flag(SELECTED);
      buttonbox()->draw(X, y1+1, W, h1-2, buttoncolor(), widget->flags()&~VALUE);
      int save_w = widget->w(); widget->w(W-10);
      int save_h = widget->h(); widget->h(h1-2);
      push_matrix();
      translate(X+5, (h()-widget->h())>>1);
      widget->draw();
      pop_matrix();
      widget->w(save_w);
      widget->h(save_h);
    }
    X += W;
  }
  last_ = highlight_;
}

int MenuBar::handle(int event) {
  int children = this->children();
  if (!children) return 0;
  int X = 3;
  int i;
  highlight_ = -1;
  // LEAVE events don't get the right coordinates
  if (event != LEAVE && takesevents()) for (i = 0; i < children; ++i) {
    Widget* widget = child(i);
    if (!widget->visible()) continue;
    int W = widget->width() + 10;
    if (event_inside(X, 0, W, h())) {
      highlight_ = widget->takesevents() ? i : -1;
      break;
    }
    X += W;
  }
  switch (event) {
  case MOVE:
  case ENTER:
  case LEAVE:
    if (highlight_ == last_) return 1;
    Tooltip::exit();
    redraw(DAMAGE_HIGHLIGHT);
    return 1;
  case PUSH:
    if (highlight_ < 0) return 0;
    value(-1);
  J1:
    highlight_ = -1; redraw(DAMAGE_HIGHLIGHT);
    popup(0, 0, w(), h(), 0, true);
    return 1;
  case SHORTCUT:
    // First check against the &x or shortcut() of top-level items:
    for (i = 0; i < children; i++) {
      Widget* w = child(i);
      if (w->active() && w->test_shortcut()) {
	if (w->is_group()) {value(i); goto J1;} // menu title
	execute(w); // button in the menu bar
	if (checkmark(w)) redraw();
	return 1;
      }
    }
    // Now test against the shortcut() of any item in any submenu:
    if (handle_shortcut()) return 1;
    return 0;
  case KEYUP:
    // In the future maybe any shortcut() will work, but for now
    // only the Alt key does. Setting the shortcut to zero will disable
    // the alt key shortcut.
    if (shortcut() != LeftAltKey && shortcut() != RightAltKey) break;
    if (event_key() != LeftAltKey && event_key() != RightAltKey) break;
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
  s->color = GRAY75;
  s->box = FLAT_BOX;
#if 0
  // NT 4.0 style
  s->buttonbox = FLAT_BOX;
#else
  // Windows98 style:
  s->buttonbox = HIGHLIGHT_UP_BOX;
#endif
}
static NamedStyle style("Menu_Bar", revert, &MenuBar::default_style);
NamedStyle* MenuBar::default_style = &::style;

MenuBar::MenuBar(int x,int y,int w,int h,const char *l)
  : Menu(x,y,w,h,l)
{
  style(default_style);
  shortcut(LeftAltKey);
}

//
// End of "$Id: Fl_Menu_Bar.cxx,v 1.58 2002/12/18 08:34:22 spitzak Exp $".
//
