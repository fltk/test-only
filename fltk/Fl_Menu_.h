//
// "$Id: Fl_Menu_.h,v 1.7 2002/10/29 00:37:23 easysw Exp $"
//
// Menu base class header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu__H
#define Fl_Menu__H

#include "Fl_Group.h"
#ifndef FLTK_2
#include "Fl_Menu_Item.h" // for back compatability,should not be necessary,
#endif

class FL_API Fl_Menu_;
/**

   Allows an Fl_Browser or Fl_Choice or other subclass of Fl_Menu_ 
   to display a hierarchy of data that is managed by the application 
   rather than FLTK. This is done by creating a "dummy" widget, typically
   a subclass of Fl_Item, that describes a particular item that the browser
   or menu should display. Only one item is examined at any time and thus 
   the dummy widget can be reused, so there is very little space overhead. 

   This is designed for data formats where finding the Nth child of a parent
   is a very quick operation, ie an array. If your data is a list you can 
   search it, the performance is probably acceptable for small lists with 
   less than a hundred or so items. For a bidirectional list it may be useful
   to cache the last request and do a relative search, as Fl_Browser and 
   Fl_Menu will usually ask for adjoining items. 

   If you wish to make a hierarcial Fl_Browser, you must have space in 
   your data to store the state of the FL_VALUE flag on each parent item,
   and must implement the flags_changed() method. 

   If you wish to use an Fl_Multi_Browser you must also have space in 
   your data to store the state of the FL_SELECTED flag on each item, 
   and and must implement the flags_changed() method. 

*/
class FL_API Fl_List {
public:
  /**

   Return how many children are under a given item. If level is zero, this 
   should return how many items are at the top level. Otherwise indexes is 
   an array of level numbers indicating the index of an item at the top level, 
   the index of the an item that is the child of that, and so on. 
   This should return -1 if the item is not a "parent" item or the index array 
   is illegal. It is not necessary to return the correct value until the 
   parent is "open", which means the FL_VALUE flag was set in it, so if it is
   expensiveto calculate the number you can return 1 for any closed parent. 

   Here is a sample implementation, where Node is a data type that 
   you have defined: 

   \code

   int My_Fl_List::children(const Fl_Menu_*, const int* indexes, int level) {
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
  virtual int children(const Fl_Menu_*, const int* indexes, int level);
  /**
   Return a given child as a widget. draw() and measure() will be called on
   this widget to figure out where to place it and to draw it. Typical 
   implementations create a reusable Fl_Item and fill it in with the correct
   data. This should return NULL if there is anything illegal about the indexes.
   
   Here is a sample implementation, where Node is a data type that
   you have defined: 

   \code

   Fl_Widget* My_Fl_List::child(const Fl_Menu_*, const int* indexes, int level) {
     Node* node = root;
     for (int l = 0; l <= level; l++) {
       if (!node->is_parent()) return 0;
       if (indexes[l] >= node->children_count()) return 0;
       node = node->child(indexes[l]);
     }
     static Fl_Widget* widget;
     if (!widget) {
       Fl_Group::current(0);
       widget = new Fl_Item();
     }
     widget->label(node->text());
     widget->w(0); // cause measure() to be called
     widget->user_data(node);
     if (node->selected) widget->set_flag(FL_SELECTED);
     else widget->clear_flag(FL_SELECTED);
     if (node->is_parent() && node->open) widget->set_flag(FL_VALUE);
     else widget->clear_flag(FL_VALUE);
     return widget;
   }

   \endcode

  */
  virtual Fl_Widget* child(const Fl_Menu_*, const int* indexes, int level);
  /**

   This is called if the browser changes any flags on a widget, so that you 
   can copy the values to permanent storage, and perhaps change other 
   displays of the selection. 
   Currently only the FL_VALUE and FL_SELECTED flags are ever changed. 

   Here is a sample implementation, where Node is a data type that 
   you have defined: 

   \code

   void My_Fl_List::flags_changed(const Fl_Menu_*, Fl_Widget* widget) {
     Node* node = (Node*)(widget->user_data());
     node->open = (widget->flags() & FL_VALUE) !=0;
     node->selected = (widget->flags() & FL_SELECTED) != 0;
   }

   \endcode

  */
  virtual void flags_changed(const Fl_Menu_*, Fl_Widget*);
};

/**

   All widgets that display a (possibly hierarchial) list of similar items are
   subclassed off of Fl_Menu_. This includes scrolling browsers, pop-up and 
   pull-down menus and menubars, and the Windows-style "combo box". 

   This is a subclass of Fl_Group and each item is a child Fl_Widget, usually 
   Fl_Item widgets, or Fl_Item_Group widgets to make a hierarchy. 

   An Fl_Menu_ can take a pointer to an Fl_List, which allows the user program
   to dynamically provide the items as they are needed. This is much easier 
   than trying to maintain an array of Fl_Widgets in parallel with your own 
   data structures. 

   It also provides several convienence functions for creating, rearranging, 
   and deleteing Fl_Item and Fl_Item_Group widgets. 

*/
class FL_API Fl_Menu_ : public Fl_Group {

public:

  /**
   Creates a new Fl_Menu_ widget using the given position, size, 
   and label string. It is created with no child items. 
  */
  Fl_Menu_(int x,int y,int w,int h, const char* l=0);
  /*@{*/
  /**
   Get or set the Fl_List that generates widgets for the menu. 
   If this is set it hides any child widgets (though an Fl_List 
   implementation may retrieve them in order to combine them 
   with it's own widgets). 
  */
  Fl_List* list() const {return list_;}
  void list(Fl_List* l) {list_ = l;}
  /*@}*/

  /**
   Return how many children are under a given item. If level is zero, 
   this returns how many items are at the top level. Otherwise indexes
   is an array of level numbers indicating the index of an item at the 
   top level, the index of the an item that is the child of that, and so on.
   
   This returns -1 if any item is not a "parent" or the index array is 
   illegal. It is not necessary to return the correct value until the 
   parent is "open", which means the FL_OPEN flag was set in it, so if it is
   expensive to calculate the number an Fl_List can return 1 for any 
   closed parent. 
  */
  int children(const int* indexes, int level) const ;
  /**
   Returns the number of children at the top level. Same as children(0,0).
   
   This Overrides the method of the same name on Fl_Group. This is so that 
   an Fl_List can be used. However if no Fl_List is specified the action is 
   identical to Fl_Group::children(). 
  */
  int children() const ;
  /**
   Return a given child from somewhere in the hierarchy. indexes is an array 
   of indexes into each parent and contains level+1 numbers. NULL is returned 
   if any of the passed indexes are out of range or the level goes below a 
   non-parent. 

   If an Fl_List is used, this widget may be a temporary data structure and 
   may be overwritten by another call to child() in this or any other Fl_Menu_. 
  */
  Fl_Widget* child(const int* indexes, int level) const ;
  /**
   Returns the given top-level child. Same as child(&index,0).
   
   This Overrides the method of the same name on Fl_Group. 
   This is so that an Fl_List can be used. However if no Fl_List is specified
   the action is identical to Fl_Group::child(index). 
  */
  Fl_Widget* child(int index) const ;
  /*@{*/
  /**
   The "current item". In callbacks this is the item the user clicked on. 
   Otherwise you probably can't make any assumptions about it's value. 
   The Fl_Browser sets this to the widget when you call goto_index(). 

   Since this may be the result of calling child() the returned structure 
   may be short-lived if an Fl_List is used. 

   You can set the item with the second call, useful for outwitting 
   the callback. 
  */
  Fl_Widget* item() const {return item_;}
  Fl_Widget* item(Fl_Widget* v) {return item_ = v;}
  /*@}*/
  /**
   The current item is remembered in the focus() of each parent at each 
   level. This is used by popup menus to pop up at the same item next time.
   
   Storing it this way allows widgets to be inserted and deleted and the 
   currently selected item does not change (because Fl_Group updates the
   focus index). But if an Fl_List is used and it does not return a different
   Fl_Group for each level in the hierarchy, the focus indexes will write over
   each other. Fl_Browser currently uses it's own code (with the insert/delete
   broken) to get around this. 

   item() is set to the located widget. 
   True is returned if the indexes differ from last time. 
  */
  bool focus(const int* indexes, int level);
  /**
   Return the widget identified by the focus fields 
   (or NULL if it is illegal).This also sets item() to the same value. 
  */
  Fl_Widget* get_focus();

  /*@{*/
  /**
   For a non-hierarchial menu this does the same thing as focus(). 
   The items are numbered from zero. 
  */
  int value() const {return Fl_Group::focus();}
  void value(int v) {Fl_Group::focus(v);}
  /*@}*/
  /**
   Returns children() (for back compatability with older versions of fltk). 
  */
  int  size() const {return children();}

  /**
   Create and display a pop-up menu (or hierarchy of menus) showing the 
   children widgets, and waits until the user picks an item or dismisses 
   the menu. If the user picks an item then execute() is called for it and
   true is returned. False is returned if the user cancels the menu. 

   x,y,w,h describe a rectangle that the current menu item should be centered 
   over, and the menu is widened horizontally to w if it is narrower. 
   The coordinates are measured relative to the widget whose handle() method
   is being executed now. 

   title is a widget (usually an Fl_Item) that is used to make a title atop 
   the menu, in the style of SGI's popup menus. 

   menubar is for internal use by menubars and should be left false. 
  */
  int popup(int x,int y,int w=0,int h=0,Fl_Widget* title=0,bool menubar=false);
  /**
   Respond to the current FL_SHORTCUT or FL_KEY event by finding a menu item 
   it matches and calling execute() on it. True is returned if a menu item is 
   found, false if none. Items are searched for a matching shortcut() or 
   for '&' characters in the label. 
  */
  int handle_shortcut();
  static void default_callback(Fl_Widget*, void*);
  /**
   Standard action when the user picks an item. item() is set to 
   it (so the callback can find it) and the callback of the
   Fl_Menu_ is called. If you don't change the callback, the default 
   version does item()->do_callback() so by default the callback for 
   each menu item is done. 

   This function also handles toggle/radio menu items, but that might 
   change in the future. 
  */
  void execute(Fl_Widget*);
  /**
   Make the shortcuts for this menu work no matter what window has the focus 
   when you type it (as long as Fl::modal() is off). This is done by using 
   Fl::add_handler(). This Fl_Menu_ widget does not have to be visible 
   (ie the window it is in can be hidden, or it does not have to be put
   in a window at all). 

   Currently there can be only one global()menu. Setting a new one will 
   replace the old one. There is no way to remove the global() setting 
   (so don't destroy the widget!)
  */
  void global();

  /**
   Does the same search as add() or replace() for an item with that 
   name (including using '/' for submenus) and returns that item if 
   it exists, or NULL if it does not. 
  */
  Fl_Widget* find(const char* label) const;
  /**
   Same as add() but if an item with that name (and in the same submenu) 
   already exists, that widget is altered to these new values. 
   This is what add() on menus did in fltk 1.0. 
  */
  Fl_Widget* replace(const char*, int scut, Fl_Callback*, void* = 0, int = 0);
  /*@{*/
  /**
   Convenience function for populating your menu. This creates a new Fl_Item 
   with the given fields filled in. The label is copied to the widget so the 
   passed string can be a temporary value. 

   The label text is split at '/' characters to automatically produce submenus. 
   To actually put a '/' into a name you must either quote it with a '\\' 
   before it or make it be the first character in a name. The new item is 
   added to the end of the submenu if it already exists. 
  */
  Fl_Widget* add(const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
  Fl_Widget* add(const char* label, void* user_data);
  /*@}*/
  /**
   The passed string is split at any '|' characters and then 
   add(s,0,0,0,0) is done with each section. For instance 
   menu->add("red|green|blue") will add three items. This is compatable 
   with XForms and many other GL-based programs.
  */
  Fl_Widget* add(const char*);
  
  /**
   Same as add() but the new item is inserted before item n in the top level.
   If this is a new submenu the whole submenu is inserted before that item.
   However if this is an existing submenu the item is added to it and
   it is not moved. 
  */
  Fl_Widget* insert(int n, const char* s, void* data = 0);
  /**
   Does delete find(label), thus destroying the matching item, if it exists.
  */
  void remove(const char* l) { delete find(l); }

  // Undo the overrides of stuff from Fl_Group:
  void add(Fl_Widget& o) {Fl_Group::add(o);}
  void add(Fl_Widget* o) {add(*o);}
  void insert(Fl_Widget& o, int n) {Fl_Group::insert(o, n);}
  void replace(int index, Fl_Widget& o) {Fl_Group::replace(index, o);}
  void replace(Fl_Widget& old, Fl_Widget& o) {Fl_Group::replace(old,o);}
  void remove(int index) {Fl_Group::remove(index);}
  void remove(Fl_Widget& o) {Fl_Group::remove(o);}
  void remove(Fl_Widget* o) {Fl_Group::remove(o);}

#ifndef FLTK_2
  // Commented-out methods cannot be emulated.
//const Fl_Menu_Item* test_shortcut();
//Fl_Menu_Item* menu() const;
  void copy(const Fl_Menu_Item* m, void* data = 0) {clear(); m->add_to(this,data);}
  void menu(const Fl_Menu_Item* m) {copy(m,0);}
  void replace(int n, const char* s) { child(n)->label(s); }
  void replace(const char* l, const char* s) { find(l)->label(s); }
  void shortcut(const char* l, int s) { find(l)->shortcut(s); }
  void shortcut(int s) {Fl_Widget::shortcut(s);}
  int shortcut() const {return Fl_Widget::shortcut();}
  void shortcut(int i, int s) { child(i)->shortcut(s); }
//int index(Fl_Menu_Item* m) const { return m - menu_; }
//int index(const char* label) const;
//void replace(Fl_Menu_Item* m, const char* s) { replace(index(m), s); }
//void remove(Fl_Menu_Item* m) { remove(index(m)); }
//void shortcut(Fl_Menu_Item* m, int s) {shortcut(index(m), s);}
//void mode(int i,int x);
//void mode(Fl_Menu_Item* m, int x) {mode(index(m), x);}
//void mode(const char* l, int x) {mode(index(l), x);}
  unsigned mode(int i) const {return child(i)->flags() >> 8;}
//unsigned mode(Fl_Menu_Item* m) const {return mode(index(m));}
  unsigned mode(const char* l) const {return find(l)->flags() >> 8;}

  // in fltk 1.0 these returned/took an Fl_Menu_Item*:
  Fl_Widget* mvalue() {return item();}
//void value(Fl_Widget* o) {set_item(o);}

  const char *text(int i) const {return i >= 0 ? child(i)->label() : 0;}
  const char *text() const {Fl_Widget* w = item(); return w ? w->label() : 0;}
#endif

private:
  Fl_List* list_;
  Fl_Widget* item_;
};

#endif

//
// End of "$Id: Fl_Menu_.h,v 1.7 2002/10/29 00:37:23 easysw Exp $".
//
