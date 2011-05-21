//
// "$Id: TreeItem.h 8340 2011-01-30 20:22:06Z greg.ercolano $"
//

#ifndef FLtk3_TREE_ITEM_H
#define FLtk3_TREE_ITEM_H

#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/Image.h>
#include <fltk3/draw.h>

#include <fltk3/TreeItemArray.h>
#include <fltk3/TreePrefs.h>

namespace fltk3 {
  
  //////////////////////
  // fltk3/TreeItem.h
  //////////////////////
  //
  // fltk3::Tree -- This file is part of the fltk3::Tree widget for FLTK
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
  
  ///
  /// \file
  /// \brief This file contains the definitions for fltk3::TreeItem
  ///
  
  /// \brief Tree item
  ///
  /// This class is a single tree item, and manages all of the item's attributes.
  /// fltk3::TreeItem is used by fltk3::Tree, which is comprised of many instances of fltk3::TreeItem.
  ///
  /// fltk3::TreeItem is hierarchical; it dynamically manages an fltk3::TreeItemArray of children
  /// that are themselves instances of fltk3::TreeItem. Each item can have zero or more children.
  /// When an item has children, close() and open() can be used to hide or show them.
  ///
  /// Items have their own attributes; font size, face, color.
  /// Items maintain their own hierarchy of children.
  ///
  /// When you make changes to items, you'll need to tell the tree to redraw()
  /// for the changes to show up.
  ///
  class FLTK3_EXPORT TreeItem {
    const char             *_label;		// label (memory managed)
    fltk3::Font                 _labelfont;		// label's font face
    fltk3::Fontsize             _labelsize;		// label's font size
    fltk3::Color                _labelfgcolor;	// label's fg color
    fltk3::Color                _labelbgcolor;	// label's bg color
    char                    _open;		// item is open?
    char                    _visible;		// item is visible?
    char                    _active;		// item activated?
    char                    _selected;		// item selected?
    int                     _xywh[4];		// xywh of this widget (if visible)
    int                     _collapse_xywh[4];	// xywh of collapse icon (if any)
    int                     _label_xywh[4];	// xywh of label
    fltk3::Widget              *_widget;		// item's label widget (optional)
    fltk3::Image               *_usericon;		// item's user-specific icon (optional)
    fltk3::TreeItemArray      _children;		// array of child items
    fltk3::TreeItem           *_parent;		// parent item (=0 if root)
    void                   *_userdata;    	// user data that can be associated with an item
  protected:
    void show_widgets();
    void hide_widgets();
    void draw_vertical_connector(int x, int y1, int y2, const fltk3::TreePrefs &prefs);
    void draw_horizontal_connector(int x1, int x2, int y, const fltk3::TreePrefs &prefs);
  public:
    TreeItem(const fltk3::TreePrefs &prefs);	// CTOR
    ~TreeItem();				// DTOR
    TreeItem(const fltk3::TreeItem *o);		// COPY CTOR
    int x() const { return(_xywh[0]); }
    int y() const { return(_xywh[1]); }
    int w() const { return(_xywh[2]); }
    int h() const { return(_xywh[3]); }
    void draw(int X, int &Y, int W, fltk3::Widget *tree, fltk3::TreeItem *itemfocus, const fltk3::TreePrefs &prefs, int lastchild=1);
    void show_self(const char *indent = "") const;
    void label(const char *val);
    const char *label() const;
    
    /// Set a user-data value for the item.
    inline void user_data( void* data ) { _userdata = data; }
    
    /// Retrieve the user-data value that has been assigned to the item.
    inline void* user_data() const { return _userdata; }
    
    /// Set item's label font face.
    void labelfont(fltk3::Font val) {
      _labelfont = val; 
    }
    /// Get item's label font face.
    fltk3::Font labelfont() const {
      return(_labelfont);
    }
    /// Set item's label font size.
    void labelsize(fltk3::Fontsize val) {
      _labelsize = val; 
    }
    /// Get item's label font size.
    fltk3::Fontsize labelsize() const {
      return(_labelsize);
    }
    /// Set item's label foreground text color.
    void labelfgcolor(fltk3::Color val) {
      _labelfgcolor = val; 
    }
    /// Set item's label text color.
    void labelcolor(fltk3::Color val) {
      _labelfgcolor = val; 
    }
    /// Return item's label text color.
    fltk3::Color labelcolor() const {
      return(_labelfgcolor); 
    }
    /// Return item's label foreground text color.
    fltk3::Color labelfgcolor() const {
      return(_labelfgcolor); 
    }
    /// Set item's label background color.
    void labelbgcolor(fltk3::Color val) {
      _labelbgcolor = val; 
    }
    /// Return item's background text color.
    fltk3::Color labelbgcolor() const {
      return(_labelbgcolor); 
    }
    /// Assign an FLTK widget to this item.
    void widget(fltk3::Widget *val) {
      _widget = val; 
    }
    /// Return FLTK widget assigned to this item.
    fltk3::Widget *widget() const {
      return(_widget); 
    }
    /// Return the number of children this item has.
    int children() const {
      return(_children.total()); 
    }
    /// Return the child item for the given 'index'.
    fltk3::TreeItem *child(int index) {
      return(_children[index]); 
    }
    /// Return the const child item for the given 'index'.
    const fltk3::TreeItem *child(int t) const;
    /// See if this item has children.
    int has_children() const {
      return(children()); 
    }
    int find_child(const char *name);
    int find_child(fltk3::TreeItem *item);
    int remove_child(fltk3::TreeItem *item);
    int remove_child(const char *new_label);
    void clear_children();
    void swap_children(int ax, int bx);
    int swap_children(fltk3::TreeItem *a, fltk3::TreeItem *b);
    const fltk3::TreeItem *find_child_item(char **arr) const;	// const
    fltk3::TreeItem *find_child_item(char **arr);		// non-const
    const fltk3::TreeItem *find_item(char **arr) const;		// const
    fltk3::TreeItem *find_item(char **arr);			// non-const
                                                                //////////////////
                                                                // Adding items
                                                                //////////////////
    fltk3::TreeItem *add(const fltk3::TreePrefs &prefs, const char *new_label);
    fltk3::TreeItem *add(const fltk3::TreePrefs &prefs, char **arr);
    fltk3::TreeItem *insert(const fltk3::TreePrefs &prefs, const char *new_label, int pos=0);
    fltk3::TreeItem *insert_above(const fltk3::TreePrefs &prefs, const char *new_label);
    int depth() const;
    fltk3::TreeItem *prev();
    fltk3::TreeItem *next();
    fltk3::TreeItem *next_sibling();
    fltk3::TreeItem *prev_sibling();
    fltk3::TreeItem *next_displayed(fltk3::TreePrefs &prefs);
    fltk3::TreeItem *prev_displayed(fltk3::TreePrefs &prefs);
    
    /// Return the parent for this item. Returns NULL if we are the root.
    fltk3::TreeItem *parent() {
      return(_parent);
    }
    /// Return the const parent for this item. Returns NULL if we are the root.
    const fltk3::TreeItem *parent() const {
      return(_parent);
    }
    /// Set the parent for this item.
    /// Should only be used by fltk3::Tree's internals.
    ///
    void parent(fltk3::TreeItem *val) {
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
    
    /// Set the user icon's image. '0' will disable.
    void usericon(fltk3::Image *val) {
      _usericon = val;
    }
    /// Get the user icon. Returns '0' if disabled.
    fltk3::Image *usericon() const {
      return(_usericon);
    }
    //////////////////
    // Events
    //////////////////
    const fltk3::TreeItem *find_clicked(const fltk3::TreePrefs &prefs) const;
    fltk3::TreeItem *find_clicked(const fltk3::TreePrefs &prefs);
    int event_on_collapse_icon(const fltk3::TreePrefs &prefs) const;
    int event_on_label(const fltk3::TreePrefs &prefs) const;
    /// Is this item the root of the tree?
    int is_root() const {
      return(_parent==0?1:0);
    }
  };
  
}

#endif /*fltk3::TREE_ITEM_H*/

//
// End of "$Id: TreeItem.h 8340 2011-01-30 20:22:06Z greg.ercolano $".
//
