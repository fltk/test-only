//
// "$Id: Fl_Menu_.cxx,v 1.32 2000/10/17 07:50:08 spitzak Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Item.H> // for FL_TOGGLE_ITEM, FL_RADIO_ITEM

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

void Fl_List::flags_changed(const Fl_Menu_*, Fl_Widget* w) {}

static Fl_List default_list; // this should be local!

Fl_Menu_::Fl_Menu_(int x,int y,int w, int h,const char* l)
  : Fl_Group(x,y,w,h,l), list_(&default_list) {
  item_ = 0;
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
void Fl_Menu_::execute() {
  if (fl_dont_execute) return;
  Fl_Widget* w = item();
  if (!w) {do_callback(); return;}
  if (w->type() == FL_TOGGLE_ITEM) {
    if (w->value()) w->clear_value(); else w->set_value();
  } else if (w->type() == FL_RADIO_ITEM) {
    w->set_value();
    Fl_Group* g = w->parent();
    int i = g->find(w);
    int j;
    for (j = i-1; j >= 0; j--) {
      Fl_Widget* o = g->child(j);
      if (o->type() != FL_RADIO_ITEM) break;
      o->clear_value();
    }
    for (j = i+1; j < g->children(); j++) {
      Fl_Widget* o = g->child(j);
      if (o->type() != FL_RADIO_ITEM) break;
      o->clear_value();
    }
  }
  // If the item's data is zero, use the menus data:
  void* data = w->user_data(); if (!data) data = user_data();
  // If the item's callback is not set, use the menu's callback:
  if (w->callback() == Fl_Widget::default_callback) {
    if (!data) data = w;
    w = this;
  }
  w->do_callback(w, data);
}

int Fl_Menu_::goto_item(const int* indexes, int level) {
  // The current item is remembered in the focus index from the Fl_Group
  // at each level.  This is used by popup menus to pop up at the same
  // item next time.
  // If an Fl_List is used and it does not return Fl_Groups then the
  // position is not remembered. For this reason Fl_Browser uses it's
  // own storage of the indexes and replaces this function.
  focus(indexes[0]);
  for (int l = 0; l <= level; l++) {
    item(child(indexes, l));
    if (!item() || !item()->is_group()) continue;
    Fl_Group* group = (Fl_Group*)item();
    group->focus(l < level ? indexes[l+1] : -1);
  }
  // The return value is for subclasses that check to see if the current
  // item has changed.
  return 1;
}

////////////////////////////////////////////////////////////////

// recursive innards of handle_shortcut:
static Fl_Widget* shortcut_search(Fl_Group* g) {
  Fl_Widget* ret = 0;
  for (int i = 0; i < g->children(); i++) {
    Fl_Widget* w = g->child(i);
    if (!w->takesevents()) continue;
    if (Fl::test_shortcut(w->shortcut())) {g->focus(i); return w;}
    if (!ret && w->is_group() /*&& IS_OPEN*/) {
      ret = shortcut_search((Fl_Group*)w);
      if (ret) g->focus(i);
    }
  }
  return ret;
}

int Fl_Menu_::handle_shortcut() {
  Fl_Widget* widget = shortcut_search(this);
  if (widget) {item(widget); execute(); return 1;}
  return 0;
}

//
// End of "$Id: Fl_Menu_.cxx,v 1.32 2000/10/17 07:50:08 spitzak Exp $"
//
