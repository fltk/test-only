//
// "$Id$"
//
// Group widget for the Fast Light Tool Kit (FLTK).
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

// The fltk3::Group is the only defined container type in FLTK.

// fltk3::Window itself is a subclass of this, and most of the event
// handling is designed so windows themselves work correctly.

#include <stdio.h>
#include <fltk3/run.h>
#include <fltk3/Group.h>
#include <fltk3/Window.h>
#include <fltk3/draw.h>
#include <stdlib.h>

fltk3::Group* fltk3::Group::current_;

// Hack: A single child is stored in the pointer to the array, while
// multiple children are stored in an allocated array:

/**
  Returns a pointer to the array of children. <I>This pointer is only
  valid until the next time a child is added or removed.</I>
*/
fltk3::Widget*const* fltk3::Group::array() const {
  return children_ <= 1 ? (fltk3::Widget**)(&array_) : array_;
}

/**
  Searches the child array for the widget and returns the index. Returns children()
  if the widget is NULL or not found.
*/
int fltk3::Group::find(const fltk3::Widget* o) const {
  fltk3::Widget*const* a = array();
  int i; for (i=0; i < children_; i++) if (*a++ == o) break;
  return i;
}

// Metrowerks CodeWarrior and others can't export the static
// class member: current_, so these methods can't be inlined...

/**
  Sets the current group so you can build the widget
  tree by just constructing the widgets.

  begin() is automatically called by the constructor for fltk3::Group (and thus for
  fltk3::Window as well). begin() <I>is exactly the same as</I> current(this).
  <I>Don't forget to end() the group or window!</I>
*/
void fltk3::Group::begin() {current_ = this;}

/**
  <I>Exactly the same as</I> current(this->parent()). Any new widgets
  added to the widget tree will be added to the parent of the group.
*/
void fltk3::Group::end() {current_ = parent();}

/**
  Returns the currently active group.
  
  The fltk3::Widget constructor automatically does current()->add(widget) if this
  is not null. To prevent new widgets from being added to a group, call
  fltk3::Group::current(0).
*/
fltk3::Group *fltk3::Group::current() {return current_;}

/**
  Sets the current group.
  \see fltk3::Group::current() 
*/
void fltk3::Group::current(fltk3::Group *g) {current_ = g;}

extern fltk3::Widget* fl_oldfocus; // set by fltk3::focus

// For back-compatibility, we must adjust all events sent to child
// windows so they are relative to that window.

// translate the current keystroke into up/down/left/right for navigation:
#define ctrl(x) (x^0x40)
static int navkey() {
  switch (fltk3::event_key()) {
  case 0: // not an fltk3::KEYBOARD/fltk3::SHORTCUT event
    break;
  case fltk3::TabKey:
    if (!fltk3::event_state(fltk3::SHIFT)) return fltk3::RightKey;
    return fltk3::LeftKey;
  case fltk3::RightKey:
    return fltk3::RightKey;
  case fltk3::LeftKey:
    return fltk3::LeftKey;
  case fltk3::UpKey:
    return fltk3::UpKey;
  case fltk3::DownKey:
    return fltk3::DownKey;
  }
  return 0;
}

int fltk3::Group::handle(int event) {
  fltk3::Widget*const* a = array();
  int i;
  fltk3::Widget* o;

  switch (event) {

  case fltk3::FOCUS:
    switch (navkey()) {
    default:
      if (savedfocus_ && savedfocus_->take_focus()) return 1;
    case fltk3::RightKey:
    case fltk3::DownKey:
      for (i = children(); i--;) if ((*a++)->take_focus()) return 1;
      break;
    case fltk3::LeftKey:
    case fltk3::UpKey:
      for (i = children(); i--;) if (a[i]->take_focus()) return 1;
      break;
    }
    return 0;

  case fltk3::UNFOCUS:
    savedfocus_ = fl_oldfocus;
    return 0;

  case fltk3::KEYBOARD:
    return navigation(navkey());

  case fltk3::SHORTCUT:
    for (i = children(); i--;) {
      o = a[i];
      if (o->takesevents() && fltk3::event_inside(o) && o->send(fltk3::SHORTCUT))
	return 1;
    }
    for (i = children(); i--;) {
      o = a[i];
      if (o->takesevents() && !fltk3::event_inside(o) && o->send(fltk3::SHORTCUT))
	return 1;
    }
    if ((fltk3::event_key() == fltk3::EnterKey || fltk3::event_key() == fltk3::KPEnterKey)) return navigation(fltk3::DownKey);
    return 0;

  case fltk3::ENTER:
  case fltk3::MOVE:
    for (i = children(); i--;) {
      o = a[i];
      if (o->visible() && fltk3::event_inside(o)) {
	if (o->contains(fltk3::belowmouse())) {
	  return o->send(fltk3::MOVE);
	} else {
	  fltk3::belowmouse(o);
	  if (o->send(fltk3::ENTER)) return 1;
	}
      }
    }
    fltk3::belowmouse(this);
    return 1;

  case fltk3::DND_ENTER:
  case fltk3::DND_DRAG:
    for (i = children(); i--;) {
      o = a[i];
      if (o->takesevents() && fltk3::event_inside(o)) {
	if (o->contains(fltk3::belowmouse())) {
	  return o->send(fltk3::DND_DRAG);
	} else if (o->send(fltk3::DND_ENTER)) {
	  if (!o->contains(fltk3::belowmouse())) fltk3::belowmouse(o);
	  return 1;
	}
      }
    }
    fltk3::belowmouse(this);
    return 0;

  case fltk3::PUSH:
    for (i = children(); i--;) {
      o = a[i];
      if (o->takesevents() && fltk3::event_inside(o)) {
	fltk3::WidgetTracker wp(o);
	if (o->send(fltk3::PUSH)) {
	  if (fltk3::pushed() && wp.exists() && !o->contains(fltk3::pushed())) fltk3::pushed(o);
	  return 1;
	}
      }
    }
    return 0;

  case fltk3::RELEASE:
  case fltk3::DRAG:
    o = fltk3::pushed();
    if (o == this) return 0;
    else if (o) o->send(event);
    else {
      for (i = children(); i--;) {
	o = a[i];
	if (o->takesevents() && fltk3::event_inside(o)) {
	  if (o->send(event)) return 1;
	}
      }
    }
    return 0;

  case fltk3::MOUSEWHEEL:
    for (i = children(); i--;) {
      o = a[i];
      if (o->takesevents() && fltk3::event_inside(o) && o->send(fltk3::MOUSEWHEEL))
	return 1;
    }
    for (i = children(); i--;) {
      o = a[i];
      if (o->takesevents() && !fltk3::event_inside(o) && o->send(fltk3::MOUSEWHEEL))
	return 1;
    }
    return 0;

  case fltk3::DEACTIVATE:
  case fltk3::ACTIVATE:
    for (i = children(); i--;) {
      o = *a++;
      if (o->active()) o->handle(event);
    }
    return 1;

  case fltk3::SHOW:
  case fltk3::HIDE:
    for (i = children(); i--;) {
      o = *a++;
      if (event == fltk3::HIDE && o == fltk3::focus()) {
        // Give up input focus...
	int old_event = fltk3::e_number;
        o->handle(fltk3::e_number = fltk3::UNFOCUS);
	fltk3::e_number = old_event;
	fltk3::focus(0);
      }
      if (o->visible()) o->handle(event);
    }
    return 1;

  default:
    // For all other events, try to give to each child, starting at focus:
    for (i = 0; i < children(); i ++)
      if (fltk3::focus_ == a[i]) break;

    if (i >= children()) i = 0;

    if (children()) {
      for (int j = i;;) {
        if (a[j]->takesevents()) if (a[j]->send(event)) return 1;
        j++;
        if (j >= children()) j = 0;
        if (j == i) break;
      }
    }

    return 0;
  }
}

//void fltk3::Group::focus(fltk3::Widget *o) {fltk3::focus(o); o->handle(fltk3::FOCUS);}

#if 0
const char *nameof(fltk3::Widget *o) {
  if (!o) return "NULL";
  if (!o->label()) return "<no label>";
  return o->label();
}
#endif

// try to move the focus in response to a keystroke:
int fltk3::Group::navigation(int key) {
  if (children() <= 1) return 0;
  int i;
  for (i = 0; ; i++) {
    if (i >= children_) return 0;
    if (array_[i]->contains(fltk3::focus())) break;
  }
  fltk3::Widget *previous = array_[i];

  for (;;) {
    switch (key) {
    case fltk3::RightKey:
    case fltk3::DownKey:
      i++;
      if (i >= children_) {
	if (parent()) return 0;
	i = 0;
      }
      break;
    case fltk3::LeftKey:
    case fltk3::UpKey:
      if (i) i--;
      else {
	if (parent()) return 0;
	i = children_-1;
      }
      break;
    default:
      return 0;
    }
    fltk3::Widget* o = array_[i];
    if (o == previous) return 0;
    switch (key) {
    case fltk3::DownKey:
    case fltk3::UpKey:
      // for up/down, the widgets have to overlap horizontally:
      if (o->x() >= previous->x()+previous->w() ||
	  o->x()+o->w() <= previous->x()) continue;
    }
    if (o->take_focus()) return 1;
  }
}

////////////////////////////////////////////////////////////////

fltk3::Group::Group(int X,int Y,int W,int H,const char *l)
: fltk3::Widget(X,Y,W,H,l),
  array_(0L),
  savedfocus_(0L),
  resizable_(0L),
  children_(0),
  sizes_(0L)
{
  align(fltk3::ALIGN_TOP);
  resizable_ = this;
  //set_flag(GROUP_RELATIVE); // not yet implemented - don't set
  // Subclasses may want to construct child objects as part of their
  // constructor, so make sure they are add()'d to this object.
  // But you must end() the object!
  begin();
}

/**
  Deletes all child widgets from memory recursively.

  This method differs from the remove() method in that it
  affects all child widgets and deletes them from memory.
*/
void fltk3::Group::clear() {
  savedfocus_ = 0;
  resizable_ = this;
  init_sizes();

  // we must change the fltk3::pushed() widget, if it is one of
  // the group's children. Otherwise fl_fix_focus() would send
  // lots of events to children that are about to be deleted
  // anyway.

  fltk3::Widget *pushed = fltk3::pushed();	// save pushed() widget
  if (contains(pushed)) pushed = this;	// set it to be the group, if it's a child
  fltk3::pushed(this);			// for fl_fix_focus etc.

  // okay, now it is safe to destroy the children:

#define REVERSE_CHILDREN
#ifdef  REVERSE_CHILDREN
  // Reverse the order of the children. Doing this and deleting
  // always the last child is much faster than the other way around.
  if (children_ > 1) {
    fltk3::Widget *temp;
    fltk3::Widget **a = (fltk3::Widget**)array();
    for (int i=0,j=children_-1; i<children_/2; i++,j--) {
      temp = a[i];
      a[i] = a[j];
      a[j] = temp;
    }
  }
#endif // REVERSE_CHILDREN

  while (children_) {			// delete all children
    int idx = children_-1;		// last child's index
    fltk3::Widget* w = child(idx);		// last child widget
    if (w->parent()==this) {		// should always be true
      if (children_>2) {		// optimized removal
        w->parent_ = 0;			// reset child's parent
        children_--;			// update counter
      } else {				// slow removal
        remove(idx);
      }
      delete w;				// delete the child
    } else {				// should never happen
      remove(idx);			// remove it anyway
    }
  }

  if (pushed != this) fltk3::pushed(pushed); // reset pushed() widget

}

/**
  The destructor <I>also deletes all the children</I>. This allows a
  whole tree to be deleted at once, without having to keep a pointer to
  all the children in the user code.
  
  It is allowed that the fltk3::Group and all of its children are automatic
  (local) variables, but you must declare the fltk3::Group \e first, so that
  it is destroyed last.
  
  If you add static or automatic (local) variables to an fltk3::Group, then it
  is your responsibility to remove (or delete) all such static or automatic
  child widgets \e \b before destroying the group - otherwise the child
  widgets' destructors would be called twice!
*/
fltk3::Group::~Group() {
  clear();
}

/**
  The widget is removed from its current group (if any) and then
  inserted into this group. It is put at index n - or at the end,
  if n >= children(). This can also be used to rearrange
  the widgets inside a group.
*/
void fltk3::Group::insert(fltk3::Widget &o, int index) {
  if (o.parent()) {
    fltk3::Group* g = o.parent();
    int n = g->find(o);
    if (g == this) {
      if (index > n) index--;
      if (index == n) return;
    }
    g->remove(n);
  }
  o.parent_ = this;
  if (children_ == 0) { // use array pointer to point at single child
    array_ = (fltk3::Widget**)&o;
  } else if (children_ == 1) { // go from 1 to 2 children
    fltk3::Widget* t = (fltk3::Widget*)array_;
    array_ = (fltk3::Widget**)malloc(2*sizeof(fltk3::Widget*));
    if (index) {array_[0] = t; array_[1] = &o;}
    else {array_[0] = &o; array_[1] = t;}
  } else {
    if (!(children_ & (children_-1))) // double number of children
      array_ = (fltk3::Widget**)realloc((void*)array_,
				    2*children_*sizeof(fltk3::Widget*));
    int j; for (j = children_; j > index; j--) array_[j] = array_[j-1];
    array_[j] = &o;
  }
  children_++;
  init_sizes();
}

/**
  The widget is removed from its current group (if any) and then added
  to the end of this group.
*/
void fltk3::Group::add(fltk3::Widget &o) {insert(o, children_);}

/**
  Removes the widget at \p index from the group but does not delete it.

  This method does nothing if \p index is out of bounds.

  This method differs from the clear() method in that it only affects
  a single widget and does not delete it from memory.
  
  \since FLTK 1.3.0
*/
void fltk3::Group::remove(int index) {
  if (index < 0 || index >= children_) return;
  fltk3::Widget &o = *child(index);
  if (&o == savedfocus_) savedfocus_ = 0;
  if (o.parent_ == this) {	// this should always be true
    o.parent_ = 0;
  } 

  // remove the widget from the group

  children_--;
  if (children_ == 1) { // go from 2 to 1 child
    fltk3::Widget *t = array_[!index];
    free((void*)array_);
    array_ = (fltk3::Widget**)t;
  } else if (children_ > 1) { // delete from array
    for (; index < children_; index++) array_[index] = array_[index+1];
  }
  init_sizes();
}

/**
  Removes a widget from the group but does not delete it.

  This method does nothing if the widget is not a child of the group.

  This method differs from the clear() method in that it only affects
  a single widget and does not delete it from memory.
  
  \note If you have the child's index anyway, use remove(int index)
  instead, because this doesn't need a child lookup in the group's
  table of children. This can be much faster, if there are lots of
  children.
*/
void fltk3::Group::remove(fltk3::Widget &o) {
  if (!children_) return;
  int i = find(o);
  if (i < children_) remove(i);
}

////////////////////////////////////////////////////////////////

// Rather lame kludge here, I need to detect windows and ignore the
// changes to X,Y, since all children are relative to X,Y.  That
// is why I check type():

// sizes array stores the initial positions of widgets as
// left,right,top,bottom quads.  The first quad is the group, the
// second is the resizable (clipped to the group), and the
// rest are the children.  This is a convenient order for the
// algorithm.  If you change this be sure to fix fltk3::TiledGroup which
// also uses this array!

/**
  Resets the internal array of widget sizes and positions.

  The fltk3::Group widget keeps track of the original widget sizes and
  positions when resizing occurs so that if you resize a window back to its
  original size the widgets will be in the correct places. If you rearrange
  the widgets in your group, call this method to register the new arrangement
  with the fltk3::Group that contains them.
  
  If you add or remove widgets, this will be done automatically.
  
  \note The internal array of widget sizes and positions will be allocated and
  filled when the next resize() occurs.
  
  \sa sizes()
*/
void fltk3::Group::init_sizes() {
  delete[] sizes_; sizes_ = 0;
}

/**
  Returns the internal array of widget sizes and positions.

  If the sizes() array does not exist, it will be allocated and filled
  with the current widget sizes and positions.

  \note You should never need to use this method directly, unless you have
  special needs to rearrange the children of a fltk3::Group. fltk3::TiledGroup uses
  this to rearrange its widget positions.
  
  \sa init_sizes()

  \todo Should the internal representation of the sizes() array be documented?
*/
int* fltk3::Group::sizes() {
  if (!sizes_) {
    int* p = sizes_ = new int[4*(children_+2)];
    // first thing in sizes array is the group's size:
    if (type() < fltk3::WINDOW) {p[0] = x(); p[2] = y();} else {p[0] = p[2] = 0;}
    p[1] = p[0]+w(); p[3] = p[2]+h();
    // next is the resizable's size:
    p[4] = p[0]; // init to the group's size
    p[5] = p[1];
    p[6] = p[2];
    p[7] = p[3];
    fltk3::Widget* r = resizable();
    if (r && r != this) { // then clip the resizable to it
      int t;
      t = r->x(); if (t > p[0]) p[4] = t;
      t +=r->w(); if (t < p[1]) p[5] = t;
      t = r->y(); if (t > p[2]) p[6] = t;
      t +=r->h(); if (t < p[3]) p[7] = t;
    }
    // next is all the children's sizes:
    p += 8;
    fltk3::Widget*const* a = array();
    for (int i=children_; i--;) {
      fltk3::Widget* o = *a++;
      *p++ = o->x();
      *p++ = o->x()+o->w();
      *p++ = o->y();
      *p++ = o->y()+o->h();
    }
  }
  return sizes_;
}

/**
  Resizes the fltk3::Group widget and all of its children.

  The fltk3::Group widget first resizes itself, and then it moves and resizes
  all its children according to the rules documented for
  fltk3::Group::resizable(fltk3::Widget*)

  \sa fltk3::Group::resizable(fltk3::Widget*)
  \sa fltk3::Group::resizable()
  \sa fltk3::Widget::resize(int,int,int,int)
*/
void fltk3::Group::resize(int X, int Y, int W, int H) {

  int dx = X-x();
  int dy = Y-y();
  int dw = W-w();
  int dh = H-h();
  
  int *p = sizes(); // save initial sizes and positions

  Widget::resize(X,Y,W,H); // make new xywh values visible for children

  if (!resizable() || (dw==0 && dh==0) ) {

    if (type() < fltk3::WINDOW) {
      fltk3::Widget*const* a = array();
      for (int i=children_; i--;) {
	fltk3::Widget* o = *a++;
	o->resize(o->x()+dx, o->y()+dy, o->w(), o->h());
      }
    }

  } else if (children_) {

    // get changes in size/position from the initial size:
    dx = X - p[0];
    dw = W - (p[1]-p[0]);
    dy = Y - p[2];
    dh = H - (p[3]-p[2]);
    if (type() >= fltk3::WINDOW) dx = dy = 0;
    p += 4;

    // get initial size of resizable():
    int IX = *p++;
    int IR = *p++;
    int IY = *p++;
    int IB = *p++;

    fltk3::Widget*const* a = array();
    for (int i=children_; i--;) {
      fltk3::Widget* o = *a++;
#if 1
      int XX = *p++;
      if (XX >= IR) XX += dw;
      else if (XX > IX) XX = IX+((XX-IX)*(IR+dw-IX)+(IR-IX)/2)/(IR-IX);
      int R = *p++;
      if (R >= IR) R += dw;
      else if (R > IX) R = IX+((R-IX)*(IR+dw-IX)+(IR-IX)/2)/(IR-IX);

      int YY = *p++;
      if (YY >= IB) YY += dh;
      else if (YY > IY) YY = IY+((YY-IY)*(IB+dh-IY)+(IB-IY)/2)/(IB-IY);
      int B = *p++;
      if (B >= IB) B += dh;
      else if (B > IY) B = IY+((B-IY)*(IB+dh-IY)+(IB-IY)/2)/(IB-IY);
#else // much simpler code from Francois Ostiguy:
      int XX = *p++;
      if (XX >= IR) XX += dw;
      else if (XX > IX) XX += dw * (XX-IX)/(IR-IX);
      int R = *p++;
      if (R >= IR) R += dw;
      else if (R > IX) R = R + dw * (R-IX)/(IR-IX);

      int YY = *p++;
      if (YY >= IB) YY += dh;
      else if (YY > IY) YY = YY + dh*(YY-IY)/(IB-IY);
      int B = *p++;
      if (B >= IB) B += dh;
      else if (B > IY) B = B + dh*(B-IY)/(IB-IY);
#endif
      o->resize(XX+dx, YY+dy, R-XX, B-YY);
    }
  }
}

/**
  Draws all children of the group.

  This is useful, if you derived a widget from fltk3::Group and want to draw a special
  border or background. You can call draw_children() from the derived draw() method
  after drawing the box, border, or background.
*/
void fltk3::Group::draw_children() {
  fltk3::Widget*const* a = array();

  if (clip_children()) {
    fltk3::push_clip(fltk3::box_dx(box()),
                     fltk3::box_dy(box()),
                     w() - fltk3::box_dw(box()),
                     h() - fltk3::box_dh(box()));
  }

  if (damage() & ~fltk3::DAMAGE_CHILD) { // redraw the entire thing:
    for (int i=children_; i--;) {
      fltk3::Widget& o = **a++;
      draw_child(o);
      draw_outside_label(o);
    }
  } else {	// only redraw the children that need it:
    for (int i=children_; i--;) {
      update_child(**a++);
    }
  }

  if (clip_children()) fltk3::pop_clip();
}

void fltk3::Group::draw() {
  if (damage() & ~fltk3::DAMAGE_CHILD) { // redraw the entire thing:
    draw_box();
    draw_label();
  }
  draw_children();
}

/**
  Draws a child only if it needs it.

  This draws a child widget, if it is not clipped \em and if any damage() bits
  are set. The damage bits are cleared after drawing.

  \sa fltk3::Group::draw_child(fltk3::Widget& widget) const
*/
void fltk3::Group::update_child(fltk3::Widget& widget) const {
  if (widget.damage() && widget.visible() && widget.type() < fltk3::WINDOW &&
      fltk3::not_clipped(widget.x(), widget.y(), widget.w(), widget.h())) {
    push_origin();
    translate_origin(widget.x(), widget.y());
    widget.draw();	
    pop_origin();
    widget.clear_damage();
  }
}

/**
  Forces a child to redraw.

  This draws a child widget, if it is not clipped.
  The damage bits are cleared after drawing.
*/
void fltk3::Group::draw_child(fltk3::Widget& widget) const {
  if (widget.visible() && widget.type() < fltk3::WINDOW &&
      fltk3::not_clipped(widget.x(), widget.y(), widget.w(), widget.h())) {
    widget.set_damage(fltk3::DAMAGE_ALL);
    push_origin();
    translate_origin(widget.x(), widget.y());
    widget.draw();
    pop_origin();
    widget.clear_damage();
  }
}

extern char fltk3::draw_shortcut;

/** Parents normally call this to draw outside labels of child widgets. */
void fltk3::Group::draw_outside_label(const fltk3::Widget& widget) const {
  if (!widget.visible()) return;
  // skip any labels that are inside the widget:
  if (!(widget.align()&15) || (widget.align() & fltk3::ALIGN_INSIDE)) return;
  // invent a box that is outside the widget:
  fltk3::Align a = widget.align();
  int X = widget.x();
  int Y = widget.y();
  int W = widget.w();
  int H = widget.h();
  int wx, wy;
  if (const_cast<fltk3::Group*>(this)->as_window()) {
    wx = wy = 0;
  } else {
    wx = x(); wy = y();
  }
  if ( (a & 0x0f) == fltk3::ALIGN_LEFT_TOP ) {
    a = (a &~0x0f ) | fltk3::ALIGN_TOP_RIGHT;
    X = wx;
    W = widget.x()-X-3;
  } else if ( (a & 0x0f) == fltk3::ALIGN_LEFT_BOTTOM ) {
    a = (a &~0x0f ) | fltk3::ALIGN_BOTTOM_RIGHT; 
    X = wx;
    W = widget.x()-X-3;
  } else if ( (a & 0x0f) == fltk3::ALIGN_RIGHT_TOP ) {
    a = (a &~0x0f ) | fltk3::ALIGN_TOP_LEFT; 
    X = X+W+3;
    W = wx+this->w()-X;
  } else if ( (a & 0x0f) == fltk3::ALIGN_RIGHT_BOTTOM ) {
    a = (a &~0x0f ) | fltk3::ALIGN_BOTTOM_LEFT; 
    X = X+W+3;
    W = wx+this->w()-X;
  } else if (a & fltk3::ALIGN_TOP) {
    a ^= (fltk3::ALIGN_BOTTOM|fltk3::ALIGN_TOP);
    Y = wy;
    H = widget.y()-Y;
  } else if (a & fltk3::ALIGN_BOTTOM) {
    a ^= (fltk3::ALIGN_BOTTOM|fltk3::ALIGN_TOP);
    Y = Y+H;
    H = wy+h()-Y;
  } else if (a & fltk3::ALIGN_LEFT) {
    a ^= (fltk3::ALIGN_LEFT|fltk3::ALIGN_RIGHT);
    X = wx;
    W = widget.x()-X-3;
  } else if (a & fltk3::ALIGN_RIGHT) {
    a ^= (fltk3::ALIGN_LEFT|fltk3::ALIGN_RIGHT);
    X = X+W+3;
    W = wx+this->w()-X;
  }
  widget.draw_label(X,Y,W,H,(fltk3::Align)a);
}

void fltk3::Group::forms_end() {
  // set the dimensions of a group to surround contents
  if (children() && !w()) {
    fltk3::Widget*const* a = array();
    fltk3::Widget* o = *a++;
    int rx = o->x();
    int ry = o->y();
    int rw = rx+o->w();
    int rh = ry+o->h();
    for (int i=children_-1; i--;) {
      o = *a++;
      if (o->x() < rx) rx = o->x();
      if (o->y() < ry) ry = o->y();
      if (o->x()+o->w() > rw) rw = o->x()+o->w();
      if (o->y()+o->h() > rh) rh = o->y()+o->h();
    }
    x(rx);
    y(ry);
    w(rw-rx);
    h(rh-ry);
  }
  // flip all the children's coordinate systems:
  //if (fl_flip) {
    fltk3::Widget* o = (type()>=fltk3::WINDOW) ? this : window();
    int Y = o->h();
    fltk3::Widget*const* a = array();
    for (int i=children(); i--;) {
      fltk3::Widget* ow = *a++;
      int newy = Y-ow->y()-ow->h();
      ow->y(newy);
    }
  //}
  end();
}

//
// End of "$Id$".
//
