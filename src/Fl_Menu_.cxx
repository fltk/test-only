//
// "$Id: Fl_Menu_.cxx,v 1.41 2002/01/20 07:37:15 spitzak Exp $"
//
// The Fl_Menu_ base class is used by browsers, choices, menu bars
// menu buttons, and perhaps other things.  It is simply an Fl_Group
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/Fl.h>
#include <fltk/Fl_Menu_.h>
#include <fltk/Fl_Item.h> // for TOGGLE, RADIO
#define checkmark(item) (item->type()>=Fl_Item::TOGGLE && item->type()<=Fl_Item::RADIO)

////////////////////////////////////////////////////////////////

// The base Fl_List class just returns the widget from the Fl_Group's
// children.  All Fl_Menu_s share a single instance of this by default,
// so the default behavior is that child widgets appear as items in
// the menu or browser.

// Subclasses of Fl_List may want to call the base class to allow
// normal widgets to be prepended to whatever they return.

int Fl_List::children(const Fl_Menu_* menu, const int* indexes, int level) {
  Fl_Group* group = (Fl_Group*)menu;
  while (level--) {
    int i = *indexes++;
    if (i < 0 || i >= group->children()) return -1;
    Fl_Widget* widget = group->child(i);
    if (!widget->is_group()) return -1;
    group = (Fl_Group*)widget;
  }
  return group->children();
}

Fl_Widget* Fl_List::child(const Fl_Menu_* menu, const int* indexes,int level) {
  Fl_Group* group = (Fl_Group*)menu;
  for (;;) {
    int i = *indexes++;
    if (i < 0 || i >= group->children()) return 0;
    Fl_Widget* widget = group->child(i);
    if (!level--) return widget;
    if (!widget->is_group()) return 0;
    group = (Fl_Group*)widget;
  }
}

void Fl_List::flags_changed(const Fl_Menu_*, Fl_Widget*) {}

static Fl_List default_list;

Fl_Menu_::Fl_Menu_(int x,int y,int w, int h,const char* l)
  : Fl_Group(x,y,w,h,l), list_(&default_list), item_(0) {
  callback(default_callback);
  end();
}

int Fl_Menu_::children(const int* indexes, int level) const {
  return list_->children(this, indexes, level);
}

int Fl_Menu_::children() const {
  return list_->children(this, 0, 0);
}

Fl_Widget* Fl_Menu_::child(const int* indexes, int level) const {
  return list_->child(this, indexes, level);
}

Fl_Widget* Fl_Menu_::child(int n) const {
  return list_->child(this, &n, 0);
}

////////////////////////////////////////////////////////////////

FL_API int fl_dont_execute = 0; // hack for fluid

// Do the callback for the current item:
void Fl_Menu_::execute(Fl_Widget* widget) {
  item(widget);
  if (fl_dont_execute) return;
  if (!widget) return; // never do callback when no widget is picked
  if (widget->type() == Fl_Item::RADIO) {
    widget->set_value();
    Fl_Group* g = widget->parent();
    int i = g->find(widget);
    int j;
    for (j = i-1; j >= 0; j--) {
      Fl_Widget* o = g->child(j);
      if (o->type() != Fl_Item::RADIO) break;
      o->clear_value();
    }
    for (j = i+1; j < g->children(); j++) {
      Fl_Widget* o = g->child(j);
      if (o->type() != Fl_Item::RADIO) break;
      o->clear_value();
    }
  } else if (checkmark(widget)) {
    if (widget->value()) widget->clear_value(); else widget->set_value();
  }

  do_callback();
}

// Normally the callback for the menu is set to this:
void Fl_Menu_::default_callback(Fl_Widget* widget, void*) {
  ((Fl_Menu_*)widget)->item()->do_callback();
}

////////////////////////////////////////////////////////////////
//
// The current item is remembered in the focus index from the Fl_Group
// at each level.  This is used by popup menus to pop up at the same
// item next time.
//
// Storing it this way allows widgets to be inserted and deleted and
// the currently selected item does not change (because Fl_Group updates
// the focus index). But if an Fl_List is used and it does not return
// a different Fl_Group for each level in the hierarchy, the focus
// indexes will write over each other. Fl_Browser currently uses it's
// own code (with the insert/delete broken) to get around this.
//
// item() is set to the located widget.
// True is returned if the indexes differ from last time.

bool Fl_Menu_::focus(const int* indexes, int level) {
  int i = indexes[0];
  bool ret = false;
  if (value() != i) {value(i); ret = true;}
  item(child(i));
  int j = 1;
  while (item() && item()->is_group()) {
    Fl_Group* group = (Fl_Group*)item();
    int i = (j > level) ? -1 : indexes[j++];
    if (group->focus() != i) {group->focus(i); ret = true;}
    if (i < 0 || i >= group->children()) break;
    item(group->child(i));
  }
  return ret;
}

// Set item() according to the focus fields. item() may have been altered
// by the widget drawing or because an Fl_List is being used. The new
// value for item() is returned.
Fl_Widget* Fl_Menu_::get_focus() {
  int i = value();
  item(child(i));
  while (item() && item()->is_group()) {
    Fl_Group* group = (Fl_Group*)item();
    int i = group->focus();
    if (i < 0 || i >= group->children()) break;
    item(group->child(i));
  }
  return item();
}

////////////////////////////////////////////////////////////////

// Shortcuts only search the immediate children of an Fl_Menu_ that
// uses an Fl_List. If the Fl_List returns an Fl_Group indicating
// nested children, the actual children of that group are searched,
// rather than asking the Fl_List to enumerate all of them. This is
// necessary because some Fl_Lists are infinite in size, and usually
// they don't have shortcuts anyway.

// This will return invisible (but active) items, even
// though it is impossible to select these items with the gui. This
// is done so that more than one shortcut for an action may be given
// by putting multiple copies of the item in, where only the first is
// visible.

// recursive innards of handle_shortcut:
static Fl_Widget* shortcut_search(Fl_Group* g) {
  Fl_Widget* widget = 0;
  for (int i = 0; i < g->children(); i++) {
    Fl_Widget* item = g->child(i);
    if (!item->active()) continue;
    if (Fl::test_shortcut(item->shortcut())) {g->focus(i); return item;}
    if (!widget && item->is_group() /*&& IS_OPEN*/) {
      widget = shortcut_search((Fl_Group*)item);
      if (widget) g->focus(i);
    }
  }
  return widget;
}

int Fl_Menu_::handle_shortcut() {
  if (Fl::event_clicks()) return 0; // ignore repeating keys
  Fl_Widget* widget = 0;
  int children = this->children();
  for (int i = 0; i < children; i++) {
    Fl_Widget* item = child(i);
    if (!item->takesevents()) continue;
    if (Fl::test_shortcut(item->shortcut())) {value(i); widget=item; break;}
    if (!widget && item->is_group() /*&& IS_OPEN*/) {
      widget = shortcut_search((Fl_Group*)item);
      if (widget) value(i);
    }
  }
  if (widget) {execute(widget); return 1;}
  return 0;
}

//
// End of "$Id: Fl_Menu_.cxx,v 1.41 2002/01/20 07:37:15 spitzak Exp $"
//
