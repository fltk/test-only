//
// "$Id: Fl_Menu_Item.cxx,v 1.6 2002/12/09 04:52:26 spitzak Exp $"
//
// The obsolete MenuItem structure.  This code should not be used
// in new fltk programs.
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#include <FL/Fl_Menu_Item.h>
#include <fltk/Menu.h>
#include <fltk/Item.h>
#include <fltk/ItemGroup.h>
#include <fltk/Divider.h>
using namespace fltk;

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

// Recursive function to create Items and add them to a group:

static const Fl_Menu_Item* add(Group* g, const Fl_Menu_Item* m,void* data) {
  Group* saved = Group::current();
  g->begin();
  while (m && m->text) {
    Widget* o;
    const Fl_Menu_Item* next = m+1;
    if (m->flags & (FL_SUBMENU | FL_SUBMENU_POINTER)) {
      ItemGroup* g = new ItemGroup(m->text); g->end(); o = g;
      if (m->flags & FL_SUBMENU_POINTER) {
	add(g, (Fl_Menu_Item*)(m->user_data_), data);
      } else {
	next = add(g, next, data) + 1;
      }
    } else {
      o = new Item(m->text);
      o->shortcut(m->shortcut_);
      if (m->callback_) o->callback(m->callback_);
      o->user_data(data ? data : m->user_data_);
      if (m->flags & FL_MENU_RADIO) o->type(Item::RADIO);
      else if (m->flags & FL_MENU_TOGGLE) o->type(Item::TOGGLE);
    }
    if (m->labeltype_) o->labeltype(m->labeltype_);
    if (m->labelfont_) o->labelfont(m->labelfont_);
    if (m->labelsize_) o->labelsize(m->labelsize_);
    if (m->labelcolor_) o->labelcolor(m->labelcolor_);
    // these flags have been cleverly assigned so this shift and mask
    // converts from the old values to the new ones:
    o->set_flag((m->flags<<8)&(INACTIVE|VALUE|INVISIBLE));
    if (m->flags & FL_MENU_DIVIDER) new Divider();
    m = next;
  }
  Group::current(saved);
  return m;
}

void Fl_Menu_Item::add_to(Menu* menu, void* data) const {
  ::add(menu, this, data);
}
    
// Emulate old popup and test-shortcut methods on Fl_Menu_Item arrays:
#include <fltk/PopupMenu.h>
#include <fltk/events.h>

const Fl_Menu_Item*
Fl_Menu_Item::pulldown(int X, int Y, int W, int H,
		       const Fl_Menu_Item* picked,
		       const char* title) const
{
  Group::current(0);
  Item dummy(title);
  PopupMenu menu(0,0,0,0);
  add_to(&menu);
  //menu.user_data(data);
  if (picked) menu.value(picked-this);
  if (menu.Menu::popup(X, Y, W, H, title ? &dummy : 0))
    return this + menu.value();
  return 0;
}

const Fl_Menu_Item*
Fl_Menu_Item::test_shortcut() const
{
  Group::current(0);
  Menu menu(0,0,0,0);
  add_to(&menu);
  //menu.user_data(data);
  if (menu.handle_shortcut()) return this + menu.value();
  return 0;
}
