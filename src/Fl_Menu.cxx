//
// "$Id: Fl_Menu.cxx,v 1.105 2001/03/08 07:39:05 clip Exp $"
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

#define MAX_LEVELS 30

class MenuWindow;

struct MenuState {
  int level;	// which level of nesting current item is in
  int indexes[MAX_LEVELS]; // index in each level of selected item
  MenuWindow* menus[MAX_LEVELS]; // windows that have been created
  int nummenus; // number of windows (may be level+1 or level+2)
  int menubar; // if true menus[0] is a menubar
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
};

////////////////////////////////////////////////////////////////

// The box, color, and selection color of all menus is taken from
// this. The color is rather clumsily replaced with the text_background
// of the calling widget to allow compatability with fltk 1.0 and
// to allow popup menus to have different colors.

static void revert(Fl_Style *s) {
  s->text_box = FL_NO_BOX;
}

static Fl_Named_Style* style = new Fl_Named_Style("Menu", revert, &style);

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
  set_modal();
  set_override();
  widget = L;
  // can't use sgi overlay for images:
  if (L->image()) clear_overlay();
}

// In order to draw the rather inconsiste Windows-style menubars, I use
// the text_box() of the menubar to draw a box around the selected items.
// If Windows was consistent they would use a dark blue selection box
// like earlier versions of NT did. Sigh...

void MenuTitle::draw() {

  Fl_Widget* style_widget = menustate->widget;
  Fl_Boxtype box = style_widget->text_box();
  // popup menus may have no box set:
  if (box == FL_NO_BOX) box = Fl_Widget::default_style->box;

  Fl_Flags flags;
  if (!menustate->menubar) {
    // a title on a popup menu
    flags = 0;
  } else if (box == FL_FLAT_BOX) {
    // NT 4 style
    flags = FL_SELECTED; widget->set_flag(FL_SELECTED);
  } else {
    // Windows98 style
    flags = FL_VALUE;
  }

  box->draw(style_widget, 0, 0, w(), h(), flags);

  // this allow a toggle or other widget to preview it's state:
  if (Fl::event_pushed()) Fl::pushed_ = widget;
  fl_x_ = 5;
  fl_y_ = (h()-widget->height())/2;
  int save_w = widget->w(); widget->w(w()-10);
  widget->draw();
  widget->w(save_w);
  widget->clear_flag(FL_SELECTED);
  Fl::pushed_ = 0;
  fl_x_ = fl_y_ = 0;
}

////////////////////////////////////////////////////////////////

class MenuWindow : public Fl_Menu_Window {
  void draw();
public:
  MenuState* menustate;
  int level;
  MenuTitle* title;
  int is_menubar;
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
};

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
  set_modal();
  set_override();
  style(::style);

  int selected = l <= menustate->level ? menustate->indexes[l] : -1;

  drawn_selected = -1;

  is_menubar = !level && menustate->menubar;
  if (is_menubar) {
    title = 0;
    return;
  }

  int leading = menustate->widget->leading(); // +2 ?

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
      int w1 = fl_width(Fl::key_name(widget->shortcut())) + 8;
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

// scroll so item i is visible on screen, return true if it moves
int MenuWindow::autoscroll(int i) {
  if (is_menubar) return 0;
  int Y = y()+ypos(i);
  if (Y <= Fl::y()) Y = Fl::y()-Y+10;
  else if (i <= 0) return 0;
  else {
    Fl_Widget* widget = get_widget(i-1);
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
  int leading = menustate->widget->leading(); // +2 ?
  for (int i = 0; i < index; i++) {
    Fl_Widget* widget = get_widget(i);
    if (!widget->visible()) continue;
    y += widget->height()+leading;
  }
  return y;
}

void MenuWindow::draw() {
  if (damage() != FL_DAMAGE_CHILD) box()->draw(this,0,0,w(),h(),0);
  int x=0; int y=0; int w=this->w(); int h=0; box()->inset(x,y,w,h);
  int selected = level <= menustate->level ? menustate->indexes[level] : -1;
  int leading = menustate->widget->leading(); // +2 ?
  for (int i = 0; ; i++) {
    Fl_Widget* widget = get_widget(i);
    if (!widget) break;
    if (!widget->visible()) continue;
    int ih = widget->height() + leading;
    // for minimal update, only draw the items that changed selection:
    if (damage() != FL_DAMAGE_CHILD || i == selected || i == drawn_selected) {

      Fl_Flags flags = widget->flags()&~(FL_VALUE|FL_SELECTED|FL_ALIGN_MASK);
      if (i == selected && !(flags & FL_OUTPUT)) {
	flags = FL_SELECTED; widget->set_flag(FL_SELECTED);
	// this allow a toggle or other widget to preview it's state:
	if (Fl::event_pushed() && widget->takesevents()) Fl::pushed_ = widget;
	fl_color(menustate->widget->selection_color());
	fl_rectf(x,y,w,ih);
      } else {
	widget->clear_flag(FL_SELECTED);
	// erase the background if only doing partial update. This uses
	// clipping so background pixmaps will work:
	if (damage() == FL_DAMAGE_CHILD) {
	  fl_push_clip(x,y,w,ih);
	  box()->draw(this,0,0,this->w(),this->h(),0);
	  fl_pop_clip();
	}
      }
      if (!widget->active_r()) flags |= FL_INACTIVE;
      fl_x_ = x;
      fl_y_ = y+leading/2;
      int save_w = widget->w(); widget->w(w);
      widget->draw();
      widget->w(save_w);
      Fl::pushed_ = 0;
      fl_x_ = fl_y_ = 0;

      if (is_parent(i)) {
	// Use the item's fontsize for the size of the arrow, rather than h:
        int nh = fl_height(widget->label_font(), widget->label_size());
        draw_glyph(FL_GLYPH_RIGHT, x+w-nh, y+(ih-nh)/2, nh, nh, flags);
      } else if (widget->shortcut()) {
        Fl_Color c = widget->get_label_color(flags);
        //fl_font(widget->text_font(), widget->text_size());
	widget->label_type()->draw(Fl::key_name(widget->shortcut()),
				   x, y, w-3, ih, c, flags | FL_ALIGN_RIGHT);
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
    int x = 3;
    for (int i = 0; ; i++) {
      Fl_Widget* widget = get_widget(i);
      if (!widget) return i-1;
      if (!widget->visible()) continue;
      x += widget->width()+10;
      if (x > mx) return i;
    }
  } else {
    int leading = menustate->widget->leading(); // +2 ?
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
	DONE_STATE,	// all done, execute the item
	ABORT_STATE};	// all done, don't execute anything

static inline void setitem(MenuState& p, int m, int n) {
  p.level = m;
  if (p.indexes[m] != n) {
    p.indexes[m] = n;
    p.menus[m]->damage(FL_DAMAGE_CHILD);
  }
  if (m+1 < p.nummenus && p.indexes[m+1] >= 0)
    p.menus[m+1]->damage(FL_DAMAGE_CHILD);
  p.indexes[m+1] = -1;
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
      if (p.menubar && p.level == 0) ;
      else if (backward(p, p.level));
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
      p.state = ABORT_STATE;
      return 1;
    }
    for (int menu = p.nummenus; menu--;) {
      MenuWindow &mw = *(p.menus[menu]);
      for (int item = 0; ; item++) {
	widget = mw.get_widget(item);
	if (!widget) break;
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
  	if (p.menus[menu]->is_parent(item) // this is a submenu title
  	    && item != p.indexes[menu]) // and it is not already on
  	  p.state = INITIAL_STATE;
	// redraw checkboxes so they preview the state they will be in:
	Fl_Widget* widget = p.menus[menu]->get_widget(item);
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
      if (p.indexes[p.level] >= 0) {
	widget = p.current_widget();
	if (widget->type()==FL_TOGGLE_ITEM || widget->type()==FL_RADIO_ITEM)
	  p.menus[p.level]->damage(FL_DAMAGE_CHILD);
      }
      return 1;
    }
  EXECUTE: // execute the item pointed to by w and current item
    // If they click outside menu we quit:
    widget = p.current_widget();
    if (!widget) {p.state = ABORT_STATE; return 1;}
    // ignore clicks on inactive items:
    if (!widget->takesevents()) return 1;
    if ((widget->flags() & FL_MENU_STAYS_UP) && (!p.menubar || p.level)) {
      p.widget->goto_item(p.indexes, p.level);
      p.widget->execute();
      Fl_Window* mw = p.menus[p.level];
      if (widget->type() == FL_RADIO_ITEM) mw->redraw();
      else if (widget->type() == FL_TOGGLE_ITEM) mw->damage(FL_DAMAGE_CHILD);
    } else {
      // ignore clicks on menu titles unless it they have a callback:
      if (widget->callback() == Fl_Widget::default_callback &&
	  p.current_children() >= 0) return 1;
      p.state = DONE_STATE;
    }
    return 1;
  }
  return 0;
}

static void autoscroll_timeout(void* data) {
  if (last_event == FL_DRAG || last_event == FL_MOVE) handle(FL_MOVE, data);
}

int Fl_Menu_::popup(int X, int Y, const char* title) {
  Fl_Group::current(0);
  Fl_Item dummy(title);
  return pulldown(X, Y, 0, 0, title ? &dummy : 0);
}

int Fl_Menu_::pulldown(
    int X, int Y, int W, int H,
    Fl_Widget* t,
    int menubar)
{
  Fl_Group::current(0); // fix possible programmer error...

  ::style->color = text_background();

  // figure out where to pop up in screen coordinates:
  if (W) for (Fl_Widget* w = this; w; w = w->parent()) {
    X += w->x();
    Y += w->y();
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
  p.indexes[0] = focus();
  p.indexes[1] = -1;
  MenuWindow mw(&p, 0, X, Y, W, H, t);
  p.menus[0] = &mw;
  //Fl::local_grab(::handle, &p); // use this if testing!
  Fl::grab(::handle, &p);

  if (menubar) {
    if (focus() < 0)
      ::handle(FL_PUSH, &p); // get menu mouse points at to appear
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
      if (mw->title) mw->title->show();
      mw->show();
    }
  }

  int oldlevel = 0;
  int oldindex = -1;

  MenuWindow* fakemenu = 0;

  for (; p.state < DONE_STATE; Fl::wait()) {
    if (p.level == oldlevel && p.indexes[p.level] == oldindex) continue;
    oldlevel = p.level;
    oldindex = p.indexes[p.level];
    delete fakemenu; fakemenu = 0; // turn off "menubar button"
    if (oldindex < 0) continue;

    MenuWindow* mw = p.menus[p.level];
    if (p.menus[p.level]->autoscroll(oldindex)) {
      Fl::remove_timeout((Fl_Timeout_Handler)autoscroll_timeout, &p);
      if (last_event == FL_DRAG || last_event == FL_MOVE)
	Fl::add_timeout(.1,(Fl_Timeout_Handler)autoscroll_timeout, &p);
    }

    Fl_Widget* widget = p.current_widget();

    if (widget->takesevents() && p.current_children()>=0) {
      // a submenu title has been selected
      if (p.nummenus > p.level+1) {
	// there are already submenus up:
	// delete all menus greater than 2 below current:
	while (p.nummenus > p.level+2) delete p.menus[--p.nummenus];
	// see if this is the same submenu as before:
	MenuWindow* oldmenu = p.menus[p.level+1];
	// otherwise delete the previous submenu:
	delete oldmenu;
	p.nummenus--;
      }	
      // figure out where new menu goes:
      int nX, nY;
      Fl_Widget* title;
      if (!p.level && p.menubar) { // menu off a menubar:
	nX = mw->x() + mw->titlex(oldindex);
	nY = mw->y() + mw->h();
	title = widget;
      } else {
	nX = mw->x() + mw->w();
	nY = mw->y() + mw->ypos(oldindex)-mw->ypos(0);
	title = 0;
      }
      mw = new MenuWindow(&p, p.nummenus, nX, nY, 0, 0, title);
      p.menus[p.nummenus++] = mw;
      if (title) goto SHOW_MENUBAR_TITLE;
      mw->show();

    } else {
      // a normal item (not a submenu title) is selected
      // delete all the submenus:
      while (p.nummenus > p.level+1) delete p.menus[--p.nummenus];

      // make a fake menu to display buttons in menubars correctly:
      if (!p.level && p.menubar) {
	fakemenu = new MenuWindow(&p, 1,
			mw->x() + mw->titlex(oldindex),
			mw->y() + mw->h(), 0, 0, widget);
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

  Fl::remove_timeout((Fl_Timeout_Handler)autoscroll_timeout, &p);
  delete fakemenu;
  while (--p.nummenus) delete p.menus[p.nummenus];
  mw.hide();
  Fl::release();

  if (p.state == DONE_STATE) {
    goto_item(p.indexes, p.level);
    if (menubar && !p.level &&
	(item()->type() == FL_RADIO_ITEM || item()->type() == FL_TOGGLE_ITEM))
      redraw();
    execute();
    return 1;
  }
  return 0;
}

//
// End of "$Id: Fl_Menu.cxx,v 1.105 2001/03/08 07:39:05 clip Exp $".
//
