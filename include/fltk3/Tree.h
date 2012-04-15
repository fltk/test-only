//
// "$Id$"
//

#ifndef FLtk3_TREE_H
#define FLtk3_TREE_H

#include <fltk3/run.h>
#include <fltk3/Group.h>
#include <fltk3/Scrollbar.h>
#include <fltk3/draw.h>

#include <fltk3/TreeItem.h>
#include <fltk3/TreePrefs.h>


namespace fltk3 {
  
  //////////////////////
  // fltk3/Tree.h
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
  /// \brief This file contains the definitions of the fltk3::Tree class
  ///
  
  /// \class fltk3::Tree
  ///
  /// \brief Tree widget.
  ///
  ///     \image html tree-simple.png "fltk3::Tree example program"
  ///     \image latex tree-simple.png "fltk3::Tree example program" width=4cm
  ///
  /// \code
  ///     fltk3::Tree                                         // Top level widget
  ///        |--- fltk3::TreeItem                            // Items in the tree
  ///        |--- fltk3::TreePrefs                           // Preferences for the tree
  ///                  |--- fltk3::TreeConnector (enum)      // Connection modes
  ///                  |--- fltk3::TreeSelect (enum)         // Selection modes
  ///                  |--- fltk3::TreeSort (enum)           // Sort behavior
  /// \endcode
  ///
  ///     Similar to fltk3::Browser, fltk3::Tree is a browser of fltk3::TreeItem's, which is arranged
  ///     in a parented hierarchy, or 'tree'. Subtrees can be expanded or closed. Items can be
  ///     added, deleted, inserted, sorted and re-ordered.
  ///
  ///     The tree items may also contain other FLTK widgets, like buttons, input fields,
  ///     or even "custom" widgets.
  ///
  ///     The callback() is invoked depending on the value of when():
  ///
  ///         - fltk3::WHEN_RELEASE -- callback invoked when left mouse button is released on an item
  ///         - fltk3::WHEN_CHANGED -- callback invoked when left mouse changes selection state
  ///
  ///     The simple way to define a tree:
  /// \code
  ///    #include <fltk3/Tree.h>
  ///    [..]
  ///    fltk3::Tree tree(X,Y,W,H);
  ///    tree.begin();
  ///      tree.add("Flintstones/Fred");
  ///      tree.add("Flintstones/Wilma");
  ///      tree.add("Flintstones/Pebbles");
  ///      tree.add("Simpsons/Homer");
  ///      tree.add("Simpsons/Marge");
  ///      tree.add("Simpsons/Bart");
  ///      tree.add("Simpsons/Lisa");
  ///    tree.end();
  /// \endcode
  ///     
  ///     Items can be added with add(),
  ///     removed with remove(),
  ///     completely cleared with clear(),
  ///     inserted with insert() and insert_above(),
  ///     selected/deselected with select() and deselect(),
  ///     open/closed with open() and closed().
  ///     Children of an item can be swapped around with fltk3::TreeItem::swap_children(),
  ///     sorting can be controlled when items are add()ed via sortorder().
  ///     You can walk the entire tree with first() and next().
  ///     You can walk selected items with first_selected_item() and
  ///     next_selected_item().
  ///     Items can be found by their pathname using find_item(const char*),
  ///     and an item's pathname can be found with item_pathname().
  ///     The selected items' colors are controlled by selection_color() (inherited from fltk3::Widget).
  ///
  ///     The tree can have different selection behaviors controlled by selectmode().
  ///
  ///     FLTK widgets (including custom widgets) can be assigned to tree items via
  ///     fltk3::TreeItem::widget().
  ///
  ///     Icons for individual items can be changed with
  ///     fltk3::TreeItem::openicon(), 
  ///     fltk3::TreeItem::closeicon(),
  ///     fltk3::TreeItem::usericon().
  ///
  ///     Various default preferences can be globally manipulated via fltk3::TreePrefs, 
  ///     including colors, margins, icons, connection lines. 
  ///
  ///     The tree's callback() will be invoked when items change state or are open/closed.
  ///     when() controls when mouse/keyboard events invoke the callback.
  ///     callback_item() and callback_reason() can be used to determine the cause of the callback. eg:
  ///
  /// \code
  /// void MyTreeCallback(fltk3::Widget *w, void *data) {
  ///   fltk3::Tree      *tree = (fltk3::Tree*)w;
  ///   fltk3::TreeItem *item = (fltk3::TreeItem*)tree->callback_item();	// get selected item
  ///   switch ( tree->callback_reason() ) {
  ///     case fltk3::TREE_REASON_SELECTED: [..]
  ///     case fltk3::TREE_REASON_DESELECTED: [..]
  ///     case fltk3::TREE_REASON_RESELECTED: [..]
  ///     case fltk3::TREE_REASON_OPENED: [..]
  ///     case fltk3::TREE_REASON_CLOSED: [..]
  ///   }
  /// \endcode
  ///
  ///     To get the item's full menu pathname, you can use fltk3::TreeItem::item_pathname(), eg:
  ///
  /// \code
  ///   char pathname[256] = "???";
  ///   tree->item_pathname(pathname, sizeof(pathname), item);		// eg. "Parent/Child/Item"
  /// \endcode
  ///
  ///     To walk all the items of the tree from top to bottom:
  /// \code
  /// // Walk all the items in the tree, and print their labels
  /// for ( fltk3::TreeItem *item = tree->first(); item; item = tree->next(item) ) {
  ///     printf("Item: %s\n", item->label());
  /// }
  /// \endcode
  ///
  ///     To recursively walk all the children of a particular item, 
  ///     define a function that uses recursion:
  ///     \code
  /// // Find all of the item's children and print an indented report of their labels
  /// void my_print_all_children(fltk3::TreeItem *item, int indent=0) {
  ///     for ( int t=0; t<item->children(); t++ ) {
  ///         printf("%*s Item: %s\n", indent, "", item->child(t)->label());
  ///         my_print_all_children(item->child(t), indent+4);   // recurse
  ///     }
  /// }
  ///     \endcode
  ///
  ///     To change the default label font and color for creating new items:
  /// \code
  ///  tree = new fltk3::Tree(..);
  ///  tree->item_labelfont(fltk3::COURIER);	// Use Courier font for all new items
  ///  tree->item_labelfgcolor(fltk3::RED);	// Use red color for labels of all new items
  ///  [..]
  ///  // Now create the items in the tree using the above defaults.
  ///  tree->add("Aaa");
  ///  tree->add("Bbb");
  ///  [..]
  /// \endcode
  ///
  ///     To change the font and color of all items in the tree:
  /// \code
  /// // Change the font and color of all items currently in the tree
  /// for ( fltk3::TreeItem *item = tree->first(); item; item = tree->next(item) ) {
  ///     item->labelfont(fltk3::COURIER);
  ///     item->labelcolor(fltk3::RED);
  /// }
  /// \endcode
  ///
  ///     The following image shows the tree's various visual elements
  ///     and the methods that control them:
  ///     
  ///     \image html tree-elements.png
  ///     \image latex tree-elements.png "fltk3::Tree dimensions" width=6cm
  ///
  
  /// \enum fltk3::TreeReason
  /// The reason the callback was invoked.
  ///
  enum TreeReason {
    TREE_REASON_NONE=0,	///< unknown reason
    TREE_REASON_SELECTED,	///< an item was selected
    TREE_REASON_DESELECTED,	///< an item was de-selected
    TREE_REASON_RESELECTED,	///< an item was re-selected
    TREE_REASON_OPENED,	///< an item was opened
    TREE_REASON_CLOSED		///< an item was closed
  };
  
  
  class FLTK3_EXPORT Tree : public fltk3::Group {

    fltk3::TreeItem  *_root;				// can be null!
    fltk3::TreeItem  *_item_focus;			// item that has focus box
    fltk3::TreeItem  *_callback_item;		// item invoked during callback (can be NULL)
    fltk3::TreeReason _callback_reason;		// reason for the callback
    fltk3::TreePrefs  _prefs;			// all the tree's settings
    int            _scrollbar_size;		// size of scrollbar trough
  
  public:
    /// \enum Fl_Tree_Item_Select_Mode
    /// Defines the ways an item can be (re) selected.
    ///
    enum Tree_Item_Reselect_Mode
    {
      TREE_SELECTABLE_ONCE=0, /// backward compatible default: an item can only be selected once
      TREE_SELECTABLE_ALWAYS, /// needed for new RESELECT feature
    };
    
    //! Returns the current item re/selection mode
    Tree_Item_Reselect_Mode item_reselect_mode() const {
      return _itemReselectMode;
    }
  
    //! Sets the item re/selection mode
    void item_reselect_mode(Tree_Item_Reselect_Mode mode) {
      _itemReselectMode = mode;
    }

  private:
    Tree_Item_Reselect_Mode _itemReselectMode;

  protected:
    /// Vertical scrollbar
    fltk3::Scrollbar *_vscroll;
    
  protected:
    void item_clicked(fltk3::TreeItem* val);
    /// Do the callback for the item, setting the item and reason
    void do_callback_for_item(fltk3::TreeItem* item, fltk3::TreeReason reason) {
      callback_reason(reason);
      callback_item(item);
      do_callback((fltk3::Widget*)this, user_data());
    }
    fltk3::TreeItem *next_visible_item(fltk3::TreeItem *start, unsigned int dir);
    
  public:
    Tree(int X, int Y, int W, int H, const char *L=0);
    ~Tree();
    int handle(int e);
    void draw();
    
    ///////////////////////
    // root methods
    ///////////////////////
    
    /// Set the label for the root item. 
    ///
    /// Makes an internally managed copy of 'new_label'.
    ///
    void root_label(const char *new_label) {
      if ( ! _root ) return;
      _root->label(new_label);
    }
    /// Returns the root item.
    fltk3::TreeItem* root() {
      return(_root);
    }
    
    ////////////////////////////////
    // Item creation/removal methods
    ////////////////////////////////
    fltk3::TreeItem *add(const char *path);
    fltk3::TreeItem* add(fltk3::TreeItem *item, const char *name);
    fltk3::TreeItem *insert_above(fltk3::TreeItem *above, const char *name);
    fltk3::TreeItem* insert(fltk3::TreeItem *item, const char *name, int pos);
    
    /// Remove the specified \p item from the tree.
    /// \p item may not be NULL.
    /// If it has children, all those are removed too.
    /// \returns 0 if done, -1 if 'item' not found.
    ///
    int remove(fltk3::TreeItem *item) {
      if ( item == _root ) {
        clear();
      } else {
        fltk3::TreeItem *parent = item->parent();		// find item's parent
        if ( ! parent ) return(-1);
        parent->remove_child(item);			// remove child + children
      }
      return(0);
    } 
    /// Clear all children from the tree.
    /// The tree will be left completely empty.
    ///
    void clear() {
      if ( ! _root ) return;
      _root->clear_children();
      delete _root; _root = 0;
    } 
    /// Clear all the children of a particular node in the tree specified by \p item.
    /// Item may not be NULL.
    ///
    void clear_children(fltk3::TreeItem *item) {
      if ( item->has_children() ) {
        item->clear_children();
        redraw();				// redraw only if there were children to clear
      }
    } 
    
    ////////////////////////
    // Item lookup methods
    ////////////////////////
    fltk3::TreeItem *find_item(const char *path);
    const fltk3::TreeItem *find_item(const char *path) const;
    int item_pathname(char *pathname, int pathnamelen, const fltk3::TreeItem *item) const;
    
    const fltk3::TreeItem *find_clicked() const;
    
    /// Return the item that was last clicked.
    ///
    /// Valid only from within the callback().
    ///
    /// Deprecated: use callback_item() instead.
    ///
    /// \returns the item clicked, or 0 if none.
    ///          0 may also be used to indicate several items were clicked/changed.
    ///
    fltk3::TreeItem *item_clicked() {
      return(_callback_item);
    }
    fltk3::TreeItem *first();
    fltk3::TreeItem *next(fltk3::TreeItem *item=0);
    fltk3::TreeItem *prev(fltk3::TreeItem *item=0);
    fltk3::TreeItem *last();
    fltk3::TreeItem *first_selected_item();
    fltk3::TreeItem *next_selected_item(fltk3::TreeItem *item=0);
    
    //////////////////////////
    // Item open/close methods
    //////////////////////////
    
    /// Open the specified 'item'.
    /// This causes the item's children (if any) to be shown.
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] item -- the item to be opened. Must not be NULL.
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - callback() is not invoked
    ///     -   1 - callback() is invoked if item changed,
    ///             callback_reason() will be fltk3::TREE_REASON_OPENED
    /// \returns
    ///     -   1 -- item was opened
    ///     -   0 -- item was already open, no change
    ///
    /// \see open(), close(), is_open(), is_close(), callback_item(), callback_reason()
    ///
    int open(fltk3::TreeItem *item, int docallback=1) {
      if ( item->is_open() ) return(0);
      item->open();
      redraw();
      if ( docallback ) {
        do_callback_for_item(item, fltk3::TREE_REASON_OPENED);
      }
      return(1);
    }
    /// Opens the item specified by \p path (eg: "Parent/child/item").
    /// This causes the item's children (if any) to be shown.
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// Items or submenus that themselves contain slashes ('/' or '\')
    /// should be escaped, e.g. open("Holidays/12\\/25\//2010").
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] path -- the tree item's pathname (e.g. "Flintstones/Fred")
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - callback() is not invoked
    ///     -   1 - callback() is invoked if item changed,
    ///             callback_reason() will be fltk3::TREE_REASON_OPENED
    /// \returns
    ///     -   1 -- OK: item opened
    ///     -   0 -- OK: item was already open, no change
    ///     -  -1 -- ERROR: item was not found
    ///
    /// \see open(), close(), is_open(), is_close(), callback_item(), callback_reason()
    ///         
    int open(const char *path, int docallback=1) {
      fltk3::TreeItem *item = find_item(path);
      if ( ! item ) return(-1);
      return(open(item, docallback));
    }
    /// Toggle the open state of \p item.
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] item -- the item whose open state is to be toggled. Must not be NULL.
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - callback() is not invoked
    ///     -   1 - callback() is invoked, callback_reason() will be either
    ///             fltk3::TREE_REASON_OPENED or fltk3::TREE_REASON_CLOSED
    ///
    /// \see open(), close(), is_open(), is_close(), callback_item(), callback_reason()
    ///
    void open_toggle(fltk3::TreeItem *item, int docallback=1) {
      if ( item->is_open() ) {
        close(item, docallback);
      } else {
        open(item, docallback);
      }
    }
    /// Closes the specified \p item.
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] item -- the item to be closed. Must not be NULL.
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - callback() is not invoked
    ///     -   1 - callback() is invoked if item changed,
    ///             callback_reason() will be fltk3::TREE_REASON_CLOSED
    /// \returns
    ///     -   1 -- item was closed
    ///     -   0 -- item was already closed, no change
    ///
    /// \see open(), close(), is_open(), is_close(), callback_item(), callback_reason()
    ///
    int close(fltk3::TreeItem *item, int docallback=1) {
      if ( item->is_close() ) return(0);
      item->close();
      redraw();
      if ( docallback ) {
        do_callback_for_item(item, fltk3::TREE_REASON_CLOSED);
      }
      return(1);
    }
    /// Closes the item specified by \p path, eg: "Parent/child/item".
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// Items or submenus that themselves contain slashes ('/' or '\')
    /// should be escaped, e.g. close("Holidays/12\\/25\//2010").
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] path -- the tree item's pathname (e.g. "Flintstones/Fred")
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - callback() is not invoked
    ///     -   1 - callback() is invoked if item changed,
    ///             callback_reason() will be fltk3::TREE_REASON_CLOSED
    /// \returns
    ///     -   1 -- OK: item closed
    ///     -   0 -- OK: item was already closed, no change
    ///     -  -1 -- ERROR: item was not found
    ///
    /// \see open(), close(), is_open(), is_close(), callback_item(), callback_reason()
    ///         
    int close(const char *path, int docallback=1) {
      fltk3::TreeItem *item = find_item(path);
      if ( ! item ) return(-1);
      return(close(item, docallback));
    }
    /// See if \p item is open.
    ///
    /// Items that are 'open' are themselves not necessarily visible;
    /// one of the item's parents might be closed.
    ///
    /// \param[in] item -- the item to be tested. Must not be NULL.
    /// \returns
    ///     -  1 : item is open
    ///     -  0 : item is closed
    ///
    int is_open(fltk3::TreeItem *item) const {
      return(item->is_open()?1:0);
    }
    /// See if item specified by \p path (eg: "Parent/child/item") is open.
    ///
    /// Items or submenus that themselves contain slashes ('/' or '\')
    /// should be escaped, e.g. is_open("Holidays/12\\/25\//2010").
    ///
    /// Items that are 'open' are themselves not necessarily visible;
    /// one of the item's parents might be closed.
    ///
    /// \param[in] path -- the tree item's pathname (e.g. "Flintstones/Fred")
    /// \returns
    ///     -    1 - OK: item is open
    ///     -    0 - OK: item is closed
    ///     -   -1 - ERROR: item was not found
    ///
    int is_open(const char *path) const {
      const fltk3::TreeItem *item = find_item(path);
      if ( ! item ) return(-1);
      return(item->is_open()?1:0);
    }
    /// See if the specified \p item is closed.
    ///
    /// \param[in] item -- the item to be tested. Must not be NULL.
    /// \returns
    ///     -   1 : item is open
    ///     -   0 : item is closed
    ///
    int is_close(fltk3::TreeItem *item) const {
      return(item->is_close());
    }
    /// See if item specified by \p path (eg: "Parent/child/item") is closed.
    ///
    /// Items or submenus that themselves contain slashes ('/' or '\')
    /// should be escaped, e.g. is_close("Holidays/12\\/25\//2010").
    ///
    /// \param[in] path -- the tree item's pathname (e.g. "Flintstones/Fred")
    /// \returns
    ///     -   1 - OK: item is closed
    ///     -   0 - OK: item is open
    ///     -  -1 - ERROR: item was not found
    ///
    int is_close(const char *path) const {
      const fltk3::TreeItem *item = find_item(path);
      if ( ! item ) return(-1);
      return(item->is_close()?1:0);
    }
    
    /// Select the specified \p item. Use 'deselect()' to de-select it.
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] item -- the item to be selected. Must not be NULL.
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - the callback() is not invoked
    ///     -   1 - the callback() is invoked if item changed state,
    ///             callback_reason() will be fltk3::TREE_REASON_SELECTED
    /// \returns
    ///     -   1 - item's state was changed
    ///     -   0 - item was already selected, no change was made
    ///
    int select(fltk3::TreeItem *item, int docallback=1) {
      int alreadySelected = item->is_selected();
  
      if ( !alreadySelected || item_reselect_mode()== TREE_SELECTABLE_ALWAYS ) {
	item->select();
	set_changed();
	if ( docallback ) {
	  do_callback_for_item(item, alreadySelected ? 
			       TREE_REASON_RESELECTED : TREE_REASON_SELECTED);
        }
        redraw();
        return(1);
      }
      return(0);
    }
    /// Select the item specified by \p path (eg: "Parent/child/item").
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// Items or submenus that themselves contain slashes ('/' or '\')
    /// should be escaped, e.g. select("Holidays/12\\/25\//2010").
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] path -- the tree item's pathname (e.g. "Flintstones/Fred")
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - the callback() is not invoked
    ///     -   1 - the callback() is invoked if item changed state,
    ///             callback_reason() will be fltk3::TREE_REASON_SELECTED
    /// \returns
    ///     -   1 : OK: item's state was changed
    ///     -   0 : OK: item was already selected, no change was made
    ///     -  -1 : ERROR: item was not found
    ///
    int select(const char *path, int docallback=1) {
      fltk3::TreeItem *item = find_item(path);
      if ( ! item ) return(-1);
      return(select(item, docallback));
    }
    /// Toggle the select state of the specified \p item.
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] item -- the item to be selected. Must not be NULL.
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - the callback() is not invoked
    ///     -   1 - the callback() is invoked, callback_reason() will be
    ///             either fltk3::TREE_REASON_SELECTED or fltk3::TREE_REASON_DESELECTED
    ///
    void select_toggle(fltk3::TreeItem *item, int docallback=1) {
      item->select_toggle();
      set_changed();
      if ( docallback ) {
        do_callback_for_item(item, item->is_selected() ? fltk3::TREE_REASON_SELECTED
                             : fltk3::TREE_REASON_DESELECTED);
      }
      redraw();
    }
    /// De-select the specified \p item.
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] item -- the item to be selected. Must not be NULL.
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - the callback() is not invoked
    ///     -   1 - the callback() is invoked if item changed state,
    ///             callback_reason() will be fltk3::TREE_REASON_DESELECTED
    /// \returns
    ///     -   0 - item was already deselected, no change was made
    ///     -   1 - item's state was changed
    ///
    int deselect(fltk3::TreeItem *item, int docallback=1) {
      if ( item->is_selected() ) {
        item->deselect();
        set_changed();
        if ( docallback ) {
          do_callback_for_item(item, fltk3::TREE_REASON_DESELECTED);
        }
        redraw();
        return(1);
      }
      return(0);
    }
    /// Deselect an item specified by \p path (eg: "Parent/child/item").
    /// Handles redrawing if anything was actually changed.
    /// Invokes the callback depending on the value of optional parameter \p docallback.
    ///
    /// Items or submenus that themselves contain slashes ('/' or '\')
    /// should be escaped, e.g. deselect("Holidays/12\\/25\//2010").
    ///
    /// The callback can use callback_item() and callback_reason() respectively to determine 
    /// the item changed and the reason the callback was called.
    ///
    /// \param[in] path -- the tree item's pathname (e.g. "Flintstones/Fred")
    /// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
    ///     -   0 - the callback() is not invoked
    ///     -   1 - the callback() is invoked if item changed state,
    ///             callback_reason() will be fltk3::TREE_REASON_DESELECTED
    ///  \returns
    ///     -   1 - OK: item's state was changed
    ///     -   0 - OK: item was already deselected, no change was made
    ///     -  -1 - ERROR: item was not found
    ///
    int deselect(const char *path, int docallback=1) {
      fltk3::TreeItem *item = find_item(path);
      if ( ! item ) return(-1);
      return(deselect(item, docallback));
    }
    
    int deselect_all(fltk3::TreeItem *item=0, int docallback=1);
    int select_only(fltk3::TreeItem *selitem, int docallback=1);
    int select_all(fltk3::TreeItem *item=0, int docallback=1);
    void set_item_focus(fltk3::TreeItem *o);
    
    /// See if the specified \p item is selected.
    ///
    /// \param[in] item -- the item to be tested. Must not be NULL.
    ///
    /// \return
    ///     -   1 : item selected
    ///     -   0 : item deselected
    ///
    int is_selected(fltk3::TreeItem *item) const {
      return(item->is_selected()?1:0);
    }
    /// See if item specified by \p path (eg: "Parent/child/item") is selected.
    ///
    /// Items or submenus that themselves contain slashes ('/' or '\')
    /// should be escaped, e.g. is_selected("Holidays/12\\/25\//2010").
    ///
    /// \param[in] path -- the tree item's pathname (e.g. "Flintstones/Fred")
    /// \returns
    ///     -   1 : item selected
    ///     -   0 : item deselected
    ///     -  -1 : item was not found
    ///
    int is_selected(const char *path) {
      fltk3::TreeItem *item = find_item(path);
      if ( ! item ) return(-1);
      return(is_selected(item));
    }
    /// Print the tree as 'ascii art' to stdout.
    /// Used mainly for debugging.
    ///
    void show_self() {
      if ( ! _root ) return;
      _root->show_self();
    }
    
    /////////////////////////////////
    // Item attribute related methods
    /////////////////////////////////
    
    /// Get the default label fontsize used for creating new items.
    fltk3::Fontsize item_labelsize() const {
      return(_prefs.labelsize());
    }
    /// Set the default label font size used for creating new items.
    /// To change the font size on a per-item basis, use fltk3::TreeItem::labelsize(fltk3::Fontsize)
    ///
    void item_labelsize(fltk3::Fontsize val) {
      _prefs.labelsize(val);
    }
    /// Get the default font face used for creating new items.
    fltk3::Font item_labelfont() const {
      return(_prefs.labelfont());
    }
    /// Set the default font face used for creating new items.
    /// To change the font face on a per-item basis, use fltk3::TreeItem::labelfont(fltk3::Font)
    ///
    void item_labelfont(fltk3::Font val) {
      _prefs.labelfont(val);
    }
    /// Get the default label foreground color used for creating new items.
    fltk3::Color item_labelfgcolor(void) const {
      return(_prefs.labelfgcolor());
    }
    /// Set the default label foreground color used for creating new items.
    /// To change the foreground color on a per-item basis, use fltk3::TreeItem::labelfgcolor(fltk3::Color)
    ///
    void item_labelfgcolor(fltk3::Color val) {
      _prefs.labelfgcolor(val);
    }
    /// Get the default label background color used for creating new items.
    fltk3::Color item_labelbgcolor(void) const {
      return(_prefs.labelbgcolor());
    }
    /// Set the default label background color used for creating new items.
    /// To change the background color on a per-item basis, use fltk3::TreeItem::labelbgcolor(fltk3::Color)
    ///
    void item_labelbgcolor(fltk3::Color val) {
      _prefs.labelbgcolor(val);
    }
    /// Get the connector color used for tree connection lines.
    fltk3::Color connectorcolor() const {
      return(_prefs.connectorcolor());
    }
    /// Set the connector color used for tree connection lines.
    void connectorcolor(fltk3::Color val) {
      _prefs.connectorcolor(val);
    }
    /// Get the amount of white space (in pixels) that should appear
    /// between the widget's left border and the tree's contents.
    ///
    int marginleft() const {
      return(_prefs.marginleft());
    }
    /// Set the amount of white space (in pixels) that should appear
    /// between the widget's left border and the left side of the tree's contents.
    ///
    void marginleft(int val) {
      _prefs.marginleft(val);
      redraw();
    }
    /// Get the amount of white space (in pixels) that should appear
    /// between the widget's top border and the top of the tree's contents.
    ///
    int margintop() const {
      return(_prefs.margintop());
    }
    /// Sets the amount of white space (in pixels) that should appear
    /// between the widget's top border and the top of the tree's contents.
    ///
    void margintop(int val) {
      _prefs.margintop(val);
      redraw();
    }
    /// Get the amount of white space (in pixels) that should appear
    /// below an open child tree's contents.
    ///
    int openchild_marginbottom() const {
      return(_prefs.openchild_marginbottom());
    }
    /// Set the amount of white space (in pixels) that should appear
    /// below an open child tree's contents.
    ///
    void openchild_marginbottom(int val) {
      _prefs.openchild_marginbottom(val);
      redraw();
    }
    /// Gets the width of the horizontal connection lines (in pixels) 
    /// that appear to the left of each tree item's label.
    ///
    int connectorwidth() const {
      return(_prefs.connectorwidth());
    }
    /// Sets the width of the horizontal connection lines (in pixels) 
    /// that appear to the left of each tree item's label.
    ///
    void connectorwidth(int val) {
      _prefs.connectorwidth(val);
      redraw();
    }
    /// Returns the fltk3::Image being used as the default user icon for all
    /// newly created items.
    /// Returns zero if no icon has been set, which is the default.
    ///
    fltk3::Image *usericon() const {
      return(_prefs.usericon());
    }
    /// Sets the fltk3::Image to be used as the default user icon for all
    /// newly created items.
    ///
    /// If you want to specify user icons on a per-item basis,
    /// use fltk3::TreeItem::usericon() instead.
    ///
    /// \param[in] val -- The new image to be used, or
    ///                   zero to disable user icons.
    ///
    void usericon(fltk3::Image *val) {
      _prefs.usericon(val);
      redraw();
    }
    /// Returns the icon to be used as the 'open' icon.
    /// If none was set, the internal default is returned,
    /// a simple '[+]' icon.
    ///
    fltk3::Image *openicon() const {
      return(_prefs.openicon());
    }
    /// Sets the icon to be used as the 'open' icon.
    /// This overrides the built in default '[+]' icon.
    ///
    /// \param[in] val -- The new image, or zero to use the default [+] icon.
    ///
    void openicon(fltk3::Image *val) {
      _prefs.openicon(val);
      redraw();
    }
    /// Returns the icon to be used as the 'close' icon.
    /// If none was set, the internal default is returned,
    /// a simple '[-]' icon.
    ///
    fltk3::Image *closeicon() const {
      return(_prefs.closeicon());
    }
    /// Sets the icon to be used as the 'close' icon.
    /// This overrides the built in default '[-]' icon.
    ///
    /// \param[in] val -- The new image, or zero to use the default [-] icon.
    ///
    void closeicon(fltk3::Image *val) {
      _prefs.closeicon(val);
      redraw();
    }
    /// Returns 1 if the collapse icon is enabled, 0 if not.
    int showcollapse() const {
      return(_prefs.showcollapse());
    }
    /// Set if we should show the collapse icon or not.
    /// If collapse icons are disabled, the user will not be able
    /// to interactively collapse items in the tree, unless the application
    /// provides some other means via open() and close().
    ///
    /// \param[in] val 1: shows collapse icons (default),\n
    ///                0: hides collapse icons.
    ///
    void showcollapse(int val) {
      _prefs.showcollapse(val);
      redraw();
    }
    /// Returns 1 if the root item is to be shown, or 0 if not.
    int showroot() const {
      return(_prefs.showroot());
    }
    /// Set if the root item should be shown or not.
    /// \param[in] val 1 -- show the root item (default)\n
    ///                0 -- hide the root item.
    ///
    void showroot(int val) {
      _prefs.showroot(val);
      redraw();
    }
    /// Returns the line drawing style for inter-connecting items.
    fltk3::TreeConnector connectorstyle() const {
      return(_prefs.connectorstyle());
    }
    /// Sets the line drawing style for inter-connecting items.
    void connectorstyle(fltk3::TreeConnector val) {
      _prefs.connectorstyle(val);
      redraw();
    }
    /// Set the default sort order used when items are added to the tree.
    ///     See fltk3::TreeSort for possible values.
    ///
    fltk3::TreeSort sortorder() const {
      return(_prefs.sortorder());
    }
    /// Gets the sort order used to add items to the tree.
    void sortorder(fltk3::TreeSort val) {
      _prefs.sortorder(val);
      // no redraw().. only affects new add()itions
    }
    /// Sets the style of box used to draw selected items.
    /// This is an fltk fltk3::Boxtype.
    /// The default is influenced by FLTK's current fltk3::scheme()
    ///
    fltk3::Boxtype selectbox() const {
      return(_prefs.selectbox());
    }
    /// Gets the style of box used to draw selected items.
    /// This is an fltk fltk3::Boxtype.
    /// The default is influenced by FLTK's current fltk3::scheme()
    ///
    void selectbox(fltk3::Boxtype val) {
      _prefs.selectbox(val);
      redraw();
    }
    /// Gets the tree's current selection mode.
    fltk3::TreeSelect selectmode() const {
      return(_prefs.selectmode());
    }
    /// Sets the tree's selection mode.
    void selectmode(fltk3::TreeSelect val) {
      _prefs.selectmode(val);
    }
    int displayed(fltk3::TreeItem *item);
    void show_item(fltk3::TreeItem *item, int yoff);
    void show_item(fltk3::TreeItem *item);
    void show_item_bottom(fltk3::TreeItem *item);
    void show_item_middle(fltk3::TreeItem *item);
    void show_item_top(fltk3::TreeItem *item);
    void display(fltk3::TreeItem *item);
    int  vposition() const;
    void vposition(int ypos);
    
    /// See if widget \p w is one of the fltk3::Tree widget's scrollbars.
    /// Use this to skip over the scrollbars when walking the child() array. Example:
    /// \code
    /// for ( int i=0; i<tree->children(); i++ ) {    // walk children
    ///     fltk3::Widget *w= tree->child(i);
    ///     if ( brow->is_scrollbar(w) ) continue;    // skip scrollbars
    ///     ..do work here..
    /// }
    /// \endcode
    /// \param[in] w Widget to test
    /// \returns 1 if \p w is a scrollbar, 0 if not.
    ///
    int is_scrollbar(fltk3::Widget *w) {
      return( ( w == _vscroll ) ? 1 : 0 );
    }
    /// Gets the current size of the scrollbars' troughs, in pixels.
    ///
    /// If this value is zero (default), this widget will use the global
    /// fltk3::scrollbar_size() value as the scrollbar's width.
    /// 
    /// \returns Scrollbar size in pixels, or 0 if the global fltk3::scrollsize() is being used.
    /// \see fltk3::scrollbar_size(int)
    ///
    int scrollbar_size() const {
      return(_scrollbar_size);
    }
    /// Sets the pixel size of the scrollbars' troughs to the \p size, in pixels.
    ///
    /// Normally you should not need this method, and should use the global
    /// fltk3::scrollbar_size(int) instead to manage the size of ALL 
    /// your widgets' scrollbars. This ensures your application 
    /// has a consistent UI, is the default behavior, and is normally
    /// what you want.
    ///
    /// Only use THIS method if you really need to override the global
    /// scrollbar size. The need for this should be rare.
    ///   
    /// Setting \p size to the special value of 0 causes the widget to
    /// track the global fltk3::scrollbar_size(), which is the default.
    ///   
    /// \param[in] size Sets the scrollbar size in pixels.\n
    ///                 If 0 (default), scrollbar size tracks the global fltk3::scrollbar_size()
    /// \see fltk3::scrollbar_size()
    ///
    void scrollbar_size(int size) {
      _scrollbar_size = size;
      int scrollsize = _scrollbar_size ? _scrollbar_size : fltk3::scrollbar_size();
      if ( _vscroll->w() != scrollsize ) {
        _vscroll->resize(x()+w()-scrollsize, h(), scrollsize, _vscroll->h());
      }
    }   
    
    ///////////////////////
    // callback related
    ///////////////////////
    
    /// Sets the item that was changed for this callback.
    ///    Used internally to pass the item that invoked the callback.
    ///
    void callback_item(fltk3::TreeItem* item) {
      _callback_item = item;
    }
    /// Gets the item that caused the callback.
    /// The callback() can use this value to see which item changed.
    ///
    fltk3::TreeItem* callback_item() {
      return(_callback_item);
    }
    /// Sets the reason for this callback.
    ///    Used internally to pass the reason the callback was invoked.
    ///
    void callback_reason(fltk3::TreeReason reason) {
      _callback_reason = reason;
    }
    /// Gets the reason for this callback.
    ///
    /// The callback() can use this value to see why it was called. Example:
    /// \code
    ///     void MyTreeCallback(fltk3::Widget *w, void *userdata) {
    ///         fltk3::Tree *tree = (fltk3::Tree*)w;
    ///         fltk3::TreeItem *item = tree->callback_item();    // the item changed (can be NULL if more than one item was changed!)
    ///         switch ( tree->callback_reason() ) {           // reason callback was invoked
    ///             case     fltk3::TREE_REASON_OPENED: ..item was opened..
    ///             case     fltk3::TREE_REASON_CLOSED: ..item was closed..
    ///             case   fltk3::TREE_REASON_SELECTED: ..item was selected..
    ///             case fltk3::TREE_REASON_DESELECTED: ..item was deselected..
    ///         }
    ///     }
    /// \endcode
    ///
    fltk3::TreeReason callback_reason() const {
      return(_callback_reason);
    }
    
    /// Load FLTK preferences
    void load(class fltk3::Preferences&);
  };
  
}

#endif /*fltk3::TREE_H*/

//
// End of "$Id$".
//
