//
// "$Id: Fl_Menu.cxx,v 1.92 2000/08/10 09:24:31 spitzak Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Item.H> // for FL_TOGGLE_ITEM, FL_RADIO_ITEM
#include <FL/fl_draw.H>

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

////////////////////////////////////////////////////////////////

// Style used by both classes.  The text_box is used to draw the box
// around selected items:

static void revert(Fl_Style* s) {
  s->text_box = FL_FLAT_BOX;
  s->leading = 4;
}

static Fl_Named_Style* style = new Fl_Named_Style("Menu", revert, &style);

////////////////////////////////////////////////////////////////

class MenuTitle : public Fl_Menu_Window {
  void draw();
public:
  Fl_Widget* widget;
  MenuTitle(int X, int Y, int W, int H, Fl_Widget*, Fl_Group*);
};

MenuTitle::MenuTitle(int X, int Y, int W, int H, Fl_Widget* L, Fl_Group* button)
  : Fl_Menu_Window(X, Y, W, H, 0)
{
  end();
  copy_style(button->style());
  set_modal();
  clear_border();
  widget = L;
  // can't use sgi overlay for images:
  if (L->image()) clear_overlay();
}

void MenuTitle::draw() {
  Fl_Boxtype box = text_box(); // used for menubars
  Fl_Flags f = FL_VALUE|FL_SELECTED;
  if (box == FL_NO_BOX) {
    box = Fl_Widget::default_style->box; // used for popup menus
    f = FL_SELECTED;
  }
  box->draw(this, 0, 0, w(), h(), f);
  // this allow a toggle or other widget to preview it's state:
  if (Fl::event_pushed()) Fl::pushed_ = widget;
  widget->x(5);
  widget->y((h()-widget->height())/2);
  int save_w = widget->w(); widget->w(w()-10);
  fl_color(selection_text_color()); widget->draw();
  widget->w(save_w);
  Fl::pushed_ = 0;
}

////////////////////////////////////////////////////////////////

class MenuWindow : public Fl_Menu_Window {
  void draw();
public:
  int which_item;	// which item in parent's menu this is
  int real_leading;	// includes the size of the text_box()+leading()
  MenuTitle* title;
  int is_menubar;
  int numitems;
  int selected;
  int drawn_selected;	// last redraw has this selected
  Fl_Group* group;
  MenuWindow(Fl_Group*, int X, int Y, int W, int H,
	     Fl_Widget* title, Fl_Menu_* button);
  ~MenuWindow();
  int find_selected(int mx, int my);
  int titlex(int);
  int autoscroll(int);
  void position(int x, int y);
  int ypos(int);
};

MenuWindow::MenuWindow(Fl_Group* m, int X, int Y, int Wp, int Hp,
		       Fl_Widget* t, Fl_Menu_* button)
  : Fl_Menu_Window(X, Y, Wp, Hp, 0)
{
  end();
  set_modal();
  clear_border();
  style(::style);

  group = m;
  numitems = m ? m->children() : 0;
  selected = m ? m->focus() : -1;

  drawn_selected = -1;

  is_menubar = !button;
  if (is_menubar) {
    title = 0;
    return;
  }

  // If pulldown is going to point at a submenu, we must make the
  // higher-level menus have minimum width:
  if (selected >= 0) {
    Fl_Widget* widget = m->child(selected);
    if (widget->active() && widget->is_group() && ((Fl_Group*)widget)->focus()>=0)
      Wp = Hp = 0;
  }

  int dx=0; int dy=0; int dw=0; int dh=0; text_box()->inset(dx,dy,dw,dh);
  real_leading = leading()-dh;

  int Wtitle = 0;
  int Htitle = 0;
  if (t) {
    Wtitle = t->width()+10;
    Htitle = t->height()+real_leading;
  }

  int W = 0;
  int hotKeysW = 0;
  int H = 0;
  int selected_y = 0;
  for (int i = 0; i < numitems; i++) {
    Fl_Widget* widget = group->child(i);
    if (!widget->visible()) continue;
    int ih = widget->height();
    if (i == selected) selected_y = H+(ih+real_leading)/2;
    H += ih+real_leading;
    int iw = widget->width();
    if (iw > W) W = iw;
    if (widget->is_group()) {
      if (16 > hotKeysW) hotKeysW = 16;
    } else if (widget->shortcut()) {
      int w1 = fl_width(fl_shortcut_label(widget->shortcut())) + 8;
      if (w1 > hotKeysW) hotKeysW = w1;
    }
    // can't use sgi overlay for images:
    if (widget->image()) clear_overlay();
  }
  W -= dw;

  dx = dy = dw = dh = 0; box()->inset(dx,dy,dw,dh);

  W += hotKeysW-dw;
  if (Wp > W) W = Wp;
  if (Wtitle > W) W = Wtitle;

  if (!Wp) {
    if (selected >= 0) X -= W/2;
    if (X < 0) X = 0;
    if (X > Fl::w()-W) X= Fl::w()-W;
  }

  x(X); w(W); h(H-dh);
  if (selected >= 0) Y += Hp/2-selected_y-dy; else Y += Hp;
  if (group) y(Y); else {y(Y-2); w(1); h(1);}

  if (t) {
    title = new MenuTitle(X, Y-Htitle-2+dh, Wtitle, Htitle-dh, t, button);
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

// scroll so item i is visible on screen, return true if it moves
int MenuWindow::autoscroll(int i) {
  if (is_menubar) return 0;
  int Y = y()+ypos(i);
  if (Y <= Fl::y()) Y = Fl::y()-Y+10;
  else if (i <= 0) return 0;
  else {
    Fl_Widget* widget = group->child(i-1);
    Y = Y+widget->height()-Fl::h()-Fl::y();
    if (Y < 0) return 0;
    Y = -Y-10;
  }
  Fl_Menu_Window::position(x(), y()+Y);
  // y(y()+Y); // don't wait for response from X
  return 1;
}

// return the top edge of item:
int MenuWindow::ypos(int index) {
  int x=0; int y=0; int w=0; int h=0; box()->inset(x,y,w,h);
  for (int i = 0; i < index; i++) {
    Fl_Widget* widget = group->child(i);
    if (!widget->visible()) continue;
    y += widget->height()+real_leading;
  }
  return y;
}

void MenuWindow::draw() {
  if (damage() != FL_DAMAGE_CHILD) draw_box(0, 0, w(), h(), FL_FRAME_ONLY);
  int x=0; int y=0; int w=this->w(); int h=0; box()->inset(x,y,w,h);
  for (int i = 0; i < numitems; i++) {
    Fl_Widget* widget = group->child(i);
    if (!widget->visible()) continue;
    int ih = widget->height();
    // for minimal update, only draw the items that changed selection:
    if (damage() != FL_DAMAGE_CHILD || i == selected || i == drawn_selected) {

      Fl_Flags flags = widget->flags() & ~(FL_VALUE|FL_SELECTED);
      if (i == selected && !(flags & FL_OUTPUT)) {
	widget->set_flag(FL_SELECTED);
	flags |= FL_VALUE|FL_SELECTED;
	// this allow a toggle or other widget to preview it's state:
	if (Fl::event_pushed() && widget->takesevents()) Fl::pushed_ = widget;
      } else {
	widget->clear_flag(FL_SELECTED);
      }

      int X = x; int Y = y; int W = w; int H = ih+real_leading;
      text_box()->draw(this, X, Y, W, H, flags);
      text_box()->inset(X, Y, W, H);
      widget->x(X);
      widget->y(Y+leading()/2);
      int save_w = widget->w(); widget->w(W);

      widget->draw();
      widget->w(save_w);
      Fl::pushed_ = 0;

      if (widget->is_group()) {
	// Use the item's fontsize for the size of the arrow, rather than h:
	int nh = widget->label_size()+2;
	widget->draw_glyph(FL_GLYPH_RIGHT, X+W-nh, Y+(H-nh)/2, nh, nh, flags);
      } else if (widget->shortcut()) {
	flags = (flags & (~FL_ALIGN_MASK)) | FL_ALIGN_RIGHT;
	Fl_Color c = (flags&FL_SELECTED) ? widget->selection_text_color() :
	  widget->text_color();
	widget->label_type()->draw(fl_shortcut_label(widget->shortcut()),
			X, Y, W-3, H, c, flags);
      }
    }
    y += ih+real_leading;
  }
  drawn_selected = selected;
}

int MenuWindow::find_selected(int mx, int my) {
  if (numitems<1) return -1;
  mx -= x();
  my -= y();
  if (my < 0 || my >= h()) return -1;
  if (is_menubar) {
    int x = 3; int i = 0;
    for (;;i++) {
      if (i >= numitems) return -1;
      Fl_Widget* widget = group->child(i);
      if (!widget->visible()) continue;
      x += widget->width()+10;
      if (x > mx) break;
    }
    return i;
  } else {
    int x=0; int y=0; int w=this->w(); int h=this->h(); box()->inset(x,y,w,h);
    if (mx < x || mx >= w) return -1;
    int i; for (i = 0; i < numitems-1; i++) {
      Fl_Widget* widget = group->child(i);
      if (!widget->visible()) continue;
      y += widget->height()+real_leading;
      if (y > my) break;
    }
    if (i < 0) return 0;
    if (i >= numitems) return numitems-1;
    return i;
  }
}

// return horizontal position for item i in a menubar:
int MenuWindow::titlex(int index) {
  int x = 3;
  for (int i = 0; i < index; i++) {
    Fl_Widget* widget = group->child(i);
    if (!widget->visible()) continue;
    x += widget->width()+10;
  }
  return x;
}

////////////////////////////////////////////////////////////////
// The Fl_Menu_::popup() run-time:

struct MenuState {
  int menu_number; // which menu it is in
  int item_number; // which item in that menu, -1 if none
  MenuWindow* menus[20]; // pointers to menus
  int nummenus;
  int menubar; // if true menus[0] is a menubar
  int state; // one of the enumeration below
  Fl_Menu_* widget; // widget that is making this menu
};

enum {INITIAL_STATE = 0,// no mouse up or down since popup() called
	PUSH_STATE,	// mouse has been pushed on a normal item
	DONE_STATE,	// all done, execute the item
	ABORT_STATE};	// all done, don't execute anything

static inline void setitem(MenuState& p, int m, int n) {
  p.menu_number = m;
  p.item_number = n;
  if (p.menus[m]->selected != n) {
    p.menus[m]->selected = n;
    p.menus[m]->damage(FL_DAMAGE_CHILD);
  }
}

static int forward(MenuState& p, int menu) {
  // go to next item in menu menu if possible
  MenuWindow &m = *(p.menus[menu]);
  int item = (menu == p.menu_number) ? p.item_number : m.selected;
  while (++item < m.numitems) {
    if (m.group->child(item)->takesevents()) {setitem(p, menu, item); return 1;}
  }
  return 0;
}

static int backward(MenuState& p, int menu) {
  // previous item in menu menu if possible
  MenuWindow &m = *(p.menus[menu]);
  int item = (menu == p.menu_number) ? p.item_number : m.selected;
  if (item < 0) item = m.numitems;
  while (--item >= 0) {
    if (m.group->child(item)->takesevents()) {setitem(p, menu, item); return 1;}
  }
  return 0;
}

static int last_event;

static int handle(int e, void* data) {
  MenuState &p = *((MenuState*)data);
  Fl_Widget* widget = 0;
  last_event = e;
  switch (e) {

  case FL_KEYBOARD:
  case FL_SHORTCUT: {
    switch (Fl::event_key()) {
    case FL_Up:
      if (p.menubar && p.menu_number == 0) ;
      else if (backward(p, p.menu_number));
      //else if (p.menubar && p.menu_number==1) setitem(p, 0, p.menus[0]->selected);
      return 1;
    case FL_Down:
      if (p.menu_number || !p.menubar) forward(p, p.menu_number);
      else if (p.menu_number < p.nummenus-1) forward(p, p.menu_number+1);
      return 1;
    case FL_Right:
      if (p.menubar && (p.menu_number<=0 || p.menu_number==1 && p.nummenus==2))
	forward(p, 0);
      else if (p.menu_number < p.nummenus-1) forward(p, p.menu_number+1);
      return 1;
    case FL_Left:
      if (p.menubar && p.menu_number<=1) backward(p, 0);
      else if (p.menu_number>0)
	setitem(p, p.menu_number-1, p.menus[p.menu_number-1]->selected);
      return 1;
    case ' ':
    case FL_Enter:
      goto EXECUTE;
    case FL_Escape:
      p.state = ABORT_STATE;
      return 1;
    }
    for (int menu = p.nummenus; menu--;) {
      MenuWindow &mw = *(p.menus[menu]);
      for (int item = 0; item < mw.numitems; item++) {
	widget = mw.group->child(item);
	if (widget->takesevents() && widget->test_shortcut()) {
	  setitem(p, menu, item);
	  goto EXECUTE;
	}
      }
    }} break;

  case FL_KEYUP:
    if ((Fl::event_key() == FL_Alt_L || Fl::event_key() == FL_Alt_R)
	&& Fl::event_clicks()) {
      // checking for event_clicks insures that the keyup matches the
      // keydown that preceeded it, so Alt was pressed & released without
      // any intermediate values.
      p.state = ABORT_STATE;
      return 1;
    } else
      return 0;

  case FL_PUSH:
  case FL_MOVE:
  case FL_DRAG: {
    int mx = Fl::event_x_root();
    int my = Fl::event_y_root();
    int item=0; int menu;
    for (menu = p.nummenus-1; ; menu--) {
      item = p.menus[menu]->find_selected(mx, my);
      if (item >= 0) break;
      if (menu <= 0) {menu = p.nummenus-1; item = -1; break;}
    }
    if (e == FL_PUSH) {
      p.state = PUSH_STATE;
      if (item >= 0) {
	Fl_Widget* widget = p.menus[menu]->group->child(item);
  	if (widget->is_group() // this is a submenu title
  	    && item != p.menus[menu]->selected) // and it is not already on
  	  p.state = INITIAL_STATE;
	// redraw checkboxes so they preview the state they will be in:
	if (widget->type()==FL_TOGGLE_ITEM || widget->type()==FL_RADIO_ITEM)
	  p.menus[menu]->damage(FL_DAMAGE_CHILD);
      }
    }
    setitem(p, menu, item);
    return 1;}

  case FL_RELEASE:
    Fl::pushed_ = 0;
    // Allow menus to be "clicked-up".  Without this a single click will
    // pick whatever item the mouse is pointing at:
    if (p.state == INITIAL_STATE && Fl::event_is_click()) {
      // redraw checkboxes so they preview the state they will be in:
      if (p.item_number >= 0) {
	widget = p.menus[p.menu_number]->group->child(p.item_number);
	if (widget->type()==FL_TOGGLE_ITEM || widget->type()==FL_RADIO_ITEM)
	  p.menus[p.menu_number]->damage(FL_DAMAGE_CHILD);
      }
      return 1;
    }
  EXECUTE: // execute the item pointed to by w and current item
    // If they click outside menu we quit:
    if (p.item_number < 0) {p.state = ABORT_STATE; return 1;}
    // Do nothing if they click inactive items:
    widget = p.menus[p.menu_number]->group->child(p.item_number);
    if (!widget->takesevents() || widget->is_group() && !widget->user_data()) {
      // except if they click on an already-up menu title, in which case
      // we are done:
      // if (p.menubar && !p.menu_number && Fl::event_is_click())
      //   p.state = ABORT_STATE;
      return 1;
    }
    if ((widget->flags() & FL_MENU_STAYS_UP) && (!p.menubar || p.menu_number)) {
      p.widget->execute(widget);
      Fl_Window* mw = p.menus[p.menu_number];
      if (widget->type() == FL_RADIO_ITEM) mw->redraw();
      else if (widget->type() == FL_TOGGLE_ITEM) mw->damage(FL_DAMAGE_CHILD);
    } else {
      p.state = DONE_STATE;
    }
    return 1;
  }
  return 0;
}

static void autoscroll_timeout(void* data) {
  if (last_event == FL_DRAG || last_event == FL_MOVE) handle(FL_MOVE, data);
}

int Fl_Menu_::pulldown(
    int X, int Y, int W, int H,
    Fl_Widget* t,
    int menubar)
{
  Fl_Group::current(0); // fix possible programmer error...

  // figure out where to pop up in screen coordinates:
  if (W) for (Fl_Window* w = window(); w; w = w->window()) {
    X += w->x();
    Y += w->y();
  } else {
    X += Fl::event_x_root()-Fl::event_x();
    Y += Fl::event_y_root()-Fl::event_y();
  }

  MenuWindow mw(this, X, Y, W, H, t, menubar ? 0 : this);
  MenuState p;
  p.menus[0] = &mw;
  p.nummenus = 1;
  p.menubar = menubar;
  p.widget = this;
  p.state = INITIAL_STATE;
  Fl::grab(::handle, &p);

  if (menubar) {
    p.menu_number = 0;
    p.item_number = p.menus[0]->selected;
    if (p.item_number < 0)
      ::handle(FL_PUSH, &p); // get menu mouse points at to appear
  } else {
    // create submenus until we locate the one with selected item
    // in it, positioning them so that one is selected:
    for (;;) {
      p.menu_number = p.nummenus-1;
      MenuWindow* mw = p.menus[p.nummenus-1];
      p.item_number = mw->selected;
      if (p.item_number < 0) break;
      Fl_Widget* widget = mw->group->child(p.item_number);
      if (!widget->takesevents() || !widget->is_group()) break;
      Fl_Group* group = (Fl_Group*)widget;
      if (group->focus() < 0) break;
      int nX = mw->x() + mw->w();
      int nY = mw->y() + mw->ypos(p.item_number)-mw->ypos(0);
      MenuWindow* n = new MenuWindow(group, X,Y,W,H, 0, this);
      n->which_item = p.item_number;
      p.menus[p.nummenus++] = n;
      // move all earlier menus to line up with this new one:
      if (n->selected >= 0) {
	int dy = n->y()-nY;
	int dx = n->x()-nX;
	for (int menu = 0; menu <= p.menu_number; menu++) {
	  MenuWindow* t = p.menus[menu];
	  int nx = t->x()+dx; if (nx < 0) {nx = 0; dx = -t->x();}
	  int ny = t->y()+dy; if (ny < 0) {ny = 0; dy = -t->y();}
	  t->position(nx, ny);
	}
      }
    }
    // show all the menus:
    for (int menu = 0; menu <= p.menu_number; menu++) {
      MenuWindow* mw = p.menus[menu];
      if (mw->title) mw->title->show();
      mw->show();
    }
  }

  int oldmenu = 0;
  int olditem = -1;

  MenuWindow* fakemenu = 0;

  for (; p.state < DONE_STATE; Fl::wait()) {
    if (p.item_number == olditem && p.menu_number == oldmenu) continue;
    olditem = p.item_number;
    oldmenu = p.menu_number;
    delete fakemenu; fakemenu = 0; // turn off "menubar button"
    if (p.item_number < 0) continue;

    MenuWindow* mw = p.menus[p.menu_number];
    if (p.menus[p.menu_number]->autoscroll(p.item_number)) {
      Fl::remove_timeout(autoscroll_timeout, &p);
      if (last_event == FL_DRAG || last_event == FL_MOVE)
	Fl::add_timeout(.1,autoscroll_timeout, &p);
    }

    Fl_Widget* widget = mw->group->child(p.item_number);

    if (widget->takesevents() && widget->is_group()) {
      // a submenu title has been selected
      if (p.nummenus > p.menu_number+1) {
	// there are already submenus up:
	// delete all menus greater than 2 below current:
	while (p.nummenus > p.menu_number+2) delete p.menus[--p.nummenus];
	// see if this is the same submenu as before:
	MenuWindow* oldmenu = p.menus[p.menu_number+1];
	if (oldmenu->which_item == p.item_number) {
	  // yes, leave it up with nothing selected:
	  if (oldmenu->selected >= 0) {
	    oldmenu->selected=-1; oldmenu->damage(FL_DAMAGE_CHILD);}
	  continue;
	}
	// otherwise delete the previous submenu:
	delete oldmenu;
	p.nummenus--;
      }	
      // figure out where new menu goes:
      int nX, nY;
      Fl_Widget* title;
      if (!p.menu_number && p.menubar) { // menu off a menubar:
	nX = mw->x() + mw->titlex(p.item_number);
	nY = mw->y() + mw->h();
	title = widget;
      } else {
	nX = mw->x() + mw->w();
	nY = mw->y() + mw->ypos(p.item_number)-mw->ypos(0);
	title = 0;
      }
      ((Fl_Group*)widget)->focus(-1); // don't preselect anything on this menu
      p.menus[p.nummenus++] = mw =
	new MenuWindow((Fl_Group*)widget, nX, nY, 0, 0, title, this);
      mw->which_item = p.item_number;
      if (title) goto SHOW_MENUBAR_TITLE;
      mw->show();

    } else {
      // a normal item (not a submenu title) is selected
      // delete all the submenus:
      while (p.nummenus > p.menu_number+1) delete p.menus[--p.nummenus];

      // make a fake menu to display buttons in menubars correctly:
      if (!p.menu_number && p.menubar) {
	fakemenu = new MenuWindow(0,
			mw->x() + mw->titlex(p.item_number),
			mw->y() + mw->h(), 0, 0, widget, this);
	mw = fakemenu;
      SHOW_MENUBAR_TITLE:
	// fix the title box size to match menubar thickness:
	int dx=0; int y1=Y; int dw=0; int h1=this->h();
	box()->inset(dx,y1,dw,h1);
	mw->title->y(y1+1);
	mw->title->h(h1-2);
	mw->title->show();
	if (mw != fakemenu) mw->show();
      }
    }
  }

  Fl::remove_timeout(autoscroll_timeout, &p);

  Fl_Widget* widget = 0;
  if (p.state == DONE_STATE) {
    widget = p.menus[p.menu_number]->group->child(p.item_number);
    // set the value of all the submenus:
    for (int i=0; i < p.nummenus; i++)
      p.menus[i]->group->focus(p.menus[i]->selected);
  }

  delete fakemenu;
  while (--p.nummenus) delete p.menus[p.nummenus];
  mw.hide();
  Fl::release();

  if (widget) {
    execute(widget);
    if (menubar && !p.menu_number &&
	(widget->type() == FL_RADIO_ITEM || widget->type() == FL_TOGGLE_ITEM))
      redraw();
    return 1;
  }
  else return 0;
}

int Fl_Menu_::popup(int X, int Y, const char* title) {
  Fl_Group::current(0);
  Fl_Item dummy(title);
  return pulldown(X, Y, 0, 0, title ? &dummy : 0);
}

//
// End of "$Id: Fl_Menu.cxx,v 1.92 2000/08/10 09:24:31 spitzak Exp $".
//
