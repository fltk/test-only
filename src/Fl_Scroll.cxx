//
// "$Id: Fl_Scroll.cxx,v 1.45 2005/01/24 08:07:44 spitzak Exp $"
//
// Scroll widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/ScrollGroup.h>
#include <fltk/events.h>
#include <fltk/layout.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <config.h>
using namespace fltk;

#if USE_CLIPOUT
extern Widget* fl_did_clipping;
#endif

void ScrollGroup::draw_clip(void* v, const Rectangle& r) {
  push_clip(r);
  ScrollGroup* s = (ScrollGroup*)v;
#if !USE_CLIPOUT
  // fill the rest of the region with color:
  setcolor(s->color()); fillrect(r);
#endif
  // draw all the children, clipping them out of the region:
  int numchildren = s->children(); int i;
  for (i = numchildren; i--;) {
    Widget& w = *s->child(i);
    // Partial-clipped children with their own damage will still need
    // to be redrawn before the scroll is finished drawing.  Don't clear
    // their damage in this case:
    uchar save = 0;
    if (!(s->damage()&DAMAGE_ALL)) {
      if (w.x() < r.x() || w.y() < r.y() ||
	  w.r() > r.r() || w.b() > r.b())
	save = w.damage();
    }
#if USE_CLIPOUT
    fl_did_clipping = 0;
    s->draw_child(w);
    if (fl_did_clipping != &w) clipout(w);
#else
    s->draw_child(w);
#endif
    w.set_damage(save);
  }
#if USE_CLIPOUT
  // fill the rest of the region with color:
  setcolor(s->color()); fillrect(r);
#endif
  // draw the outside labels:
  for (i = numchildren; i--;)
    s->draw_outside_label(*s->child(i));
  pop_clip();
}

void ScrollGroup::bbox(Rectangle& r) {
  r.set(0,0,w(),h()); box()->inset(r);
  if (scrollbar.visible()) {
    if (scrollbar_align() & ALIGN_LEFT) r.move_x(scrollbar.w());
    else r.move_r(-scrollbar.w());
  }
  if (hscrollbar.visible()) {
    if (scrollbar_align() & ALIGN_TOP) r.move_y(hscrollbar.h());
    else r.move_b(-hscrollbar.h());
  }
}

void ScrollGroup::draw() {
  Rectangle r; bbox(r);

  uchar d = damage();
  if (d & DAMAGE_ALL) { // full redraw
    draw_frame();
    draw_clip(this, r);
  } else {
    if (scrolldx || scrolldy) {
      scrollrect(r, scrolldx, scrolldy, draw_clip, this);
    }
    if (d & DAMAGE_CHILD) { // draw damaged children
      push_clip(r);
      for (int i = children(); i--;) {
	Widget& w = *child(i);
	if (w.damage() & DAMAGE_CHILD_LABEL) {
	  draw_outside_label(w);
	  w.set_damage(w.damage() & ~DAMAGE_CHILD_LABEL);
	}
	update_child(w);
      }
      pop_clip();
    }
  }
  scrolldx = scrolldy = 0;

  // draw the scrollbars:
  if (d & DAMAGE_ALL) {
    scrollbar.set_damage(DAMAGE_ALL);
    hscrollbar.set_damage(DAMAGE_ALL);
    if (scrollbar.visible() && hscrollbar.visible()) {
      // fill in the little box in the corner
      setcolor(buttoncolor());
      fillrect(Rectangle(scrollbar.x(),hscrollbar.y(),scrollbar.w(),hscrollbar.h()));
    }
  }
  update_child(scrollbar);
  update_child(hscrollbar);
}

void ScrollGroup::layout() {

  // move all the children and accumulate their bounding boxes:
  int dx = layoutdx;
  int dy = layoutdy;
  layoutdx = layoutdy = 0;
  scrolldx += dx;
  scrolldy += dy;
  int l = w();
  int r = 0;
  int t = h();
  int b = 0;
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Widget* o = child(i);
    o->position(o->x()+dx, o->y()+dy);
    o->layout();
    if (o->x() < l) l = o->x();
    if (o->y() < t) t = o->y();
    if (o->x()+o->w() > r) r = o->x()+o->w();
    if (o->y()+o->h() > b) b = o->y()+o->h();
  }

  const int sw = scrollbar_width();

  // See if children would fit if we had no scrollbars...
  Rectangle R(w(),h()); box()->inset(R);
  int vneeded = 0;
  int hneeded = 0;
  if (type() & VERTICAL) {
    if ((type() & ALWAYS_ON) || t < R.y() || b > R.b()) {
      vneeded = 1;
      if (scrollbar_align() & ALIGN_LEFT) R.move_x(sw);
      else R.move_r(-sw);
    }
  }

  if (type() & HORIZONTAL) {
    if ((type() & ALWAYS_ON) || l < R.x() || r > R.r()) {
      hneeded = 1;
      if (scrollbar_align() & ALIGN_TOP) R.move_y(sw);
      else R.move_b(-sw);
      // recheck vertical since we added a horizontal scrollbar
      if (!vneeded && (type() & VERTICAL)) {
	if (t < R.y() || b > R.b()) {
	  vneeded = 1;
	  if (scrollbar_align() & ALIGN_LEFT) R.move_x(sw);
	  else R.move_r(-sw);
	}
      }
    }
  }
  // Now that we know what's needed, make it so.
  if (vneeded) {
    if (!scrollbar.visible()) {
      scrollbar.set_visible();
      redraw(DAMAGE_ALL);
    }
  } else {
    if (scrollbar.visible()) {
      scrollbar.clear_visible();
      redraw(DAMAGE_ALL);
    }
  }
  if (hneeded) {
    if (!hscrollbar.visible()) {
      hscrollbar.set_visible();
      redraw(DAMAGE_ALL);
    }
  } else {
    if (hscrollbar.visible()) {
      hscrollbar.clear_visible();
      redraw(DAMAGE_ALL);
    }
  }

  scrollbar.resize(scrollbar_align()&ALIGN_LEFT ? R.x()-sw : R.r(), R.y(), sw, R.h());
  scrollbar.value(yposition_ = (R.y()-t), R.h(), 0, b-t);
  hscrollbar.resize(R.x(), scrollbar_align()&ALIGN_TOP ? R.y()-sw : R.b(), R.w(), sw);
  hscrollbar.value(xposition_ = (R.x()-l), R.w(), 0, r-l);

  Widget::layout();
  redraw(DAMAGE_SCROLL);
}

void ScrollGroup::position(int X, int Y) {
  int dx = xposition_-X;
  int dy = yposition_-Y;
  if (!dx && !dy) return;
  xposition_ = X;
  yposition_ = Y;
  layoutdx += dx;
  layoutdy += dy;
  relayout();
}

void ScrollGroup::hscrollbar_cb(Widget* o, void*) {
  ScrollGroup* s = (ScrollGroup*)(o->parent());
  s->position(int(((Scrollbar*)o)->value()), s->yposition());
}

void ScrollGroup::scrollbar_cb(Widget* o, void*) {
  ScrollGroup* s = (ScrollGroup*)(o->parent());
  s->position(s->xposition(), int(((Scrollbar*)o)->value()));
}

#define SLIDER_WIDTH 16

static inline int nogroup(int X) {Group::current(0); return X;}

ScrollGroup::ScrollGroup(int X,int Y,int W,int H,const char* L)
  : Group(X,Y,W,H,L),
    scrollbar(nogroup(X)+W-SLIDER_WIDTH,Y,SLIDER_WIDTH,H-SLIDER_WIDTH),
    hscrollbar(X,Y+H-SLIDER_WIDTH,W-SLIDER_WIDTH,SLIDER_WIDTH) {
  type(BOTH);
  xposition_ = 0;
  yposition_ = 0;
  scrolldx = scrolldy = layoutdx = layoutdy = 0;
  hscrollbar.parent(this);
  hscrollbar.callback(hscrollbar_cb);
  scrollbar.set_vertical();
  scrollbar.parent(this);
  scrollbar.callback(scrollbar_cb);
  Group::current(parent());
}

int ScrollGroup::handle(int event) {
  switch (event) {

  case FOCUS_CHANGE: {
    // The event indicates that the focus changed to a different child,
    // auto-scroll to show it:
    Widget* w = fltk::focus();
    int x = w->x();
    int y = w->y();
    for (Group* p = w->parent(); p != this; p = p->parent()) {
      // if (!p) return 0; // this should never happen
      x += p->x();
      y += p->y();
    }
    Rectangle R; bbox(R);
    int r = x+w->w();
    int dx = 0;
    if (x < R.x()) {dx = R.x()-x; if (r+dx > R.r()) {dx = R.r()-r; if (dx < 0) dx = 0;}}
    else if (r > R.r()) {dx = R.r()-r; if (x+dx < R.x()) {dx = R.x()-x; if (dx > 0) dx = 0;}}
    int b = y+w->h();
    int dy = 0;
    if (y < R.y()) {dy = R.y()-y; if (b+dy > R.b()) {dy = R.b()-b; if (dy < 0) dy = 0;}}
    else if (b > R.b()) {dy = R.b()-b; if (y+dy < R.y()) {dy = R.y()-y; if (dy > 0) dy = 0;}}
    position(xposition_-dx, yposition_-dy);
    break;}

  case PUSH:
  case ENTER:
  case MOVE:
  case DND_ENTER:
  case DND_DRAG:
    // For all mouse events check to see if we are in the scrollbar
    // areas and send to them:
    if (scrollbar.visible() &&
	(scrollbar_align()&ALIGN_LEFT ?
	 (event_x() < scrollbar.x()+scrollbar.w()) :
	 (event_x() >= scrollbar.x())))
      return scrollbar.send(event);
    if (hscrollbar.visible() &&
	(scrollbar_align()&ALIGN_TOP ?
	 (event_y() < hscrollbar.y()+hscrollbar.h()) :
	 (event_y() >= hscrollbar.y())))
      return hscrollbar.send(event);
    break;

  case MOUSEWHEEL:
    return scrollbar.send(event);

  case KEY:
    if (scrollbar.send(event)) return 1;
    if (hscrollbar.send(event)) return 1;
    break;
  }

  return Group::handle(event);
}

//
// End of "$Id: Fl_Scroll.cxx,v 1.45 2005/01/24 08:07:44 spitzak Exp $".
//
