//
// "$Id: Fl_Widget.cxx,v 1.91 2002/06/09 23:20:19 spitzak Exp $"
//
// Base widget class for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Window.h>
#include <fltk/fl_draw.h>
#include <string.h> // for strdup
#include <stdlib.h> // free
#include <config.h>

void Fl_Widget::default_callback(Fl_Widget*, void*) {}
    
Fl_Widget::Fl_Widget(int X, int Y, int W, int H, const char* L) {
  style_	= default_style;
  parent_	= 0;
  callback_	= default_callback;
  user_data_ 	= 0;
  label_	= L;
  image_	= 0;
  tooltip_	= 0;
  shortcut_	= 0;
#if CLICK_MOVES_FOCUS
  flags_	= FL_CLICK_TO_FOCUS;
#else
  flags_	= 0;
#endif
  x_ = X; y_ = Y; w_ = W; h_ = H;
  type_		= 0;
  damage_	= FL_DAMAGE_ALL;
  layout_damage_= FL_LAYOUT_DAMAGE;
  when_		= FL_WHEN_RELEASE;
  if (Fl_Group::current()) Fl_Group::current()->add(this);
}

Fl_Widget::~Fl_Widget() {
  if (parent_) parent_->remove(this);
  throw_focus();
  if (style_->dynamic()) {
    // When a widget is destroyed it can destroy unique styles:
    delete (Fl_Style*)style_; // cast away const
  }
  if (flags_&FL_COPIED_LABEL) free((void*)label_);
}

void Fl_Widget::label(const char* a) {
  if (flags_&FL_COPIED_LABEL) {
    free((void*)label_);
    flags_ &= ~FL_COPIED_LABEL;
  }
  label_ = a;
}

void Fl_Widget::copy_label(const char* s) {
  if (flags_&FL_COPIED_LABEL) free((void*)label_);
  if (s) {
    label_ = strdup(s);
    flags_ |= FL_COPIED_LABEL;
  } else {
    label_ = 0;
    flags_ &= ~FL_COPIED_LABEL;
  }
}

////////////////////////////////////////////////////////////////
// layout damage:

void Fl_Widget::layout() {
  layout_damage_ = 0;
}

int Fl_Widget::height() {
  if (!h_) layout();
  return h_;
}

int Fl_Widget::width() {
  if (!w_) layout();
  return w_;
}

bool Fl_Widget::resize(int X, int Y, int W, int H) {
  uchar flags = 0;
  if (X != x_) flags = FL_LAYOUT_X;
  if (Y != y_) flags |= FL_LAYOUT_Y;
  if (W != w_) flags |= FL_LAYOUT_W;
  if (H != h_) flags |= FL_LAYOUT_H;
  if (flags) {
    x_ = X; y_ = Y; w_ = W; h_ = H;
    // parent must get FL_LAYOUT_DAMAGE as well as FL_LAYOUT_CHILD:
    if (parent()) {
      layout_damage_ |= flags;
      parent()->relayout(FL_LAYOUT_DAMAGE|FL_LAYOUT_CHILD);
    } else {
      relayout(flags);
    }
    return true;
  } else {
    return false;
  }
}

void Fl_Widget::relayout() {
  relayout(FL_LAYOUT_DAMAGE);
}

void Fl_Widget::relayout(uchar flags) {
  //if (!(flags & ~layout_damage_)) return;
  layout_damage_ |= flags;
  for (Fl_Widget* w = this->parent(); w; w = w->parent())
    w->layout_damage_ |= FL_LAYOUT_CHILD;
  Fl::damage(FL_LAYOUT_DAMAGE); // make Fl::flush() do something
}

////////////////////////////////////////////////////////////////
// damage:

/*

New scheme to reduce the expense of calling redraw() on widgets.
The loss here is that if a widget is changed and also gets exposed, it
will be drawn twice. The gain is that region calculations are not done
on every redraw(), restoring the speed we had in earlier fltk.

I renamed damage(n) to redraw(n) to match fltk method style. damage(n)
is reserved for changing damage_ but for now use set_damage(n) for this.

Normal redraw(n) will turn on those damage bits in the widget and turn
on FL_DAMAGE_CHILD in all the parents. Groups should call redraw_child()
on each child when FL_DAMAGE_CHILD is on.

Expose events accumulates a region but does not turn on any damage
bits.  The flush() method on a window must cause this area to be
updated. This may be done by copying a back buffer. The normal window
calls draw() again with the clip region set and damage() set to
FL_DAMAGE_EXPOSE. All widgets should redraw every pixel in the clip
region if FL_DAMAGE_EXPOSE is turned on. Exposure that covers the
entire window is detected and changed into FL_DAMAGE_ALL, so only one
redraw is done.

The redraw(x,y,w,h) function does the same thing as expose events but
it sets FL_DAMAGE_EXPOSE. This is so flush() for double-buffered
windows can tell these apart from normal expose events.

*/

void Fl_Widget::redraw() {
  redraw(FL_DAMAGE_ALL);
}

void Fl_Widget::redraw(uchar flags) {
  if (!(flags & ~damage_)) return;
  damage_ |= flags;
  if (!is_window())
    for (Fl_Widget* widget = parent(); widget; widget = widget->parent()) {
      widget->damage_ |= FL_DAMAGE_CHILD;
      if (widget->is_window()) break;
    }
  Fl::damage(FL_DAMAGE_CHILD);
}

void Fl_Widget::redraw_label() {
  if (!label() && !image()) return;
  // inside label redraws the widget:
  if (!(flags()&15) || (flags() & FL_ALIGN_INSIDE)) redraw();
  // outside label requires a marker flag and damage to parent:
  else redraw(FL_DAMAGE_CHILD_LABEL);
}

////////////////////////////////////////////////////////////////
// Events:

// Default handler. This returns 1 for mouse movement over opaque widgets,
// so they can block widgets they overlap from getting events, also so
// you can put tooltips on them.
int Fl_Widget::handle(int event) {
  switch (event) {
  case FL_ENTER:
  case FL_MOVE:
    // Though returning true will work for normal widgets, it will not
    // work if this is a group and some child has the belowmouse because
    // send() will not change the belowmouse then. Setting belowmouse
    // directly fixes this.
    // The check for is_window is there to fix problems with a large
    // number of older fltk programs that set FL_NO_BOX on windows to
    // stop them from blinking (this is not necessary in fltk2.0):
    if (box()!=FL_NO_BOX || is_window()) {Fl::belowmouse(this); return true;}
  default:
    return 0;
  }
}

// send(event) is a wrapper for handle() that should be called to send
// events. It does a few things:
//
// 1. It adjusts Fl::event_x/y to be relative to the widget.
//
// 2. It makes sure the widget is active and/or visible if the event
// requres this. It is the caller's responsibility to see if the
// mouse is pointing at the widget.
//
// 3. If handle returns true it sets the belowmouse or focus widget
// to reflect this.

int Fl_Widget::send(int event) {

  // Figure out the mouse position in this widget from the root mouse position:
  int ex = Fl::e_x_root;
  int ey = Fl::e_y_root;
  {for (Fl_Widget *t=this; t; t = t->parent()) {ex -= t->x(); ey -= t->y();}}
  int save_x = Fl::e_x;
  Fl::e_x = ex;
  int save_y = Fl::e_y;
  Fl::e_y = ey;

  int ret = 0;
  switch (event) {

  case FL_FOCUS:
    if (!takesevents()) break;
    ret = handle(event);
    if (ret) {
      // If it returns true then this is the focus() widget, but only
      // set this if handle() did not pass this on to a child:
      if (!contains(Fl::focus())) Fl::focus(this);
    }
    break;

  case FL_ENTER:
  case FL_MOVE:
    if (!visible()) break;
    // figure out correct type of event:
    event = (contains(Fl::belowmouse())) ? FL_MOVE : FL_ENTER;
    ret = handle(event);
    if (ret) {
      // If return value is true then this is the belowmouse widget,
      // set it, but only if handle() did not set it to some child:
      if (!contains(Fl::belowmouse())) Fl::belowmouse(this);
    }
    break;

  case FL_DND_ENTER:
  case FL_DND_DRAG:
    if (!takesevents()) break;
    // figure out correct type of event:
    event = (contains(Fl::belowmouse())) ? FL_DND_DRAG : FL_DND_ENTER;
    // see if it wants the event:
    ret = handle(event);
    if (ret) {
      // If return value is true then this is the belowmouse widget,
      // set it, but only if handle() did not set it to some child:
      if (!contains(Fl::belowmouse())) Fl::belowmouse(this);
    }
    break;

  case FL_PUSH:
    if (!takesevents()) break;
    // see if it wants the event:
    ret = handle(event);
    if (ret) {
      // If it returns true then this is the pushed() widget. But we
      // only set this if handle() did not pass this on to a child,
      // and if the mouse is still down:
      // Widgets with click_to_focus flag on will get the focus if
      // they accept the mouse push.
      if (Fl::event_state(0x0f000000) && !contains(Fl::pushed())) {
	Fl::pushed(this);
	if (click_to_focus()) take_focus();
      }
    }
    break;

  case FL_SHOW:
  case FL_HIDE:
    if (visible()) handle(event);
    // we always return zero as we want this event sent to every child
    break;

  case FL_ACTIVATE:
  case FL_DEACTIVATE:
    if (takesevents()) handle(event);
    // we always return zero as we want this event sent to every child
    break;

  default:
    if (!takesevents()) break;
    ret = handle(event);
    break;
  }

  Fl::e_x = save_x; Fl::e_y = save_y;
  return ret;
}

// Very similar to send(FL_FOCUS) except it does not send it if it already
// has the focus.
bool Fl_Widget::take_focus() {
  if (focused()) return true;
  if (!takesevents() || !handle(FL_FOCUS)) return false;
  if (!contains(Fl::focus())) Fl::focus(this);
  return true;
}

void Fl_Widget::activate() {
  if (!active()) {
    clear_flag(FL_INACTIVE);
    if (active_r()) {
      redraw_label(); redraw();
      handle(FL_ACTIVATE);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void Fl_Widget::deactivate() {
  if (active_r()) {
    set_flag(FL_INACTIVE);
    redraw_label(); redraw();
    handle(FL_DEACTIVATE);
    throw_focus();
  } else {
    set_flag(FL_INACTIVE);
  }
}

bool Fl_Widget::active_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->active()) return false;
  return true;
}

void Fl_Widget::show() {
  if (!visible()) {
    clear_flag(FL_INVISIBLE);
    if (visible_r()) {
      redraw_label(); redraw();
      handle(FL_SHOW);
    }
  }
}

void Fl_Widget::hide() {
  if (visible_r()) {
    set_flag(FL_INVISIBLE);
    // we must redraw the enclosing group that has an opaque box:
    for (Fl_Widget *p = parent(); p; p = p->parent())
      if (p->box() != FL_NO_BOX || !p->parent()) {p->redraw(); break;}
    handle(FL_HIDE);
    throw_focus();
  } else {
    set_flag(FL_INVISIBLE);
  }
}

bool Fl_Widget::visible_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->visible()) return false;
  return true;
}

// return true if widget is inside (or equal to) this:
// Returns false for null widgets.
bool Fl_Widget::contains(const Fl_Widget *o) const {
  for (; o; o = o->parent_) if (o == this) return true;
  return false;
}

bool Fl_Widget::pushed() const {return this == Fl::pushed();}

bool Fl_Widget::focused() const {return this == Fl::focus();}

bool Fl_Widget::belowmouse() const {return this == Fl::belowmouse();}

////////////////////////////////////////////////////////////////

// Code to test and parse fltk shortcut numbers.
//
// A shortcut is a keysym or'd with shift flags.  In the simplest
// sense a shortcut is matched if the shift state is exactly as
// given and the key returning that keysym is pressed.
//
// To make it easier to match some things it is more complex:
//
// Only FL_WIN, FL_ALT, FL_SHIFT, and FL_CTRL must be "off".  A
// zero in the other shift flags indicates "dont care".
//
// It also checks against the first character of Fl::event_text(),
// and zero for FL_SHIFT means "don't care".
// This allows punctuation shortcuts like "#" to work (rather than
// calling it "shift+3")

// Test against an arbitrary shortcut:
bool Fl::test_shortcut(int shortcut) {
  if (!shortcut) return false;

  int shift = Fl::event_state();
  // see if any required shift flags are off:
  if ((shortcut&shift) != (shortcut&0x7fff0000)) return false;
  // record shift flags that are wrong:
  int mismatch = (shortcut^shift)&0x7fff0000;
  // these three must always be correct:
  if (mismatch&(FL_WIN|FL_ALT|FL_CTRL)) return false;

  int key = shortcut & 0xffff;

  // if shift is also correct, check for exactly equal keysyms:
  if (!(mismatch&(FL_SHIFT)) && key == Fl::event_key()) return true;

  // try matching ascii, ignore shift:
  if (key == Fl::event_text()[0]) return true;

  // kludge so that Ctrl+'_' works (as opposed to Ctrl+'^_'):
  if ((shift&FL_CTRL) && key >= 0x3f && key <= 0x5F
      && Fl::event_text()[0]==(key^0x40)) return true;
  return false;
}

// Test against shortcut() and possibly against a &x shortcut in the label:
#include <ctype.h>

int Fl_Widget::test_shortcut() const {

  if (Fl::test_shortcut(shortcut())) return true;

  if (flags() & FL_RAW_LABEL) return false;

  char c = Fl::event_text()[0];
  const char* label = this->label();
  if (!c || !label) return false;
  for (;;) {
    if (!*label) return false;
    if (*label++ == '&') {
      if (*label == '&') label++;
      else if (tolower(*label) == c) return 2; // signal for Fl_Menu code
      else return false;
    }
  }
}

////////////////////////////////////////////////////////////////
// Drawing methods (designed to be called from a draw() implementation):

// Draw only the edge of the widget. Assummes boxtype is rectangular:
void Fl_Widget::draw_frame() const {
  Fl_Flags flags = this->flags();
  if (!active_r()) flags |= FL_INACTIVE;
  if (focused()) flags |= FL_SELECTED;
  box()->draw(0, 0, w(), h(), color(), flags|FL_INVISIBLE);
}

// Fill the entire widget with it's box, handle non-rectangular boxes:
void Fl_Widget::draw_box() const {
  Fl_Boxtype box = this->box();
  if (damage()&FL_DAMAGE_EXPOSE && !box->fills_rectangle() && parent()) {
    fl_push_clip(0, 0, w(), h());
    parent()->draw_group_box();
    fl_pop_clip();
  }
  Fl_Flags flags = this->flags();
  if (!active_r()) flags |= FL_INACTIVE;
  if (focused()) flags |= FL_SELECTED;
  box->draw(0, 0, w(), h(), color(), flags);
}

// Set up for incremental redraw:
void Fl_Widget::make_current() const {
  int x = 0;
  int y = 0;
  const Fl_Widget* widget = this;
  while (!widget->is_window()) {
    x += widget->x();
    y += widget->y();
    widget = widget->parent();
  }
  ((const Fl_Window*)widget)->make_current();
  fl_x_ = x;
  fl_y_ = y;
}

////////////////////////////////////////////////////////////////

// This should eventually do all of the button->draw stuff:
void Fl_Widget::draw()
{
  draw_box();
  draw_inside_label();
}

//
// End of "$Id: Fl_Widget.cxx,v 1.91 2002/06/09 23:20:19 spitzak Exp $".
//
