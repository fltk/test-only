//
// "$Id: Fl_Menu.cxx,v 1.41 1999/09/20 04:33:45 bill Exp $"
//
// Menu code for the Fast Light Tool Kit (FLTK).
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

// Warning: this menu code is quite a mess!
// Yes, it is.

// This file contains code for implementing Fl_Menu_Item, and for
// methods for bringing up popup menu hierarchies without using the
// Fl_Menu_ widget.

#include <FL/Fl.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Menu_.H>
#include <FL/fl_draw.H>

int Fl_Menu_Item::size() const {
  const Fl_Menu_Item* m = this;
  int nest = 0;
  for (;;) {
    if (!m->text) {
      if (!nest) return (m-this+1);
      nest--;
    } else if (m->flags() & FL_SUBMENU) {
      nest++;
    }
    m++;
  }
}

const Fl_Menu_Item* Fl_Menu_Item::next(int n) const {
  if (n < 0) return 0; // this is so selected==-1 returns NULL
  const Fl_Menu_Item* m = this;
  int nest = 0;
  while (n>0) {
    if (!m->text) {
      if (!nest) return m;
      nest--;
    } else if (m->flags() & FL_SUBMENU) {
      nest++;
    }
    m++;
    if (!nest && m->visible()) n--;
  }
  return m;
}

// need to do something so this inherits from basic style..
Fl_Style Fl_Menu_Item::default_style = {
  FL_FLAT_BOX,	// box
  0,		// glyph
  0,		// label_font
  0,		// text_font
  0,		// label_type
  0,		// color
  0,		// label_color
  FL_WHITE,	// selection_color / on_color (used by checkboxes)
  0,		// selection_text_color
  FL_WHITE,	// off_color (used by checkboxes)
  FL_BLUE_SELECTION_COLOR, // highlight_color
  FL_WHITE	// highlight_text_color
  // rest is zero
};

Fl_Style Fl_Menu_Item::title_style = {
  FL_FLAT_UP_BOX, // box
  0,		// glyph
  0,		// label_font
  0,		// text_font
  0,		// label_type
  0,		// color
  0,		// label_color
  FL_WHITE,	// selection color (used by checkboxes)
  0,		// selection_text_color
  FL_WHITE	// off_color (used by checkboxes)
  // rest is zero
};

// Return the style to use.  Inherit it if this is the first time this
// one has been referenced.

const Fl_Style* Fl_Menu_Item::style(int title) const {
  Fl_Style* parent = title ? &title_style : &default_style;
  if (!parent->previous) Fl_Widget::default_style.add_child(parent);
  Fl_Style* s = (Fl_Style*)style_;
  if (s) {
    if (!s->previous) parent->add_child(s);
  } else {
    s = parent;
  }
  return s;
}

extern Fl_Style* fl_unique_style(const Fl_Style* & pointer); // in Fl_Widget.c

void Fl_Menu_Item::setp(const void* const * p, const void* v) {
  int d = p-(const void**)&(style_->box);
  style_ = style();
  Fl_Style* s = fl_unique_style(style_);
  *((const void**)&(s->box) + d) = v;
  s->mbf |= 1<< d;
}

void Fl_Menu_Item::seti(const unsigned * p, unsigned v) {
  int d = p-(unsigned*)&(style_->color);
  style_ = style();
  Fl_Style* s = fl_unique_style(style_);
  *((unsigned*)&(s->color) + d) = v;
  s->mbf |= 1 << (d+((const void**)&(s->color) - ((const void**)&(s->box))));
}

////////////////////////////////////////////////////////////////

// tiny window for title of menu:
class menutitle : public Fl_Menu_Window {
  void draw();
public:
  const Fl_Menu_Item* menu;
  menutitle(int X, int Y, int W, int H, const Fl_Menu_Item*);
};

// each vertical menu has one of these:
class menuwindow : public Fl_Menu_Window {
  void draw();
  void drawentry(const Fl_Menu_Item*, int i, int erase);
public:
  menutitle* title;
  int handle(int);
  int itemheight;	// zero == menubar
  int numitems;
  int selected;
  int drawn_selected;	// last redraw has this selected
  const Fl_Menu_Item* menu;
  menuwindow(const Fl_Menu_Item* m, int X, int Y, int W, int H,
	     const Fl_Menu_Item* picked, const Fl_Menu_Item* title,
	     int menubar = 0, int menubar_title = 0);
  ~menuwindow();
  void set_selected(int);
  int find_selected(int mx, int my);
  int titlex(int);
  void autoscroll(int);
  void position(int x, int y);
  int ypos(int);
};

extern char fl_draw_shortcut;
#include <FL/Fl_Image.H>

// width of label, including effect of & characters:
int Fl_Menu_Item::measure(int* hp, const Fl_Menu_*) const {
  const Fl_Style* style = this->style(0);
  // it used to use m for default style
  fl_font(style->label_font, style->label_size);
  fl_draw_shortcut = 1;
  int h; if (!hp) hp = &h;
  int w = 0; fl_measure(label(), w, *hp);
  fl_draw_shortcut = 0;
  *hp += 2; // add a bit of leading
  if (flags() & (FL_MENU_TOGGLE|FL_MENU_RADIO)) w += 14;
  if (image_) {
    int W, H;
    image_->measure(W, H);
    if (H > *hp) *hp = H;
    w += W;
  }
  *hp += style->box->dh();
  return w;
}

void Fl_Menu_Item::draw(int x, int y, int w, int h, const Fl_Menu_*,
 			int selected) const {
  const Fl_Style* style = this->style(selected == 2);
  // it used to use m for default style
  Fl_Boxtype box = style->box;
  Fl_Color c = style->color;
  Fl_Color lc = style->label_color;
  Fl_Flags f = flags() & FL_MENU_INACTIVE;
  if (selected) {
    f |= FL_MENU_VALUE;
    if (style->highlight_color) c = style->highlight_color;
    if (style->highlight_label_color) lc = style->highlight_label_color;
  }
  box->draw(x, y, w, h, c, f<<8);
  if (selected == 2) {x += 2; w -= 8;}

  if (flags() & (FL_MENU_TOGGLE|FL_MENU_RADIO)) {
    f = flags() & FL_MENU_VALUE;
    if (selected && Fl::pushed()) {
      // if pushed, preview the resulting state
      if (flags() & FL_MENU_RADIO) f |= FL_MENU_VALUE;
      else f ^= FL_MENU_VALUE;
    }
    Fl_Color c1 = (f&FL_MENU_VALUE) ? style->selection_color: style->off_color;
    if (!c1) c1 = c;
    if (style->selection_color) c = style->selection_color;
    style->glyph(flags()&FL_MENU_RADIO ? FL_GLYPH_RADIO : FL_GLYPH_CHECK,
		 x+3, y+(h-14)/2+1, 12, 12, c1, f<<8);
    x += 14; w -= 14;
  }

  if (image_) {
    int W, H;
    image_->measure(W, H);
    fl_color(lc);
    image_->draw(x, y, W, h, (flags()<<8) | FL_ALIGN_LEFT);
    x += W; w -= W;
  }
  if (!fl_draw_shortcut) fl_draw_shortcut = 1;
  fl_font(style->text_font, style->text_size);
  f = flags() & FL_MENU_INACTIVE;
  style->label_type->draw(label(), x+3, y, w>6 ? w-6 : 0, h, lc,
			  (flags()<<8) | FL_ALIGN_LEFT);
  fl_draw_shortcut = 0;
}

menutitle::menutitle(int X, int Y, int W, int H, const Fl_Menu_Item* L) :
  Fl_Menu_Window(X, Y, W, H, 0) {
  end();
  set_modal();
  clear_border();
  menu = L;
  // can't use sgi overlay for images:
  if (L->image_) clear_overlay();
}

// appearance of current menus are pulled from this parent widget:
static const Fl_Menu_* button;

menuwindow::menuwindow(const Fl_Menu_Item* m, int X, int Y, int Wp, int Hp,
		       const Fl_Menu_Item* picked, const Fl_Menu_Item* t, 
		       int menubar, int menubar_title)
  : Fl_Menu_Window(X, Y, Wp, Hp, 0)
{
  end();
  set_modal();
  clear_border();
  menu = m;
  drawn_selected = -1;
  if (button) copy_style(button->style()); else box(FL_UP_BOX);
  selected = -1;
  {int i = 0;
  if (m) for (const Fl_Menu_Item* m1=m; ; m1 = m1->next(), i++) {
    if (picked) {
      if (m1 == picked) {selected = i; picked = 0;}
      else if (m1 > picked) {selected = i-1; picked = 0; Wp = Hp = 0;}
    }
    if (!m1->text) break;
  }
  numitems = i;}

  if (menubar) {
    itemheight = 0;
    title = 0;
    return;
  }

  itemheight = 1;
  int num_dividers = 0;
  int Wtitle = 0;
  int Htitle = 0;
  int hotKeysw = 0;
  if (t) Wtitle = t->measure(&Htitle, button) + 12;
  int W = 0;
  if (m) for (; m->text; m = m->next()) {
    int h; int w1 = m->measure(&h, button);
    if (h > itemheight) itemheight = h;
    if (m->flags() & FL_MENU_DIVIDER) num_dividers++;
    if (m->flags() & (FL_SUBMENU|FL_SUBMENU_POINTER)) w1 += 14;
    if (w1 > W) W = w1;
    if (m->shortcut_) {
      w1 = int(fl_width(fl_shortcut_label(m->shortcut_))) + 8;
      if (w1 > hotKeysw) hotKeysw = w1;
    }
    // can't use sgi overlay for images:
    if (m->image_) clear_overlay();
  }
  if (selected >= 0 && !Wp) X -= W/2;
  W += hotKeysw + box()->dw() + 7;
  if (Wp > W) W = Wp;
  if (Wtitle > W) W = Wtitle;

  if (!Wp) {if (X < 0) X = 0; if (X > Fl::w()-W) X= Fl::w()-W;}
  x(X); w(W);
  h(itemheight*numitems+2*num_dividers+box()->dh());
  if (selected >= 0) {
    Y = Y+(Hp-itemheight)/2-ypos(selected)+1;
  } else {
    Y = Y+Hp;
  }
  if (m) y(Y-1); else {y(Y-3); w(1); h(1);}

  if (t) {
    int ht = menubar_title ? button->h()-6 : Htitle + box()->dw() + 3;
    title = new menutitle(X, Y-ht-3, Wtitle, ht, t);
  } else
    title = 0;
}

menuwindow::~menuwindow() {
  delete title;
}

void menuwindow::position(int X, int Y) {
  if (title) {title->position(X, title->y()+Y-y());}
  Fl_Menu_Window::position(X, Y);
  // x(X); y(Y); // don't wait for response from X
}

// return the top edge of item i:
int menuwindow::ypos(int i) {
  int Y = box()->dy()+i*itemheight;
  for (const Fl_Menu_Item* m = menu; i>0 && m->text; m = m->next(), i--)
    if (m->flags() & FL_MENU_DIVIDER) Y += 2;
  return Y;
}

// scroll so item i is visible on screen
void menuwindow::autoscroll(int i) {
  int Y = y()+ypos(i);
  if (Y <= Fl::y()) Y = Fl::y()-Y+10;
  else {
    Y = Y+itemheight-Fl::h()-Fl::y();
    if (Y < 0) return;
    Y = -Y-10;
  }
  Fl_Menu_Window::position(x(), y()+Y);
  // y(y()+Y); // don't wait for response from X
}

////////////////////////////////////////////////////////////////

void menuwindow::drawentry(const Fl_Menu_Item* m, int i, int /*erase*/) {
  if (!m) return; // this happens if -1 is selected item and redrawn

  int x = box()->dx();
  int w = this->w()-box()->dw();
  int y = ypos(i);
  int h = itemheight;

  m->draw(x, y, w, h, button, i==selected);

  // the shortcuts and arrows assumme fl_color() was left set by draw():
  if (m->submenu()) {
    int y1 = y+(h-14)/2;
    fl_polygon(x+w-13, y1+2, x+w-13, y1+2+10, x+w-3, y1+2+5);
  } else if (m->shortcut_) {
    fl_font(label_font(), label_size());
    fl_draw(fl_shortcut_label(m->shortcut_), x, y, w-3, h, FL_ALIGN_RIGHT);
  }

  if (m->flags() & FL_MENU_DIVIDER) {
    fl_color(FL_DARK3);
    fl_xyline(x-1, y+h, w+2);
    fl_color(FL_LIGHT3);
    fl_xyline(x-1, y+h+1, w+2);
  }
}

void menutitle::draw() {
  menu->draw(0, 0, w(), h(), button, 2);
}

void menuwindow::draw() {

  if (damage() & FL_DAMAGE_ALL) {	// complete redraw
    box()->draw(0, 0, w(), h(), color(), FL_NO_FLAGS);
    if (menu) {
      const Fl_Menu_Item* m; int i;
      for (m=menu, i=0; m->text; i++, m = m->next()) drawentry(m, i, 0);
    }
  } else {
    if (selected != drawn_selected) // erase old selection
      drawentry(menu->next(drawn_selected), drawn_selected, 1);
    drawentry(menu->next(selected), selected, 1); // draw new one
  }	    
  drawn_selected = selected;
}

void menuwindow::set_selected(int i) {
  if (i != selected) {selected = i; damage(FL_DAMAGE_CHILD);}
}

////////////////////////////////////////////////////////////////

int menuwindow::find_selected(int mx, int my) {
  if (!menu || !menu->text) return -1;
  mx -= x();
  my -= y();
  if (my < 0 || my >= h()) return -1;
  if (!itemheight) { // menubar
    int x = 3; int i = 0;
    const Fl_Menu_Item* m = menu;
    for (; ; m = m->next(), i++) {
      if (!m->text) return -1;
      x += m->measure(0, button) + 16;
      if (x > mx) break;
    }
    return i;
  }
  if (mx < box()->dx() || mx >= w()) return -1;
  int i = (my-box()->dy()-1)/itemheight;
  if (i < 0 || i >= numitems) return -1;
  return i;
}

// return horizontal position for item i in a menubar:
int menuwindow::titlex(int i) {
  const Fl_Menu_Item* m;
  int x = 3;
  for (m=menu; i--; m = m->next()) x += m->measure(0, button) + 16;
  return x;
}

// match shortcuts & label shortcuts, don't search submenus:
// returns menu item and index
const Fl_Menu_Item* Fl_Menu_Item::find_shortcut(int* ip) const {
  const Fl_Menu_Item* m1 = this;
  for (int ii = 0; m1 && m1->text; m1 = m1->next(1), ii++) {
    if (m1->activevisible() &&
	(Fl::test_shortcut(m1->shortcut_)
	 || Fl_Widget::test_shortcut(m1->text))) {if (ip) *ip=ii; return m1;}
  }
  return 0;
}

////////////////////////////////////////////////////////////////
// Fl_Menu_Item::popup(...)

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
#define DONE_STATE 2	// exit the popup, the current item was picked
#define MENU_PUSH_STATE 3 // mouse has been pushed on a menu title

struct menustate {
  const Fl_Menu_Item* item; // what mouse is pointing at
  int menu_number; // which menu it is in
  int item_number; // which item in that menu, -1 if none
  menuwindow* menus[20]; // pointers to menus
  int nummenus;
  int menubar; // if true menus[0] is a menubar
  int state;
};

static menustate* p;

static inline void setitem(const Fl_Menu_Item* i, int m, int n) {
  p->item = i;
  p->menu_number = m;
  p->item_number = n;
}

static void setitem(int m, int n) {
  setitem((n >= 0) ? p->menus[m]->menu->next(n) : 0, m, n);
}

static int forward(int menu) { // go to next item in menu menu if possible
  menustate &p = *(::p);
  menuwindow &m = *(p.menus[menu]);
  int item = (menu == p.menu_number) ? p.item_number : m.selected;
  while (++item < m.numitems) {
    const Fl_Menu_Item* m1 = m.menu->next(item);
    if (m1->activevisible()) {setitem(m1, menu, item); return 1;}
  }
  return 0;
}

static int backward(int menu) { // previous item in menu menu if possible
  menustate &p = *(::p);
  menuwindow &m = *(p.menus[menu]);
  int item = (menu == p.menu_number) ? p.item_number : m.selected;
  if (item < 0) item = m.numitems;
  while (--item >= 0) {
    const Fl_Menu_Item* m1 = m.menu->next(item);
    if (m1->activevisible()) {setitem(m1, menu, item); return 1;}
  }
  return 0;
}

int menuwindow::handle(int e) {
  menustate &p = *(::p);
  switch (e) {
  case FL_KEYBOARD:
    switch (Fl::event_key()) {
    case FL_Tab:
      if (Fl::event_state()&FL_SHIFT) goto BACKTAB;
    case ' ':
      if (p.item && button && (p.item->flags_&(FL_MENU_TOGGLE|FL_MENU_RADIO)))
	goto TOGGLE;
      if (!forward(p.menu_number)) {p.item_number = -1; forward(p.menu_number);}
      return 1;
    case FL_BackSpace:
    case 0xFE20: // backtab
    BACKTAB:
      if (!backward(p.menu_number)) {p.item_number = -1;backward(p.menu_number);}
      return 1;
    case FL_Up:
      if (p.menubar && p.menu_number == 0) ;
      else if (backward(p.menu_number));
      else if (p.menubar && p.menu_number==1) setitem(0, p.menus[0]->selected);
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
    case FL_Enter:
      p.state = DONE_STATE;
      return 1;
    case FL_Escape:
      setitem(0, -1, 0);
      p.state = DONE_STATE;
      return 1;
    }
    break;
  case FL_SHORTCUT: {
    for (int menu = p.nummenus; menu--;) {
      menuwindow &mw = *(p.menus[menu]);
      int item; const Fl_Menu_Item* m = mw.menu->find_shortcut(&item);
      if (m) {
	setitem(m, menu, item);
	if (!m->submenu()) p.state = DONE_STATE;
	return 1;
      }
    }} break;
  case FL_PUSH:
    // redraw checkboxes so they preview the state they will be in:
    if (p.item && p.item->flags_&(FL_MENU_TOGGLE|FL_MENU_RADIO))
      p.menus[p.menu_number]->damage(FL_DAMAGE_CHILD);
  case FL_MOVE:
  case FL_DRAG: {
    int mx = Fl::event_x_root();
    int my = Fl::event_y_root();
    int item=0; int menu;
    for (menu = p.nummenus-1; ; menu--) {
      item = p.menus[menu]->find_selected(mx, my);
      if (item >= 0) break;
      if (menu <= 0) break;
    }
    setitem(menu, item);
    if (e == FL_PUSH) {
      if (p.item && p.item->submenu() // this is a menu title
	  && item != p.menus[menu]->selected // and it is not already on
	  && !p.item->callback_) // and it does not have a callback
	p.state = MENU_PUSH_STATE;
      else
	p.state = PUSH_STATE;
    }} return 1;
  case FL_RELEASE:
    // do nothing if they try to pick inactive items
    if (p.item && !p.item->activevisible()) return 1;
    // Mouse must either be held down/dragged some, or this must be
    // the second click (not the one that popped up the menu):
    if (!Fl::event_is_click() || p.state == PUSH_STATE ||
	p.menubar && p.item && !p.item->submenu() // button
	) {
	TOGGLE:
#if 1 // makes the check/radio items leave the menu up
      if (p.item && button && p.item->flags_&(FL_MENU_TOGGLE|FL_MENU_RADIO)) {
	if (p.menubar && !p.menu_number) {
	  // except if it is in a menubar, then we need to redraw it & return!
	  ((Fl_Menu_*)button)->redraw();
	  p.state = DONE_STATE;
	  return 1;
	}
	((Fl_Menu_*)button)->picked(p.item); // does the callback & sets value
	menuwindow& w = *p.menus[p.menu_number];
	if (p.item->flags_&FL_MENU_RADIO)
	  w.redraw();
	else { // minimual update for checkboxes:
	  w.damage(FL_DAMAGE_CHILD);
	}
	return 1;
      }
#endif
      p.state = DONE_STATE;
    }
    return 1;

  }
  return Fl_Window::handle(e);
}

const Fl_Menu_Item* Fl_Menu_Item::pulldown(
    int X, int Y, int W, int H,
    const Fl_Menu_Item* initial_item,
    const Fl_Menu_* pbutton,
    const Fl_Menu_Item* t,
    int menubar) const
{
  Fl_Group::current(0); // fix possible user error...

  button = pbutton;
  if (pbutton) {
    for (Fl_Window* w = pbutton->window(); w; w = w->window()) {
      X += w->x();
      Y += w->y();
    }
  } else {
    X += Fl::event_x_root()-Fl::event_x();
    Y += Fl::event_y_root()-Fl::event_y();
  }
  menuwindow mw(this, X, Y, W, H, initial_item, t, menubar);
  Fl::grab(mw);
  menustate p; ::p = &p;
  p.menus[0] = &mw;
  p.nummenus = 1;
  p.menubar = menubar;
  p.state = INITIAL_STATE;

  menuwindow* fakemenu = 0; // kludge for buttons in menubar

  // preselected item, pop up submenus if necessary:
  if (initial_item && mw.selected >= 0) {
    setitem(0, mw.selected);
    goto STARTUP;
  }

  setitem(0, 0, -1);
  if (menubar) mw.handle(FL_DRAG); // find the initial menu
  initial_item = p.item;
  if (initial_item) goto STARTUP;

  // the main loop, runs until p.state goes to DONE_STATE:
  for (;;) {

    // make sure all the menus are shown:
    {for (int k = menubar; k < p.nummenus; k++) {
      if (p.menus[k]->title) p.menus[k]->title->show();
      p.menus[k]->show();
    }}

    // get events:
    {const Fl_Menu_Item* oldi = p.item;
    Fl::wait();
    if (p.state == DONE_STATE) break; // done.
    if (p.item == oldi) continue;}
    // only do rest if item changes:

    delete fakemenu; fakemenu = 0; // turn off "menubar button"

    if (!p.item) { // pointing at nothing
      // turn off selection in deepest menu, but don't erase other menus:
      p.menus[p.nummenus-1]->set_selected(-1);
      continue;
    }

    delete fakemenu; fakemenu = 0;
    initial_item = 0; // stop the startup code
    p.menus[p.menu_number]->autoscroll(p.item_number);

  STARTUP:
    menuwindow& cw = *p.menus[p.menu_number];
    const Fl_Menu_Item* m = p.item;
    if (!m->activevisible()) { // pointing at inactive item
      cw.set_selected(-1);
      initial_item = 0; // turn off startup code
      continue;
    }
    cw.set_selected(p.item_number);

    if (m==initial_item) initial_item=0; // stop the startup code if item found
    if (m->submenu()) {
      const Fl_Menu_Item* title = m;
      const Fl_Menu_Item* menutable;
      if (m->flags() & FL_SUBMENU) menutable = m+1;
      else menutable = (Fl_Menu_Item*)(m)->user_data_;
      // figure out where new menu goes:
      int nX, nY;
      if (!p.menu_number && p.menubar) {	// menu off a menubar:
	nX = cw.x() + cw.titlex(p.item_number);
	nY = cw.y() + cw.h();
	initial_item = 0;
      } else {
	nX = cw.x() + cw.w();
	nY = cw.y() + 1 + cw.ypos(p.item_number)-cw.ypos(0);
	title = 0;
      }
      if (initial_item) { // bring up submenu containing initial item:
	menuwindow* n = new menuwindow(menutable,X,Y,W,H,initial_item,title);
	p.menus[p.nummenus++] = n;
	// move all earlier menus to line up with this new one:
	if (n->selected>=0) {
	  int dy = n->y()-nY;
	  int dx = n->x()-nX;
	  for (int menu = 0; menu <= p.menu_number; menu++) {
	    menuwindow* t = p.menus[menu];
	    int nx = t->x()+dx; if (nx < 0) {nx = 0; dx = -t->x();}
	    int ny = t->y()+dy+1; if (ny < 0) {ny = 0; dy = -t->y()-1;}
	    t->position(nx, ny);
	  }
	  setitem(p.nummenus-1, n->selected);
	  goto STARTUP;
	}
      } else if (p.nummenus > p.menu_number+1 &&
		 p.menus[p.menu_number+1]->menu == menutable) {
	// the menu is already up:
	while (p.nummenus > p.menu_number+2) delete p.menus[--p.nummenus];
	p.menus[p.nummenus-1]->set_selected(-1);
      } else {
	// delete all the old menus and create new one:
	while (p.nummenus > p.menu_number+1) delete p.menus[--p.nummenus];
	p.menus[p.nummenus++]= new menuwindow(menutable, nX, nY,
					  title?1:0, 0, 0, title, 0, menubar);
      }
    } else { // !m->submenu():
      while (p.nummenus > p.menu_number+1) delete p.menus[--p.nummenus];
      if (!p.menu_number && p.menubar) {
	// kludge so "menubar buttons" turn "on" by using menu title:
	fakemenu = new menuwindow(0,
				  cw.x()+cw.titlex(p.item_number),
				  cw.y()+cw.h(), 0, 0,
				  0, m, 0, 1);
	fakemenu->title->show();
      }
    }
  }
  const Fl_Menu_Item* m = p.item;
  delete fakemenu;
  while (p.nummenus>1) delete p.menus[--p.nummenus];
  mw.hide();
  Fl::release();
  return m;
}

const Fl_Menu_Item*
Fl_Menu_Item::popup(
  int X, int Y,
  const char* title,
  const Fl_Menu_Item* picked,
  const Fl_Menu_* button
  ) const
{
  static Fl_Menu_Item dummy; // static so it is all zeros
  dummy.text = title;
  return pulldown(X, Y, 0, 0, picked, button, title ? &dummy : 0);
}

const Fl_Menu_Item* Fl_Menu_Item::test_shortcut() const {
  const Fl_Menu_Item* m = this;
  const Fl_Menu_Item* ret = 0;
  if (m) for (; m->text; m = m->next()) {
    if (m->activevisible()) {
      // return immediately any match of an item in top level menu:
      if (Fl::test_shortcut(m->shortcut_)) return m;
      // if (Fl_Widget::test_shortcut(m->text)) return m;
      // only return matches from lower menu if nothing found in top menu:
      if (!ret && m->submenu()) {
	const Fl_Menu_Item* s =
	  (m->flags() & FL_SUBMENU) ? m+1:(const Fl_Menu_Item*)m->user_data_;
	ret = s->test_shortcut();
      }
    }
  }
  return ret;
}

//
// End of "$Id: Fl_Menu.cxx,v 1.41 1999/09/20 04:33:45 bill Exp $".
//
