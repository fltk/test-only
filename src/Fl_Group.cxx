//
// "$Id: Fl_Group.cxx,v 1.101 2001/12/16 22:32:03 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Group.h>
#include <fltk/Fl_Window.h>
#include <fltk/fl_draw.h>
#include <stdlib.h>
#include <fltk/Fl_Tooltip.h>

////////////////////////////////////////////////////////////////

FL_API Fl_Group* Fl_Group::current_;

static void revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX; //FL_NO_BOX;
}

// This style is unnamed since there is no reason for themes to change it:
extern Fl_Named_Style* group_style;

static Fl_Named_Style the_style(0, revert, &group_style);

Fl_Named_Style* group_style = &the_style;

Fl_Group::Fl_Group(int X,int Y,int W,int H,const char *l)
: Fl_Widget(X,Y,W,H,l),
  children_(0),
  focus_(-1),
  array_(0),
  resizable_(0), // fltk 1.0 used (this)
  sizes_(0), // this is allocated when the group is end()ed.
  ox_(X),
  oy_(Y),
  ow_(W),
  oh_(H)
{
  type(FL_GROUP_TYPE);
  style(::group_style);
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
      o->parent(0); // stops it from calling remove()
      delete o;
    }
    free((void*)a);
  }
}

Fl_Group::~Fl_Group() {clear();}

void Fl_Group::insert(Fl_Widget &o, int index) {
  if (o.parent()) {
    int n = o.parent()->find(o);
    if (o.parent() == this) {
      if (index > n) index--;
      if (index == n) return;
    }
    o.parent()->remove(n);
  }
  o.parent(this);
  if (children_ == 0) {
    // allocate for 1 child
    array_ = (Fl_Widget**)malloc(sizeof(Fl_Widget*));
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

void Fl_Group::add(Fl_Widget &o) {
	insert(o, children_);
}

void Fl_Group::remove(int index) {
  if (index >= children_) return;
  Fl_Widget* o = array_[index];
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
  for (;;) {
    if (!o) return children_;
    if (o->parent() == this) break;
    o = o->parent();
  }
  // Search backwards so if children are deleted in backwards order
  // they are found quickly:
  for (int index = children_; index--;)
    if (array_[index] == o) return index;
  return children_;
}

////////////////////////////////////////////////////////////////
// Handle

// send(event,o) is mostly wrapper stuff that should have been done by
// the handle() methods of widgets.  This could probably have been done
// by having handle() check active/visible and having it return a pointer
// to the widget rather than 1 on success.  Oh well...

int Fl_Group::send(int event, Fl_Widget& to) {

  // First see if the widget really should get the event:
  switch (event) {

  case FL_FOCUS:
    // All current group implemetations do this before calling here, but
    // this is reasonable:
    return to.take_focus();

  case FL_ENTER:
  case FL_MOVE:
//  if (&to == Fl::pushed()) return 1; // don't send both move & drag to widget
    // figure out correct type of event:
    event = (to.contains(Fl::belowmouse())) ? FL_MOVE : FL_ENTER;
    // Enter/exit are sent to inactive widgets so that tooltips will work.
  case FL_SHOW:
  case FL_HIDE:
    // These events don't need the widget active.
    if (!to.visible()) return 0;
    break;

  case FL_DND_ENTER:
  case FL_DND_DRAG:
    // figure out correct type of event:
    event = (to.contains(Fl::belowmouse())) ? FL_DND_DRAG : FL_DND_ENTER;

  default:
    if (!to.takesevents()) return 0;
  }

  // Now send the event and return if the widget does not use it.
  // Adjust event to be relative to the widget:
  int save_x = Fl::e_x; Fl::e_x -= to.x();
  int save_y = Fl::e_y; Fl::e_y -= to.y();
  int ret = to.handle(event);
  Fl::e_y = save_y;
  Fl::e_x = save_x;
  if (!ret) return 0;

  switch (event) {

  case FL_ENTER:
  case FL_DND_ENTER:
    // Successful completion of FL_ENTER means the widget is now the
    // belowmouse widget, but only call Fl::belowmouse if the child
    // widget did not do so:
    if (!to.contains(Fl::belowmouse())) Fl::belowmouse(to);
    break;

  case FL_PUSH:
    // Successful completion of FL_PUSH means the widget is now the
    // pushed widget, but only set Fl::pushed() if the child
    // widget did not do so and the mouse is still down:
    if (Fl::event_state(0x0f000000) && !to.contains(Fl::pushed()))
      Fl::pushed(to);
    break;

  case FL_SHOW:
  case FL_HIDE:
    // always return zero, as this should be sent to every child widget
    return 0;
  }

  return 1;
}

// Turn FL_Tab into FL_Right or FL_Left for keyboard navigation
int Fl_Group::navigation_key() {
  if (Fl::event_key() == FL_Tab && !Fl::event_state(FL_CTRL))
    return Fl::event_state(FL_SHIFT) ? FL_Left : FL_Right;
  return Fl::event_key();
}

int Fl_Group::handle(int event) {
  const int numchildren = children();
  int i;

  switch (event) {

  case FL_FOCUS:
    if (contains(Fl::focus())) {
      // The focus is being changed to some widget inside this.
      focus_ = find(Fl::focus());
      return 1;
    }
    // otherwise it indicates an attempt to give this widget focus:
    switch (navigation_key()) {
    default: {
      // try to give it to whatever child had focus last:
      if (focus_ >= 0 && focus_ < numchildren)
	if (child(focus_)->take_focus()) return 1;
      // otherwise search for the widget that needs the focus, but
      // prefer a widget that returns 2:
      Fl_Widget* f1 = 0; int ret = 0;
      for (i = 0; i < numchildren; ++i) {
	Fl_Widget* w = child(i);
	if (w->takesevents()) {
	  int n = w->handle(FL_FOCUS);
	  if (n == 2) {f1 = w; ret = 2; break;}
	  else if (n) {f1 = w; ret = 1;}
	}
      }
      if (f1 && !f1->contains(Fl::focus())) Fl::focus(f1);
      return ret;}
    case FL_Right:
    case FL_Down:
      for (i=0; i < numchildren; ++i)
	if (child(i)->take_focus()) return 1;
      break;
    case FL_Left:
    case FL_Up:
      for (i = numchildren; i--;)
	if (child(i)->take_focus()) return 1;
      break;
    }
    return 0;

  case FL_PUSH:
  case FL_ENTER:
  case FL_MOVE:
  case FL_DND_ENTER:
  case FL_DND_DRAG:
    for (i = numchildren; i--;) {
      Fl_Widget& o = *child(i);
      int mx = Fl::event_x() - o.x();
      int my = Fl::event_y() - o.y();
      if (mx >= 0 && mx < o.w() && my >= 0 && my < o.h())
	if (send(event, *child(i))) return 1;
    }
    if (tooltip() && (event==FL_ENTER || event==FL_MOVE)) {
      Fl::belowmouse(this);
      return 1;
    }
    return 0;

  case FL_LEAVE:
    return 0;
  }

  // For all other events, try to give to each child, starting at focus:
  i = focus_; if (i < 0 || i >= numchildren) i = 0;
  if (numchildren) {
    for (int j = i;;) {
      if (send(event, *child(j))) return 1;
      j++;
      if (j >= numchildren) j = 0;
      if (j == i) break;
    }
  }

  if (event == FL_SHORTCUT && !focused() && contains(Fl::focus())) {
    // Try to do keyboard navigation for unused shortcut keys:
    int key = navigation_key();

    // loop from the current focus looking for a new focus, quit when
    // we reach the original again:
    int previous = i = focus_;
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
      if (child(i)->take_focus()) return 1;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////
// Layout

// The effect of "init_sizes()" is that layout() is prevented from
// changing the current positions of anything and thus the resizing
// starts up from the state it is in when layout() is called (which
// is hopefully the same state that it was in when init_sizes() was
// called). This is useful when
// a program assembles a resizable group and wants to fix up it's
// size to surround the children or fit into a surrounding group,
// without the automatic resize messing things up. This is much 
// easier than the old fltk kludge of removing the resizable(),
// fixing things, and restoring it.
//
// The implementation is such that init_sizes() is inexpensive and
// can thus be called many times during the construction of a group.
//
// The sizes() array stores the initial positions of widgets as
// left,right,top,bottom quads.  The first quad is the group, the
// second is the resizable (clipped to the group), and the
// rest are the children.  This is a convienent order for the
// algorithim.  If you change this be sure to fix Fl_Tile which
// also uses this array!
//
// Calling init_sizes() "resets" the sizes array to the current group
// and children positions.  Actually it just deletes the sizes array,
// and it is not recreated until the next time layout is called. This
// allows arbitrary layout changes to be made by the program until the
// moment that the group is displayed to the user. This appears to be
// the desired behavior.

void Fl_Group::init_sizes() {
  delete[] sizes_; sizes_ = 0;
  set_old_size();
  relayout();
}

int* Fl_Group::sizes() {
  if (!sizes_) {
    int* p = sizes_ = new int[4*(children_+2)];
    // first thing in sizes array is the group's size:
    p[0] = x();
    p[1] = w();
    p[2] = y();
    p[3] = h();
    // next is the resizable's size:
    p[4] = 0; // init to the group's size
    p[5] = p[1];
    p[6] = 0;
    p[7] = p[3];
    Fl_Widget* r = resizable();
    if (r && r != this) { // then clip the resizable to it
      int t;
      t = r->x(); if (t > 0) p[4] = t;
      t +=r->w(); if (t < p[1]) p[5] = t;
      t = r->y(); if (t > 0) p[6] = t;
      t +=r->h(); if (t < p[3]) p[7] = t;
    }
    // next is all the children's sizes:
    p += 8;
    Fl_Widget*const* a = array_;
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
  if (resizable() && children_ && (!sizes_ || layout_damage()&FL_LAYOUT_WH)) {

    // get changes in size from the initial size:
    // If this is the first call assumme this is the initial size.
    int* p = sizes();
    int dw = w()-p[1];
    int dh = h()-p[3];

    p+=4;

    // Calculate a new size & position for every child widget:
    // get initial size of resizable():
    int IX = *p++;
    int IR = *p++;
    int IY = *p++;
    int IB = *p++;

    Fl_Widget*const* a = array_;
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

      o->resize(X, Y, R-X, B-Y);
      o->layout();
    }

  } else {

    // propagate layout() calls to all the children. The main purpose of
    // this is so that moving a widget will move any Fl_Windows that
    // are inside of it. This is not necessary for Fl_Windows themselves
    // on X and Win32 as the system has already done the work but we
    // still need to call inner things that need layout:
    Fl_Widget*const* a = array_;
    Fl_Widget*const* e = a+children_;
    if ((layout_damage() & FL_LAYOUT_XY) && !is_window()) {
    while (a < e) {
      Fl_Widget* widget = *a++;
	widget->layout_damage(widget->layout_damage()|FL_LAYOUT_XY);
	widget->layout();
    }
    } else {
      while (a < e) {
	Fl_Widget* widget = *a++;
	if (widget->layout_damage()) widget->layout();
  }
    }
  }
  if (layout_damage() & FL_LAYOUT_WH) redraw();
  Fl_Widget::layout();
  set_old_size();
}

////////////////////////////////////////////////////////////////
// Draw

void Fl_Group::draw() {
  int numchildren = children();
  if (damage() & ~FL_DAMAGE_CHILD) {
#if 0
    // blinky-draw:
    draw_box();
    draw_inside_label();
    int n; for (n = 0; n < numchildren; n++) {
      Fl_Widget& w = *child(n);
      w.set_damage(FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE);
      update_child(w);
    }
#else
    // Non-blinky draw, draw the inside widgets first, clip their areas
    // out, and then draw the background:
    fl_push_clip(0, 0, w(), h());
    int n; for (n = numchildren; n--;) draw_child(*child(n));
    if (!box()->fills_rectangle()) {
      if (parent()) parent()->draw_group_box();
    }
    draw_box();
    draw_inside_label();
    fl_pop_clip();
#endif
    // labels are drawn without the clip for back compatability so they
    // can draw atop sibling widgets:
    for (n = 0; n < numchildren; n++) draw_outside_label(*child(n));
  } else {
    // only some child widget has been damaged, draw them without
    // doing any clipping.  This is for maximum speed, even though
    // this may result in different output if this widget overlaps
    // another widget or a label.
    for (int n = 0; n < numchildren; n++) {
      Fl_Widget& w = *child(n);
      if (w.damage() & FL_DAMAGE_CHILD_LABEL) {
	draw_outside_label(w);
	w.set_damage(w.damage() & ~FL_DAMAGE_CHILD_LABEL);
      }
      update_child(w);
    }
  }
}

// Pieces of draw() that subclasses may want to use:

// Draw the background. If FL_DAMAGE_EXPOSE is on, widgets are expected
// to completely fill their rectangle. To allow non-rectangular widgets
// to appear to work, a widget can call this (with the clip region set)
// to draw the area of it's parent that is visible behind it.
void Fl_Group::draw_group_box() const {
// So that this may be called from any child's draw context, I need to
// figure out the correct origin:
  int save_x = fl_x_;
  int save_y = fl_y_;
#if 1
  fl_x_ = 0;
  fl_y_ = 0;
  const Fl_Group* group = this;
  while (!group->is_window()) {
    fl_x_ += group->x();
    fl_y_ += group->y();
    group = group->parent();
  }
#else
  // this does not work because it resets the clip region:
  make_current();
#endif
  if (!box()->fills_rectangle()) {
    if (parent()) {
      parent()->draw_group_box();
// WAS: leave this out, so that FL_NO_BOX can be used to stop blinking:
//      } else {
//        fl_color(color());	
//        fl_rectf(0, 0, w(), h());
    }
  }
  draw_box();
  draw_inside_label();
  fl_y_ = save_y;
  fl_x_ = save_x;
}

// Widgets that want to outwit the clip-out can set this when they are
// drawn to indicate that they did the clip-out. Only Fl_Tabs really uses
// this (and I'm not certain it has to), plus a bunch of back-compatability
// widgets that want to be "invisible" (they turn this on but don't draw
// anything). This is a pointer so if it is left on by a child widget
// it does not fool this into thinking the clipping is done.
Fl_Widget* fl_did_clipping;

// Force a child to redraw and remove the rectangle it used from the clip
// region.
void Fl_Group::draw_child(Fl_Widget& w) const {
  if (w.visible() && !w.is_window()) {
  if (!fl_not_clipped(w.x(), w.y(), w.w(), w.h())) return;
  int save_x = fl_x_; fl_x_ += w.x();
  int save_y = fl_y_; fl_y_ += w.y();
    fl_did_clipping = 0;
    w.set_damage(FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE);
    w.draw();
    w.set_damage(0);
    if (fl_did_clipping != &w) fl_clip_out(0,0,w.w(),w.h());
  fl_y_ = save_y;
  fl_x_ = save_x;
  }
}

// Redraw a single child in response to it's damage:
void Fl_Group::update_child(Fl_Widget& w) const {
  if (w.damage() && w.visible() && !w.is_window()) {
    if (!fl_not_clipped(w.x(), w.y(), w.w(), w.h())) return;
    int save_x = fl_x_; fl_x_ += w.x();
    int save_y = fl_y_; fl_y_ += w.y();
    w.draw();	
    w.set_damage(0);
    fl_y_ = save_y;
    fl_x_ = save_x;
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
    Y = 0;
    H = w.y();
  } else if (align & FL_ALIGN_BOTTOM) {
    align ^= (FL_ALIGN_BOTTOM|FL_ALIGN_TOP);
    Y = Y+H;
    H = h()-Y;
  } else if (align & FL_ALIGN_LEFT) {
    align ^= (FL_ALIGN_LEFT|FL_ALIGN_RIGHT);
    X = 0;
    W = w.x()-3;
  } else if (align & FL_ALIGN_RIGHT) {
    align ^= (FL_ALIGN_LEFT|FL_ALIGN_RIGHT);
    X = X+W+3;
    W = this->w()-X;
  }
  w.draw_label(X,Y,W,H,(Fl_Flags)align);
}

void Fl_Group::fix_old_positions() {
  if (is_window()) return; // in fltk 1.0 children of windows were relative
  for (int i = 0; i < children(); i++) {
    Fl_Widget& w = *(child(i));
    w.x(w.x()-x());
    w.y(w.y()-y());
  }
}

//
// End of "$Id: Fl_Group.cxx,v 1.101 2001/12/16 22:32:03 spitzak Exp $".
//
