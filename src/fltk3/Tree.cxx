//
// "$Id$"
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fltk3/Tree.h>
#include <fltk3/Preferences.h>
#include <fltk3/Wrapper.h>

//////////////////////
// fltk3::Tree.cxx
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

// INTERNAL: scroller callback
static void scroll_cb(fltk3::Widget*,void *data) {
  ((fltk3::Tree*)data)->redraw();
}

// INTERNAL: Parse elements from path into an array of null terminated strings
//    Handles escape characters.
//    Path="/aa/bb"
//    Return: arr[0]="aa", arr[1]="bb", arr[2]=0
//    Caller must call free_path(arr).
//
static char **parse_path(const char *path) {
  while ( *path == '/' ) path++;	// skip leading '/' 
                                        // First pass: identify, null terminate, and count separators
  int seps = 1;				// separator count (1: first item)
  int arrsize = 1;			// array size (1: first item)
  char *save = strdup(path);		// make copy we can modify
  char *sin = save, *sout = save;
  while ( *sin ) {
    if ( *sin == '\\' ) {		// handle escape character
      *sout++ = *++sin;
      if ( *sin ) ++sin;
    } else if ( *sin == '/' ) {		// handle submenu
      *sout++ = 0;
      sin++;
      seps++;
      arrsize++;
    } else {				// all other chars
      *sout++ = *sin++;
    }
  }
  *sout = 0;
  arrsize++;				// (room for terminating NULL) 
                                        // Second pass: create array, save nonblank elements
  char **arr = (char**)malloc(sizeof(char*) * arrsize);
  int t = 0;
  sin = save;
  while ( seps-- > 0 ) {
    if ( *sin ) { arr[t++] = sin; }	// skips empty fields, e.g. '//'
    sin += (strlen(sin) + 1);
  }
  arr[t] = 0;
  return(arr);
}

// INTERNAL: Free the array returned by parse_path()
static void free_path(char **arr) {
  if ( arr ) {
    if ( arr[0] ) { free((void*)arr[0]); }
    free((void*)arr);
  }
}

// INTERNAL: Recursively descend tree hierarchy, accumulating total child count
static int find_total_children(fltk3::TreeItem *item, int count=0) {
  count++;
  for ( int t=0; t<item->children(); t++ ) {
    count = find_total_children(item->child(t), count);
  }
  return(count);
}

/// Constructor.
fltk3::Tree::Tree(int X, int Y, int W, int H, const char *L) : fltk3::Group(X,Y,W,H,L) { 
  _root = new fltk3::TreeItem(_prefs);
  _root->parent(0);				// we are root of tree
  _root->label("ROOT");
  _item_focus      = 0;
  _callback_item   = 0;
  _callback_reason = fltk3::TREE_REASON_NONE;
  _scrollbar_size  = 0;				// 0: uses fltk3::scrollbar_size()
  _itemReselectMode = TREE_SELECTABLE_ONCE;

  box(fltk3::DOWN_BOX);
  color(fltk3::BACKGROUND2_COLOR, fltk3::SELECTION_COLOR);
  when(fltk3::WHEN_CHANGED);
  _vscroll = new fltk3::Scrollbar(0,0,0,0);		// will be resized by draw()
  _vscroll->hide();
  _vscroll->type(fltk3::VERTICAL);
  _vscroll->step(1);
  _vscroll->callback(scroll_cb, (void*)this);
  end();
}

/// Destructor.
fltk3::Tree::~Tree() {
  if ( _root ) { delete _root; _root = 0; }
}

/// Adds a new item, given a 'menu style' path, eg: "/Parent/Child/item".
/// Any parent nodes that don't already exist are created automatically.
/// Adds the item based on the value of sortorder().
///
/// To specify items or submenus that contain slashes ('/' or '\')
/// use an escape character to protect them, e.g.
///
/// \code
///     tree->add("/Holidays/Photos/12\\/25\\2010");          // Adds item "12/25/2010"
///     tree->add("/Pathnames/c:\\\\Program Files\\\\MyApp"); // Adds item "c:\Program Files\MyApp"
/// \endcode
///
/// \returns the child item created, or 0 on error.
///
fltk3::TreeItem* fltk3::Tree::add(const char *path) {
  if ( ! _root ) {					// Create root if none
    _root = new fltk3::TreeItem(_prefs);
    _root->parent(0);
    _root->label("ROOT");
  }
  char **arr = parse_path(path);
  fltk3::TreeItem *item = _root->add(_prefs, arr);
  free_path(arr);
  return(item);
}

/// Inserts a new item above the specified fltk3::TreeItem, with the label set to 'name'.
/// \param[in] above -- the item above which to insert the new item. Must not be NULL.
/// \param[in] name -- the name of the new item
/// \returns the item that was added, or 0 if 'above' could not be found.
/// 
fltk3::TreeItem* fltk3::Tree::insert_above(fltk3::TreeItem *above, const char *name) {
  return(above->insert_above(_prefs, name));
}

/// Insert a new item into a tree-item's children at a specified position.
///
/// \param[in] item The existing item to insert new child into. Must not be NULL.
/// \param[in] name The label for the new item
/// \param[in] pos The position of the new item in the child list
/// \returns the item that was added.
///
fltk3::TreeItem* fltk3::Tree::insert(fltk3::TreeItem *item, const char *name, int pos) {
  return(item->insert(_prefs, name, pos));
}

/// Add a new child to a tree-item.
///
/// \param[in] item The existing item to add new child to. Must not be NULL.
/// \param[in] name The label for the new item
/// \returns the item that was added.
///
fltk3::TreeItem* fltk3::Tree::add(fltk3::TreeItem *item, const char *name) {
  return(item->add(_prefs, name));
}

/// Find the item, given a menu style path, eg: "/Parent/Child/item".
/// There is both a const and non-const version of this method.
/// Const version allows pure const methods to use this method 
/// to do lookups without causing compiler errors.
///
/// To specify items or submenus that contain slashes ('/' or '\')
/// use an escape character to protect them, e.g.
///
/// \code
///     tree->add("/Holidays/Photos/12\\/25\\2010");          // Adds item "12/25/2010"
///     tree->add("/Pathnames/c:\\\\Program Files\\\\MyApp"); // Adds item "c:\Program Files\MyApp"
/// \endcode
///
/// \param[in] path -- the tree item's pathname to be found (e.g. "Flintstones/Fred")
/// \returns the item, or NULL if not found.
///
/// \see item_pathname()
///
fltk3::TreeItem *fltk3::Tree::find_item(const char *path) {
  if ( ! _root ) return(NULL);
  char **arr = parse_path(path);
  fltk3::TreeItem *item = _root->find_item(arr);
  free_path(arr);
  return(item);
}

/// A const version of fltk3::Tree::find_item(const char *path)
const fltk3::TreeItem *fltk3::Tree::find_item(const char *path) const {
  if ( ! _root ) return(NULL);
  char **arr = parse_path(path);
  const fltk3::TreeItem *item = _root->find_item(arr);
  free_path(arr);
  return(item);
}

// Handle safe 'reverse string concatenation'.
//   In the following we build the pathname from right-to-left,
//   since we start at the child and work our way up to the root.
//
#define SAFE_RCAT(c) { \
slen += 1; if ( slen >= pathnamelen ) { pathname[0] = '\0'; return(-2); } \
*s-- = c; \
}

/// Find the pathname for the specified \p item.
/// If \p item is NULL, root() is used.
/// The tree's root will be included in the pathname of showroot() is on.
/// Menu items or submenus that contain slashes ('/' or '\') in their names
/// will be escaped with a backslash. This is symmetrical with the add()
/// function which uses the same escape pattern to set names.
/// \param[in] pathname The string to use to return the pathname
/// \param[in] pathnamelen The maximum length of the string (including NULL). Must not be zero.
/// \param[in] item The item whose pathname is to be returned.
/// \returns
///	-   0 : OK (\p pathname returns the item's pathname)
///	-  -1 : item not found (pathname="")
///	-  -2 : pathname not large enough (pathname="")
/// \see find_item()
///
int fltk3::Tree::item_pathname(char *pathname, int pathnamelen, const fltk3::TreeItem *item) const {
  pathname[0] = '\0';
  item = item ? item : _root;
  if ( !item ) return(-1);
  // Build pathname starting at end
  char *s = (pathname+pathnamelen-1);
  int slen = 0;			// length of string compiled so far (including NULL)
  SAFE_RCAT('\0');
  while ( item ) {
    if ( item->is_root() && showroot() == 0 ) break;		// don't include root in path if showroot() off
                                                                // Find name of current item
    const char *name = item->label() ? item->label() : "???";	// name for this item
    int len = (int)strlen(name);
    // Add name to end of pathname[]
    for ( --len; len>=0; len-- ) {
      SAFE_RCAT(name[len]);					// rcat name of item
      if ( name[len] == '/' || name[len] == '\\' ) {
        SAFE_RCAT('\\');					// escape front or back slashes within name
      }
    }
    SAFE_RCAT('/');						// rcat leading slash
    item = item->parent();					// move up tree (NULL==root)
  }
  if ( *(++s) == '/' ) ++s;				// leave off leading slash from pathname
  if ( s != pathname ) memmove(pathname, s, slen);	// Shift down right-aligned string
  return(0);
}

/// Standard FLTK draw() method, handles draws the tree widget.
void fltk3::Tree::draw() {
  // Let group draw box+label but *NOT* children.
  // We handle drawing children ourselves by calling each item's draw()
  //
  // Handle group's bg
  Group::draw_box();
  Group::draw_label();
  // Handle tree
  if ( ! _root ) return;
  int cx = x() + fltk3::box_dx(box());
  int cy = y() + fltk3::box_dy(box());
  int cw = w() - fltk3::box_dw(box());
  int ch = h() - fltk3::box_dh(box());
  // These values are changed during drawing
  // 'Y' will be the lowest point on the tree
  int X = cx + _prefs.marginleft();
  int Y = cy + _prefs.margintop() - (_vscroll->visible() ? _vscroll->value() : 0);
  int W = cw - _prefs.marginleft();			// - _prefs.marginright();
  int Ysave = Y;
  fltk3::push_clip(cx,cy,cw,ch);
  {
    fltk3::font(_prefs.labelfont(), _prefs.labelsize());
    _root->draw(X, Y, W, this,
                (fltk3::focus()==this)?_item_focus:0,	// show focus item ONLY if fltk3::Tree has focus
		_prefs);
  }
  fltk3::pop_clip();
  
  // Show vertical scrollbar?
  int ydiff = (Y+_prefs.margintop())-Ysave;		// ydiff=size of tree
  int ytoofar = (cy+ch) - Y;				// ytoofar -- scrolled beyond bottom (e.g. stow)
  
  //printf("ydiff=%d ch=%d Ysave=%d ytoofar=%d value=%d\n",
  //int(ydiff),int(ch),int(Ysave),int(ytoofar), int(_vscroll->value()));
  
  if ( ytoofar > 0 ) ydiff += ytoofar;
  if ( Ysave<cy || ydiff > ch || int(_vscroll->value()) > 1 ) {
    _vscroll->visible();
    
    int scrollsize = _scrollbar_size ? _scrollbar_size : fltk3::scrollbar_size();
    int sx = x()+w()-fltk3::box_dx(box())-scrollsize;
    int sy = y()+fltk3::box_dy(box());
    int sw = scrollsize;
    int sh = h()-fltk3::box_dh(box());
    _vscroll->show();
    _vscroll->range(0.0,ydiff-ch);
    _vscroll->resize(sx,sy,sw,sh);
    _vscroll->slider_size(float(ch)/float(ydiff));
  } else {
    _vscroll->Slider::value(0);
    _vscroll->hide();
  }
  fltk3::push_clip(cx,cy,cw,ch);
  Group::draw_children();	// draws any FLTK children set via fltk3::Tree::widget()
  fltk3::pop_clip();
}

/// Returns next visible item above (dir==Fl_Up) or below (dir==Fl_Down) the specified \p item.
/// If \p item is 0, returns first() if \p dir is Fl_Up, or last() if \p dir is fltk3::DownKey.
///
/// \param[in] item The item above/below which we'll find the next visible item
/// \param[in] dir The direction to search. Can be fltk3::UpKey or fltk3::DownKey.
/// \returns The item found, or 0 if there's no visible items above/below the specified \p item.
///
fltk3::TreeItem *fltk3::Tree::next_visible_item(fltk3::TreeItem *item, unsigned int dir) {
  if ( ! item ) {				// no start item?
    item = ( dir == fltk3::UpKey ) ? last() : first();	// start at top or bottom
    if ( ! item ) return(0);
    if ( item->visible_r() ) return(item);	// return first/last visible item
  }
  switch ( dir ) {
    case fltk3::UpKey:   return(item->prev_displayed(_prefs));
    case fltk3::DownKey: return(item->next_displayed(_prefs));
    default:      return(item->next_displayed(_prefs));
  }
}

/// Set the item that currently should have keyboard focus.
/// Handles calling redraw() to update the focus box (if it is visible).
///
/// \param[in] item The item that should take focus. If NULL, none will have focus.
///
void fltk3::Tree::set_item_focus(fltk3::TreeItem *item) {
  if ( _item_focus != item ) {		// changed?
    _item_focus = item;			// update
    if ( visible_focus() ) redraw();	// redraw to update focus box
  }
}

/// Find the item that was clicked.
/// You should use callback_item() instead, which is fast,
/// and is meant to be used within a callback to determine the item clicked.
///
/// This method walks the entire tree looking for the first item that is
/// under the mouse (ie. at fltk3::event_x()/Fl:event_y().
///
/// Use this method /only/ if you've subclassed fltk3::Tree, and are receiving
/// events before fltk3::Tree has been able to process and update callback_item().
/// 
/// \returns the item clicked, or 0 if no item was under the current event.
///
const fltk3::TreeItem* fltk3::Tree::find_clicked() const {
  if ( ! _root ) return(NULL);
  return(_root->find_clicked(_prefs));
}

/// Set the item that was last clicked.
/// Should only be used by subclasses needing to change this value.
/// Normally fltk3::Tree manages this value.
///
/// Deprecated: use callback_item() instead.
///
void fltk3::Tree::item_clicked(fltk3::TreeItem* val) {
  _callback_item = val;
}

/// Returns the first item in the tree.
///
/// Use this to walk the tree in the forward direction, eg:
/// \code
/// for ( fltk3::TreeItem *item = tree->first(); item; item = tree->next(item) ) {
///     printf("Item: %s\n", item->label());
/// }
/// \endcode
///
/// \returns first item in tree, or 0 if none (tree empty).
/// \see first(),next(),last(),prev()
///
fltk3::TreeItem* fltk3::Tree::first() {
  return(_root);					// first item always root
}

/// Return the next item after \p item, or 0 if no more items.
///
/// Use this code to walk the entire tree:
/// \code
/// for ( fltk3::TreeItem *item = tree->first(); item; item = tree->next(item) ) {
///     printf("Item: %s\n", item->label());
/// }
/// \endcode
///
/// \param[in] item The item to use to find the next item. If NULL, returns 0.
/// \returns Next item in tree, or 0 if at last item.
///
/// \see first(),next(),last(),prev()
///
fltk3::TreeItem *fltk3::Tree::next(fltk3::TreeItem *item) {
  if ( ! item ) return(0);
  return(item->next());
}

/// Return the previous item before \p item, or 0 if no more items.
///
/// This can be used to walk the tree in reverse, eg:
///
/// \code
/// for ( fltk3::TreeItem *item = tree->first(); item; item = tree->prev(item) ) {
///     printf("Item: %s\n", item->label());
/// }
/// \endcode
///
/// \param[in] item The item to use to find the previous item. If NULL, returns 0.
/// \returns Previous item in tree, or 0 if at first item.
///
/// \see first(),next(),last(),prev()
///
fltk3::TreeItem *fltk3::Tree::prev(fltk3::TreeItem *item) {
  if ( ! item ) return(0);
  return(item->prev());
}

/// Returns the last item in the tree.
///
/// This can be used to walk the tree in reverse, eg:
///
/// \code
/// for ( fltk3::TreeItem *item = tree->last(); item; item = tree->prev() ) {
///     printf("Item: %s\n", item->label());
/// }
/// \endcode
///
/// \returns last item in the tree, or 0 if none (tree empty).
///
/// \see first(),next(),last(),prev()
///
fltk3::TreeItem* fltk3::Tree::last() {
  if ( ! _root ) return(0);
  fltk3::TreeItem *item = _root;
  while ( item->has_children() ) {
    item = item->child(item->children()-1);
  }
  return(item);
}

/// Returns the first selected item in the tree.
///
/// Use this to walk the tree looking for all the selected items, eg:
///
/// \code
/// for ( fltk3::TreeItem *item = tree->first_selected_item(); item; item = tree->next_selected_item(item) ) {
///     printf("Item: %s\n", item->label());
/// }
/// \endcode
///
/// \returns The next selected item, or 0 if there are no more selected items.
///     
fltk3::TreeItem *fltk3::Tree::first_selected_item() {
  return(next_selected_item(0));
}

/// Returns the next selected item after \p item.
/// If \p item is 0, search starts at the first item (root).
///
/// Use this to walk the tree looking for all the selected items, eg:
/// \code
/// for ( fltk3::TreeItem *item = tree->first_selected_item(); item; item = tree->next_selected_item(item) ) {
///     printf("Item: %s\n", item->label());
/// }
/// \endcode
///
/// \param[in] item The item to use to find the next selected item. If NULL, first() is used.
/// \returns The next selected item, or 0 if there are no more selected items.
///     
fltk3::TreeItem *fltk3::Tree::next_selected_item(fltk3::TreeItem *item) {
  if ( ! item ) {
    if ( ! (item = first()) ) return(0);
    if ( item->is_selected() ) return(item);
  }
  while ( (item = item->next()) )
    if ( item->is_selected() )
      return(item);
  return(0);
}

/// Standard FLTK event handler for this widget.
int fltk3::Tree::handle(int e) {
  int ret = 0;
  // Developer note: fltk3::Browser_::handle() used for reference here..
  // #include <fltk3/names.h>	// for event debugging
  // fprintf(stderr, "DEBUG: %s (%d)\n", fl_eventnames[e], e);
  if (e == fltk3::ENTER || e == fltk3::LEAVE) return(1);
  switch (e) {
    case fltk3::FOCUS: {
      // FLTK tests if we want focus. 
      //     If a nav key was used to give us focus, and we've got no saved
      //     focus widget, determine which item gets focus depending on nav key.
      //
      if ( ! _item_focus ) {					// no focus established yet?
	switch (fltk3::event_key()) {				// determine if focus was navigated..
	  case fltk3::TabKey: {					// received focus via TAB?
	    if ( fltk3::event_state(fltk3::SHIFT) ) {			// SHIFT-TAB similar to fltk3::UpKey
	      set_item_focus(next_visible_item(0, fltk3::UpKey));
	    } else {						// TAB similar to fltk3::DownKey
	      set_item_focus(next_visible_item(0, fltk3::DownKey));
	    }
	    break;
	  }
	  case fltk3::LeftKey:		// received focus via LEFT or UP?
	  case fltk3::UpKey: { 	// XK_ISO_Left_Tab
	    set_item_focus(next_visible_item(0, fltk3::UpKey));
	    break;
	  }
	  case fltk3::RightKey: 	// received focus via RIGHT or DOWN?
	  case fltk3::DownKey:
	  default: {
	    set_item_focus(next_visible_item(0, fltk3::DownKey));
	    break;
	  }
	}
      }
      if ( visible_focus() ) redraw();	// draw focus change
      return(1);
    }
    case fltk3::UNFOCUS: {		// FLTK telling us some other widget took focus.
      if ( visible_focus() ) redraw();	// draw focus change
      return(1);
    }
    case fltk3::KEYBOARD: {		// keyboard shortcut
                                        // Do shortcuts first or scrollbar will get them...
      if (_prefs.selectmode() > fltk3::TREE_SELECT_NONE ) {
	if ( !_item_focus ) {
	  set_item_focus(first());
	}
	if ( _item_focus ) {
	  unsigned int ekey = fltk3::event_key();
	  switch (ekey) {
	    case fltk3::EnterKey:	// ENTER: selects current item only
	    case fltk3::KPEnterKey:
	      if ( when() & ~fltk3::WHEN_ENTER_KEY) {
		select_only(_item_focus);
		show_item(_item_focus);		// STR #2426
		return(1);
	      }
	      break;
	    case ' ':		// toggle selection state
	      switch ( _prefs.selectmode() ) {
		case fltk3::TREE_SELECT_NONE:
		  break;
		case fltk3::TREE_SELECT_SINGLE:
		  if ( ! _item_focus->is_selected() )		// not selected?
		    select_only(_item_focus);			// select only this
		  else
		    deselect_all();				// select nothing
		  break;
		case fltk3::TREE_SELECT_MULTI:
		  select_toggle(_item_focus);
		  break;
	      }
	      break;
	    case fltk3::RightKey:  	// open children (if any)
	    case fltk3::LeftKey: {	// close children (if any)
	      if ( _item_focus ) {
		if ( ekey == fltk3::RightKey && _item_focus->is_close() ) {
		  // Open closed item
		  open(_item_focus);
		  redraw();
		  ret = 1;
		} else if ( ekey == fltk3::LeftKey && _item_focus->is_open() ) {
		  // Close open item
		  close(_item_focus);
		  redraw();	
		  ret = 1;
		}
		return(1);
	      }
	      break;
	    }
	    case fltk3::UpKey:		// next item up
	    case fltk3::DownKey: {	// next item down
	      set_item_focus(next_visible_item(_item_focus, ekey));	// next item up|dn
	      if ( _item_focus ) {					// item in focus?
                                                                        // Autoscroll
		int itemtop = _item_focus->y();
		int itembot = _item_focus->y()+_item_focus->h();
		if ( itemtop < y() ) { show_item_top(_item_focus); }
		if ( itembot > y()+h() ) { show_item_bottom(_item_focus); }
		// Extend selection
		if ( _prefs.selectmode() == fltk3::TREE_SELECT_MULTI &&	// multiselect on?
                    (fltk3::event_state() & fltk3::SHIFT) &&			// shift key?
                    ! _item_focus->is_selected() ) {			// not already selected?
                  select(_item_focus);				// extend selection..
		}
		return(1);
	      }
	      break;
	    }
	  }
	}
      }
      break;
    }
  }
  
  // Let fltk3::Group take a shot at handling the event
  if (Group::handle(e)) {
    return(1);			// handled? don't continue below
  }
  
  // Handle events the child FLTK widgets didn't need
  
  static fltk3::TreeItem *lastselect = 0;
  // fprintf(stderr, "ERCODEBUG: fltk3::Tree::handle(): Event was %s (%d)\n", fl_eventnames[e], e); // DEBUGGING
  if ( ! _root ) return(ret);
  switch ( e ) {
    case fltk3::PUSH: {					// clicked on a tree item?
      if (fltk3::visible_focus() && handle(fltk3::FOCUS)) {
        fltk3::focus(this);
      }
      lastselect = 0;
      fltk3::TreeItem *o = _root->find_clicked(_prefs);
      if ( ! o ) break;
      set_item_focus(o);				// becomes new focus widget
      redraw();
      ret |= 1;						// handled
      if ( fltk3::event_button() == fltk3::LEFT_MOUSE ) {
	if ( o->event_on_collapse_icon(_prefs) ) {	// collapse icon clicked?
	  open_toggle(o);
	} else if ( o->event_on_label(_prefs) && 	// label clicked?
                   (!o->widget() || !fltk3::event_inside(o->widget())) &&		// not inside widget
                   (!_vscroll->visible() || !fltk3::event_inside(_vscroll)) ) {	// not on scroller
	  switch ( _prefs.selectmode() ) {
	    case fltk3::TREE_SELECT_NONE:
	      break;
	    case fltk3::TREE_SELECT_SINGLE:
	      select_only(o);
	      break;
	    case fltk3::TREE_SELECT_MULTI: {
	      if ( fltk3::event_state() & fltk3::SHIFT ) {		// SHIFT+PUSH?
	        select(o);					// add to selection
	      } else if ( fltk3::event_state() & fltk3::CTRL ) {	// CTRL+PUSH?
		select_toggle(o);				// toggle selection state
		lastselect = o;					// save toggled item (prevent oscillation)
	      } else {
		select_only(o);
	      }
	      break;
	    }
	  }
	}
      }
      break;
    }
    case fltk3::DRAG: {
      // do the scrolling first:
      int my = fltk3::event_y();
      if ( my < y() ) {				// above top?
        int p = vposition()-(y()-my);
	if ( p < 0 ) p = 0;
        vposition(p);
      } else if ( my > (y()+h()) ) {		// below bottom?
        int p = vposition()+(my-y()-h());
	if ( p > (int)_vscroll->maximum() ) p = (int)_vscroll->maximum();
        vposition(p);
      }
      if ( fltk3::event_button() != fltk3::LEFT_MOUSE ) break;
      fltk3::TreeItem *o = _root->find_clicked(_prefs);
      if ( ! o ) break;
      set_item_focus(o);			// becomes new focus widget
      redraw();
      ret |= 1;
      // Item's label clicked?
      if ( o->event_on_label(_prefs) && 
          (!o->widget() || !fltk3::event_inside(o->widget())) &&
          (!_vscroll->visible() || !fltk3::event_inside(_vscroll)) ) {
	// Handle selection behavior
	switch ( _prefs.selectmode() ) {
	  case fltk3::TREE_SELECT_NONE: break;	// no selection changes
	  case fltk3::TREE_SELECT_SINGLE:
	    select_only(o);
	    break;
	  case fltk3::TREE_SELECT_MULTI:
	    if ( fltk3::event_state() & fltk3::CTRL &&	// CTRL-DRAG: toggle?
                lastselect != o ) {		// not already toggled from last microdrag?
	      select_toggle(o);			// toggle selection
	      lastselect = o;			// save we toggled it (prevents oscillation)
	    } else {
	      select(o);			// select this
	    }
	    break;
	}
      }
      break;
    }
  }
  return(ret);
}

/// Deselect \p item and all its children.
/// If item is NULL, first() is used.
/// Handles calling redraw() if anything was changed.
/// Invokes the callback depending on the value of optional parameter \p docallback.
///
/// The callback can use callback_item() and callback_reason() respectively to determine 
/// the item changed and the reason the callback was called.
///
/// \param[in] item The item that will be deselected (along with all its children).
///                 If NULL, first() is used.
/// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
///     -   0 - the callback() is not invoked
///     -   1 - the callback() is invoked for each item that changed state,
///             callback_reason() will be fltk3::TREE_REASON_DESELECTED
///
/// \returns count of how many items were actually changed to the deselected state.
///
int fltk3::Tree::deselect_all(fltk3::TreeItem *item, int docallback) {
  item = item ? item : first();			// NULL? use first()
  if ( ! item ) return(0);
  int count = 0;
  // Deselect item
  if ( item->is_selected() )
    if ( deselect(item, docallback) )
      ++count;
  // Deselect its children
  for ( int t=0; t<item->children(); t++ ) {
    count += deselect_all(item->child(t), docallback);	// recurse
  }
  return(count);
}

/// Select \p item and all its children.
/// If item is NULL, first() is used.
/// Handles calling redraw() if anything was changed.
/// Invokes the callback depending on the value of optional parameter \p docallback.
///
/// The callback can use callback_item() and callback_reason() respectively to determine 
/// the item changed and the reason the callback was called.
///
/// \param[in] item The item that will be selected (along with all its children). 
///            If NULL, first() is used.
/// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
///     -   0 - the callback() is not invoked
///     -   1 - the callback() is invoked for each item that changed state,
///             callback_reason() will be fltk3::TREE_REASON_SELECTED
/// \returns count of how many items were actually changed to the selected state.
///
int fltk3::Tree::select_all(fltk3::TreeItem *item, int docallback) {
  item = item ? item : first();			// NULL? use first()
  if ( ! item ) return(0);
  int count = 0;
  // Select item
  if ( !item->is_selected() )
    if ( select(item, docallback) )
      ++count;
  // Select its children
  for ( int t=0; t<item->children(); t++ ) {
    count += select_all(item->child(t), docallback);	// recurse
  }
  return(count);
}

/// Select only the specified \p item, deselecting all others that might be selected.
/// If item is 0, first() is used.
/// Handles calling redraw() if anything was changed.
/// Invokes the callback depending on the value of optional parameter \p docallback.
///
/// The callback can use callback_item() and callback_reason() respectively to determine 
/// the item changed and the reason the callback was called.
///
/// \param[in] selitem The item to be selected. If NULL, first() is used.
/// \param[in] docallback -- A flag that determines if the callback() is invoked or not:
///     -   0 - the callback() is not invoked
///     -   1 - the callback() is invoked for each item that changed state, 
///             callback_reason() will be either fltk3::TREE_REASON_SELECTED or 
///             fltk3::TREE_REASON_DESELECTED
/// \returns the number of items whose selection states were changed, if any.
///
int fltk3::Tree::select_only(fltk3::TreeItem *selitem, int docallback) {
  selitem = selitem ? selitem : first();	// NULL? use first()
  if ( ! selitem ) return(0);
  int changed = 0;
  for ( fltk3::TreeItem *item = first(); item; item = item->next() ) {
    if ( item == selitem ) {
      if ( item->is_selected() &&  item_reselect_mode()!=TREE_SELECTABLE_ALWAYS
) continue;	// don't count if already selected
      select(item, docallback);
      ++changed;
    } else {
      if ( item->is_selected() ) {
        deselect(item, docallback);
        ++changed;
      }
    }
  }
  return(changed);
}

/// Adjust the vertical scroll bar so that \p item is visible
/// \p yoff pixels from the top of the fltk3::Tree widget's display.
///
/// For instance, yoff=0 will position the item at the top.
///
/// If yoff is larger than the vertical scrollbar's limit,
/// the value will be clipped. So if yoff=100, but scrollbar's max
/// is 50, then 50 will be used.
///
/// \param[in] item The item to be shown. If NULL, first() is used.
/// \param[in] yoff The pixel offset from the top for the displayed position.
///
/// \see show_item_top(), show_item_middle(), show_item_bottom()
///
void fltk3::Tree::show_item(fltk3::TreeItem *item, int yoff) {
  item = item ? item : first();
  if (!item) return;
  int newval = item->y() - y() - yoff + (int)_vscroll->value();
  if ( newval < _vscroll->minimum() ) newval = (int)_vscroll->minimum();
  if ( newval > _vscroll->maximum() ) newval = (int)_vscroll->maximum();
  _vscroll->value(newval);
  redraw();
}

/// See if \p item is currently displayed on-screen (visible within the widget).
/// This can be used to detect if the item is scrolled off-screen.
/// Checks to see if the item's vertical position is within the top and bottom
/// edges of the display window. This does NOT take into account the hide()/show()
/// or open()/close() status of the item.
///
/// \param[in] item The item to be checked. If NULL, first() is used.
/// \returns 1 if displayed, 0 if scrolled off screen or no items are in tree.
///
int fltk3::Tree::displayed(fltk3::TreeItem *item) {
  item = item ? item : first();
  if (!item) return(0);
  return( (item->y() >= y()) && (item->y() <= (y()+h()-item->h())) ? 1 : 0);
}

/// Adjust the vertical scroll bar to show \p item at the top
/// of the display IF it is currently off-screen (e.g. show_item_top()).
/// If it is already on-screen, no change is made.
///
/// \param[in] item The item to be shown. If NULL, first() is used.
///
/// \see show_item_top(), show_item_middle(), show_item_bottom()
///
void fltk3::Tree::show_item(fltk3::TreeItem *item) {
  item = item ? item : first();
  if (!item) return;
  if ( displayed(item) ) return;
  show_item_top(item);
}

/// Adjust the vertical scrollbar so that \p item is at the top of the display.
///
/// \param[in] item The item to be shown. If NULL, first() is used.
///
void fltk3::Tree::show_item_top(fltk3::TreeItem *item) {
  item = item ? item : first();
  if (item) show_item(item, 0);
}

/// Adjust the vertical scrollbar so that \p item is in the middle of the display.
///
/// \param[in] item The item to be shown. If NULL, first() is used.
///
void fltk3::Tree::show_item_middle(fltk3::TreeItem *item) {
  item = item ? item : first();
  if (item) show_item(item, (h()/2)-(item->h()/2));
}

/// Adjust the vertical scrollbar so that \p item is at the bottom of the display.
///
/// \param[in] item The item to be shown. If NULL, first() is used.
///
void fltk3::Tree::show_item_bottom(fltk3::TreeItem *item) {
  item = item ? item : first();
  if (item) show_item(item, h()-item->h());
}

/// Returns the vertical scroll position as a pixel offset.
/// The position returned is how many pixels of the tree are scrolled off the top edge
/// of the screen.  Example: A position of '3' indicates the top 3 pixels of 
/// the tree are scrolled off the top edge of the screen.
/// \see vposition(), hposition()
///
int fltk3::Tree::vposition() const {
  return((int)_vscroll->value());
}

///  Sets the vertical scroll offset to position \p pos.
///  The position is how many pixels of the tree are scrolled off the top edge
///  of the screen. Example: A position of '3' scrolls the top three pixels of
///  the tree off the top edge of the screen.
///  \param[in] pos The vertical position (in pixels) to scroll the browser to.
///
void fltk3::Tree::vposition(int pos) {
  if (pos < 0) pos = 0;
  if (pos > _vscroll->maximum()) pos = (int)_vscroll->maximum();
  if (pos == _vscroll->value()) return;
  _vscroll->value(pos);
  redraw();
}

/// Displays \p item, scrolling the tree as necessary.
/// \param[in] item The item to be displayed. If NULL, first() is used.
///
void fltk3::Tree::display(fltk3::TreeItem *item) {
  item = item ? item : first();
  if (item) show_item_middle(item);
}

/**
 * Read a preferences database into the tree widget.
 * A preferences database is a hierarchical collection of data which can be
 * directly loaded into the tree view for inspection.
 * \param[in] prefs the fltk3::Preferences database
 */
void fltk3::Tree::load(fltk3::Preferences &prefs) 
{
  int i, j, n, pn = (int)strlen(prefs.path());
  char *p;
  const char *path = prefs.path();
  if (strcmp(path, ".")==0)
    path += 1; // root path is empty
  else
    path += 2; // child path starts with "./"
  n = prefs.groups();
  for (i=0; i<n; i++) {
    fltk3::Preferences prefsChild(prefs, i);
    add(prefsChild.path()+2); // children always start with "./"
    load(prefsChild);
  }
  n = prefs.entries();
  for (i=0; i<n; i++) {
    // We must remove all fwd slashes in the key and value strings. Replace with backslash.
    char *key = strdup(prefs.entry(i));
    int kn = (int)strlen(key);
    for (j=0; j<kn; j++) {
      if (key[j]=='/') key[j]='\\'; 
    }
    char *val;  prefs.get(key, val, "");
    int vn = (int)strlen(val);
    for (j=0; j<vn; j++) {
      if (val[j]=='/') val[j]='\\'; 
    }
    if (vn<40) {
      size_t sze = pn + strlen(key) + vn;
      p = (char*)malloc(sze+5);
      sprintf(p, "%s/%s = %s", path, key, val);
    } else {
      size_t sze = pn + strlen(key) + 40;
      p = (char*)malloc(sze+5);
      sprintf(p, "%s/%s = %.40s...", path, key, val);
    }
    add(p[0]=='/'?p+1:p);
    free(p);
    free(val);
    free(key);
  }
}

//
// End of "$Id$".
//
