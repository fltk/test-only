//
// "$Id: Fl_Menu_Item.cxx,v 1.2 2000/09/11 07:29:33 spitzak Exp $"
//
// Menu code for the Fast Light Tool Kit (FLTK).
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

// The obsolete Fl_Menu_Item structure.  This code should not be used
// in new fltk programs.

#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Item.H>
#include <FL/Fl_Item_Group.H>
#include <FL/Fl_Divider.H>

int Fl_Menu_Item::size() const {
  const Fl_Menu_Item* m = this;
  int nest = 0;
  for (;;) {
    if (!m->text) {
      if (!nest) return (m-this+1);
      nest--;
    } else if (m->flags & FL_SUBMENU) {
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
    } else if (m->flags & FL_SUBMENU) {
      nest++;
    }
    m++;
    if (!nest && !(m->flags & FL_MENU_INVISIBLE)) n--;
  }
  return m;
}

// Recursive function to create Fl_Items and add them to a group:

static const Fl_Menu_Item* add(Fl_Group* g, const Fl_Menu_Item* m,void* data) {
  Fl_Group* saved = Fl_Group::current();
  g->begin();
  while (m && m->text) {
    Fl_Widget* o;
    const Fl_Menu_Item* next = m+1;
    if (m->flags & (FL_SUBMENU | FL_SUBMENU_POINTER)) {
      Fl_Item_Group* g = new Fl_Item_Group(m->text); g->end(); o = g;
      if (m->flags & FL_SUBMENU_POINTER) {
	add(g, (Fl_Menu_Item*)(m->user_data_), data);
      } else {
	next = add(g, next, data) + 1;
      }
    } else {
      o = new Fl_Item(m->text);
      o->shortcut(m->shortcut_);
      if (m->callback_) o->callback(m->callback_);
      o->user_data(data ? data : m->user_data_);
      if (m->flags & FL_MENU_TOGGLE) o->type(FL_TOGGLE_ITEM);
      if (m->flags & FL_MENU_RADIO) o->type(FL_RADIO_ITEM);
    }
    if (m->labeltype_) o->label_type(m->labeltype_);
    if (m->labelfont_) o->label_font(m->labelfont_);
    if (m->labelsize_) o->label_size(m->labelsize_);
    if (m->labelcolor_) o->label_color(m->labelcolor_);
    // these flags have been cleverly assigned so this shift and mask
    // converts from the old values to the new ones:
    o->set_flag((m->flags<<8)&(FL_INACTIVE|FL_VALUE|FL_INVISIBLE));
    if (m->flags & FL_MENU_DIVIDER) new Fl_Divider();
    m = next;
  }
  Fl_Group::current(saved);
  return m;
}

void Fl_Menu_::add(const Fl_Menu_Item* m, void* data) {
  ::add(this, m, data);
}
    
// Emulate old popup and test-shortcut methods on Fl_Menu_Item arrays:
#include <FL/Fl_Menu_Button.H>

static Fl_Menu_* get_menu(const Fl_Menu_Item* m, void* data) {
  static const Fl_Menu_Item* cached_item;
  static Fl_Menu_Button* cached_menu;
  if (m != cached_item) {
    if (!cached_menu) {
      Fl_Group::current(0); // fix missing end()
      cached_menu = new Fl_Menu_Button(0,0,0,0);
    }
    cached_menu->menu(m);
    cached_item = m;
  }
  cached_menu->user_data(data);
  return cached_menu;
}

int Fl_Menu_Item::popup(int X, int Y, const char *title, void* data) const {
  return get_menu(this,data)->popup(X,Y,title);
}

int Fl_Menu_Item::test_shortcut(void* data) const {
  return get_menu(this,data)->handle_shortcut();
}

