//
// "$Id: Fl_Group.cxx,v 1.131 2004/04/17 18:58:19 spitzak Exp $"
//
// Group widget for the Fast Light Tool Kit (FLTK).
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

/*! \class fltk::Group

The fltk::Group class is the FLTK container widget. It maintains an
array of child widgets. These children can themselves be any widget
including fltk::Group, nesting groups allows much more control over
layout and resize behavior. Nested groups are also necessary to group
radio buttons together.

By default fltk::Group preserves the positions and sizes of all it's
children, they do not move no matter what sizes or other children are
added or removed.

Setting resizable() will change the layout behavior so that it
responds to resizing by moving or resizing the children to fit. See
below for details.

You may want to use an fltk::Pack or a fltk::Scroll to get other
common layout behavior that can respond to changes in the sizes of
child widgets.

The most-used subclass of fltk::Group is fltk::Window, all the rules
about resizing apply to windows. Setting resizable() on a window will
allow the user to resize it. If you want different behavior (such as
from fltk::Pack) for your window you should make the window have that
as a single resizable child that fills it.

fltk::Menu is a subclass and thus all menus and browsers are groups
and the items in them are widgets.

*/

#include <fltk/Group.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/events.h>
#include <fltk/layout.h>
#include <fltk/damage.h>
#include <stdlib.h>

using namespace fltk;

////////////////////////////////////////////////////////////////

FL_API Group* Group::current_;

static void revert(Style* s) {
  s->color_ = GRAY75;
  s->box_ = NO_BOX;
}

// This style is unnamed since there is no reason for themes to change it:
extern NamedStyle* group_style;
static NamedStyle the_style(0, revert, &group_style);
NamedStyle* group_style = &the_style;

/*! Creates a new fltk::Group widget using the given position, size,
  and label string. The default boxtype is fltk::NO_BOX. */
Group::Group(int X,int Y,int W,int H,const char *l,bool begin)
: Widget(X,Y,W,H,l),
  children_(0),
  focus_(-1),
  array_(0),
  resizable_(0), // fltk 1.0 used (this)
  sizes_(0)
{
  type(GROUP_TYPE);
  style(::group_style);
  align(ALIGN_TOP);
  if (begin) this->begin();
}

/*! \e Deletes all children from the group and makes it empty. 
  This calls the destructor on all the children!!! */
void Group::clear() {
  init_sizes();
  if (children_) {
    Widget*const* a = array_;
    Widget*const* e = a+children_;
    // clear everything now, in case fix_focus recursively calls us:
    children_ = 0;
    focus_ = -1;
    if (resizable_) resizable_ = this;
    // okay, now it is safe to destroy the children:
    while (e > a) {
      Widget* o = *--e;
      o->parent(0); // stops it from calling remove()
      delete o;
    }
    free((void*)a);
  }
}

/*! Calls clear(), and thus <i>deletes all child widgets</i> */
Group::~Group() {clear();}

/*! \fn Widget * Group::child(int n) const
  Returns a child, n >= 0 && n < children(). <i>No range checking is done!</i>
*/

/*! \fn int Group::children() const
  Returns how many child widgets the group has. */

/*! This does insert(w, find(beforethis)). This will append the
  widget if beforethis is not in the group. */
void Group::insert(Widget &o, int index) {
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
    array_ = (Widget**)malloc(sizeof(Widget*));
    array_[0] = &o;
  } else {
    if (!(children_ & (children_-1))) // double number of children
      array_ = (Widget**)realloc((void*)array_,
				    2*children_*sizeof(Widget*));
    for (int j = children_; j > index; --j) array_[j] = array_[j-1];
    array_[index] = &o;
  }
  ++children_;
  init_sizes();
}

/*! \fn Group * Group::current()
  Returns the group being currently built. The fltk::Widget
  constructor automatically does current()->add(widget) if this is not
  null. To prevent new widgets from being added to a group, call
  Group::current(0).
*/

/*! \fn void Group::begin()

  begin() sets the current group so you can build the widget tree by
  just constructing the widgets. begin() is exactly the same as current(this).

  <i>Don't forget to end() the group or window!</i>
*/

/*! \fn void Group::end()
  end() is exactly the same as current(this->parent()). Any new
  widgets added to the widget tree will be added to the parent of the
  group.
*/

/*! The widget is removed from it's current group (if any) and then
  added to the end of this group. */
void Group::add(Widget &o) {
  insert(o, children_);
}

/*! Remove the indexed widget from the group. */
void Group::remove(int index) {
  if (index >= children_) return;
  Widget* o = array_[index];
  o->parent(0);
  children_--;
  for (int i=index; i < children_; ++i) array_[i] = array_[i+1];
  init_sizes();
}

/*! \fn void Group::remove(Widget& widget)
  Removes a widget from the group. This does nothing if the widget is
  not currently a child of this group. */

/*! Remove the indexed widget and insert the passed widget in it's place. */
void Group::replace(int index, Widget& o) {
  if (index >= children_) {add(o); return;}
  o.parent(this);
  array_[index]->parent(0);
  array_[index] = &o;
  init_sizes();
}

/*! \fn void Group::replace(Widget& old, Widget& nu)
  Find the \a old widget and remove it and insert the \a nu one. If \a old
  is not a child, the new widget is appended to the end of the
  list. */

void Group::swap(int indexA, int indexB) {
  if (indexA >= children_ || indexB >= children_) return;
  Widget* o = array_[indexA];
  array_[indexA] = array_[indexB];
  array_[indexB] = o;
  init_sizes();
}

/*! Searches the children for \a widget, returns the index of \a
  widget or of a parent of \a widget that is a child() of
  this. Returns children() if the widget is NULL or not found. */
int Group::find(const Widget* widget) const {
  for (;;) {
    if (!widget) return children_;
    if (widget->parent() == this) break;
    widget = widget->parent();
  }
  // Search backwards so if children are deleted in backwards order
  // they are found quickly:
  for (int index = children_; index--;)
    if (array_[index] == widget) return index;
  return children_;
}

////////////////////////////////////////////////////////////////
// Handle

/*! \fn int Group::focus() const
  The index of the widget that contains the focus. You can initialize
  this before the group is displayed. Changing it while it is
  displayed does not work, use widget->take_focus() instead.  */

/*! Turn Tab into Right or Left for keyboard navigation */
int Group::navigation_key() {
  switch (event_key()) {
  case TabKey:
    if (event_state(CTRL)) return 0;
    return event_state(SHIFT) ? LeftKey : RightKey;
  case UpKey:
  case DownKey:
  case LeftKey:
  case RightKey:
    return event_key();
  default:
    return 0;
  }
}

/*! Calls send() on some or all of the children widgets. */
int Group::handle(int event) {
  const int numchildren = children();
  int i;

  switch (event) {

  case FOCUS_CHANGE:
    // The focus is being changed to some widget inside this.
    focus_ = find(fltk::focus());
    return true;

  case FOCUS:
    // an attempt to give this widget focus:
    switch (navigation_key()) {
    default: {
      // try to give it to whatever child had focus last:
      if (focus_ >= 0 && focus_ < numchildren)
	if (child(focus_)->take_focus()) return true;
      // otherwise search for the widget that needs the focus. We
      // give it to the first one that returns 2, or the last one
      // that returns 1 if none return 2:
      Widget* f1 = 0; int ret = 0;
      for (i = 0; i < numchildren; ++i) {
	Widget* w = child(i);
	if (w->takesevents()) {
	  int n = w->handle(FOCUS);
	  if (n) {ret = n; f1 = w; if (n & 2) break;}
	}
      }
      if (f1 && !f1->contains(fltk::focus())) focus(f1);
      return ret;}
    case RightKey:
    case DownKey:
      for (i=0; i < numchildren; ++i)
	if (child(i)->take_focus()) return true;
      return false;
    case LeftKey:
    case UpKey:
      for (i = numchildren; i--;)
	if (child(i)->take_focus()) return true;
      return false;
    }

  case DRAG:
  case RELEASE:
  case LEAVE:
  case DND_LEAVE:
    // Ignore these. We handle them if the belowmouse of pushed widget
    // has been set to this. Subclasses may do something with these.
    // Definately do not pass them to child widgets!
    break;

  case KEY: {
    // keyboard navigation
    if (numchildren < 2) break;
    int key = navigation_key();
    if (!key) break;
    int previous = focus_;
    if (previous < 0 || previous >= numchildren) previous = 0;
    for (i = previous;;) {
      if (key == LeftKey || key == UpKey) {
	if (i) --i;
	else {
	  if (parent()) return false;
	  i = numchildren-1;
	}
      } else {
	++i;
	if (i >= numchildren) {
	  if (parent()) return false;
	  i = 0;
	}
      }
      if (i == previous) {
	focus(0);
	return child(i)->take_focus();
      }
      if (key == DownKey || key == UpKey) {
	// for up/down, the widgets have to overlap horizontally:
	Widget* o = child(i);
	Widget* p = child(previous);
	if (o->x() >= p->x()+p->w() || o->x()+o->w() <= p->x()) continue;
      }
      if (child(i)->take_focus()) return true;
    }}

  case PUSH:
  case ENTER:
  case MOVE:
  case DND_ENTER:
  case DND_DRAG:
    // search the children in backwards (top to bottom) order:
    for (i = numchildren; i--;) {
      Widget* child = this->child(i);
      // ignore widgets we are not pointing at:
      if (event_x() < child->x()) continue;
      if (event_x() >= child->x()+child->w()) continue;
      if (event_y() < child->y()) continue;
      if (event_y() >= child->y()+child->h()) continue;
      // see if it wants the event:
      if (child->send(event)) return true;
      // quit when we reach a widget that claims mouse points at it,
      // so we don't pass the events to widgets "hidden" behind that one.
      if (event != ENTER && event != MOVE &&
	  child->contains(fltk::belowmouse())) return false;
    }
    return Widget::handle(event);

  default:
    // Try to give any other event to the focus:
    if (focus_ >= 0 && focus_ < numchildren)
      if (child(focus_)->send(event)) return true;
    // Then try all other children in top to bottom order:
    for (i = numchildren; i--;) if (i != focus_)
      if (child(i)->send(event)) return true;
    break;

  }
  return Widget::handle(event);
}

////////////////////////////////////////////////////////////////
// Layout

/*! \fn Widget* Group::resizable() const

  The resizable widget defines the resizing box for the group. When
  the group is resized it calculates a new size and position for all
  of its children. Widgets that are horizontally or vertically inside
  the dimensions of the box are scaled to the new size. Widgets
  outside the box are moved.

  Orignal size, the gray area is the resizable():
  \image html resizebox1.gif

  And here is the same Group resized larger:
  \image html resizebox2.gif

  The resizable may be set to the group itself, in which case all the
  contents are resized. If the resizable is NULL (the default) then
  all widgets remain a fixed size and distance from the top-left
  corner.

  It is possible to achieve any type of resize behavior by using an
  InvisibleBox as the resizable and/or by using a hierarchy of child
  fltk::Group's.
*/

/** Indicate that all the remembered child sizes should be reinitialized.

    The group remembers the initial size of itself and all it's children,
    so that the layout can be restored even if the group is resized so
    that some children go to zero or negative sizes. Normally these
    sizes are calculated the first time layout() is called, though you
    can do so by calling sizes() directly.

    Though this makes sense it often results in unexpected behavior
    when a program wants to rearrange the child widgets or change the
    size of a group to surround a new arrangement of child widgets. The
    widgets tend to snap back to a previous size.

    Calling init_sizes() "resets" the sizes array to the current group
    and children positions.  Actually it just deletes the sizes array,
    and it is not recreated until the next time layout is called. Call
    this if you manually adjust the sizes of the children, or attempt
    to change the size of the group without wanting the children to scale.

    This is automatically done when any child is added or removed.  */
void Group::init_sizes() {
  delete[] sizes_; sizes_ = 0;
  relayout();
}

/** This non-virtual override is for programs that set up a group's
    layout and then call resize() on it to set the correct size before
    it is displayed. What it does is remember the current sizes (the
    thing the init_sizes() method makes it forget) before calling the
    normal widget resize().

    This is a non-virtual override because in normal use fltk will call
    layout() anyway before any use of the widget, and Group's layout()
    initializes the sizes. This is only for programs that use resize()
    directly.
*/
bool Group::resize(int x, int y, int w, int h) {
  if (!sizes_ && resizable() && children_ && (w != this->w() || h != this->h()))
    layout(); // this is needed to recursively get inner groups...
  return Widget::resize(x,y,w,h);
}

/** Returns array of initial sizes of the widget and it's children.

    The sizes() array stores the initial positions of widgets as
    left,right,top,bottom quads.  The first quad is the group, the
    second is the resizable (clipped to the group), and the
    rest are the children.  This is a convienent order for the
    algorithim.

    This array is not calculated until the first time sizes() is
    called. It then returns the same array from then on. Calling
    init_sizes() deletes the array so the next call of this creates
    a new one.
*/
int* Group::sizes() {
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
    Widget* r = resizable();
    if (r && r != this) { // then clip the resizable to it
      int t;
      t = r->x(); if (t > 0) p[4] = t;
      t +=r->w(); if (t < p[1]) p[5] = t;
      t = r->y(); if (t > 0) p[6] = t;
      t +=r->h(); if (t < p[3]) p[7] = t;
    }
    // next is all the children's sizes:
    p += 8;
    Widget*const* a = array_;
    Widget*const* e = a+children_;
    while (a < e) {
      Widget* o = *a++;
      *p++ = o->x();
      *p++ = o->x()+o->w();
      *p++ = o->y();
      *p++ = o->y()+o->h();
    }
  }
  return sizes_;
}

void Group::layout() {

  // Save the layout damage and then clear it. This is so layout() of a
  // child can turn it back on and subclasses like PackedGroup can detect that:
  int layout_damage = this->layout_damage();
  Widget::layout();

  if (resizable() && children_) {
    int* p = sizes(); // initialize the size array

    if (layout_damage&LAYOUT_WH) {

      // get changes in size from the initial size:
      int dw = w()-p[1];
      int dh = h()-p[3];

      p+=4;

      // Calculate a new size & position for every child widget:
      // get initial size of resizable():
      int IX = *p++;
      int IR = *p++;
      int IY = *p++;
      int IB = *p++;

      Widget*const* a = array_;
      Widget*const* e = a+children_;
      while (a < e) {
	Widget* o = *a++;
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
      }
    }
  }

  Widget*const* a = array_;
  Widget*const* e = a+children_;
  if ((layout_damage & LAYOUT_XY) && !is_window()) {
    // If this is not an Window and the xy position is changed, we must
    // call layout() on every child. This is necessary so that child
    // Windows will move to their new positions.
    while (a < e) {
      Widget* widget = *a++;
      widget->layout_damage(widget->layout_damage()|LAYOUT_XY);
      widget->layout();
    }
  } else {
    // Otherwise we only need to call layout on children with the
    // layout bit set:
    while (a < e) {
      Widget* widget = *a++;
      if (widget->layout_damage()) widget->layout();
    }
  }

  if (layout_damage & LAYOUT_WH) redraw();
}

////////////////////////////////////////////////////////////////
// Draw

void Group::draw() {
  int numchildren = children();
  if (damage() & ~DAMAGE_CHILD) {
#if 0
    // blinky-draw:
    draw_box();
    draw_label();
    int n; for (n = 0; n < numchildren; n++) {
      Widget& w = *child(n);
      w.set_damage(DAMAGE_ALL|DAMAGE_EXPOSE);
      update_child(w);
    }
#else
    // Non-blinky draw, draw the inside widgets first, clip their areas
    // out, and then draw the background:
    push_clip(0, 0, w(), h());
    int n; for (n = numchildren; n--;) draw_child(*child(n));
    draw_box();
    draw_label();
    pop_clip();
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
      Widget& w = *child(n);
      if (w.damage() & DAMAGE_CHILD_LABEL) {
	draw_outside_label(w);
	w.set_damage(w.damage() & ~DAMAGE_CHILD_LABEL);
      }
      update_child(w);
    }
  }
}

// Pieces of draw() that subclasses may want to use:

/*! Draw the background. If DAMAGE_EXPOSE is on, widgets are expected
  to completely fill their rectangle. To allow non-rectangular widgets
  to appear to work, a widget can call this to draw the area of it's
  parent that is visible behind it. If you want to avoid blinking you
  should draw your contents first, clip them out, then call this. */
void Widget::draw_background() const {
  if (!parent()) {
    setcolor(color());
    fillrect(0, 0, w(), h());
    return;
  }
  push_clip(0, 0, w(), h());
  push_matrix();
  translate(-x(), -y());
  if (!parent()->box()->fills_rectangle()) parent()->draw_background();
  parent()->draw_box();
  parent()->draw_label();
  pop_matrix();
  pop_clip();
}

// Widgets that want to outwit the clip-out can set this when they are
// drawn to indicate that they did the clip-out. Only TabGroup really uses
// this (and I'm not certain it has to), plus a bunch of back-compatability
// widgets that want to be "invisible" (they turn this on but don't draw
// anything). This is a pointer so if it is left on by a child widget
// it does not fool this into thinking the clipping is done.
Widget* fl_did_clipping;

/*! Force a child to redraw and remove the rectangle it used from the clip
  region. */
void Group::draw_child(Widget& w) const {
  if (w.visible() && !w.is_window()) {
    if (!not_clipped(w.x(), w.y(), w.w(), w.h())) return;
    push_matrix();
    translate(w.x(), w.y());
    fl_did_clipping = 0;
    w.set_damage(DAMAGE_ALL|DAMAGE_EXPOSE);
    w.draw();
    w.set_damage(0);
    if (fl_did_clipping != &w) clip_out(0,0,w.w(),w.h());
    pop_matrix();
  }
}

/*! Redraw a single child in response to it's damage. */
void Group::update_child(Widget& w) const {
  if (w.damage() && w.visible() && !w.is_window()) {
    if (!not_clipped(w.x(), w.y(), w.w(), w.h())) return;
    push_matrix();
    translate(w.x(), w.y());
    w.draw();	
    w.set_damage(0);
    pop_matrix();
  }
}

/** Add x() and y() to the position of all children. This can be used
    to fix children widgets created for fltk1.1, where the x/y were
    always relative to the window, rather than the surrounding group.
*/
void Group::fix_old_positions() {
  if (is_window()) return; // in fltk 1.0 children of windows were relative
  for (int i = 0; i < children(); i++) {
    Widget& w = *(child(i));
    w.x(w.x()-x());
    w.y(w.y()-y());
  }
}

//
// End of "$Id: Fl_Group.cxx,v 1.131 2004/04/17 18:58:19 spitzak Exp $".
//
