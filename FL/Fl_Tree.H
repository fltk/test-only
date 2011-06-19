//
// "$Id$"
//

#ifndef FL_TREE_H
#define FL_TREE_H

#include <fltk3/Tree.h>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/fl_draw.H>

#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Tree_Prefs.H>

//////////////////////
// FL/Fl_Tree.H
//////////////////////
//
// Fl_Tree -- This file is part of the Fl_Tree widget for FLTK
// Copyright (C) 2009-2011 by Greg Ercolano.
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
// 123 Wrapper started
//  - TODO: 123 - many methods not yet wrapped!
//

enum Fl_Tree_Reason {
  FL_TREE_REASON_NONE=0,
  FL_TREE_REASON_SELECTED,
  FL_TREE_REASON_DESELECTED,
  FL_TREE_REASON_OPENED,
  FL_TREE_REASON_CLOSED
};


class FL_EXPORT Fl_Tree : public Fl_Group {

protected:

  // Fl_Scrollbar *_vscroll;
  
  void item_clicked(Fl_Tree_Item* val) {
    ((fltk3::Tree*)_p)->item_clicked( (fltk3::TreeItem*)val );
  }

  //void do_callback_for_item(Fl_Tree_Item* item, Fl_Tree_Reason reason) {

  //Fl_Tree_Item *next_visible_item(Fl_Tree_Item *start, int dir);

public:
  
  Fl_Tree(int X, int Y, int W, int H, const char *L=0) {
    _p = new fltk3::Widget(X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Tree, draw(), draw(), Draw)
  
  FLTK3_WRAPPER_VCALLS_OBJECT_INT(Tree, handle(int event), handle(event), Handle)
  
#if 0
  void root_label(const char *new_label) {
    if ( ! _root ) return;
    _root->label(new_label);
  }
  Fl_Tree_Item* root() {
    return(_root);
  }
#endif
  
  Fl_Tree_Item *add(const char *path) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->add(path);
  }
  
  Fl_Tree_Item* add(Fl_Tree_Item *item, const char *name) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->add(item, name);
  }
  
#if 0
  Fl_Tree_Item *insert_above(Fl_Tree_Item *above, const char *name);
  Fl_Tree_Item* insert(Fl_Tree_Item *item, const char *name, int pos);
  int remove(Fl_Tree_Item *item) {
    if ( item == _root ) {
      clear();
    } else {
      Fl_Tree_Item *parent = item->parent();		// find item's parent
      if ( ! parent ) return(-1);
      parent->remove_child(item);			// remove child + children
    }
    return(0);
  } 
  void clear() {
    if ( ! _root ) return;
    _root->clear_children();
    delete _root; _root = 0;
  } 
  void clear_children(Fl_Tree_Item *item) {
    if ( item->has_children() ) {
      item->clear_children();
      redraw();				// redraw only if there were children to clear
    }
  } 
  Fl_Tree_Item *find_item(const char *path);
  const Fl_Tree_Item *find_item(const char *path) const;
  int item_pathname(char *pathname, int pathnamelen, const Fl_Tree_Item *item) const;
  const Fl_Tree_Item *find_clicked() const;
  Fl_Tree_Item *item_clicked() {
    return(_callback_item);
  }
  Fl_Tree_Item *first();
  Fl_Tree_Item *next(Fl_Tree_Item *item=0);
  Fl_Tree_Item *prev(Fl_Tree_Item *item=0);
  Fl_Tree_Item *last();
  Fl_Tree_Item *first_selected_item();
  Fl_Tree_Item *next_selected_item(Fl_Tree_Item *item=0);
  int open(Fl_Tree_Item *item, int docallback=1) {
    if ( item->is_open() ) return(0);
    item->open();
    redraw();
    if ( docallback ) {
      do_callback_for_item(item, FL_TREE_REASON_OPENED);
    }
    return(1);
  }
  int open(const char *path, int docallback=1) {
    Fl_Tree_Item *item = find_item(path);
    if ( ! item ) return(-1);
    return(open(item, docallback));
  }
  void open_toggle(Fl_Tree_Item *item, int docallback=1) {
    if ( item->is_open() ) {
      close(item, docallback);
    } else {
      open(item, docallback);
    }
  }
  int close(Fl_Tree_Item *item, int docallback=1) {
    if ( item->is_close() ) return(0);
    item->close();
    redraw();
    if ( docallback ) {
      do_callback_for_item(item, FL_TREE_REASON_CLOSED);
    }
    return(1);
  }
  int close(const char *path, int docallback=1) {
    Fl_Tree_Item *item = find_item(path);
    if ( ! item ) return(-1);
    return(close(item, docallback));
  }
  int is_open(Fl_Tree_Item *item) const {
    return(item->is_open()?1:0);
  }
  int is_open(const char *path) const {
    const Fl_Tree_Item *item = find_item(path);
    if ( ! item ) return(-1);
    return(item->is_open()?1:0);
  }
  int is_close(Fl_Tree_Item *item) const {
    return(item->is_close());
  }
  int is_close(const char *path) const {
    const Fl_Tree_Item *item = find_item(path);
    if ( ! item ) return(-1);
    return(item->is_close()?1:0);
  }
  int select(Fl_Tree_Item *item, int docallback=1) {
    if ( ! item->is_selected() ) {
      item->select();
      set_changed();
      if ( docallback ) {
        do_callback_for_item(item, FL_TREE_REASON_SELECTED);
      }
      redraw();
      return(1);
    }
    return(0);
  }
  int select(const char *path, int docallback=1) {
    Fl_Tree_Item *item = find_item(path);
    if ( ! item ) return(-1);
    return(select(item, docallback));
  }
  void select_toggle(Fl_Tree_Item *item, int docallback=1) {
    item->select_toggle();
    set_changed();
    if ( docallback ) {
      do_callback_for_item(item, item->is_selected() ? FL_TREE_REASON_SELECTED
                                                     : FL_TREE_REASON_DESELECTED);
    }
    redraw();
  }
  int deselect(Fl_Tree_Item *item, int docallback=1) {
    if ( item->is_selected() ) {
      item->deselect();
      set_changed();
      if ( docallback ) {
        do_callback_for_item(item, FL_TREE_REASON_DESELECTED);
      }
      redraw();
      return(1);
    }
    return(0);
  }
  int deselect(const char *path, int docallback=1) {
    Fl_Tree_Item *item = find_item(path);
    if ( ! item ) return(-1);
    return(deselect(item, docallback));
  }
  int deselect_all(Fl_Tree_Item *item=0, int docallback=1);
  int select_only(Fl_Tree_Item *selitem, int docallback=1);
  int select_all(Fl_Tree_Item *item=0, int docallback=1);
  void set_item_focus(Fl_Tree_Item *o);
  int is_selected(Fl_Tree_Item *item) const {
    return(item->is_selected()?1:0);
  }
  int is_selected(const char *path) {
    Fl_Tree_Item *item = find_item(path);
    if ( ! item ) return(-1);
    return(is_selected(item));
  }
  void show_self() {
    if ( ! _root ) return;
    _root->show_self();
  }
  Fl_Fontsize item_labelsize() const {
    return(_prefs.labelsize());
  }
  void item_labelsize(Fl_Fontsize val) {
    _prefs.labelsize(val);
  }
  Fl_Font item_labelfont() const {
    return(_prefs.labelfont());
  }
  void item_labelfont(Fl_Font val) {
    _prefs.labelfont(val);
  }
  Fl_Color item_labelfgcolor(void) const {
    return(_prefs.labelfgcolor());
  }
  void item_labelfgcolor(Fl_Color val) {
    _prefs.labelfgcolor(val);
  }
  Fl_Color item_labelbgcolor(void) const {
    return(_prefs.labelbgcolor());
  }
  void item_labelbgcolor(Fl_Color val) {
    _prefs.labelbgcolor(val);
  }
  Fl_Color connectorcolor() const {
    return(_prefs.connectorcolor());
  }
  void connectorcolor(Fl_Color val) {
    _prefs.connectorcolor(val);
  }
  int marginleft() const {
    return(_prefs.marginleft());
  }
  void marginleft(int val) {
    _prefs.marginleft(val);
    redraw();
  }
  int margintop() const {
    return(_prefs.margintop());
  }
  void margintop(int val) {
    _prefs.margintop(val);
    redraw();
  }
  int openchild_marginbottom() const {
    return(_prefs.openchild_marginbottom());
  }
  void openchild_marginbottom(int val) {
    _prefs.openchild_marginbottom(val);
    redraw();
  }
  int connectorwidth() const {
    return(_prefs.connectorwidth());
  }
  void connectorwidth(int val) {
    _prefs.connectorwidth(val);
    redraw();
  }
  Fl_Image *usericon() const {
    return(_prefs.usericon());
  }
  void usericon(Fl_Image *val) {
    _prefs.usericon(val);
    redraw();
  }
  Fl_Image *openicon() const {
    return(_prefs.openicon());
  }
  void openicon(Fl_Image *val) {
    _prefs.openicon(val);
    redraw();
  }
  Fl_Image *closeicon() const {
    return(_prefs.closeicon());
  }
  void closeicon(Fl_Image *val) {
    _prefs.closeicon(val);
    redraw();
  }
  int showcollapse() const {
    return(_prefs.showcollapse());
  }
  void showcollapse(int val) {
    _prefs.showcollapse(val);
    redraw();
  }
  int showroot() const {
    return(_prefs.showroot());
  }
  void showroot(int val) {
    _prefs.showroot(val);
    redraw();
  }
  Fl_Tree_Connector connectorstyle() const {
    return(_prefs.connectorstyle());
  }
  void connectorstyle(Fl_Tree_Connector val) {
    _prefs.connectorstyle(val);
    redraw();
  }
  Fl_Tree_Sort sortorder() const {
    return(_prefs.sortorder());
  }
  void sortorder(Fl_Tree_Sort val) {
    _prefs.sortorder(val);
    // no redraw().. only affects new add()itions
  }
  Fl_Boxtype selectbox() const {
    return(_prefs.selectbox());
  }
  void selectbox(Fl_Boxtype val) {
    _prefs.selectbox(val);
    redraw();
  }
  Fl_Tree_Select selectmode() const {
    return(_prefs.selectmode());
  }
  void selectmode(Fl_Tree_Select val) {
    _prefs.selectmode(val);
  }
  int displayed(Fl_Tree_Item *item);
  void show_item(Fl_Tree_Item *item, int yoff);
  void show_item(Fl_Tree_Item *item);
  void show_item_bottom(Fl_Tree_Item *item);
  void show_item_middle(Fl_Tree_Item *item);
  void show_item_top(Fl_Tree_Item *item);
  void display(Fl_Tree_Item *item);
  int  vposition() const;
  void vposition(int ypos);
  int is_scrollbar(Fl_Widget *w) {
      return( ( w == _vscroll ) ? 1 : 0 );
  }
#endif
  
  int scrollbar_size() const {
    return ((fltk3::Tree*)_p)->scrollbar_size();
  }
  
  void scrollbar_size(int size) {
    ((fltk3::Tree*)_p)->scrollbar_size(size);
  }   
  
#if 0
  void callback_item(Fl_Tree_Item* item) {
    _callback_item = item;
  }
  Fl_Tree_Item* callback_item() {
    return(_callback_item);
  }
  void callback_reason(Fl_Tree_Reason reason) {
    _callback_reason = reason;
  }
  Fl_Tree_Reason callback_reason() const {
    return(_callback_reason);
  }
  void load(class Fl_Preferences&);
#endif
  
};

#endif /*FL_TREE_H*/

//
// End of "$Id$".
//
