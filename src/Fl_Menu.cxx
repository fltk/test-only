//
// "$Id: Fl_Menu.cxx,v 1.126 2002/07/01 15:28:19 spitzak Exp $"
//
// Implementation of popup menus.  These are called by using the
// Fl_Menu_::popup and Fl_Menu_::pulldown methods.  See also the
// Fl_Menu_ base class in Fl_Menu_.cxx.  Warning: this code is
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/Fl.h>
#include <fltk/Fl_Menu_Window.h>
#include <fltk/Fl_Menu_.h>
#include <fltk/Fl_Item.h> // for TOGGLE, RADIO
#define checkmark(item) (item->type()>=Fl_Item::TOGGLE && item->type()<=Fl_Item::RADIO)
#include <fltk/fl_draw.h>

// class MenuTitle is a small window for the title, it will look like
// it is being drawn in the menu bar, but is actually a window
// floating above it (this was a rather clever trick SGI came up with
// and makes the menus faster if backing store works on the X server).

// class MenuWindow is the larger window containing all the menu
// items.  It is NOT the Fl_Menu_ itself, instead it has a pointer
// called "list" that points at it.  This contains a lot of ugly code
// to draw the menu items and to draw arrows for parents and draw
// shortcuts.

// The rest of the code is the run-time system for Fl_Menu_::popup.
// Fl_Menu_::popup does not return until the user has selected an item
// and all the widgets created here are destroyed.

// Fl::grab() is done to send all events to our own handle function
// The entire menu "state" is stuck into a structure passed to the
// handle mentod.  The handle() method changes variables in this state
// to indicate what item is picked, but does not actually alter the
// display, instead the main loop does that.  This is because the X
// mapping and unmapping of windows is slow, and we don't want to fall
// behind the events.  This communication is unfortunately extremely
// messy.

#define MAX_LEVELS 30

class MenuWindow;

struct MenuState {
  int level;	// which level of nesting current item is in
  int indexes[MAX_LEVELS]; // index in each level of selected item
  MenuWindow* menus[MAX_LEVELS]; // windows that have been created
  int nummenus; // number of windows (may be level+1 or level+2)
  bool menubar; // if true menus[0] is a menubar
  bool changed;	// did the menu items change
  int state; // one of the enumeration below
  Fl_Menu_* widget; // widget that is making this menu
  // return the widget being pointed at:
  Fl_Widget* current_widget() {
    return widget->child(indexes, level);
  }
  // return the widget at a given level:
  Fl_Widget* current_widget(int level) {
    return widget->child(indexes, level);
  }
  int current_children() {
    return widget->children(indexes, level+1);
  }
  MenuWindow* fakemenu;
};

////////////////////////////////////////////////////////////////

class MenuTitle : public Fl_Menu_Window {
  void draw();
  MenuState* menustate;
public:
  Fl_Widget* widget;
  MenuTitle(MenuState* m, int X, int Y, int W, int H, Fl_Widget*);
};

MenuTitle::MenuTitle(MenuState* m, int X, int Y, int W, int H, Fl_Widget* L)
  : Fl_Menu_Window(X, Y, W, H, 0), menustate(m)
{
  end();
  set_override();
  widget = L;
  // can't use sgi overlay for images:
  if (L->image()) clear_overlay();
}

// In order to draw the rather inconsistant Windows-style menubars, the
// button_box() of the menubar is only used to draw the selected menu
// titles. The single static Fl_Menu::style->button_box is used for drawing
// the selected items in the menus. If Windows was consistent (like NT4 was)
// then we could have used the button_box of the menu widget to control
// the popup items so menus in the same program could appear different. Sigh!

void MenuTitle::draw() {

  Fl_Widget* style_widget = menustate->widget;
  Fl_Boxtype box = style_widget->button_box();
  // popup menus may have no box set:
  if (box == FL_NO_BOX) box = Fl_Widget::default_style->button_box;

  Fl_Flags flags;
  Fl_Color color;
  if (!menustate->menubar) {
    // a title on a popup menu
    flags = 0;
    color = style_widget->button_color();
  } else if (box == FL_FLAT_BOX) {
    // NT 4 style
    flags = FL_SELECTED;
    if (widget->active()) widget->set_flag(FL_SELECTED);
    color = style_widget->selection_color();
  } else {
    // Windows98 style
    flags = FL_VALUE;
    color = style_widget->button_color();
  }

  box->draw(0, 0, w(), h(), color, flags);

  // this allow a toggle or other widget to preview it's state:
  if (Fl::event_state(FL_BUTTONS)) Fl::pushed_ = widget;
  fl_push_matrix();
  fl_translate(5, (h()-widget->height())/2);
  int save_w = widget->w(); widget->w(w()-10);
  widget->draw();
  widget->w(save_w);
  widget->clear_flag(FL_SELECTED);
  Fl::pushed_ = 0;
  fl_pop_matrix();
}

////////////////////////////////////////////////////////////////

class MenuWindow : public Fl_Menu_Window {
  void draw();
public:
  MenuState* menustate;
  int level;
  MenuTitle* title;
  bool is_menubar;
  int drawn_selected;	// last redraw has this selected
  MenuWindow(MenuState*, int level, int X,int Y,int W,int H, Fl_Widget* title);
  ~MenuWindow();
  int find_selected(int mx, int my);
  int titlex(int);
  int autoscroll(int);
  void position(int x, int y);
  int ypos(int);
  Fl_Widget* get_widget(int i);
  int is_parent(int i);
  int handle(int);
  static Fl_Named_Style* default_style;
};

// The box, color, and selection color of all menus is taken from
// this. The button_box is drawn around each item in the menu (nyi).
// The color is rather clumsily replaced with the color()
// of the calling widget to allow compatability with fltk 1.0 and
// to allow popup menus to have different colors.

static void revert(Fl_Style *s) {
  s->box = FL_UP_BOX;
  s->button_box = FL_FLAT_BOX;
  s->leading = 4;
}
static Fl_Named_Style style("Menu", revert, &MenuWindow::default_style);
Fl_Named_Style* MenuWindow::default_style = &::style;

// return any widget at a given level:
Fl_Widget* MenuWindow::get_widget(int index) {
  int saved = menustate->indexes[level];
  menustate->indexes[level] = index;
  Fl_Widget* w = menustate->current_widget(level);
  menustate->indexes[level] = saved;
  return w;
}

// return true if item is a parent of a submenu:
int MenuWindow::is_parent(int index) {
  int saved = menustate->indexes[level];
  menustate->indexes[level] = index;
  int n = menustate->widget->children(menustate->indexes, level+1);
  menustate->indexes[level] = saved;
  return n >= 0;
}

MenuWindow::MenuWindow(MenuState* m, int l, int X, int Y, int Wp, int Hp, Fl_Widget* t)
  : Fl_Menu_Window(X, Y, Wp, Hp, 0), menustate(m), level(l)
{
  end();
  set_override();
  style(default_style);

  int selected = l <= menustate->level ? menustate->indexes[l] : -1;

  drawn_selected = -1;

  is_menubar = !level && menustate->menubar;
  if (is_menubar) {
    title = 0;
    return;
  }

  int leading = this->leading(); // +2 ?

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
  for (int i = 0;; i++) {
    Fl_Widget* widget = get_widget(i);
    if (!widget) break;
    if (!widget->visible()) continue;
    int ih = widget->height();
    if (i == selected) selected_y = H+(ih+leading)/2;
    H += ih+leading;
    int iw = widget->width();
    if (iw > W) W = iw;
    if (is_parent(i)) {
      if (16 > hotKeysW) hotKeysW = 16;
    } else if (widget->shortcut()) {
      int w1 = int(fl_width(Fl::key_name(widget->shortcut())) + 8.5);
      if (w1 > hotKeysW) hotKeysW = w1;
    }
    // can't use sgi overlay for images:
    if (widget->image()) clear_overlay();
  }

  Fl_Boxtype box = this->box();

  W += hotKeysW + box->dw();
  if (Wp > W) W = Wp;
  if (Wtitle > W) W = Wtitle;

  if (!Wp) {
    if (selected >= 0) X -= W/2;
    if (X < 0) X = 0;
    if (X > Fl::w()-W) X= Fl::w()-W;
  }

  int dh = box->dh();
  x(X); w(W); h(H + dh);
  if (selected >= 0) Y += Hp/2-selected_y-box->dy(); else Y += Hp;
  y(Y); // if !group: else {y(Y-2); w(1); h(1);}

  if (t) {
    title = new MenuTitle(menustate, X, Y-Htitle-2-dh, Wtitle, Htitle+dh, t);
  } else
    title = 0;
}

MenuWindow::~MenuWindow() {
  delete title;
}

void MenuWindow::position(int X, int Y) {
  if (title) {title->position(X, title->y()+Y-y());}
  Fl_Menu_Window::position(X, Y);
  // x(X); y(Y); // don't wait for response from X
}

// return the top edge of item:
int MenuWindow::ypos(int index) {
  int x=0; int y=0; int w=0; int h=0; box()->inset(x,y,w,h);
  int leading = this->leading(); // +2 ?
  for (int i = 0; i < index; i++) {
    Fl_Widget* widget = get_widget(i);
    if (!widget->visible()) continue;
    y += widget->height()+leading;
  }
  return y;
}

void MenuWindow::draw() {
  if (damage() != FL_DAMAGE_CHILD) box()->draw(0, 0, w(), h(), color(), 0);
  int x=0; int y=0; int w=this->w(); int h=0; box()->inset(x,y,w,h);
  int selected = level <= menustate->level ? menustate->indexes[level] : -1;
  int leading = this->leading(); // +2 ?
  for (int i = 0; ; i++) {
    Fl_Widget* widget = get_widget(i);
    if (!widget) break;
    if (!widget->visible()) continue;
    int ih = widget->height() + leading;
    // for minimal update, only draw the items that changed selection:
    if (damage() != FL_DAMAGE_CHILD || i==selected || i==drawn_selected) {

      Fl_Flags flags = widget->flags();
      if (i == selected && !(flags & (FL_OUTPUT|FL_INACTIVE))) {
	flags |= FL_SELECTED;
	//flags &= ~FL_INACTIVE; // damn Windoze compatability...
	// this allow a toggle or other widget to preview it's state:
	if (Fl::event_state(FL_BUTTONS) && widget->takesevents())
	  Fl::pushed_ = widget;
	fl_color(menustate->widget->selection_color());
	fl_rectf(x,y,w,ih);
      } else {
	flags &= ~FL_SELECTED;
	// erase the background if only doing partial update. This uses
	// clipping so background pixmaps will work:
	if (damage() == FL_DAMAGE_CHILD) {
	  fl_push_clip(x,y,w,ih);
	  box()->draw(0, 0, this->w(), this->h(), color(), 0);
	  fl_pop_clip();
	}
      }
      fl_push_matrix();
      fl_translate(x, y+leading/2);
      int save_w = widget->w(); widget->w(w);
      int save_flags = widget->flags();
      widget->flags(flags);
      widget->draw();
      widget->flags(save_flags);
      widget->w(save_w);
      Fl::pushed_ = 0;
      fl_pop_matrix();
      flags &= ~(FL_VALUE|FL_ALIGN_MASK);

      if (is_parent(i)) {
	// Use the item's fontsize for the size of the arrow, rather than h:
	int nh = widget->text_size();
	draw_glyph(FL_GLYPH_RIGHT, x+w-nh, y+(ih-nh)/2, nh, nh, flags);
      } else if (widget->shortcut()) {
	fl_font(widget->text_font(), widget->text_size());
	widget->label_type()->draw(Fl::key_name(widget->shortcut()),
				   x, y, w-3, ih,
				   (flags&FL_SELECTED) ?
				   widget->selection_text_color() : 
				   widget->label_color(),
				   flags|FL_ALIGN_RIGHT);
      }
    }
    y += ih;
  }
  drawn_selected = selected;
}

int MenuWindow::find_selected(int mx, int my) {
  mx -= x();
  my -= y();
  if (my < 0 || my >= h()) return -1;
  if (is_menubar) {
    if (mx < 0) return -1;
    int x = 3;
    for (int i = 0; ; i++) {
      Fl_Widget* widget = get_widget(i);
      if (!widget) return -1;
      if (!widget->visible()) continue;
      x += widget->width()+10;
      if (x > mx) return i;
    }
  } else {
    int leading = this->leading(); // +2 ?
    int x=0; int y=0; int w=this->w(); int h=this->h(); box()->inset(x,y,w,h);
    if (mx < x || mx >= w) return -1;
    for (int i = 0; ; i++) {
      Fl_Widget* widget = get_widget(i);
      if (!widget) return i-1;
      if (!widget->visible()) continue;
      y += widget->height()+leading;
      if (y > my) return i;
    }
  }
}

// return horizontal position for item i in a menubar:
int MenuWindow::titlex(int index) {
  int x = 3;
  for (int i = 0; i < index; i++) {
    Fl_Widget* widget = get_widget(i);
    if (!widget->visible()) continue;
    x += widget->width()+10;
  }
  return x;
}

////////////////////////////////////////////////////////////////
// The Fl_Menu_::popup() run-time:

enum {INITIAL_STATE = 0,// no mouse up or down since popup() called
	PUSH_STATE,	// mouse has been pushed on a normal item
      DONE_STATE	// execute the selected item
};

// scroll so item i is visible on screen, return true if it moves
int MenuWindow::autoscroll(int i) {
  if (is_menubar || i < 0) return 0;
  int Y = y()+ypos(i);
  if (Y <= Fl::y()) {
    Y = Fl::y()-Y+10;
  } else {
    Fl_Widget* widget = get_widget(i);
    Y = Y+widget->height()+leading()-Fl::h()-Fl::y();
    if (Y < 0) return 0;
    Y = -Y-10;
  }
  Fl_Menu_Window::position(x(), y()+Y);
  // y(y()+Y); // don't wait for response from X
  return 1;
}

static void autoscroll_timeout(void*) {
  // this will call MenuWindow::handle(FL_MOVE) but also set Fl::event()
  // so that the timeout gets repeated.
  Fl::handle(FL_MOVE, 0);
}

static inline void setitem(MenuState& p, int level, int index) {

  if (p.level == level && p.indexes[level] == index) return;
  if (level < 0) return; // this should not happen!

  if (level < p.nummenus && p.indexes[level] != index)
    p.menus[level]->redraw(FL_DAMAGE_CHILD);
  if (level+1 < p.nummenus && p.indexes[level+1] >= 0)
    p.menus[level+1]->redraw(FL_DAMAGE_CHILD);

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
  Fl::remove_timeout(autoscroll_timeout, &p);
  if (p.menus[level]->autoscroll(index))
    Fl::repeat_timeout(.05, autoscroll_timeout, &p);
}

static int forward(MenuState& p, int menu) {
  // go to next item in menu menu if possible
  MenuWindow &m = *(p.menus[menu]);
  for (int item = p.indexes[menu]+1;;item++) {
    Fl_Widget* widget = m.get_widget(item);
    if (!widget) return 0;
    if (widget->takesevents()) {setitem(p, menu, item); return 1;}
  }
}

static int backward(MenuState& p, int menu) {
  // previous item in menu menu if possible
  MenuWindow &m = *(p.menus[menu]);
  for (int item = p.indexes[menu]-1; item >= 0; item--) {
    Fl_Widget* widget = m.get_widget(item);
    if (!widget) return 0;
    if (widget->takesevents()) {setitem(p, menu, item); return 1;}
  }
  return 0;
}

int MenuWindow::handle(int event) {
  MenuState &p = *menustate;
  Fl_Widget* widget = 0;
  switch (event) {

  case FL_KEY:
  case FL_SHORTCUT:
    switch (Fl::event_key()) {
    case FL_Up:
      if (p.menubar && p.level == 0) ;
      else if (backward(p, p.level));
      else if (p.menubar) setitem(p, 0, p.indexes[0]);
      return 1;
    case FL_Down:
      if (p.level || !p.menubar) forward(p, p.level);
      else if (p.level+1 < p.nummenus) forward(p, p.level+1);
      return 1;
    case FL_Right:
      if (p.menubar && (p.level<=0 || p.level==1 && p.nummenus==2))
	forward(p, 0);
      else if (p.level+1 < p.nummenus) forward(p, p.level+1);
      return 1;
    case FL_Left:
      if (p.menubar && p.level<=1) backward(p, 0);
      else if (p.level > 0)
	setitem(p, p.level-1, p.indexes[p.level-1]);
      return 1;
    case ' ':
    case FL_Enter:
      goto EXECUTE;
    case FL_Escape:
      Fl::exit_modal();
      return 1;
    }
    {for (int menu = p.nummenus; menu--;) {
      MenuWindow &mw = *(p.menus[menu]);
      for (int item = 0; ; item++) {
	widget = mw.get_widget(item);
	if (!widget) break;
	if (widget->takesevents() && widget->test_shortcut()) {
	  setitem(p, menu, item);
	  goto EXECUTE;
	}
      }
    }}
    return 1; // always eat all the keystrokes

  case FL_KEYUP:
    if ((Fl::event_key() == FL_Alt_L || Fl::event_key() == FL_Alt_R)
	&& Fl::event_is_click()) {
      // checking for event_clicks insures that the keyup matches the
      // keydown that preceeded it, so Alt was pressed & released without
      // any intermediate values.
      Fl::exit_modal();
      return 1;
    } else
      return 0;

  // case FL_ENTER: // this messes up menu bar pulldown shortcuts
  case FL_PUSH:
  case FL_MOVE:
  case FL_DRAG: {
    int mx = Fl::event_x_root();
    int my = Fl::event_y_root();
    int item, menu;
    // search the visible menus from top down for item being pointed at:
    for (menu = p.nummenus-1; ; menu--) {
      if (menu < 0) { // not pointing at anything:
	// quit if they clicked off the menus:
	if (event == FL_PUSH) {Fl::exit_modal(); return 0;}
	// keep the lowest-visible menu visible but with nothing selected:
	menu = p.nummenus-1;
	item = -1;
	break;
      }
      item = p.menus[menu]->find_selected(mx, my);
      if (item >= 0) break;
    }
    if (event == FL_PUSH) {
      p.state = PUSH_STATE;
      // redraw checkboxes so they preview the state they will be in:
      Fl_Widget* widget = p.menus[menu]->get_widget(item);
      if (checkmark(widget)) p.menus[menu]->redraw(FL_DAMAGE_CHILD);
    }
    setitem(p, menu, item);
    return 1;}

  case FL_RELEASE:
    Fl::pushed_ = 0;
    // Allow menus to be "clicked-up".  Without this a single click will
    // pick whatever item the mouse is pointing at in a pop-up menu:
    if (p.state == INITIAL_STATE && Fl::event_is_click()) {
      // don't do this for checkboxes as it is confusing to the user
      // as to whether or not they turned it on.
      widget = p.current_widget();
      if (!widget || !checkmark(widget)) return 1;
    }
  EXECUTE: // execute the item pointed to by w and current item
    // If they click outside menu we quit:
    widget = p.current_widget();
    if (!widget) {Fl::exit_modal(); return 1;}
    // ignore clicks on inactive items:
    if (!widget->takesevents()) return 1;
#if 0
    if ((widget->flags() & FL_MENU_STAYS_UP) && (!p.menubar || p.level)) {
      p.widget->focus(p.indexes, p.level);
      p.widget->execute(widget);
      Fl_Window* mw = p.menus[p.level];
      if (widget->type() == Fl_Item::RADIO) mw->redraw();
      else if (checkmark(widget)) mw->redraw(FL_DAMAGE_CHILD);
      return 1;
    }
#endif
    // ignore clicks on menu titles unless it they have a callback:
//  if (widget->callback() == Fl_Widget::default_callback &&
//	p.current_children() >= 0) return 1;
    p.state = DONE_STATE;
    Fl::exit_modal();
    return 1;
  }
  return Fl_Menu_Window::handle(event);
}

int Fl_Menu_::popup(
    int X, int Y, int W, int H,
    Fl_Widget* title,
    bool menubar)
{
  Fl_Group::current(0); // fix possible programmer error...

  MenuWindow::default_style->color = color();

  // figure out where to pop up in screen coordinates:
  if (parent()) {
    for (Fl_Widget* w = this; w; w = w->parent()) {
      X += w->x();
      Y += w->y();
    }
  } else {
    X += Fl::event_x_root()-Fl::event_x();
    Y += Fl::event_y_root()-Fl::event_y();
  }

  MenuState p;
  p.nummenus = 1;
  p.menubar = menubar;
  p.widget = this;
  p.state = INITIAL_STATE;
  p.level = 0;
  p.indexes[0] = value();
  p.indexes[1] = -1;
  MenuWindow toplevel(&p, 0, X, Y, W, H, title);
  toplevel.child_of(Fl::first_window());
  p.menus[0] = &toplevel;
  p.fakemenu = 0;

  if (menubar) {
    if (value() < 0)
      toplevel.handle(FL_PUSH); // get menu mouse points at to appear
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
      Fl_Widget* widget = p.current_widget();
      if (!widget->takesevents()) break;
      if (!widget->is_group()) break;
      int item = ((Fl_Group*)widget)->focus();
      if (item < 0) break;

      MenuWindow* mw = p.menus[p.level];
      int nX = mw->x() + mw->w();
      int nY = mw->y() + mw->ypos(p.indexes[p.level])-mw->ypos(0);
      p.level++;
      p.indexes[p.level] = item;
      p.indexes[p.level+1] = -1;
      mw = new MenuWindow(&p, p.level, X,Y,W,H, 0);
      p.menus[p.nummenus++] = mw;
      // move all earlier menus to line up with this new one:
      int dy = mw->y()-nY;
      int dx = mw->x()-nX;
      for (int menu = 0; menu < p.level; menu++) {
	MenuWindow* t = p.menus[menu];
	int nx = t->x()+dx; if (nx < 0) {nx = 0; dx = -t->x();}
	int ny = t->y()+dy; if (ny < 0) {ny = 0; dy = -t->y();}
	t->position(nx, ny);
      }
    }
    // show all the menus:
    for (int menu = 0; menu <= p.level; menu++) {
      MenuWindow* mw = p.menus[menu];
      if (mw->title) mw->title->show(mw->child_of());
      mw->show();
    }
    p.changed = false;
  }

  Fl_Widget* saved_modal = Fl::modal(); bool saved_grab = Fl::grab();
  p.state = INITIAL_STATE;

  for (Fl::modal(&toplevel, true); !Fl::exit_modal_flag(); Fl::wait()) {

    if (!p.changed) continue;

    // Create windows for submenus. Since this is expensive we don't
    // do this when the item changes, instead we wait until there are no
    // events (after Fl::wait() returns) to create menus. This means that
    // intermediate windows are not created when the user moves the mouse
    // quickly.
    p.changed = false;

    int index = p.indexes[p.level];
    if (index < 0) continue;	// no item selected, so no submenu changes
    if (p.level < p.nummenus-1) continue; // submenu already up

    Fl_Widget* widget = p.current_widget();
    if (!widget) continue; // this should not happen

    MenuWindow* mw = p.menus[p.level];

    if (p.menubar && !p.level) {
      // create a submenu off a menubar with a title box:
      int nX = mw->x() + mw->titlex(index);
      int nY = mw->y() + mw->h();
      mw = new MenuWindow(&p, 1, nX, nY, 0, 0, widget);
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
      mw = new MenuWindow(&p, p.nummenus, nX, nY, 0, 0, 0);
      p.menus[p.nummenus++] = mw;
      mw->show(p.menus[0]->child_of());

    }
  }

  Fl::modal(saved_modal, saved_grab);

  Fl::remove_timeout(autoscroll_timeout, &p);

  // destroy all the submenus we created:
  delete p.fakemenu;
  while (--p.nummenus) delete p.menus[p.nummenus];
  toplevel.hide();

  if (p.state != DONE_STATE) return 0; // user did not pick anything

  // Execute whatever item the user picked:
  focus(p.indexes, p.level);
  if (menubar && !p.level && checkmark(item())) redraw();
  execute(item());
  return 1;
}

//
// End of "$Id: Fl_Menu.cxx,v 1.126 2002/07/01 15:28:19 spitzak Exp $".
//
