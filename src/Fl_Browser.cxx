//
// "$Id: Fl_Browser.cxx,v 1.43 2001/07/23 09:50:04 spitzak Exp $"
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

// This file is temporary, it will completely replace the current
// Fl_Browser.cxx and Fl_Browser_.cxx

#include <fltk/Fl.h>
#include <fltk/Fl_Browser.h>
#include <fltk/fl_draw.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////
// Compile-time style configuration options.
// For "run time configuration" copy this file, rename the widget,
// compile with your desired options, and use your renamed widget.
// Deal with it.

#define DRAW_STRIPES 0 // draw Macintosh-style stripes
#define TRIANGLE_GLYPH 0 // use Macintosh/fltk-style glyphs

////////////////////////////////////////////////////////////////
// Moving between items:

// A "mark" is like a pointer to a widget somewhere in the hierarchy
// of the Browser. Actually it is an array of child indicies, and also
// the vertical position and index number (used to decide stripe color)
// of the item.

// The Fl_Browser has space to store a fixed number of marks.

// The "current item" is the one pointed to by the mark called HERE.
// A pointer to it is stored in item(). To allow Fl_List to work, this
// pointer is only valid until the next item is referenced, so don't
// copy it to any storage. You can move the current item forward and
// backward, skipping invisible items.

// this just makes things more readable
#define TOPLEVEL 0

// This function increases the number of levels we can store in each
// mark, and sets the level of the current mark to n:
void Fl_Browser::set_level(int n) {
  if (n > levels) {
    for (int i = 0; i < NUMMARKS; i++) {
      item_index[i]=(int*)realloc((void*)item_index[i],(n+1)*sizeof(int));
    }
    levels = n;
  }
  item_level[HERE] = n;
}

// set the current item to the first visible item:
Fl_Widget* Fl_Browser::goto_top() {
  item_level[HERE] = 0;
  item_position[HERE] = 0;
  item_index[HERE][0] = 0;
  if (children(item_index[HERE],0) <= 0) {
    // empty browser must return a null widget
    item(0);
    return 0;
  }
  item(child(item_index[HERE],0));
  // skip leading invisible widgets:
  if (!item()->visible()) return next_visible();
  return item();
}

// set current item to a particular mark
Fl_Widget* Fl_Browser::goto_mark(int mark) {
  item_position[HERE] = item_position[mark];
  item_level[HERE] = item_level[mark];
  for (int L = 0; L <= item_level[HERE]; L++)
    item_index[HERE][L] = item_index[mark][L];
  item(child(item_index[HERE], item_level[HERE]));
  return item();
}

// copy one mark to another
void Fl_Browser::set_mark(int dest_mark, int src_mark) {
  item_position[dest_mark] = item_position[src_mark];
  item_level[dest_mark] = item_level[src_mark];
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
      return -9; // mark1 is on a parent of mark2
    } else if (L > L2) {
      return +9; // mark2 is on a parent of mark1
    }
    int n = index1[L] - index2[L];
    if (n) return n;
  }
}

int Fl_Browser::compare_marks(int mark1, int mark2) {
  return ::compare_marks(item_index[mark1], item_level[mark1],
			 item_index[mark2], item_level[mark2]);
}

// unsets this mark in toplevel
void Fl_Browser::unset_mark(int mark) {
  item_index[mark][0] = -1;
}

// is this toplevel mark set?
bool Fl_Browser::is_set(int mark) {
  return item_index[mark][0] >= 0;
}

// Move forward to the next visible item (what down-arrow does).
// This does not move and returns null if we are at the bottom
Fl_Widget* Fl_Browser::next_visible() {
  if (item()->visible()) item_position[HERE] += item()->height();

  // If we are on an open group title with children, go to first item in group
  if (item()->flags()&FL_OPEN && item()->visible()) {
    set_level(item_level[HERE]+1);
    item_index[HERE][item_level[HERE]] = 0;
  } else {
    // go to next item in this group
    item_index[HERE][item_level[HERE]] ++;
  }

  // CET - FIXME - is this loop logic correct?
  // loop to find the next real item:
  for (;;) {

    item(child(item_index[HERE], item_level[HERE]));

    if (!item()) {
      // we moved off the end of a group
      if (!item_level[HERE]) return 0; // end of the entire browser
      item_level[HERE]--;
      item_index[HERE][item_level[HERE]] ++;
      continue;
    }

    // skip invisible items:
    if (item()->visible()) break;
    item_index[HERE][item_level[HERE]] ++;
  }

  return item();
}

// Move backward to previous visible item:
// This does not move and returns null if we are at the top.
Fl_Widget* Fl_Browser::previous_visible() {

  // there's got to be simpler logic for this loop...
  for (;;) {
    // go up to parent from first item in a group:
    if (!item_index[HERE][item_level[HERE]]) {
      if (!item_level[HERE]) { // start of browser
	item_position[HERE] = 0;
	return 0;
      }
      item_level[HERE]--;
      item(child(item_index[HERE], item_level[HERE]));
      break;
    }

    // go back to previous item in this group:
    item_index[HERE][item_level[HERE]] --;
    item(child(item_index[HERE], item_level[HERE]));

    // go to last child in a group:
    while (item()->flags()&FL_OPEN && item()->visible()) {
      int n = children(item_index[HERE], item_level[HERE]+1);
      if (n <= 0) break; // the group is empty, remain on it's title
      set_level(item_level[HERE]+1);
      item_index[HERE][item_level[HERE]] = n-1;
      item(child(item_index[HERE], item_level[HERE]));
    }

    if (item()->visible()) break;
  }

  item_position[HERE] -= item()->height();
  return item();
}

// Move to the next item even if invisible. This will walk the entire
// tree. The vertical position is NOT set!
// Move forward to the next visible item (what down-arrow does).
Fl_Widget* Fl_Browser::next() {
  if (children(item_index[HERE], item_level[HERE]+1) > 0) {
    set_level(item_level[HERE]+1);
    item_index[HERE][item_level[HERE]] = 0;
  } else {
    item_index[HERE][item_level[HERE]]++;
  }
  for (;;) {
    if (item(child(item_index[HERE], item_level[HERE]))) return item();
    if (item_level[HERE] <= 0) return 0;
    item_level[HERE]--;
    item_index[HERE][item_level[HERE]]++;
  }
}

// set current item to one at or before Y pixels from top of browser
Fl_Widget* Fl_Browser::goto_position(int Y) {
  if (Y < 0) Y = 0;
  if (damage()&FL_DAMAGE_LAYOUT ||
      Y<=yposition_/2 || !goto_mark(FIRST_VISIBLE)) {
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

// set item referenced by this mark as being damaged:
void Fl_Browser::damage_item(int mark) {
  if (!compare_marks(REDRAW_0, mark) || !compare_marks(REDRAW_1, mark))
    return;
  int m = REDRAW_0;
  if (is_set(m)) {
    m = REDRAW_1;
    // if both marks are used we give up and damage the whole thing:
    if (is_set(m)) {damage(FL_DAMAGE_EXPOSE); return;}
  }
  set_mark(m, mark);
  damage(FL_DAMAGE_SCROLL);
}

////////////////////////////////////////////////////////////////
// Drawing

// Glyphs that are drawn to the left of items.  Enumeration order
// is important!
enum {
  NONE, BAR, ELL, TEE, CLOSED_ELL, CLOSED_TEE, OPEN_ELL, OPEN_TEE
};

#include <fltk/Fl_Bitmap.h>
#include "browser_plus.xbm"
static Fl_Bitmap browser_plus(browser_plus_bits, browser_plus_width, browser_plus_height);
#include "browser_minus.xbm"
static Fl_Bitmap browser_minus(browser_minus_bits, browser_minus_width, browser_minus_height);

static void
glyph(const Fl_Widget* widget, int glyph, int x,int y,int w,int h, Fl_Flags f)
{
  // fl_color((Fl_Color)9); fl_rect(x,y,w,h); // draw boundary for testing
  fl_color(widget->get_glyph_color(f|FL_INACTIVE));
  int lx = x+w/2;
  int ly = y+(h-1)/2;
  switch (glyph) {
  case NONE:
    break;
  case BAR:
    fl_line(lx, y, lx, y+h-1);
    break;
  case ELL:
    fl_line(lx, y, lx, ly);
    fl_line(lx, ly, x+w, ly);
    break;
  case TEE:
    fl_line(lx, y, lx, y+h-1);
    fl_line(lx, ly, x+w, ly);
    break;
#if TRIANGLE_GLYPH
  case CLOSED_TEE:
  case OPEN_TEE:
    fl_line(lx, y, lx, y+h-1);
    goto J1;
  case CLOSED_ELL:
  case OPEN_ELL:
    fl_yxline(lx, y, lx, ly);
  J1:
    fl_glyph(widget, glyph < OPEN_ELL ? FL_GLYPH_RIGHT : FL_GLYPH_DOWN,
	     x, y, w, h, f);
    break;
#else
  default: {
    Fl_Bitmap* image = (glyph < OPEN_ELL) ? &browser_plus : &browser_minus;
    const int boxsize = browser_plus_width/2;
    image->draw(lx-boxsize, ly-boxsize);
    fl_line(lx, y, lx, ly-boxsize);
    if (glyph&1) fl_line(lx, ly+boxsize, lx, y+h-1);
    fl_line(lx+boxsize, ly, x+w, ly);
    }
#endif
  }
}

// this is non-zero if a drag was started in a group open/close box:
static char openclose_drag;

// Draws the current item:
void Fl_Browser::draw_item() {

  Fl_Widget* widget = item();

  int y = Y+item_position[HERE]-yposition_;
  int h = widget->height();

  Fl_Flags flags;

  int is_focus = at_mark(FOCUS);

  if (multi() ? widget->value() : is_focus) {
    fl_color(selection_color());
    fl_rectf(X, y, W, h);
    widget->set_flag(FL_SELECTED);
    flags = FL_SELECTED;
  } else {
    widget->clear_flag(FL_SELECTED);
    flags = 0;
#if DRAW_STRIPES
    Fl_Color c0 = text_background();
    Fl_Color c1 = color();
    int x=1; for (int j=0; j<=item_level[HERE]; j++) x ^= item_index[HERE][j]+1;
    if (x & 1 && c1 != c0) {
      // draw odd-numbered items with a dark stripe, plus contrast-enhancing
      // pixel rows on top and bottom:
      fl_color(c1);
      fl_rectf(X, y+1, W, h-2);
      fl_color(c0 <= FL_LIGHT1 ? c1 : Fl_Color(FL_LIGHT1));
      //fl_color(fl_color_average(c1, c0, 1.9));
      fl_line(X, y, X+W, y);
      fl_line(X, y+h-1, X+W, y+h-1);
    } else {
      fl_color(c0);
      fl_rectf(X, y, W, h);
    }
#else
    fl_color(text_background());
    fl_rectf(X, y, W, h);
#endif
  }

  int arrow_size = fl_height(text_font(), text_size())|1;
  int preview_open =
    (openclose_drag == 1 && pushed() && at_mark(FOCUS)) ? FL_OPEN : 0;
  widget->invert_flag(preview_open);

  int x = X-xposition_;
  // draw the glyphs, one for each nesting level:
  for (int j = indented() ? 0 : 1; j <= item_level[HERE]; j++) {
    int g = item_index[HERE][j] < children(item_index[HERE],j) - 1;
    if (j == item_level[HERE]) {
      g += ELL;
      if (indented()) {
	if (widget->flags() & FL_OPEN)
	  g += OPEN_ELL-ELL;
	else if (children(item_index[HERE],j+1)>=0)
	  g += CLOSED_ELL-ELL;
      }
    }
    draw_glyph(g, x, y, arrow_size, h, flags);
    x += arrow_size;
  }

  if (focused() && is_focus) {
    fl_color(get_glyph_color(flags));
    fl_line_style(FL_DASH);
    int w = widget->width();
    if (x + w > X+W) {
      // X bug?  clipped dashed rectangles don't draw exactly correct...
      fl_line(x, y, X+W, y);
      fl_line(x, y, x, y+h-1);
      fl_line(x, y+h-1, X+W, y+h-1);
    } else {
      fl_rect(x, y, w, h);
    }
    fl_line_style(0);
  }
  int save_x = fl_x_; fl_x_ += x;
  int save_y = fl_y_; fl_y_ += y+(leading()+1)/2-1;
  int save_w = widget->w(); widget->w(X+W-x);
  widget->draw();
  widget->w(save_w);
  fl_y_ = save_y;
  fl_x_ = save_x;

  widget->invert_flag(preview_open);
}

void Fl_Browser::draw_clip_cb(void* v,int X, int Y, int W, int H) {
  ((Fl_Browser*)v)->draw_clip(X,Y,W,H);
}

void Fl_Browser::draw_clip(int x, int y, int w, int h) {
  fl_push_clip(x,y,w,h);

  int draw_all = damage() & (FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE|FL_DAMAGE_LAYOUT);
  if (goto_mark(FIRST_VISIBLE)) for (;;) {
    int item_y = Y+item_position[HERE]-yposition_;
    if (item_y >= y+h) break;
    if (draw_all || !at_mark(REDRAW_0) && !at_mark(REDRAW_1)) draw_item();
    if (!next_visible()) break;
  }

  // erase the area below the last item:
  int bottom_y = Y+item_position[HERE]-yposition_;
  if (bottom_y < y+h) {
    fl_color(text_background());
    fl_rectf(x, bottom_y, w, y+h-bottom_y);
  }
  fl_pop_clip();
}

//#include <stdio.h>

void Fl_Browser::draw() {
  uchar d = damage();
  if (d & FL_DAMAGE_ALL) { // full redraw
    //printf("full redraw damage %x\n", d);
    draw_text_frame();
    draw_clip(X, Y, W, H);
  } else if (d & (FL_DAMAGE_EXPOSE|FL_DAMAGE_LAYOUT)) { // redraw contents
    //printf("contents redraw damage %x\n", d);
    draw_clip(X, Y, W, H);
  } else { // minimal update
    //printf("minimal redraw damage %x\n", d);
    if (scrolldx || scrolldy) {
      fl_scroll(X, Y, W, H, scrolldx, scrolldy, draw_clip_cb, this);
    }
    int clipped = 0;
    for (int n = REDRAW_0; n <= REDRAW_1; n++) {
      if (goto_mark(n)) {
	if (!clipped) {fl_push_clip(X,Y,W,H); clipped = 1;}
	draw_item();
      }
    }
    if (clipped) fl_pop_clip();
  }
  scrolldx = scrolldy = 0;
  unset_mark(REDRAW_0);
  unset_mark(REDRAW_1);

  // draw the scrollbars:
  if (d & FL_DAMAGE_ALL) {
    scrollbar.set_damage(FL_DAMAGE_ALL);
    hscrollbar.set_damage(FL_DAMAGE_ALL);
    if (scrollbar.visible() && hscrollbar.visible()) {
      // fill in the little box in the corner
      fl_color(parent()->color());
      fl_rectf(scrollbar.x(), hscrollbar.y(), scrollbar.w(), hscrollbar.h());
    }
  }
  update_child(scrollbar);
  update_child(hscrollbar);
}

////////////////////////////////////////////////////////////////
// Scrolling and layout:

void Fl_Browser::layout() {
  // figure out the visible area:

  X = 0; Y = 0; W = w(); H = h();
  text_box()->inset(X,Y,W,H);
  if (scrollbar.visible()) {
    W -= scrollbar.w();
    if (scrollbar.flags() & FL_ALIGN_LEFT) X += scrollbar.w();
  }
  if (hscrollbar.visible()) {
    H -= hscrollbar.h();
    if (scrollbar.flags() & FL_ALIGN_TOP) Y += hscrollbar.h();
  }

  // Measure the height of all items and find widest one
  last_max_width = 0;

  // count all the items scrolled off the top:
  int arrow_size = fl_height(text_font(), text_size())|1;
  if (!goto_top()) yposition_ = 0;
  else for (;;) {
    if (item_position[HERE]+item()->height() > yposition_) break;
    if (at_mark(FOCUS)) set_mark(FOCUS, HERE);
    int w = item()->width()+arrow_size*item_level[HERE];
    if (w > last_max_width) last_max_width = w;
    if (!next_visible()) {goto_top(); yposition_ = 0; break;}
  }
  set_mark(FIRST_VISIBLE, HERE);
  // count all the rest of the items:
  last_num_lines = 0;
  if (item()) for (;;) {
    if (at_mark(FOCUS)) set_mark(FOCUS, HERE);
    int w = item()->width()+arrow_size*item_level[HERE];
    if (w > last_max_width) last_max_width = w;
    if (item_position[HERE] < H) last_num_lines++;
    if (!next_visible()) break;
  }
  if (indented()) last_max_width += arrow_size; // optional if no groups!

  last_height = item_position[HERE];

  // turn the scrollbars on and off as necessary:

  for (int z = 0; z<2; z++) {
    if ((type()&VERTICAL) && (type()&ALWAYS_ON || last_height > H || yposition_)) {
      if (!scrollbar.visible()) {
	scrollbar.set_visible();
	W -= scrollbar.w();
	damage(FL_DAMAGE_ALL);
      }
    } else {
      if (scrollbar.visible()) {
	scrollbar.clear_visible();
	W += scrollbar.w();
	damage(FL_DAMAGE_ALL);
      }
    }
    if ((type()&HORIZONTAL) && (type()&ALWAYS_ON || last_max_width > W || xposition_)) {
      if (!hscrollbar.visible()) {
	hscrollbar.set_visible();
	H -= hscrollbar.h();
	damage(FL_DAMAGE_ALL);
      }
    } else {
      if (hscrollbar.visible()) {
	hscrollbar.clear_visible();
	H += hscrollbar.h();
	damage(FL_DAMAGE_ALL);
      }
    }
  }

  scrollbar.resize(scrollbar.flags()&FL_ALIGN_LEFT ? X-scrollbar.w() : X+W,
		   Y, scrollbar.w(), H);
  scrollbar.value(yposition_, H, 0, last_height);
  scrollbar.linesize(fl_height(text_font(), text_size())+leading());
  hscrollbar.resize(X,
		    scrollbar.flags()&FL_ALIGN_TOP ? Y-hscrollbar.h() : Y+H,
		    W, hscrollbar.h());
  hscrollbar.value(xposition_, W, 0, last_max_width);
  hscrollbar.linesize(fl_height(text_font(), text_size()));
  Fl_Widget::layout();
  damage(FL_DAMAGE_LAYOUT);
  focus(item_index[FOCUS][0]); // make value() work for top level
}

void Fl_Browser::hscrollbar_cb(Fl_Widget* o, void*) {
  ((Fl_Browser*)(o->parent()))->xposition(int(((Fl_Scrollbar*)o)->value()));
}

void Fl_Browser::xposition(int X) {
  int dx = xposition_-X;
  if (dx) {xposition_ = X; scrolldx += dx; damage(FL_DAMAGE_SCROLL);}
}

void Fl_Browser::scrollbar_cb(Fl_Widget* o, void*) {
  ((Fl_Browser*)(o->parent()))->yposition(int(((Fl_Scrollbar*)o)->value()));
}

void Fl_Browser::yposition(int Y) {
  if (Y == yposition_) return;
  ((Fl_Slider*)(&scrollbar))->value(Y);
  goto_position(Y);
  set_mark(FIRST_VISIBLE, HERE);
  scrolldy += (yposition_-Y); damage(FL_DAMAGE_SCROLL);
  yposition_ = Y;
}

////////////////////////////////////////////////////////////////
// Event handling

// Set the focus (the one with the box around it):
bool Fl_Browser::set_focus() {
  if (at_mark(FOCUS)) return false; // current item already focused
  damage_item(HERE); // so will draw focus box around item?
  damage_item(FOCUS); // so focus box around old focus item will be removed?
  set_mark(FOCUS, HERE); // current item is new focus item
  if (item_position[FOCUS] < yposition_) {
    yposition(item_position[FOCUS]); // make it first line
    goto_mark(FOCUS);
  } else {
    int h = item() ? item()->height() : 0;
    if (item_position[FOCUS]+h-yposition_ > H) {
      yposition(item_position[FOCUS]+h-H); // make it last line
      goto_mark(FOCUS);
    }
  }
  return true;
}

// force current item to a state and do callback for multibrowser:
bool Fl_Browser::item_select(bool value, bool do_callback) {
  if (!multi()) {
    if (value) return (item_select_only(do_callback));
    else return deselect(do_callback);
  }
  Fl_Flags f = item()->flags();
  if (value) {
    if (f & FL_VALUE) return false;
    item()->set_flag(FL_VALUE);
  } else {
    if (!(f & FL_VALUE)) return false;
    item()->clear_flag(FL_VALUE);
  }
  list()->flags_changed(this, item());
  damage_item(HERE);
  if (when() && do_callback) {
    clear_changed();
    execute(item());
  } else if (do_callback) {
    set_changed();
  }
  return true;
}

// Turn off all lines in the browser:
bool Fl_Browser::deselect(bool do_callback) {
  unset_mark(HERE);
  return item_select_only(do_callback);
}

// Set both the single and multi-browser to only this item:
bool Fl_Browser::item_select_only(bool do_callback) {
  if (multi()) {
    set_focus();
    bool ret = false;
    // Turn off all other items:
    if (goto_top()) do {
      if (item_select(at_mark(FOCUS), do_callback)) ret = true;
    } while (next());
    goto_mark(FOCUS);
    return ret;
  } else {
    if (!set_focus()) return false;
    if (when() && do_callback) {
      clear_changed();
      execute(item());
    } else if (do_callback) {
      set_changed();
    }
    return true;
  }
}

#define item_is_parent() \
(item()->flags()&FL_OPEN || children(item_index[HERE],item_level[HERE]+1)>=0)

int Fl_Browser::handle(int event) {
  static bool drag_type; // for multibrowser

  switch (event) {
  case FL_FOCUS:
  case FL_UNFOCUS:
    damage_item(FOCUS);
    return 1;

  case FL_PUSH:
  case FL_MOVE:
  case FL_ENTER:
    if (scrollbar.align()&FL_ALIGN_LEFT ?
	(Fl::event_x() < scrollbar.x()+scrollbar.w()) :
	(Fl::event_x() >= scrollbar.x()))
      if (send(event,scrollbar)) return 1;
    if (hscrollbar.align()&FL_ALIGN_TOP ?
	(Fl::event_y() < hscrollbar.y()+hscrollbar.h()) :
	(Fl::event_y() >= hscrollbar.y()))
      if (send(event,hscrollbar)) return 1;
    if (event != FL_PUSH) {
      Fl::belowmouse(this);
      return 1;
    }
    take_focus();
    openclose_drag = 0;
  case FL_DRAG: {
    if (!goto_position(Fl::event_y()-Y+yposition_) && !item()) break;

    // see if they clicked the open/close box
    int arrow_size = fl_height(text_font(), text_size())|1;
    int xx = (item_level[HERE]+1)*arrow_size+X-xposition_-Fl::event_x();
    if ((event==FL_PUSH || openclose_drag) && xx > 0 && xx < arrow_size &&
	item_is_parent()) {
      if (openclose_drag != 1) {openclose_drag = 1; damage_item(HERE);}
    } else {
      if (openclose_drag == 1) {openclose_drag = 2; damage_item(HERE);}
    }

    if (multi()) {
      if (event == FL_PUSH) {
	if (Fl::event_state(FL_CTRL)) {
	  // start a new selection block without changing state
	  drag_type = !item()->value();
	  if (openclose_drag) drag_type = !drag_type; // don't change it
	  item_select(drag_type, FL_WHEN_CHANGED);
	  set_focus();
	  return 1;
	} else if (Fl::event_state(FL_SHIFT)) {
	  // extend the current focus
	  drag_type = !item()->value();
	} else {
	  item_select_only(FL_WHEN_CHANGED);
	  drag_type = true;
	  return 1;
	}
	Fl::event_clicks(0);
      }
      // set everything from old focus to current to drag_type:
      int direction = compare_marks(HERE,FOCUS);
      set_mark(USER_0,FOCUS);
      set_focus();
      for (;;) {
	item_select(drag_type, FL_WHEN_CHANGED);
	if (at_mark(USER_0)) break;
	if (!(direction<0 ? next_visible() : previous_visible())) break;
      }
    } else {
      item_select_only(FL_WHEN_CHANGED);
    }
    return 1;}

  case FL_RELEASE:
    if (openclose_drag) {
      if (openclose_drag == 1) {
	goto_mark(FOCUS);
	goto TOGGLE_OPEN;
      }
    } else if (when() && Fl::event_clicks())
      goto DO_CALLBACK;
    goto RELEASE;

  case FL_KEYBOARD:
    switch (Fl::event_key()) {
    case FL_Up:
    case FL_Down:
      if (!goto_visible_focus()) {
	if (!goto_top()) break;
      } else
	if (!(Fl::event_key() == FL_Up ? previous_visible() : next_visible())) return 1;
      if (multi() && Fl::event_state(FL_SHIFT|FL_CTRL)) {
	if (Fl::event_state(FL_SHIFT)) item_select(1,FL_WHEN_CHANGED);
	set_focus();
      } else {
	item_select_only(FL_WHEN_CHANGED);
      }
      goto RELEASE;
    case FL_Enter:
      if (!goto_visible_focus()) {
	return 1;
      } else if (item_is_parent()) {
      TOGGLE_OPEN:
	item()->invert_flag(FL_OPEN);
	list()->flags_changed(this, item());
	relayout();
	goto RELEASE;
      } else if (when()) {
	Fl::event_clicks(1); // make program think it was a double-click
	goto DO_CALLBACK;
      }
    case ' ':
      if (!goto_visible_focus()) break;
      if (multi()) item_select(!item()->value(), FL_WHEN_CHANGED);
    RELEASE:
      Fl::event_clicks(0); // make program not think it is a double-click
      if (!(when()&FL_WHEN_RELEASE)) return 1;
      if (!changed() && !(when()&FL_WHEN_NOT_CHANGED)) return 1;
    DO_CALLBACK:
      clear_changed();
      goto_mark(FOCUS);
      execute(item());
      return 1;
    default:
      if (send(event,scrollbar)) return 1;
      if (send(event,hscrollbar)) return 1;
    }
    break;

  case FL_MOUSEWHEEL:
    return send(event, scrollbar);
#if 0
    int n = Fl::event_dy() * Fl_Style::wheel_scroll_lines;
    goto_mark(FIRST_VISIBLE);
    while (n > 0 && previous_visible()) n--;
    while (n < 0 && next_visible()) n++;
    set_top();
    return 1;
#endif
  }
  return 0;
}

// go to focus unless it is not visible, in that case go to first item
// visible in the menu and return false.  This is used by keystrokes so
// the browser does not scroll unexpectedly.
Fl_Widget* Fl_Browser::goto_visible_focus() {
  if (item_position[FOCUS] < yposition_ ||
      item_position[FOCUS] > yposition_+H) {
    if (!goto_mark(FIRST_VISIBLE)) return 0;
    if (item_position[HERE] < yposition_) next_visible();
    return item();
  } else {
    return goto_mark(FOCUS);
  }
}

// Go to a supplied list of indexes, stopping on that item or a closed
// parent of it:
Fl_Widget* Fl_Browser::goto_index(const int* indexes, int level) {
  // negative numbers make nothing be selected:
  if (indexes[0] < 0) {
    goto_top();
    item_index[HERE][0] = -1;
    item_level[HERE] = 0;
    return 0;
  }
  // start at the old focus:
  if (damage() & FL_DAMAGE_LAYOUT || !goto_mark(FOCUS))
    if (!goto_top()) return 0;
  // move until we are before it:
  while (::compare_marks(item_index[HERE],item_level[HERE],indexes,level) > 0)
    if (!previous_visible()) return 0;
  // move until we are after it:
  while (::compare_marks(item_index[HERE],item_level[HERE],indexes,level) < 0)
    if (!next_visible())  return previous_visible();
  return item();
}

////////////////////////////////////////////////////////////////
// Fltk 1.0 emulation
// Items are numbered and children of top-level items are ignored.
// In 1.0 the item numbers started at 1, I changed this to zero so
// this matches Fl_Menu.

bool Fl_Browser::select(int line, bool value) {
  if (!goto_number(line)) return false;
  return item_select(value, false);
}

bool Fl_Browser::selected(int line) {
  if (!goto_number(line)) return false;
  return item()->value();
}

void Fl_Browser::set_position(linepos lpos) {
/*
  set_mark(FIRST_VISIBLE, HERE);
  scrolldy += (yposition_-item_position[HERE]); damage(FL_DAMAGE_SCROLL);
  yposition_ = item_position[HERE];
  ((Fl_Slider*)&scrollbar)->value(yposition_);
*/
  int h = item() ? item()->height() : 0;
  int p = item_position[HERE];
  switch (lpos) {
    case TOP: break;
    case BOTTOM: p += h-H; break;
    case MIDDLE: p += h-H/2; break;
  }

  if (p > last_height-H) p = last_height-H;
  if (p < 0) p = 0;
  yposition(p);
}

bool Fl_Browser::displayed(int line) {
  if (!goto_number(line)) return false;
  return item()->visible();
}

void Fl_Browser::display(int line, bool value) {
  if (!goto_number(line)) return;
  value ? item()->show() : item()->hide();
  relayout();
}

////////////////////////////////////////////////////////////////
// Constructor

#define SLIDER_WIDTH 16

static void revert(Fl_Style* s) {
  s->color = FL_LIGHT3-1;
  s->glyph = ::glyph;
}
static Fl_Named_Style style("Browser", revert, &Fl_Browser::default_style);
Fl_Named_Style* Fl_Browser::default_style = &::style;

Fl_Browser::Fl_Browser(int X,int Y,int W,int H,const char* L)
  : Fl_Menu_(X,Y,W,H,L), endgroup(0),
    scrollbar(X+W-SLIDER_WIDTH,Y,SLIDER_WIDTH,H-SLIDER_WIDTH),
    hscrollbar(X,Y+H-SLIDER_WIDTH,W-SLIDER_WIDTH,SLIDER_WIDTH) {
  type(BOTH);
  style(default_style);
  xposition_ = 0;
  yposition_ = 0;
  scrolldx = scrolldy = 0;
  hscrollbar.parent(this);
  hscrollbar.type(FL_HORIZONTAL);
  hscrollbar.callback(hscrollbar_cb);
  scrollbar.parent(this);
  scrollbar.callback(scrollbar_cb);
  indented_ = 0;
  format_char_ = '@';
  column_char_ = '\t';
  // set all the marks to the top:
  levels = 0;
  for (int i = 0; i < NUMMARKS; i++) {
    // allocate space for TOPLEVEL (0)
    item_index[i] = (int*)malloc(sizeof(int));
    item_index[i][TOPLEVEL] = 0;
    item_position[i] = 0;
    item_level[i] = 0;
  }
  Fl_Group::current(parent());
}

Fl_Browser::~Fl_Browser() {
  for (int i = 0; i < NUMMARKS; i++) free(item_index[i]);
}

//
// End of "$Id: Fl_Browser.cxx,v 1.43 2001/07/23 09:50:04 spitzak Exp $".
//
