//
// "$Id: Fl_Group.cxx,v 1.67 2000/04/24 08:31:25 bill Exp $"
//
// Group widget for the Fast Light Tool Kit (FLTK).
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

// The Fl_Group is the only defined container type in fltk.

// Fl_Window itself is a subclass of this, and most of the event
// handling is designed so windows themselves work correctly.

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <stdlib.h>
#include <FL/Fl_Tooltip.H>

Fl_Group* Fl_Group::current_;

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
}

// This style is unnamed since there is no reason for themes to change it:
static Fl_Named_Style* style = new Fl_Named_Style(0, revert, &style);

Fl_Group::Fl_Group(int X,int Y,int W,int H,const char *l)
: Fl_Widget(X,Y,W,H,l),
  children_(0),
  focus_(0),
  array_(0),
  resizable_(0), // fltk 1.0 used (this)
  sizes_(0), // this is allocated when the group is end()ed.
  ox_(X),
  oy_(Y),
  ow_(W),
  oh_(H) {
  type(FL_GROUP);
  style(::style);
  align(FL_ALIGN_TOP);
  // Subclasses may want to construct child objects as part of their
  // constructor, so make sure they are add()'d to this object.
  // But you must end() the object!
  begin();
}

void Fl_Group::clear() {
  init_sizes();
  if (children_) {
    Fl_Widget*const* a = array_;
    Fl_Widget*const* e = a+children_;
    // clear everything now, in case fl_fix_focus recursively calls us:
    children_ = 0;
    focus_ = 0;
    if (resizable_) resizable_ = this;
    // okay, now it is safe to destroy the children:
    while (e > a) {
      Fl_Widget* o = *--e;
      // This test is to detect if ~Fl_Widget was already called on the
      // child (it set parent to zero).  This is a hack but it makes
      // destruction of composite objects faster:
      if (o->parent() == this) delete o;
    }
    free((void*)a);
  }
}

Fl_Group::~Fl_Group() {clear();}

void Fl_Group::insert(Fl_Widget &o, int index) {
  if (o.parent()) o.parent()->remove(o);
  o.parent(this);
  if (children_ == 0) {
    // allocate for 1 child
    array_ = (Fl_Widget**)malloc(sizeof(Fl_Widget));
    array_[0] = &o;
  } else {
    if (!(children_ & (children_-1))) // double number of children
      array_ = (Fl_Widget**)realloc((void*)array_,
				    2*children_*sizeof(Fl_Widget*));
    for (int j = children_; j > index; --j) array_[j] = array_[j-1];
    array_[index] = &o;
  }
  ++children_;
  init_sizes();
}

void Fl_Group::add(Fl_Widget &o) {insert(o, children_);}

void Fl_Group::remove(int index) {
  if (index >= children_) return;
  Fl_Widget* o = array_[index];
#if 0
// WAS: I removed this to be consistent with others methods that don't
// do redraw().  Not sure what may have relied on it:
  if (o->visible_r()) {
    // we must redraw the enclosing group that has an opaque box:
    for (Fl_Group *p = this; p; p = p->parent())
      if (p->box() != FL_NO_BOX || !p->parent()) {p->redraw(); break;}
  }
#endif
  o->parent(0);
  children_--;
  for (int i=index; i < children_; ++i) array_[i] = array_[i+1];
  init_sizes();
}

void Fl_Group::replace(int index, Fl_Widget& o) {
  if (index >= children_) {add(o); return;}
  o.parent(this);
  array_[index]->parent(0);
  array_[index] = &o;
  init_sizes();
}

int Fl_Group::find(const Fl_Widget* o) const {
  while (o && o->parent() != this) o = o->parent();
  Fl_Widget*const* a = array();
  int i; for (i=0; i < children_; ++i) if (*a++ == o) break;
  return i;
}

////////////////////////////////////////////////////////////////
// Handle

// We must adjust the xy of events sent to child windows so they
// are relative to that window.  All other widgets use absolute
// coordinates:
static int send_i(int event, Fl_Widget& to) {
  if (!to.is_window()) return to.handle(event);
  int save_x = Fl::e_x; Fl::e_x -= to.x();
  int save_y = Fl::e_y; Fl::e_y -= to.y();
  int ret = to.handle(event);
  Fl::e_y = save_y;
  Fl::e_x = save_x;
  return ret;
}

// send(event,o) will decide if object o needs the event and then will
// send it, and does some extra stuff if the widget likes the event.
// Subclasses of Fl_Group can call this for every child.  The return
// value is true if the child "ate" the event, if false you should
// continue trying other children.

int Fl_Group::send(int event, Fl_Widget& to) {
  switch (event) {

  case FL_UNFOCUS:
  case FL_DRAG:
  case FL_RELEASE:
  case FL_LEAVE:
    // These events are sent directly by Fl.cxx to the widgets.  Trying
    // to redirect them is a mistake.  It appears best to ignore attempts.
    return 1; // return 1 so callers stops calling this.

  case FL_FOCUS:
    if (to.takesevents() && send_i(FL_FOCUS, to)) {
      // only call Fl::focus if the child widget did not do so:
      if (!to.contains(Fl::focus())) Fl::focus(&to);
      return 1;
    }
    return 0;

  case FL_ENTER:
  case FL_MOVE:
    // visible() rather than takesevents() so tooltips work on inactive:
    if (to.visible() && Fl::event_inside(to)) {
      // If the mouse is already in a child, just send the move events
      // through.  Fltk does not do this directly like it does for
      // FL_DRAG so that widgets may overlap:
      if (to.contains(Fl::belowmouse())) {
	if (&to != Fl::pushed()) send_i(FL_MOVE, to);
	return 1;
      }
      // Mouse is moving into a new child widget, see if it wants it:
      if (send_i(FL_ENTER, to)) {
	// only call Fl::belowmouse if the child widget did not do so:
	if (!to.contains(Fl::belowmouse())) Fl::belowmouse(to);
	return 1;
      }
    }
    return 0;

  case FL_PUSH:
    // Mouse events are supposedly sent to the belowmouse() widget, but
    // handling them here allows overlapping Fl_Menu_Button widgets to
    // consume some of the buttons:
    if (to.takesevents() && Fl::event_inside(to)) {
      if (send_i(FL_PUSH, to)) {
	// only call Fl::pushed if the child widget did not do so and
	// if the mouse is still down:
	if (Fl::pushed() && !to.contains(Fl::pushed())) Fl::pushed(to);
	return 1;
      }
    }
    return 0;

  case FL_DEACTIVATE:
  case FL_ACTIVATE:
    if (to.active()) send_i(event,to);
    return 0; // return zero so all children are tried

  case FL_SHOW:
  case FL_HIDE:
    if (to.visible()) send_i(event,to);
    return 0; // return zero so all children are tried

  default:
    if (to.takesevents()) return send_i(event, to);
    return 0;
  }
}

// Translate the current keystroke into up/down/left/right for navigation,
// returns zero for non shortcut/keyboard events:
#define ctrl(x) (x^0x40)
int Fl_Group::navigation_key() {
  switch (Fl::event_key()) {
  case 0: // not an FL_KEYBOARD/FL_SHORTCUT event
    break;
  case FL_Tab:
    if (Fl::event_state(FL_CTRL)) return 0; // reserved for Fl_Tabs
    return Fl::event_state(FL_SHIFT) ? FL_Left : FL_Right;
  case FL_Right:
    return FL_Right;
  case FL_Left:
    return FL_Left;
  case FL_Up:
    return FL_Up;
  case FL_Down:
    return FL_Down;
  default:
    switch (Fl::event_text()[0]) {
    case ctrl('N') : return FL_Down;
    case ctrl('P') : return FL_Up;
    case ctrl('F') : return FL_Right;
    case ctrl('B') : return FL_Left;
    }
  }
  return 0;
}

int Fl_Group::handle(int event) {
  const int numchildren = children();
  int i;

  switch (event) {

  case FL_FOCUS:
    if (contains(Fl::focus())) {
      // This is called to indicate that the focus is being set/changed
      focus_ = find(Fl::focus());
      return 1;
    }
    // otherwise it indicates an attempt to give this widget focus:
    switch (navigation_key()) {
    default:
      // try to give it to whatever child had focus last:
      if (focus_ >= 0 && focus_ < children() &&
	  send(event, *child(focus_))) return 1;
      // otherwise fall through to search for first one that wants focus:
    case FL_Right:
    case FL_Down:
      for (i=0; i < numchildren; ++i) if (send(event, *child(i))) return 1;
      return 0;
    case FL_Left:
    case FL_Up:
      break; // default is to try send the focus backwards
    }
    break;

  case FL_ENTER:
  case FL_MOVE:
    for (i = numchildren; i--;) if (send(event, *child(i))) return 1;
    Fl::belowmouse(this);
    return 1;

  case FL_SHORTCUT: {
    // see if any other child widgets want the shortcut, search forward:
    for (i=0; i < numchildren; ++i) if (send(event, *child(i))) return 1;

    // If we don't have focus we don't do any keyboard navigation:
    if (!contains(Fl::focus())) return 0;

    int key = navigation_key(); if (!key) return 0;

    i = focus_;

    // loop from the current focus looking for a new focus, quit when
    // we reach the original again:
    int previous = i;
    Fl_Widget* o = child(i);
    int old_x = o->x();
    int old_r = o->x()+o->w();
    for (;;) {
      switch (key) {
      case FL_Right:
      case FL_Down:
	++i;
	if (i >= children_) {
	  if (parent()) return 0;
	  i = 0;
	}
	break;
      case FL_Left:
      case FL_Up:
	if (i) --i;
	else {
	  if (parent()) return 0;
	  i = children_-1;
	}
	break;
      default:
	return 0;
      }
      if (i == previous) return 0;
      switch (key) {
      case FL_Down:
      case FL_Up:
	// for up/down, the widgets have to overlap horizontally:
	o = child(i);
	if (o->x() >= old_r || o->x()+o->w() <= old_x) continue;
      }
      if (send(FL_FOCUS, *child(i))) return 1;
    }
    return 0;}
  }

  for (i = numchildren; i--;) if (send(event, *child(i))) return 1;
  return 0;
}

////////////////////////////////////////////////////////////////
// Layout

// sizes() array stores the initial positions of widgets as
// left,right,top,bottom quads.  The first quad is the group, the
// second is the resizable (clipped to the group), and the
// rest are the children.  This is a convienent order for the
// algorithim.  If you change this be sure to fix Fl_Tile which
// also uses this array!
// Calling init_sizes() "resets" the sizes array to the current group
// and children positions.  Actually it just deletes the sizes array,
// and it is not recreated until the next time layout is called.

void Fl_Group::init_sizes() {
  delete[] sizes_; sizes_ = 0;
  set_old_size();
}

int* Fl_Group::sizes() {
  if (!sizes_) {
    int* p = sizes_ = new int[4*(children_+2)];
    // first thing in sizes array is the group's size:
    if (!is_window()) {p[0] = ox(); p[2] = oy();} else {p[0] = p[2] = 0;}
    p[1] = p[0]+ow(); p[3] = p[2]+oh();
    // next is the resizable's size:
    p[4] = p[0]; // init to the group's size
    p[5] = p[1];
    p[6] = p[2];
    p[7] = p[3];
    Fl_Widget* r = resizable();
    if (r && r != this) { // then clip the resizable to it
      int t;
      t = r->x(); if (t > p[0]) p[4] = t;
      t +=r->w(); if (t < p[1]) p[5] = t;
      t = r->y(); if (t > p[2]) p[6] = t;
      t +=r->h(); if (t < p[3]) p[7] = t;
    }
    // next is all the children's sizes:
    p += 8;
    Fl_Widget*const* a = array();
    Fl_Widget*const* e = a+children_;
    while (a < e) {
      Fl_Widget* o = *a++;
      *p++ = o->x();
      *p++ = o->x()+o->w();
      *p++ = o->y();
      *p++ = o->y()+o->h();
    }
  }
  return sizes_;
}

void Fl_Group::layout() {
  // get changes from previous position:
  if (!resizable() || ow()==w() && oh()==h()) {
    if (!is_window()) {
      int dx = x()-ox();
      int dy = y()-oy();
      Fl_Widget*const* a = array();
      Fl_Widget*const* e = a+children_;
      while (a < e) {
	Fl_Widget* o = *a++;
	o->resize(o->x()+dx, o->y()+dy, o->w(), o->h());
	o->layout();
      }
    }
  } else if (children_) {
    // get changes in size from the initial size:
    int* p = sizes();
    int dx = x()-p[0];
    int dy = y()-p[2];
    if (is_window()) dx = dy = 0;
    int dw = w()-(p[1]-p[0]);
    int dh = h()-(p[3]-p[2]);
    p+=4;

    // get initial size of resizable():
    int IX = *p++;
    int IR = *p++;
    int IY = *p++;
    int IB = *p++;

    Fl_Widget*const* a = array();
    Fl_Widget*const* e = a+children_;
    while (a < e) {
      Fl_Widget* o = *a++;

      int X = *p++;
      if (X >= IR) X += dw;
      else if (X > IX) X = X + dw * (X-IX)/(IR-IX);
      int R = *p++;
      if (R >= IR) R += dw;
      else if (R > IX) R = R + dw * (R-IX)/(IR-IX);

      int Y = *p++;
      if (Y >= IB) Y += dh;
      else if (Y > IY) Y = Y + dh*(Y-IY)/(IB-IY);
      int B = *p++;
      if (B >= IB) B += dh;
      else if (B > IY) B = B + dh*(B-IY)/(IB-IY);

      o->resize(X+dx, Y+dy, R-X, B-Y);
      o->layout();
    }
  }
  Fl_Widget::layout();
  set_old_size();
}

////////////////////////////////////////////////////////////////
// Draw

void Fl_Group::draw() {
  Fl_Widget*const* a = array();
  Fl_Widget*const* e = a+children_;
  if (damage() & ~FL_DAMAGE_CHILD) {
    // Full redraw of the group:
    fl_clip(x(), y(), w(), h());
    while (e > a) draw_child(**--e);
    draw_group_box();
    fl_pop_clip(); // this is here for back compatability?
    e = a+children_;
    while (a < e) draw_outside_label(**a++);
    // perhaps fl_pop_clip() should be here?
  } else {
    // only some child widget has been damaged, draw them without
    // doing any clipping.  This is for maximum speed, even though
    // this may result in different output if this widget overlaps
    // another widget or a label.
    while (a < e) {
      Fl_Widget& w = **a++;
      if (w.damage() & FL_DAMAGE_CHILD_LABEL) {
	draw_outside_label(w);
	w.set_damage(w.damage() & ~FL_DAMAGE_CHILD_LABEL);
      }
      update_child(w);
    }
  }
}

// Draw and then clip the rectangle:
void Fl_Group::draw_n_clip()
{
  draw();
  fl_clip_out(x(), y(), w(), h());
}

// Pieces of draw() that subclasses may want to use:

// Draw the box and possibly some of the parent's box so that this
// fills a rectangle:
void Fl_Group::draw_group_box() const {
  if (!(box()->fills_rectangle() ||
	image() && (flags()&FL_ALIGN_TILED) &&
	(!(flags()&15) || (flags() & FL_ALIGN_INSIDE)))) {
    if (parent()) parent()->draw_group_box();
    else {
      fl_color(color());	
      fl_rectf(x(),y(),w(),h());
    }
  }
  draw_box();
  draw_label();
}

// Force a child to redraw and remove the rectangle it used from the clip
// region:
void Fl_Group::draw_child(Fl_Widget& w) const {
  if (!w.visible() || w.is_window()) return;
  if (!fl_not_clipped(w.x(), w.y(), w.w(), w.h())) return;
  w.set_damage(FL_DAMAGE_ALL);
  w.draw_n_clip();
  w.set_damage(0);
}

// Redraw a single child in response to it's damage:
void Fl_Group::update_child(Fl_Widget& w) const {
  if (w.damage() && w.visible() && !w.is_window() &&
      fl_not_clipped(w.x(), w.y(), w.w(), w.h())) {
    w.draw();	
    w.clear_damage();
  }
}

// Parents normally call this to draw outside labels:
void Fl_Group::draw_outside_label(Fl_Widget& w) const {
  if (!w.visible()) return;
  // skip any labels that are inside the widget:
  if (!(w.flags()&15) || (w.flags() & FL_ALIGN_INSIDE)) return;
  // invent a box that is outside the widget:
  unsigned align = w.flags();
  int X = w.x();
  int Y = w.y();
  int W = w.w();
  int H = w.h();
  if (align & FL_ALIGN_TOP) {
    align ^= (FL_ALIGN_BOTTOM|FL_ALIGN_TOP);
    Y = y();
    H = w.y()-Y;
  } else if (align & FL_ALIGN_BOTTOM) {
    align ^= (FL_ALIGN_BOTTOM|FL_ALIGN_TOP);
    Y = Y+H;
    H = y()+h()-Y;
  } else if (align & FL_ALIGN_LEFT) {
    align ^= (FL_ALIGN_LEFT|FL_ALIGN_RIGHT);
    X = x();
    W = w.x()-X-3;
  } else if (align & FL_ALIGN_RIGHT) {
    align ^= (FL_ALIGN_LEFT|FL_ALIGN_RIGHT);
    X = X+W+3;
    W = x()+this->w()-X;
  }
  w.draw_label(X,Y,W,H,(Fl_Flags)align);
}

//
// End of "$Id: Fl_Group.cxx,v 1.67 2000/04/24 08:31:25 bill Exp $".
//
