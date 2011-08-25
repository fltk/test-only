//
// "$Id$"
//
// Menu code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Warning: this menu code is quite a mess!

// This file contains code for implementing fltk3::MenuItem, and for
// methods for bringing up popup menu hierarchies without using the
// fltk3::Menu_ widget.

#include <fltk3/run.h>
#include <fltk3/MenuWindow.h>
#include <fltk3/Menu_.h>
#include <fltk3/draw.h>
#include <stdio.h>
#include "flstring.h"

/** Size of the menu starting from this menu item */
int fltk3::MenuItem::size() const {
  const fltk3::MenuItem* m = this;
  int nest = 0;
  for (;;) {
    if (!m->text) {
      if (!nest) return (m-this+1);
      nest--;
    } else if (m->flags & fltk3::SUBMENU) {
      nest++;
    }
    m++;
  }
}

// Advance a pointer to next visible or invisible item of a menu array, 
// skipping the contents of submenus.
static const fltk3::MenuItem* next_visible_or_not(const fltk3::MenuItem* m) {
  int nest = 0;
  do {
    if (!m->text) {
      if (!nest) return m;
      nest--;
    } else if (m->flags&fltk3::SUBMENU) {
      nest++;
    }
    m++;
  }
  while (nest);
  return m;
}

/**
  Advance a pointer by n items through a menu array, skipping
  the contents of submenus and invisible items.  There are two calls so
  that you can advance through const and non-const data.
*/
const fltk3::MenuItem* fltk3::MenuItem::next(int n) const {
  if (n < 0) return 0; // this is so selected==-1 returns NULL
  const fltk3::MenuItem* m = this;
  if (!m->visible()) n++;
  while (n) {
    m = next_visible_or_not(m);
    if (m->visible() || !m->text) n--;
  }
  return m;
}

// appearance of current menus are pulled from this parent widget:
static const fltk3::Menu_* button=0;

////////////////////////////////////////////////////////////////

// tiny window for title of menu:
class menutitle : public fltk3::MenuWindow {
  void draw();
public:
  const fltk3::MenuItem* menu;
  menutitle(int X, int Y, int W, int H, const fltk3::MenuItem*);
};

// each vertical menu has one of these:
class menuwindow : public fltk3::MenuWindow {
  void draw();
  void drawentry(const fltk3::MenuItem*, int i, int erase);
public:
  menutitle* title;
  int handle(int);
#if defined (__APPLE__) || defined (USE_X11)
  int early_hide_handle(int);
#endif
  int itemheight;	// zero == menubar
  int numitems;
  int selected;
  int drawn_selected;	// last redraw has this selected
  int shortcutWidth;
  const fltk3::MenuItem* menu;
  menuwindow(const fltk3::MenuItem* m, int X, int Y, int W, int H,
	     const fltk3::MenuItem* picked, const fltk3::MenuItem* title,
	     int menubar = 0, int menubar_title = 0, int right_edge = 0);
  ~menuwindow();
  void set_selected(int);
  int find_selected(int mx, int my);
  int titlex(int);
  void autoscroll(int);
  void position(int x, int y);
  int is_inside(int x, int y);
};

#define LEADING 4 // extra vertical leading

extern char fltk3::draw_shortcut;

/** 
  Measures width of label, including effect of & characters. 
  Optionally, can get height if hp is not NULL. 
*/
int fltk3::MenuItem::measure(int* hp, const fltk3::Menu_* m) const {
  fltk3::Label l;
  l.value   = text;
  l.image   = 0;
  l.deimage = 0;
  l.type    = labeltype_;
  l.font    = labelsize_ || labelfont_ ? labelfont_ : (m ? m->textfont() : fltk3::HELVETICA);
  l.size    = labelsize_ ? labelsize_ : m ? m->textsize() : fltk3::NORMAL_SIZE;
  l.color   = fltk3::FOREGROUND_COLOR; // this makes no difference?
  fltk3::draw_shortcut = 1;
  int w = 0; int h = 0;
  l.measure(w, hp ? *hp : h);
  fltk3::draw_shortcut = 0;
  if (flags & (fltk3::MENU_TOGGLE|fltk3::MENU_RADIO)) w += 14;
  return w;
}

/** Draws the menu item in bounding box x,y,w,h, optionally selects the item. */
void fltk3::MenuItem::draw(int x, int y, int w, int h, const fltk3::Menu_* m,
			int selected) const {
  fltk3::Label l;
  l.value   = text;
  l.image   = 0;
  l.deimage = 0;
  l.type    = labeltype_;
  l.font    = labelsize_ || labelfont_ ? labelfont_ : (m ? m->textfont() : fltk3::HELVETICA);
  l.size    = labelsize_ ? labelsize_ : m ? m->textsize() : fltk3::NORMAL_SIZE;
  l.color   = labelcolor_ ? labelcolor_ : m ? m->textcolor() : int(fltk3::FOREGROUND_COLOR);
  if (!active()) l.color = fltk3::inactive((fltk3::Color)l.color);
  fltk3::Color color = m ? m->color() : fltk3::GRAY;
  if (selected) {
    fltk3::Color r = m ? m->selection_color() : fltk3::SELECTION_COLOR;
    fltk3::Boxtype b = m && m->down_box() ? m->down_box() : fltk3::FLAT_BOX;
    if (fltk3::contrast(r,color)!=r) { // back compatibility boxtypes
      if (selected == 2) { // menu title
	r = color;
	b = m ? m->box() : fltk3::UP_BOX;
      } else {
	r = (fltk3::Color)(fltk3::COLOR_CUBE-1); // white
	l.color = fltk3::contrast((fltk3::Color)labelcolor_, r);
      }
    } else {
      l.color = fltk3::contrast((fltk3::Color)labelcolor_, r);
    }
    if (selected == 2) { // menu title
      fltk3::draw_box(b, x, y, w, h, r);
      x += 3;
      w -= 8;
    } else {
      fltk3::draw_box(b, x+1, y-(LEADING-2)/2, w-2, h+(LEADING-2), r);
    }
  }

  if (flags & (fltk3::MENU_TOGGLE|fltk3::MENU_RADIO)) {
    int d = (h - fltk3::NORMAL_SIZE + 1) / 2;
    int W = h - 2 * d;

    if (flags & fltk3::MENU_RADIO) {
      fltk3::draw_box(fltk3::ROUND_DOWN_BOX, x+2, y+d, W, W, fltk3::BACKGROUND2_COLOR);
      if (value()) {
	int tW = (W - fltk3::box_dw(fltk3::ROUND_DOWN_BOX)) / 2 + 1;
	if ((W - tW) & 1) tW++;	// Make sure difference is even to center
	int td = fltk3::box_dx(fltk3::ROUND_DOWN_BOX) + 1;
        if (!fltk3::scheme() || !strcmp(fltk3::scheme(), "classic")) {
	  // Offset the radio circle...
	  td ++;

	  if (!fltk3::scheme()) {
	    fltk3::color(fltk3::SELECTION_COLOR);
	    tW --;
	    fltk3::pie(x + td + 1, y + d + td - 1, tW + 3, tW + 3, 0.0, 360.0);
	    fltk3::arc(x + td + 1, y + d + td - 1, tW + 3, tW + 3, 0.0, 360.0);
	    fltk3::color(fltk3::color_average(fltk3::WHITE, fltk3::SELECTION_COLOR, 0.2f));
	  } else fltk3::color(labelcolor_);
	} else fltk3::color(labelcolor_);

	switch (tW) {
	  // Larger circles draw fine...
	  default :
            fltk3::pie(x + td + 2, y + d + td, tW, tW, 0.0, 360.0);
	    break;

          // Small circles don't draw well on many systems...
	  case 6 :
	    fltk3::rectf(x + td + 4, y + d + td, tW - 4, tW);
	    fltk3::rectf(x + td + 3, y + d + td + 1, tW - 2, tW - 2);
	    fltk3::rectf(x + td + 2, y + d + td + 2, tW, tW - 4);
	    break;

	  case 5 :
	  case 4 :
	  case 3 :
	    fltk3::rectf(x + td + 3, y + d + td, tW - 2, tW);
	    fltk3::rectf(x + td + 2, y + d + td + 1, tW, tW - 2);
	    break;

	  case 2 :
	  case 1 :
	    fltk3::rectf(x + td + 2, y + d + td, tW, tW);
	    break;
	}

	if (!fltk3::scheme()) {
	  fltk3::color(fltk3::color_average(fltk3::WHITE, fltk3::SELECTION_COLOR, 0.5));
	  fltk3::arc(x + td + 2, y + d + td, tW + 1, tW + 1, 60.0, 180.0);
	}
      }
    } else {
      fltk3::draw_box(fltk3::DOWN_BOX, x+2, y+d, W, W, fltk3::BACKGROUND2_COLOR);
      if (value()) {
	if (!fltk3::scheme()) {
	  fltk3::color(fltk3::SELECTION_COLOR);
	} else {
	  fltk3::color(labelcolor_);
	}
	int tx = x + 5;
	int tw = W - 6;
	int d1 = tw/3;
	int d2 = tw-d1;
	int ty = y + d + (W+d2)/2-d1-2;
	for (int n = 0; n < 3; n++, ty++) {
	  fltk3::line(tx, ty, tx+d1, ty+d1);
	  fltk3::line(tx+d1, ty+d1, tx+tw-1, ty+d1-d2+1);
	}
      }
    }
    x += W + 3;
    w -= W + 3;
  }

  if (!fltk3::draw_shortcut) fltk3::draw_shortcut = 1;
  l.draw(x+3, y, w>6 ? w-6 : 0, h, fltk3::ALIGN_LEFT);
  fltk3::draw_shortcut = 0;
}

menutitle::menutitle(int X, int Y, int W, int H, const fltk3::MenuItem* L) :
  fltk3::MenuWindow(X, Y, W, H, 0) {
  end();
  set_modal();
  clear_border();
  set_menu_window();
  menu = L;
  if (L->labelcolor_ || fltk3::scheme() || L->labeltype_ > fltk3::NO_LABEL) clear_overlay();
}

menuwindow::menuwindow(const fltk3::MenuItem* m, int X, int Y, int Wp, int Hp,
		       const fltk3::MenuItem* picked, const fltk3::MenuItem* t, 
		       int menubar, int menubar_title, int right_edge)
  : fltk3::MenuWindow(X, Y, Wp, Hp, 0)
{
  int scr_x, scr_y, scr_w, scr_h;
  int tx = X, ty = Y;

  fltk3::screen_xywh(scr_x, scr_y, scr_w, scr_h);
  if (!right_edge || right_edge > scr_x+scr_w) right_edge = scr_x+scr_w;

  end();
  set_modal();
  clear_border();
  set_menu_window();
  menu = m;
  if (m) m = m->first(); // find the first item that needs to be rendered
  drawn_selected = -1;
  if (button) {
    box(button->box());
    if (box() == fltk3::NO_BOX || box() == fltk3::FLAT_BOX) box(fltk3::UP_BOX);
  } else {
    box(fltk3::UP_BOX);
  }
  color(button && !fltk3::scheme() ? button->color() : fltk3::GRAY);
  selected = -1;
  {
    int j = 0;
    if (m) for (const fltk3::MenuItem* m1=m; ; m1 = m1->next(), j++) {
      if (picked) {
        if (m1 == picked) {selected = j; picked = 0;}
        else if (m1 > picked) {selected = j-1; picked = 0; Wp = Hp = 0;}
    }
    if (!m1->text) break;
  }
  numitems = j;}

  if (menubar) {
    itemheight = 0;
    title = 0;
    return;
  }

  itemheight = 1;

  int hotKeysw = 0;
  int hotModsw = 0;
  int Wtitle = 0;
  int Htitle = 0;
  if (t) Wtitle = t->measure(&Htitle, button) + 12;
  int W = 0;
  if (m) for (; m->text; m = m->next()) {
    int hh; 
    int w1 = m->measure(&hh, button);
    if (hh+LEADING>itemheight) itemheight = hh+LEADING;
    if (m->flags&(fltk3::SUBMENU|fltk3::SUBMENU_POINTER)) w1 += 14;
    if (w1 > W) W = w1;
    // calculate the maximum width of all shortcuts
    if (m->shortcut_) {
      // s is a pointerto the utf8 string for the entire shortcut
      // k points only to the key part (minus the modifier keys)
      const char *k, *s = fltk3::shortcut_label(m->shortcut_, &k);
      if (fltk3::utf_nb_char((const unsigned char*)k, strlen(k))<=4) {
        // a regular shortcut has a right-justified modifier followed by a left-justified key
        w1 = int(fltk3::width(s, k-s));
        if (w1 > hotModsw) hotModsw = w1;
        w1 = int(fltk3::width(k))+4;
        if (w1 > hotKeysw) hotKeysw = w1;
      } else {
        // a shortcut with a long modifier is right-justified to the menu
        w1 = int(fltk3::width(s))+4;
        if (w1 > (hotModsw+hotKeysw)) {
          hotModsw = w1-hotKeysw;
        }
      }
    }
    if (m->labelcolor_ || fltk3::scheme() || m->labeltype_ > fltk3::NO_LABEL) clear_overlay();
  }
  shortcutWidth = hotKeysw;
  if (selected >= 0 && !Wp) X -= W/2;
  int BW = fltk3::box_dx(box());
  W += hotKeysw+hotModsw+2*BW+7;
  if (Wp > W) W = Wp;
  if (Wtitle > W) W = Wtitle;

  if (X < scr_x) X = scr_x; if (X > scr_x+scr_w-W) X = right_edge-W; //X= scr_x+scr_w-W;
  x(X); w(W);
  h((numitems ? itemheight*numitems-LEADING : 0)+2*BW+3);
  if (selected >= 0) {
    Y = Y+(Hp-itemheight)/2-selected*itemheight-BW;
  } else {
    Y = Y+Hp;
    // if the menu hits the bottom of the screen, we try to draw
    // it above the menubar instead. We will not adjust any menu
    // that has a selected item.
    if (Y+h()>scr_y+scr_h && Y-h()>=scr_y) {
      if (Hp>1) {
        // if we know the height of the fltk3::Menu_, use it
        Y = Y-Hp-h();
      } else if (t) {
        // assume that the menubar item height relates to the first
        // menuitem as well
        Y = Y-itemheight-h()-fltk3::box_dh(box());
      } else {
        // draw the menu to the right
        Y = Y-h()+itemheight+fltk3::box_dy(box());
      }
    }
  }
  if (m) y(Y); else {y(Y-2); w(1); h(1);}

  if (t) {
    if (menubar_title) {
      int dy = fltk3::box_dy(button->box())+1;
      int ht = button->h()-dy*2;
      title = new menutitle(tx, ty-ht-dy, Wtitle, ht, t);
    } else {
      int dy = 2;
      int ht = Htitle+2*BW+3;
      title = new menutitle(X, Y-ht-dy, Wtitle, ht, t);
    }
  } else {
    title = 0;
  }
}

menuwindow::~menuwindow() {
  hide();
  delete title;
}

void menuwindow::position(int X, int Y) {
  if (title) {title->position(X, title->y()+Y-y());}
  MenuWindow::position(X, Y);
  // x(X); y(Y); // don't wait for response from X
}

// scroll so item i is visible on screen
void menuwindow::autoscroll(int n) {
  int scr_x, scr_y, scr_w, scr_h;
  int Y = y()+fltk3::box_dx(box())+2+n*itemheight;

  fltk3::screen_xywh(scr_x, scr_y, scr_w, scr_h);
  if (Y <= scr_y) Y = scr_y-Y+10;
  else {
    Y = Y+itemheight-scr_h-scr_y;
    if (Y < 0) return;
    Y = -Y-10;
  }
  MenuWindow::position(x(), y()+Y);
  // y(y()+Y); // don't wait for response from X
}

////////////////////////////////////////////////////////////////

void menuwindow::drawentry(const fltk3::MenuItem* m, int n, int eraseit) {
  if (!m) return; // this happens if -1 is selected item and redrawn

  int BW = fltk3::box_dx(box());
  int xx = BW;
  int W = w();
  int ww = W-2*BW-1;
  int yy = BW+1+n*itemheight;
  int hh = itemheight - LEADING;

  if (eraseit && n != selected) {
    fltk3::push_clip(xx+1, yy-(LEADING-2)/2, ww-2, hh+(LEADING-2));
    draw_box(box(), 0, 0, w(), h(), button ? button->color() : color());
    fltk3::pop_clip();
  }

  m->draw(xx, yy, ww, hh, button, n==selected);

  // the shortcuts and arrows assume fltk3::color() was left set by draw():
  if (m->submenu()) {
    int sz = (hh-7)&-2;
    int y1 = yy+(hh-sz)/2;
    int x1 = xx+ww-sz-3;
    fltk3::polygon(x1+2, y1, x1+2, y1+sz, x1+sz/2+2, y1+sz/2);
  } else if (m->shortcut_) {
    fltk3::Font f = m->labelsize_ || m->labelfont_ ? (fltk3::Font)m->labelfont_ :
                    button ? button->textfont() : fltk3::HELVETICA;
    fltk3::font(f, m->labelsize_ ? m->labelsize_ :
                   button ? button->textsize() : fltk3::NORMAL_SIZE);
    const char *k, *s = fltk3::shortcut_label(m->shortcut_, &k);
    if (fltk3::utf_nb_char((const unsigned char*)k, strlen(k))<=4) {
      // righ-align the modifiers and left-align the key
      char buf[32]; strcpy(buf, s); buf[k-s] = 0;
      fltk3::draw(buf, xx, yy, ww-shortcutWidth, hh, fltk3::ALIGN_RIGHT);
      fltk3::draw(  k, xx+ww-shortcutWidth, yy, shortcutWidth, hh, fltk3::ALIGN_LEFT);
    } else {
      // right-align to the menu
      fltk3::draw(s, xx, yy, ww-4, hh, fltk3::ALIGN_RIGHT);
    }
  }

  if (m->flags & fltk3::MENU_DIVIDER) {
    fltk3::color(fltk3::DARK3);
    fltk3::xyline(BW-1, yy+hh+(LEADING-2)/2, W-2*BW+2);
    fltk3::color(fltk3::LIGHT3);
    fltk3::xyline(BW-1, yy+hh+((LEADING-2)/2+1), W-2*BW+2);
  }
}

void menutitle::draw() {
  menu->draw(0, 0, w(), h(), button, 2);
}

void menuwindow::draw() {
  if (damage() != fltk3::DAMAGE_CHILD) {	// complete redraw
    fltk3::draw_box(box(), 0, 0, w(), h(), button ? button->color() : color());
    if (menu) {
      const fltk3::MenuItem* m; int j;
      for (m=menu->first(), j=0; m->text; j++, m = m->next()) drawentry(m, j, 0);
    }
  } else {
    if (damage() & fltk3::DAMAGE_CHILD && selected!=drawn_selected) { // change selection
      drawentry(menu->next(drawn_selected), drawn_selected, 1);
      drawentry(menu->next(selected), selected, 1);
    }
  }	    
  drawn_selected = selected;
}

void menuwindow::set_selected(int n) {
  if (n != selected) {selected = n; damage(fltk3::DAMAGE_CHILD);}
}

////////////////////////////////////////////////////////////////

int menuwindow::find_selected(int mx, int my) {
  if (!menu || !menu->text) return -1;
  mx -= x();
  my -= y();
  if (my < 0 || my >= h()) return -1;
  if (!itemheight) { // menubar
    int xx = 3; int n = 0;
    const fltk3::MenuItem* m = menu ? menu->first() : 0;
    for (; ; m = m->next(), n++) {
      if (!m->text) return -1;
      xx += m->measure(0, button) + 16;
      if (xx > mx) break;
    }
    return n;
  }
  if (mx < fltk3::box_dx(box()) || mx >= w()) return -1;
  int n = (my-fltk3::box_dx(box())-1)/itemheight;
  if (n < 0 || n>=numitems) return -1;
  return n;
}

// return horizontal position for item n in a menubar:
int menuwindow::titlex(int n) {
  const fltk3::MenuItem* m;
  int xx = 3;
  for (m=menu->first(); n--; m = m->next()) xx += m->measure(0, button) + 16;
  return xx;
}

// return 1, if the given root coordinates are inside the window
int menuwindow::is_inside(int mx, int my) {
  if ( mx < x_root() || mx >= x_root() + w() ||
       my < y_root() || my >= y_root() + h()) {
    return 0;
  }
  if (itemheight == 0 && find_selected(mx, my) == -1) {
    // in the menubar but out from any menu header
    return 0;
    }
  return 1;
}

////////////////////////////////////////////////////////////////
// fltk3::MenuItem::popup(...)

// Because fltk3::grab() is done, all events go to one of the menu windows.
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
  const fltk3::MenuItem* current_item; // what mouse is pointing at
  int menu_number; // which menu it is in
  int item_number; // which item in that menu, -1 if none
  menuwindow* p[20]; // pointers to menus
  int nummenus;
  int menubar; // if true p[0] is a menubar
  int state;
  menuwindow* fakemenu; // kludge for buttons in menubar
  int is_inside(int mx, int my);
};
static menustate* p=0;

// return 1 if the coordinates are inside any of the menuwindows
int menustate::is_inside(int mx, int my) {
  int i;
  for (i=nummenus-1; i>=0; i--) {
    if (p[i]->is_inside(mx, my))
      return 1;
  }
  return 0;
}

static inline void setitem(const fltk3::MenuItem* i, int m, int n) {
  p->current_item = i;
  p->menu_number = m;
  p->item_number = n;
}

static void setitem(int m, int n) {
  menustate &pp = *p;
  pp.current_item = (n >= 0) ? pp.p[m]->menu->next(n) : 0;
  pp.menu_number = m;
  pp.item_number = n;
}

static int forward(int menu) { // go to next item in menu menu if possible
  menustate &pp = *p;
  // fltk3::MenuButton can generate menu=-1. This line fixes it and selectes the first item.
  if (menu==-1) 
    menu = 0;
  menuwindow &m = *(pp.p[menu]);
  int item = (menu == pp.menu_number) ? pp.item_number : m.selected;
  while (++item < m.numitems) {
    const fltk3::MenuItem* m1 = m.menu->next(item);
    if (m1->activevisible()) {setitem(m1, menu, item); return 1;}
  }
  return 0;
}

static int backward(int menu) { // previous item in menu menu if possible
  menustate &pp = *p;
  menuwindow &m = *(pp.p[menu]);
  int item = (menu == pp.menu_number) ? pp.item_number : m.selected;
  if (item < 0) item = m.numitems;
  while (--item >= 0) {
    const fltk3::MenuItem* m1 = m.menu->next(item);
    if (m1->activevisible()) {setitem(m1, menu, item); return 1;}
  }
  return 0;
}

int menuwindow::handle(int e) {
#if defined (__APPLE__) || defined (USE_X11)
  // This off-route takes care of the "detached menu" bug on OS X.
  // Apple event handler requires that we hide all menu windows right
  // now, so that Carbon can continue undisturbed with handling window
  // manager events, like dragging the application window.
  int ret = early_hide_handle(e);
  menustate &pp = *p;
  if (pp.state == DONE_STATE) {
    hide();
    if (pp.fakemenu) {
      pp.fakemenu->hide();
      if (pp.fakemenu->title)
        pp.fakemenu->title->hide();
    }
    int i = pp.nummenus;
    while (i>0) {
      menuwindow *mw = pp.p[--i];
      if (mw) {
        mw->hide();
        if (mw->title) 
          mw->title->hide();
      }
    }
  }
  return ret;
}

int menuwindow::early_hide_handle(int e) {
#endif
  menustate &pp = *p;
  switch (e) {
  case fltk3::KEYBOARD:
    switch (fltk3::event_key()) {
    case fltk3::BackSpaceKey:
    BACKTAB:
      if (!backward(pp.menu_number)) {pp.item_number = -1;backward(pp.menu_number);}
      return 1;
    case fltk3::UpKey:
      if (pp.menubar && pp.menu_number == 0) {
        // Do nothing...
      } else if (backward(pp.menu_number)) {
        // Do nothing...
      } else if (pp.menubar && pp.menu_number==1) {
        setitem(0, pp.p[0]->selected);
      }
      return 1;
    case fltk3::TabKey:
      if (fltk3::event_shift()) goto BACKTAB;
    case fltk3::DownKey:
      if (pp.menu_number || !pp.menubar) {
        if (!forward(pp.menu_number) && fltk3::event_key()==fltk3::TabKey) {
          pp.item_number = -1;
          forward(pp.menu_number);
        }
      } else if (pp.menu_number < pp.nummenus-1) {
        forward(pp.menu_number+1);
      }
      return 1;
    case fltk3::RightKey:
      if (pp.menubar && (pp.menu_number<=0 || (pp.menu_number==1 && pp.nummenus==2)))
	forward(0);
      else if (pp.menu_number < pp.nummenus-1) forward(pp.menu_number+1);
      return 1;
    case fltk3::LeftKey:
      if (pp.menubar && pp.menu_number<=1) backward(0);
      else if (pp.menu_number>0)
	setitem(pp.menu_number-1, pp.p[pp.menu_number-1]->selected);
      return 1;
    case fltk3::EnterKey:
    case fltk3::KPEnterKey:
    case ' ':
      pp.state = DONE_STATE;
      return 1;
    case fltk3::EscapeKey:
      setitem(0, -1, 0);
      pp.state = DONE_STATE;
      return 1;
    }
    break;
  case fltk3::SHORTCUT: 
    {
      for (int mymenu = pp.nummenus; mymenu--;) {
	menuwindow &mw = *(pp.p[mymenu]);
	int item; const fltk3::MenuItem* m = mw.menu->find_shortcut(&item);
	if (m) {
	  setitem(m, mymenu, item);
	  if (!m->submenu()) pp.state = DONE_STATE;
	  return 1;
	}
      }
    }
    break;
    case fltk3::MOVE:
#if ! (defined(WIN32) || defined(__APPLE__))
      if (pp.state == DONE_STATE) {
	return 1; // Fix for STR #2619
      }
      /* FALLTHROUGH */
#endif
  case fltk3::ENTER:
  case fltk3::PUSH:
  case fltk3::DRAG:
    {
      int mx = fltk3::event_x_root();
      int my = fltk3::event_y_root();
      int item=0; int mymenu = pp.nummenus-1;
      // Clicking or dragging outside menu cancels it...
      if ((!pp.menubar || mymenu) && !pp.is_inside(mx, my)) {
	setitem(0, -1, 0);
	if (e==fltk3::PUSH)
	  pp.state = DONE_STATE;
	return 1;
      }
      for (mymenu = pp.nummenus-1; ; mymenu--) {
	item = pp.p[mymenu]->find_selected(mx, my);
	if (item >= 0) 
	  break;
	if (mymenu <= 0) {
	  // buttons in menubars must be deselected if we move outside of them!
	  if (pp.menu_number==-1 && e==fltk3::PUSH) {
	    pp.state = DONE_STATE;
	    return 1;
	  }
	  if (pp.current_item && pp.menu_number==0 && !pp.current_item->submenu()) {
	    if (e==fltk3::PUSH)
	      pp.state = DONE_STATE;
	    setitem(0, -1, 0);
	    return 1;
	  }
	  // all others can stay selected
	  return 0;
	}
      }
      if (my == 0 && item > 0) setitem(mymenu, item - 1);
      else setitem(mymenu, item);
      if (e == fltk3::PUSH) {
	if (pp.current_item && pp.current_item->submenu() // this is a menu title
	    && item != pp.p[mymenu]->selected // and it is not already on
	    && !pp.current_item->callback_) // and it does not have a callback
	  pp.state = MENU_PUSH_STATE;
	else
	  pp.state = PUSH_STATE;
      }
    }
    return 1;
  case fltk3::RELEASE:
    // Mouse must either be held down/dragged some, or this must be
    // the second click (not the one that popped up the menu):
    if (   !fltk3::event_is_click() 
        || pp.state == PUSH_STATE 
        || (pp.menubar && pp.current_item && !pp.current_item->submenu()) // button
	) {
#if 0 // makes the check/radio items leave the menu up
      const fltk3::MenuItem* m = pp.current_item;
      if (m && button && (m->flags & (fltk3::MENU_TOGGLE|fltk3::MENU_RADIO))) {
	((fltk3::Menu_*)button)->picked(m);
	pp.p[pp.menu_number]->redraw();
      } else
#endif
      // do nothing if they try to pick inactive items
      if (!pp.current_item || pp.current_item->activevisible())
	pp.state = DONE_STATE;
    }
    return 1;
  }
  return Window::handle(e);
}

/**
  Pulldown() is similar to popup(), but a rectangle is
  provided to position the menu.  The menu is made at least W
  wide, and the picked item is centered over the rectangle
  (like fltk3::Choice uses).  If picked is zero or not
  found, the menu is aligned just below the rectangle (like a pulldown
  menu).
  <P>The title and menubar arguments are used
  internally by the fltk3::MenuBar widget.
*/
const fltk3::MenuItem* fltk3::MenuItem::pulldown(
    int X, int Y, int W, int H,
    const fltk3::MenuItem* initial_item,
    const fltk3::Menu_* pbutton,
    const fltk3::MenuItem* t,
    int menubar) const {
  fltk3::Group::current(0); // fix possible user error...

  button = pbutton;
  if (pbutton && pbutton->window()) {
    for (fltk3::Window* w = pbutton->window(); w; w = w->window()) {
      X += w->x();
      Y += w->y();
    }
  } else {
    X += fltk3::event_x_root()-fltk3::event_x();
    Y += fltk3::event_y_root()-fltk3::event_y();
  }
  menuwindow mw(this, X, Y, W, H, initial_item, t, menubar);
  fltk3::grab(mw);
  menustate pp; p = &pp;
  pp.p[0] = &mw;
  pp.nummenus = 1;
  pp.menubar = menubar;
  pp.state = INITIAL_STATE;
  pp.fakemenu = 0; // kludge for buttons in menubar

  // preselected item, pop up submenus if necessary:
  if (initial_item && mw.selected >= 0) {
    setitem(0, mw.selected);
    goto STARTUP;
  }

  pp.current_item = 0; pp.menu_number = 0; pp.item_number = -1;
  if (menubar) {
    // find the initial menu
    if (!mw.handle(fltk3::DRAG)) {
      fltk3::grab(0);
      return 0;
    }
  }
  initial_item = pp.current_item;
  if (initial_item) goto STARTUP;

  // the main loop, runs until p.state goes to DONE_STATE:
  for (;;) {

    // make sure all the menus are shown:
    {
      for (int k = menubar; k < pp.nummenus; k++) {
        if (!pp.p[k]->shown()) {
	  if (pp.p[k]->title) pp.p[k]->title->show();
	  pp.p[k]->show();
        }
      }
    }

    // get events:
    {
      const fltk3::MenuItem* oldi = pp.current_item;
      fltk3::wait();
      if (pp.state == DONE_STATE) break; // done.
      if (pp.current_item == oldi) continue;
    }

    // only do rest if item changes:
    if(pp.fakemenu) {delete pp.fakemenu; pp.fakemenu = 0;} // turn off "menubar button"

    if (!pp.current_item) { // pointing at nothing
      // turn off selection in deepest menu, but don't erase other menus:
      pp.p[pp.nummenus-1]->set_selected(-1);
      continue;
    }

    if(pp.fakemenu) {delete pp.fakemenu; pp.fakemenu = 0;}
    initial_item = 0; // stop the startup code
    pp.p[pp.menu_number]->autoscroll(pp.item_number);

  STARTUP:
    menuwindow& cw = *pp.p[pp.menu_number];
    const fltk3::MenuItem* m = pp.current_item;
    if (!m->activevisible()) { // pointing at inactive item
      cw.set_selected(-1);
      initial_item = 0; // turn off startup code
      continue;
    }
    cw.set_selected(pp.item_number);

    if (m==initial_item) initial_item=0; // stop the startup code if item found
    if (m->submenu()) {
      const fltk3::MenuItem* title = m;
      const fltk3::MenuItem* menutable;
      if (m->flags&fltk3::SUBMENU) menutable = m+1;
      else menutable = (fltk3::MenuItem*)(m)->user_data_;
      // figure out where new menu goes:
      int nX, nY;
      if (!pp.menu_number && pp.menubar) {	// menu off a menubar:
	nX = cw.x() + cw.titlex(pp.item_number);
	nY = cw.y() + cw.h();
	initial_item = 0;
      } else {
	nX = cw.x() + cw.w();
	nY = cw.y() + pp.item_number * cw.itemheight;
	title = 0;
      }
      if (initial_item) { // bring up submenu containing initial item:
	menuwindow* n = new menuwindow(menutable,X,Y,W,H,initial_item,title,0,0,cw.x());
	pp.p[pp.nummenus++] = n;
	// move all earlier menus to line up with this new one:
	if (n->selected>=0) {
	  int dy = n->y()-nY;
	  int dx = n->x()-nX;
	  for (int menu = 0; menu <= pp.menu_number; menu++) {
	    menuwindow* tt = pp.p[menu];
	    int nx = tt->x()+dx; if (nx < 0) {nx = 0; dx = -tt->x();}
	    int ny = tt->y()+dy; if (ny < 0) {ny = 0; dy = -tt->y();}
	    tt->position(nx, ny);
	  }
	  setitem(pp.nummenus-1, n->selected);
	  goto STARTUP;
	}
      } else if (pp.nummenus > pp.menu_number+1 &&
		 pp.p[pp.menu_number+1]->menu == menutable) {
	// the menu is already up:
	while (pp.nummenus > pp.menu_number+2) delete pp.p[--pp.nummenus];
	pp.p[pp.nummenus-1]->set_selected(-1);
      } else {
	// delete all the old menus and create new one:
	while (pp.nummenus > pp.menu_number+1) delete pp.p[--pp.nummenus];
	pp.p[pp.nummenus++]= new menuwindow(menutable, nX, nY,
					  title?1:0, 0, 0, title, 0, menubar, cw.x());
      }
    } else { // !m->submenu():
      while (pp.nummenus > pp.menu_number+1) delete pp.p[--pp.nummenus];
      if (!pp.menu_number && pp.menubar) {
	// kludge so "menubar buttons" turn "on" by using menu title:
	pp.fakemenu = new menuwindow(0,
				  cw.x()+cw.titlex(pp.item_number),
				  cw.y()+cw.h(), 0, 0,
				  0, m, 0, 1);
	pp.fakemenu->title->show();
      }
    }
  }
  const fltk3::MenuItem* m = pp.current_item;
  delete pp.fakemenu;
  while (pp.nummenus>1) delete pp.p[--pp.nummenus];
  mw.hide();
  fltk3::grab(0);
  return m;
}

/**
  This method is called by widgets that want to display menus.

  The menu stays up until the user picks an item or dismisses it.
  The selected item (or NULL if none) is returned. <I>This does not
  do the callbacks or change the state of check or radio items.</I>

  X,Y is the position of the mouse cursor, relative to the
  window that got the most recent event (usually you can pass 
  fltk3::event_x() and fltk3::event_y() unchanged here).

  \p title is a character string title for the menu.  If
  non-zero a small box appears above the menu with the title in it.

  The menu is positioned so the cursor is centered over the item 
  picked.  This will work even if \p picked is in a submenu.
  If \p picked is zero or not in the menu item table the menu is
  positioned with the cursor in the top-left corner.

  \p button is a pointer to an fltk3::Menu_ from which the color and
  boxtypes for the menu are pulled.  If NULL then defaults are used.
*/
const fltk3::MenuItem* fltk3::MenuItem::popup(
  int X, int Y,
  const char* title,
  const fltk3::MenuItem* picked,
  const fltk3::Menu_* button
  ) const {
  static fltk3::MenuItem dummy; // static so it is all zeros
  dummy.text = title;
  return pulldown(X, Y, 0, 0, picked, button, title ? &dummy : 0);
}

/**
  Search only the top level menu for a shortcut.  
  Either &x in the label or the shortcut fields are used.

  This tests the current event, which must be an fltk3::KEYBOARD or 
  fltk3::SHORTCUT, against a shortcut value.

  \param ip returns the index of the item, if \p ip is not NULL.
  \param require_alt if true: match only if Alt key is pressed.

  \return found fltk3::MenuItem or NULL
*/
const fltk3::MenuItem* fltk3::MenuItem::find_shortcut(int* ip, const bool require_alt) const {
  const fltk3::MenuItem* m = this;
  if (m) for (int ii = 0; m->text; m = next_visible_or_not(m), ii++) {
    if (m->active()) {
      if (fltk3::test_shortcut(m->shortcut_)
	 || fltk3::Widget::test_shortcut(m->text, require_alt)) {
	if (ip) *ip=ii;
	return m;
      }
    }
  }
  return 0;
}

// Recursive search of all submenus for anything with this key as a
// shortcut.  Only uses the shortcut field, ignores &x in the labels:
/**
  This is designed to be called by a widgets handle() method in
  response to a fltk3::SHORTCUT event.  If the current event matches
  one of the items shortcut, that item is returned.  If the keystroke
  does not match any shortcuts then NULL is returned.  This only
  matches the shortcut() fields, not the letters in the title
  preceeded by '
*/
const fltk3::MenuItem* fltk3::MenuItem::test_shortcut() const {
  const fltk3::MenuItem* m = this;
  const fltk3::MenuItem* ret = 0;
  if (m) for (; m->text; m = next_visible_or_not(m)) {
    if (m->active()) {
      // return immediately any match of an item in top level menu:
      if (fltk3::test_shortcut(m->shortcut_)) return m;
      // if (fltk3::Widget::test_shortcut(m->text)) return m;
      // only return matches from lower menu if nothing found in top menu:
      if (!ret && m->submenu()) {
	const fltk3::MenuItem* s =
	  (m->flags&fltk3::SUBMENU) ? m+1:(const fltk3::MenuItem*)m->user_data_;
	ret = s->test_shortcut();
      }
    }
  }
  return ret;
}

//
// End of "$Id$".
//
