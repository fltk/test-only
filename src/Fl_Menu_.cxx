//
// "$Id: Fl_Menu_.cxx,v 1.35 2001/07/25 19:05:44 robertk Exp $"
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
#include <fltk/Fl_Item.h> // for FL_TOGGLE_ITEM, FL_RADIO_ITEM

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
  if (!widget) {do_callback(); return;}
  if (widget->type() == FL_TOGGLE_ITEM) {
    if (widget->value()) widget->clear_value(); else widget->set_value();
  } else if (widget->type() == FL_RADIO_ITEM) {
    widget->set_value();
    Fl_Group* g = widget->parent();
    int i = g->find(widget);
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

  Fl_Callback* callback = widget->callback();

  // If the item's callback is not set, use the menu's callback:
  if (callback == Fl_Widget::default_callback) callback = this->callback();

#if 0
  // Notice that "this" is passed as the widget. This appears to
  // be necessary for back compatability. You can use item() to
  // get the actual widget.
  callback(this, widget->user_data());
#endif
  // If the item has data, use it, else use the menus data:
  void* data = widget->user_data() ? widget->user_data() : this->user_data(); 
  // If the item's callback is not set, use the menu's callback:
  if (widget->callback() == Fl_Widget::default_callback) {
    if (!data) data = widget;
    widget = this;
  }
  callback(widget, data);
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

int Fl_Menu_::goto_item(const int* indexes, int level) {
  int i = indexes[0];
  bool ret = false;
  if (focus() != i) {focus(i); ret = true;}
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

#if 0
// Before I restored the item_ field, I used this method to go to the
// current item by using the focus_ pointers to track down to it.
Fl_Widget* Fl_Menu_::get_item() const {
  int i = focus();
  Fl_Widget* item = child(i);
  while (item && item->is_group()) {
    Fl_Group* group = (Fl_Group*)item;
    int i = group->focus();
    if (i < 0 || i >= group->children()) break;
    item = group->child(i);
  }
  return item;
}
#endif

////////////////////////////////////////////////////////////////

// If an Fl_List is used, the search for shortcut keys will only search
// the top level items, and children where a real Fl_Group with children
// widgets is returned. That is, shortcuts on nested items will not be
// found from Fl_List (except for ones that return real Fl_Groups, such
// as by copying another Fl_Menu_).
//
// For most uses of Fl_List I think this is ok. It also avoids enumerating
// all the items that an Fl_List can return.

// recursive innards of handle_shortcut:
static Fl_Widget* shortcut_search(Fl_Group* g) {
  Fl_Widget* widget = 0;
  for (int i = 0; i < g->children(); i++) {
    Fl_Widget* item = g->child(i);
    if (!item->takesevents()) continue;
    if (Fl::test_shortcut(item->shortcut())) {g->focus(i); return item;}
    if (!widget && item->is_group() /*&& IS_OPEN*/) {
      widget = shortcut_search((Fl_Group*)item);
      if (widget) g->focus(i);
    }
  }
  return widget;
}

int Fl_Menu_::handle_shortcut() {
  Fl_Widget* widget = 0;
  for (int i = 0; i < children(); i++) {
    Fl_Widget* item = child(i);
    if (!item->takesevents()) continue;
    if (Fl::test_shortcut(item->shortcut())) {focus(i); widget=item; break;}
    if (!widget && item->is_group() /*&& IS_OPEN*/) {
      widget = shortcut_search((Fl_Group*)item);
      if (widget) focus(i);
    }
  }
  if (widget) {execute(widget); return 1;}
  return 0;
}

//
// End of "$Id: Fl_Menu_.cxx,v 1.35 2001/07/25 19:05:44 robertk Exp $"
//
