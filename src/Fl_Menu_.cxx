//
// "$Id: Fl_Menu_.cxx,v 1.47 2003/01/21 07:53:39 spitzak Exp $"
//
// The Menu base class is used by browsers, choices, menu bars
// menu buttons, and perhaps other things.  It is simply an Group
// but provides functions to select and identify one of the widgets
// in the hierarchy below it and do that widget's callback directly.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/events.h>
#include <fltk/Menu.h>

#include <fltk/Item.h> // for TOGGLE, RADIO
#define checkmark(item) (item->type()>=Item::TOGGLE && item->type()<=Item::RADIO)

using namespace fltk;

////////////////////////////////////////////////////////////////

// The base List class just returns the widget from the Group's
// children.  All Menus share a single instance of this by default,
// so the default behavior is that child widgets appear as items in
// the menu or browser.

// Subclasses of List may want to call the base class to allow
// normal widgets to be prepended to whatever they return.

int List::children(const Menu* menu, const int* indexes, int level) {
  Group* group = (Group*)menu;
  while (level--) {
    int i = *indexes++;
    //if (i < 0 || i >= group->children()) return -1;
    Widget* widget = group->child(i);
    if (!widget->is_group()) return -1;
    group = (Group*)widget;
  }
  return group->children();
}

Widget* List::child(const Menu* menu, const int* indexes,int level) {
  Group* group = (Group*)menu;
  for (;;) {
    int i = *indexes++;
    //if (i < 0 || i >= group->children()) return 0;
    Widget* widget = group->child(i);
    if (!level--) return widget;
    if (!widget->is_group()) return 0;
    group = (Group*)widget;
  }
}

void List::flags_changed(const Menu*, Widget*) {}

static List default_list;

Menu::Menu(int x,int y,int w, int h,const char* l)
  : Group(x,y,w,h,l), list_(&default_list), item_(0) {
  callback(default_callback);
  end();
}

int Menu::children(const int* indexes, int level) const {
  return list_->children(this, indexes, level);
}

int Menu::children() const {
  return list_->children(this, 0, 0);
}

Widget* Menu::child(const int* indexes, int level) const {
  return list_->child(this, indexes, level);
}

Widget* Menu::child(int n) const {
  return list_->child(this, &n, 0);
}

////////////////////////////////////////////////////////////////

FL_API bool fl_dont_execute; // hack for fluid

// Do the callback for the current item:
void Menu::execute(Widget* widget) {
  item(widget);
  if (fl_dont_execute) return;
  if (!widget) return; // never do callback when no widget is picked
#if 0
  // This is removed, hopefully we can put *real* buttons into the menus
  // and it breaks the ability to put real ones into browsers.
  if (widget->type() == Item::RADIO) {
    widget->set_value();
    Group* g = widget->parent();
    int i = g->find(widget);
    int j;
    for (j = i-1; j >= 0; j--) {
      Widget* o = g->child(j);
      if (o->type() != Item::RADIO) break;
      o->clear_value();
    }
    for (j = i+1; j < g->children(); j++) {
      Widget* o = g->child(j);
      if (o->type() != Item::RADIO) break;
      o->clear_value();
    }
  } else if (checkmark(widget)) {
    if (widget->value()) widget->clear_value(); else widget->set_value();
  }
#endif
  do_callback();
}

// Normally the callback for the menu is set to this:
void Menu::default_callback(Widget* widget, void*) {
  Widget* item = ((Menu*)widget)->item();
  if (item) item->do_callback();
}

////////////////////////////////////////////////////////////////
//
// The current item is remembered in the focus index from the Group
// at each level.  This is used by popup menus to pop up at the same
// item next time.
//
// Storing it this way allows widgets to be inserted and deleted and
// the currently selected item does not change (because Group updates
// the focus index). But if an List is used and it does not return
// a different Group for each level in the hierarchy, the focus
// indexes will write over each other. Browser currently uses it's
// own code (with the insert/delete broken) to get around this.
//
// item() is set to the located widget.
// True is returned if the indexes differ from last time.

bool Menu::focus(const int* indexes, int level) {
  int i = indexes[0];
  bool ret = false;
  if (value() != i) {value(i); ret = true;}
  if (i < 0 || i >= children()) {item(0); return ret;}
  item(child(i));
  int j = 1;
  while (item() && item()->is_group()) {
    Group* group = (Group*)item();
    int i = (j > level) ? -1 : indexes[j++];
    if (group->focus() != i) {group->focus(i); ret = true;}
    if (i < 0 || i >= group->children()) break;
    item(group->child(i));
  }
  return ret;
}

// Set item() according to the focus fields. item() may have been altered
// by the widget drawing or because an List is being used. The new
// value for item() is returned.
Widget* Menu::get_focus() {
  int i = value();
  if (i < 0 || i >= children()) {item(0); return 0;}
  item(child(i));
  while (item() && item()->is_group()) {
    Group* group = (Group*)item();
    int i = group->focus();
    if (i < 0 || i >= group->children()) break;
    item(group->child(i));
  }
  return item();
}

////////////////////////////////////////////////////////////////

// Shortcuts only search the immediate children of an Menu that
// uses an List. If the List returns an Group indicating
// nested children, the actual children of that group are searched,
// rather than asking the List to enumerate all of them. This is
// necessary because some Lists are infinite in size, and usually
// they don't have shortcuts anyway.

// This will return invisible (but active) items, even
// though it is impossible to select these items with the gui. This
// is done so that more than one shortcut for an action may be given
// by putting multiple copies of the item in, where only the first is
// visible.

// recursive innards of handle_shortcut:
static Widget* shortcut_search(Group* g) {
  Widget* widget = 0;
  for (int i = 0; i < g->children(); i++) {
    Widget* item = g->child(i);
    if (!item->active()) continue;
    if (test_shortcut(item->shortcut())) {g->focus(i); return item;}
    if (!widget && item->is_group() /*&& IS_OPEN*/) {
      widget = shortcut_search((Group*)item);
      if (widget) g->focus(i);
    }
  }
  return widget;
}

int Menu::handle_shortcut() {
  if (event_clicks()) return 0; // ignore repeating keys
  Widget* widget = 0;
  int children = this->children();
  for (int i = 0; i < children; i++) {
    Widget* item = child(i);
    if (!item->takesevents()) continue;
    if (fltk::test_shortcut(item->shortcut())) {value(i); widget=item; break;}
    if (!widget && item->is_group() /*&& IS_OPEN*/) {
      widget = shortcut_search((Group*)item);
      if (widget) value(i);
    }
  }
  if (widget) {execute(widget); return 1;}
  return 0;
}

//
// End of "$Id: Fl_Menu_.cxx,v 1.47 2003/01/21 07:53:39 spitzak Exp $"
//
