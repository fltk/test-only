//
// "$Id: Fl_Menu_.cxx,v 1.55 2004/01/25 08:49:51 spitzak Exp $"
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

/*! \class fltk::List

  Allows a Browser or Choice or other subclass of Menu to display a
  hierarchy of data that is managed by the application rather than
  FLTK.

  This is done by making a subclass of List which creats a "dummy" widget,
  typically a subclass of Item, that describes a particular item that
  the browser or menu should display. Only one item is examined at any
  time and thus the dummy widget can be reused, so there is very
  little space overhead.

  This is designed for data formats where finding the Nth child of a
  parent is a very quick operation, ie an array. If your data is a
  list you can search it, the performance is probably acceptable for
  small lists with less than a hundred or so items. For a
  bidirectional list it may be useful to cache the last request and do
  a relative search, a Browser and Menu will usually ask for adjoining
  items.

  If you wish to make a hierarcial Browser, you must have space in
  your data to store the state of the fltk::VALUE flag on each parent
  item, and must implement the flags_changed() method.

  If you wish to use an MultiBrowser you must also have space in your
  data to store the state of the fltk::SELECTED flag on each item, and
  and must implement the flags_changed() method.

  The base List class returns the child widgets from the Menu that
  owns it.  All Menus share a single instance of this by default, so
  the default behavior is that child widgets appear as items in the
  menu or browser. Subclasses of List may want to call the base class
  to allow normal widgets to be prepended to whatever they return.
*/

/*!
  Return how many children are under a given item. If level is
  zero, this should return how many items are at the top
  level. Otherwise indexes is an array of level numbers indicating the
  index of an item at the top level, the index of the an item that is
  the child of that, and so on.

  This should return -1 if the item is not a "parent" item or the
  index array is illegal. It is not necessary to return the correct
  value until the parent is "open", which means the fltk::VALUE flag
  was set in it, so if it is expensive to calculate the number you can
  return 1 for any closed parent.

  Here is a sample implementation, where Node is a data type that you
  have defined:

\code
int My_List::children(const fltk::Menu*, const int* indexes, int level) {
  Node* node = root;
  for (int l = 0; l < level; l++) {
    if (indexes[l] >= node->children_count()) return -1;
    node = node->child(indexes[l]);
    if (!node->is_parent()) return -1;
  }
  return node->children_count();
}
\endcode
*/
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

/*!
  Return a given child as a widget. draw() and measure() will be
  called on this widget to figure out where to place it and to draw
  it. Typical implementations create a reusable fltk::Item and fill it
  in with the correct data. This should return NULL if there is
  anything illegal about the indexes.

  Here is a sample implementation, where Node is a data type that you
  have defined. This demonstrates how to create the dummy widget:

\code
fltk::Widget* My_List::child(const fltk::Menu*, const int* indexes, int level) {
  Node* node = root;
  for (int l = 0; l <= level; l++) {
    if (!node->is_parent()) return 0;
    if (indexes[l] >= node->children_count()) return 0;
    node = node->child(indexes[l]);
  }
  static fltk::Widget* widget;
  if (!widget) {
    fltk::Group::current(0);
    widget = new fltk::Item();
  }
  widget->label(node->text());
  widget->w(0); // cause measure() to be called
  widget->user_data(node);
  if (node->selected) widget->set_flag(fltk::SELECTED);
  else widget->clear_flag(fltk::SELECTED);
  if (node->is_parent() && node->open) widget->set_flag(fltk::VALUE);
  else widget->clear_flag(fltk::VALUE);
  return widget;
}
\endcode
*/
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

/*!
  This is called if the browser changes any flags on a widget, so that
  you can copy the values to permanent storage, and perhaps change
  other displays of the selection.

  Currently only the fltk::VALUE and fltk::SELECTED flags are ever changed. 

  Here is a sample implementation, where Node is a data type that you
  have defined:
\code
void My_List::flags_changed(const fltk::Menu*, fltk::Widget* widget) {
  Node* node = (Node*)(widget->user_data());
  node->open = (widget->flags() & fltk::VALUE) !=0;
  node->selected = (widget->flags() & fltk::SELECTED) != 0;
}
\endcode
*/
void List::flags_changed(const Menu*, Widget*) {}

static List default_list;

////////////////////////////////////////////////////////////////

/*! \class fltk::Menu

  All widgets that display a (possibly hierarchial) list of similar
  items are subclassed off of fltk::Menu. This includes scrolling
  browsers, pop-up and pull-down menus and menubars, and the
  Windows-style "combo box".

  This is a subclass of Group and each item is a child Widget, usually
  Item widgets, or ItemGroup widgets to make a hierarchy.

  A Menu can take a pointer to a List object, which allows the
  user program to dynamically provide the items as they are
  needed. This is much easier than trying to maintain an array of
  Widgets in parallel with your own data structures.

  It also provides several convienence functions for creating,
  rearranging, and deleting child Item and ItemGroup widgets.
*/

Menu::Menu(int x,int y,int w, int h,const char* l)
  : Group(x,y,w,h,l), list_(&default_list), item_(0) {
  callback(default_callback);
}

/*! \fn void Menu::list(List* list)
  Set the List that generates widgets for the menu. By default this
  is a dummy List that returns the child widgets of the Menu.
*/

/*! Calls list()->children(this, indexes, level) */
int Menu::children(const int* indexes, int level) const {
  return list_->children(this, indexes, level);
}

/*!
  Returns the number of children at the top level. Same as children(0,0). 

  <i>This Overrides the method of the same name on fltk::Group</i>. This is
  so that an fltk::List can be used. However if no fltk::List is
  specified the action is identical to fltk::Group::children().
*/
int Menu::children() const {
  return list_->children(this, 0, 0);
}

/*! Calls list()->child(this, indexes, level). 
  If an fltk::List is used, the returned widget may be a temporary data
  structure and may be overwritten by another call to child() in this
  <i>or any other Menu</i>!
*/
Widget* Menu::child(const int* indexes, int level) const {
  return list_->child(this, indexes, level);
}

/*! Returns the given top-level child. Same as child(&index,0). 

  <i>This Overrides the method of the same name on fltk::Group</i>. This is
  so that an fltk::List can be used. However if no fltk::List is
  specified the action is identical to fltk::Group::child(index).
*/
Widget* Menu::child(int n) const {
  return list_->child(this, &n, 0);
}

////////////////////////////////////////////////////////////////

/*! \fn Widget* Menu::item() const

  The "current item". In callbacks this is the item the user clicked
  on. Otherwise you probably can't make any assumptions about it's
  value.

  The Browser sets this to the widget when you call goto_index().

  Since this may be the result of calling child() the returned
  structure may be short-lived if an fltk::List is used.
*/

/*! \fn Widget* Menu::item(Widget* v)
  You can set the item with the second call, useful for outwitting the
  callback. This does not change which item is selected, you should
  use focus() or value() for that.
*/

FL_API bool fl_dont_execute; // hack for fluid

/*!
  Calls do_callback(). First it sets item() to the given widget, so
  the callback code can see it.

  Notice that this calls the callback on the Menu widget itself, not
  on the menu item. However the default callback for Menu widget does
  item()->do_callback() so by default the callback for each menu item
  is done.
*/
void Menu::execute(Widget* widget) {
  item(widget);
  if (fl_dont_execute) return;
  if (widget) do_callback();
}

/*! The default callback for Menu calls item()->do_callback() if
  item() is not null. However if item()->user_data() is null, the
  callback is called with the user_data() from the Menu widget
  instead.
*/
void Menu::default_callback(Widget* widget, void*) {
  Widget* item = ((Menu*)widget)->item();
  if (item) item->do_callback(item,
	   item->user_data() ? item->user_data() : widget->user_data());
}

/*! Does nothing. This avoids wasting time measuring all the menu items. */
void Menu::layout() {}

////////////////////////////////////////////////////////////////

/*!
  The current item is remembered in the focus() of each parent at
  each level.  This is used by popup menus to pop up at the same
  item next time.

  Storing it this way allows widgets to be inserted and deleted and
  the currently selected item does not change (because Group updates
  the focus index). But if an List is used and it does not return
  a different Group for each level in the hierarchy, the focus
  indexes will write over each other. Browser currently uses it's
  own code (with the insert/delete broken) to get around this.

  item() is set to the located widget.
  True is returned if the indexes differ from last time.
*/
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

/*!
  Return the widget identified by the focus fields (or NULL if it is
  illegal). This also sets item() to the same value.
*/
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

/*! \fn void Menu::value(int v)

  For a non-hierarchial menu this does the same thing as focus(), it
  moves the selected item or keyboard focus to item n, starting at
  zero. Asking for the value() returns the index of the current item.

  Don't call this for hierarchial menus, it will screw up.
*/

/*! \fn int Menu::size() const
  Returns children() (for back compatability with older versions of fltk).
*/

////////////////////////////////////////////////////////////////

/*! Recursive innards of handle_shortcut. The recursive part only works
  for real children of a child Group, the hierarchy in a List is ignored.
  This is because generating every item in a list could take a very
  long time, possibly forever. */
static Widget* shortcut_search(Group* g) {
  for (int i = 0; i < g->children(); i++) {
    Widget* item = g->child(i);
    if (!item->active()) continue;
    if (fltk::test_shortcut(item->shortcut())) {
      g->focus(i);
      return item;
    }
    if (item->is_group()) {
      item = shortcut_search((Group*)item);
      if (item) {
	g->focus(i);
	return item;
      }
    }
  }
  return 0;
}

/*!
  Respond to the current fltk::SHORTCUT or fltk::KEY event by finding
  a menu item it matches and calling execute() on it. True is returned
  if a menu item is found, false if none. Items are searched for a
  matching shortcut() value. "&x" shortcuts are ignored, they are
  used only for navigating when the menu is visible.

  If you use a List, only the top-most level items are searched for
  shortcuts. Recursion is done only if the children are Group widgets,
  and then only the actual children of that Group (not any List it may
  contain if it is another Menu) are searched. This is necessary
  because some Lists are infinite in size, and usually they don't have
  shortcuts anyway.

  This will return invisible (but active) items, even though it is
  impossible to select these items with the gui. This is done so that
  more than one shortcut for an action may be given by putting
  multiple copies of the item in, where only the first is visible.
*/
int Menu::handle_shortcut() {
  if (event_clicks()) return 0; // ignore repeating keys
  int children = this->children();
  for (int i = 0; i < children; i++) {
    Widget* item = child(i);
    if (!item->active()) continue;
    if (fltk::test_shortcut(item->shortcut())) {
      value(i);
      execute(item);
      return 1;
    }
    if (item->is_group()) {
      item = shortcut_search((Group*)item);
      if (item) {
	value(i);
	execute(item);
	return 1;
      }
    }
  }
  return 0;
}

//
// End of "$Id: Fl_Menu_.cxx,v 1.55 2004/01/25 08:49:51 spitzak Exp $"
//
