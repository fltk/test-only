//
// "$Id: Fl_Browser.cxx,v 1.102 2004/11/12 06:50:14 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/Browser.h>
#include <fltk/Button.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/layout.h>
#include <fltk/Box.h>
#include <fltk/Item.h>
#include <fltk/draw.h>
#include <fltk/error.h>
#include <fltk/Cursor.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
using namespace fltk;

/*! \class fltk::Browser

  Displays a scrolling vertical list of text widgets, possibly with a
  hierachial arrangement, and lets the user select one of them.

  \image html browser.gif

  The items may be created as child widgets (usually the same widgets
  as are used to create menus: fltk::Item widgets, or fltk::ItemGroup
  widgets to make a hierarchy). Alternatively you can build simple
  text lists by using the add() method, which will create the child
  widgets for you (and even create a hierarchy if you put slashes in
  the text). You can also us an fltk::List which allows you to control
  the storage by dynamically creating a "fake" widget for the browser
  to use to draw each item.

  All the functions used to add, remove, or modify items in the list
  are defined by the base class fltk::Menu. See that for much more
  information.

  For a simple constant list you can populate the list by calling
  browser->add("text of item") once for each item.  If you give the
  browser a callback you can find out what item was selected with
  value(), the first item is zero (this is different from
  older versions of fltk that started at 1!), and will be negative if
  no item is selected. You can change the selected item with
  value(new_value).

  The subclass fltk::MultiBrowser lets the user select more than one
  item at the same time.

  The callback() is done when the user changes the selected items or
  when they open/close parents. In addition, if the user double-clicks
  a non-parent item, then it is "executed" which usually means that
  the callback() on the item itself is run. However if that is the
  default callback, the callback() of this widget is called with the
  user_data() of the item.

  You can control when callbacks are done with the when() method. The
  following values are useful, the default value is fltk::WHEN_CHANGED.

  - fltk::WHEN_NEVER - Callback is never done. changed() can be used
    to see if the user has modified the browser.

  - fltk::WHEN_CHANGED - Callback is called on every change to each
    item as it happens. The method item() will return the one that
    is being changed. Notice that selecting large numbers in a mulit
    browser will produce large numbers of callbacks.

  - fltk::WHEN_RELEASE - Callback is done when the user releases the
    mouse after some changes, and on any keystroke that
    changes the item. For a multi browser you will only be able
    to find out all the changes by scanning all the items in the callback.

  - fltk::WHEN_RELEASE_ALWAYS - Callback is done when the user
    releases the mouse even if the current item has not changed, and
    on any arrow keystroke even when at the top or bottom of the
    browser.

  - fltk::WHEN_ENTER_KEY - If you turn this on then the enter key is
    a shortcut and executes the current item like double-click.

*/

////////////////////////////////////////////////////////////////
// Compile-time style configuration options.
// For "run time configuration" copy this file, rename the widget,
// compile with your desired options, and use your renamed widget.
// Deal with it.

#define DRAW_STRIPES 0 // draw Macintosh-style stripes
#define TRIANGLE_GLYPH 0 // use Macintosh/fltk-style glyphs

/*! \fn int Browser::width() const
  The width of the longest item in the browser, measured in pixels. If
  this is changed (by adding or deleting items or opening or closing a
  parent item) then layout() must be called before this is correct.
*/

/*! \fn int Browser::height() const
  The height of all the visible items in the browser, measured in
  pixels. If this is changed (by adding or deleting items or opening
  or closing a parent item) then layout() must be called before this
  is correct.
*/

/*! \fn int Browser::box_width() const
  The width of the display area of the browser in pixels, this is w()
  minus the edges of the box() minus the width of the vertical
  scrollbar, if visible. If this is changed (by resizing the widget,
  adding or deleting items or opening or closing a parent item such
  that the scrollbar visibility changes) then layout() must be called
  before this is correct.
*/

/*! \fn int Browser::box_height() const
  The height of the display area of the browser in pixels, this is h()
  minus the edges of the box() minus the height of the horizontal
  scrollbar, if visible. If this is changed (by resizing the widget,
  adding or deleting items or opening or closing a parent item such
  that the scrollbar visibility changes) then layout() must be called
  before this is correct.
*/

/*! \fn void Browser::indented(bool v)
  Turn this on to for space to be reserved for open/close boxes drawn
  to the left of top-level items. You usually want this for a
  hierarchial browser. This should be off for a flat browser, or to
  emulate Windows Explorer where "my computer" does not have an
  open/close to the left of it. The default value is false.
*/

/*! \fn Widget *Browser::header(int col)
  Return pointer to Widget in column \a col, starting from index 0.
  If column \a col is invalid, NULL is returned.
*/
  
/*! \fn int Browser::nheader() const
  Return number of columns in browser.
*/

////////////////////////////////////////////////////////////////
// Moving between items:

// A "mark" is like a pointer to a widget somewhere in the hierarchy
// of the Browser. Actually it is an array of child indicies, and also
// the vertical position and index number (used to decide stripe color)
// of the item.

// The Browser has space to store a fixed number of marks.

// The "current item" is the one pointed to by the mark called HERE.
// A pointer to it is stored in item(). To allow List to work, this
// pointer is only valid until the next item is referenced, so don't
// copy it to any storage. You can move the current item forward and
// backward, skipping invisible items. You can also place it on any
// item, even ones that cannot be seen, by using goto_item.

/*!
  Return true if the item would be visible to the user if the browser
  was scrolled to the correct location. This means that the
  fltk::INVISIBLE flag is not set on it, and all parents of it are
  open and visible as well.
*/
bool Browser::item_is_visible() const {
  return open_level[HERE] >= item_level[HERE] && item()->visible();
}

/*! This function increases the number of levels we can store in each
  mark, and sets the level of the current mark to n. */
void Browser::set_level(int n) {
  if (n > levels) {
    if (n > 255) fatal("More than 255 levels in Browser");
    for (int i = 0; i < NUMMARKS; i++) {
      item_index[i]=(int*)realloc((void*)item_index[i],(n+1)*sizeof(int));
    }
    levels = n;
  }
  item_level[HERE] = n;
}

/*!
  Because of the hierarchial structure it is difficult to identify
  an item in the browser. Instead of passing an identifier to all the
  calls that can modify an item, the browser provides several calls to
  set item() based on various rules, and then calls to modify the
  current item().

  This call sets item() to the very first visible widget in
  the browser. It returns the widget for that item, or null if the
  browser is empty.

  If you have invisible items in the browser you should use
  goto_index(0) if you want to go to the first item even if it is
  invisible.
*/
Widget* Browser::goto_top() {
  item_level[HERE] = 0;
  open_level[HERE] = 0;
  item_position[HERE] = 0;
  item_index[HERE][0] = 0;
  siblings = children(item_index[HERE],0);
  // empty browser must return a null widget
  if (siblings <= 0) {
    item(0);
  } else {
    item(child(item_index[HERE],0));
    // skip leading invisible widgets:
    if (!item()->visible()) return next_visible();
  }
  return item();
}

/*! \fn Widget* Browser::goto_focus()
  Sets the item() to the "focus" (the item with the dotted square in
  an fltk::MultiBrowser, and the selected item in a normal
  fltk::Browser.
*/

/*! set current item to a particular mark */
Widget* Browser::goto_mark(int mark) {
  item_position[HERE] = item_position[mark];
  item_level[HERE] = item_level[mark];
  open_level[HERE] = open_level[mark];
  for (int L = 0; L <= item_level[HERE]; L++) {
    int i = item_index[HERE][L] = item_index[mark][L];
    siblings = children(item_index[HERE], L);
    if (i < 0 || i >= siblings) {item(0); return 0;}
  }
  item(child(item_index[HERE], item_level[HERE]));
  return item();
}

// copy one mark to another
void Browser::set_mark(int dest_mark, int src_mark) {
  item_position[dest_mark] = item_position[src_mark];
  item_level[dest_mark] = item_level[src_mark];
  open_level[dest_mark] = open_level[src_mark];
  for (int L = item_level[src_mark]; L >= 0; L--)
    item_index[dest_mark][L] = item_index[src_mark][L];
}

// compare the relative locations of these two marks
// return < 0 if mark1 is before mark2
// return   0 if mark1 is mark2
// return > 0 if mark1 is after mark2

static int compare_marks(const int* index1,int L1, const int* index2, int L2) {
  for (int L = 0; ; L++) {
    if (L > L1) {
      if (L > L2) return 0;
      return -1; // mark1 is on a parent of mark2
    } else if (L > L2) {
      return +1; // mark2 is on a parent of mark1
    }
    int n = index1[L] - index2[L];
    if (n) return n > 0 ? +2 : -2;
  }
}

int Browser::compare_marks(int mark1, int mark2) {
  return ::compare_marks(item_index[mark1], item_level[mark1],
			 item_index[mark2], item_level[mark2]);
}

// unsets this mark in toplevel
void Browser::unset_mark(int mark) {
  item_index[mark][0] = -1;
}

// is this toplevel mark set?
bool Browser::is_set(int mark) {
  return item_index[mark][0] >= 0;
}

/*!  Return true if the current item is a parent. Notice that it may
  have zero children. */
bool Browser::item_is_parent() const {
  return children(item_index[HERE],item_level[HERE]+1) >= 0;
}

/*! Move forward to the next visible item (what down-arrow does).
  This does not move and returns null if we are at the bottom. */
Widget* Browser::next_visible() {
  if (item_is_visible()) {
    if (!layout_damage()) item_position[HERE] += item()->height();
    // If we are on an open group title with children, go to first item in group
    if (item()->flags()&VALUE && item_is_parent()) {
      int n = item_level[HERE]+1;
      set_level(n);
      open_level[HERE] = n;
      item_index[HERE][n] = 0;
      siblings = children(item_index[HERE], n);
    } else {
      // go to next item in this group
      item_index[HERE][item_level[HERE]] ++;
    }
  } else {
    // item is not visible, point after it (or after closed/invisible parent)
    item_level[HERE] = open_level[HERE];
    siblings = children(item_index[HERE], item_level[HERE]);
    item_index[HERE][item_level[HERE]] ++;
  }

  // loop to find the next real item:
  for (;;) {

    if (item_index[HERE][item_level[HERE]] >= siblings) {
      // we moved off the end of a group
      if (!item_level[HERE]) {item(0); return 0;} // end of the entire browser
      open_level[HERE] = --item_level[HERE];
      item_index[HERE][item_level[HERE]] ++;
      siblings = children(item_index[HERE], item_level[HERE]);
      continue;
    }

    item(child(item_index[HERE], item_level[HERE]));

    // skip invisible items:
    if (item()->visible()) break;
    item_index[HERE][item_level[HERE]] ++;
  }

  return item();
}

/*! Move backward to previous visible item:
  This does not move and returns null if we are at the top. */
Widget* Browser::previous_visible() {

  // if we are on a child of a closed or invisible parent, pretend
  // we are on the item after that parent:
  if (item_level[HERE] > open_level[HERE]) {
    item_level[HERE] = open_level[HERE];
    item_index[HERE][item_level[HERE]] ++;
  }

  // there's got to be simpler logic for this loop...
  for (;;) {
    // go up to parent from first item in a group:
    if (!item_index[HERE][item_level[HERE]]) {
      if (!item_level[HERE]) { // start of browser
	item_position[HERE] = 0;
	return 0;
      }
      open_level[HERE] = --item_level[HERE];
      item(child(item_index[HERE], item_level[HERE]));
      siblings = children(item_index[HERE], item_level[HERE]);
      break;
    }

    // go back to previous item in this group:
    item_index[HERE][item_level[HERE]] --;
    item(child(item_index[HERE], item_level[HERE]));

    // go to last child in a group:
    while (item()->flags()&VALUE && item()->visible() && item_is_parent()) {
      int n = children(item_index[HERE], item_level[HERE]+1);
      if (n <= 0) break; // the group is empty, remain on it's title
      set_level(item_level[HERE]+1);
      open_level[HERE] = item_level[HERE];
      item_index[HERE][item_level[HERE]] = n-1;
      item(child(item_index[HERE], item_level[HERE]));
      siblings = n;
    }

    if (item()->visible()) break;
  }

  if (!layout_damage()) item_position[HERE] -= item()->height();
  return item();
}

/*!  Move the current item to the next item. If if is a parent it
  moves to the first child. If not a parent, it moves to the next
  child of it's parent. If it is the last child it moves to the
  parent's brother. Repeatedly calling this will visit every child of
  the browser. This returns the widget. If the current widget is the
  last one this returns null, but the current widget remains on the
  last one.

  The current_position() is NOT set by this! It cannot be calculated
  efficiently and would slow down the use of this function for
  visiting all items.
*/
Widget* Browser::next() {
  int n = children(item_index[HERE], item_level[HERE]+1);
  if (n > 0) {
    set_level(item_level[HERE]+1);
    item_index[HERE][item_level[HERE]] = 0;
    siblings = n;
  } else {
    item_index[HERE][item_level[HERE]]++;
  }
  for (;;) {
    if (item_index[HERE][item_level[HERE]] < siblings) {
      item(child(item_index[HERE], item_level[HERE]));
      return item();
    }
    if (item_level[HERE] <= 0) {item(0); return 0;}
    item_level[HERE]--;
    item_index[HERE][item_level[HERE]]++;
    siblings = children(item_index[HERE], item_level[HERE]);
  }
}

/*! Set the current item() to the last one who's top is at or before
  \a Y pixels from the top. */
Widget* Browser::goto_position(int Y) {
  if (Y < 0) Y = 0;
  if (layout_damage() || Y<=yposition_/2 || !goto_mark(FIRST_VISIBLE)) {
    goto_top();
  } else {
    // move backwards until we are before or at the position:
    while (item_position[HERE] > Y) {
      if (!previous_visible()) {goto_top(); break;}
    }
  }
  // move forward to the item:
  if (item()) for (;;) {
    int h = item()->height();
    if (item_position[HERE]+h > Y) break;
    if (!next_visible()) {previous_visible(); return 0;}
  }
  return item();
}

/*! \fn void Browser::damage_item()
  Tell the browser to redraw the current item. Do this if you know it
  has changed appearance. This is better than redrawing the entire
  browser because it will blink a lot less.
*/

static bool nodamage;
/*! Set item referenced by this mark as being damaged. */
void Browser::damage_item(int mark) {
  if (nodamage) return;
  if (!compare_marks(REDRAW_0, mark) || !compare_marks(REDRAW_1, mark))
    return;
  int m = REDRAW_0;
  if (is_set(m)) {
    m = REDRAW_1;
    // if both marks are used we give up and damage the whole thing:
    if (is_set(m)) {redraw(DAMAGE_CONTENTS); return;}
  }
  set_mark(m, mark);
  redraw(DAMAGE_VALUE);
}

/*! \fn int Browser::current_level() const
  Return the nesting level of the current item (how many parents it has).  */

/*! \fn const int* Browser::current_index() const
 Return an array of current_level()+1 indexes saying which child at
 each level includes the current item.
*/

/*! \fn int Browser::current_position() const
  Return the y position, in pixels, of the top edge of the current
  item. You may also want the height, which is in
  item()->height().
*/

/*! \fn int Browser::focus_level() const
  Return the nesting level of the focus (how many parents it has).
  The focus is the selected item the user sees.
*/

/*! \fn const int* Browser::focus_index() const
 Return an array of focus_level()+1 indexes saying which child at
 each level includes the focus.
*/

/*! \fn int Browser::focus_position() const
  Return the y position, in pixels, of the top edge of the focus
  item. You may also want the height, which is in
  goto_focus(); item()->height().
*/

/*! \fn void Browser::value(int v)
  Same as goto_index(v);set_focus();, to change the current item in a
  non-hierarchial browser.
*/

/*! \fn int Browser::value() const
  Returns focus_index(v)[0], to get the current item in a
  non-hierarchial browser.
*/

////////////////////////////////////////////////////////////////
// Drawing

// Glyphs that are drawn to the left of items.  Enumeration order
// is important!
enum {
  NONE, BAR, ELL, TEE, CLOSED_ELL, CLOSED_TEE, OPEN_ELL, OPEN_TEE
};

#define BOXSIZE 9

void
browser_glyph(int glyph, int x,int y,int w,int h, const Style* style, Flags f)
{
  Color bg, fg; style->boxcolors(f|OUTPUT, bg, fg);
  //if (fg == BLACK) fg = GRAY33;
  setcolor(fg);
  int lx = x+w/2;
  int ly = y+(h-1)/2;
  switch (glyph) {
  case NONE:
    break;
  case BAR:
    drawline(lx, y, lx, y+h-1);
    break;
  case ELL:
    drawline(lx, y, lx, ly);
    drawline(lx, ly, x+w, ly);
    break;
  case TEE:
    drawline(lx, y, lx, y+h-1);
    drawline(lx, ly, x+w, ly);
    break;
#if TRIANGLE_GLYPH
  case CLOSED_TEE:
  case OPEN_TEE:
    drawline(lx, y, lx, y+h-1);
    goto J1;
  case CLOSED_ELL:
  case OPEN_ELL:
    drawline(lx, y, lx, ly);
  J1:
    Widget::default_glyph(glyph < OPEN_ELL ? GLYPH_RIGHT : GLYPH_DOWN,
	     x, y, w, h, style, f);
    break;
#else
  default: {
    const int boxsize = BOXSIZE/2;
    drawline(lx, y, lx, ly-boxsize);
    if (glyph&1) drawline(lx, ly+boxsize, lx, y+h-1);
    drawline(lx+boxsize, ly, x+w, ly);
    strokerect(lx-boxsize, ly-boxsize, BOXSIZE, BOXSIZE);
    drawline(lx-boxsize+2, ly, lx+boxsize-2, ly);
    if (glyph < OPEN_ELL) drawline(lx, ly-boxsize+2, lx, ly+boxsize-2);
    }
#endif
  }
}

// this is non-zero if a drag was started in a group open/close box:
static char openclose_drag;

// Draws the current item:
void Browser::draw_item() {

  Widget* widget = item();

  int y = Y+item_position[HERE]-yposition_;
  int h = widget->height();

  Flags flags;

  int is_focus = at_mark(FOCUS);

  if (multi() ? widget->selected() : is_focus) {
    setcolor(selection_color());
    fillrect(X, y, W, h);
    widget->set_selected();
    flags = SELECTED;
  } else {
    widget->clear_selected();
    flags = 0;
#if DRAW_STRIPES
    Color c0 = color();
    Color c1 = buttoncolor();
    int x=1; for (int j=0; j<=item_level[HERE]; j++) x ^= item_index[HERE][j]+1;
    if (x & 1 && c1 != c0) {
      // draw odd-numbered items with a dark stripe, plus contrast-enhancing
      // pixel rows on top and bottom:
      setcolor(c1);
      fillrect(X, y+1, W, h-2);
      setcolor(c0 <= GRAY85 ? c1 : GRAY85);
      //setcolor(lerp(c1, c0, 1.9));
      drawline(X, y, X+W, y);
      drawline(X, y+h-1, X+W, y+h-1);
    } else {
      setcolor(c0);
      fillrect(X, y, W, h);
    }
#else
    setcolor(color());
    fillrect(X, y, W, h);
#endif
  }

  int col_shift = X;
  int arrow_size = int(textsize())|1;
  int preview_open =
    (openclose_drag == 1 && pushed() && at_mark(FOCUS)) ? VALUE : 0;
  widget->invert_flag(preview_open);

  int x = X-xposition_;
  // draw the glyphs, one for each nesting level:
  for (int j = indented() ? 0 : 1; j <= item_level[HERE]; j++) {
    int g = item_index[HERE][j] < children(item_index[HERE],j) - 1;
    if (j == item_level[HERE]) {
      if (children(item_index[HERE],j+1)>=0)
	if (widget->flags() & VALUE)
	  g += OPEN_ELL;
	else
	  g += CLOSED_ELL;
      else
	g += ELL;
    }
    draw_glyph(g, x, y, arrow_size, h, flags);
    x += arrow_size;
    col_shift += arrow_size;
  }

  if (focused() && is_focus) {
    if (column_widths_p) 
      focusbox()->draw(x, y, W-col_shift+xposition_, h, style(), flags|(FOCUSED|OUTPUT));
    else
      focusbox()->draw(x, y, widget->width(), h, style(), flags|(FOCUSED|OUTPUT));
  }

  // Shift image width 
  if (widget->image()) {
    int iw,ih; widget->image()->measure(iw,ih);
    col_shift += iw;
  }

  // Shift first column width, so labels after 1. column are lined up correctly.
  int saved_colw = 0;
  int *cols = (int *)column_widths_p;
  if(cols) {
    saved_colw = cols[0];    
    cols[0] -= col_shift;
    if (cols[0]==0) cols[0]--;    
  }

  push_matrix();
  y += (int(leading())-1)/2;
  widget->x(x);
  widget->y(y);
  translate(x, y);
  widget->set_damage(DAMAGE_ALL|DAMAGE_EXPOSE);
  int save_w = widget->w();
  if (!save_w) widget->w(X+W-x);
  widget->draw();
  widget->w(save_w);
  widget->set_damage(0);
  pop_matrix();

  // Restore column width
  if(cols) cols[0] = saved_colw;

  widget->invert_flag(preview_open);
}

void Browser::draw_clip_cb(void* v,int X, int Y, int W, int H) {
  ((Browser*)v)->draw_clip(X,Y,W,H);
}

void Browser::draw_clip(int x, int y, int w, int h) {
  push_clip(x,y,w,h);

  int draw_all = damage() & (DAMAGE_ALL|DAMAGE_CONTENTS);
  if (goto_mark(FIRST_VISIBLE)) for (;;) {
    int item_y = Y+item_position[HERE]-yposition_;
    if (item_y >= y+h) break;
    if (draw_all || !at_mark(REDRAW_0) && !at_mark(REDRAW_1)) draw_item();
    if (!next_visible()) break;
  }

  // erase the area below the last item:
  int bottom_y = Y+item_position[HERE]-yposition_;
  if (bottom_y < y+h) {
    setcolor(color());
    fillrect(x, bottom_y, w, y+h-bottom_y);
  }
  pop_clip();
}

void Browser::draw() {
  const int *last_columns = fltk::column_widths();
  fltk::column_widths(column_widths_p);
  uchar d = damage();
  Item::set_style(this);
  if (d & DAMAGE_ALL) { // full redraw
    //printf("full redraw damage %x\n", d);
    draw_frame();
    draw_clip(X, Y, W, H);
  } else if (d & DAMAGE_CONTENTS) { // redraw contents
    //printf("contents redraw damage %x\n", d);
    draw_clip(X, Y, W, H);
  } else { // minimal update
    //printf("minimal redraw damage %x\n", d);
    if (scrolldx || scrolldy) {
      scrollrect(X, Y, W, H, scrolldx, scrolldy, draw_clip_cb, this);
    }
    int clipped = 0;
    for (int n = REDRAW_0; n <= REDRAW_1; n++) {
      if (goto_mark(n)) {
	    if (!clipped) {push_clip(X,Y,W,H); clipped = 1;}
	    draw_item();
      }
    }
    if (d & DAMAGE_CHILD) {
      if (goto_mark(FIRST_VISIBLE)) for (;;) {
	int y = Y+item_position[HERE]-yposition_+(int(leading())-1)/2;
	if (y >= Y+H) break;
	if (item()->damage()) {
	  if (!clipped) {push_clip(X,Y,W,H); clipped = 1;}
	  int arrow_size = int(textsize())|1;
	  int x = (item_level[HERE]+indented())*arrow_size+X-xposition_;
	  push_matrix();
	  translate(x, y);
	  item()->draw();
	  item()->set_damage(0);
	  pop_matrix();
	}
	if (!next_visible()) break;
      }
    }
    if (clipped) pop_clip();
  }
  fltk::column_widths(last_columns);
  //fltk::column_widths(0);
  scrolldx = scrolldy = 0;
  unset_mark(REDRAW_0);
  unset_mark(REDRAW_1);

  // draw the scrollbars:
  if (d & DAMAGE_ALL) {
    scrollbar.set_damage(DAMAGE_ALL);
    hscrollbar.set_damage(DAMAGE_ALL);
    if (scrollbar.visible() && hscrollbar.visible()) {
      // fill in the little box in the corner
      setcolor(buttoncolor());
      fillrect(scrollbar.x(), hscrollbar.y(), scrollbar.w(), hscrollbar.h());
    }
    if (header_) {
      for (int i=0; i<nHeader; i++) {
        header_[i]->set_damage(DAMAGE_ALL);
      }
    }
  }

  update_child(scrollbar);
  update_child(hscrollbar);

  if (header_) {
    push_clip(box()->dx(), box()->dy(), w()-box()->dw(), header_[0]->h());
    for (int i=0; i<nHeader; i++) {
      update_child(*header_[i]);
    }
    pop_clip();
    
    // Update box in upper-right corner, if necessary
    Widget *hi = header_[nHeader-1];
    int r = hi->x()+hi->w(), rr = X+W+(scrollbar.visible()?scrollbar.w():0);
    if (r<rr) {
      setcolor(buttoncolor());
      fillrect(r, hi->y(), rr-r, hi->h());      
    }
  }  
  Item::clear_style();
}

////////////////////////////////////////////////////////////////
// Scrolling and layout:

/*!  If the current item is a parent, set the open state (the
  fltk::VALUE flags) to the given value and redraw the browser
  correctly. Returns true if the state was actually changed, returns
  false if it was already in that state.  */
bool Browser::set_item_opened(bool open)
{
  if (!item() || !item_is_parent()) return false;
  if (open) {
    if (item()->flags() & VALUE) return false;
    item()->set_flag(VALUE);
  } else {
    if (!(item()->flags() & VALUE)) return false;
    item()->clear_flag(VALUE);
  }
  list()->flags_changed(this, item());
  relayout(LAYOUT_CHILD);
  return true;
}

/*!  Turn off or on the fltk::INVISIBLE flag on the given item and
  redraw the browser if necessary. Returns true if the state was
  actually changed, returns false if it was already in that state.  */
bool Browser::set_item_visible(bool value)
{
  if (!item()) return false;
  if (value) {
    if (item()->visible()) return false;
    item()->set_flag(INVISIBLE);
  } else {
    if (!item()->visible()) return false;
    item()->clear_flag(INVISIBLE);
  }
  list()->flags_changed(this, item());
  if (open_level[HERE] >= item_level[HERE]) relayout(LAYOUT_CHILD);
  return true;
}

void Browser::layout() {
  // This flag is used by relayout() to indicate that autoscroll is needed:
  bool scroll_to_item = layout_damage()&LAYOUT_CHILD;

  // clear the flags first so the other methods know it is ok to measure
  // the widgets:
  Widget::layout();

  Item::set_style(this);
  const int *last_columns = fltk::column_widths();
  fltk::column_widths(column_widths_p);

  // figure out the visible area:
  const int sw = scrollbar_width();

  X = 0; Y = 0; W = w(); H = h();
  box()->inset(X,Y,W,H);
  if (scrollbar.visible()) W -= sw;
  if (hscrollbar.visible()) H -= sw;

  int headerh = 0;
  if (header_) {
    header_[0]->layout();
    headerh = header_[0]->h();
    H -= headerh; Y += headerh;     
  }

  // Measure the height of all items and find widest one
  width_ = 0;

  // count all the items scrolled off the top:
  int arrow_size = int(textsize())|1;
  if (!goto_top()) yposition_ = 0;
  else for (;;) {
    if (item_position[HERE]+item()->height() > yposition_) break;
    //if (!indented_ && item_is_parent()) indented_ = true;
    if (at_mark(FOCUS)) set_mark(FOCUS, HERE);
    int w = item()->width()+arrow_size*item_level[HERE];
    if (w > width_) width_ = w;
    if (!next_visible()) {goto_top(); yposition_ = 0; break;}
  }
  set_mark(FIRST_VISIBLE, HERE);
  // count all the rest of the items:
  if (item()) for (;;) {
    if (at_mark(FOCUS)) set_mark(FOCUS, HERE);
    int w = item()->width()+arrow_size*item_level[HERE];
    if (w > width_) width_ = w;
    //if (!indented_ && item_is_parent()) indented_ = true;
    if (!next_visible()) break;
  }
  if (indented()) width_ += arrow_size;
  height_ = item_position[HERE];

  // Do we have flexible column?
  bool has_flex = false;

  if (header_) {
    // calculate the combined column width and detect flexible column
    int col_width = 0, i;
    for (i=0; i<nHeader; i++) {
      int itemwidth = (i<nColumn)?column_widths_i[i]:0;
      if (itemwidth==0) itemwidth = -1;
      if (itemwidth<0)
        has_flex = true;
      else
        col_width += itemwidth;
    }
    //if (col_width > width_)
    // Always set to headers width
    width_ = col_width;
  }

  // Act as width=0, if we have flexible column so hcrollbar is invisible always
  if (has_flex) width_ = 0;

  // turn the scrollbars on and off as necessary:
  for (int z = 0; z<2; z++) {
    if (height_ > H || yposition_) {
      if (!scrollbar.visible()) {
	scrollbar.set_visible();
	W -= sw;
	redraw(DAMAGE_ALL);
      }
    } else {
      if (scrollbar.visible()) {
	scrollbar.clear_visible();
	W += sw;
	redraw(DAMAGE_ALL);
      }
    }
    if (width_ > W || xposition_) {
      if (!hscrollbar.visible()) {
	hscrollbar.set_visible();
	H -= sw;
	redraw(DAMAGE_ALL);
      }
    } else {
      if (hscrollbar.visible()) {
	hscrollbar.clear_visible();
	H += sw;
	redraw(DAMAGE_ALL);
      }
    }
  }

  // If we have flexible column, set width to W
  if (has_flex) width_ = W;

  if (scrollbar.visible() && scrollbar_align()&ALIGN_LEFT) X += sw;
  if (hscrollbar.visible() && scrollbar_align()&ALIGN_TOP) Y += sw;

  scrollbar.resize(scrollbar_align()&ALIGN_LEFT ? X-sw : X+W, Y, sw, H);
  scrollbar.value(yposition_, H, 0, height_);
  scrollbar.linesize(textsize()+leading());

  hscrollbar.resize(X, scrollbar_align()&ALIGN_TOP ? Y-sw : Y+H, W, sw);
  hscrollbar.value(xposition_, W, 0, width_);
  hscrollbar.linesize(scrollbar.linesize());

  if (header_) {
    // first, calculate the combined column width    
    int width = 0, nflex = 0, i;
    for (i=0; i<nHeader; i++) {
      int itemwidth = (i<nColumn)?column_widths_i[i]:0;
      if (itemwidth==0) itemwidth = -1;
      if (itemwidth<0)
        nflex -= itemwidth;
      else
        width += itemwidth;
    }
    int space = W-width; // number of pixels that will fill the flex columns
    int hx = X;          // current x position for this column
    // now set the actual column widths
    for (i=0; i<nHeader; i++) {
      Widget *hi = header_[i];
      int itemwidth = (i<nColumn)?column_widths_i[i]:0;
      if (itemwidth==0) itemwidth = -1;
      if (itemwidth<0)
        itemwidth = -itemwidth*space/nflex;
      int ww = itemwidth;
      if (ww<0) ww = 0;
      if (column_widths_p)
        column_widths_p[i] = itemwidth;
      hi->resize(-xposition_+hx, Y-headerh, ww, headerh);	  
      hi->layout();
      hx += itemwidth;
    }
  }
  
  layout_damage(0); // resize of scrollbars may have turned this on

  // Now that we got the sizes of everything, scroll to show current item:
  if (scroll_to_item) {
    goto_mark(FOCUS);
    make_item_visible(NOSCROLL);
  }

  redraw(DAMAGE_CONTENTS); // assumme we need to redraw
  fltk::column_widths(last_columns);
  Item::clear_style();
}

void Browser::hscrollbar_cb(Widget* o, void*) {
  ((Browser*)(o->parent()))->xposition(int(((Scrollbar*)o)->value()));
}

/*! Set the horizontal scrolling position, measured in pixels. Zero is
  the normal position where the left edge of the child widgets is visible. */
void Browser::xposition(int X) {
  int dx = xposition_-X;
  if (dx) {
    xposition_ = X; 
    scrolldx += dx; 
    if (nHeader) relayout();
    redraw(DAMAGE_VALUE);
  }
}

void Browser::scrollbar_cb(Widget* o, void*) {
  ((Browser*)(o->parent()))->yposition(int(((Scrollbar*)o)->value()));
}

/*! Set the vertical scrolling position, measured in pixels. Zero means
  the top of the first item is visible. Positive numbers scroll the
  display up. */
void Browser::yposition(int Y) {
  if (Y == yposition_) return;
  ((Slider*)(&scrollbar))->value(Y);
  goto_position(Y);
  set_mark(FIRST_VISIBLE, HERE);
  scrolldy += (yposition_-Y); redraw(DAMAGE_VALUE);
  yposition_ = Y;
}

////////////////////////////////////////////////////////////////
// Event handling

/*! Change the focus (the selected item, or in an fltk::MultiBrowser
  the item that has a dotted box around it, to the current item.
  This calls make_item_visible().  */
bool Browser::set_focus() {
  bool ret = !at_mark(FOCUS);
  if (ret) {
    damage_item(HERE); // so will draw focus box around item
    damage_item(FOCUS); // so focus box around old focus item will be removed
    set_mark(FOCUS, HERE); // current item is new focus item
  }
  // make the item do it's own focus highlighting:
  if (item() && item()->take_focus());
  // otherwise take the focus from any other child:
  else if (contains(fltk::focus())) fltk::focus(this);
  // scroll to show the item:
  make_item_visible();
  return ret;
}

/*! This makes the current item visible to the user.

  First it turns off the fltk::INVISIBLE flag on the current item, and
  turns off the fltk::INVISIBLE flag and opens (turning on the
  fltk::VALUE flag) all parent items. These flag changes cause
  flags_changed() to be called on any fltk::List that you have
  assigned to the browser.

  The browser is then scrolled by calling yposition() so the item is
  visible. The optional argument tells how to scroll. If not specified
  (or the default value of fltk::Browser::NOSCROLL is given) then the
  browser is scrolled as little as possible to show the item. If it is
  fltk::Browser::TOP then the item is put at the top of the
  browser. If it is fltk::Browser::MIDDLE then the item is centered
  vertically in the browser. If it is fltk::Browser::BOTTOM then the
  item is put at the bottom of the browser.

  This does nothing if the current item is null.  
*/
bool Browser::make_item_visible(linepos where) {
  if (!item()) return false;
  bool changed = set_item_visible(true);
  // make any parents open and visible:
  if (open_level[HERE] < item_level[HERE]) {
    for (int n = open_level[HERE]; n < item_level[HERE]; n++) {
      if (item_index[HERE][n] < 0) break;
      int children = this->children(item_index[HERE], n);
      if (item_index[HERE][n] >= children) break;
      Widget* i = child(item_index[HERE], n);
      i->set_visible();
      i->set_value();
      list()->flags_changed(this, item());
    }
    changed = true;
    relayout(LAYOUT_CHILD);
  } else if (!layout_damage()) {
    set_mark(TEMP,HERE);
    int h = item()->height();
    int p = item_position[HERE];
    switch (where) {
    case 0:
      if (p < yposition_) break; // act like TOP
      if (p+h-yposition_ <= H) return changed;
      // fall through:
    case BOTTOM:
      p += h-H; break;
    case MIDDLE:
      p += h-H/2; break;
    case TOP:
      break;
    }
    // clip scrolling to the useful range:
    if (p > height_-H) p = height_-H;
    if (p < 0) p = 0;
    // go there:
    yposition(p);
    goto_mark(TEMP);
  } else {
    // turn on flag indicating that scrolling to item is wanted:
    relayout(LAYOUT_CHILD);
  }
  return changed;
}

/*! This is for use by the MultiBrowser subclass.
  Turn the fltk::VALUE flag on or off in the current item (use
  goto_index() to set the current item before calling this).

  If this is not a MultiBrowser, this does select_only_this()
  if \a value is true, and deselect() if \a value is false.

  If do_callback has some bits that are also in when() then the
  callback is done for each item that changes selected state.
*/
bool Browser::set_item_selected(bool value, int do_callback) {
  if (multi()) {
    //if (value) set_focus();
    if (value) {
      if (item()->selected()) return false;
      item()->set_selected();
    } else {
      if (!item()->selected()) return false;
      item()->clear_selected();
    }
    list()->flags_changed(this, item());
    damage_item(HERE);
    if (when() & do_callback) {
      clear_changed();
      this->do_callback();
    } else if (do_callback) {
      set_changed();
    }
    return true;
  } else {
    if (value) return (select_only_this(do_callback));
    else return deselect(do_callback);
  }
}

/*! Turn off selection of all items in the browser. For the normal
  (not Multi) Browser, this puts it in a special state where nothing
  is highlighted and index(0) returns -1. The user cannot get it into
  this state with the GUI.

  For a MultiBrowser the user can get this state by ctrl+clicking the
  selected items off.

  If \a do_callback has some bits that are also in when() then the
  callback is done for each item that changes selected state.
*/
bool Browser::deselect(int do_callback) {
  unset_mark(HERE); item(0);
  return select_only_this(do_callback);
}

/*! Make the given item be the current one. For the MultiBrowser subclass
  this will turn off selection of all other items and turn it on
  for this one and also set the focus here. If the selection
  changes and when()&do_callback is non-zero, the callback is done. */
bool Browser::select_only_this(int do_callback) {
  if (multi()) {
    set_focus();
    bool ret = false;
    // Turn off all other items and set damage:
    if (goto_top()) do {
      if (set_item_selected(at_mark(FOCUS), do_callback)) ret = true;
    } while (next_visible());
    // turn off any invisible ones:
    nodamage = true;
    if (goto_top()) do {
      if (set_item_selected(at_mark(FOCUS), do_callback)) ret = true;
    } while (next());
    nodamage = false;
    goto_mark(FOCUS);
    return ret;
  } else {
    if (!set_focus()) return false;
    if (when() & do_callback) {
      clear_changed();
      this->do_callback();
    } else if (do_callback) {
      set_changed();
    }
    return true;
  }
}

int Browser::handle(int event) {
  static bool drag_type; // for multibrowser

  switch (event) {
  case fltk::FOCUS:
    if (goto_mark(FOCUS) && item()) item()->take_focus();
  case UNFOCUS:
    damage_item(FOCUS);
    return 1;

  case PUSH:
  case ENTER:
  case MOVE: {
    // For all mouse events check to see if we are in the scrollbar
    // areas and send to them:
    if (scrollbar.visible() &&
        (event_y() >= scrollbar.y()) && 
	(scrollbar_align()&ALIGN_LEFT ?
	 (event_x() < scrollbar.x()+scrollbar.w()) :
	 (event_x() >= scrollbar.x())))
      return scrollbar.send(event);
    if (hscrollbar.visible() &&
	(scrollbar_align()&ALIGN_TOP ?
	 (event_y() < hscrollbar.y()+hscrollbar.h()) :
	 (event_y() >= hscrollbar.y())))
      return hscrollbar.send(event);
    if (header_ && nHeader && event_y()<header_[0]->y()+header_[0]->h()) {
      for (int i=0; i<nHeader; i++) {
        Widget *hi = header_[i];
	if (event_x()>=hi->x() && event_x()<hi->x()+hi->w())
	return hi->send(event);
      }
    }
    // find the item we are pointing at:
    if (!goto_position(event_y()-Y+yposition_) && !item()) return 0;
    // set xx to how far to left of widget they clicked:
    int arrow_size = int(textsize())|1;
    int xx = (item_level[HERE]+indented())*arrow_size+X-xposition_-event_x();
    // see if they are inside the widget and it takes the event:
    if (xx <= 0 && item()->send(event));
    else fltk::belowmouse(this);
    // accept enter/move events so the browser's tooltip appears:
    if (event != PUSH) return 1;
    // drag must start & end in open/close box for it to work:
    openclose_drag = (xx > 0 && xx < arrow_size && item_is_parent());
    if (openclose_drag) {
      set_focus();
      damage_item(HERE);
      return 1;
    }
    //take_focus();
    if (multi()) {
      if (event_state(CTRL)) {
	// start a new selection block without changing state
	drag_type = !item()->selected();
	if (openclose_drag) drag_type = !drag_type; // don't change it
	set_item_selected(drag_type, WHEN_CHANGED);
	set_focus();
	event_clicks(0); // make it not be a double-click for callback
	return 1;
      } else if (event_state(SHIFT)) {
	// extend the current focus
	drag_type = !item()->selected();
	event_clicks(0); // make it not be a double-click for callback
      } else {
	select_only_this(WHEN_CHANGED);
	drag_type = true;
	return 1;
      }
    }
    goto MOUSE_TO_ITEM;}

  case DRAG:
    // find the item they are now pointing at:
    if (!goto_position(event_y()-Y+yposition_) && !item()) break;
    if (openclose_drag) {
      set_focus();
      // set xx to how far to left of widget they clicked:
      int arrow_size = int(textsize())|1;
      int xx = (item_level[HERE]+indented())*arrow_size+X-xposition_-event_x();
      if (xx > 0 && xx < arrow_size && item_is_parent()) {
	if (openclose_drag != 1) {openclose_drag = 1; damage_item(HERE);}
      } else {
	if (openclose_drag == 1) {openclose_drag = 2; damage_item(HERE);}
      }
      return 1;
    }
  MOUSE_TO_ITEM:
    if (multi()) {
      // set everything from old focus to current to drag_type:
      int direction = compare_marks(HERE,FOCUS);
      set_mark(TEMP, HERE);
      for (;;) {
	set_item_selected(drag_type, WHEN_CHANGED);
	if (at_mark(FOCUS)) break;
	if (!(direction<0 ? next_visible() : previous_visible())) break;
      }
      goto_mark(TEMP);
      set_focus();
    } else {
      select_only_this(WHEN_CHANGED);
    }
    return 1;

  case RELEASE:
    goto_mark(FOCUS);
    if (openclose_drag == 1 || event_clicks() && item_is_parent()) {
      // toggle the open/close state of this item:
      set_item_opened(!(item()->flags()&VALUE));
      event_is_click(0); // make next click not be double
      if (when()&WHEN_CHANGED) do_callback();
      return 1;
    } else if (openclose_drag) return 1;
    if (event_clicks()) {
      // double clicks act like ReturnKey
      //e_keysym = ReturnKey; // fltk1 did this
      clear_changed();
      execute(item());
      return 1;
    }
    goto RELEASE;

  case KEY:
    event_clicks(0); // make program not think it is a double-click
    switch (event_key()) {
    case RightKey:
      if (goto_visible_focus())
	{set_item_opened(true); next_visible();}
      goto AFTER_MOVEMENT_KEY;
    case LeftKey:
      if (goto_visible_focus())
	if (!set_item_opened(false)) {previous_visible(); set_item_opened(false);}
      goto AFTER_MOVEMENT_KEY;
    case UpKey:
      if (goto_visible_focus()) previous_visible();
      goto AFTER_MOVEMENT_KEY;
    case DownKey:
      if (goto_visible_focus()) next_visible();
    AFTER_MOVEMENT_KEY:
      if (!item()) return 1;
      if (multi() && event_state(SHIFT|CTRL)) {
	if (event_state(SHIFT)) set_item_selected(1,WHEN_CHANGED);
	set_focus();
      } else {
	bool did_callback = when()&WHEN_CHANGED;
        select_only_this(WHEN_CHANGED);
	// allow selection of item to destroy the browser:
	if (did_callback) return 1;
      }
      goto RELEASE;
    case SpaceKey:
      if (!multi() || !goto_visible_focus()) break;
      set_item_selected(!item()->selected(), WHEN_CHANGED);
    RELEASE:
      if ((when()&WHEN_RELEASE) &&
	  (changed() || (when()&WHEN_NOT_CHANGED))) {
	clear_changed();
	do_callback();
      }
      return 1;
    case ReturnKey:
    case KeypadEnter:
      if (!(when() & WHEN_ENTER_KEY)) break;
      if (!goto_visible_focus()) break;
      clear_changed();
      execute(item());
      return 1;
    default:
      if (scrollbar.send(event)) return 1;
      if (hscrollbar.send(event)) return 1;
    }
    break;

  case MOUSEWHEEL:
    return scrollbar.send(event);
#if 0
    int n = event_dy() * Style::wheel_scroll_lines;
    goto_mark(FIRST_VISIBLE);
    while (n > 0 && previous_visible()) n--;
    while (n < 0 && next_visible()) n++;
    set_top();
    return 1;
#endif
  }
  return 0;
}

/*! Go to the focus if it is visible and return it.
  If it is not visible, go to the top of the visible region and return
  zero. This is used by keystrokes so the browser does not scroll
  unexpectedly. */
Widget* Browser::goto_visible_focus() {
  if (item_position[FOCUS] >= yposition_ &&
      item_position[FOCUS] <= yposition_+H) {
    if (goto_mark(FOCUS)) return item();
  }
  if (goto_mark(FIRST_VISIBLE)) {
    if (item_position[HERE] < yposition_) next_visible();
    if (item()) return 0;
  }
  goto_top();
  return 0;
}

/*! Go to a nested item. indexes must contain level+1 index
  numbers. The first number indicates the top-level item number, the
  second indicates the child number of that parent, and so on. This
  sets the current item() to the given item and also returns it. If
  the values are out of range then null is returned.

  A negative number in indexes[0] will make it go into a special
  no-item state where select_only_this() will do deselect().
*/
Widget* Browser::goto_index(const int* indexes, int level) {
  // negative numbers make nothing be selected:
  if (indexes[0] < 0) {
    item_index[HERE][0] = -1;
    item(0);
    return 0;
  }
  // go to the 0'th item if needed (otherwise go to the focus):
  if (!indexes[0] && !level || layout_damage() || !goto_mark(FOCUS)) {
    item_level[HERE] = 0;
    open_level[HERE] = 0;
    item_position[HERE] = 0;
    item_index[HERE][0] = 0;
    siblings = children(item_index[HERE],0);
    if (siblings <= 0) {item(0); return 0;}// empty browser
    item(child(item_index[HERE],0));
    // quit if this is correct:
    if (!level && !indexes[0]) return item();
  } else {
    // move from the focus backwards until we are before it:
    while (::compare_marks(item_index[HERE],item_level[HERE],indexes,level)>0)
      if (!previous_visible()) {goto_index(0); break;}
  }
  // move forward until we are after it:
  for (;;) {
    int n = ::compare_marks(item_index[HERE],item_level[HERE],indexes,level);
    if (!n) return item(); // we found it!
    if (n > 0 || !next_visible()) break;
  }
  // if we get here we passed the item because it was invisible or a
  // parent was closed or invisible, directly set the indexes in this case:
  set_level(level);
  for (int n = 0; n <= level; n++) item_index[HERE][n] = indexes[n];
  item(child(item_index[HERE], item_level[HERE]));
  return item();
}

/*! Go to the i'th item in the top level. If i is out of range null is
  returned. */
Widget* Browser::goto_index(int i) {
  return goto_index(&i,0);
}

/*! Go to an item at any level up to 5. Negative numbers indicate that
  no more levels should be looked at. */
Widget* Browser::goto_index(int a, int b, int c, int d, int e) {
  int indexes[6];
  int i = 0;
  indexes[0] = a;
  if (b>=0) {
    indexes[++i] = b;
    if (c>=0) {
      indexes[++i] = b;
      if (d>=0) {
	indexes[++i] = d;
	if (e>=0) {
	  indexes[++i] = e;
	}
      }
    }
  }
  return goto_index(indexes,i);
}

/*!  Sets the horizontal locations that each '\\t' character in an item
  should start printing text at. These are measured from the left edge
  of the browser, including any area for the open/close + glyphs.  

  \li Array must end with 0 (zero) always
  
  \li You can define flexible column by setting column width to -1.
      If you have flexible column in browser, all columns are resized to 
      match width of the browser, by resizing flexible column.

\code
  // Example 1: make three columns, total width of columns is 300 pixels.
  // Columns are resizable, but total width is kept always.
  const int widths[]   = { 100, 100, 100, 0 };

  // Example 2: make three columns, total width of columns is always width of the browser.
  // Columns are resizable, third column is flexible and will take remaining space left.
  const int widths[]   = { 100, 100, -1, 0 };
\endcode
  */
void Browser::column_widths(const int *t) {
  column_widths_ = t;
  int pnc = nColumn;
  nColumn = 0;
  // count the number of columns, end with 0 or <-1 , -1 is used for flexible column
  if (t) while (*t++!=0) nColumn++;
  if (nColumn==0) {
    // free the column memory
    if (column_widths_p) free(column_widths_p);
    column_widths_p = 0;
    if (column_widths_i) free(column_widths_i);
    column_widths_i = 0;
  } else {
    // reallocate the column storage if needed
    if (nColumn>pnc) {
      column_widths_p = (int*)realloc(column_widths_p, (nColumn+1)*sizeof(int));
      column_widths_i = (int*)realloc(column_widths_i, (nColumn+1)*sizeof(int));
    }
    // copy the widths over into the new array
    memcpy(column_widths_p, column_widths_, (nColumn+1)*sizeof(int));
    memcpy(column_widths_i, column_widths_, (nColumn+1)*sizeof(int));
  }
  // now recalculate the layout
  relayout();
  redraw();
}

int Browser::set_column_start(int col, int x) {
  // we must adjust all this column and the column to the left so that the
  // resulting edge ends at x
  if (col<=0) return -1; // we don't adjust the first column
  if (col>nColumn) return -1; // out of bounds

  bool has_flex = false;
  // find the current column x and calculate the desired delta
  int ox = 0;
  for (int i=0; i<col; i++) { 
    ox += column_widths_p[i];
    if (column_widths_i[i]==-1) has_flex = true;
  }
  int dx = x + xposition_ - ox;
  int cwp, cwi;

  if (col==nColumn) {
    // Cant tesize last column, if we have flexible column
    int *p=column_widths_i; while (*p!=0) { if (*p++==-1) return 0; }
    // Resize last column.
    if (column_widths_p[col]==-1) return 0;
    if (column_widths_p[col-1]+dx<4) dx = -column_widths_p[col-1]+4;
    column_widths_i[col-1] += dx;
    goto RETURN;
  }

  // make sure that no column is smaller than 4 pixels (to the left)
  if (column_widths_p[col-1]+dx<4) {
    dx = -column_widths_p[col-1]+4;
  }
  // make sure that no column is smaller than 4 pixels (to the right)
  cwp = column_widths_p[col], cwi = column_widths_i[col];
  if (cwi>0 && cwi<cwp) cwp = cwi;
  if (cwp-dx<4) {
    dx = cwp-4;
  }
  // now adjust the columns in the interactive field
  if (column_widths_i[col-1]>0) column_widths_i[col-1] += dx;
  if (has_flex) { column_widths_i[col] -= dx; }

RETURN:
  // finally recalculate the layout
  relayout();
  redraw();
  return dx;
}

void Browser::column_click_cb_(Widget *ww, void *d) {
  Browser *w = (Browser*)(ww->parent());
  w->selected_column_ = (int)d;
  w->do_callback();
  w->selected_column_ = -1;
}
#include <fltk/Font.h>
class BButton : public Button {
  uchar sides; // bit 0 set: user can drag left side, bit 1: right side
public:
  BButton(uchar s, const char *l=0) 
  : Button(0,0,0,0, l) 
  { 
    sides = s;
    align(ALIGN_INSIDE|ALIGN_CLIP);
  }
  int handle(int event) {
    static int ox = -1;
    static bool left = true;
    static bool enter_before_leave = false;
    switch (event) {
      case fltk::LEAVE: 
        if (!enter_before_leave) cursor(fltk::CURSOR_DEFAULT);
        enter_before_leave = false;
        break;
      case fltk::ENTER:
        enter_before_leave = true;
        // fall through
      case fltk::MOVE:
        if (sides&1 && fltk::event_x()<=2) {
          cursor(fltk::CURSOR_WE);
        } else if (sides&2 && fltk::event_x()>=w()-2) {
          cursor(fltk::CURSOR_WE);
        } else {
          cursor(fltk::CURSOR_DEFAULT);
        }
        if (event==fltk::ENTER) { Button::handle(event); return 1; }
        break;
      case fltk::PUSH:
        if (sides&1 && fltk::event_x()<=2) {
          left = true;
          ox = fltk::event_x_root() - x();
          return 1;
        } else if (sides&2 && fltk::event_x()>=w()-2) {
          left = false;
          ox = fltk::event_x_root() - x() - w();
          return 1;
        }
        break;
      case fltk::DRAG: {
        if (ox==-1) break;
        Browser *w = (Browser*)(parent());
        int col = (int)user_data();
        if (left)
          w->set_column_start(col, fltk::event_x_root()-ox);
        else
          w->set_column_start(col+1, fltk::event_x_root()-ox);
        return 1; }
      case fltk::RELEASE:
        if (ox==-1) break;
        ox = -1;
        return 1;
    }
    return Button::handle(event);
  }

  void layout() {
    setfont(labelfont(),labelsize());
    h(int(getascent()+getdescent()+leading()+box()->dh()));
    Button::layout();
  }
};

/*! Set an array of labels to put at the top of the browser. The initial
  sizes of them are set with column_widths(). Items in the browser can
  print into the correct columns by putting '\\t' characters into
  their text. Or they can look at fltk::column_widths() to find
  the settings from their draw() methods.
*/
void Browser::column_labels(const char **t) {
  column_labels_ = t;
  int i;
  if (header_) {
    for (i=0; i<nHeader; i++) delete header_[i];  
    delete[] header_;
  }
  nHeader = 0; header_ = 0;
  if (t) { // create new header_ widgets
    Group *g = Group::current();
    Group::current(0);
    while (*t++) nHeader++;
    header_ = new Widget*[nHeader];
    for (i=0; i<nHeader; i++) {
      uchar sides = 0;
      if (i>0) sides |= 1;
      if (i<nHeader) sides |= 2;
      header_[i] = new BButton(sides, column_labels_[i]);
      header_[i]->parent(this);
      header_[i]->callback(column_click_cb_, (void*)i);
    }
    Group::current(g);
  }
  layout();
}

/*! \fn int Browser::selected_column()
  It will call the callback() if the user clicks on a column title.
  Check this to see which one they clicked. This will return a
  negative number if the callback is being done for some other
  reason, such as the user clicking on an item.
*/

////////////////////////////////////////////////////////////////
// Fltk 1.0 emulation
// Items are numbered and children of top-level items are ignored.
// In 1.0 the item numbers started at 1, I changed this to zero so
// this matches Menu.

/*! Same as goto_index(line),set_item_selected(value), to change the
  selected state of an item in a non-hierarchial
  MultiBrowser. If \a line is out of range nothing happens. */
bool Browser::select(int line, bool value) {
  if (!goto_index(line)) return false;
  return set_item_selected(value, false);
}

/*! Does goto_index(line),item_selected() to return the selection
  state of an item in a non-hierarchial MultiBrowser. If \a line
  is out of range it returns false.
*/
bool Browser::selected(int line) {
  if (!goto_index(line)) return false;
  return item()->selected();
}

/*! Convenience function for non-hierarchial browsers.
  Returns true if the indexed item is visible (ie not hidden).
*/
bool Browser::displayed(int line) {
  if (!goto_index(line)) return false;
  return item()->visible();
}

/*! Convenience function for non-hierarchial browsers. Make the
  indexed item be visible() and scroll the browser so it can be seen
  by the user. */
bool Browser::display(int line, bool value) {
  if (!goto_index(line)) return false;
  return set_item_visible(value);
}

/*! \fn int Browser::topline() const
  Convenience function for non-hierarchial browsers.
  Returns the index if the top-level item that is at the top of the
  scrolling window. */

/*! \fn void Browser::topline(int line)
  Convenience function for non-hierarchial browsers.
  Make the indexed item visible and scroll to put it at the top of
  the browser. */

/*! \fn void Browser::middleline(int line)
  Convenience function for non-hierarchial browsers.
  Make the indexed item visible and scroll to put it in the middle
  of the browser if it is not already visible (passes NO_SCROLL
  to make_item_visible(). */

/*! \fn void Browser::bottomline(int line)
  Convenience function for non-hierarchial browsers.
  Make the indexed item visible and scroll to put it at the bottom of
  the browser. */

////////////////////////////////////////////////////////////////
// Constructor

#define SLIDER_WIDTH 16

static void revert(Style* s) {
  s->glyph_ = ::browser_glyph;
}
static NamedStyle style("Browser", revert, &Browser::default_style);
/*! This style mostly serves to set the parenting back to
  Widget::default_style to avoid the gray color and larger leading
  set by Menu::default_style. However it also sets it's own glyph()
  function to one that draws the [+] and [-] indicators.
*/
NamedStyle* Browser::default_style = &::style;

static int nogroup(int x) {Group::current(0); return x;}

/*! The constructor makes an empty browser. */
Browser::Browser(int X,int Y,int W,int H,const char* L)
  : Menu(X,Y,W,H,L),
    scrollbar(nogroup(X+W-SLIDER_WIDTH),Y,SLIDER_WIDTH,H-SLIDER_WIDTH),
    hscrollbar(X,Y+H-SLIDER_WIDTH,W-SLIDER_WIDTH,SLIDER_WIDTH) {
  set_click_to_focus();
  style(default_style);
  xposition_ = 0;
  yposition_ = 0;
  scrolldx = scrolldy = 0;
  hscrollbar.parent(this);
  hscrollbar.callback(hscrollbar_cb);
  scrollbar.set_vertical();
  scrollbar.parent(this);
  scrollbar.callback(scrollbar_cb);
  indented_ = 0;
  column_widths_ = 0;
  column_widths_p = 0;
  column_widths_i = 0;
  column_labels_ = 0;
  selected_column_ = -1;
  nColumn = 0;
  nHeader = 0; header_ = 0;
  // set all the marks to the top:
  levels = 0;
  for (int i = 0; i < NUMMARKS; i++) {
    // allocate space for the top level of indexes
    item_index[i] = (int*)malloc(sizeof(int));
    item_index[i][0] = 0;
    item_position[i] = 0;
    item_level[i] = 0;
  }
  Group::current(parent());
}

/*! The destructor deletes all the list items (because they are child
  fltk::Widgets of an fltk::Group) and destroys the browser. */
Browser::~Browser() {
  int i; for (i = 0; i < NUMMARKS; i++) free(item_index[i]);
  if (column_widths_p) free(column_widths_p);
  if (column_widths_i) free(column_widths_i);
  if (header_) {
    for (i=0; i<nHeader; i++) delete header_[i];  
    delete[] header_;
  }
}

////////////////////////////////////////////////////////////////

/*! \class fltk::MultiBrowser

  The fltk::MultiBrowser class is a subclass of fltk::Browser which
  lets the user select any set of the lines. Clicking on an item
  selects only that one. Ctrl+click toggles items on/off. Shift+drag
  (or shift+arrows) will extend selections. Normally the callback is
  done when any item changes it's state, but you can change this with
  when().

  See fltk::Browser for methods to control the display and "current
  item", and fltk::Menu for methods to add and remove lines from the
  browser.

  The methods on fltk::Browser for controlling the "value" control which
  item has the keyboard focus in a multi-browser. You must use the
  "select" methods described here to change what items are turned on:
  - set_item_selected()
  - select_only_this()
  - deselect()
  - select()
  - selected()

*/

//
// End of "$Id: Fl_Browser.cxx,v 1.102 2004/11/12 06:50:14 spitzak Exp $".
//
