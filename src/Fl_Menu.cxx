//
// "$Id: Fl_Menu.cxx,v 1.83 2000/04/03 17:09:19 bill Exp $"
//
// Implementation of the Fl_Menu_ class, which includes most of the
// code needed to do popup menus and menubars.
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

// Implementation of the Fl_Menu_ base class (the trailing underscore
// is on the class name for historical reasons).  Most of this is
// the implementation of the pulldown()/popup() methods, which take
// over the machine with Fl::grab() and create temporary objects of
// class MenuWindow to display the popup menus.
// Warning: this menu code is quite a mess!
// Yes, it is.

#include <FL/Fl.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Item.H> // for FL_TOGGLE_ITEM, FL_RADIO_ITEM
#include <FL/fl_draw.H>

////////////////////////////////////////////////////////////////
// Styles used by parts of the menu:

// The style used by the actual pull-down windows created for menus.
// The window_box is used to draw the box around an item!

static void mw_revert(Fl_Style* s) {
  s->box = FL_UP_BOX;
  s->window_box = FL_FLAT_BOX;
  s->leading = 4;
}

static Fl_Named_Style* menu_style =
  new Fl_Named_Style("Menu", mw_revert, &menu_style);

// Style used by the little windows that serve as menu titles.  
// This style is also referred to by Fl_Menu_Bar.cxx:

static void mt_revert(Fl_Style* s) {
#if 0
  // NT 4.0 style
  s->box = FL_FLAT_BOX;
#else
  // Windows98 style:
  s->box = FL_HIGHLIGHT_BOX;
  s->selection_color = FL_GRAY;
  s->selection_text_color = FL_BLACK;
#endif
}

Fl_Named_Style* fl_title_style =
  new Fl_Named_Style("Menu_Title", mt_revert, &fl_title_style);

////////////////////////////////////////////////////////////////

// tiny window for title of menu:
class MenuTitle : public Fl_Menu_Window {
  void draw();
public:
  Fl_Widget* item;
  MenuTitle(int X, int Y, int W, int H, Fl_Widget*);
};

MenuTitle::MenuTitle(int X, int Y, int W, int H, Fl_Widget* L) :
  Fl_Menu_Window(X, Y, W, H, 0)
{
  end();
  style(fl_title_style);
  set_modal();
  clear_border();
  item = L;
  // can't use sgi overlay for images:
  if (L->image()) clear_overlay();
}

void MenuTitle::draw() {
  Fl_Color bgcolor = selection_color();
  Fl_Color label_color = selection_text_color();
  box()->draw(0, 0, w(), h(), bgcolor, FL_VALUE);
  // this allow a toggle or other widget to preview it's state:
  if (Fl::pushed()) Fl::pushed_ = item;
  item->x(5);
  item->y((h()-item->h())/2);
  int save_w = item->w(); item->w(w()-10);
  fl_color(label_color); item->draw();
  item->w(save_w);
  if (Fl::pushed()) Fl::pushed_ = Fl::grab_;
}

////////////////////////////////////////////////////////////////

// each vertical menu has one of these:
class MenuWindow : public Fl_Menu_Window {
  void draw();
public:
  int which_item;	// which item in parent's menu this is
  int real_leading;	// includes the size of the window_box()+leading()
  MenuTitle* title;
  int handle(int);
  int is_menubar;
  int numitems;
  int selected;
  int drawn_selected;	// last redraw has this selected
  Fl_Group* list;
  MenuWindow(Fl_Group*, int X, int Y, int W, int H,
	     Fl_Widget* title, int menubar=0);
  ~MenuWindow();
  int find_selected(int mx, int my);
  int titlex(int);
  void autoscroll(int);
  void position(int x, int y);
  int ypos(int);
};

MenuWindow::MenuWindow(Fl_Group* m, int X, int Y, int Wp, int Hp,
		       Fl_Widget* t, int menubar)
  : Fl_Menu_Window(X, Y, Wp, Hp, 0)
{
  end();
  set_modal();
  clear_border();
  style(menu_style);

  list = m;
  numitems = m ? m->children() : 0;
  selected = m ? m->focus() : -1;

  drawn_selected = -1;

  is_menubar = menubar;
  if (menubar) {
    title = 0;
    return;
  }

  // If pulldown is going to point at a submenu, we must make the
  // higher-level menus have minimum width:
  if (selected >= 0) {
    Fl_Widget* w = m->child(selected);
    if (w->active() && w->is_group() && ((Fl_Group*)w)->focus() >= 0)
      Wp = Hp = 0;
  }

  int dx=0; int dy=0; int dw=0; int dh=0; window_box()->inset(dx,dy,dw,dh);
  real_leading = leading()-dh;

  int Wtitle = 0;
  int Htitle = 0;
  if (t) {
    if (!t->h() || t->damage() & FL_DAMAGE_LAYOUT) t->layout();
    Wtitle = t->w()+10;
    Htitle = t->h()+real_leading;
  }

  int W = 0;
  int hotKeysW = 0;
  int H = 0;
  int selected_y = 0;
  for (int i = 0; i < numitems; i++) {
    Fl_Widget* o = list->child(i);
    if (!o->visible()) continue;
    if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
    if (i == selected) selected_y = H+(o->h()+real_leading)/2+1;
    H += o->h()+real_leading;
    if (o->w() > W) W = o->w();
    if (o->is_group()) {
      if (16 > hotKeysW) hotKeysW = 16;
    } else if (o->shortcut()) {
      int w1 = fl_width(fl_shortcut_label(o->shortcut())) + 8;
      if (w1 > hotKeysW) hotKeysW = w1;
    }
    // can't use sgi overlay for images:
    if (o->image()) clear_overlay();
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
  if (selected >= 0) Y += Hp/2-selected_y; else Y += Hp;
  if (list) y(Y-1); else {y(Y-3); w(1); h(1);}

  if (t) {
    title = new MenuTitle(X, Y-Htitle-3, Wtitle, Htitle, t);
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

////////////////////////////////////////////////////////////////

// return the top edge of item:
int MenuWindow::ypos(int index) {
  int x=0; int y=0; int w=0; int h=0; box()->inset(x,y,w,h);
  for (int i = 0; i < index; i++) {
    Fl_Widget* o = list->child(i);
    if (!o->visible()) continue;
    if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
    y += o->h()+real_leading;
  }
  return y;
}

void MenuWindow::draw() {
  if (damage() != FL_DAMAGE_CHILD)
    box()->draw(0, 0, w(), h(), color(), FL_FRAME_ONLY);
  int x=0; int y=0; int w=this->w(); int h=0; box()->inset(x,y,w,h);
  for (int i = 0; i < numitems; i++) {
    Fl_Widget* o = list->child(i);
    if (!o->visible()) continue;
    if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
    // for minimal update, only draw the items that changed selection:
    if (damage() != FL_DAMAGE_CHILD || i == selected || i == drawn_selected) {

      Fl_Flags flags = o->flags() & ~FL_VALUE;
      Fl_Color bgcolor = color();
      Fl_Color label_color = o->label_color();
      if (i == selected && !(flags & FL_OUTPUT)) {
	flags |= FL_VALUE;
	bgcolor = selection_color();
	label_color = selection_text_color();
	// this allow a toggle or other widget to preview it's state:
	if (Fl::pushed() && o->takesevents()) Fl::pushed_ = o;
      }

      int X = x; int Y = y; int W = w; int H = o->h()+real_leading;
      window_box()->draw(X, Y, W, H, bgcolor, flags);
      window_box()->inset(X, Y, W, H);
      o->x(X);
      o->y(Y+leading()/2);
      int save_w = o->w(); o->w(W);
      fl_color(label_color); o->draw();
      o->w(save_w);
      if (Fl::pushed()) Fl::pushed_ = Fl::grab_;

      if (o->is_group()) {
	// Use the item's fontsize for the size of the arrow, rather than h:
	int nh = o->label_size()+2;
	Y += (H-nh)/2;
	H = nh;
	glyph()(FL_GLYPH_RIGHT, X+W-H, Y, H, H, bgcolor,
		label_color, flags, FL_NO_BOX);
      } else if (o->shortcut()) {
	flags = flags & ~FL_ALIGN_MASK | FL_ALIGN_RIGHT;
	o->label_type()->draw(fl_shortcut_label(o->shortcut()),
			      X, Y, W-3, H, label_color, flags);
      }
    }
    y += o->h()+real_leading;
  }
  drawn_selected = selected;
}

////////////////////////////////////////////////////////////////

int MenuWindow::find_selected(int mx, int my) {
  if (numitems<1) return -1;
  mx -= x();
  my -= y();
  if (my < 0 || my >= h()) return -1;
  if (is_menubar) {
    int x = 3; int i = 0;
    for (;;i++) {
      if (i >= numitems) return -1;
      Fl_Widget* o = list->child(i);
      if (!o->visible()) continue;
      if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
      x += o->w()+10;
      if (x > mx) break;
    }
    return i;
  } else {
    int x=0; int y=0; int w=this->w(); int h=this->h(); box()->inset(x,y,w,h);
    if (mx < x || mx >= w) return -1;
    int i; for (i = 0; i < numitems-1; i++) {
      Fl_Widget* o = list->child(i);
      if (!o->visible()) continue;
      if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
      y += o->h()+real_leading;
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
    Fl_Widget* o = list->child(i);
    if (!o->visible()) continue;
    if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
    x += o->w()+10;
  }
  return x;
}

////////////////////////////////////////////////////////////////
// Fl_Menu_::popup():

// Because Fl::grab() is done, all events go to one of the menu windows.
// But the handle method needs to look at all of them to find out
// what item the user is pointing at.  And it needs a whole lot
// of other state variables to determine what is going on with
// the currently displayed menus.
// So the main loop (handlemenu()) puts all the state in a structure
// and puts a pointer to it in a static location, so the handle()
// on menus can refer to it and alter it.  The handle() method
// changes variables in this state to indicate what item is
// picked, but does not actually alter the display, instead the
// main loop does that.  This is because the X mapping and unmapping
// of windows is slow, and we don't want to fall behind the events.

// values for menustate.state:
#define INITIAL_STATE 0	// no mouse up or down since popup() called
#define PUSH_STATE 1	// mouse has been pushed on a normal item
#define DONE_STATE 2	// all done, execute the item
#define ABORT_STATE 3	// all done, don't execute anything

struct menustate {
  int menu_number; // which menu it is in
  int item_number; // which item in that menu, -1 if none
  MenuWindow* menus[20]; // pointers to menus
  int nummenus;
  int menubar; // if true menus[0] is a menubar
  int state;
  Fl_Menu_* widget; // widget that is making this menu
};

static menustate* p;

static inline void setitem(int m, int n) {
  p->menu_number = m;
  p->item_number = n;
  if (p->menus[m]->selected != n) {
    p->menus[m]->selected = n;
    p->menus[m]->damage(FL_DAMAGE_CHILD);
  }
}

static int forward(int menu) { // go to next item in menu menu if possible
  menustate &p = *(::p);
  MenuWindow &m = *(p.menus[menu]);
  int item = (menu == p.menu_number) ? p.item_number : m.selected;
  while (++item < m.numitems) {
    Fl_Widget* w = m.list->child(item);
    if (w->takesevents()) {setitem(menu, item); return 1;}
  }
  return 0;
}

static int backward(int menu) { // previous item in menu menu if possible
  menustate &p = *(::p);
  MenuWindow &m = *(p.menus[menu]);
  int item = (menu == p.menu_number) ? p.item_number : m.selected;
  if (item < 0) item = m.numitems;
  while (--item >= 0) {
    Fl_Widget* w = m.list->child(item);
    if (w->takesevents()) {setitem(menu, item); return 1;}
  }
  return 0;
}

static int last_event;

int MenuWindow::handle(int e) {
  menustate &p = *(::p);
  Fl_Widget* w;
  last_event = e;
  switch (e) {

  case FL_KEYBOARD:
    switch (Fl::event_key()) {
    case FL_Up:
      if (p.menubar && p.menu_number == 0) ;
      else if (backward(p.menu_number));
      //else if (p.menubar && p.menu_number==1) setitem(0, p.menus[0]->selected);
      return 1;
    case FL_Down:
      if (p.menu_number || !p.menubar) forward(p.menu_number);
      else if (p.menu_number < p.nummenus-1) forward(p.menu_number+1);
      return 1;
    case FL_Right:
      if (p.menubar && (p.menu_number<=0 || p.menu_number==1 && p.nummenus==2))
	forward(0);
      else if (p.menu_number < p.nummenus-1) forward(p.menu_number+1);
      return 1;
    case FL_Left:
      if (p.menubar && p.menu_number<=1) backward(0);
      else if (p.menu_number>0)
	setitem(p.menu_number-1, p.menus[p.menu_number-1]->selected);
      return 1;
    case ' ':
    case FL_Enter:
      goto EXECUTE;
    case FL_Escape:
      p.state = ABORT_STATE;
      return 1;
    }
    break;

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

  case FL_SHORTCUT: {
    for (int menu = p.nummenus; menu--;) {
      MenuWindow &mw = *(p.menus[menu]);
      for (int item = 0; item < mw.numitems; item++) {
	w = mw.list->child(item);
	if (w->takesevents() && w->test_shortcut()) {
	  setitem(menu, item);
	  goto EXECUTE;
	}
      }
    }} break;

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
	Fl_Widget* w = p.menus[menu]->list->child(item);
  	if (w->is_group() // this is a submenu title
  	    && item != p.menus[menu]->selected) // and it is not already on
  	  p.state = INITIAL_STATE;
	// redraw checkboxes so they preview the state they will be in:
	if (w->type()==FL_TOGGLE_ITEM || w->type()==FL_RADIO_ITEM)
	  p.menus[menu]->damage(FL_DAMAGE_CHILD);
      }
    }
    setitem(menu, item);
    return 1;}

  case FL_RELEASE:
    // Allow menus to be "clicked-up".  Without this a single click will
    // pick whatever item the mouse is pointing at:
    if (p.state == INITIAL_STATE && Fl::event_is_click()) {
      // redraw checkboxes so they preview the state they will be in:
      if (p.item_number >= 0) {
	w = p.menus[p.menu_number]->list->child(p.item_number);
	if (w->type()==FL_TOGGLE_ITEM || w->type()==FL_RADIO_ITEM)
	  p.menus[p.menu_number]->damage(FL_DAMAGE_CHILD);
      }
      return 1;
    }
  EXECUTE: // execute the item pointed to by w and current item
    // If they click outside menu we quit:
    if (p.item_number < 0) {p.state = ABORT_STATE; return 1;}
    // Do nothing if they click inactive items:
    w = p.menus[p.menu_number]->list->child(p.item_number);
    if (!w->takesevents() || w->is_group() && !w->user_data()) {
      // except if they click on an already-up menu title, in which case
      // we are done:
      // if (p.menubar && !p.menu_number && Fl::event_is_click())
      //   p.state = ABORT_STATE;
      return 1;
    }
    if ((w->flags() & FL_MENU_STAYS_UP) && (!p.menubar || p.menu_number)) {
      p.widget->execute(w);
      Fl_Window* mw = p.menus[p.menu_number];
      if (w->type() == FL_RADIO_ITEM) mw->redraw();
      else if (w->type() == FL_TOGGLE_ITEM) mw->damage(FL_DAMAGE_CHILD);
    } else {
      p.state = DONE_STATE;
    }
    return 1;
  }
  return Fl_Window::handle(e);
}

static void autoscroll_timeout(void*) {
  if (last_event == FL_DRAG || last_event == FL_MOVE)
    p->menus[0]->handle(last_event);
}

// scroll so item i is visible on screen
void MenuWindow::autoscroll(int i) {
  if (is_menubar) return;
  int Y = y()+ypos(i);
  if (Y <= Fl::y()) Y = Fl::y()-Y+10;
  else if (i <= 0) return;
  else {
    Fl_Widget* o = list->child(i-1);
    Y = Y+o->h()-Fl::h()-Fl::y();
    if (Y < 0) return;
    Y = -Y-10;
  }
  Fl_Menu_Window::position(x(), y()+Y);
  // y(y()+Y); // don't wait for response from X
  Fl::remove_timeout(autoscroll_timeout, 0);
  if (last_event == FL_DRAG || last_event == FL_MOVE)
    Fl::add_timeout(.1,autoscroll_timeout, 0);
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

  MenuWindow mw(this, X, Y, W, H, t, menubar);
  Fl::grab(mw);
  menustate p; ::p = &p;
  p.menus[0] = &mw;
  p.nummenus = 1;
  p.menubar = menubar;
  p.widget = this;
  p.state = INITIAL_STATE;

  if (menubar) {
    p.menu_number = 0;
    p.item_number = p.menus[0]->selected;
    if (p.item_number < 0)
      mw.handle(FL_PUSH); // get menu mouse points at to appear
  } else {
    // create submenus until we locate the one with selected item
    // in it, positioning them so that one is selected:
    for (;;) {
      p.menu_number = p.nummenus-1;
      MenuWindow* mw = p.menus[p.nummenus-1];
      p.item_number = mw->selected;
      if (p.item_number < 0) break;
      Fl_Widget* m = mw->list->child(p.item_number);
      if (!m || !m->takesevents() || !m->is_group()) break;
      Fl_Group* g = (Fl_Group*)m;
      if (g->focus() < 0) break;
      int nX = mw->x() + mw->w();
      int nY = mw->y() + 1 + mw->ypos(p.item_number)-mw->ypos(0);
      MenuWindow* n = new MenuWindow(g, X,Y,W,H, 0);
      n->which_item = p.item_number;
      p.menus[p.nummenus++] = n;
      // move all earlier menus to line up with this new one:
      if (n->selected >= 0) {
	int dy = n->y()-nY;
	int dx = n->x()-nX;
	for (int menu = 0; menu <= p.menu_number; menu++) {
	  MenuWindow* t = p.menus[menu];
	  int nx = t->x()+dx; if (nx < 0) {nx = 0; dx = -t->x();}
	  int ny = t->y()+dy+1; if (ny < 0) {ny = 0; dy = -t->y()-1;}
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
    p.menus[p.menu_number]->autoscroll(p.item_number);
    Fl_Widget* m = mw->list->child(p.item_number);

    if (m->takesevents() && m->is_group()) {
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
	title = m;
      } else {
	nX = mw->x() + mw->w();
	nY = mw->y() + 1 + mw->ypos(p.item_number)-mw->ypos(0);
	title = 0;
      }
      ((Fl_Group*)m)->focus(-1); // don't preselect anything on this menu
      p.menus[p.nummenus++] = mw =
	new MenuWindow((Fl_Group*)m, nX, nY, 0, 0, title);
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
			mw->y() + mw->h(), 0, 0, m);
	mw = fakemenu;
      SHOW_MENUBAR_TITLE:
	// fix the title box size to match menubar thickness:
	int nh = this->h()-6;
	mw->title->y(mw->title->y()+mw->title->h()-nh);
	mw->title->h(nh);
	mw->title->show();
	if (mw != fakemenu) mw->show();
      }
    }
  }

  Fl::remove_timeout(autoscroll_timeout, 0);

  Fl_Widget* w = 0;
  if (p.state == DONE_STATE) {
    w = p.menus[p.menu_number]->list->child(p.item_number);
    // set the value of all the submenus:
    for (int i=0; i < p.nummenus; i++)
      p.menus[i]->list->focus(p.menus[i]->selected);
  }

  delete fakemenu;
  while (--p.nummenus) delete p.menus[p.nummenus];
  mw.hide();
  Fl::release();

  if (w) {
    execute(w);
    if (menubar && !p.menu_number &&
	(w->type() == FL_RADIO_ITEM || w->type() == FL_TOGGLE_ITEM))
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

////////////////////////////////////////////////////////////////

int fl_dont_execute = 0; // hack for fluid

void Fl_Menu_::execute(Fl_Widget* w) {
  if (fl_dont_execute) return;
  if (w->type() == FL_TOGGLE_ITEM) {
    if (w->value()) w->clear(); else w->set();
  } else if (w->type() == FL_RADIO_ITEM) {
    w->set();
    Fl_Group* g = w->parent();
    int i = g->find(w);
    int j;
    for (j = i-1; j >= 0; j--) {
      Fl_Widget* o = g->child(j);
      if (o->type() == FL_RADIO_ITEM) o->clear();
      else break;
    }
    for (j = i+1; j < g->children(); j++) {
      Fl_Widget* o = g->child(j);
      if (o->type() == FL_RADIO_ITEM) o->clear();
      else break;
    }
  }
  // We search from the item up to find a non-zero user-data and
  // a non-default callback to call:
  void* data = w->user_data();
  Fl_Widget* cb_w = w;
  while (w != this) {
    w = w->parent();
    if (!data) data = w->user_data();
    if (cb_w->callback() == Fl_Widget::default_callback) cb_w = w;
  }
  cb_w->do_callback(cb_w, data);
}

Fl_Widget* Fl_Menu_::item() const {
  if (value() < 0) return 0;
  Fl_Widget* w = child(value());
  while (w->is_group() && ((Fl_Group*)w)->focus() >= 0)
    w = ((Fl_Group*)w)->child(((Fl_Group*)w)->focus());
  return w;
}

void Fl_Menu_::item(Fl_Widget* w) {
  if (!w) {focus(-1); return;}
  while (w->parent()) {
    w->parent()->focus(w);
    w = w->parent();
  }
}

////////////////////////////////////////////////////////////////

// recursive innards of handle_shortcut:
static Fl_Widget* shortcut_search(Fl_Group* g) {
  Fl_Widget* ret = 0;
  for (int i = 0; i < g->children(); i++) {
    Fl_Widget* w = g->child(i);
    if (!w->takesevents()) continue;
    if (Fl_Widget::test_shortcut(w->shortcut())) {g->focus(i); return w;}
    if (!ret && w->is_group() /*&& IS_OPEN*/) {
      ret = shortcut_search((Fl_Group*)w);
      if (ret) g->focus(i);
    }
  }
  return ret;
}

int Fl_Menu_::handle_shortcut() {
  Fl_Widget* w = shortcut_search(this);
  if (w) {execute(w); return 1;}
  return 0;
}

//
// End of "$Id: Fl_Menu.cxx,v 1.83 2000/04/03 17:09:19 bill Exp $".
//
