//
// "$Id: Fl_Browser.cxx,v 1.18 2000/05/15 05:52:25 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Browser.H>
#include <FL/fl_draw.H>
#include <stdlib.h>

////////////////////////////////////////////////////////////////
// Compile-time style configuration options.
// For "run time configuration" copy this file, rename the widget,
// compile with your desired options, and use your renamed widget.
// Deal with it.

#define DRAW_STRIPES 1 // draw Macintosh-style stripes
#define TRIANGLE_GLYPH 0 // use Macintosh/fltk-style glyphs

////////////////////////////////////////////////////////////////
// Moving between items:
// The browser can keep track of exactly one widget and can move that
// widget forward and backward, skipping invisible widgets.  It also
// tracks the top edge of this widget.  All algorithims must use this
// single widget storage, to allow subclasses to reuse the same memory
// for all child widgets.
// There is an array of "marks" and you can set a mark to the current
// widget or set the current widget to a mark.

// Structure for each level of hierarchy we have encountered:
struct Fl_Browser::Data {
  int index[NUMMARKS];
  Fl_Widget* widget; // the current widget
};

void Fl_Browser::set_level(int n) {
  if (n >= levels) {
    data = (Data*)realloc((void*)data, (n+1)*sizeof(Data));
    for (int i = 0; i < NUMMARKS; i++) data[levels].index[i] = -1;
    levels = n+1;
  }
  item_level[0] = n;
}

static int measure(Fl_Widget* widget) {
  if (!widget->h() || widget->damage()&FL_DAMAGE_LAYOUT) widget->layout();
  return widget->h();
}

Fl_Widget* Fl_Browser::goto_top() {
  item_level[0] = 0;
  item_position[0] = 0;
  item_number[0] = 0;
  data[0].index[0] = 0;
  if (!children()) {
    item_height_ = 0;
    data[0].widget = 0;
    widget = 0;
    return 0;
  }
  widget = data[0].widget = child(0);
  if (!widget->visible()) {
    // skip leading invisible widgets
    item_height_ = 0; // makes it not mess up position
    item_number[0] = -1;
    return forward();
  }
  item_height_ = measure(widget);
  return widget;
}

Fl_Widget* Fl_Browser::goto_mark(int n) {
  item_position[0] = item_position[n];
  item_number[0] = item_number[n];
  item_level[0] = item_level[n];
  Fl_Group* parent = this;
  widget = 0;
  for (int L = 0; L <= item_level[0]; L++) {
    int i = data[L].index[0] = data[L].index[n];
    if (i < 0 || i >= parent->children()) return 0; // some kind of foulup?
    widget = data[L].widget = parent->child(i);
    parent = (Fl_Group*)widget;
  }
  item_height_ = measure(widget);
  return widget;
}

void Fl_Browser::set_mark(int n, int from) {
  int L = item_level[n] = item_level[from];
  for (; L >= 0; L--) data[L].index[n] = data[L].index[from];
  item_position[n] = item_position[from];
  item_number[n] = item_number[from];
}

int Fl_Browser::at_mark(int n, int from) {
  int L = item_level[from];
  if (L != item_level[n]) return 0;
  for (; L >= 0; L--)
    if (data[L].index[n] != data[L].index[from]) return 0;
  return 1;
}

int Fl_Browser::before_mark(int n, int from) {
  for (int L = 0;; L++) {
    if (L > item_level[n]) return 0;
    if (L > item_level[from]) return 1;
    int d = data[L].index[n] - data[L].index[from];
    if (d > 0) return 1;
    if (d < 0) return 0;
  }
}

void Fl_Browser::unset_mark(int n) {
  data[0].index[n] = -1;
}

int Fl_Browser::is_set(int n) {
  return data[0].index[n] >= 0;
}

Fl_Widget* Fl_Browser::forward() {
  item_position[0] += item_height_;
  item_number[0]++;

  Fl_Group* parent =
    item_level[0] ? (Fl_Group*)(data[item_level[0]-1].widget) : this;
  int index = data[item_level[0]].index[0];

  // If we are on an open group title, go to first item in group:
  if (widget->is_group() && (widget->flags()&FL_OPEN) && widget->visible()
      && ((Fl_Group*)widget)->children()) {
    set_level(item_level[0]+1);
    parent = (Fl_Group*)widget;
    index = 0;
  } else {
    // go to next widget in this group
    index++;
  }

  // loop to find the next real item:
  for (;;) {

    // move up until we are at a group with another child:
    while (index >= parent->children()) {
      if (!item_level[0]) return 0; // end of the entire browser
      item_level[0]--;
      parent = item_level[0] ? (Fl_Group*)(data[item_level[0]-1].widget) : this;
      index = data[item_level[0]].index[0] + 1;
    }

    // skip invisible items:
    widget = parent->child(index);
    if (widget->visible()) break;
    index++;
  }

  data[item_level[0]].index[0] = index;
  data[item_level[0]].widget = widget;
  item_height_ = measure(widget);
  return widget;
}

Fl_Widget* Fl_Browser::backward() {
  Fl_Group* parent =
    item_level[0] ? (Fl_Group*)(data[item_level[0]-1].widget) : this;
  int index = data[item_level[0]].index[0];

  for (;;) {

    // go up to parent from first item in a group:
    if (!index) {
      if (!item_level[0]) { // start of browser
	item_position[0] = 0;
	return 0;
      }
      item_level[0]--;
      widget = data[item_level[0]].widget;
      break;
    }

    // go back to previous item in this group:
    index--;
    widget = parent->child(index);

    // go to last child in a group:
    while (widget->is_group() && (widget->flags()&FL_OPEN) && widget->visible()
	&& ((Fl_Group*)widget)->children()) {
      data[item_level[0]].index[0] = index;
      data[item_level[0]].widget = widget;
      set_level(item_level[0]+1);
      parent = (Fl_Group*)widget;
      index = parent->children()-1;
      widget = parent->child(index);
    }

    if (widget->visible()) {
      data[item_level[0]].index[0] = index;
      data[item_level[0]].widget = widget;
      break;
    }
  }

  data[item_level[0]].widget = widget;
  item_height_ = measure(widget);
  item_position[0] -= item_height_;
  item_number[0]--;
  return widget;
}

Fl_Widget* Fl_Browser::goto_position(int Y) {
  if (Y < 0) Y = 0;
  if (damage()&FL_DAMAGE_LAYOUT || Y<=yposition_/2 || !goto_mark(FIRST_VISIBLE)) {
    goto_top();
  } else {
    // move backwards until we are before or at the position:
    while (item_position[0] > Y) {
      if (!backward()) {goto_top(); break;}
    }
  }
  // move forward to the item:
  if (widget) for (;;) {
    if (item_position[0]+item_height_ > Y) break;
    if (!forward()) {
      item_position[0] -= item_height_;
      item_number[0]--;
      break;
    }
  }
  return widget;
}

// mark current item as being damaged:
void Fl_Browser::damage_item(int n) {
  if (at_mark(REDRAW_0,n) || at_mark(REDRAW_1,n)) return;
  int m = REDRAW_0;
  if (is_set(m)) {
    m = REDRAW_1;
    // if both marks are used we give up and damage the whole thing:
    if (is_set(m)) {damage(FL_DAMAGE_EXPOSE); return;}
  }
  set_mark(m,n);
  damage(FL_DAMAGE_SCROLL);
}

////////////////////////////////////////////////////////////////
// Drawing

// Glyphs that are drawn to the left of items.  Enumeration order
// is important!
enum {
  NONE, BAR, ELL, TEE, CLOSED_ELL, CLOSED_TEE, OPEN_ELL, OPEN_TEE
};

static void
glyph(int glyph, int x, int y, int w, int h, Fl_Color, Fl_Color color,
      Fl_Flags, Fl_Boxtype) {
  // fl_color((Fl_Color)9); fl_rect(x,y,w,h); // draw boundary for testing
  fl_color(color);
  int lx = x+w/2;
  int ly = y+(h-1)/2;
  switch (glyph) {
  case NONE:
    break;
  case BAR:
    fl_yxline(lx, y, y+h-1);
    break;
  case ELL:
    fl_yxline(lx, y, ly, x+w);
    break;
  case TEE:
    fl_yxline(lx, y, y+h-1);
    fl_xyline(lx, ly, x+w);
    break;
#if TRIANGLE_GLYPH
  case CLOSED_TEE:
  case OPEN_TEE:
    fl_yxline(lx, y, y+h-1);
    goto J1;
  case CLOSED_ELL:
  case OPEN_ELL:
    fl_yxline(lx, y, ly);
  J1:
    fl_glyph(glyph < OPEN_ELL ? FL_GLYPH_RIGHT : FL_GLYPH_DOWN,
	     x, y, w, h, color, color, 0, FL_NO_BOX);
    break;
#else
  default: {
    int boxsize = w/3;
    fl_rect(lx-boxsize, ly-boxsize, 2*boxsize+1, 2*boxsize+1);
    fl_yxline(lx, y, ly-boxsize);
    if (glyph&1) fl_yxline(lx, ly+boxsize, y+h-1);
    fl_xyline(lx+boxsize, ly, x+w);
    fl_xyline(lx-boxsize+2, ly, lx+boxsize-2);
    if (glyph < OPEN_ELL)
      fl_yxline(lx,ly-boxsize+2,ly+boxsize-2);
    }
#endif
  }
}

// this is non-zero if a drag was started in a group open/close box:
static char openclose_drag;

// Draws the current item:
void Fl_Browser::draw_item() {

  int y = Y+item_position[0]-yposition_;
  int h = item_height_;

  Fl_Color label_color;
  Fl_Color glyph_color;

  int is_focus = at_mark(FOCUS);

  if (multi() ? item_selected() : is_focus) {
    fl_color(selection_color());
    fl_rectf(X, y, W, h);
    label_color = glyph_color = selection_text_color();
  } else {
    glyph_color = text_color();
    label_color = widget->text_color();
    Fl_Color c0 = window_color();
    Fl_Color c1 = color();
    if (item_number[0] & 1 && c1 != c0) {
      // draw odd-numbered items with a dark stripe, plus contrast-enhancing
      // pixel rows on top and bottom:
      fl_color(c1);
      fl_rectf(X, y+1, W, h-2);
      fl_color(c0 <= FL_LIGHT1 ? c1 : Fl_Color(FL_LIGHT1));
      //fl_color(fl_color_average(c1, c0, 1.9));
      fl_xyline(X, y, X+W);
      fl_xyline(X, y+h-1, X+W);
    } else {
      fl_color(c0);
      fl_rectf(X, y, W, h);
    }
  }

  int arrow_size = text_size()|1;
  int preview_open =
    (openclose_drag == 1 && pushed() && at_mark(FOCUS)) ? FL_OPEN : 0;
  widget->invert_flag(preview_open);

  int x = X-xposition_;
  // draw the glyphs, one for each nesting level:
  for (int j = indented()?0:1; j <= item_level[0]; j++) {
    Fl_Group* parent = j ? (Fl_Group*)(data[j-1].widget) : this;
    int g = data[j].index[0] < parent->children()-1;
    if (j == item_level[0]) {
      if (indented() && widget->is_group()) {
	g += (widget->flags() & FL_OPEN) ? OPEN_ELL : CLOSED_ELL;
      } else {
	g += ELL;
      }
    }
    glyph()(g, x, y, arrow_size, h, FL_BLACK, glyph_color, 0, FL_NO_BOX);
    x += arrow_size;
  }

  if (focused() && is_focus) {
    fl_color(glyph_color);
    fl_line_style(FL_DASH);
    if (x + widget->w() > X+W) {
      // X bug?  clipped dashed rectangles don't draw exactly correct...
      fl_xyline(x, y, X+W);
      fl_yxline(x, y, y+h-1, X+W);
    } else {
      fl_rect(x, y, widget->w(), h);
    }
    fl_line_style(0);
  }
  fl_color(label_color);
  widget->x(x);
  widget->y(y+(leading()+1)/2-1);
  widget->draw();
  widget->invert_flag(preview_open);
}

void Fl_Browser::draw_clip_cb(void* v,int X, int Y, int W, int H) {
  ((Fl_Browser*)v)->draw_clip(X,Y,W,H);
}

void Fl_Browser::draw_clip(int x, int y, int w, int h) {
  fl_clip(x,y,w,h);

  int draw_all = damage() & (FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE|FL_DAMAGE_LAYOUT);
  if (goto_mark(FIRST_VISIBLE)) for (;;) {
    int item_y = Y+item_position[0]-yposition_;
    if (item_y >= y+h) break;
    if (item_y+item_height_ > y)
      if (draw_all || !at_mark(REDRAW_0) && !at_mark(REDRAW_1)) draw_item();
    if (!forward()) break;
  }

  // erase the area below the last item:
  int bottom_y = Y+item_position[0]-yposition_;
  if (bottom_y < y+h) {
    fl_color(window_color());
    fl_rectf(x, bottom_y, w, y+h-bottom_y);
  }
  fl_pop_clip();
}

void Fl_Browser::draw() {

  uchar d = damage();
  if (d & FL_DAMAGE_ALL) { // full redraw
    draw_window_frame();
    draw_clip(X, Y, W, H);
  } else if (d & (FL_DAMAGE_EXPOSE|FL_DAMAGE_LAYOUT)) { // redraw contents
    draw_clip(X, Y, W, H);
  } else { // minimal update
    if (scrolldx || scrolldy) {
      fl_scroll(X, Y, W, H, scrolldx, scrolldy, draw_clip_cb, this);
    }
    int clipped = 0;
    for (int n = REDRAW_0; n <= REDRAW_1; n++) {
      if (goto_mark(n)) {
	if (!clipped) {fl_clip(X,Y,W,H); clipped = 1;}
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

  // Measure the height of all items and find widest one
  int max_width = 0;

  // count all the items scrolled off the top:
  int arrow_size = text_size()|1;
  widget = goto_top();
  while (widget) {
    if (item_position[0]+item_height_ > yposition_) break;
    int w = widget->w()+arrow_size*item_level[0];
    if (w > max_width) max_width = w;
    widget = forward();
  }
  set_mark(FIRST_VISIBLE);
  // count all the rest of the items:
  while (widget) {
    int w = widget->w()+arrow_size*item_level[0];
    if (w > max_width) max_width = w;
    widget = forward();
  }
  if (indented()) max_width += arrow_size; // optional if no groups!

  int height = item_position[0];

  // figure out the visible area:

  X = x(); Y = y(); W = w(); H = h();
  window_box()->inset(X,Y,W,H);
  if (scrollbar.visible()) {
    W -= scrollbar.w();
    if (scrollbar.flags() & FL_ALIGN_LEFT) X += scrollbar.w();
  }
  if (hscrollbar.visible()) {
    H -= hscrollbar.h();
    if (scrollbar.flags() & FL_ALIGN_TOP) Y += hscrollbar.h();
  }

  // turn the scrollbars on and off as necessary:

  for (int z = 0; z<2; z++) {
    if ((type()&VERTICAL) && (type()&ALWAYS_ON || height > H || yposition_)) {
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
    if ((type()&HORIZONTAL) && (type()&ALWAYS_ON || max_width > W || xposition_)) {
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
  scrollbar.value(yposition_, H, 0, height);
  scrollbar.linesize(text_size()+leading());
  hscrollbar.resize(X,
		    scrollbar.flags()&FL_ALIGN_TOP ? Y-hscrollbar.h() : Y+H,
		    W, hscrollbar.h());
  hscrollbar.value(xposition_, W, 0, max_width);
  hscrollbar.linesize(text_size());
  Fl_Widget::layout();
  damage(FL_DAMAGE_LAYOUT);
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
  set_mark(FIRST_VISIBLE);
  scrolldy += (yposition_-Y); damage(FL_DAMAGE_SCROLL);
  yposition_ = Y;
  goto_mark(FOCUS); // set_focus() requires this
}

////////////////////////////////////////////////////////////////
// Event handling

// Set the focus (the one with the box around it):
int Fl_Browser::set_focus() {
  if (at_mark(FOCUS)) return 0;
  damage_item();
  damage_item(FOCUS);
  set_mark(FOCUS);
  if (item_position[0] < yposition_)
    yposition(item_position[0]);
  else if (item_position[0]+item_height_-yposition_ > H)
    yposition(item_position[0]+item_height_-H);
  return 1;
}

// Sets up so that Fl_Menu_::item() returns the current item:
void Fl_Browser::set_item() {
  // set the locations used by Fl_Menu_ for the current value:
  focus(data[0].index[0]);
  for (int i = 0; i < item_level[0]; i++)
    ((Fl_Group*)(data[i].widget))->focus(data[i+1].index[0]);
  if (data[item_level[0]].widget->is_group())
    ((Fl_Group*)(data[item_level[0]].widget))->focus(-1);
}

// force current item to a state and do callback for multibrowser:
int Fl_Browser::item_select(int value, int do_callback) {
  if (value) {
    if (!multi()) return item_select_only(do_callback);
    if (item_selected()) return 0;
    widget->set_flag(FL_VALUE);
  } else {
    if (!multi()) {deselect(do_callback); return 1;}
    if (!item_selected()) return 0;
    widget->clear_flag(FL_VALUE);
  }
  damage_item();
  if (when() & do_callback) {
    clear_changed();
    set_item();
    execute(widget);
  } else if (do_callback) {
    set_changed();
  }
  return 1;
}

// Turn off all lines in the browser:
void Fl_Browser::deselect(int do_callback) {
  data->index[0] = -1;
  item_select_only(do_callback);
}

// Set the browser to only this item:
int Fl_Browser::item_select_only(int do_callback) {
  if (multi()) {
    // turn off all other items:
    set_focus();
    goto_top(); for (;;) {
      item_select(at_mark(FOCUS), do_callback);
      if (!forward()) break;
    }
  } else {
    if (!set_focus()) return 0;
    set_item();
    if (when() & do_callback) {
      clear_changed();
      execute(widget);
    } else if (do_callback) {
      set_changed();
    }
  }
  return 1;
}

int Fl_Browser::handle(int event) {
  static char drag_type; // for multibrowser

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
  case FL_DRAG:
  case FL_RELEASE: {
    if (!goto_position(Fl::event_y()-Y+yposition_)) return 1;
    // see if they clicked the open/close box
    int arrow_size = text_size()|1;
    int xx = (item_level[0]+1)*arrow_size+X-xposition_-Fl::event_x();
    if ((event==FL_PUSH || openclose_drag) && 
	widget->is_group() && xx > 0 && xx < arrow_size) {
      if (event == FL_RELEASE) {
	widget->invert_flag(FL_OPEN);
	if (((Fl_Group*)widget)->children()) relayout();
	else damage_item();
      } else {
	if (openclose_drag != 1) {openclose_drag = 1; damage_item();}
      }
    } else {
      if (openclose_drag == 1) {openclose_drag = 2; damage_item();}
    }

    if (multi()) {
      if (event == FL_PUSH) {
	if (Fl::event_state(FL_CTRL)) {
	  // start a new selection block without changing state
	  set_focus();
	  drag_type = !item_selected();
	} else if (Fl::event_state(FL_SHIFT)) {
	  // extend the current focus
	  drag_type = !item_selected();
	} else {
	  item_select_only(FL_WHEN_CHANGED);
	  drag_type = 1;
	  return 1;
	}
      }
      // set everything from old focus to current to drag_type:
      int direction = before_mark(FOCUS);
      set_mark(USER_0,FOCUS);
      set_focus();
      for (;;) {
	item_select(drag_type, FL_WHEN_CHANGED);
	if (at_mark(USER_0)) break;
	if (!(direction ? forward() : backward())) break;
      }
    } else {
      item_select_only(FL_WHEN_CHANGED);
    }
    if (event==FL_RELEASE) goto RELEASE;
    return 1;}

  case FL_KEYBOARD:
    switch (Fl::event_key()) {
    case FL_Up:
    case FL_Down:
      if (!goto_visible_focus()) break;
      Fl::event_key() == FL_Up ? backward() : forward();
      if (multi() && Fl::event_state(FL_SHIFT|FL_CTRL)) {
	if (Fl::event_state(FL_SHIFT)) item_select(1,FL_WHEN_CHANGED);
	set_focus();
      } else {
	item_select_only(FL_WHEN_CHANGED);
      }
      goto RELEASE;
    case ' ':
      if (!goto_visible_focus()) break;
      if (multi()) item_select(!item_selected(), FL_WHEN_CHANGED);
    RELEASE:
      if (when()&FL_WHEN_RELEASE && (changed() || when()&FL_WHEN_NOT_CHANGED)) {
	clear_changed();
	execute(widget);
      }
      return 1;
    case FL_Enter:
      if (!goto_visible_focus()) break;
      if (widget->is_group()) {
	widget->invert_flag(FL_OPEN);
	if (((Fl_Group*)widget)->children()) relayout();
	else damage_item();
      }
      if (when() && (changed() || when()&FL_WHEN_NOT_CHANGED)) {
	clear_changed();
	execute(widget);
      }
      return 1;
    }

    if (send(event,scrollbar)) return 1;
    if (send(event,hscrollbar)) return 1;
    break;

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
    if (item_position[0] < yposition_) forward();
    return widget;
  } else {
    return goto_mark(FOCUS);
  }
}

////////////////////////////////////////////////////////////////
// Fltk 1.0 emulation

Fl_Widget* Fl_Browser::goto_number(int number) {
  if (number < 0) number = 0;
  if (damage()&FL_DAMAGE_LAYOUT || number <= item_number[0]/2) {
    goto_top();
  } else {
    // move backwards until we are before or at the position:
    while (item_number[0] > number) {
      if (!backward()) {goto_top(); break;}
    }
  }
  // move forward to the item:
  if (widget) for (;;) {
    if (item_number[0] >= number) break;
    if (!forward()) {
      item_position[0] -= item_height_;
      item_number[0]--;
      break;
    }
  }
  return widget;
}

int Fl_Browser::select(int line, int value) {
  if (!goto_number(line)) return 0;
  return item_select(value, 0);
}

int Fl_Browser::selected(int line) {
  if (!goto_number(line)) return 0;
  return item_selected();
}

void Fl_Browser::set_top() {
  set_mark(FIRST_VISIBLE);
  scrolldy += (yposition_-item_position[0]); damage(FL_DAMAGE_SCROLL);
  yposition_ = item_position[0];
}

int Fl_Browser::displayed(int line) {
  if (!goto_number(line)) return 0;
  return item_visible();
}

void Fl_Browser::display(int line, int value) {
  if (!goto_number(line)) return;
  value ? item_show() : item_hide();
}

////////////////////////////////////////////////////////////////
// Constructor

#define SLIDER_WIDTH 16

static void revert(Fl_Style* s) {
  s->color = FL_LIGHT3-1;
  s->glyph = ::glyph;
}

static Fl_Named_Style* style = new Fl_Named_Style("Browser", revert, &style);

Fl_Browser::Fl_Browser(int X,int Y,int W,int H,const char* L)
  : Fl_Menu_(X,Y,W,H,L), endgroup(0),
    scrollbar(X+W-SLIDER_WIDTH,Y,SLIDER_WIDTH,H-SLIDER_WIDTH),
    hscrollbar(X,Y+H-SLIDER_WIDTH,W-SLIDER_WIDTH,SLIDER_WIDTH) {
  type(BOTH);
  style(::style);
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
  data = 0;
  // set all the marks to the top:
  levels = 1;
  data = (Data*)malloc(sizeof(Data));
  for (int i = 0; i < NUMMARKS; i++) {
    data->index[i] = 0;
    item_position[i] = 0;
    item_number[i] = 0;
    item_level[i] = 0;
  }
  Fl_Group::current(parent());
}

Fl_Browser::~Fl_Browser() {
  delete[] data;
}

//
// End of "$Id: Fl_Browser.cxx,v 1.18 2000/05/15 05:52:25 bill Exp $".
//
