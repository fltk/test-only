//
// "$Id: Fl_Menu_add.cxx,v 1.36 2003/04/27 01:54:53 spitzak Exp $"
//
// Menu utilities for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// Methods to create/destroy the items in an Menu widget.  Used to
// emulate XForms and to provide convienence functions for building
// menus.

// These functions are provided for fltk 1.0 compatability and probably
// should be avoided in new programs. For new programs either create the
// widgets directly, or use List to return menu or browser items out
// of your own data.

// Compatability with fltk 1.0 and with XForms is only partial!

#include <fltk/Menu.h>
#include <fltk/Item.h>
#include <fltk/ItemGroup.h>
#include <fltk/Divider.h>
#include <string.h>
#include "ARRAY.h"

using namespace fltk;

////////////////////////////////////////////////////////////////

// Return a new menu item:
static int insert_here;

static Widget* append(
  Group* g,
  const char *text,
  int flags
) {
  Group* saved = Group::current();
  Group::current(0);
  Widget* o;
  if (flags & SUBMENU) {
    o = new ItemGroup();
  } else {
    o = new Item();
  }
  o->copy_label(text);
  if (flags & MENU_RADIO) o->type(Item::RADIO);
  else if (flags & MENU_TOGGLE) o->type(Item::TOGGLE);
  // these flags have been cleverly assigned so this shift and mask
  // converts from the old values to the new ones:
  o->set_flag((flags<<8)&(INACTIVE|VALUE|INVISIBLE));
  if (insert_here) {g->insert(*o, insert_here-1); insert_here = 0;}
  else g->add(o);
  if (flags & MENU_DIVIDER) g->add(new Divider());
  Group::current(saved);
  return o;
}

// Comparison that does not care about deleted '&' signs:
static int compare(const char* a, const char* b) {
  for (;;) {
    int n = *a-*b;
    if (n) {
      if (*a == '&') a++;
      else if (*b == '&') b++;
      else return n;
    } else if (*a) {
      a++; b++;
    } else {
      return 0;
    }
  }
}

static bool find_flag; // lame-o attempt to reuse the code
static bool replace_flag;
FL_API bool fl_menu_replaced; // hack so program can tell what replace() does

// Add an item.  The text is split at '/' characters to automatically
// produce submenus (actually a totally unnecessary feature as you can
// now add submenu titles directly by setting SUBMENU in the flags).
// The replace flag allows the item to be replaced if it already exists.
// Backslashes in the string "quote" the next character, which allows
// you to put forward slashes into a menu item.

Widget* Menu::add(
  const char *text,
  int shortcut,
  Callback *cb,	
  void *data,
  int flags
) {
  Group* group = this;

  int bufsize = strlen(text)+1;
  ARRAY(char, buf, bufsize);

  int flags1 = 0;
  const char* item;
  if (flags & RAW_LABEL) item = text;
  else for (;;) {    /* do all the supermenus: */

    // leading slash makes us assumme it is a filename:
    if (*text == '/') {item = text; break;}

    // leading underscore causes divider line:
    if (*text == '_') {text++; flags1 = MENU_DIVIDER;}

    // copy to buf, changing \x to x:
    char* q = buf;
    const char* p;
    for (p=text; *p && *p != '/'; *q++ = *p++) if (p[0]=='\\' && p[1]) p++;
    *q = 0;
    item = buf;

    // if not followed by slash it is not a menu title:
    if (*p != '/') break;

    // point at the next text:
    text = p+1;

    // find a matching menu title:
    for (int n = group->children();;) {
      if (!n) { // create a new menu
	if (find_flag) return 0;
	group = (Group*)append(group, item, SUBMENU|flags1);
	break;
      }
      Widget* w = group->child(--n);
      if (w->is_group() && w->label() && !compare(w->label(), item)) {
	group = (Group*)w;
	break;
      }
    }
    flags1 = 0;
  }

  Widget* o = 0;
  // names ending in '/' create an empty menu ore return the menu title:
  if (!*item) {
    o = group;
    if (find_flag) return o;
    fl_menu_replaced = true;
    goto REPLACED;
  }

  // find a matching menu item:
  if (replace_flag | find_flag) for (int n = group->children(); n--;) {
    Widget* w = group->child(n);
    if (w->label() && !compare(w->label(), item) && !w->is_group()) {
      o = w;
      if (find_flag) return o;
      fl_menu_replaced = true;
      goto REPLACED;
    }
  }
  if (find_flag) return 0;
  o = append(group, item, flags|flags1);
  fl_menu_replaced = false;

 REPLACED:
  /* fill it in */
  o->shortcut(shortcut);
  if (cb) o->callback(cb);
  o->user_data(data);
  relayout();
  return o;
}

// This is what menu::add() did in fltk 1.0, matching items were changed
// to the new value. Browser::add() always added new items.
Widget* Menu::replace(
  const char *text,
  int shortcut,
  Callback *cb,	
  void *data,
  int flags
) {
  replace_flag = true;
  Widget* ret = add(text, shortcut, cb, data, flags);
  replace_flag = false;
  return ret;
}

// This is a method from the old Browser:
Widget* Menu::add(const char* text, void* data) {
  return add(text, 0, 0, data, 0);
}

// This is a method from the old Browser:
Widget* Menu::insert(int n, const char* text, void* data) {
  insert_here = n+1;
  return add(text, 0, 0, data, 0);
}

// Does the exact same parsing as add() and return a pointer to the item,
// or return null if none:
Widget* Menu::find(const char* label) const {
  find_flag = true;
  Widget* r = ((Menu*)this)->add(label,0,0,0,0);
  find_flag = false;
  return r;
}

// This is a Forms (and SGI GL library) compatable add function, it
// adds many menu items, with '|' seperating the menu items, and tab
// seperating the menu item names from an optional shortcut string.
Widget* Menu::add(const char *str) {
  int bufsize = strlen(str)+1;
  ARRAY(char, buf, bufsize);
  Widget* r = 0;
  while (*str) {
    const char* start = str;
    int shortcut = 0;
    char *c;
    for (c = buf; *str && *str != '|'; str++) {
//    if (*str == '\t') {*c++ = 0; shortcut = old_shortcut(str);}
//    else
      *c++ = *str;
    }
    *c = 0;
    if (*str) {
      r = add(buf, shortcut, 0, 0, 0);
      str++;
    } else {
      r = add(start, shortcut, 0, 0, 0);
    }
  }
  return r;
}

//
// End of "$Id: Fl_Menu_add.cxx,v 1.36 2003/04/27 01:54:53 spitzak Exp $".
//
