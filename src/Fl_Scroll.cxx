//
// "$Id: Fl_Scroll.cxx,v 1.23 2000/05/17 07:08:08 bill Exp $"
//
// Scroll widget for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>
#include <FL/fl_draw.H>

void Fl_Scroll::draw_clip(void* v,int X, int Y, int W, int H) {
  fl_clip(X,Y,W,H);
  Fl_Scroll* s = (Fl_Scroll*)v;
  // draw all the children, clipping them out of the region:
  int numchildren = s->children(); int i;
  for (i = numchildren; i--;) {
    Fl_Widget& w = *s->child(i);
    if (!fl_not_clipped(w.x(), w.y(), w.w(), w.h())) continue;
    // Partial-clipped children with their own damage will still need
    // to be redrawn before the scroll is finished drawing.  Don't clear
    // their damage in this case:
    uchar save = 0;
    if (!(s->damage()&FL_DAMAGE_ALL)) {
      if (w.x() < X || w.y() < Y ||
	  w.x()+w.w() > X+W || w.y()+w.h() > Y+H)
	save = w.damage();
    }
    w.set_damage(FL_DAMAGE_ALL);
    w.draw_n_clip();
    w.set_damage(save);
  }
  // fill the rest of the region with color:
  fl_color(s->color()); fl_rectf(X,Y,W,H);
  // draw the outside labels:
  for (i = numchildren; i--;)
    s->draw_outside_label(*s->child(i));
  fl_pop_clip();
}

void Fl_Scroll::bbox(int& X, int& Y, int& W, int& H) {
  X = x(); Y = y(); W = w(); H = h(); box()->inset(X,Y,W,H);
  if (scrollbar.visible()) {
    W -= scrollbar.w();
    if (scrollbar.flags() & FL_ALIGN_LEFT) X += scrollbar.w();
  }
  if (hscrollbar.visible()) {
    H -= hscrollbar.h();
    if (scrollbar.flags() & FL_ALIGN_TOP) Y += hscrollbar.h();
  }
}

void Fl_Scroll::draw() {
  int X,Y,W,H; bbox(X,Y,W,H);

  uchar d = damage();
  if (d & FL_DAMAGE_ALL) { // full redraw
    draw_frame();
    draw_clip(this, X, Y, W, H);
  } else {
    if (scrolldx || scrolldy) {
      fl_scroll(X, Y, W, H, scrolldx, scrolldy, draw_clip, this);
    }
    if (d & FL_DAMAGE_CHILD) { // draw damaged children
      fl_clip(X, Y, W, H);
      for (int i = children(); i--;) {
	Fl_Widget& w = *child(i);
	if (w.damage() & FL_DAMAGE_CHILD_LABEL) {
	  draw_outside_label(w);
	  w.set_damage(w.damage() & ~FL_DAMAGE_CHILD_LABEL);
	}
	update_child(w);
      }
      fl_pop_clip();
    }
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

void Fl_Scroll::layout() {

  int X,Y,W,H; bbox(X,Y,W,H);

  // move all the children and accumulate their bounding boxes:
  int l = X; int r = X; int t = Y; int b = Y;
  int dx = layoutdx + ox() - x();
  int dy = layoutdy + oy() - y();
  layoutdx = layoutdy = 0;
  scrolldx += dx;
  scrolldy += dy;
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Fl_Widget* o = child(i);
    o->position(o->x()+dx, o->y()+dy);
    o->layout();
    if (o->x() < l) l = o->x();
    if (o->y() < t) t = o->y();
    if (o->x()+o->w() > r) r = o->x()+o->w();
    if (o->y()+o->h() > b) b = o->y()+o->h();
  }

  // turn the scrollbars on and off as necessary:
  for (int z = 0; z<2; z++) {
    if ((type()&VERTICAL) && (type()&ALWAYS_ON || t < Y || b > Y+H)) {
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
    if ((type()&HORIZONTAL) && (type()&ALWAYS_ON || l < X || r > X+W)) {
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
  scrollbar.value(yposition_ = (Y-t), H, 0, b-t);

  hscrollbar.resize(X,
		    scrollbar.flags()&FL_ALIGN_TOP ? Y-hscrollbar.h() : Y+H,
		    W, hscrollbar.h());
  hscrollbar.value(xposition_ = (X-l), W, 0, r-l);
  set_old_size();
  Fl_Widget::layout();
  damage(FL_DAMAGE_SCROLL);
}

void Fl_Scroll::position(int X, int Y) {
  int dx = xposition_-X;
  int dy = yposition_-Y;
  if (!dx && !dy) return;
  xposition_ = X;
  yposition_ = Y;
  layoutdx += dx;
  layoutdy += dy;
  relayout();
}

void Fl_Scroll::hscrollbar_cb(Fl_Widget* o, void*) {
  Fl_Scroll* s = (Fl_Scroll*)(o->parent());
  s->position(int(((Fl_Scrollbar*)o)->value()), s->yposition());
}

void Fl_Scroll::scrollbar_cb(Fl_Widget* o, void*) {
  Fl_Scroll* s = (Fl_Scroll*)(o->parent());
  s->position(s->xposition(), int(((Fl_Scrollbar*)o)->value()));
}

#define SLIDER_WIDTH 16

Fl_Scroll::Fl_Scroll(int X,int Y,int W,int H,const char* L)
  : Fl_Group(X,Y,W,H,L), endgroup(0),
    scrollbar(X+W-SLIDER_WIDTH,Y,SLIDER_WIDTH,H-SLIDER_WIDTH),
    hscrollbar(X,Y+H-SLIDER_WIDTH,W-SLIDER_WIDTH,SLIDER_WIDTH) {
  type(BOTH);
  xposition_ = 0;
  yposition_ = 0;
  scrolldx = scrolldy = layoutdx = layoutdy = 0;
  hscrollbar.parent(this);
  hscrollbar.type(FL_HORIZONTAL);
  hscrollbar.callback(hscrollbar_cb);
  scrollbar.parent(this);
  scrollbar.callback(scrollbar_cb);
  Fl_Group::current(this);
}

int Fl_Scroll::handle(int event) {
  switch (event) {

  case FL_FOCUS:
    if (contains(Fl::focus())) {
      // The event indicates that the focus changed to a different child,
      // auto-scroll to show it:
      Fl_Widget* w = Fl::focus();
      int X,Y,R,B; bbox(X,Y,R,B); R += X; B += Y;
      int x = w->x();
      int r = x+w->w();
      int dx = 0;
      if (x < X) {dx = X-x; if (r+dx > R) {dx = R-r; if (dx < 0) dx = 0;}}
      else if (r > R) {dx = R-r; if (x+dx < X) {dx = X-x; if (dx > 0) dx = 0;}}
      int y = w->y();
      int b = y+w->h();
      int dy = 0;
      if (y < Y) {dy = Y-y; if (b+dy > B) {dy = B-b; if (dy < 0) dy = 0;}}
      else if (b > B) {dy = B-b; if (y+dy < Y) {dy = Y-y; if (dy > 0) dy = 0;}}
      position(xposition_-dx+layoutdx, yposition_-dy+layoutdy);
    }
    break;

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
    break;

  }

  if (Fl_Group::handle(event)) return 1;

  if (event == FL_SHORTCUT) {
    if (send(FL_KEYBOARD, scrollbar)) return 1;
    if (send(FL_KEYBOARD, hscrollbar)) return 1;
  }
  return 0;
}

//
// End of "$Id: Fl_Scroll.cxx,v 1.23 2000/05/17 07:08:08 bill Exp $".
//
