//
// "$Id$"
//

#ifndef FL_TREE_ITEM_H
#define FL_TREE_ITEM_H

#include <fltk3/TreeItem.h>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

#include <FL/Fl_Tree_Item_Array.H>
#include <FL/Fl_Tree_Prefs.H>

//////////////////////
// FL/Fl_Tree_Item.H
//////////////////////
//
// Fl_Tree -- This file is part of the Fl_Tree widget for FLTK
// Copyright (C) 2009-2010 by Greg Ercolano.
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


class FL_EXPORT Fl_Tree_Item : public fltk3::TreeItem {

protected:

  //void show_widgets();
  //void hide_widgets();
  //void draw_vertical_connector(int x, int y1, int y2, const Fl_Tree_Prefs &prefs);
  //void draw_horizontal_connector(int x1, int x2, int y, const Fl_Tree_Prefs &prefs);
  
public:
  
  Fl_Tree_Item(const Fl_Tree_Prefs &prefs) 
  : fltk3::TreeItem(prefs)
  {
  }
  
#if 0
  
  ~Fl_Tree_Item();				// DTOR
  Fl_Tree_Item(const Fl_Tree_Item *o);		// COPY CTOR
  int x() const { return(_xywh[0]); }
  int y() const { return(_xywh[1]); }
  int w() const { return(_xywh[2]); }
  int h() const { return(_xywh[3]); }
  void draw(int X, int &Y, int W, Fl_Widget *tree, Fl_Tree_Item *itemfocus, const Fl_Tree_Prefs &prefs, int lastchild=1);
  void show_self(const char *indent = "") const;
  void label(const char *val);
  const char *label() const;

  /// Set a user-data value for the item.
  inline void user_data( void* data ) { _userdata = data; }

  /// Retrieve the user-data value that has been assigned to the item.
  inline void* user_data() const { return _userdata; }
  
  /// Set item's label font face.
  void labelfont(Fl_Font val) {
    _labelfont = val; 
  }
  /// Get item's label font face.
  Fl_Font labelfont() const {
    return(_labelfont);
  }
  /// Set item's label font size.
  void labelsize(Fl_Fontsize val) {
    _labelsize = val; 
  }
  /// Get item's label font size.
  Fl_Fontsize labelsize() const {
    return(_labelsize);
  }
  /// Set item's label foreground text color.
  void labelfgcolor(Fl_Color val) {
    _labelfgcolor = val; 
  }
  /// Set item's label text color.
  void labelcolor(Fl_Color val) {
    _labelfgcolor = val; 
  }
  /// Return item's label text color.
  Fl_Color labelcolor() const {
    return(_labelfgcolor); 
  }
  /// Return item's label foreground text color.
  Fl_Color labelfgcolor() const {
    return(_labelfgcolor); 
  }
  /// Set item's label background color.
  void labelbgcolor(Fl_Color val) {
    _labelbgcolor = val; 
  }
  /// Return item's background text color.
  Fl_Color labelbgcolor() const {
    return(_labelbgcolor); 
  }
#endif

  void widget(Fl_Widget *val) {
    fltk3::TreeItem::widget( fltk3::_1to3_widget(val) );
  }

  Fl_Widget *widget() const {
    return fltk3::_3to1_widget( fltk3::TreeItem::widget() );
  }

#if 0
  /// Return the number of children this item has.
  int children() const {
    return(_children.total()); 
  }
  /// Return the child item for the given 'index'.
  Fl_Tree_Item *child(int index) {
    return(_children[index]); 
  }
  /// Return the const child item for the given 'index'.
  const Fl_Tree_Item *child(int t) const;
  /// See if this item has children.
  int has_children() const {
    return(children()); 
  }
  int find_child(const char *name);
  int find_child(Fl_Tree_Item *item);
  int remove_child(Fl_Tree_Item *item);
  int remove_child(const char *new_label);
  void clear_children();
  void swap_children(int ax, int bx);
  int swap_children(Fl_Tree_Item *a, Fl_Tree_Item *b);
  const Fl_Tree_Item *find_child_item(char **arr) const;	// const
        Fl_Tree_Item *find_child_item(char **arr);		// non-const
  const Fl_Tree_Item *find_item(char **arr) const;		// const
        Fl_Tree_Item *find_item(char **arr);			// non-const
  //////////////////
  // Adding items
  //////////////////
  Fl_Tree_Item *add(const Fl_Tree_Prefs &prefs, const char *new_label);
  Fl_Tree_Item *add(const Fl_Tree_Prefs &prefs, char **arr);
  Fl_Tree_Item *insert(const Fl_Tree_Prefs &prefs, const char *new_label, int pos=0);
  Fl_Tree_Item *insert_above(const Fl_Tree_Prefs &prefs, const char *new_label);
  int depth() const;
#endif 
  Fl_Tree_Item *prev() {
    return (Fl_Tree_Item*)fltk3::TreeItem::prev();
  }

  Fl_Tree_Item *next() {
    return (Fl_Tree_Item*)fltk3::TreeItem::next();
  }

  Fl_Tree_Item *next_sibling() {
    return (Fl_Tree_Item*)fltk3::TreeItem::next_sibling();
  }

  Fl_Tree_Item *prev_sibling() {
    return (Fl_Tree_Item*)fltk3::TreeItem::prev_sibling();
  }

#if 0
  Fl_Tree_Item *next_displayed(Fl_Tree_Prefs &prefs);
  Fl_Tree_Item *prev_displayed(Fl_Tree_Prefs &prefs);
#endif  

  Fl_Tree_Item *parent() {
    return (Fl_Tree_Item*)fltk3::TreeItem::parent();
  }

  const Fl_Tree_Item *parent() const {
    return (Fl_Tree_Item*)fltk3::TreeItem::parent();
  }

#if 0
  /// Set the parent for this item.
  /// Should only be used by Fl_Tree's internals.
  ///
  void parent(Fl_Tree_Item *val) {
    _parent = val;
  }
  //////////////////
  // State
  //////////////////
  void open();
  void close();
  /// See if the item is 'open'.
  int is_open() const {
    return(_open?1:0);
  }
  /// See if the item is 'closed'.
  int is_close() const {
    return(_open?0:1);
  }
  /// Toggle the item's open/closed state.
  void open_toggle() {
    _open?close():open();
  }
  /// Change the item's selection state to the optionally specified 'val'.
  /// If 'val' is not specified, the item will be selected.
  ///
  void select(int val=1) {
    _selected = val;
  }
  /// Toggle the item's selection state.
  void select_toggle() {
    if ( is_selected() ) {
      deselect();		// deselect if selected
    } else {
      select();		// select if deselected
    }
  }
  /// Select self and all children
  ///     Returns count of how many items were in the 'deselected' state,
  ///     ie. how many items were "changed".
  ///
  int select_all() {
    int count = 0;
    if ( ! is_selected() ) {
      select();
      ++count;
    }
    for ( int t=0; t<children(); t++ ) {
      count += child(t)->select_all();
    }
    return(count);
  }
  /// Disable the item's selection state.
  void deselect() {
    _selected = 0;
  }
  /// Deselect self and all children
  ///     Returns count of how many items were in the 'selected' state,
  ///     ie. how many items were "changed".
  ///
  int deselect_all() {
    int count = 0;
    if ( is_selected() ) {
      deselect();
      ++count;
    }
    for ( int t=0; t<children(); t++ ) {
      count += child(t)->deselect_all();
    }
    return(count);
  }
  /// See if the item is selected.
  char is_selected() const {
    return(_selected);
  }
  /// Change the item's activation state to the optionally specified 'val'.
  ///
  /// When deactivated, the item will be 'grayed out'; the callback() 
  /// won't be invoked if the user clicks on the label. If the item
  /// has a widget() associated with the item, its activation state
  /// will be changed as well.
  ///
  /// If 'val' is not specified, the item will be activated.
  ///
  void activate(int val=1) {
    _active = val;
    if ( _widget && val != (int)_widget->active() ) {
      if ( val ) {
	_widget->activate();
      } else {
	_widget->deactivate();
      }
      _widget->redraw();
    }
  }
  /// Deactivate the item; the callback() won't be invoked when clicked.
  /// Same as activate(0)
  ///
  void deactivate() {
    activate(0);
  }
  /// See if the item is activated.
  char is_activated() const {
    return(_active);
  }
  /// See if the item is activated.
  char is_active() const {
    return(_active);
  }
  /// See if the item is visible.
  int visible() const {
    return(_visible ? 1 : 0);
  }
  int visible_r() const;
#endif

  void usericon(Fl_Image *val) {
    fltk3::TreeItem::usericon(fltk3::_1to3_image(val));
  }

  Fl_Image *usericon() const {
    return fltk3::_3to1_image( fltk3::TreeItem::usericon() );
  }

#if 0
  //////////////////
  // Events
  //////////////////
  const Fl_Tree_Item *find_clicked(const Fl_Tree_Prefs &prefs) const;
  Fl_Tree_Item *find_clicked(const Fl_Tree_Prefs &prefs);
  int event_on_collapse_icon(const Fl_Tree_Prefs &prefs) const;
  int event_on_label(const Fl_Tree_Prefs &prefs) const;
  /// Is this item the root of the tree?
  int is_root() const {
    return(_parent==0?1:0);
  }
#endif
  
};

#endif /*FL_TREE_ITEM_H*/

//
// End of "$Id$".
//
