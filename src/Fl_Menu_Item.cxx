//
// "$Id: Fl_Menu_Item.cxx,v 1.4 2002/01/20 07:37:15 spitzak Exp $"
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

#include <fltk/Fl_Menu_Item.h>
#include <fltk/Fl_Menu_.h>
#include <fltk/Fl_Item.h>
#include <fltk/Fl_Item_Group.h>
#include <fltk/Fl_Divider.h>

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
      if (m->flags & FL_MENU_RADIO) o->type(Fl_Item::RADIO);
      else if (m->flags & FL_MENU_TOGGLE) o->type(Fl_Item::TOGGLE);
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

void Fl_Menu_Item::add_to(Fl_Menu_* menu, void* data) const {
  ::add(menu, this, data);
}
    
// Emulate old popup and test-shortcut methods on Fl_Menu_Item arrays:
#include <fltk/Fl_Menu_Button.h>
#include <fltk/Fl.h>

const Fl_Menu_Item*
Fl_Menu_Item::pulldown(int X, int Y, int W, int H,
		       const Fl_Menu_Item* picked,
		       const char* title) const
{
  Fl_Group::current(0);
  Fl_Item dummy(title);
  Fl_Menu_Button menu(0,0,0,0);
  menu.menu(this);
  //menu.user_data(data);
  if (picked) menu.value(picked-this);
  if (menu.Fl_Menu_::popup(X, Y, W, H, title ? &dummy : 0))
    return this + menu.value();
  return 0;
}

const Fl_Menu_Item*
Fl_Menu_Item::test_shortcut() const
{
  Fl_Group::current(0);
  Fl_Menu_ menu(0,0,0,0);
  menu.menu(this);
  //menu.user_data(data);
  if (menu.handle_shortcut()) return this + menu.value();
  return 0;
}
