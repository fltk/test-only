//
// "$Id$"
//
// Base Browser widget class for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#define DISPLAY_SEARCH_BOTH_WAYS_AT_ONCE

#include <stdio.h>
#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/Browser_.h>
#include <fltk3/draw.h>
#include <fltk3/Wrapper.h>


/* redraw bits:
 1 = redraw children (the scrollbar)
 2 = redraw one or two items
 4 = redraw all items
 */


static void scrollbar_callback(fltk3::Widget* s, void*) {
  ((fltk3::Browser_*)(s->parent()))->position(int(((fltk3::Scrollbar*)s)->value()));
}

static void hscrollbar_callback(fltk3::Widget* s, void*) {
  ((fltk3::Browser_*)(s->parent()))->hposition(int(((fltk3::Scrollbar*)s)->value()));
}


// Documented in the header file
void fltk3::Browser_::bbox(int& X, int& Y, int& W, int& H) const {
  int scrollsize = scrollbar_size_ ? scrollbar_size_ : fltk3::scrollbar_size();
  fltk3::Box* b = box() ? box() : fltk3::DOWN_BOX;
  X = fltk3::box_dx(b);
  Y = fltk3::box_dy(b);
  W = w()-fltk3::box_dw(b);
  H = h()-fltk3::box_dh(b);
  if (scrollbar.visible()) {
    W -= scrollsize;
    if (scrollbar.align() & fltk3::ALIGN_LEFT) X += scrollsize;
  }
  if (W < 0) W = 0;
  if (hscrollbar.visible()) {
    H -= scrollsize;
    if (scrollbar.align() & fltk3::ALIGN_TOP) Y += scrollsize;
  }
  if (H < 0) H = 0;
}


// Documented in the header file
int fltk3::Browser_::leftedge() const {
  int X, Y, W, H; bbox(X, Y, W, H);
  return X;
}


// Documented in the header file
void fltk3::Browser_::resize(int X, int Y, int W, int H) {
  int scrollsize = scrollbar_size_ ? scrollbar_size_ : fltk3::scrollbar_size();
  Widget::resize(X, Y, W, H);
  // move the scrollbars so they can respond to events:
  bbox(X,Y,W,H);
  scrollbar.resize(
                   scrollbar.align()&fltk3::ALIGN_LEFT ? X-scrollsize : X+W,
                   Y, scrollsize, H);
  hscrollbar.resize(
                    X, scrollbar.align()&fltk3::ALIGN_TOP ? Y-scrollsize : Y+H,
                    W, scrollsize);
}


// Documented in the header file
void fltk3::Browser_::redraw_line(void* item) {
  if (!redraw1 || redraw1 == item) {redraw1 = item; damage(fltk3::DAMAGE_EXPOSE);}
  else if (!redraw2 || redraw2 == item) {redraw2 = item; damage(fltk3::DAMAGE_EXPOSE);}
  else damage(fltk3::DAMAGE_SCROLL);
}


// Documented in the header file
void fltk3::Browser_::update_top() {
  if (!top_) top_ = item_first();
  if (position_ != real_position_) {
    void* l;
    int ly;
    int yy = position_;
    // start from either head or current position, whichever is closer:
    if (!top_ || yy <= (real_position_/2)) {
      l = item_first();
      ly = 0;
    } else {
      l = top_;
      ly = real_position_-offset_;
    }
    if (!l) {
      top_ = 0;
      offset_ = 0;
      real_position_ = 0;
    } else {
      int hh = item_quick_height(l);
      // step through list until we find line containing this point:
      while (ly > yy) {
	void* l1 = item_prev(l);
	if (!l1) {ly = 0; break;} // hit the top
	l  = l1;
	hh = item_quick_height(l);
	ly -= hh;
      }
      while ((ly+hh) <= yy) {
	void* l1 = item_next(l);
	if (!l1) {yy = ly+hh-1; break;}
	l = l1;
	ly += hh;
	hh = item_quick_height(l);
      }
      // top item must *really* be visible, use slow height:
      for (;;) {
	hh = item_height(l);
	if ((ly+hh) > yy) break; // it is big enough to see
                                 // go up to top of previous item:
	void* l1 = item_prev(l);
	if (!l1) {ly = yy = 0; break;} // hit the top
	l = l1; yy = position_ = ly = ly-item_quick_height(l);
      }
      // use it:
      top_ = l;
      offset_ = yy-ly;
      real_position_ = yy;
    }
    damage(fltk3::DAMAGE_SCROLL);
  }
}


// Documented in the header file
void fltk3::Browser_::position(int pos) {
  if (pos < 0) pos = 0;
  if (pos == position_) return;
  position_ = pos;
  if (pos != real_position_) redraw_lines();
}


// Documented in the header file
void fltk3::Browser_::hposition(int pos) {
  if (pos < 0) pos = 0;
  if (pos == hposition_) return;
  hposition_ = pos;
  if (pos != real_hposition_) redraw_lines();
}


// Documented in the header file
int fltk3::Browser_::displayed(void* item) const {
  int X, Y, W, H; bbox(X, Y, W, H);
  int yy = H+offset_;
  for (void* l = top_; l && yy > 0; l = item_next(l)) {
    if (l == item) return 1;
    yy -= item_height(l);
  }
  return 0;
}


// Documented in the header file
// Messy because we have no idea if it is before top or after bottom:
void fltk3::Browser_::display(void* item) {
  
  // First special case - want to display first item in the list?
  update_top();
  if (item == item_first()) {position(0); return;}
  
  int X, Y, W, H, Yp; bbox(X, Y, W, H);
  void* l = top_;
  Y = Yp = -offset_;
  int h1;
  
  // 2nd special case - want to display item already displayed at top of browser?
  if (l == item) {position(real_position_+Y); return;} // scroll up a bit
  
  // 3rd special case - want to display item just above top of browser?
  void* lp = item_prev(l);
  if (lp == item) {position(real_position_+Y-item_quick_height(lp)); return;}
  
#ifdef DISPLAY_SEARCH_BOTH_WAYS_AT_ONCE
  // search for item.  We search both up and down the list at the same time,
  // this evens up the execution time for the two cases - the old way was
  // much slower for going up than for going down.
  while (l || lp) {
    if (l) {
      h1 = item_quick_height(l);
      if (l == item) {
	if (Y <= H) { // it is visible or right at bottom
	  Y = Y+h1-H; // find where bottom edge is
	  if (Y > 0) position(real_position_+Y); // scroll down a bit
	} else {
	  position(real_position_+Y-(H-h1)/2); // center it
	}
	return;
      }
      Y += h1;
      l = item_next(l);
    }
    if (lp) {
      h1 = item_quick_height(lp);
      Yp -= h1;
      if (lp == item) {
	if ((Yp + h1) >= 0) position(real_position_+Yp);
	else position(real_position_+Yp-(H-h1)/2);
	return;
      }
      lp = item_prev(lp);
    }
  }
#else
  // Old version went forwards and then backwards:
  // search forward for it:
  l = top_;
  for (; l; l = item_next(l)) {
    h1 = item_quick_height(l);
    if (l == item) {
      if (Y <= H) { // it is visible or right at bottom
	Y = Y+h1-H; // find where bottom edge is
	if (Y > 0) position(real_position_+Y); // scroll down a bit
      } else {
	position(real_position_+Y-(H-h1)/2); // center it
      }
      return;
    }
    Y += h1;
  }
  // search backward for it, if found center it:
  l = lp;
  Y = -offset_;
  for (; l; l = item_prev(l)) {
    h1 = item_quick_height(l);
    Y -= h1;
    if (l == item) {
      if ((Y + h1) >= 0) position(real_position_+Y);
      else position(real_position_+Y-(H-h1)/2);
      return;
    }
  }
#endif
}


// Documented in the header file
void fltk3::Browser_::draw() {
  int drawsquare = 0;
  update_top();
  int full_width_ = full_width();
  int full_height_ = full_height();
  int X, Y, W, H; bbox(X, Y, W, H);
  int dont_repeat = 0;
  int xo = 0, yo = 0;
J1:
  if (damage() & fltk3::DAMAGE_ALL) { // redraw the box if full redraw
    fltk3::Box* b = box() ? box() : fltk3::DOWN_BOX;
    draw_box(b, xo, yo, w(), h(), color());
    drawsquare = 1;
  }
  // see if scrollbar needs to be switched on/off:
  if ((has_scrollbar_ & VERTICAL) && (
                                      (has_scrollbar_ & ALWAYS_ON) || position_ || full_height_ > H)) {
    if (!scrollbar.visible()) {
      scrollbar.set_visible();
      drawsquare = 1;
      bbox(X, Y, W, H);
    }
  } else {
    top_ = item_first(); real_position_ = offset_ = 0;
    if (scrollbar.visible()) {
      scrollbar.clear_visible();
      set_damage((uchar)(damage()|fltk3::DAMAGE_SCROLL));
    }
  }
  
  if ((has_scrollbar_ & HORIZONTAL) && (
                                        (has_scrollbar_ & ALWAYS_ON) || hposition_ || full_width_ > W)) {
    if (!hscrollbar.visible()) {
      hscrollbar.set_visible();
      drawsquare = 1;
      bbox(X, Y, W, H);
    }
  } else {
    real_hposition_ = 0;
    if (hscrollbar.visible()) {
      hscrollbar.clear_visible();
      set_damage((uchar)(damage()|fltk3::DAMAGE_SCROLL));
    }
  }
  
  // Check the vertical scrollbar again, just in case it needs to be drawn
  // because the horizontal one is drawn.  There should be a cleaner way
  // to do this besides copying the same code...
  if ((has_scrollbar_ & VERTICAL) && (
                                      (has_scrollbar_ & ALWAYS_ON) || position_ || full_height_ > H)) {
    if (!scrollbar.visible()) {
      scrollbar.set_visible();
      drawsquare = 1;
      bbox(X, Y, W, H);
    }
  } else {
    top_ = item_first(); real_position_ = offset_ = 0;
    if (scrollbar.visible()) {
      scrollbar.clear_visible();
      set_damage((uchar)(damage()|fltk3::DAMAGE_SCROLL));
    }
  }
  
  bbox(X, Y, W, H);
  
  fltk3::push_clip(X, Y, W, H);
  // for each line, draw it if full redraw or scrolled.  Erase background
  // if not a full redraw or if it is selected:
  void* l = top();
  int yy = -offset_;
  for (; l && yy < H; l = item_next(l)) {
    int hh = item_height(l);
    if (hh <= 0) continue;
    if ((damage()&(fltk3::DAMAGE_SCROLL|fltk3::DAMAGE_ALL)) || l == redraw1 || l == redraw2) {
      if (item_selected(l)) {
	fltk3::color(active_r() ? selection_color() : fltk3::inactive(selection_color()));
	fltk3::rectf(X, yy+Y, W, hh);
      } else if (!(damage()&fltk3::DAMAGE_ALL)) {
	fltk3::push_clip(X, yy+Y, W, hh);
	draw_box(box() ? box() : fltk3::DOWN_BOX, xo, yo, w(), h(), color());
	fltk3::pop_clip();
      }
      item_draw(l, X-hposition_, yy+Y, W+hposition_, hh);
      if (l == selection_ && fltk3::focus() == this) {
	draw_box(fltk3::BORDER_FRAME, X, yy+Y, W, hh, color());
	draw_focus(fltk3::NO_BOX, X, yy+Y, W+1, hh+1);
      }
      int ww = item_width(l);
      if (ww > max_width) {max_width = ww; max_width_item = l;}
    }
    yy += hh;
  }
  // erase the area below last line:
  if (!(damage()&fltk3::DAMAGE_ALL) && yy < H) {
    fltk3::push_clip(X, yy+Y, W, H-yy);
    draw_box(box() ? box() : fltk3::DOWN_BOX, xo, yo, w(), h(), color());
    fltk3::pop_clip();
  }
  fltk3::pop_clip();
  redraw1 = redraw2 = 0;
  
  if (!dont_repeat) {
    dont_repeat = 1;
    // see if changes to full_height caused by calls to slow_height
    // caused scrollbar state to change, in which case we have to redraw:
    full_height_ = full_height();
    full_width_ = full_width();
    if ((has_scrollbar_ & VERTICAL) &&
	((has_scrollbar_ & ALWAYS_ON) || position_ || full_height_>H)) {
      if (!scrollbar.visible()) { damage(fltk3::DAMAGE_ALL); goto J1; }
    } else {
      if (scrollbar.visible()) { damage(fltk3::DAMAGE_ALL); goto J1; }
    }
    if ((has_scrollbar_ & HORIZONTAL) &&
	((has_scrollbar_ & ALWAYS_ON) || hposition_ || full_width_>W)) {
      if (!hscrollbar.visible()) { damage(fltk3::DAMAGE_ALL); goto J1; }
    } else {
      if (hscrollbar.visible()) { damage(fltk3::DAMAGE_ALL); goto J1; }
    }
  }
  
  // update the scrollbars and redraw them:
  int scrollsize = scrollbar_size_ ? scrollbar_size_ : fltk3::scrollbar_size();
  int dy = top_ ? item_quick_height(top_) : 0; if (dy < 10) dy = 10;
  if (scrollbar.visible()) {
    scrollbar.damage_resize(
                            scrollbar.align()&fltk3::ALIGN_LEFT ? X-scrollsize : X+W,
                            Y, scrollsize, H);
    scrollbar.value(position_, H, 0, full_height_);
    scrollbar.linesize(dy);
    if (drawsquare) draw_child(scrollbar);
    else update_child(scrollbar);
  }
  if (hscrollbar.visible()) {
    hscrollbar.damage_resize(
                             X, scrollbar.align()&fltk3::ALIGN_TOP ? Y-scrollsize : Y+H,
                             W, scrollsize);
    hscrollbar.value(hposition_, W, 0, full_width_);
    hscrollbar.linesize(dy);
    if (drawsquare) draw_child(hscrollbar);
    else update_child(hscrollbar);
  }
  
  // draw that little square between the scrollbars:
  if (drawsquare && scrollbar.visible() && hscrollbar.visible()) {
    fltk3::color(parent()->color());
    fltk3::rectf(scrollbar.x(), hscrollbar.y(), scrollsize, scrollsize);
  }
  
  real_hposition_ = hposition_;
}


// Documented in the header file
void fltk3::Browser_::new_list() {
  top_ = 0;
  position_ = real_position_ = 0;
  hposition_ = real_hposition_ = 0;
  selection_ = 0;
  offset_ = 0;
  max_width = 0;
  max_width_item = 0;
  redraw_lines();
}

// Documented in the header file
void fltk3::Browser_::deleting(void* item) {
  if (displayed(item)) {
    redraw_lines();
    if (item == top_) {
      real_position_ -= offset_;
      offset_ = 0;
      top_ = item_next(item);
      if (!top_) top_ = item_prev(item);
    }
  } else {
    // we don't know where this item is, recalculate top...
    real_position_ = 0;
    offset_ = 0;
    top_ = 0;
  }
  if (item == selection_) selection_ = 0;
  if (item == max_width_item) {max_width_item = 0; max_width = 0;}
}


// Documented in the header file
void fltk3::Browser_::replacing(void* a, void* b) {
  redraw_line(a);
  if (a == selection_) selection_ = b;
  if (a == top_) top_ = b;
  if (a == max_width_item) {max_width_item = 0; max_width = 0;}
}


// Documented in the header file
void fltk3::Browser_::swapping(void* a, void* b) {
  redraw_line(a);
  redraw_line(b);
  if (a == selection_) selection_ = b;
  else if (b == selection_) selection_ = a;
  if (a == top_) top_ = b;
  else if (b == top_) top_ = a;
}


// Documented in the header file
void fltk3::Browser_::inserting(void* a, void* b) {
  if (displayed(a)) redraw_lines();
  if (a == top_) top_ = b;
}


// Documented in the header file
void* fltk3::Browser_::find_item(int ypos) {
  update_top();
  int X, Y, W, H; bbox(X, Y, W, H);
  int yy = Y-offset_;
  for (void *l = top_; l; l = item_next(l)) {
    int hh = item_height(l); if (hh <= 0) continue;
    yy += hh;
    if (ypos <= yy || yy>=(Y+H)) return l;
  }
  return 0;
}


// Documented in the header file
int fltk3::Browser_::select(void* item, int val, int docallbacks) {
  if (type() == fltk3::MULTI_BROWSER) {
    if (selection_ != item) {
      if (selection_) redraw_line(selection_);
      selection_ = item;
      redraw_line(item);
    }
    if ((!val)==(!item_selected(item))) return 0;
    item_select(item, val);
    redraw_line(item);
  } else {
    if (val && selection_ == item) return 0;
    if (!val && selection_ != item) return 0;
    if (selection_) {
      item_select(selection_, 0);
      redraw_line(selection_);
      selection_ = 0;
    }
    if (val) {
      item_select(item, 1);
      selection_ = item;
      redraw_line(item);
      display(item);
    }
  }	    
  if (docallbacks) {
    set_changed();
    do_callback();
  }
  return 1;
}


// Documented in the header file
int fltk3::Browser_::deselect(int docallbacks) {
  if (type() == fltk3::MULTI_BROWSER) {
    int change = 0;
    for (void* p = item_first(); p; p = item_next(p))
      change |= select(p, 0, docallbacks);
    return change;
  } else {
    if (!selection_) return 0;
    item_select(selection_, 0);
    redraw_line(selection_);
    selection_ = 0;
    return 1;
  }
}


// Documented in the header file
int fltk3::Browser_::select_only(void* item, int docallbacks) {
  if (!item) return deselect(docallbacks);
  int change = 0;
  fltk3::WidgetTracker wp(this);
  if (type() == fltk3::MULTI_BROWSER) {
    for (void* p = item_first(); p; p = item_next(p)) {
      if (p != item) change |= select(p, 0, docallbacks);
      if (wp.deleted()) return change;
    }
  }
  change |= select(item, 1, docallbacks);
  if (wp.deleted()) return change;
  display(item);
  return change;
}


// Documented in the header file
int fltk3::Browser_::handle(int event) {
  // NOTE:
  // We use fltk3::WidgetTracker to test if the user has deleted
  // this widget in a callback. Callbacks can be called by:
  //  - do_callback()
  //  - select()
  //  - select_only()
  //  - deselect()
  // Thus we must test wp.deleted() after each of these calls,
  // unless we return directly after one of these.
  // If wp.deleted() is true, we return 1 because we used the event.
  
  fltk3::WidgetTracker wp(this);
  
  // must do shortcuts first or the scrollbar will get them...
  if (event == fltk3::ENTER || event == fltk3::LEAVE) return 1;
  if (event == fltk3::KEYBOARD && type() >= fltk3::HOLD_BROWSER) {
    void* l1 = selection_;
    void* l = l1; if (!l) l = top_; if (!l) l = item_first();
    if (l) {
      if (type()==fltk3::HOLD_BROWSER) {
        switch (fltk3::event_key()) {
          case fltk3::DownKey:
            while ((l = item_next(l)))
              if (item_height(l)>0) {select_only(l, when()); break;}
            return 1;
          case fltk3::UpKey:
            while ((l = item_prev(l))) {
              if (item_height(l)>0) {
                select_only(l, when());
                break; // no need to test wp (return 1)
              }
            }
            return 1;
        } 
      } else  {
        switch (fltk3::event_key()) {
          case fltk3::EnterKey:
          case fltk3::KPEnterKey:
            select_only(l, when() & ~fltk3::WHEN_ENTER_KEY);
            if (wp.deleted()) return 1;
            if (when() & fltk3::WHEN_ENTER_KEY) {
              set_changed();
              do_callback();
            }
            return 1;
          case ' ':
            selection_ = l;
            select(l, !item_selected(l), when() & ~fltk3::WHEN_ENTER_KEY);
            return 1;
          case fltk3::DownKey:
            while ((l = item_next(l))) {
              if (fltk3::event_state(fltk3::SHIFT|fltk3::CTRL))
                select(l, l1 ? item_selected(l1) : 1, when());
              if (wp.deleted()) return 1;
              if (item_height(l)>0) goto J1;
            }
            return 1;
          case fltk3::UpKey:
            while ((l = item_prev(l))) {
              if (fltk3::event_state(fltk3::SHIFT|fltk3::CTRL))
                select(l, l1 ? item_selected(l1) : 1, when());
              if (wp.deleted()) return 1;
              if (item_height(l)>0) goto J1;
            }
            return 1;
          J1:
            if (selection_) redraw_line(selection_);
            selection_ = l; redraw_line(l);
            display(l);
            return 1;
        }
      }
    }
  }
  
  if (Group::handle(event)) return 1;
  if (wp.deleted()) return 1;
  
  int X, Y, W, H; bbox(X, Y, W, H);
  int my;
  // NOTE:
  // instead of:
  //     change = select_only(find_item(my), when() & fltk3::WHEN_CHANGED)
  // we use the construct:
  //     change = select_only(find_item(my), 0);
  //     if (change && (when() & fltk3::WHEN_CHANGED)) {
  //	 set_changed();
  //       do_callback();
  //     }
  // See str #834
  // The first form calls the callback *before* setting change.
  // The callback may execute an fltk3::wait(), resulting in another
  // call of fltk3::Browser_::handle() for the same widget. The sequence
  // of events can be an fltk3::PUSH followed by an fltk3::RELEASE.
  // This second call of fltk3::Browser_::handle() may result in a -
  // somewhat unexpected - second concurrent invocation of the callback.
  
  static char change;
  static char whichway;
  static int py;
  switch (event) {
    case fltk3::PUSH:
      if (!fltk3::event_inside(X, Y, W, H)) return 0;
      if (fltk3::visible_focus()) {
        fltk3::focus(this);
        redraw();
      }
      my = py = fltk3::event_y();
      change = 0;
      if (type() == fltk3::NORMAL_BROWSER || !top_)
        ;
      else if (type() != fltk3::MULTI_BROWSER) {
        change = select_only(find_item(my), 0);
        if (wp.deleted()) return 1;
        if (change && (when() & fltk3::WHEN_CHANGED)) {
          set_changed();
          do_callback();
          if (wp.deleted()) return 1;
        }
      } else {
        void* l = find_item(my);
        whichway = 1;
        if (fltk3::event_state(fltk3::CTRL)) { // toggle selection:
        TOGGLE:
          if (l) {
            whichway = !item_selected(l);
            change = select(l, whichway, 0);
            if (wp.deleted()) return 1;
            if (change && (when() & fltk3::WHEN_CHANGED)) {
              set_changed();
              do_callback();
              if (wp.deleted()) return 1;
            }
          }
        } else if (fltk3::event_state(fltk3::SHIFT)) { // extend selection:
          if (l == selection_) goto TOGGLE;
          // state of previous selection determines new value:
          whichway = l ? !item_selected(l) : 1;
          // see which of the new item or previous selection is earlier,
          // by searching from the previous forward for this one:
          int down;
          if (!l) down = 1;
          else {for (void* m = selection_; ; m = item_next(m)) {
            if (m == l) {down = 1; break;}
            if (!m) {down = 0; break;}
          }}
          if (down) {
            for (void* m = selection_; m != l; m = item_next(m)) {
              select(m, whichway, when() & fltk3::WHEN_CHANGED);
              if (wp.deleted()) return 1;
            }
          } else {
            void* e = selection_;
            for (void* m = item_next(l); m; m = item_next(m)) {
              select(m, whichway, when() & fltk3::WHEN_CHANGED);
              if (wp.deleted()) return 1;
              if (m == e) break;
            }
          }
          // do the clicked item last so the select box is around it:
          change = 1;
          if (l) select(l, whichway, when() & fltk3::WHEN_CHANGED);
          if (wp.deleted()) return 1;
        } else { // select only this item
          change = select_only(l, 0);
          if (wp.deleted()) return 1;
          if (change && (when() & fltk3::WHEN_CHANGED)) {
            set_changed();
            do_callback();
            if (wp.deleted()) return 1;
          }
        }
      }
      return 1;
    case fltk3::DRAG:
      // do the scrolling first:
      my = fltk3::event_y();
      if (my < Y && my < py) {
        int p = real_position_+my-Y;
        if (p<0) p = 0;
        position(p);
      } else if (my > (Y+H) && my > py) {
        int p = real_position_+my-(Y+H);
        int hh = full_height()-H; if (p > hh) p = hh;
        if (p<0) p = 0;
        position(p);
      }
      if (type() == fltk3::NORMAL_BROWSER || !top_)
        ;
      else if (type() == fltk3::MULTI_BROWSER) {
        void* l = find_item(my);
        void* t; void* b; // this will be the range to change
        if (my > py) { // go down
          t = selection_ ? item_next(selection_) : 0;
          b = l ? item_next(l) : 0;
        } else {	// go up
          t = l;
          b = selection_;
        }
        for (; t && t != b; t = item_next(t)) {
          char change_t;
          change_t = select(t, whichway, 0);
          if (wp.deleted()) return 1;
          change |= change_t;
          if (change_t && (when() & fltk3::WHEN_CHANGED)) {
            set_changed();
            do_callback();
            if (wp.deleted()) return 1;
          }
        }
        if (l) selection_ = l;
      } else {
        void* l1 = selection_;
        void* l =
	(fltk3::event_x()<x() || fltk3::event_x()>x()+w()) ? selection_ :
	find_item(my);
        change = (l != l1);
        select_only(l, when() & fltk3::WHEN_CHANGED);
        if (wp.deleted()) return 1;
      }
      py = my;
      return 1;
    case fltk3::RELEASE:
      if (type() == fltk3::SELECT_BROWSER) {
        void* t = selection_;
        deselect();
        if (wp.deleted()) return 1;
        selection_ = t;
      }
      if (change) {
        set_changed();
        if (when() & fltk3::WHEN_RELEASE) do_callback();
      } else {
        if (when() & fltk3::WHEN_NOT_CHANGED) do_callback();
      }
      if (wp.deleted()) return 1;
      
      // double click calls the callback: (like Enter Key)
      if (fltk3::event_clicks() && (when() & fltk3::WHEN_ENTER_KEY)) {
        set_changed();
        do_callback();
      }
      return 1;
    case fltk3::FOCUS:
    case fltk3::UNFOCUS:
      if (type() >= fltk3::HOLD_BROWSER && fltk3::visible_focus()) {
        redraw();
        return 1;
      } else return 0;
  }
  
  return 0;
}


// Documented in the header file
fltk3::Browser_::Browser_(int X, int Y, int W, int H, const char* L)
: fltk3::Group(X, Y, W, H, L),
  scrollbar(0, 0, 0, 0, 0), // they will be resized by draw()
  hscrollbar(0, 0, 0, 0, 0)
{
  box(fltk3::NO_BOX);
  align(fltk3::ALIGN_BOTTOM);
  position_ = real_position_ = 0;
  hposition_ = real_hposition_ = 0;
  offset_ = 0;
  top_ = 0;
  when(fltk3::WHEN_RELEASE_ALWAYS);
  selection_ = 0;
  color(fltk3::BACKGROUND2_COLOR, fltk3::SELECTION_COLOR);
  scrollbar.callback(scrollbar_callback);
  //scrollbar.align(fltk3::ALIGN_LEFT|fltk3::ALIGN_BOTTOM); // back compatibility?
  hscrollbar.callback(hscrollbar_callback);
  hscrollbar.type(fltk3::HORIZONTAL);
  textfont(fltk3::HELVETICA);
  textsize(fltk3::NORMAL_SIZE);
  textcolor(fltk3::FOREGROUND_COLOR);
  has_scrollbar_ = BOTH;
  max_width = 0;
  max_width_item = 0;
  scrollbar_size_ = 0;
  redraw1 = redraw2 = 0;
  end();
}


// Documented in the header file
void fltk3::Browser_::sort(int flags) {
  //
  // Simple bubble sort - pure lazyness on my side.
  //
  int i, j, n = -1, desc = ((flags&fltk3::SORT_DESCENDING)==fltk3::SORT_DESCENDING);
  void *a =item_first(), *b, *c;
  if (!a) return;
  while (a) {
    a = item_next(a);
    n++;
  }
  for (i=n; i>0; i--) {
    char swapped = 0;
    a = item_first();
    b = item_next(a);
    for (j=0; j<i; j++) {
      const char *ta = item_text(a);
      const char *tb = item_text(b);
      c = item_next(b);
      if (desc) {
        if (strcmp(ta, tb)<0) {
          item_swap(a, b);
          swapped = 1;
        }
      } else {
        if (strcmp(ta, tb)>0) {
          item_swap(a, b);
          swapped = 1;
        }
      }
      if (!c) break;
      b = c; a = item_prev(b);
    }
    if (!swapped)
      break;
  }
}


// Default versions of some of the virtual functions:


// Documented in the header file
int fltk3::Browser_::item_quick_height(void* item) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, item_quick_height(item), BrowserItemQuickHeight)
  return item_height(item);
}


// Documented in the header file
int fltk3::Browser_::incr_height() const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, incr_height(), BrowserIncrHeight)
  return item_quick_height(item_first());
}


// Documented in the header file
int fltk3::Browser_::full_height() const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, full_height(), BrowserFullHeight)
  int t = 0;
  for (void* p = item_first(); p; p = item_next(p))
    t += item_quick_height(p);
  return t;
}


// Documented in the header file
int fltk3::Browser_::full_width() const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, full_width(), BrowserFullWidth)
  return max_width;
}


// Documented in the header file
void fltk3::Browser_::item_select(void *item, int val) {
  FLTK3_WIDGET_VCALLS_WRAPPER(item_select(item, val), BrowserItemSelect)
}


// Documented in the header file
int fltk3::Browser_::item_selected(void* item) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, item_selected(item), BrowserItemSelected)
  return item==selection_ ? 1 : 0; 
}


// Documented in the header file
void *fltk3::Browser_::item_first() const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(void*, item_first(), BrowserItemFirst)
  return 0;
}


// Documented in the header file
void *fltk3::Browser_::item_next(void *item) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(void*, item_next(item), BrowserItemNext)
  return 0;
}


// Documented in the header file
void *fltk3::Browser_::item_prev(void *item) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(void*, item_prev(item), BrowserItemPrev)
  return 0;
}


// Documented in the header file
void *fltk3::Browser_::item_last() const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(void*, item_last(), BrowserItemLast)
  return 0;
}


// Documented in the header file
int fltk3::Browser_::item_height(void *item) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, item_height(item), BrowserItemHeight)
  return 0;
}


// Documented in the header file
int fltk3::Browser_::item_width(void *item) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, item_width(item), BrowserItemWidth)
  return 0;
}


// Documented in the header file
void fltk3::Browser_::item_draw(void *item,int X,int Y,int W,int H) const {
  FLTK3_WIDGET_VCALLS_WRAPPER(item_draw(item, X, Y, W, H), BrowserItemDraw)
}


// Documented in the header file
const char *fltk3::Browser_::item_text(void *item) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(const char *, item_text(item), BrowserItemText)
  return 0L;
}


// Documented in the header file
void fltk3::Browser_::item_swap(void *a,void *b) {
  FLTK3_WIDGET_VCALLS_WRAPPER(item_swap(a, b), BrowserItemSwap)
}


// Documented in the header file
void *fltk3::Browser_::item_at(int index) const {
  FLTK3_WIDGET_VCALLS_WRAPPER_RET(void *, item_at(index), BrowserItemAt)
  return 0L; 
}


//
// End of "$Id$".
//
