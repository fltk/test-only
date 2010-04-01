//
// "$Id$"
//
// Tab widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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
#include <fltk3/Fl_Tabs.H>
#include <fltk3/fl_draw.H>
#include <fltk3/Tooltip.h>

#define BORDER 2
#define EXTRASPACE 10

// return the left edges of each tab (plus a fake left edge for a tab
// past the right-hand one).  These position are actually of the left
// edge of the slope.  They are either separated by the correct distance
// or by EXTRASPACE or by zero.
// Return value is the index of the selected item.

int Fl_Tabs::tab_positions(int* p, int* wp) {
  int selected = 0;
  fltk3::Widget*const* a = array();
  int i;
  char prev_draw_shortcut = fl_draw_shortcut;
  fl_draw_shortcut = 1;

  p[0] = fltk3::box_dx(box());
  for (i=0; i<children(); i++) {
    fltk3::Widget* o = *a++;
    if (o->visible()) selected = i;

    int wt = 0; int ht = 0;
    o->measure_label(wt,ht);

    wp[i]  = wt+EXTRASPACE;
    p[i+1] = p[i]+wp[i]+BORDER;
  }
  fl_draw_shortcut = prev_draw_shortcut;

  int r = w();
  if (p[i] <= r) return selected;
  // uh oh, they are too big:
  // pack them against right edge:
  p[i] = r;
  for (i = children(); i--;) {
    int l = r-wp[i];
    if (p[i+1] < l) l = p[i+1];
    if (p[i] <= l) break;
    p[i] = l;
    r -= EXTRASPACE;
  }
  // pack them against left edge and truncate width if they still don't fit:
  for (i = 0; i<children(); i++) {
    if (p[i] >= i*EXTRASPACE) break;
    p[i] = i*EXTRASPACE;
    int W = w()-1-EXTRASPACE*(children()-i) - p[i];
    if (wp[i] > W) wp[i] = W;
  }
  // adjust edges according to visiblity:
  for (i = children(); i > selected; i--) {
    p[i] = p[i-1]+wp[i-1];
  }
  return selected;
}

// return space needed for tabs.  Negative to put them on the bottom:
int Fl_Tabs::tab_height() {
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

// this is used by fluid to pick tabs:
fltk3::Widget *Fl_Tabs::which(int event_x, int event_y) {
  int H = tab_height();
  if (H < 0) {
    if (event_y > y()+h() || event_y < y()+h()+H) return 0;
  } else {
    if (event_y > y()+H || event_y < y()) return 0;
  }
  if (event_x < x()) return 0;
  int p[128], wp[128];
  tab_positions(p, wp);
  for (int i=0; i<children(); i++) {
    if (event_x < x()+p[i+1]) return child(i);
  }
  return 0;
}

void Fl_Tabs::redraw_tabs()
{
  int H = tab_height();
  if (H >= 0) {
    H += fltk3::box_dy(box());
    damage(FL_DAMAGE_SCROLL, x(), y(), w(), H);
  } else {
    H = fltk3::box_dy(box()) - H;
    damage(FL_DAMAGE_SCROLL, x(), y() + h() - H, w(), H);
  }
}

int Fl_Tabs::handle(int event) {

  fltk3::Widget *o;
  int i;

  switch (event) {

  case fltk3::PUSH: {
    int H = tab_height();
    if (H >= 0) {
      if (fltk3::event_y() > y()+H) return fltk3::Group::handle(event);
    } else {
      if (fltk3::event_y() < y()+h()+H) return fltk3::Group::handle(event);
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
        Fl_Widget_Tracker wp(o);
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
    int ret = fltk3::Group::handle(event);
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
      if (!fltk3::visible_focus()) return fltk3::Group::handle(event);
    if (fltk3::event() == fltk3::RELEASE ||
	fltk3::event() == fltk3::SHORTCUT ||
	fltk3::event() == fltk3::KEY ||
	fltk3::event() == fltk3::FOCUS ||
	fltk3::event() == fltk3::UNFOCUS) {
      redraw_tabs();
      if (fltk3::event() == fltk3::FOCUS || fltk3::event() == fltk3::UNFOCUS) return 0;
      else return 1;
    } else return fltk3::Group::handle(event);
  case fltk3::KEY:
    switch (fltk3::event_key()) {
      case FL_Left:
        if (child(0)->visible()) return 0;
	for (i = 1; i < children(); i ++)
	  if (child(i)->visible()) break;
	value(child(i - 1));
	set_changed();
	do_callback();
        return 1;
      case FL_Right:
        if (child(children() - 1)->visible()) return 0;
	for (i = 0; i < children(); i ++)
	  if (child(i)->visible()) break;
	value(child(i + 1));
	set_changed();
	do_callback();
        return 1;
      case FL_Down:
        redraw();
        return fltk3::Group::handle(fltk3::FOCUS);
      default:
        break;
    }
      return fltk3::Group::handle(event);
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
      return fltk3::Group::handle(event);
  case fltk3::SHOW:
    value(); // update visibilities and fall through
  default:
      return fltk3::Group::handle(event);

  }
}

int Fl_Tabs::push(fltk3::Widget *o) {
  if (push_ == o) return 0;
  if (push_ && !push_->visible() || o && !o->visible())
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
fltk3::Widget* Fl_Tabs::value() {
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
int Fl_Tabs::value(fltk3::Widget *newvalue) {
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

void Fl_Tabs::draw() {
  fltk3::Widget *v = value();
  int H = tab_height();

  if (damage() & FL_DAMAGE_ALL) { // redraw the entire thing:
    Fl_Color c = v ? v->color() : color();

    draw_box(box(), x(), y()+(H>=0?H:0), w(), h()-(H>=0?H:-H), c);

    if (selection_color() != c) {
      // Draw the top 5 lines of the tab pane in the selection color so
      // that the user knows which tab is selected...
      if (H >= 0) fl_push_clip(x(), y() + H, w(), 5);
      else fl_push_clip(x(), y() + h() - H - 4, w(), 5);

      draw_box(box(), x(), y()+(H>=0?H:0), w(), h()-(H>=0?H:-H),
               selection_color());

      fl_pop_clip();
    }
    if (v) draw_child(*v);
  } else { // redraw the child
    if (v) update_child(*v);
  }
  if (damage() & (FL_DAMAGE_SCROLL|FL_DAMAGE_ALL)) {
    int p[128]; int wp[128];
    int selected = tab_positions(p,wp);
    int i;
    fltk3::Widget*const* a = array();
    for (i=0; i<selected; i++)
      draw_tab(x()+p[i], x()+p[i+1], wp[i], H, a[i], LEFT);
    for (i=children()-1; i > selected; i--)
      draw_tab(x()+p[i], x()+p[i+1], wp[i], H, a[i], RIGHT);
    if (v) {
      i = selected;
      draw_tab(x()+p[i], x()+p[i+1], wp[i], H, a[i], SELECTED);
    }
  }
}

void Fl_Tabs::draw_tab(int x1, int x2, int W, int H, fltk3::Widget* o, int what) {
  int sel = (what == SELECTED);
  int dh = fltk3::box_dh(box());
  int dy = fltk3::box_dy(box());
  char prev_draw_shortcut = fl_draw_shortcut;
  fl_draw_shortcut = 1;

  fltk3::Boxtype bt = (o==push_ &&!sel) ? fl_down(box()) : box();

  // compute offsets to make selected tab look bigger
  int yofs = sel ? 0 : BORDER;

  if ((x2 < x1+W) && what == RIGHT) x1 = x2 - W;

  if (H >= 0) {
    if (sel) fl_push_clip(x1, y(), x2 - x1, H + dh - dy);
    else fl_push_clip(x1, y(), x2 - x1, H);

    H += dh;

    Fl_Color c = sel ? selection_color() : o->selection_color();

    draw_box(bt, x1, y() + yofs, W, H + 10 - yofs, c);

    // Save the previous label color
    Fl_Color oc = o->labelcolor();

    // Draw the label using the current color...
    o->labelcolor(sel ? labelcolor() : o->labelcolor());    
    o->draw_label(x1, y() + yofs, W, H - yofs, fltk3::ALIGN_CENTER);

    // Restore the original label color...
    o->labelcolor(oc);

    if (fltk3::focus() == this && o->visible())
      draw_focus(box(), x1, y(), W, H);

    fl_pop_clip();
  } else {
    H = -H;

    if (sel) fl_push_clip(x1, y() + h() - H - dy, x2 - x1, H + dy);
    else fl_push_clip(x1, y() + h() - H, x2 - x1, H);

    H += dh;

    Fl_Color c = sel ? selection_color() : o->selection_color();

    draw_box(bt, x1, y() + h() - H - 10, W, H + 10 - yofs, c);

    // Save the previous label color
    Fl_Color oc = o->labelcolor();

    // Draw the label using the current color...
    o->labelcolor(sel ? labelcolor() : o->labelcolor());
    o->draw_label(x1, y() + h() - H, W, H - yofs, fltk3::ALIGN_CENTER);

    // Restore the original label color...
    o->labelcolor(oc);

    if (fltk3::focus() == this && o->visible())
      draw_focus(box(), x1, y() + h() - H, W, H);

    fl_pop_clip();
  }
  fl_draw_shortcut = prev_draw_shortcut;
}

/**
    Creates a new Fl_Tabs widget using the given position, size,
    and label string. The default boxtype is fltk3::THIN_UP_BOX.

    <P>Use add(fltk3::Widget
    *) to add each child, which are usually
    fltk3::Group widgets. The children should be sized to stay
    away from the top or bottom edge of the Fl_Tabs widget,
    which is where the tabs will be drawn.

    <P>The destructor <I>also deletes all the children</I>. This
    allows a whole tree to be deleted at once, without having to
    keep a pointer to all the children in the user code. A kludge
    has been done so the Fl_Tabs and all of its children
    can be automatic (local) variables, but you must declare the
    Fl_Tabs widget <I>first</I> so that it is destroyed last.
*/
Fl_Tabs::Fl_Tabs(int X,int Y,int W, int H, const char *l) :
fltk3::Group(X,Y,W,H,l)
{
  box(fltk3::THIN_UP_BOX);
  push_ = 0;
}

//
// End of "$Id$".
//
