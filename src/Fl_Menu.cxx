//
// "$Id: Fl_Menu.cxx,v 1.77 2000/01/10 06:31:22 bill Exp $"
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

// this style structure is only used for the frames of menuwindows:
static void mw_revert(Fl_Style* s) {
  s->box = FL_UP_BOX;
  s->leading = 4;
}

static Fl_Named_Style* menuwindow_default_style =
  new Fl_Named_Style("Menu_Window", mw_revert, &menuwindow_default_style);

// The default style for all menu items:

static void mi_revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX;
  s->glyph_box = FL_DOWN_BOX;
  s->glyph = fl_glyph;
  s->selection_color = FL_BLUE_SELECTION_COLOR;
  s->selection_text_color = FL_WHITE;
  s->off_color = FL_WHITE;
//s->parent = Fl_Widget::default_style;
}

Fl_Named_Style* Fl_Menu_Item::default_style = 
  new Fl_Named_Style("Menu_Item", mi_revert, &Fl_Menu_Item::default_style);

// This style is directly referenced for the menu titles for some fields
// (search this file for "title_style" to find out which).  This allows
// the inconsistency in Win98 display to be emulated.

static void mt_revert(Fl_Style* s) {
#if 1
  mi_revert(s); // makes blue titles, like NT4.0, I like this better
#else
  // win98 style?
  s->glyph = fl_glyph;
  s->box = FL_HIGHLIGHT_BOX;
#endif
}

Fl_Named_Style* Fl_Menu_Item::title_style =
  new Fl_Named_Style("Menu_Title", mt_revert, &Fl_Menu_Item::title_style);

extern Fl_Style* fl_unique_style(const Fl_Style* & pointer); // in Fl_Widget.c

// Return the style to use:
const Fl_Style* Fl_Menu_Item::style() const {
  return style_ ? style_ : default_style;
}

// Retrieve/set values from a style, using parent's value if not in child:

#define style_functions(TYPE,FIELD)	\
TYPE Fl_Menu_Item::FIELD() const {	\
  for (const Fl_Style* s = style();;) {	\
    if (s->FIELD) return s->FIELD;	\
    s = s->parent;			\
    if (!s) return 0;			\
  }					\
}					\
void Fl_Menu_Item::FIELD(TYPE v) {	\
  if (!style_) style_ = default_style;	\
  fl_unique_style(style_)->FIELD = v;	\
}

style_functions(Fl_Boxtype,box);
style_functions(Fl_Boxtype,glyph_box);
style_functions(Fl_Glyph,glyph);
style_functions(Fl_Font,label_font);
//style_functions(Fl_Font,text_font);
style_functions(Fl_Labeltype,label_type);
style_functions(Fl_Color,color);
style_functions(Fl_Color,label_color);
style_functions(Fl_Color,selection_color);
style_functions(Fl_Color,selection_text_color);
style_functions(Fl_Color,off_color);
style_functions(Fl_Color,highlight_color);
style_functions(Fl_Color,highlight_label_color);
//style_functions(Fl_Color,text_color);
style_functions(unsigned,label_size);
//style_functions(unsigned,text_size);
//style_functions(unsigned,leading);

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
	     int menubar = 0);
  ~menuwindow();
  int find_selected(int mx, int my);
  int titlex(int);
  void autoscroll(int);
  void position(int x, int y);
  int ypos(int);
};

extern char fl_draw_shortcut;
#include <FL/Fl_Image.H>

// width of label, including effect of & characters:
int Fl_Menu_Item::measure(int* hp) const {
  fl_font(label_font(), label_size());
  fl_draw_shortcut = 1;
  int h; if (!hp) hp = &h;
  int w = 0; fl_measure(label(), w, *hp);
  fl_draw_shortcut = 0;
  if (flags() & (FL_MENU_TOGGLE|FL_MENU_RADIO)) w += 15;
  if (image_) {
    int W, H;
    image_->measure(W, H);
    if (H > *hp) *hp = H;
    w += W;
  }
  *hp += box()->dh();
  w += box()->dw();
  return w;
}

void Fl_Menu_Item::draw(int x, int y, int w, int h, int selected) const {
  Fl_Boxtype lbox = box();
  Fl_Flags lflags = 0;
  Fl_Color lcolor = color();
  Fl_Color llabel_color = label_color();

  if (flags()&FL_MENU_INACTIVE) lflags = FL_INACTIVE;

  switch (selected) {
  case 0: // unselected menu item
    lflags |= FL_ALIGN_LEFT;
    break;
  case 1: // selected menu item
    lflags |= (FL_VALUE|FL_ALIGN_LEFT);
    lcolor = selection_color();
    if (!(flags()&FL_MENU_INACTIVE) || !Fl_Style::inactive_menu_hack)
      llabel_color = selection_text_color();
    break;
  case 2: // title or menubar item when menu popped up
    lflags |= (FL_VALUE|FL_ALIGN_CENTER);
    lbox = title_style->box;
    if (title_style->selection_color) 
      lcolor = title_style->selection_color;
    if (title_style->selection_text_color) 
      llabel_color = title_style->selection_text_color;
    break;
  case 3: // highlighted menubar item
    lflags |= (FL_HIGHLIGHT|FL_ALIGN_CENTER);
    lbox = title_style->box;
    if (title_style->highlight_color) 
      lcolor = title_style->highlight_color;
    if (title_style->highlight_label_color) 
      llabel_color = title_style->highlight_label_color;
    break;
  case 4: // plain menubar item
    lflags |= FL_ALIGN_CENTER;
    lbox = title_style->box;
    break;
  case 5: // draw the text in an Fl_Choice
    lflags |= FL_ALIGN_LEFT | FL_ALIGN_RIGHT;
    lbox = FL_NO_BOX;
    break;
  }
  lbox->draw(x, y, w, h, lcolor, lflags);

  if (flags() & (FL_MENU_TOGGLE|FL_MENU_RADIO)) {
    Fl_Flags gflags = (flags() & FL_MENU_VALUE) ? FL_VALUE : 0;
    if ((selected==1 || selected==2) && Fl::pushed()) {
      // if pushed, preview the resulting state
      if (flags() & FL_MENU_RADIO) gflags |= FL_VALUE;
      else gflags ^= FL_VALUE;
    }
    glyph()((flags()&FL_MENU_RADIO) ? FL_GLYPH_RADIO : FL_GLYPH_CHECK,
	    x+3, y+(h-13)/2, 13, 13,
	    off_color(),
	    (glyph_box()==FL_NO_BOX) ? llabel_color : label_color(),
	    gflags,
	    glyph_box());
    x += 15; w -= 15;
  }

  if (image_) {
    int W, H;
    image_->measure(W, H);
    fl_color(llabel_color);
    image_->draw(x, y, W, h, lflags);
    x += W; w -= W;
  }
  if (!fl_draw_shortcut) fl_draw_shortcut = 1;
  fl_font(label_font(), label_size());
  // hack so that selected menu items aren't drawn inactive--
  // just with inactive color
  if (selected == 1 && lflags&FL_INACTIVE && Fl_Style::inactive_menu_hack)
    { llabel_color = fl_inactive(llabel_color); lflags &= (~FL_INACTIVE); }
  label_type()->draw(label(), x+3, y, w>6 ? w-6 : 0, h, llabel_color, lflags);
  fl_draw_shortcut = 0;
}

menutitle::menutitle(int X, int Y, int W, int H, const Fl_Menu_Item* L) :
  Fl_Menu_Window(X, Y, W, H, 0)
{
  end();
  set_modal();
  clear_border();
  menu = L;
  // can't use sgi overlay for images:
  if (L->image_) clear_overlay();
}

menuwindow::menuwindow(const Fl_Menu_Item* m, int X, int Y, int Wp, int Hp,
		       const Fl_Menu_Item* picked, const Fl_Menu_Item* t, 
		       int menubar)
  : Fl_Menu_Window(X, Y, Wp, Hp, 0)
{
  end();
  set_modal();
  clear_border();
  menu = m;
  drawn_selected = -1;
  style(menuwindow_default_style);
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
  if (t) Wtitle = t->measure(&Htitle) + 16;
  int W = 0;
  if (m) for (; m->text; m = m->next()) {
    int h; int w1 = m->measure(&h);
    if (h > itemheight) itemheight = h;
    if (m->flags() & FL_MENU_DIVIDER) num_dividers++;
    if (m->flags() & (FL_SUBMENU|FL_SUBMENU_POINTER)) w1 += 16;
    if (w1 > W) W = w1;
    if (m->shortcut_) {
      w1 = fl_width(fl_shortcut_label(m->shortcut_)) + 8;
      if (w1 > hotKeysw) hotKeysw = w1;
    }
    // can't use sgi overlay for images:
    if (m->image_) clear_overlay();
  }

  itemheight += leading();

  if (selected >= 0 && !Wp) X -= W/2;
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  W += hotKeysw-dw + 7;
  if (Wp > W) W = Wp;
  if (Wtitle > W) W = Wtitle;

  if (!Wp) {if (X < 0) X = 0; if (X > Fl::w()-W) X= Fl::w()-W;}

  x(X); w(W);
  h(itemheight*numitems+2*num_dividers-dh);
  if (selected >= 0) {
    Y = Y+(Hp-itemheight)/2-ypos(selected)+1;
  } else {
    Y = Y+Hp;
  }
  if (m) y(Y-1); else {y(Y-3); w(1); h(1);}

  if (t) {
    int ht = Htitle-dw + 3;
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
  int X=0; int Y=0; int W=0; int H=0; box()->inset(X,Y,W,H);
  Y += i*itemheight;
  for (const Fl_Menu_Item* m = menu; i>0 && m->text; m = m->next(), i--)
    if (m->flags() & FL_MENU_DIVIDER) Y += 2;
  return Y;
}

////////////////////////////////////////////////////////////////

void menutitle::draw() {
  menu->draw(0, 0, w(), h(), 2);
}

void menuwindow::drawentry(const Fl_Menu_Item* m, int i, int /*erase*/) {
  if (!m) return; // this happens if -1 is selected item and redrawn

  int x=0; int y=0; int w=this->w(); int h=0; box()->inset(x,y,w,h);
  y = ypos(i);
  h = itemheight;

  int draw_selected = 0;
  if (selected == i && (m->active() || Fl_Style::inactive_menu_hack))
    draw_selected = 1;

  m->draw(x, y, w, h, draw_selected);

  Fl_Flags f = m->active() ? 0 : FL_INACTIVE;

  Fl_Color fc = m->label_color(), bc = color();
  if (draw_selected) {
    bc = m->selection_color();
    // hack so that selected menu items aren't drawn inactive--
    // just with inactive color
    if (m->active()) { fc = m->selection_text_color(); f |= FL_VALUE; }
    else { f = 0; fc = fl_inactive(fc); }
  }

  if (m->submenu()) {
    int X=x; int Y=y; int W=w; int H=h;
    m->box()->inset(X,Y,W,H);
    // we need the leading() part so that the height is based solely on the
    // itemheight and not on leading().  Necessary to get the size of the
    // arrow right for themes because leading() really screws things up
    // I changed this so the arrows are larger in the default setup:
    if (leading()>4) {
      H -= leading();
      Y += (leading())/2;
    }
    m->glyph()(FL_GLYPH_RIGHT, X+W-H, Y, H, H, bc, fc, f, FL_NO_BOX);
  } else if (m->shortcut_) {
    fl_font(label_font(), label_size());
    m->label_type()->draw(fl_shortcut_label(m->shortcut_), x, y, w-3, h,
                          fc, f|FL_ALIGN_RIGHT);
  }

  if (m->flags() & FL_MENU_DIVIDER) {
    fl_color(FL_DARK3);
    fl_xyline(x-1, y+h, w+2);
    fl_color(FL_LIGHT2);
    fl_xyline(x-1, y+h+1, w+2);
  }
}

void menuwindow::draw() {
  if (damage() & (FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE)) {	// complete redraw
    box()->draw(0, 0, w(), h(), color(), FL_FRAME_ONLY);
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
      x += m->measure(0) + 16;
      if (x > mx) break;
    }
    return i;
  }
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  if (mx < dx || mx >= w()+dw) return -1;
  int i = (my-dy-1)/itemheight;
  if (i < 0) return 0;
  if (i >= numitems) return numitems-1;
  return i;
}

// return horizontal position for item i in a menubar:
int menuwindow::titlex(int i) {
  const Fl_Menu_Item* m;
  int x = 3;
  for (m=menu; i--; m = m->next()) x += m->measure(0) + 16;
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
  int menu_number; // which menu it is in
  int item_number; // which item in that menu, -1 if none
  menuwindow* menus[20]; // pointers to menus
  int nummenus;
  int menubar; // if true menus[0] is a menubar
  int state;
  Fl_Menu_* widget; // widget that is making this menu
  const Fl_Menu_Item* item() {
    return (item_number>=0) ? menus[menu_number]->menu->next(item_number) : 0;
  }
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
  menuwindow &m = *(p.menus[menu]);
  int item = (menu == p.menu_number) ? p.item_number : m.selected;
  while (++item < m.numitems) {
    const Fl_Menu_Item* m1 = m.menu->next(item);
    if (m1->activevisible()) {setitem(menu, item); return 1;}
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
    if (m1->activevisible()) {setitem(menu, item); return 1;}
  }
  return 0;
}

static int last_event;

int menuwindow::handle(int e) {
  menustate &p = *(::p);
  const Fl_Menu_Item* m;
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
      m = p.item();
      if (m && (m->flags_&(FL_MENU_TOGGLE|FL_MENU_RADIO))) goto TOGGLE;
      break; // perhaps it should act like FL_Enter?
    case FL_Enter:
      p.state = DONE_STATE;
      return 1;
    case FL_Escape:
      setitem(0, -1);
      p.state = DONE_STATE;
      return 1;
    }
    break;

  case FL_KEYUP:
    if ((Fl::event_key() == FL_Alt_L || Fl::event_key() == FL_Alt_R)
	&& Fl::event_clicks()) {
      // checking for event_clicks insures that the keyup matches the
      // keydown that preceeded it, so Alt was pressed & released without
      // any intermediate values.
      setitem(0, -1);
      p.state = DONE_STATE;
      return 1;
    } else
      return 0;

  case FL_SHORTCUT: {
    for (int menu = p.nummenus; menu--;) {
      menuwindow &mw = *(p.menus[menu]);
      int item; m = mw.menu->find_shortcut(&item);
      if (m) {
	setitem(menu, item);
	if (!m->submenu()) p.state = DONE_STATE;
	return 1;
      }
    }} break;

  case FL_PUSH:
    // redraw checkboxes so they preview the state they will be in:
    m = p.item();
    if (m && m->flags_&(FL_MENU_TOGGLE|FL_MENU_RADIO))
      p.menus[p.menu_number]->damage(FL_DAMAGE_CHILD);
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
    setitem(menu, item);
    if (e == FL_PUSH) {
      m = p.item();
      if (m && m->submenu() // this is a menu title
	  && item != p.menus[menu]->selected // and it is not already on
	  && !m->callback_) // and it does not have a callback
	p.state = MENU_PUSH_STATE;
      else
	p.state = PUSH_STATE;
    }} return 1;

  case FL_RELEASE:
    // do nothing if they try to pick inactive items
    m = p.item();
    if (m && !m->activevisible()) return 1;
    // Clicking buttons in a menubar always returns:
    if (p.menubar && !p.menu_number && m && !m->submenu()) {
      if (m->flags_&(FL_MENU_TOGGLE|FL_MENU_RADIO)) p.widget->redraw();
      p.state = DONE_STATE;
      return 1;
    }
    // Mouse must either be held down/dragged some, or this must be
    // the second click (not the one that popped up the menu):
    if (!Fl::event_is_click() || p.state == PUSH_STATE) {
    TOGGLE:
      if (m && m->flags_&FL_MENU_STAYS_UP) {
	if (p.widget) p.widget->picked(m);
	Fl_Window* w = p.menus[p.menu_number];
	if (p.menubar && !p.menu_number) w = p.menus[1]->title; // fakemenu
	if (m->flags_&FL_MENU_RADIO)
	  w->redraw();
	else if (m->flags_&FL_MENU_TOGGLE) {
	  w->damage(FL_DAMAGE_CHILD);
	}
	return 1;
      }
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
  Fl::remove_timeout(autoscroll_timeout, 0);
  if (last_event == FL_DRAG || last_event == FL_MOVE)
    Fl::add_timeout(.1,autoscroll_timeout, 0);
}

const Fl_Menu_Item* Fl_Menu_Item::pulldown(
    int X, int Y, int W, int H,
    const Fl_Menu_Item* initial_item,
    Fl_Menu_* widget,
    const Fl_Menu_Item* t,
    int menubar) const
{
  Fl_Group::current(0); // fix possible programmer error...

  if (widget) {
    for (Fl_Window* w = widget->window(); w; w = w->window()) {
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
  p.widget = widget;

  if (menubar) {
    p.menu_number = 0;
    p.item_number = p.menus[0]->selected;
    if (p.item_number < 0)
      mw.handle(FL_DRAG); // get menu mouse points at to appear
  } else {
    // create submenus until we locate the one with selected item
    // in it, positioning them so that one is selected:
    for (;;) {
      p.menu_number = p.nummenus-1;
      menuwindow* mw = p.menus[p.nummenus-1];
      p.item_number = mw->selected;
      if (p.item_number < 0) break;
      const Fl_Menu_Item* m = p.item();
      if (!m || !m->activevisible() || !m->submenu()) break;
      if (m == initial_item) break;
      const Fl_Menu_Item* menutable;
      if (m->flags() & FL_SUBMENU) menutable = m+1;
      else menutable = (Fl_Menu_Item*)(m)->user_data_;
      int nX = mw->x() + mw->w();
      int nY = mw->y() + 1 + mw->ypos(p.item_number)-mw->ypos(0);
      menuwindow* n = new menuwindow(menutable, X, Y, W, H, initial_item, 0);
      p.menus[p.nummenus++] = n;
      // move all earlier menus to line up with this new one:
      if (n->selected >= 0) {
	int dy = n->y()-nY;
	int dx = n->x()-nX;
	for (int menu = 0; menu <= p.menu_number; menu++) {
	  menuwindow* t = p.menus[menu];
	  int nx = t->x()+dx; if (nx < 0) {nx = 0; dx = -t->x();}
	  int ny = t->y()+dy+1; if (ny < 0) {ny = 0; dy = -t->y()-1;}
	  t->position(nx, ny);
	}
      }
    }
    // show all the menus:
    for (int menu = 0; menu <= p.menu_number; menu++) {
      menuwindow* mw = p.menus[menu];
      if (mw->title) mw->title->show();
      mw->show();
    }
  }

  const Fl_Menu_Item* oldi = 0;

  menuwindow* fakemenu = 0;

  while (p.state != DONE_STATE) {
    const Fl_Menu_Item* m = p.item();
    if (m != oldi) {
      oldi = m;
      delete fakemenu; fakemenu = 0; // turn off "menubar button"
      if (m) {
	menuwindow* mw = p.menus[p.menu_number];
	p.menus[p.menu_number]->autoscroll(p.item_number);
	if (m->activevisible() && m->submenu()) {
	  const Fl_Menu_Item* menutable =
	    (m->flags() & FL_SUBMENU) ? m+1 : (Fl_Menu_Item*)(m->user_data_);
	  if (p.nummenus > p.menu_number+1 &&
	      p.menus[p.menu_number+1]->menu == menutable) {
	    // the menu is already up, delete lower ones:
	    while (p.nummenus > p.menu_number+2) delete p.menus[--p.nummenus];
	    mw = p.menus[p.menu_number+1];
	    if (mw->selected >= 0) {
	      mw->selected=-1; mw->damage(FL_DAMAGE_CHILD);
	    }
	  } else {
	    // delete all the old menus:
	    while (p.nummenus > p.menu_number+1) delete p.menus[--p.nummenus];
	    // figure out where new menu goes:
	    int nX, nY;
	    const Fl_Menu_Item* title;
	    if (!p.menu_number && p.menubar) {	// menu off a menubar:
	      nX = mw->x() + mw->titlex(p.item_number);
	      nY = mw->y() + mw->h();
	      title = m;
	    } else {
	      nX = mw->x() + mw->w();
	      nY = mw->y() + 1 + mw->ypos(p.item_number)-mw->ypos(0);
	      title = 0;
	    }
	    p.menus[p.nummenus++] = mw =
	      new menuwindow(menutable, nX, nY, 0, 0, 0, title, 0);
	    if (title) goto SHOW_MENUBAR_TITLE;
	    mw->show();
	  }
	} else { // !m->submenu():
	  while (p.nummenus > p.menu_number+1) delete p.menus[--p.nummenus];
	  if (m->activevisible() && !p.menu_number && p.menubar) {
	    // kludge so "menubar buttons" turn "on" by using menu title:
	    p.menus[1] = fakemenu =
	      new menuwindow(0,
			     mw->x()+mw->titlex(p.item_number),
			     mw->y()+1+mw->h(), 0, 0, 0, m, 0);
	    mw = fakemenu;
	  SHOW_MENUBAR_TITLE:
	    // fix the title box size to match menubar thickness:
	    int nh = widget->h()-6;
	    mw->title->y(mw->title->y()+mw->title->h()-nh);
	    mw->title->h(nh);
	    mw->title->show();
	    if (mw != fakemenu) mw->show();
	  }
	}
      }
    }
    Fl::wait();
  }

  Fl::remove_timeout(autoscroll_timeout, 0);
  oldi = p.item();
  delete fakemenu;
  while (p.nummenus>1) delete p.menus[--p.nummenus];
  mw.hide();
  Fl::release();
  return oldi;
}

const Fl_Menu_Item*
Fl_Menu_Item::popup(
  int X, int Y,
  const char* title,
  const Fl_Menu_Item* picked,
  Fl_Menu_* widget
  ) const
{
  static Fl_Menu_Item dummy; // static so it is all zeros
  dummy.text = title;
  return pulldown(X, Y, 0, 0, picked, widget, title ? &dummy : 0);
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
// End of "$Id: Fl_Menu.cxx,v 1.77 2000/01/10 06:31:22 bill Exp $".
//
