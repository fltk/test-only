//
// "$Id: Fl_Menu_.cxx,v 1.28 2000/08/21 03:56:24 spitzak Exp $"
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

FL_API int fl_dont_execute = 0; // hack for fluid

void Fl_Menu_::execute(Fl_Widget* w) {
  if (fl_dont_execute) return;
  if (w->type() == FL_TOGGLE_ITEM) {
    if (w->value()) w->clear_value(); else w->set_value();
  } else if (w->type() == FL_RADIO_ITEM) {
    w->set_value();
    Fl_Group* g = w->parent();
    int i = g->find(w);
    int j;
    for (j = i-1; j >= 0; j--) {
      Fl_Widget* o = g->child(j);
      if (o->type() == FL_RADIO_ITEM) o->clear_value();
      else break;
    }
    for (j = i+1; j < g->children(); j++) {
      Fl_Widget* o = g->child(j);
      if (o->type() == FL_RADIO_ITEM) o->clear_value();
      else break;
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

Fl_Widget* Fl_Menu_::item() const {
  const Fl_Group* parent = this;
  Fl_Widget* widget = 0;
  for (;;) {
    int i = parent->focus();
    if (i < 0) break;
    if (i >= parent->children()) return 0; // some kind of foul-up
    widget = parent->child(i);
    if (!widget->is_group()) break;
    parent = (Fl_Group*)widget;
  }
  return widget;
}

void Fl_Menu_::item(Fl_Widget* w) {
  if (!w) {focus(-1); return;}
  while (w->parent()) {
    w->parent()->focus(w);
    w = w->parent();
  }
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
  Fl_Widget* w = shortcut_search(this);
  if (w) {execute(w); return 1;}
  return 0;
}

//
// End of "$Id: Fl_Menu_.cxx,v 1.28 2000/08/21 03:56:24 spitzak Exp $"
//
