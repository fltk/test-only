//
// "$Id: Fl_Group.cxx,v 1.34 1999/10/13 16:04:03 vincent Exp $"
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

int Fl_Group::find(const Fl_Widget* o) const {
  Fl_Widget*const* a = array();
  int i; for (i=0; i < children_; i++) if (*a++ == o) break;
  return i;
}

extern Fl_Widget* fl_oldfocus; // set by Fl::focus

// For back-compatability, we must adjust all events sent to child
// windows so they are relative to that window.

static int send(Fl_Widget* o, int event) {
  if (o->type() < FL_WINDOW) return o->handle(event);
  int save_x = Fl::e_x; Fl::e_x -= o->x();
  int save_y = Fl::e_y; Fl::e_y -= o->y();
  int ret = o->handle(event);
  Fl::e_y = save_y;
  Fl::e_x = save_x;
  return ret;
}

// translate the current keystroke into up/down/left/right for navigation:
#define ctrl(x) (x^0x40)
static int navkey() {
  switch (Fl::event_key()) {
  case FL_Tab:
    if (!Fl::event_state(FL_SHIFT)) return FL_Right;
  case 0xfe20: // XK_ISO_Left_Tab
    return FL_Left;
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

  Fl_Widget*const* a = array();
  Fl_Widget*const* e = array()+children();

  switch (event) {

  case FL_FOCUS:
    switch (navkey()) {
    default:
      if (savedfocus_ && savedfocus_->take_focus()) return 1;
    case FL_Right:
    case FL_Down:
      while (a < e) if ((*a++)->take_focus()) return 1;
      break;
    case FL_Left:
    case FL_Up:
      while (e > a) if ((*--e)->take_focus()) return 1;
      break;
    }
    return 0;

  case FL_UNFOCUS:
    savedfocus_ = fl_oldfocus;
    return 0;

  case FL_KEYBOARD:
    Fl_Tooltip::exit(this);
    return navigation(navkey());

  case FL_SHORTCUT:
    Fl_Tooltip::exit(this);
    while (e > a) {
      Fl_Widget* o = *--e;
      if (o->takesevents() && send(o,FL_SHORTCUT)) return 1;
    }
    if (Fl::event_key() == FL_Enter) return navigation(FL_Down);
    return 0;

  case FL_ENTER:
  case FL_MOVE:
    while (e > a) {
      Fl_Widget* o = *--e;
      if (o->takesevents() && Fl::event_inside(o)) {
        if (o->contains(Fl::belowmouse())) {
	  return send(o, FL_MOVE);
        } else if (send(o, FL_ENTER)) {
	  if (!o->contains(Fl::belowmouse())) Fl::belowmouse(o);
	  return 1;
        }
      }
    }
    Fl::belowmouse(this);
    return 1;

  case FL_PUSH:
    Fl_Tooltip::enter(0);
    while (e > a) {
      Fl_Widget* o = *--e;
      if (o->takesevents() && Fl::event_inside(o)) {
	if (send(o,FL_PUSH)) {
	  if (Fl::pushed() && !o->contains(Fl::pushed())) Fl::pushed(o);
	  return 1;
	}
      }
    }
    return 0;

  case FL_DEACTIVATE:
  case FL_ACTIVATE:
    while (a < e) {
      Fl_Widget* o = *a++;
      if (o->active()) o->handle(event);
    }
    return 1;

  case FL_SHOW:
  case FL_HIDE:
    while (a < e) {
      Fl_Widget* o = *a++;
      if (o->visible()) o->handle(event);
    }
    return 1;

  default:
    return 0;

  }
}

// try to move the focus in response to a keystroke:
int Fl_Group::navigation(int key) {
  if (children() <= 1) return 0;
  int i;
  for (i = 0; ; i++) {
    if (i >= children_) return 0;
    if (array_[i]->contains(Fl::focus())) break;
  }
  Fl_Widget *previous = array_[i];

  for (;;) {
    switch (key) {
    case FL_Right:
    case FL_Down:
      i++;
      if (i >= children_) {
	if (parent()) return 0;
	i = 0;
      }
      break;
    case FL_Left:
    case FL_Up:
      if (i) i--;
      else {
	if (parent()) return 0;
	i = children_-1;
      }
      break;
    default:
      return 0;
    }
    Fl_Widget* o = array_[i];
    if (o == previous) return 0;
    switch (key) {
    case FL_Down:
    case FL_Up:
      // for up/down, the widgets have to overlap horizontally:
      if (o->x() >= previous->x()+previous->w() ||
	  o->x()+o->w() <= previous->x()) continue;
    }
    if (o->take_focus()) return 1;
  }
}

////////////////////////////////////////////////////////////////

// this is private as there is no need for themes to alter this:
static Fl_Style group_style = {
  FL_NO_BOX // box
  // rest is zero and inherited from parent's style
};

Fl_Style_Definer fl_group_style_definer("group", group_style);

Fl_Group::Fl_Group(int X,int Y,int W,int H,const char *l)
: Fl_Widget(X,Y,W,H,l),
  children_(0),
  array_(0),
  savedfocus_(0),
  resizable_(this),
  sizes_(0), // this is allocated when the group is end()ed.
  ox_(X),
  oy_(Y),
  ow_(W),
  oh_(H) {
  type(FL_GROUP);
  style(group_style);
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
    savedfocus_ = 0;
    resizable_ = this;
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

void Fl_Group::insert(Fl_Widget &o, int i) {
  if (o.parent()) o.parent()->remove(o);
  o.parent_ = this;
  if (children_ == 0) {
    // allocate for 1 child
    array_ = (Fl_Widget**)malloc(sizeof(Fl_Widget));
    array_[0] = &o;
  } else {
    if (!(children_ & (children_-1))) // double number of children
      array_ = (Fl_Widget**)realloc((void*)array_,
				    2*children_*sizeof(Fl_Widget*));
    int j; for (j = children_; j > i; --j) array_[j] = array_[j-1];
    array_[j] = &o;
  }
  ++children_;
  init_sizes();
}

void Fl_Group::add(Fl_Widget &o) {insert(o, children_);}

void Fl_Group::remove(Fl_Widget &o) {
  int i = find(o);
  if (i >= children_) return;
  if (o.visible_r()) {
    // we must redraw the enclosing group that has an opaque box:
    for (Fl_Group *p = this; p; p = p->parent())
      if (p->box() != FL_NO_BOX || !p->parent()) {p->redraw(); break;}
  }
  if (&o == savedfocus_) savedfocus_ = 0;
  o.parent_ = 0;
  children_--;
  for (; i < children_; ++i) array_[i] = array_[i+1];
  init_sizes();
}

////////////////////////////////////////////////////////////////

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
    if (type() < FL_WINDOW) {p[0] = ox(); p[2] = oy();} else {p[0] = p[2] = 0;}
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

#if 0
// A child widget can use this inside layout() to figure out their
// previous size and position.  This does not seem to be working
// correctly, I removed the only call to it.  To fix this we need to
// duplicate exactly the code in layout()...
void Fl_Group::old_size(const Fl_Widget* o,int* r) {
  // get changes in size from the initial size:
  int* p = sizes();
  int dw = w()-(p[1]-p[0]);
  int dh = h()-(p[3]-p[2]);
  p+=4;
 
  // get initial size of resizable():
  int IX = *p++;
  int IR = *p++;
  int IY = *p++;
  int IB = *p++;

  // find the widget's current size:
  Fl_Widget*const* a = array();
  Fl_Widget*const* e = a+children_;
  while (a < e) {if (o == *a++) break; p+=4;}

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

  r[0]=X; r[1]=R-X; r[2]=Y; r[3]=B-Y;
}
#endif

void Fl_Group::layout() {

  Fl_Widget::layout();
  // get changes from previous position:
  if (!resizable() || ow()==w() && oh()==h()) {
    if (type() < FL_WINDOW) {
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
    if (type() >= FL_WINDOW) dx = dy = 0;
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
  set_old_size();
}

#include <FL/Fl_Tabs.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

// Draw the surrounding box of a group ; if non rectangular box, 
// draw parent's box behind
void Fl_Group::draw_group_box() const {
  if (!box()->rectangular)
    if (parent()) parent()->draw_group_box();
  draw_box();
}

void Fl_Group::draw() {
  Fl_Widget*const* a = array();
  Fl_Widget*const* e = a+children_;
  if (damage() == FL_DAMAGE_CHILD) {
    // only some child widget has been damaged, draw it
    while (a < e) update_child(**a++);
  } else {
    fl_clip(x(), y(), w(), h());
    while (e > a) {
      Fl_Widget& w = **--e;
      if (w.visible() && w.type() < FL_WINDOW &&
	  fl_not_clipped(w.x(), w.y(), w.w(), w.h()))
	w.draw_n_clip();
    }
    draw_group_box();
    draw_label();
    e = a+children_;
    while (a < e) {
      Fl_Widget& w = **a++;
      if (w.visible()) draw_outside_label(w);
    }
    fl_pop_clip();
  }
}

// Draw a child only if it needs it:
void Fl_Group::update_child(Fl_Widget& w) const {
  if (!w.visible()) return;
  if (w.damage() & FL_DAMAGE_CHILD_LABEL) {
    draw_outside_label(w);
    w.clear_damage(w.damage() & ~FL_DAMAGE_CHILD_LABEL);
  }
  if (w.damage() && w.type() < FL_WINDOW &&
      fl_not_clipped(w.x(), w.y(), w.w(), w.h())) {
    w.draw();	
    w.clear_damage();
  }
}

// Call the draw method, handle the clip out (nothing for groups)
void Fl_Group::draw_n_clip()
{
  clear_damage(FL_DAMAGE_ALL);
  draw();
  clear_damage();
  fl_clip_out(x(), y(), w(), h());
}

// Force a child to redraw:
void Fl_Group::draw_child(Fl_Widget& w) const {
  if (!w.visible()) return;
  draw_outside_label(w);
  if (w.type() < FL_WINDOW && fl_not_clipped(w.x(), w.y(), w.w(), w.h())) {
    w.clear_damage(FL_DAMAGE_ALL);
    w.draw();
    w.clear_damage();
  }
}

// Parents normally call this to draw outside labels:
void Fl_Group::draw_outside_label(Fl_Widget& w) const {
  if (!w.visible()) return;
  // skip any labels that are inside the widget:
  if (!(w.align()&15) || (w.align() & FL_ALIGN_INSIDE)) return;
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
// End of "$Id: Fl_Group.cxx,v 1.34 1999/10/13 16:04:03 vincent Exp $".
//
