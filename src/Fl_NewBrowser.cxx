//
// "$Id: Fl_NewBrowser.cxx,v 1.1 2000/04/24 08:31:26 bill Exp $"
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
#include <FL/Fl_NewBrowser.H>
#include <FL/fl_draw.H>

////////////////////////////////////////////////////////////////
// Compile-time style configuration options.
// For "run time configuration" copy this file, rename the widget,
// compile with your desired options, and use your renamed widget.
// Deal with it.
// Most likely draw-item will be a virtual function so your subclass
// only needs to implement that.

#define DRAW_STRIPES 1
#define INDENT_STRIPES 1
#define DRAW_LINES 0
#define WIN32_STYLE 0 // [+] on groups rather than arrow

////////////////////////////////////////////////////////////////
// Moving between items:
// The browser can keep track of exactly one widget, move foward and
// backward between visible widgets, and keep track of the top edge
// of that widget.  All the algorithims must be written to use this
// single widget storage.

// Structure for each level of hierarchy we have encountered:
struct Fl_Browser::Data {
  int first_visible; // index of topmost visible item
  int last_visible; // index of bottommost visible item
  int index; // index of current item
  Fl_Widget* widget; // the current widget
};

void Fl_Browser::set_level(int n) {
  if (n >= levels) {
    Data* newdata = new Data[n+1];
    memcpy(newdata, data, levels*sizeof(Data));
    delete[] data;
    data = newdata;
    levels = n+1;
  }
  level = n;
}

int Fl_Browser::measure(Fl_Widget* widget) {
  if (!widget->h() || widget->damage()&FL_DAMAGE_LAYOUT) widget->layout();
  return widget->h();
}

Fl_Widget* Fl_Browser::goto_top() {
  set_level(0);
  data[0].index = 0;
  if (!children()) {
    data[0].widget = 0;
    return 0;
  }
  Fl_Widget* widget = data[0].widget = child(0);
  if (!widget->visible()) {
    // skip leading invisible widgets
    widget->h(0); // makes it not mess up position
    return forward();
  }
  return widget;
}

Fl_Widget* Fl_Browser::goto_first_visible() {
  level = first_visible_level;
  Fl_Group* parent = this;
  for (int i = 0; i <= level; i++) {
    data[i].index = data[i].first_visible;
    data[i].widget = parent->child(data[i].index);
    parent = (Fl_Group*)(data[i].widget);
  }
  return data[level].widget;
}

void Fl_Browser::set_first_visible() {
  for (int i = 0; i <= level; i++) data[i].first_visible = data[i].index;
  first_visible_level = level;
}

Fl_Widget* Fl_Browser::forward() {
  Fl_Group* parent = level ? (Fl_Group*)(data[level-1].widget) : this;
  int index = data[level].index;

  // If we are on an open group title, go to first item in group:
  Fl_Widget* widget = data[level].widget;
  if (widget->is_group() && (widget->flags()&FL_OPEN) && widget->visible()
      && ((Fl_Group*)widget)->children()) {
    set_level(level+1);
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
      if (!level) return 0; // end of the entire browser
      level--;
      parent = level ? (Fl_Group*)(data[level-1].widget) : this;
      index = data[level].index + 1;
    }

    // skip invisible items:
    widget = parent->child(index);
    if (widget->visible()) break;
  }

  data[level].index = index;
  data[level].widget = widget;
  return widget;
}

Fl_Widget* Fl_Browser::backward() {
  Fl_Group* parent = level ? (Fl_Group*)(data[level-1].widget) : this;
  int index = data[level].index;
  Fl_Widget* widget = 0;

  for (;;) {

    // go up to parent from first item in a group:
    if (!index) {
      if (!level) return 0; // start of browser
      level--;
      return data[level].widget;
    }

    // go back to previous item in this group:
    index--;
    widget = parent->child(index);

    // go to last child in a group:
    while (widget->is_group() && (widget->flags()&FL_OPEN) && widget->visible()
	&& ((Fl_Group*)widget)->children()) {
      data[level].index = index;
      data[level].widget = widget;
      set_level(level+1);
      parent = (Fl_Group*)widget;
      index = parent->children()-1;
      widget = parent->child(index);
    }

    if (widget->visible()) break;
  }

  data[level].index = index;
  data[level].widget = widget;
  return widget;
}

int Fl_Browser::goto_position(int Y) {
  int y;
  Fl_Widget* widget;
  int i;
  if (damage()&FL_DAMAGE_LAYOUT || Y <= yposition_/2) {
    // jump to the top
    widget = goto_top();
    y = 0;
    i = 0;
  } else {
    // move backwards until we are before or at the position:
    y = first_visible_position;
    widget = goto_first_visible();
    i = first_visible_index;
    while (y > Y) {
      i--;
      Fl_Widget* widget = backward();
      if (!widget) break;
      y -= measure(widget)+leading();
    }
  }
  // move forward to the item:
  while (widget) {
    int h = measure(widget)+leading();	
    if (y+h > Y) break;
    y += h;
    i++; widget = forward();
  }
  item_index = i;
  return y;
}

Fl_Widget* Fl_Browser::goto_focus() {
  int index = focus();
  if (index < 0 || index >= children()) return goto_top();
  Fl_Group* parent = this;
  for (int l = 0;;l++) {
    set_level(l);
    data[level].index = index;
    Fl_Widget* widget = data[level].widget = parent->child(index);
    if (widget->is_group() && widget->flags()&FL_OPEN) {
      parent = (Fl_Group*)(widget);
      index = parent->focus();
      if (index >= 0 && index < parent->children()) continue;
    }
    return widget;
  }
}

void Fl_Browser::set_focus() {
  focus(data[0].index);
  for (int i = 0; i < level; i++)
    ((Fl_Group*)(data[i].widget))->focus(data[i+1].index);
  if (data[level].widget->is_group())
    ((Fl_Group*)(data[level].widget))->focus(-1);
}

////////////////////////////////////////////////////////////////
// Drawing

void Fl_Browser::draw_clip_cb(void* v,int X, int Y, int W, int H) {
  ((Fl_Browser*)v)->draw_clip(X,Y,W,H);
}

void Fl_Browser::draw_clip(int X, int Y, int W, int H) {
  fl_clip(X,Y,W,H);
  int x,y,w,h; bbox(x,y,w,h);

  // y = top of the current widget in window coordinates:
  y += first_visible_position-yposition_;

  int arrow_size = text_size()+2;

  Fl_Color c0 = window_color();
#if DRAW_STRIPES
  int i = first_visible_index;
  Fl_Color c1 = color();
  Fl_Color c2 = FL_GRAY; if (c0 <= c2) c2 = c1;
  //Fl_Color c2 = fl_color_average(c1, c0, 1.9);
#endif

  Fl_Widget* widget = goto_first_visible();

  while (widget) {
    int item_h = measure(widget)+leading();

    Fl_Flags flags = widget->flags();
    Fl_Color label_color = widget->label_color();
    int item_x = x+arrow_size*level-xposition_;

#if DRAW_STRIPES
    if (i&1) {
#if INDENT_STRIPES
      if (X < item_x) {
	fl_color(c0);
	fl_rectf(X, y, item_x-X, item_h);
      }
      fl_color(c1);
      fl_rectf(item_x, y+1, X+W-item_x, item_h-2);
      fl_color(c2);
      fl_xyline(item_x-(data[level].index?0:arrow_size), y, X+W);
      int n = level;
      if (widget->is_group() && widget->flags()&FL_OPEN &&
	  ((Fl_Group*)widget)->children());
      else for (; n > 0; n--) {
	Fl_Group* parent = n ? (Fl_Group*)(data[n-1].widget) : this;
	if (data[n].index < parent->children()-1) break;
      }
      int x1 = x+n*arrow_size-xposition_;
      if (item_x < x1) x1 = item_x;
      fl_xyline(x1, y+item_h-1, X+W);
#else
      fl_color(c1);
      fl_rectf(X, y+1, W, item_h-2);
      fl_color(c2);
      fl_xyline(X, y, X+W);
      fl_xyline(X, y+item_h-1, X+W);
#endif
    } else {
      fl_color(c0);
      fl_rectf(X, y, W, item_h);
    }
#if INDENT_STRIPES
    if (level > 0 && X < item_x) {
      fl_color(c2);
      fl_yxline(item_x, y, y+item_h);
    }
#endif
#else
    fl_color(c0);
    fl_rectf(X, y, W, item_h);
#endif

#if DRAW_LINES
    fl_color(text_color());
    for (int j = 0;; j++) {
      int last;
      if (j) {
	last = data[j].index >= ((Fl_Group*)(data[j-1].widget))->children()-1;
      } else {
	last = data[0].index >= children()-1;
      }
      int lx = j*arrow_size+arrow_size/2+1-xposition_;
      int ix = item_x+arrow_size-1;
      if (j == level) {
	int ly = y+(item_h-1)/2;
#if WIN32_STYLE
	if (widget->is_group()) {
	  int boxsize = arrow_size/3;
	  fl_rect(lx-boxsize, ly-boxsize, 2*boxsize+1, 2*boxsize+1);
	  fl_yxline(lx, y, ly-boxsize);
	  if (!last) fl_yxline(lx, ly+boxsize, y+item_h);
	  fl_xyline(lx+boxsize, ly, ix);
	  fl_xyline(lx-boxsize+2, ly, lx+boxsize-2);
	  if (!(widget->flags()&FL_OPEN))
	    fl_yxline(lx,ly-boxsize+2,ly+boxsize-2);
	  break;
	}
#endif
	if (last) {
	  fl_yxline(lx, y, ly, ix);
	} else {
	  fl_yxline(lx, y, y+item_h);
	  fl_xyline(lx, ly, ix);
	}
	break;
      } else if (!last) {
	fl_yxline(lx, y, y+item_h);
      }
    }
#endif
#if !WIN32_STYLE
    if (widget->is_group()) {
      glyph()((widget->flags()&FL_OPEN) ? FL_GLYPH_DOWN : FL_GLYPH_RIGHT,
	      item_x, y+(item_h-arrow_size)/2,
	      arrow_size, arrow_size, c0,
	      text_color(), flags, FL_NO_BOX);
    }
#endif

    int j = level;
    if (widget->is_group() && widget->flags()&FL_OPEN &&
	((Fl_Group*)widget)->focus() >= 0) j++;
    else for (; j > 0; j--)
      if (((Fl_Group*)(data[j-1].widget))->focus() != data[j].index) break;
    int is_focus = !j && (focus() == data[0].index);

    if (!(type()&MULTI_BROWSER) && is_focus) flags |= FL_VALUE;
    if (flags & FL_VALUE) {
      fl_color(selection_color());
      int xx = item_x+arrow_size-3;
      fl_rectf(xx, y, x+w-xx, item_h);
      label_color = selection_text_color();
    }
    fl_color(label_color);
    if (focused() && is_focus) {
      int xx = item_x+arrow_size-3;
      fl_line_style(FL_DASH);
      if (hscrollbar.visible()) {
	fl_xyline(xx, y, x+w);
	fl_yxline(xx, y, y+item_h-1, x+w);
      } else {
	fl_rect(xx, y, x+w-xx, item_h);
      }
      label_color = selection_text_color();
      fl_line_style(0);
    }

    widget->x(item_x+arrow_size-3);
    widget->y(y+leading()/2);
    int save_w = widget->w(); widget->w(x+w-widget->x());
    widget->draw();
    widget->w(save_w);

    y += item_h;
    widget = forward();
#if DRAW_STRIPES
    i++;
#endif
    if (y >= Y+H) break;
  }
  // erase the area below the last item
  if (y < Y+H) {
#if DRAW_STRIPES
    if (i&i) {
      fl_color(c2);
      fl_xyline(X, y, X+W);
      fl_color(c1);
      y++;
    } else
#endif
      fl_color(c0);
    fl_rectf(X,y,W,Y+H-y);
  }
  fl_pop_clip();
}

void Fl_Browser::bbox(int& X, int& Y, int& W, int& H) {
  X = x(); Y = y(); W = w(); H = h(); window_box()->inset(X,Y,W,H);
  if (scrollbar.visible()) {
    W -= scrollbar.w();
    if (scrollbar.flags() & FL_ALIGN_LEFT) X += scrollbar.w();
  }
  if (hscrollbar.visible()) {
    H -= hscrollbar.h();
    if (scrollbar.flags() & FL_ALIGN_TOP) Y += hscrollbar.h();
  }
}

void Fl_Browser::draw() {
  int X,Y,W,H; bbox(X,Y,W,H);

  uchar d = damage();
  if (d & FL_DAMAGE_ALL) { // full redraw
    draw_window_frame();
    draw_clip(X, Y, W, H);
  } else if (d & FL_DAMAGE_HIGHLIGHT) { // minimual update
    // (not working yet)
    draw_clip(X, Y, W, H);
  } else {
    if (scrolldx || scrolldy) {
      fl_scroll(X, Y, W, H, scrolldx, scrolldy, draw_clip_cb, this);
    }
    // probably do something here to update changed items...
  }
  scrolldx = scrolldy = 0;

  // draw the scrollbars:
  if (d & FL_DAMAGE_ALL) {
    scrollbar.set_damage(FL_DAMAGE_ALL);
    hscrollbar.set_damage(FL_DAMAGE_ALL);
    if (scrollbar.visible() && hscrollbar.visible()) {
      // fill in the little box in the corner
      fl_color(color());
      fl_rectf(scrollbar.x(), hscrollbar.y(), scrollbar.w(), hscrollbar.h());
    }
  }
  update_child(scrollbar);
  update_child(hscrollbar);
}

void Fl_Browser::layout() {

  // Measure the height of all items and find widest one
  int max_width = 0;
  int height = 0;
  Fl_Widget* widget = goto_top();

  // count all the items scrolled off the top:
  int i = 0;
  int arrow_size = text_size();
  while (widget) {
    int h = measure(widget)+leading();
    if (height+h-yposition_ > 0) break;
    height += h;
    int w = widget->w()+arrow_size*level;
    if (w > max_width) max_width = w;
    i++; widget = forward();
  }
  first_visible_position = height;
  first_visible_index = i;
  set_first_visible();
  // count all the rest of the items:
  while (widget) {
    height += measure(widget)+leading();
    int w = widget->w()+arrow_size*level;
    if (w > max_width) max_width = w;
    widget = forward();
  }
  max_width += arrow_size; // optional if no groups!

  // turn the scrollbars on and off as necessary:
  int X,Y,W,H; bbox(X,Y,W,H);
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

  hscrollbar.resize(X,
		    scrollbar.flags()&FL_ALIGN_TOP ? Y-hscrollbar.h() : Y+H,
		    W, hscrollbar.h());
  hscrollbar.value(xposition_, W, 0, max_width);
  Fl_Widget::layout();
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
  int y = goto_position(Y);
  set_first_visible();
  first_visible_position = y;
  first_visible_index = item_index;
  scrolldy += (yposition_-Y); damage(FL_DAMAGE_SCROLL);
  yposition_ = Y;
}

#define SLIDER_WIDTH 16

static void revert(Fl_Style* s) {
  s->color = FL_LIGHT2;
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
  data = 0;
  levels = 0;
  // lots of other initialization is done by first layout() and goto_top()
  Fl_Group::current(parent());
}

Fl_Browser::~Fl_Browser() {
  delete[] data;
}

int Fl_Browser::handle(int event) {
  if (event == FL_FOCUS || event == FL_UNFOCUS) {
    damage(FL_DAMAGE_HIGHLIGHT);
    return true;
  }
  if (event == FL_KEYBOARD) {
    switch (navigation_key()) {
    case FL_Down:
      goto_focus();
      if (forward()) {set_focus(); damage(FL_DAMAGE_HIGHLIGHT); return 1;}
      break;
    case FL_Up:
      goto_focus();
      if (backward()) {set_focus(); damage(FL_DAMAGE_HIGHLIGHT); return 1;}
      break;
    }
  }
  if (send(event,scrollbar)) return 1;
  if (send(event,hscrollbar)) return 1;
  if (event == FL_MOVE || event == FL_ENTER) {
    Fl::belowmouse(this);
    return 1;
  }
  return 0;
}

//
// End of "$Id: Fl_NewBrowser.cxx,v 1.1 2000/04/24 08:31:26 bill Exp $".
//
