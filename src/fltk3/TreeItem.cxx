//
// "$Id$"
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fltk3/Widget.h>
#include <fltk3/TreeItem.h>
#include <fltk3/TreePrefs.h>

//////////////////////
// fltk3::TreeItem.cxx
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

// Was the last event inside the specified xywh?
static int event_inside(const int xywh[4]) {
  return(fltk3::event_inside(xywh[0],xywh[1],xywh[2],xywh[3]));
}

/// Constructor.
///     Makes a new instance of fltk3::TreeItem using defaults from 'prefs'.
///
fltk3::TreeItem::TreeItem(const fltk3::TreePrefs &prefs) {
  _label        = 0;
  _labelfont    = prefs.labelfont();
  _labelsize    = prefs.labelsize();
  _labelfgcolor = prefs.labelfgcolor();
  _labelbgcolor = prefs.labelbgcolor();
  _widget       = 0;
  _open         = 1;
  _visible      = 1;
  _active       = 1;
  _selected     = 0;
  _xywh[0]      = 0;
  _xywh[1]      = 0;
  _xywh[2]      = 0;
  _xywh[3]      = 0;
  _collapse_xywh[0] = 0;
  _collapse_xywh[1] = 0;
  _collapse_xywh[2] = 0;
  _collapse_xywh[3] = 0;
  _label_xywh[0]    = 0;
  _label_xywh[1]    = 0;
  _label_xywh[2]    = 0;
  _label_xywh[3]    = 0;
  _usericon         = 0;
  _userdata         = 0;
  _parent           = 0;
}

// DTOR
fltk3::TreeItem::~TreeItem() {
  if ( _label ) { 
    free((void*)_label);
    _label = 0;
  }
  _widget = 0;			// fltk3::Group will handle destruction
  _usericon = 0;		// user handled allocation
  //_children.clear();		// array's destructor handles itself
}

/// Copy constructor.
fltk3::TreeItem::TreeItem(const fltk3::TreeItem *o) {
  _label        = o->label() ? strdup(o->label()) : 0;
  _labelfont    = o->labelfont();
  _labelsize    = o->labelsize();
  _labelfgcolor = o->labelfgcolor();
  _labelbgcolor = o->labelbgcolor();
  _widget       = o->widget();
  _open         = o->_open;
  _visible      = o->_visible;
  _active       = o->_active;
  _selected     = o->_selected;
  _xywh[0]      = o->_xywh[0];
  _xywh[1]      = o->_xywh[1];
  _xywh[2]      = o->_xywh[2];
  _xywh[3]      = o->_xywh[3];
  _collapse_xywh[0] = o->_collapse_xywh[0];
  _collapse_xywh[1] = o->_collapse_xywh[1];
  _collapse_xywh[2] = o->_collapse_xywh[2];
  _collapse_xywh[3] = o->_collapse_xywh[3];
  _label_xywh[0]    = o->_label_xywh[0];
  _label_xywh[1]    = o->_label_xywh[1];
  _label_xywh[2]    = o->_label_xywh[2];
  _label_xywh[3]    = o->_label_xywh[3];
  _usericon         = o->usericon();
  _userdata         = o->user_data();
  _parent           = o->_parent;
}

/// Print the tree as 'ascii art' to stdout.
/// Used mainly for debugging.
///
void fltk3::TreeItem::show_self(const char *indent) const {
  if ( label() ) {
    printf("%s-%s (%d children, this=%p, parent=%p depth=%d)\n",
           indent,label(),children(),(void*)this, (void*)_parent, depth());
  }
  if ( children() ) {
    char *i2 = (char*)malloc(strlen(indent) + 2);
    strcpy(i2, indent);
    strcat(i2, " |");
    for ( int t=0; t<children(); t++ ) {
      child(t)->show_self(i2);
    }
  }
  fflush(stdout);
}

/// Set the label. Makes a copy of the name.
void fltk3::TreeItem::label(const char *name) {
  if ( _label ) { free((void*)_label); _label = 0; }
  _label = name ? strdup(name) : 0;
}

/// Return the label.
const char *fltk3::TreeItem::label() const {
  return(_label);
}

/// Return child item for the specified 'index'.
const fltk3::TreeItem *fltk3::TreeItem::child(int index) const {
  return(_children[index]);
}

/// Clear all the children for this item.
void fltk3::TreeItem::clear_children() {
  _children.clear();
}

/// Return the index of the immediate child of this item that has the label 'name'.
///
/// \returns index of found item, or -1 if not found.
///
int fltk3::TreeItem::find_child(const char *name) {
  if ( name ) {
    for ( int t=0; t<children(); t++ ) {
      if ( child(t)->label() ) {
        if ( strcmp(child(t)->label(), name) == 0 ) {
          return(t);
        }
      }
    }
  }
  return(-1);
}

/// Find child item by descending array of names. Does not include self in search.
/// Only fltk3::Tree should need this method.
///
/// \returns item, or 0 if not found
///
const fltk3::TreeItem *fltk3::TreeItem::find_child_item(char **arr) const {
  for ( int t=0; t<children(); t++ ) {
    if ( child(t)->label() ) {
      if ( strcmp(child(t)->label(), *arr) == 0 ) {	// match?
        if ( *(arr+1) ) {				// more in arr? descend
          return(_children[t]->find_item(arr+1));
        } else {					// end of arr? done
          return(_children[t]);
        }
      }
    }
  }
  return(0);
}

/// Find child item by descending array of names. Does not include self in search.
/// Only fltk3::Tree should need this method. Use fltk3::Tree::find_item() instead.
///
/// \returns item, or 0 if not found
///
fltk3::TreeItem *fltk3::TreeItem::find_child_item(char **arr) {
  for ( int t=0; t<children(); t++ ) {
    if ( child(t)->label() ) {
      if ( strcmp(child(t)->label(), *arr) == 0 ) {	// match?
        if ( *(arr+1) ) {				// more in arr? descend
          return(_children[t]->find_item(arr+1));
        } else {					// end of arr? done
          return(_children[t]);
        }
      }
    }
  }
  return(0);
}

/// Find item by descending array of \p names. Includes self in search.
/// Only fltk3::Tree should need this method. Use fltk3::Tree::find_item() instead.
///
/// \returns item, or 0 if not found
///
const fltk3::TreeItem *fltk3::TreeItem::find_item(char **names) const {
  if ( label() && strcmp(label(), *names) == 0 ) {	// match self?
    if ( *(names+1) == 0 ) {				// end of names,
      return(this);					// found ourself.
    }
  }
  if ( children() ) {					// check children..
    return(find_child_item(names));
  }
  return(0);
}

/// Find item by descending array of \p names. Includes self in search.
/// Only fltk3::Tree should need this method.
///
/// \returns item, or 0 if not found
///
fltk3::TreeItem *fltk3::TreeItem::find_item(char **names) {
  if ( label() && strcmp(label(), *names) == 0 ) {	// match self?
    if ( *(names+1) == 0 ) {				// end of names,
      return(this);					// found ourself.
    }
  }
  if ( children() ) {					// check children..
    return(find_child_item(names));
  }
  return(0);
}

/// Find the index number for the specified 'item'
/// in the current item's list of children.
///
/// \returns the index, or -1 if not found.
///
int fltk3::TreeItem::find_child(fltk3::TreeItem *item) {
  for ( int t=0; t<children(); t++ ) {
    if ( item == child(t) ) {
      return(t);
    }
  }
  return(-1);
}

/// Add a new child to this item with the name 'new_label', with defaults from 'prefs'.
/// An internally managed copy is made of the label string.
/// Adds the item based on the value of prefs.sortorder().
///
fltk3::TreeItem *fltk3::TreeItem::add(const fltk3::TreePrefs &prefs, const char *new_label) {
  fltk3::TreeItem *item = new fltk3::TreeItem(prefs);
  item->label(new_label);
  item->_parent = this;
  switch ( prefs.sortorder() ) {
    case fltk3::TREE_SORT_NONE: {
      _children.add(item);
      return(item);
    }
    case fltk3::TREE_SORT_ASCENDING: {
      for ( int t=0; t<_children.total(); t++ ) {
        fltk3::TreeItem *c = _children[t];
        if ( c->label() && strcmp(c->label(), new_label) > 0 ) {
          _children.insert(t, item);
          return(item);
        }
      }
      _children.add(item);
      return(item);
    }
    case fltk3::TREE_SORT_DESCENDING: {
      for ( int t=0; t<_children.total(); t++ ) {
        fltk3::TreeItem *c = _children[t];
        if ( c->label() && strcmp(c->label(), new_label) < 0 ) {
          _children.insert(t, item);
          return(item);
        }
      }
      _children.add(item);
      return(item);
    }
  }
  return(item);
}

/// Descend into the path specified by \p arr, and add a new child there.
/// Should be used only by fltk3::Tree's internals.
/// Adds the item based on the value of prefs.sortorder().
/// \returns the item added.
///
fltk3::TreeItem *fltk3::TreeItem::add(const fltk3::TreePrefs &prefs, char **arr) {
  int t = find_child(*arr);
  fltk3::TreeItem *item = 0;
  if ( t == -1 ) {
    item = (fltk3::TreeItem*)add(prefs, *arr);
  } else {
    item = (fltk3::TreeItem*)child(t);
  }
  if ( *(arr+1) ) {		// descend?
    return(item->add(prefs, arr+1));
  } else {
    return(item);		// end? done
  }
}

/// Insert a new item into current item's children at a specified position.
/// \returns the new item inserted.
///
fltk3::TreeItem *fltk3::TreeItem::insert(const fltk3::TreePrefs &prefs, const char *new_label, int pos) {
  fltk3::TreeItem *item = new fltk3::TreeItem(prefs);
  item->label(new_label);
  item->_parent = this;
  _children.insert(pos, item);
  return(item);
}

/// Insert a new item above this item.
/// \returns the new item inserted, or 0 if an error occurred.
///
fltk3::TreeItem *fltk3::TreeItem::insert_above(const fltk3::TreePrefs &prefs, const char *new_label) {
  fltk3::TreeItem *p = _parent;
  if ( ! p ) return(0);
  // Walk our parent's children to find ourself
  for ( int t=0; t<p->children(); t++ ) {
    fltk3::TreeItem *c = p->child(t);
    if ( this == c ) {
      return(p->insert(prefs, new_label, t));
    }
  }
  return(0);
}

/// Remove child by item.
///    \returns 0 if removed, -1 if item not an immediate child.
///
int fltk3::TreeItem::remove_child(fltk3::TreeItem *item) {
  for ( int t=0; t<children(); t++ ) {
    if ( child(t) == item ) {
      item->clear_children();
      _children.remove(t);
      return(0);
    }
  }
  return(-1);
}

/// Remove immediate child (and its children) by its label 'name'.
/// \returns 0 if removed, -1 if not found.
///
int fltk3::TreeItem::remove_child(const char *name) {
  for ( int t=0; t<children(); t++ ) {
    if ( child(t)->label() ) {
      if ( strcmp(child(t)->label(), name) == 0 ) {
        _children.remove(t);
        return(0);
      }
    }
  }
  return(-1);
}

/// Swap two of our children, given two child index values.
/// Use this eg. for sorting.
///
/// This method is FAST, and does not involve lookups.
///
/// No range checking is done on either index value.
///
/// \returns
///    -    0 : OK
///    -   -1 : failed: 'a' or 'b' is not our immediate child
///
void fltk3::TreeItem::swap_children(int ax, int bx) {
  _children.swap(ax, bx);
}

/// Swap two of our children, given item pointers.
/// Use this eg. for sorting. 
///
/// This method is SLOW because it involves linear lookups.
/// For speed, use swap_children(int,int) instead.
///
/// \returns
///    -    0 : OK
///    -   -1 : failed: 'a' or 'b' is not our immediate child
///
int fltk3::TreeItem::swap_children(fltk3::TreeItem *a, fltk3::TreeItem *b) {
  int ax = -1, bx = -1;
  for ( int t=0; t<children(); t++ ) {	// find index for a and b
    if ( _children[t] == a ) { ax = t; if ( bx != -1 ) break; else continue; }
    if ( _children[t] == b ) { bx = t; if ( ax != -1 ) break; else continue; }
  }
  if ( ax == -1 || bx == -1 ) return(-1);	// not found? fail
  swap_children(ax,bx);
  return(0);
}

/// Internal: Horizontal connector line based on preference settings.
void fltk3::TreeItem::draw_horizontal_connector(int x1, int x2, int y, const fltk3::TreePrefs &prefs) {
  fltk3::color(prefs.connectorcolor());
  switch ( prefs.connectorstyle() ) {
    case fltk3::TREE_CONNECTOR_SOLID:
      y |= 1;				// force alignment w/dot pattern
      fltk3::line(x1,y,x2,y);
      return;
    case fltk3::TREE_CONNECTOR_DOTTED: 
        {
            y |= 1;				// force alignment w/dot pattern
            for ( int xx=x1; xx<=x2; xx++ ) {
                if ( !(xx & 1) ) fltk3::point(xx, y);
            }
        }
      return;
    case fltk3::TREE_CONNECTOR_NONE:
      return;
  }
}

/// Internal: Vertical connector line based on preference settings.
void fltk3::TreeItem::draw_vertical_connector(int x, int y1, int y2, const fltk3::TreePrefs &prefs) {
  fltk3::color(prefs.connectorcolor());
  switch ( prefs.connectorstyle() ) {
    case fltk3::TREE_CONNECTOR_SOLID:
      y1 |= 1;				// force alignment w/dot pattern
      y2 |= 1;				// force alignment w/dot pattern
      fltk3::line(x,y1,x,y2);
      return;
    case fltk3::TREE_CONNECTOR_DOTTED:
        {
            y1 |= 1;				// force alignment w/dot pattern
            y2 |= 1;				// force alignment w/dot pattern
            for ( int yy=y1; yy<=y2; yy++ ) {
                if ( yy & 1 ) fltk3::point(x, yy);
            }
        }
        return;
    case fltk3::TREE_CONNECTOR_NONE:
      return;
  }
}

/// Find the item that the last event was over.
///
///    Returns the item if it is visible, and mouse is over it.
///    Works even if widget deactivated.
///    Use event_on_collapse_icon() to determine if collapse button was pressed.
///
///    \returns const visible item under the event if found, or 0 if none.
///
const fltk3::TreeItem *fltk3::TreeItem::find_clicked(const fltk3::TreePrefs &prefs) const {
  if ( ! _visible ) return(0);
  if ( is_root() && !prefs.showroot() ) {
    // skip event check if we're root but root not being shown
  } else {
    // See if event is over us
    if ( ::event_inside(_xywh) ) {		// event within this item?
      return(this);				// found
    }
  }
  if ( is_open() ) {				// open? check children of this item
    for ( int t=0; t<children(); t++ ) {
      const fltk3::TreeItem *item;
      if ( ( item = _children[t]->find_clicked(prefs) ) != NULL) {	// check child and its descendents
        return(item);							// found?
      }
    }
  }
  return(0);
}

/// Non-const version of the above.
/// Find the item that the last event was over.
///
///    Returns the item if it is visible, and mouse is over it.
///    Works even if widget deactivated.
///    Use event_on_collapse_icon() to determine if collapse button was pressed.
///
///    \returns the visible item under the event if found, or 0 if none.
///
fltk3::TreeItem *fltk3::TreeItem::find_clicked(const fltk3::TreePrefs &prefs) {
  if ( ! _visible ) return(0);
  if ( is_root() && !prefs.showroot() ) {
    // skip event check if we're root but root not being shown
  } else {
    // See if event is over us
    if ( ::event_inside(_xywh) ) {		// event within this item?
      return(this);				// found
    }
  }
  if ( is_open() ) {				// open? check children of this item
    for ( int t=0; t<children(); t++ ) {
      fltk3::TreeItem *item;
      if ( ( item = _children[t]->find_clicked(prefs) ) != NULL ) {	// check child and its descendents
        return(item);							// found?
      }
    }
  }
  return(0);
}

static void draw_item_focus(fltk3::Box* B, fltk3::Color C, int X, int Y, int W, int H) {
  if (!fltk3::visible_focus()) return;
  if (   B==fltk3::DOWN_BOX
      || B==fltk3::DOWN_FRAME
      || B==fltk3::THIN_DOWN_BOX
      || B==fltk3::THIN_DOWN_FRAME)
  {
    X ++;
    Y ++;
  }
  fltk3::color(fltk3::contrast(fltk3::BLACK, C));

#if defined(USE_X11) || defined(__APPLE_QUARTZ__)
  fltk3::line_style(fltk3::DOT);
  fltk3::rect(X + fltk3::box_dx(B), Y + fltk3::box_dy(B),
          W - fltk3::box_dw(B) - 1, H - fltk3::box_dh(B) - 1);
  fltk3::line_style(fltk3::SOLID);
#else
  // Some platforms don't implement dotted line style, so draw
  // every other pixel around the focus area...
  //
  // Also, QuickDraw (MacOS) does not support line styles specifically,
  // and the hack we use in fltk3::line_style() will not draw horizontal lines
  // on odd-numbered rows...
  int i, xx, yy;

  X += fltk3::box_dx(B);
  Y += fltk3::box_dy(B);
  W -= fltk3::box_dw(B) + 2;
  H -= fltk3::box_dh(B) + 2;

  for (xx = 0, i = 1; xx < W; xx ++, i ++) if (i & 1) fltk3::point(X + xx, Y);
  for (yy = 0; yy < H; yy ++, i ++) if (i & 1) fltk3::point(X + W, Y + yy);
  for (xx = W; xx > 0; xx --, i ++) if (i & 1) fltk3::point(X + xx, Y + H);
  for (yy = H; yy > 0; yy --, i ++) if (i & 1) fltk3::point(X, Y + yy);
#endif
}

/// Draw this item and its children.
void fltk3::TreeItem::draw(int X, int &Y, int W, fltk3::Widget *tree,
			fltk3::TreeItem *itemfocus,
                        const fltk3::TreePrefs &prefs, int lastchild) {
  if ( ! _visible ) return; 
  fltk3::font(_labelfont, _labelsize);
  int H = _labelsize;
  if(usericon() && H < usericon()->h()) H = usericon()->h(); 
  H += prefs.linespacing() + fltk3::descent();
  // adjust horizontally if we draw no connecting lines
  if ( is_root() && prefs.connectorstyle() == fltk3::TREE_CONNECTOR_NONE ) {
    X -= prefs.openicon()->w();
    W += prefs.openicon()->w();
  }
  // Colors, fonts
  fltk3::Color fg = _selected ? fltk3::contrast(_labelfgcolor, tree->selection_color())
                          : _active ? _labelfgcolor 
			            : fltk3::inactive(_labelfgcolor);
  fltk3::Color bg = _selected ? _active ? tree->selection_color() 
                                    : fltk3::inactive(tree->selection_color())
                          : _labelbgcolor;
  // Update the xywh of this item
  _xywh[0] = X;
  _xywh[1] = Y;
  _xywh[2] = W;
  _xywh[3] = H;
  // Text size
  int textw=0, texth=0;
  fltk3::measure(_label, textw, texth, 0);
  int textycenter = Y+(H/2);
  int &icon_w = _collapse_xywh[2] = prefs.openicon()->w();
  int &icon_x = _collapse_xywh[0] = X + (icon_w + prefs.connectorwidth())/2 - 3;
  int &icon_y = _collapse_xywh[1] = textycenter - (prefs.openicon()->h()/2);
  _collapse_xywh[3] = prefs.openicon()->h();
  // Horizontal connector values
  int hstartx  = X+icon_w/2-1;
  int hendx    = hstartx + prefs.connectorwidth();
  int hcenterx = X + icon_w + ((hendx - (X + icon_w)) / 2);
  
  // See if we should draw this item
  //    If this item is root, and showroot() is disabled, don't draw.
  //
  char drawthis = ( is_root() && prefs.showroot() == 0 ) ? 0 : 1;
  if ( drawthis ) {
    // Draw connectors
    if ( prefs.connectorstyle() != fltk3::TREE_CONNECTOR_NONE ) {
      // Horiz connector between center of icon and text
      // if this is root, the connector should not dangle in thin air on the left
      if (is_root())
        draw_horizontal_connector(hcenterx, hendx, textycenter, prefs);
      else
        draw_horizontal_connector(hstartx, hendx, textycenter, prefs);
      if ( has_children() && is_open() ) {
        // Small vertical line down to children
        draw_vertical_connector(hcenterx, textycenter, Y+H, prefs);
      }
      // Connectors for last child
      if ( ! is_root() ) {
        if ( lastchild ) {
          draw_vertical_connector(hstartx, Y, textycenter, prefs);
        } else {
          draw_vertical_connector(hstartx, Y, Y+H, prefs);
        }
      }
    } 
    // Draw collapse icon
    if ( has_children() && prefs.showcollapse() ) {
      // Draw icon image
      if ( is_open() ) {
        prefs.closeicon()->draw(icon_x,icon_y);
      } else {
        prefs.openicon()->draw(icon_x,icon_y);
      }
    }
    // Background for this item
    int cw1 = icon_w+prefs.connectorwidth()/2, cw2 = prefs.connectorwidth();
    int cwidth = cw1>cw2 ? cw1 : cw2;
    int &bx = _label_xywh[0] = X+(icon_w/2-1+cwidth);
    int &by = _label_xywh[1] = Y;
    int &bw = _label_xywh[2] = W-(icon_w/2-1+cwidth);
    int &bh = _label_xywh[3] = H;
    // Draw bg only if different from tree's bg
    if ( bg != tree->color() || is_selected() ) {
      if ( is_selected() ) {
        // Selected? Use selectbox() style
        fltk3::draw_box(prefs.selectbox(), bx, by, bw, bh, bg);
      } else {
        // Not Selected? use plain filled rectangle
        fltk3::color(bg);
        fltk3::rectf(bx, by, bw, bh);
      }
    }
    // Draw user icon (if any)
    int useroff = (icon_w/2-1+cwidth);
    if ( usericon() ) {
      // Item has user icon? Use it
      useroff += prefs.usericonmarginleft();
      icon_y = textycenter - (usericon()->h() >> 1);
      usericon()->draw(X+useroff,icon_y);
      useroff += usericon()->w();
    } else if ( prefs.usericon() ) {
      // Prefs has user icon? Use it
      useroff += prefs.usericonmarginleft();
      icon_y = textycenter - (prefs.usericon()->h() >> 1);
      prefs.usericon()->draw(X+useroff,icon_y);
      useroff += prefs.usericon()->w();
    }
    useroff += prefs.labelmarginleft();
    // Draw label
    if ( widget() ) {
      // Widget? Draw it
      int lx = X+useroff;
      int ly = by;
      int lw = widget()->w();
      int lh = bh;
      if ( widget()->x() != lx || widget()->y() != ly ||
          widget()->w() != lw || widget()->h() != lh ) {
        widget()->resize(lx, ly, lw, lh);		// fltk will handle drawing this
      }
    } else {
      // No label widget? Draw text label
      if ( _label ) {
        fltk3::color(fg);
        fltk3::draw(_label, X+useroff, Y+H-fltk3::descent()-1);
      }
    }
    if ( this == itemfocus && fltk3::visible_focus() && fltk3::focus() == tree) {
      // Draw focus box around this item
      draw_item_focus(fltk3::NO_BOX,bg,bx+1,by+1,bw-1,bh-1);
    }
    Y += H;
  }			// end drawthis
  // Draw children
  if ( has_children() && is_open() ) {
    int child_x = drawthis ? 				// offset children to right,
    (hcenterx - (icon_w/2) + 1) : X;			// unless didn't drawthis
    int child_w = W - (child_x-X);
    int child_y_start = Y;
    for ( int t=0; t<children(); t++ ) {
      int lastchild = ((t+1)==children()) ? 1 : 0;
      _children[t]->draw(child_x, Y, child_w, tree, itemfocus, prefs, lastchild);
    }
    if ( has_children() && is_open() ) {
      Y += prefs.openchild_marginbottom();		// offset below open child tree
    }
    if ( ! lastchild ) {
      draw_vertical_connector(hstartx, child_y_start, Y, prefs);
    }
  }
}

/// Was the event on the 'collapse' button?
///
int fltk3::TreeItem::event_on_collapse_icon(const fltk3::TreePrefs &prefs) const {
  if ( _visible && _active && has_children() && prefs.showcollapse() ) {
    return(::event_inside(_collapse_xywh) ? 1 : 0);
  } else {
    return(0);
  }
}

/// Was event on the label()?
///
int fltk3::TreeItem::event_on_label(const fltk3::TreePrefs &prefs) const {
  if ( _visible && _active ) {
    return(::event_inside(_label_xywh) ? 1 : 0);
  } else {
    return(0);
  }
}

/// Internal: Show the FLTK widget() for this item and all children.
/// Used by open() to re-show widgets that were hidden by a previous close()
///
void fltk3::TreeItem::show_widgets() {
  if ( _widget ) _widget->show();
  if ( is_open() ) {
    for ( int t=0; t<_children.total(); t++ ) {
      _children[t]->show_widgets();
    }
  }
}

/// Internal: Hide the FLTK widget() for this item and all children.
/// Used by close() to hide widgets.
///
void fltk3::TreeItem::hide_widgets() {
  if ( _widget ) _widget->hide();
  for ( int t=0; t<_children.total(); t++ ) {
    _children[t]->hide_widgets();
  }
}

/// Open this item and all its children.
void fltk3::TreeItem::open() {
  _open = 1;
  // Tell children to show() their widgets
  for ( int t=0; t<_children.total(); t++ ) {
    _children[t]->show_widgets();
  }
}

/// Close this item and all its children.
void fltk3::TreeItem::close() {
  _open = 0;
  // Tell children to hide() their widgets
  for ( int t=0; t<_children.total(); t++ ) {
    _children[t]->hide_widgets();
  }
}

/// Returns how many levels deep this item is in the hierarchy.
///
/// For instance; root has a depth of zero, and its immediate children
/// would have a depth of 1, and so on.
///
int fltk3::TreeItem::depth() const {
  int count = 0;
  const fltk3::TreeItem *item = parent();
  while ( item ) {
    ++count;
    item = item->parent();
  }
  return(count);
}

/// Return the next item in the tree.
///
/// This method can be used to walk the tree forward.
/// For an example of how to use this method, see fltk3::Tree::first().
/// 
/// \returns the next item in the tree, or 0 if there's no more items.
///
fltk3::TreeItem *fltk3::TreeItem::next() {
  fltk3::TreeItem *p, *c = this;
  if ( c->has_children() ) {
    return(c->child(0));
  }
  while ( ( p = c->parent() ) != NULL ) {	// loop upwards through parents
    int t = p->find_child(c);			// find our position in parent's children[] array
    if ( ++t < p->children() )			// not last child?
      return(p->child(t));			// return next child
    c = p;					// child becomes parent to move up generation
  }						// loop: moves up to next parent
  return(0);					// hit root? done
}

/// Return the previous item in the tree.
///
/// This method can be used to walk the tree backwards.
/// For an example of how to use this method, see fltk3::Tree::last().
/// 
/// \returns the previous item in the tree, or 0 if there's no item above this one (hit the root).
///
fltk3::TreeItem *fltk3::TreeItem::prev() {
  fltk3::TreeItem *p=parent();		// start with parent
  if ( ! p ) return(0);			// hit root? done
  int t = p->find_child(this);		// find our position in parent's children[] array
  if ( --t == -1 ) {	 		// are we first child?
    return(p);				// return immediate parent
  }
  p = p->child(t);			// take parent's previous child
  while ( p->has_children() ) {		// has children?
    p = p->child(p->children()-1);	// take last child
  }
  return(p);
}

/// Return this item's next sibling.
///
/// Moves to the next item below us at the same level (sibling).
/// Use this to move down the tree without moving deeper into the tree,
/// effectively skipping over this item's children/descendents.
/// 
/// \returns item's next sibling, or 0 if none.
///
fltk3::TreeItem *fltk3::TreeItem::next_sibling() {
  if ( !parent() ) return(0);			// No parent (root)? We have no siblings
  int index = parent()->find_child(this);	// find our position in parent's child() array
  if ( index == -1 ) return(0);			// parent doesn't know us? weird
  if ( (index+1) < parent()->children() )	// is there a next child?
    return(parent()->child(index+1));		// return next child if there's one below us
  return(0);					// no siblings below us
}

/// Return this item's previous sibling.
///
/// Moves to the previous item above us at the same level (sibling).
/// Use this to move up the tree without moving deeper into the tree.
/// 
/// \returns This item's previous sibling, or 0 if none.
///
fltk3::TreeItem *fltk3::TreeItem::prev_sibling() {
  if ( !parent() ) return(0);				// No parent (root)? We have no siblings
  int index = parent()->find_child(this);		// find next position up in parent's child() array
  if ( index == -1 ) return(0);				// parent doesn't know us? weird
  if ( index > 0 ) return(parent()->child(index-1));	// return previous child if there's one above us
  return(0);						// no siblings above us
}

/// Return the next visible item. (If this item has children and is closed, children are skipped)
///
/// This method can be used to walk the tree forward, skipping items
/// that are not currently visible to the user.
/// 
/// \returns the next visible item below us, or 0 if there's no more items.
///
fltk3::TreeItem *fltk3::TreeItem::next_displayed(fltk3::TreePrefs &prefs) {
  fltk3::TreeItem *c = this;
  while ( c ) {
    if ( c->is_root() && !prefs.showroot() ) {		// on root and can't show it?
      c = c->next();					// skip ahead, try again
      continue;
    }
    if ( c->has_children() && c->is_close() ) {		// item has children and: invisible or closed?
      // Skip children, take next sibling. If none, try parent's sibling, repeat
      while ( c ) {
	fltk3::TreeItem *sib = c->next_sibling();		// get sibling
	if ( sib ) { c = sib; break; }			// Found? let outer loop test it
	c = c->parent();				// No sibling? move up tree, try parent's sibling
      }
    } else {						// has children and isn't closed, or no children
      c = c->next();					// use normal 'next'
    }
    if ( !c ) return(0);				// no more? done
    // Check all parents to be sure none are closed.
    // If closed, move up to that level and repeat until sure none are closed.
    fltk3::TreeItem *p = c->parent();
    while (1) {
      if ( !p || p->is_root() ) return(c);		// hit top? then we're displayed, return c
      if ( p->is_close() ) c = p;			// found closed parent? make it current
      p = p->parent();					// continue up tree
    }
    if ( c && c->visible() ) return(c);			// item visible? return it
  }
  return(0);						// hit end: no more items
}

/// Return the previous visible item. (If this item above us has children and is closed, its children are skipped)
///
/// This method can be used to walk the tree backward, 
/// skipping items that are not currently visible to the user.
/// 
/// \returns the previous visible item above us, or 0 if there's no more items.
///
fltk3::TreeItem *fltk3::TreeItem::prev_displayed(fltk3::TreePrefs &prefs) {
  fltk3::TreeItem *c = this;
  while ( c ) {
    c = c->prev();					// previous item
    if ( !c ) break;					// no more items? done
    if ( c->is_root() )					// root
      return((prefs.showroot()&&c->visible()) ? c : 0);	// return root if visible
    if ( !c->visible() ) continue;			// item not visible? skip
    // Check all parents to be sure none are closed.
    // If closed, move up to that level and repeat until sure none are closed.
    fltk3::TreeItem *p = c->parent();
    while (1) {
      if ( !p || p->is_root() ) return(c);		// hit top? then we're displayed, return c
      if ( p->is_close() ) c = p;			// found closed parent? make it current
      p = p->parent();					// continue up tree
    }
  }
  return(0);						// hit end: no more items
}

/// Returns if item and all its parents are visible.
/// Also takes into consideration if any parent is close()ed.
/// \returns
///    1 -- item and its parents are visible/open()
///    0 -- item (or parents) invisible or close()ed.
///
int fltk3::TreeItem::visible_r() const {
  for (const fltk3::TreeItem *p=this; p; p=p->parent())	// move up through parents
    if (!p->visible() || p->is_close()) return(0);	// any parent not visible or closed?
  return(1);
}

//
// End of "$Id$".
//
