//
// "$Id$"
//
// Tab widget for the Fast Light Tool Kit (FLTK).
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


// This is the "file card tabs" interface to allow you to put lots and lots
// of buttons and switches in a panel, as popularized by many toolkits.

// Each child widget is a card, and its label() is printed on the card tab.
// Clicking the tab makes that card visible.

#include <stdio.h>
#include <fltk3/run.h>
#include <fltk3/TabGroup.h>
#include <fltk3/draw.h>
#include <fltk3/Tooltip.h>

#define BORDER 2
#define EXTRASPACE 10
#define SELECTION_BORDER 5

// Return the left edges of each tab (plus a fake left edge for a tab
// past the right-hand one).  These positions are actually of the left
// edge of the slope.  They are either separated by the correct distance
// or by EXTRASPACE or by zero.
// These positions are updated in the private arrays tab_pos[] and
// tab_width[], resp.. If needed, these arrays are (re)allocated.
// Return value is the index of the selected item.

int fltk3::TabGroup::tab_positions() {
  int nc = children();
  if (nc != tab_count) {
    clear_tab_positions();
    if (nc) {
      tab_pos   = (int*)malloc((nc+1)*sizeof(int));
      tab_width = (int*)malloc((nc+1)*sizeof(int));
    }
    tab_count = nc;
  }
  if (nc == 0) return 0;
  int selected = 0;
  fltk3::Widget*const* a = array();
  int i;
  char prev_draw_shortcut = fltk3::draw_shortcut;
  fltk3::draw_shortcut = 1;

  tab_pos[0] = fltk3::box_dx(box());
  for (i=0; i<nc; i++) {
    fltk3::Widget* o = *a++;
    if (o->visible()) selected = i;

    int wt = 0; int ht = 0;
    o->measure_label(wt,ht);

    tab_width[i] = wt + EXTRASPACE;
    tab_pos[i+1] = tab_pos[i] + tab_width[i] + BORDER;
  }
  fltk3::draw_shortcut = prev_draw_shortcut;

  int r = w();
  if (tab_pos[i] <= r) return selected;
  // uh oh, they are too big:
  // pack them against right edge:
  tab_pos[i] = r;
  for (i = nc; i--;) {
    int l = r-tab_width[i];
    if (tab_pos[i+1] < l) l = tab_pos[i+1];
    if (tab_pos[i] <= l) break;
    tab_pos[i] = l;
    r -= EXTRASPACE;
  }
  // pack them against left edge and truncate width if they still don't fit:
  for (i = 0; i<nc; i++) {
    if (tab_pos[i] >= i*EXTRASPACE) break;
    tab_pos[i] = i*EXTRASPACE;
    int W = w()-1-EXTRASPACE*(children()-i) - tab_pos[i];
    if (tab_width[i] > W) tab_width[i] = W;
  }
  // adjust edges according to visiblity:
  for (i = nc; i > selected; i--) {
    tab_pos[i] = tab_pos[i-1] + tab_width[i-1];
  }
  return selected;
}

// Returns space (height) in pixels needed for tabs. Negative to put them on the bottom.
// Returns full height, if children() = 0.
int fltk3::TabGroup::tab_height() {
  if (children() == 0) return h();
  int H = h();
  int H2 = y();
  fltk3::Widget*const* a = array();
  for (int i=children(); i--;) {
    fltk3::Widget* o = *a++;
    if (o->y() < y()+H) H = o->y()-y();
    if (o->y()+o->h() > H2) H2 = o->y()+o->h();
  }
  H2 = y()+h()-H2;
  if (H2 > H) return (H2 <= 0) ? 0 : -H2;
  else return (H <= 0) ? 0 : H;
}

// This is used for event handling (clicks) and by fluid to pick tabs.
// Returns 0, if children() = 0, or if the event is outside of the tabs area.
fltk3::Widget *fltk3::TabGroup::which(int event_x, int event_y) {
  if (children() == 0) return 0;
  int H = tab_height();
  if (H < 0) {
    if (event_y > y()+h() || event_y < y()+h()+H) return 0;
  } else {
    if (event_y > y()+H || event_y < y()) return 0;
  }
  if (event_x < x()) return 0;
  fltk3::Widget *ret = 0L;
  int nc = children();
  tab_positions();
  for (int i=0; i<nc; i++) {
    if (event_x < x()+tab_pos[i+1]) {
      ret = child(i);
      break;
    }
  }
  return ret;
}

void fltk3::TabGroup::redraw_tabs()
{
  int H = tab_height();
  if (H >= 0) {
    H += fltk3::box_dy(box());
    damage(fltk3::DAMAGE_SCROLL, x(), y(), w(), H);
  } else {
    H = fltk3::box_dy(box()) - H;
    damage(fltk3::DAMAGE_SCROLL, x(), y() + h() - H, w(), H);
  }
}

int fltk3::TabGroup::handle(int event) {
  fltk3::Widget *o;
  int i;

  switch (event) {

  case fltk3::PUSH: {
    int H = tab_height();
    if (H >= 0) {
      if (fltk3::event_y() > y()+H) return Group::handle(event);
    } else {
      if (fltk3::event_y() < y()+h()+H) return Group::handle(event);
    }}
    /* FALLTHROUGH */
  case fltk3::DRAG:
  case fltk3::RELEASE:
    o = which(fltk3::event_x(), fltk3::event_y());
    if (event == fltk3::RELEASE) {
      push(0);
      if (o && fltk3::visible_focus() && fltk3::focus()!=this) { 
        fltk3::focus(this);
        redraw_tabs();
      }
      if (o && value(o)) {
        fltk3::WidgetTracker wp(o);
        set_changed();
	do_callback();
	if (wp.deleted()) return 1;
      }
      fltk3::Tooltip::current(o);
    } else {
      push(o);
    }
    return 1;
  case fltk3::MOVE: {
    int ret = Group::handle(event);
    fltk3::Widget *o = fltk3::Tooltip::current(), *n = o;
    int H = tab_height();
    if ( (H>=0) && (fltk3::event_y()>y()+H) )
      return ret;
    else if ( (H<0) && (fltk3::event_y() < y()+h()+H) )
      return ret;
    else { 
      n = which(fltk3::event_x(), fltk3::event_y());
      if (!n) n = this;
    }
    if (n!=o)
      fltk3::Tooltip::enter(n);
    return ret; }
  case fltk3::FOCUS:
  case fltk3::UNFOCUS:
    if (!fltk3::visible_focus()) return Group::handle(event);
    if (fltk3::event() == fltk3::RELEASE ||
	fltk3::event() == fltk3::SHORTCUT ||
	fltk3::event() == fltk3::KEYBOARD ||
	fltk3::event() == fltk3::FOCUS ||
	fltk3::event() == fltk3::UNFOCUS) {
      redraw_tabs();
      if (fltk3::event() == fltk3::FOCUS) return Group::handle(event);
      if (fltk3::event() == fltk3::UNFOCUS) return 0;
      else return 1;
    } else return Group::handle(event);
  case fltk3::KEYBOARD:
    switch (fltk3::event_key()) {
      case fltk3::LeftKey:
        if (child(0)->visible()) return 0;
	for (i = 1; i < children(); i ++)
	  if (child(i)->visible()) break;
	value(child(i - 1));
	set_changed();
	do_callback();
        return 1;
      case fltk3::RightKey:
        if (child(children() - 1)->visible()) return 0;
	for (i = 0; i < children(); i ++)
	  if (child(i)->visible()) break;
	value(child(i + 1));
	set_changed();
	do_callback();
        return 1;
      case fltk3::DownKey:
        redraw();
        return Group::handle(fltk3::FOCUS);
      default:
        break;
    }
    return Group::handle(event);
  case fltk3::SHORTCUT:
    for (i = 0; i < children(); ++i) {
      fltk3::Widget *c = child(i);
      if (c->test_shortcut(c->label())) {
        char sc = !c->visible();
        value(c);
        if (sc) set_changed();
        do_callback();
        return 1;
      }
    }
    return Group::handle(event);
  case fltk3::SHOW:
    value(); // update visibilities and fall through
  default:
    return Group::handle(event);

  }
}

int fltk3::TabGroup::push(fltk3::Widget *o) {
  if (push_ == o) return 0;
  if ( (push_ && !push_->visible()) || (o && !o->visible()) )
    redraw_tabs();
  push_ = o;
  return 1;
}

/**  
   Gets the currently visible widget/tab.
   The value() is the first visible child (or the last child if none
   are visible) and this also hides any other children.
   This allows the tabs to be deleted, moved to other groups, and
   show()/hide() called without it screwing up.
*/
fltk3::Widget* fltk3::TabGroup::value() {
  fltk3::Widget* v = 0;
  fltk3::Widget*const* a = array();
  for (int i=children(); i--;) {
    fltk3::Widget* o = *a++;
    if (v) o->hide();
    else if (o->visible()) v = o;
    else if (!i) {o->show(); v = o;}
  }
  return v;
}

/**
  Sets the widget to become the current visible widget/tab.
  Setting the value hides all other children, and makes this one
  visible, if it is really a child.
*/
int fltk3::TabGroup::value(fltk3::Widget *newvalue) {
  fltk3::Widget*const* a = array();
  int ret = 0;
  for (int i=children(); i--;) {
    fltk3::Widget* o = *a++;
    if (o == newvalue) {
      if (!o->visible()) ret = 1;
      o->show();
    } else {
      o->hide();
    }
  }
  return ret;
}

enum {LEFT, RIGHT, SELECTED};

void fltk3::TabGroup::draw() {
  fltk3::Widget *v = value();
  int H = tab_height();

  if (damage() & fltk3::DAMAGE_ALL) { // redraw the entire thing:
    fltk3::Color c = v ? v->color() : color();

    draw_box(box(), x(), y()+(H>=0?H:0), w(), h()-(H>=0?H:-H), c);

    if (selection_color() != c) {
      // Draw the top or bottom SELECTION_BORDER lines of the tab pane in the
      // selection color so that the user knows which tab is selected...
      int clip_y = (H >= 0) ? y() + H : y() + h() + H - SELECTION_BORDER;
      fltk3::push_clip(x(), clip_y, w(), SELECTION_BORDER);
      draw_box(box(), x(), clip_y, w(), SELECTION_BORDER, selection_color());
      fltk3::pop_clip();
    }
    if (v) draw_child(*v);
  } else { // redraw the child
    if (v) update_child(*v);
  }
  if (damage() & (fltk3::DAMAGE_SCROLL|fltk3::DAMAGE_ALL)) {
    int nc = children();
    int selected = tab_positions();
    int i;
    fltk3::Widget*const* a = array();
    for (i=0; i<selected; i++)
      draw_tab(x()+tab_pos[i], x()+tab_pos[i+1],
               tab_width[i], H, a[i], LEFT);
    for (i=nc-1; i > selected; i--)
      draw_tab(x()+tab_pos[i], x()+tab_pos[i+1],
               tab_width[i], H, a[i], RIGHT);
    if (v) {
      i = selected;
      draw_tab(x()+tab_pos[i], x()+tab_pos[i+1],
               tab_width[i], H, a[i], SELECTED);
    }
  }
}

void fltk3::TabGroup::draw_tab(int x1, int x2, int W, int H, fltk3::Widget* o, int what) {
  int sel = (what == SELECTED);
  int dh = fltk3::box_dh(box());
  int dy = fltk3::box_dy(box());
  char prev_draw_shortcut = fltk3::draw_shortcut;
  fltk3::draw_shortcut = 1;

  fltk3::Boxtype bt = (o==push_ &&!sel) ? fltk3::down(box()) : box();

  // compute offsets to make selected tab look bigger
  int yofs = sel ? 0 : BORDER;

  if ((x2 < x1+W) && what == RIGHT) x1 = x2 - W;

  if (H >= 0) {
    if (sel) fltk3::push_clip(x1, y(), x2 - x1, H + dh - dy);
    else fltk3::push_clip(x1, y(), x2 - x1, H);

    H += dh;

    fltk3::Color c = sel ? selection_color() : o->selection_color();

    draw_box(bt, x1, y() + yofs, W, H + 10 - yofs, c);

    // Save the previous label color
    fltk3::Color oc = o->labelcolor();

    // Draw the label using the current color...
    o->labelcolor(sel ? labelcolor() : o->labelcolor());    
    o->draw_label(x1, y() + yofs, W, H - yofs, fltk3::ALIGN_CENTER);

    // Restore the original label color...
    o->labelcolor(oc);

    if (fltk3::focus() == this && o->visible())
      draw_focus(box(), x1, y(), W, H);

    fltk3::pop_clip();
  } else {
    H = -H;

    if (sel) fltk3::push_clip(x1, y() + h() - H - dy, x2 - x1, H + dy);
    else fltk3::push_clip(x1, y() + h() - H, x2 - x1, H);

    H += dh;

    fltk3::Color c = sel ? selection_color() : o->selection_color();

    draw_box(bt, x1, y() + h() - H - 10, W, H + 10 - yofs, c);

    // Save the previous label color
    fltk3::Color oc = o->labelcolor();

    // Draw the label using the current color...
    o->labelcolor(sel ? labelcolor() : o->labelcolor());
    o->draw_label(x1, y() + h() - H, W, H - yofs, fltk3::ALIGN_CENTER);

    // Restore the original label color...
    o->labelcolor(oc);

    if (fltk3::focus() == this && o->visible())
      draw_focus(box(), x1, y() + h() - H, W, H);

    fltk3::pop_clip();
  }
  fltk3::draw_shortcut = prev_draw_shortcut;
}

/**
    Creates a new fltk3::TabGroup widget using the given position, size,
    and label string. The default boxtype is fltk3::THIN_UP_BOX.

    Use add(fltk3::Widget*) to add each child, which are usually
    fltk3::Group widgets. The children should be sized to stay
    away from the top or bottom edge of the fltk3::TabGroup widget,
    which is where the tabs will be drawn.

    All children of fltk3::TabGroup should have the same size and exactly fit on top of 
    each other. They should only leave space above or below where that tabs will 
    go, but not on the sides. If the first child of fltk3::TabGroup is set to 
    "resizable()", the riders will not resize when the tabs are resized.

    The destructor <I>also deletes all the children</I>. This
    allows a whole tree to be deleted at once, without having to
    keep a pointer to all the children in the user code. A kludge
    has been done so the fltk3::TabGroup and all of its children
    can be automatic (local) variables, but you must declare the
    fltk3::TabGroup widget <I>first</I> so that it is destroyed last.
*/
fltk3::TabGroup::TabGroup(int X,int Y,int W, int H, const char *l) :
  fltk3::Group(X,Y,W,H,l)
{
  box(fltk3::THIN_UP_BOX);
  push_ = 0;
  tab_pos = 0;
  tab_width = 0;
  tab_count = 0;
}

fltk3::TabGroup::~TabGroup() {
  clear_tab_positions();
}

/**
    Returns the position and size available to be used by its children.

    If there isn't any child yet the \p tabh parameter will be used to
    calculate the return values. This assumes that the children's labelsize
    is the same as the fltk3::TabGroup' labelsize and adds a small border.

    If there are already children, the values of child(0) are returned, and
    \p tabh is ignored.

    \note Children should always use the same positions and sizes.

    \p tabh can be one of
    \li    0: calculate label size, tabs on top
    \li   -1: calculate label size, tabs on bottom
    \li >  0: use given \p tabh value, tabs on top (height = tabh)
    \li < -1: use given \p tabh value, tabs on bottom (height = -tabh)

    \param[in]	tabh		position and optional height of tabs (see above)
    \param[out]	rx,ry,rw,rh	(x,y,w,h) of client area for children

    \since	FLTK 1.3.0
*/
void fltk3::TabGroup::client_area(int &rx, int &ry, int &rw, int &rh, int tabh) {

  if (children()) {			// use existing values

    rx = child(0)->x();
    ry = child(0)->y();
    rw = child(0)->w();
    rh = child(0)->h();

  } else {				// calculate values

    int y_offset;
    int label_height = fltk3::height(labelfont(), labelsize()) + BORDER*2;

    if (tabh == 0)			// use default (at top)
      y_offset = label_height;
    else if (tabh == -1)	 	// use default (at bottom)
      y_offset = -label_height;
    else
      y_offset = tabh;			// user given value

    rx = x();
    rw = w();

    if (y_offset >= 0) {		// labels at top
      ry = y() + y_offset;
      rh = h() - y_offset;
    } else {				// labels at bottom
      ry = y();
      rh = h() + y_offset;
    }
  }
}

void fltk3::TabGroup::clear_tab_positions() {
  if (tab_pos) {
    free(tab_pos);
    tab_pos = 0;
  }
  if (tab_width){
    free(tab_width);
    tab_width = 0;
  }
}

//
// End of "$Id$".
//
