//
// "$Id$"
//
// Base widget class for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/Wrapper.h>
#include <fltk3/Group.h>
#include <fltk3/Tooltip.h>
#include <fltk3/draw.h>
#include <stdlib.h>
#include "flstring.h"


////////////////////////////////////////////////////////////////
// for compatibility with Forms, all widgets without callbacks are
// inserted into a "queue" when they are activated, and the forms
// compatibility interaction functions (fl_do_events, etc.) will
// read one widget at a time from this queue and return it:

const int QUEUE_SIZE = 20;

static fltk3::Widget *obj_queue[QUEUE_SIZE];
static int obj_head, obj_tail;

void fltk3::Widget::default_callback(fltk3::Widget *o, void * /*v*/) {
#if 0
  // This is necessary for strict forms compatibility but is confusing.
  // Use the parent's callback if this widget does not have one.
  for (fltk3::Widget *p = o->parent(); p; p = p->parent())
    if (p->callback() != default_callback) {
      p->do_callback(o,v);
      return;
    }
#endif
  obj_queue[obj_head++] = o;
  if (obj_head >= QUEUE_SIZE) obj_head = 0;
  if (obj_head == obj_tail) {
    obj_tail++;
    if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  }
}
/**
    All Fl_Widgets that don't have a callback defined use a
    default callback that puts a pointer to the widget in this queue,
    and this method reads the oldest widget out of this queue.
*/
fltk3::Widget *fltk3::readqueue() {
  if (obj_tail==obj_head) return 0;
  fltk3::Widget *o = obj_queue[obj_tail++];
  if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  return o;
}
/*
    This static internal function removes all pending callbacks for a
    specific widget from the default callback queue (fltk3::readqueue()).
    It is only called from fltk3::Widget's destructor if the widget
    doesn't have an own callback.
    Note: There's no need to have this in the fltk3:: namespace.
*/
static void cleanup_readqueue(fltk3::Widget *w) {

  if (obj_tail==obj_head) return;
  
  // Read the entire queue and copy over all valid entries.
  // The new head will be determined after the last copied entry.

  int old_head = obj_head;	// save newest entry
  int entry = obj_tail;		// oldest entry
  obj_head = obj_tail;		// new queue start
  for (;;) {
    fltk3::Widget *o = obj_queue[entry++];
    if (entry >= QUEUE_SIZE) entry = 0;
    if (o != w) { // valid entry
      obj_queue[obj_head++] = o;
      if (obj_head >= QUEUE_SIZE) obj_head = 0;
    } // valid entry
    if (entry == old_head) break;
  }
  return;
}
////////////////////////////////////////////////////////////////

fltk3::Label::Label(int X, int Y, int W, int H, const char* L) 
: fltk3::Rectangle(X, Y, W, H),
  style_(&fltk3::default_style),
  labeltext_(L),
  flags_(0),
  image_(0),
  deimage_(0)
{
}

fltk3::Label::Label(const fltk3::Label &s) 
: fltk3::Rectangle(s),
  style_(s.style_->as_public()),
  labeltext_(0L),
  flags_(s.flags_),
  image_(s.image_),
  deimage_(s.deimage_)
{
  if (s.labeltext_) {
    if (s.flags() & COPIED_LABEL) {
      copy_label(s.labeltext_);
    } else {
      labeltext_ = s.labeltext_;
    }
  }
}

fltk3::Label::Label() 
: fltk3::Rectangle(),
  style_(&fltk3::default_style),
  flags_(0),
  image_(0),
  deimage_(0)
{
}

fltk3::Label::~Label() {
  if (flags() & COPIED_LABEL) 
    free((void *)(labeltext_));
  //if (style_ && style_->is_private())
  //  delete style_;
}

////////////////////////////////////////////////////////////////

int fltk3::Widget::handle(int event) {
  return 0;
}

/** Default font size for widgets */
fltk3::Fontsize fltk3::NORMAL_SIZE = 14;

fltk3::Widget::Widget(int X, int Y, int W, int H, const char* L) 
: fltk3::Label(X, Y, W, H, L)
{
  tooltip_       = 0;
  callback_	 = default_callback;
  user_data_ 	 = 0;
  type_		 = 0;
  flags_	 = VISIBLE_FOCUS;
  damage_	 = 0;
  when_		 = fltk3::WHEN_RELEASE;
  
  parent_ = 0;
  if (fltk3::Group::current()) fltk3::Group::current()->add(this);
}

fltk3::Widget::Widget(fltk3::Box* b, int X, int Y, int W, int H, const char* L) 
: fltk3::Label(X, Y, W, H, L)
{
  tooltip_       = 0;
  callback_	 = default_callback;
  user_data_ 	 = 0;
  type_		 = 0;
  flags_	 = VISIBLE_FOCUS;
  damage_	 = 0;
  when_		 = fltk3::WHEN_RELEASE;
  parent_ = 0;
  if (fltk3::Group::current()) fltk3::Group::current()->add(this);
  
  box(b);
}

void fltk3::Widget::resize(int X, int Y, int W, int H) {
  x_ = X; y_ = Y; w_ = W; h_ = H;
}

// this is useful for parent widgets to call to resize children:
int fltk3::Widget::damage_resize(int X, int Y, int W, int H) {
  if (x() == X && y() == Y && w() == W && h() == H) return 0;
  resize(X, Y, W, H);
  redraw();
  return 1;
}

int fltk3::Widget::take_focus() {
  if (!takesevents()) return 0;
  if (!visible_focus()) return 0;
  if (!handle(fltk3::FOCUS)) return 0; // see if it wants it
  if (contains(fltk3::focus())) return 1; // it called fltk3::focus for us
  fltk3::focus(this);
  return 1;
}

extern void fl_throw_focus(fltk3::Widget*); // in Fl_x.cxx

/**
   Destroys the widget, taking care of throwing focus before if any.
   Destruction removes the widget from any parent group! And groups when
   destroyed destroy all their children. This is convenient and fast.
*/
fltk3::Widget::~Widget() {
  fltk3::clear_widget_pointer(this);
  if (flags() & COPIED_TOOLTIP) free((void *)(tooltip_));
  // remove from parent group
  if (parent_) parent_->remove(this);
#ifdef DEBUG_DELETE
  if (parent_) { // this should never happen
    printf("*** fltk3::Widget: parent_->remove(this) failed [%p,%p]\n",parent_,this);
  }
#endif // DEBUG_DELETE
  parent_ = 0; // Don't throw focus to a parent widget.
  fl_throw_focus(this);
  // remove stale entries from default callback queue (fltk3::readqueue())
  if (callback_ == default_callback) cleanup_readqueue(this);
}


/** draws a focus rectangle around the widget */
void fltk3::Widget::draw_focus() {
  draw_focus(box(),0,0,w(),h());
}

/** Draws a focus box for the widget at the given position and size */
void
fltk3::Widget::draw_focus(fltk3::Box* B, int X, int Y, int W, int H) const {
  if (!fltk3::visible_focus()) return;
  if (   B==fltk3::DOWN_BOX
      || B==fltk3::DOWN_FRAME
      || B==fltk3::THIN_DOWN_BOX
      || B==fltk3::THIN_DOWN_FRAME)
  {
    X ++;
    Y ++;
  }

  fltk3::color(fltk3::contrast(fltk3::BLACK, color()));

#if defined(USE_X11) || defined(__APPLE_QUARTZ__)
  fltk3::line_style(fltk3::DOT);
  fltk3::rect(X + fltk3::box_dx(B), Y + fltk3::box_dy(B),
          W - fltk3::box_dw(B) - 1, H - fltk3::box_dh(B) - 1);
  fltk3::line_style(fltk3::SOLID);
#elif defined(WIN32) 
  // Windows 95/98/ME do not implement the dotted line style, so draw
  // every other pixel around the focus area...
  //
  // Also, QuickDraw (MacOS) does not support line styles specifically,
  // and the hack we use in fltk3::line_style() will not draw horizontal lines
  // on odd-numbered rows...
  int i, xx, yy;

  X += fltk3::box_dx(B);
  Y += fltk3::box_dy(B);
  W -= fltk3::box_dw(B) + 2;
  H -= fltk3::box_dh(B) + 2;

  for (xx = 0, i = 1; xx < W; xx ++, i ++) if (i & 1) fltk3::point(X + xx, Y);
  for (yy = 0; yy < H; yy ++, i ++) if (i & 1) fltk3::point(X + W, Y + yy);
  for (xx = W; xx > 0; xx --, i ++) if (i & 1) fltk3::point(X + xx, Y + H);
  for (yy = H; yy > 0; yy --, i ++) if (i & 1) fltk3::point(X, Y + yy);
#else
# error unsupported platform
#endif // WIN32
}


void fltk3::Widget::activate() {
  if (!active()) {
    clear_flag(INACTIVE);
    if (active_r()) {
      redraw();
      redraw_label();
      handle(fltk3::ACTIVATE);
      if (inside(fltk3::focus())) fltk3::focus()->take_focus();
    }
  }
}

void fltk3::Widget::deactivate() {
  if (active_r()) {
    set_flag(INACTIVE);
    redraw();
    redraw_label();
    handle(fltk3::DEACTIVATE);
    fl_throw_focus(this);
  } else {
    set_flag(INACTIVE);
  }
}

int fltk3::Widget::active_r() const {
  for (const fltk3::Widget* o = this; o; o = o->parent())
    if (!o->active()) return 0;
  return 1;
}

void fltk3::Widget::show() {
  if (!visible()) {
    clear_flag(INVISIBLE);
    if (visible_r()) {
      redraw();
      redraw_label();
      handle(fltk3::SHOW);
      if (inside(fltk3::focus())) fltk3::focus()->take_focus();
    }
  }
}

void fltk3::Widget::hide() {
  if (visible_r()) {
    set_flag(INVISIBLE);
    for (fltk3::Widget *p = parent(); p; p = p->parent())
      if (p->box() || !p->parent()) {p->redraw(); break;}
    handle(fltk3::HIDE);
    fl_throw_focus(this);
  } else {
    set_flag(INVISIBLE);
  }
}

int fltk3::Widget::visible_r() const {
  for (const fltk3::Widget* o = this; o; o = o->parent())
    if (!o->visible()) return 0;
  return 1;
}

// return true if widget is inside (or equal to) this:
// Returns false for null widgets.
int fltk3::Widget::contains(const fltk3::Widget *o) const {
  for (; o; o = o->parent_) if (o == this) return 1;
  return 0;
}


void
fltk3::Label::label(const char *a) {
  if (flags() & COPIED_LABEL) {
    // reassigning a copied label remains the same copied label
    if (labeltext_ == a)
      return;
    free((void *)(labeltext_));
    clear_flag(COPIED_LABEL);
  }
  labeltext_ = a;
  redraw_label();
}


void
fltk3::Label::copy_label(const char *a) {
  // reassigning a copied label remains the same copied label
  if ((flags() & COPIED_LABEL) && (labeltext_ == a))
    return;
  if (a) {
    label(strdup(a));
    set_flag(COPIED_LABEL);
  } else {
    label(0);
  }
}

/** Calls the widget callback.

  Causes a widget to invoke its callback function with arbitrary arguments.

  \param[in] o call the callback with \p o as the widget argument
  \param[in] arg use \p arg as the user data argument
  \see callback()
*/
void
fltk3::Widget::do_callback(fltk3::Widget* o,void* arg) {
  fltk3::WidgetTracker wp(this);
  if (o && o->pWrapper && o->callback_!=default_callback && o->callback_!=(CallbackPtr)Window::default_callback) {
    callback_((fltk3::Widget*)o->pWrapper, arg);
  } else {
    callback_(o,arg);
  }
  if (wp.deleted()) return;
  if (callback_ != default_callback)
    clear_changed();
}

/** Draw a box.
 */
void fltk3::Widget::draw() {
  draw_box();
  draw_label();
}


// ========================= Wrapper Support ===================================

fltk3::Object::~Object() {
  if ( pWrapper && !(pWrapper->pVCalls&Wrapper::pVCallDtor) ) {
    pWrapper->pVCalls |= Wrapper::pVCallDtor;
    delete pWrapper;
  }
}

fltk3::Wrapper::~Wrapper() {
  if ( !(pVCalls&pVCallDtor) ) {
    pVCalls |= pVCallDtor;
    delete _p;
  }
  else if (_p) {
    _p->wrapper(NULL);
    }
}


// ========================= Rectangle Support ===================================

/*! \class fltk3::Rectangle
  Describes an integer-sized rectangle. This is the base class of
  Widget, and also used a lot to pass rectangular areas to drawing
  functions. Almost all the functions are inline.

  Negative w() or h() is supposed to mean an empty and thus
  invisible rectangle, but some code will treat the rectangle as
  reflected about x or y. Set the size to zero to make sure you
  have an empty one.
*/

/*! \fn Rectangle::Rectangle()
  The default constructor does not put anything into the fields!
  You can either call set() or just modify the x_, y_, w_, and h_
  variables directly.
*/

/** Initialize to the size w,h. The rectangle is placed inside the
    source rectangle \a r either centered or against an edge depending
    on the ALIGN values in \a flags. For centered alignment if the
    difference in sizes is odd, it always rounds up and left.
    Default value for \a flags is to center in both directions.
 */
void fltk3::Rectangle::set(const fltk3::Rectangle& r, int w, int h, int flags) {
  if (flags & ALIGN_LEFT) {
    if (flags & ALIGN_RIGHT &&  w > r.w()) x_ = r.r()-w;
    else x_ = r.x();
  } else if (flags & ALIGN_RIGHT) {
    x_ = r.r()-w;
  } else {
    x_ = r.x()+((r.w()-w)>>1);
    // fabien: shouldn't it  consider the case r is smaller to avoid negative values ?
    // WAS: no, it is supposed to center at all times. The right-shift
    // instead of divide-by-2 is to avoid shifting as it goes negative.
    // fabien : well while debugging i observed the shift doesn't avoid
    //    to get negative value at least on Win32
    // WAS: no, it is *supposed* to return a negative value! I want the
    // rectangle "centered" even if it is *bigger*.
    // if (x_<0) x_=0;
  }
  if (flags & ALIGN_TOP) {
    if (flags & ALIGN_BOTTOM && h > r.h()) y_ = r.b()-h;
    else y_ = r.y();
  } else if (flags & ALIGN_BOTTOM) {
    y_ = r.b()-h;
  } else {
    y_ = r.y()+((r.h()-h)>>1);
    // see above
    // if (y_<0) y_=0;
  }
  w_ = w;
  h_ = h;
}

/**
  Replace the value with the union of this rectangle and \a R
  (ie the rectangle that surrounds both of these rectangles).
  If one rectangle is empty(), the other is returned unchanged
  (ie it does not union in the degenerate point of that rectangle).
*/
void fltk3::Rectangle::merge(const fltk3::Rectangle& R) {
  if (R.empty()) return;
  if (empty()) {*this = R; return;}
  if (R.x() < x()) set_x(R.x());
  if (R.r() > r()) set_r(R.r());
  if (R.y() < y()) set_y(R.y());
  if (R.b() > b()) set_b(R.b());
}

/**
  Replace the value with the intersection of this rectangle and \a R.
  If the rectangles do not intersect, the result may have negative
  width and/or height, this means empty() will return true, but some
  code may still draw this rectangle.
*/
void fltk3::Rectangle::intersect(const fltk3::Rectangle& R) {
  if (R.x() > x()) set_x(R.x());
  if (R.r() < r()) set_r(R.r());
  if (R.y() > y()) set_y(R.y());
  if (R.b() < b()) set_b(R.b());
}


// Call to->handle(), but first replace the mouse x/y with the correct
// values to account for nested windows. 'window' is the outermost
// window the event was posted to by the system
int fltk3::Widget::send(int event) {
  int dx = 0, dy = 0;
  for (const Widget* w = this; w; w = w->parent()) {
    dx += w->x();
    dy += w->y();
  }
  int save_x = e_x; e_x = e_x_root-dx;
  int save_y = e_y; e_y = e_y_root-dy;
  Widget* save_e_widget = e_widget;

  int old_event = fltk3::e_number; fltk3::e_number = event;
  // the widget to which fltk3::event_x() and event_y() coordinates are relative
  fltk3::e_widget = this; 
  int ret = handle(fltk3::e_number);
  fltk3::e_widget = save_e_widget;
  fltk3::e_number = old_event;
  fltk3::e_y = save_y;
  fltk3::e_x = save_x;
  
  return ret;
}

int fltk3::Widget::dx_window()
{
  int dx = x();
  for (const Widget* w = parent(); w; w = w->parent()) {
    if (w->type()>=WINDOW) break;
    dx += w->x();
  }
  return dx;
}

int fltk3::Widget::dy_window()
{
  int dy = y();
  for (const Widget* w = parent(); w; w = w->parent()) {
    if (w->type()>=WINDOW) break;
    dy += w->y();
  }
  return dy;
}


//
// End of "$Id$".
//
