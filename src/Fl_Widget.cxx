//
// "$Id: Fl_Widget.cxx,v 1.103 2003/08/25 15:28:47 spitzak Exp $"
//
// Base widget class for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Widget.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/layout.h>
#include <fltk/Group.h>
#include <fltk/run.h>
#include <string.h> // for strdup
#include <stdlib.h> // for free
#include <config.h>
using namespace fltk;

void Widget::default_callback(Widget* w, void*) {w->set_changed();}
    
Widget::Widget(int X, int Y, int W, int H, const char* L) {
  style_	= default_style;
  parent_	= 0;
  callback_	= default_callback;
  user_data_ 	= 0;
  label_	= L;
  image_	= 0;
  tooltip_	= 0;
  shortcut_	= 0;
#if CLICK_MOVES_FOCUS
  flags_	= CLICK_TO_FOCUS;
#else
  flags_	= 0;
#endif
  x_ = X; y_ = Y; w_ = W; h_ = H;
  type_		= 0;
  damage_	= DAMAGE_ALL;
  layout_damage_= LAYOUT_DAMAGE;
  when_		= WHEN_RELEASE;
  if (Group::current()) Group::current()->add(this);
}

Widget::~Widget() {
  remove_timeout();
  if (parent_) parent_->remove(this);
  throw_focus();
  if (style_->dynamic()) {
    // When a widget is destroyed it can destroy unique styles:
    delete (Style*)style_; // cast away const
  }
  if (flags_&COPIED_LABEL) free((void*)label_);
}

void Widget::label(const char* s) {
  if (label_ == s) return; // Avoid problems if label(label()) is called
  if (flags_&COPIED_LABEL) {
    free((void*)label_);
    flags_ &= ~COPIED_LABEL;
  }
  label_ = s;
}

void Widget::copy_label(const char* s) {
  if (label_ == s) return; // Avoid problems if label(label()) is called
  if (flags_&COPIED_LABEL) free((void*)label_);
  if (s) {
    label_ = strdup(s);
    flags_ |= COPIED_LABEL;
  } else {
    label_ = 0;
    flags_ &= ~COPIED_LABEL;
  }
}

////////////////////////////////////////////////////////////////
// layout damage:

void Widget::layout() {
  layout_damage_ = 0;
}

int Widget::height() {
  if (!h_) layout();
  return h_;
}

int Widget::width() {
  if (!w_) layout();
  return w_;
}

bool Widget::resize(int X, int Y, int W, int H) {
  uchar flags = 0;
  if (X != x_) flags = LAYOUT_X;
  if (Y != y_) flags |= LAYOUT_Y;
  if (W != w_) flags |= LAYOUT_W;
  if (H != h_) flags |= LAYOUT_H;
  if (flags) {
    x_ = X; y_ = Y; w_ = W; h_ = H;
    // parent must get LAYOUT_DAMAGE as well as LAYOUT_CHILD:
    if (parent()) {
      layout_damage_ |= flags;
      parent()->relayout(LAYOUT_DAMAGE|LAYOUT_CHILD);
    } else {
      relayout(flags);
    }
    return true;
  } else {
    return false;
  }
}

void Widget::relayout() {
  relayout(LAYOUT_DAMAGE);
}

void Widget::relayout(uchar flags) {
  //if (!(flags & ~layout_damage_)) return;
  layout_damage_ |= flags;
  for (Widget* w = this->parent(); w; w = w->parent())
    w->layout_damage_ |= LAYOUT_CHILD;
  fltk::damage(1); // make flush() do something
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
on DAMAGE_CHILD in all the parents. Groups should call redraw_child()
on each child when DAMAGE_CHILD is on.

Expose events accumulates a region but does not turn on any damage
bits.  The flush() method on a window must cause this area to be
updated. This may be done by copying a back buffer. The normal window
calls draw() again with the clip region set and damage() set to
DAMAGE_EXPOSE. All widgets should redraw every pixel in the clip
region if DAMAGE_EXPOSE is turned on. Exposure that covers the
entire window is detected and changed into DAMAGE_ALL, so only one
redraw is done.

The redraw(x,y,w,h) function does the same thing as expose events but
it sets DAMAGE_EXPOSE. This is so flush() for double-buffered
windows can tell these apart from normal expose events.

*/

void Widget::redraw() {
  redraw(DAMAGE_ALL);
}

void Widget::redraw(uchar flags) {
  if (!(flags & ~damage_)) return;
  damage_ |= flags;
  if (!is_window())
    for (Widget* widget = parent(); widget; widget = widget->parent()) {
      widget->damage_ |= DAMAGE_CHILD;
      if (widget->is_window()) break;
    }
  fltk::damage(1); // make flush() do something
}

void Widget::redraw_label() {
  if (!label() && !image()) return;
  // inside label redraws the widget:
  if (!(flags()&15) || (flags() & ALIGN_INSIDE)) redraw();
  // outside label requires a marker flag and damage to parent:
  else redraw(DAMAGE_CHILD_LABEL);
}

////////////////////////////////////////////////////////////////
// Events:

// Default handler. This returns 1 for mouse movement over opaque widgets,
// so they can block widgets they overlap from getting events, also so
// you can put tooltips on them.
int Widget::handle(int event) {
  switch (event) {
  case ENTER:
  case MOVE:
    // Though returning true will work for normal widgets, it will not
    // work if this is a group and some child has the belowmouse because
    // send() will not change the belowmouse then. Setting belowmouse
    // directly fixes this.
    // The check for is_window is there to fix problems with a large
    // number of older fltk programs that set NO_BOX on windows to
    // stop them from blinking (this is not necessary in fltk2.0):
    if (box()!=NO_BOX || is_window()) {fltk::belowmouse(this); return true;}
    return 0;
  case HIDE:
  case DEACTIVATE:
    throw_focus();
    return 0;
  default:
    return 0;
  }
}

// send(event) is a wrapper for handle() that should be called to send
// events. It does a few things:
//
// 1. It adjusts event_x/y to be relative to the widget.
//
// 2. It makes sure the widget is active and/or visible if the event
// requres this. It is the caller's responsibility to see if the
// mouse is pointing at the widget.
//
// 3. If handle returns true it sets the belowmouse or focus widget
// to reflect this.

int fl_pushed_dx;
int fl_pushed_dy;

int Widget::send(int event) {

  int save_x = e_x;
  e_x -= x();
  int save_y = e_y;
  e_y -= y();

  int ret = 0;
  switch (event) {

  case FOCUS:
    if (!takesevents()) break;
    ret = handle(event);
    if (ret) {
      // If it returns true then this is the focus() widget, but only
      // set this if handle() did not pass this on to a child:
      if (!contains(fltk::focus())) fltk::focus(this);
    }
    break;

  case ENTER:
  case MOVE:
    if (!visible()) break;
    // figure out correct type of event:
    event = (contains(fltk::belowmouse())) ? MOVE : ENTER;
    ret = handle(event);
    if (ret) {
      // If return value is true then this is the belowmouse widget,
      // set it, but only if handle() did not set it to some child:
      if (!contains(fltk::belowmouse())) fltk::belowmouse(this);
    }
    break;

  case DND_ENTER:
  case DND_DRAG:
    if (!takesevents()) break;
    // figure out correct type of event:
    event = (contains(fltk::belowmouse())) ? DND_DRAG : DND_ENTER;
    // see if it wants the event:
    ret = handle(event);
    if (ret) {
      // If return value is true then this is the belowmouse widget,
      // set it, but only if handle() did not set it to some child:
      if (!contains(fltk::belowmouse())) fltk::belowmouse(this);
    }
    break;

  case PUSH:
    if (!takesevents()) break;
    // see if it wants the event:
    ret = handle(event);
    if (ret) {
      // If it returns true then this is the pushed() widget. But we
      // only set this if handle() did not pass this on to a child,
      // and if the mouse is still down:
      // Widgets with click_to_focus flag on will get the focus if
      // they accept the mouse push.
      if (event_state(0x0f000000) && !contains(fltk::pushed())) {
	fltk::pushed(this);
	if (click_to_focus()) take_focus();
	// remember the mouse offset so we can send DRAG/RELEASE directly:
	fl_pushed_dx = e_x-e_x_root;
	fl_pushed_dy = e_y-e_y_root;
      }
    }
    break;

  case SHOW:
  case HIDE:
    if (visible()) handle(event);
    // we always return zero as we want this event sent to every child
    break;

  case ACTIVATE:
  case DEACTIVATE:
    if (takesevents()) handle(event);
    // we always return zero as we want this event sent to every child
    break;

  case SHORTCUT:
    if (active()) ret = handle(event);
    break;

  default:
    if (takesevents()) ret = handle(event);
    break;
  }

  e_x = save_x; e_y = save_y;
  return ret;
}

// Very similar to send(FOCUS) except it does not send it if it already
// has the focus.
bool Widget::take_focus() {
  if (focused()) return true;
  if (!takesevents() || !handle(FOCUS)) return false;
  if (!contains(fltk::focus())) fltk::focus(this);
  return true;
}

void Widget::activate() {
  if (!active()) {
    clear_flag(INACTIVE);
    if (active_r()) {
      redraw_label(); redraw();
      handle(ACTIVATE);
      if (inside(focus())) focus()->take_focus();
    }
  }
}

void Widget::deactivate() {
  if (active_r()) {
    set_flag(INACTIVE);
    redraw_label(); redraw();
    handle(DEACTIVATE);
  } else {
    set_flag(INACTIVE);
  }
}

bool Widget::active_r() const {
  for (const Widget* o = this; o; o = o->parent())
    if (!o->active()) return false;
  return true;
}

void Widget::show() {
  if (!visible()) {
    clear_flag(INVISIBLE);
    if (visible_r()) {
      redraw_label(); redraw();
      handle(SHOW);
    }
  }
}

void Widget::hide() {
  if (visible_r()) {
    set_flag(INVISIBLE);
    // we must redraw the enclosing group that has an opaque box:
    for (Widget *p = parent(); p; p = p->parent())
      if (p->box() != NO_BOX || !p->parent()) {p->redraw(); break;}
    handle(HIDE);
  } else {
    set_flag(INVISIBLE);
  }
}

bool Widget::visible_r() const {
  for (const Widget* o = this; o; o = o->parent())
    if (!o->visible()) return false;
  return true;
}

// return true if widget is inside (or equal to) this:
// Returns false for null widgets.
bool Widget::contains(const Widget *o) const {
  for (; o; o = o->parent_) if (o == this) return true;
  return false;
}

bool Widget::pushed() const {return this == fltk::pushed();}

bool Widget::focused() const {return this == fltk::focus();}

bool Widget::belowmouse() const {return this == fltk::belowmouse();}

static void widget_timeout(void* data) {
  ((Widget*)data)->handle(TIMEOUT);
}

void Widget::add_timeout(float time) {
  fltk::add_timeout(time, widget_timeout, this);
}
void Widget::repeat_timeout(float time) {
  fltk::repeat_timeout(time, widget_timeout, this);
}
void Widget::remove_timeout() {
  fltk::remove_timeout(widget_timeout, this);
}

////////////////////////////////////////////////////////////////

#include <ctype.h>

/** Test to see if the current KEY or SHORTCUT event matches a
    shortcut() value.

  A shortcut is a key number (as returned by event_key()) or'd
  with shift flags (as returned by event_state()). Basically
  a shortcut is matched if the shift state is exactly as
  given and the given key is pressed.

  To make it easier to match some things it is more complex:

  Only COMMAND, ALT, SHIFT, and CTRL must be "off".  A zero in the
  other shift flags indicates "dont care". For instance we normally
  don't care is SCROLL_LOCK or BUTTON1 is on, but you can require them
  to be on by adding them to the shortcut.
  
  The letter keys are officially named by their lower-case names,
  however there is a tendency to put the upper-case name into the
  shortcut. So if the shortcut is ALT+'A' it is treated as ALT+'a'.
  If you want the user to purposly type an upper-case 'A' you \e must
  specify SHIFT+'A' (this was not true in fltk1 but too many people
  put in upper-case even when they explicitly did \e not want shift
  held down...)

  For non-letters it ignores SHIFT being held down unnecessarily and
  matches against event_text()[0]. This will allow a shortcut like '#'
  or SHIFT+'#' to work (rather than requiring SHIFT+'3' which is what
  would be consistent with the letters). Also this allows '3' to match
  Keypad3.

*/
bool fltk::test_shortcut(int shortcut) {
  if (!shortcut) return false;

  int shift = event_state();
  // see if any required shift flags are off:
  if ((shortcut&shift) != (shortcut&0x7fff0000)) return false;
  // record shift flags that are wrong:
  int mismatch = (shortcut^shift)&0x7fff0000;
  // these three must always be correct:
  if (mismatch&(COMMAND|ALT|CTRL)) return false;

  int key = shortcut & 0xffff;
  if (key < 0x7f) key = tolower(key);

  // if shift is also correct, check for exactly equal keysyms:
  if (!(mismatch&(SHIFT)) && key == event_key()) return true;

  // try matching typed punctuation marks and numbers no matter what
  // key produces them:
  if (key < 0x7f && !isalpha(key)) {
    if (key == event_text()[0]) return true;
    // If Ctrl is held down it can change event_text, change it back:
    if ((shift&CTRL) && key >= 0x3f && key <= 0x5F
      && event_text()[0]==(key^0x40)) return true;
  }

  return false;
}

/** Test to see if the current KEY or SHORTCUT event matches a shortcut
    specified with &x in the label.

    This will match if the character after the first '&' matches the
    event_text()[0].  Case is ignored. The caller may want to
    check if ALT or some other shift key is held down before calling
    this so that plain keys do not do anything.

    This is ignored if flags() has RAW_LABEL turned on (which stops
    the &x from printing as an underscore. The sequence "&&" is ignored
    as well because that is used to print a plain '&' in the label.
*/
bool Widget::test_label_shortcut() const {

  if (flags() & RAW_LABEL) return false;

  char c = tolower(event_text()[0]);
  const char* label = this->label();
  if (!c || !label) return false;
  for (;;) {
    if (!*label) return false;
    if (*label++ == '&') {
      if (*label == '&') label++;
      else return (tolower(*label) == c);
    }
  }
}

/** Same as fltk::test_shortcut(shortcut()) || test_label_shortcut().

    This can be used by simple button type widgets to trigger a callback
    on a shortcut for either complex shortcut() values or for &x in
    the label.
*/
bool Widget::test_shortcut() const {
  return fltk::test_shortcut(shortcut()) || test_label_shortcut();
}

////////////////////////////////////////////////////////////////

extern Widget* fl_did_clipping;

// This should eventually do all of the button->draw stuff:
void Widget::draw()
{
  if (box() == NO_BOX) {
    // check for completely blank widgets. We must not clip to their
    // area because it will break lots of programs that assumme these
    // can overlap any other widgets:
    if (!label() && !image() ||
	align() != ALIGN_CENTER && !(align()&ALIGN_INSIDE)) {
      fl_did_clipping = this;
      return;
    }
    // draw the background behind the invisible widget:
    draw_background();
  } else {
    draw_box();
  }
  draw_inside_label();
}

//
// End of "$Id: Fl_Widget.cxx,v 1.103 2003/08/25 15:28:47 spitzak Exp $".
//
