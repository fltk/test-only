//
// "$Id: Fl_Menu.cxx,v 1.146 2003/11/04 08:11:00 spitzak Exp $"
//
// Implementation of popup menus.  These are called by using the
// Menu::popup and Menu::pulldown methods.  See also the
// Menu base class in Menu.cxx.  Warning: this code is
// quite a mess!  Also, the classes defined here are private and
// may change in the future!
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#include <fltk/MenuWindow.h>
#include <fltk/Menu.h>
#include <fltk/Box.h>
#include <fltk/LabelType.h>
#include <fltk/draw.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/run.h>
#include <fltk/Monitor.h>
#include <ctype.h>

#include <fltk/Item.h> // for TOGGLE, RADIO
#define checkmark(item) (item->type()>=Item::TOGGLE && item->type()<=Item::RADIO)

using namespace fltk;

// class MenuTitle is a small window for the title, it will look like
// it is being drawn in the menu bar, but is actually a window
// floating above it (this was a rather clever trick SGI came up with
// and makes the menus faster if backing store works on the X server).

// class MenuWindow is the larger window containing all the menu
// items.  It is NOT the Menu itself, instead it has a pointer
// called "list" that points at it.  This contains a lot of ugly code
// to draw the menu items and to draw arrows for parents and draw
// shortcuts.

// The rest of the code is the run-time system for Menu::popup.
// Menu::popup does not return until the user has selected an item
// and all the widgets created here are destroyed.

// grab() is done to send all events to our own handle function
// The entire menu "state" is stuck into a structure passed to the
// handle mentod.  The handle() method changes variables in this state
// to indicate what item is picked, but does not actually alter the
// display, instead the main loop does that.  This is because the X
// mapping and unmapping of windows is slow, and we don't want to fall
// behind the events.  This communication is unfortunately extremely
// messy.

#define MAX_LEVELS 30

class MWindow;

struct MenuState {
  int level;	// which level of nesting current item is in
  int indexes[MAX_LEVELS]; // index in each level of selected item
  MWindow* menus[MAX_LEVELS]; // windows that have been created
  int nummenus; // number of windows (may be level+1 or level+2)
  bool menubar; // if true menus[0] is a menubar
  bool changed;	// did the menu items change
  int state; // one of the enumeration below
  Menu* widget; // widget that is making this menu
  // return the widget being pointed at:
  Widget* current_widget() {
    return widget->child(indexes, level);
  }
  int current_children() { // # of children of current item
    return widget->children(indexes, level+1);
  }
  MWindow* fakemenu;
};

////////////////////////////////////////////////////////////////

class MenuTitle : public MenuWindow {
  void draw();
  MenuState* menustate;
public:
  Widget* widget;
  MenuTitle(MenuState* m, int X, int Y, int W, int H, Widget*);
};

MenuTitle::MenuTitle(MenuState* m, int X, int Y, int W, int H, Widget* L)
  : MenuWindow(X, Y, W, H, 0), menustate(m)
{
  widget = L;
  // can't use sgi overlay for images:
  if (L->image()) clear_overlay();
}

// In order to draw the rather inconsistant Windows-style menubars, the
// buttonbox() of the menubar is only used to draw the selected menu
// titles. The single static PopupMenu::style->buttonbox is used for drawing
// the selected items in the menus. If Windows was consistent (like NT4 was)
// then we could have used the buttonbox of the menu widget to control
// the popup items so menus in the same program could appear different. Sigh!

void MenuTitle::draw() {

  Widget* style_widget = menustate->widget;
  Box* box = style_widget->buttonbox();
  // popup menus may have no box set:
  if (box == NO_BOX) box = Widget::default_style->buttonbox();

  Flags flags;
  if (!menustate->menubar) {
    // a title on a popup menu is drawn like a button
    flags = OUTPUT;
  } else if (box == FLAT_BOX) {
    // NT 4 style, drawn using selection_color()
    flags = OUTPUT|SELECTED;
    if (widget->active()) widget->set_flag(SELECTED);
  } else {
    // Windows98 style, draw as pushed box
    flags = VALUE|OUTPUT;
  }

  box->draw(0, 0, w(), h(), style_widget->style(), flags);

  // this allow a toggle or other widget to preview it's state:
  if (event_state(ANY_BUTTON)) pushed_ = widget;
  push_matrix();
  translate(5, (h()-widget->height())>>1);
  int save_w = widget->w(); widget->w(w()-10);
  widget->draw();
  widget->w(save_w);
  widget->clear_flag(SELECTED);
  pushed_ = 0;
  pop_matrix();
}

////////////////////////////////////////////////////////////////

class MWindow : public MenuWindow {
  void draw();
public:
  MenuState* menustate;
  int level;
  int children;
  MenuTitle* title;
  bool is_menubar;
  int drawn_selected;	// last redraw has this selected
  MWindow(MenuState*, int level, int X,int Y,int W,int H, Widget* title);
  ~MWindow();
  int find_selected(int mx, int my);
  int titlex(int);
  int autoscroll(int);
  void position(int x, int y);
  int ypos(int);
  Widget* get_widget(int i);
  int is_parent(int i);
  int handle(int);
  static NamedStyle* default_style;
};

static void revert(Style *s) {
  s->box_ = UP_BOX;
  s->buttonbox_ = FLAT_BOX; // was used around selected items, ignored now
  s->leading_ = 4;
}
static NamedStyle style("Menu", revert, &MWindow::default_style);
/** For compatability with random inconsistencies in the Windows
    interface, the box drawn around the popup menu and the spacing
    between the menu items is taken from this style rather than
    from the Menu widget. This probably should be fixed somehow...
*/
NamedStyle* MWindow::default_style = &::style;

// return any widget at a given level:
Widget* MWindow::get_widget(int index) {
  int saved = menustate->indexes[level];
  menustate->indexes[level] = index;
  Widget* w = menustate->widget->child(menustate->indexes, level);
  menustate->indexes[level] = saved;
  return w;
}

// return true if item is a parent of a submenu:
int MWindow::is_parent(int index) {
  int saved = menustate->indexes[level];
  menustate->indexes[level] = index;
  int n = menustate->widget->children(menustate->indexes, level+1);
  menustate->indexes[level] = saved;
  return n >= 0;
}

static const Monitor* monitor;

MWindow::MWindow(MenuState* m, int l, int X, int Y, int Wp, int Hp, Widget* t)
  : MenuWindow(X, Y, Wp, Hp, 0), menustate(m), level(l)
{
  style(default_style);
  Item::set_style(menustate->widget);

  children = m->widget->children(m->indexes, l);

  int selected = l <= menustate->level ? menustate->indexes[l] : -1;

  drawn_selected = -1;

  is_menubar = !level && menustate->menubar;
  if (is_menubar) {
    title = 0;
    return;
  }

  int leading = int(this->leading());
  int Wtitle = 0;
  int Htitle = 0;
  if (t) {
    Wtitle = t->width()+10;
    Htitle = t->height()+leading;
  }

  int W = 0;
  int hotKeysW = 0;
  int H = 0;
  int selected_y = 0;
  for (int i = 0; i < children; i++) {
    Widget* widget = get_widget(i);
    if (!widget->visible()) continue;
    int ih = widget->height();
    if (i == selected) selected_y = H+(ih+leading)/2;
    H += ih+leading;
    int iw = widget->width();
    if (iw > W) W = iw;
    if (is_parent(i)) {
      if (16 > hotKeysW) hotKeysW = 16;
    } else if (widget->shortcut()) {
      int w1 = int(getwidth(key_name(widget->shortcut())) + 8.5);
      if (w1 > hotKeysW) hotKeysW = w1;
    }
    // can't use sgi overlay for images:
    if (widget->image()) clear_overlay();
  }

  Box* box = this->box();

  W += hotKeysW + box->dw();
  if (Wp > W) W = Wp;
  if (Wtitle > W) W = Wtitle;

  if (!Wp) {
    if (selected >= 0) X -= W/2;
    if (X < monitor->x()) X = monitor->x();
    if (X > monitor->r()-W) X = monitor->r()-W;
  }

  int dh = box->dh();
  x(X); w(W); h(H + dh);
  if (selected >= 0) Y += Hp/2-selected_y-box->dy(); else Y += Hp;
  y(Y); // if !group: else {y(Y-2); w(1); h(1);}

  if (t) {
    title = new MenuTitle(menustate, X, Y-Htitle-2-dh, Wtitle, Htitle+dh, t);
    title->style(default_style);
  } else
    title = 0;
}

MWindow::~MWindow() {
  delete title;
}

void MWindow::position(int X, int Y) {
  if (title) {title->position(X, title->y()+Y-y());}
  MenuWindow::position(X, Y);
  // x(X); y(Y); // don't wait for response from X
}

// return the top edge of item:
int MWindow::ypos(int index) {
  int x=0; int y=0; int w=0; int h=0; box()->inset(x,y,w,h);
  int leading = int(this->leading());
  for (int i = 0; i < index; i++) {
    Widget* widget = get_widget(i);
    if (!widget->visible()) continue;
    y += widget->height()+leading;
  }
  return y;
}

extern bool fl_hide_shortcut;

void MWindow::draw() {
  if (damage() != DAMAGE_CHILD) box()->draw(0, 0, w(), h(), style(), OUTPUT);
  int x=0; int y=0; int w=this->w(); int h=0; box()->inset(x,y,w,h);
  int selected = level <= menustate->level ? menustate->indexes[level] : -1;
  int leading = int(this->leading());
  if (style()->hide_shortcut() &&
      !(event_key_state(LeftAltKey)||event_key_state(RightAltKey)))
    fl_hide_shortcut = true;
  Item::set_style(menustate->widget);
  for (int i = 0; i < children; i++) {
    Widget* widget = get_widget(i);
    if (!widget->visible()) continue;
    int ih = widget->height() + leading;
    // for minimal update, only draw the items that changed selection:
    if (damage() != DAMAGE_CHILD || i==selected || i==drawn_selected) {

      Flags flags = widget->flags();
      if (i == selected && !(flags & (OUTPUT|INACTIVE))) {
	flags |= SELECTED;
	//flags &= ~INACTIVE; // damn Windoze compatability...
	// this allow a toggle or other widget to preview it's state:
	if (event_state(ANY_BUTTON) && widget->takesevents())
	  pushed_ = widget;
	setcolor(menustate->widget->selection_color());
	fillrect(x,y,w,ih);
      } else {
	flags &= ~SELECTED;
	// erase the background if only doing partial update. This uses
	// clipping so background pixmaps will work:
	if (damage() == DAMAGE_CHILD) {
	  push_clip(x,y,w,ih);
	  box()->draw(0, 0, this->w(), this->h(), style(), OUTPUT);
	  pop_clip();
	}
      }
      push_matrix();
      translate(x, y+leading/2);
      int save_w = widget->w(); widget->w(w);
      int save_flags = widget->flags();
      widget->flags(flags);
      widget->draw();
      widget->w(save_w);
      pushed_ = 0;
      pop_matrix();
      flags &= ~(VALUE|ALIGN_MASK);

      if (is_parent(i)) {
	// Use the item's fontsize for the size of the arrow, rather than h:
	int nh = int(widget->textsize());
	draw_glyph(GLYPH_RIGHT, x+w-nh, y+((ih-nh)>>1), nh, nh, flags);
      } else if (widget->shortcut()) {
	widget->labeltype()->draw(key_name(widget->shortcut()),
				  x, y, w-3, ih,
				  widget->style(),
				  flags|ALIGN_RIGHT|OUTPUT);
      }
      widget->flags(save_flags);
    }
    y += ih;
  }
  drawn_selected = selected;
  fl_hide_shortcut = false;
}

int MWindow::find_selected(int mx, int my) {
  mx -= x();
  my -= y();
  if (my < 0 || my >= h()) return -1;
  if (is_menubar) {
    if (mx < 0) return -1;
    int x = 3;
    for (int i = 0; i < children; i++) {
      Widget* widget = get_widget(i);
      if (!widget->visible()) continue;
      x += widget->width()+10;
      if (x > mx) return widget->takesevents() ? i : -1;
    }
    return -1;
  } else {
    int leading = int(this->leading());
    int x=0; int y=0; int w=this->w(); int h=this->h(); box()->inset(x,y,w,h);
    if (mx < x || mx >= w) return -1;
    int lasti = -1; // last visible one
    for (int i = 0; i < children; i++) {
      Widget* widget = get_widget(i);
      if (!widget->visible()) continue; // skip if not visible
      y += widget->height()+leading; // find bottom edge
      if (y > my) return i; // if mouse above that edge, we found it
      lasti = i; // remember last visible one
    }
    return lasti; // if mouse off bottom, return last visible one
  }
}

// return horizontal position for item i in a menubar:
int MWindow::titlex(int index) {
  int x = 3;
  for (int i = 0; i < index; i++) {
    Widget* widget = get_widget(i);
    if (!widget->visible()) continue;
    x += widget->width()+10;
  }
  return x;
}

////////////////////////////////////////////////////////////////
// The Menu::popup() run-time:

enum {INITIAL_STATE = 0,// no mouse up or down since popup() called
	PUSH_STATE,	// mouse has been pushed on a normal item
      DONE_STATE	// execute the selected item
};

// scroll so item i is visible on screen, return true if it moves
#define BORDER 2
int MWindow::autoscroll(int i) {
  if (is_menubar || i < 0) return 0;
  // figure out where the item is on the screen:
  int Y = ypos(i);
  // figure out where new top of menu should be:
  if (y()+Y <= monitor->y()) {
    Y = monitor->y()-Y+BORDER;
  } else {
    Widget* widget = get_widget(i);
    Y += widget->height()+int(leading());
    if (y()+Y >= monitor->b()) Y = monitor->b()-Y-BORDER;
    else return 0;
  }
  // move it to that new position:
  MWindow::position(x(), Y);
  return 1;
}

static void autoscroll_timeout(void*) {
  // this will call MWindow::handle(MOVE) but also set event()
  // so that the timeout gets repeated.
  handle(MOVE, 0);
}

static inline void setitem(MenuState& p, int level, int index) {

  if (p.level == level && p.indexes[level] == index) return;
  if (level < 0) return; // this should not happen!

  if (level < p.nummenus && p.indexes[level] != index)
    p.menus[level]->redraw(DAMAGE_CHILD);
  if (level+1 < p.nummenus && p.indexes[level+1] >= 0)
    p.menus[level+1]->redraw(DAMAGE_CHILD);

  delete p.fakemenu; p.fakemenu = 0; // turn off "menubar button"

  // delete all the submenus that we are no longer in:
  if (index >= 0) {
    while (p.nummenus > level+2) delete p.menus[--p.nummenus];
    // delete the next menu only if we are pointing at a different item:
    if (p.nummenus > level+1 && p.indexes[level] != index)
      delete p.menus[--p.nummenus];
  }

  p.level = level;
  p.indexes[level] = index;
  p.indexes[level+1] = -1;

  // set flag so new menus are created by the main idle loop:
  p.changed = true;

  // If we scroll to show this item and the user dragged to it, we should
  // continue scrolling after a timeout:
  fltk::remove_timeout(autoscroll_timeout, &p);
  if (p.menus[level]->autoscroll(index))
    fltk::repeat_timeout(.05f, autoscroll_timeout, &p);
}

static int forward(MenuState& p, int menu) {
  // go to next item in menu menu if possible
  MWindow &m = *(p.menus[menu]);
  for (int item = p.indexes[menu]+1; item < m.children; item++) {
    Widget* widget = m.get_widget(item);
    if (widget->takesevents()) {setitem(p, menu, item); return 1;}
  }
  return 0;
}

static int backward(MenuState& p, int menu) {
  // previous item in menu menu if possible
  MWindow &m = *(p.menus[menu]);
  for (int item = p.indexes[menu]-1; item >= 0; item--) {
    Widget* widget = m.get_widget(item);
    if (widget->takesevents()) {setitem(p, menu, item); return 1;}
  }
  return 0;
}

static bool track_mouse;

int MWindow::handle(int event) {
  MenuState &p = *menustate;
  Widget* widget = 0;
  switch (event) {

  case KEY:
    track_mouse = false;
    switch (event_key()) {
    case LeftAltKey:
    case RightAltKey:
      if (style()->hide_shortcut() && !event_clicks()) {
	for (int i = 0; i < p.nummenus; i++)
	  p.menus[i]->redraw();
      }
      return 1;
    case UpKey:
      if (p.menubar && p.level == 0) ;
      else if (backward(p, p.level));
      else if (p.menubar) setitem(p, 0, p.indexes[0]);
      return 1;
    case DownKey:
      if (p.level || !p.menubar) forward(p, p.level);
      else if (p.level+1 < p.nummenus) forward(p, p.level+1);
      return 1;
    case RightKey:
      if (p.menubar && (p.level<=0 || p.level==1 && p.nummenus==2))
	forward(p, 0);
      else if (p.level+1 < p.nummenus) forward(p, p.level+1);
      return 1;
    case LeftKey:
      if (p.menubar && p.level<=1) backward(p, 0);
      else if (p.level > 0)
	setitem(p, p.level-1, p.indexes[p.level-1]);
      return 1;
    case SpaceKey:
    case ReturnKey:
      goto EXECUTE;
    case EscapeKey:
      exit_modal();
      return 1;
    }
    {for (int menu = p.nummenus; menu--;) {
      MWindow &mw = *(p.menus[menu]);
      int nextitem = -1;
      static char lastkey; // test for same key multiple times
      if (p.indexes[menu] < 0) lastkey = 0;
      for (int item = 0; item < mw.children; item++) {
	widget = mw.get_widget(item);
	if (widget->active() && fltk::test_shortcut(widget->shortcut())) {
	  setitem(p, menu, item);
	  lastkey = 0;
	  goto EXECUTE;
	}
	// continue unless this item can be picked by the keystroke:
	if (!widget->takesevents()) continue;
	if (widget->test_label_shortcut()) {
	  // underscored items are jumped to immediately on first keystroke:
	  if (event_text()[0]!=lastkey) {nextitem = item; continue;}
	} else {
	  const char* l = widget->label();
	  if (!l || tolower(*l) != event_text()[0]) continue;
	}
	// cycle around the selectable items:
	if (nextitem < 0 ||
	    nextitem <= p.indexes[menu] && item > p.indexes[menu])
	  nextitem = item;
      }
      lastkey = event_text()[0];
      if (nextitem >= 0) {
	setitem(p, menu, nextitem);
	return 1;
      }
    }}
    return 1; // always eat all the keystrokes

  case KEYUP:
    if (event_key() == LeftAltKey || event_key() == RightAltKey) {
      if (event_is_click()) {
	// checking for event_clicks insures that the keyup matches the
	// keydown that preceeded it, so Alt was pressed & released without
	// any intermediate values.
	exit_modal();
	return 1;
      }
      if (style()->hide_shortcut()) {
	for (int i = 0; i < p.nummenus; i++)
	  p.menus[i]->redraw();
      }
    }
    return 0;

  case ENTER: // this messes up menu bar pulldown shortcuts
  case MOVE:
    if (!track_mouse) return 1;
  case PUSH:
    track_mouse = true;
  case DRAG: {
    int mx = event_x_root();
    int my = event_y_root();
    int item, menu;
    // search the visible menus from top down for item being pointed at:
    for (menu = p.nummenus-1; ; menu--) {
      if (menu < 0) { // not pointing at anything:
	// quit if they clicked off the menus:
	if (event == PUSH) {exit_modal(); return 0;}
	// keep the lowest-visible menu visible but with nothing selected:
	menu = p.nummenus-1;
	item = -1;
	break;
      }
      item = p.menus[menu]->find_selected(mx, my);
      if (item >= 0) break;
    }
    if (event == PUSH) {
      p.state = PUSH_STATE;
      // redraw checkboxes so they preview the state they will be in:
      Widget* widget = p.menus[menu]->get_widget(item);
      if (checkmark(widget)) p.menus[menu]->redraw(DAMAGE_CHILD);
    }
    setitem(p, menu, item);
    return 1;}

  case RELEASE:
    pushed_ = 0;
    // The initial click just brings up the menu. The user has to either
    // drag the mouse around, hold it still for a long time, or click
    // again to actually pick an item and dismiss the menu. You can
    // make the first click pick the item (which might be useful for
    // a Choice style list) by setting event_is_click(false) before
    // doing the popup.
    if (p.state == INITIAL_STATE && event_is_click()) {
      // except for buttons in the menubar, they execute on click:
      if (!p.menubar || p.level || p.nummenus > 1) return 1;
    }
  EXECUTE: // execute the item pointed to by w and current item
    // If they click outside menu we quit:
    if (p.indexes[p.level]<0) {exit_modal(); return 1;}
    // ignore clicks on inactive items:
    widget = p.current_widget();
    if (!widget->takesevents()) return 1;
#if 0
    if ((widget->flags() & MENU_STAYS_UP) && (!p.menubar || p.level)) {
      p.widget->focus(p.indexes, p.level);
      p.widget->execute(widget);
      Window* mw = p.menus[p.level];
      if (widget->type() == Item::RADIO) mw->redraw();
      else if (checkmark(widget)) mw->redraw(DAMAGE_CHILD);
      return 1;
    }
#endif
    // ignore clicks on menu titles unless it they have a callback:
//  if (widget->callback() == Widget::default_callback &&
//	p.current_children() >= 0) return 1;
    p.state = DONE_STATE;
    exit_modal();
    return 1;
  }
  return MenuWindow::handle(event);
}

Widget* Menu::try_popup(
    int X, int Y, int W, int H,
    Widget* title,
    bool menubar)
{
  Group::current(0); // fix possible programmer error...

  // Incredible kludge! Menus are inconsistent, they should default to
  // white to match browsers and other widgets. Unfortunately standard
  // design is to make them gray. But then users expect the labelcolor/font
  // to be used for the menu items. So I directly detect settings to gray
  // and copy the label font in that case:
  MWindow::default_style->color_ = color();
  if (MWindow::default_style->color_ == GRAY75)
    MWindow::default_style->textcolor_ = labelcolor();
  else
    MWindow::default_style->textcolor_ = textcolor();

  // figure out where to pop up in screen coordinates:
  if (parent()) {
    for (Widget* w = this; w; w = w->parent()) {
      X += w->x();
      Y += w->y();
    }
  } else {
    X += event_x_root()-event_x();
    Y += event_y_root()-event_y();
  }
  if (fltk::event() == fltk::PUSH)
    monitor = &Monitor::find(event_x_root(), event_y_root());
  else
    monitor = &Monitor::all();

  MenuState p;
  p.nummenus = 1;
  p.menubar = menubar;
  p.widget = this;
  p.state = INITIAL_STATE;
  p.level = 0;
  p.indexes[0] = value();
  p.indexes[1] = -1;

  MWindow toplevel(&p, 0, X, Y, W, H, title);
  toplevel.child_of(Window::first());
  p.menus[0] = &toplevel;
  p.fakemenu = 0;

  if (menubar) {
    if (value() < 0)
      toplevel.handle(PUSH); // get menu mouse points at to appear
    else {
      p.indexes[0] = value();
      p.indexes[1] = -1;
      p.level = 0;
    }
    p.changed = true; // make it create the pulldown menu
  } else {
    // create submenus until we locate the one with selected item
    // in it, positioning them so that one is selected:
    for (;;) {
      if (p.indexes[p.level] < 0) break;
      if (p.current_children() < 0) break;
      Widget* widget = p.current_widget();
      if (!widget->takesevents()) break;
      //if (!widget->is_group()) break;
      int item = ((Group*)widget)->focus();
      if (item < 0) break;

      MWindow* mw = p.menus[p.level];
      int nX = mw->x() + mw->w();
      int nY = mw->y() + mw->ypos(p.indexes[p.level])-mw->ypos(0);
      p.level++;
      p.indexes[p.level] = item;
      p.indexes[p.level+1] = -1;
      mw = new MWindow(&p, p.level, X,Y,W,H, 0);
      p.menus[p.nummenus++] = mw;
      // move all earlier menus to line up with this new one:
      int dy = mw->y()-nY;
      int dx = mw->x()-nX;
      for (int menu = 0; menu < p.level; menu++) {
	MWindow* t = p.menus[menu];
	int nx = t->x()+dx; if (nx < 0) {nx = 0; dx = -t->x();}
	int ny = t->y()+dy; if (ny < 0) {ny = 0; dy = -t->y();}
	t->position(nx, ny);
      }
    }
    // show all the menus:
    for (int menu = 0; menu <= p.level; menu++) {
      MWindow* mw = p.menus[menu];
      if (mw->title) mw->title->show(mw->child_of());
      mw->show();
    }
    p.changed = true;
  }

  Widget* saved_modal = modal(); bool saved_grab = grab();
  p.state = INITIAL_STATE;
  track_mouse = event_state(ANY_BUTTON);
#ifdef DEBUG
#define MODAL false
#else
#define MODAL true
#endif
  for (modal(&toplevel, MODAL); !exit_modal_flag(); wait()) {

    if (!p.changed) continue;

    // Create windows for submenus. Since this is expensive we don't
    // do this when the item changes, instead we wait until there are no
    // events (after wait() returns) to create menus. This means that
    // intermediate windows are not created when the user moves the mouse
    // quickly.
    p.changed = false;

    int index = p.indexes[p.level];
    if (index < 0) continue;	// no item selected, so no submenu changes
    if (p.level < p.nummenus-1) continue; // submenu already up

    Widget* widget = p.current_widget();

    MWindow* mw = p.menus[p.level];

    if (p.menubar && !p.level) {
      // create a submenu off a menubar with a title box:
      int nX = mw->x() + mw->titlex(index);
      int nY = mw->y() + mw->h();
      mw = new MWindow(&p, 1, nX, nY, 0, 0, widget);
      // fix the title box to match menubar thickness:
      int dx=0; int y1=p.menus[0]->y(); int dw=0; int h1=p.widget->h();
      p.widget->box()->inset(dx,y1,dw,h1);
      mw->title->y(y1+1);
      mw->title->h(h1-2);
      mw->title->show(p.menus[0]->child_of());
      if (widget->takesevents() && p.current_children()>=0) {
	// if it is a real menu we add it to the list of displayed menus
	p.menus[p.nummenus++] = mw;
	mw->show(p.menus[0]->child_of());
      } else {
	// Non-submenus in the menubar produce "buttons" which I "highlight"
	// my making a fake submenu with only the title box displayed.
	p.fakemenu = mw;
      }

    } else if (widget->takesevents() && p.current_children()>=0) {
      // Create a normal submenu:
      int nX = mw->x() + mw->w();
      int nY = mw->y() + mw->ypos(index) - mw->ypos(0);
      mw = new MWindow(&p, p.nummenus, nX, nY, 0, 0, 0);
      p.menus[p.nummenus++] = mw;
      mw->show(p.menus[0]->child_of());

    }
  }

  fltk::remove_timeout(autoscroll_timeout, &p);
  Item::clear_style();

  // destroy all the submenus we created:
  delete p.fakemenu;
  while (--p.nummenus) delete p.menus[p.nummenus];

  // I believe this is here so that if you exec() a window in response
  // to a menu item the correct window is selected as the parent:
  // The cast is to remove the const from toplevel.child_of()
  Window::first((Window*)toplevel.child_of());

  toplevel.hide();
  modal(saved_modal, saved_grab);

  if (p.state != DONE_STATE) return 0; // user did not pick anything

  // Execute whatever item the user picked:
  focus(p.indexes, p.level);
  if (menubar && !p.level && checkmark(item())) redraw();
  return item();
}

int Menu::popup(
    int X, int Y, int W, int H,
    Widget* title,
    bool menubar)
{
  Widget *selected = try_popup(X, Y, W, H, title, menubar);
  if (selected) {
    execute(selected);
    return 1;
  }
  return 0;
}

//
// End of "$Id: Fl_Menu.cxx,v 1.146 2003/11/04 08:11:00 spitzak Exp $".
//
