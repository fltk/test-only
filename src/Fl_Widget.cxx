//
// "$Id: Fl_Widget.cxx,v 1.110 2004/01/21 09:18:10 spitzak Exp $"
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

/*! \class fltk::Widget

  The base class for all widgets in FLTK. The basic Widget draws an
  empty box() and the label(), and ignores all events. This can be
  useful for making decorations or providing areas that pop up a
  tooltip().
*/

/*! This turns on the changed() flag in the widget. callback() is
  initialized to this.
*/
void Widget::default_callback(Widget* w, void*) {w->set_changed();}

/*! Standard constructor for a widget.
  The default constructor takes a value for x(), y(), w(), and h(),
  and an optional value for label(). All subclasses must provide an
  identical constructor in order to work with Fluid. They may also
  provide alternative constructors.

  If Group::begin() has been called, this widget is added as a new
  child of that group, and parent() is set to the group. If
  Group::begin() has not been called, or Group::end() has been called,
  or Group::current(0), then the parent() is set to null. In this case
  you must add the widget yourself in order to see it.
*/
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

/*! The destructor is virtual. The base class removes itself from the
  parent widget (if any), and destroys any label made with copy_label().
*/
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

/*! \fn Group* Widget::parent() const
  Returns a pointer to the parent widget.  Usually this is a
  fltk::Group or fltk::Window.  Returns NULL if none.
*/

/*! Returns true if \a b is a child of this widget, or is equal to
  this widget. Returns false if \a b is NULL. */
bool Widget::contains(const Widget* b) const {
  for (; b; b = b->parent_) if (b == this) return true;
  return false;
}

/*! bool Widget::inside(const Widget* a) const
  Returns true if this is a child of a, or is equal to a. Returns
  false if a is NULL. */

/*! Sets the label directly to a string.
  The label is printed somewhere on the widget or next to it. The
  string passed to label() is \e not copied, instead the pointer to
  the string is stored. If copy_label()
  was called earlier the old string's memory is freed.
*/
void Widget::label(const char* s) {
  if (label_ == s) return; // Avoid problems if label(label()) is called
  if (flags_&COPIED_LABEL) {
    free((void*)label_);
    flags_ &= ~COPIED_LABEL;
  }
  label_ = s;
}

/*! Sets the label to a copy of the string.
  The passed string is copied to private storage and used to set the
  label(). The memory will be freed when the widget is destroyed or when
  copy_label() is called again, or label(const char*) is called.

  Passing NULL will set label() to NULL.
*/
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

/*! void Widget::image(Image*)
  Sets the image. The image is drawn as part of the label, usually to
  the left of the text. This is designed for icons on menu items. If
  you want to replace the entire background of the widget with a picture
  you should set box() instead. Notice that you can also get images into
  labels by putting '@' commands into the label().
*/

/*! void Widget::tooltip(const char*)
  Set the string used as the pop-up tooltip. The pointer to the passed string
  is stored, it is not copied! Passing null indicates that the tooltip
  of the parent() should be used (or no
  tooltip if no parent has one). If you want to disable the tooltip but
  let the parent have one, set this tooltip to <tt>""</tt>.

  Putting '@' commands in to bring up Symbol objects will allow a lot
  of interesting things to be put into the tooltip.
*/

/*! void Widget::shortcut(ulong key)

  Set the shortcut() value. 
  Buttons and menu items use the shortcut to identify a keystroke
  that will activate them. The value is a bitwise OR of a key and a set
  of shift flags, for example <CODE>fltk::ALT | 'a'</CODE> , <CODE>fltk::ALT |
  (fltk::F1Key)</CODE>, or just <CODE>'a'</CODE>.  A value of 0 disables
  the shortcut.

  The key can be any value returned by fltk::event_key(), but will
  usually be an ASCII letter. Use a lower-case letter unless you
  require the shift key to be held down.

  The shift flags can be any set of values accepted by
  fltk::event_state(). If the bit is on that shift key must be pushed.
  META, ALT, CTRL, and SHIFT must be off if they are not in the shift
  flags (zero for the other bits indicates a "don't care" setting).

  Shortcuts can also be done in the MS Windows way
  by putting an '&amp;' in front of a letter in the label(). This is
  equivalent to <tt>fltk::ALT</tt> and the letter.
*/

/*! \fn void Widget::callback(fltk::Callback*, void*)
  Each widget has a single callback.  You can set it or examine it with 
  these methods. The callback is called with the widget as the first
  argument and the void* as the second argument. It is called in response
  to user events, but exactly whe depends on the widget. For instance a
  button calls it when the button is released.
*/

/*! \fn void Widget::user_data(void*)
  Set the second argument to the callback.
*/

/*! \fn void Widget::callback(fltk::Callback1*, long = 0)
  For convenience you can also define the callback as taking a long
  integer argument.  This is implemented by casting the function to a
  fltk::Callback and casting the <tt>long</tt> to a <tt>void*</tt> and
  may not be portable to some machines.
*/

/*! \fn void Widget::argument(long)
  Sets the second argument to the callback to a number. This is done
  by casting the long to a void*.
*/

/*! \fn void Widget::callback(fltk::Callback0*)
  For convenience you can also define the callback as taking only the
  Widget as an argument.  This is implemented by casting this to a
  fltk::Callback and may not be portable to some machines.
*/

/*! \fn void Widget::do_callback()
  You can cause a widget to do its callback at any time. The callback
  function is called with this and user_data() as arguments.
*/

/*! \fn void Widget::do_callback(Widget*, void*)
  You can also call the callback function with arbitrary arguments. */

/*! \fn void Widget::do_callback(Widget*, long)
  You can also call the callback function with arbitrary arguments. */

/*! \fn int Widget::x() const
  Left edge of the widget, relative to the x() of its enclosing 
  Widget. If this is an outer fltk::Window than this is the position of
  the left edge of the \e contents (not the frame) on the screen.
*/

/*! \fn int Widget::y() const
  Top edge of the widget, relative to the y() of its enclosing 
  Widget. If this is an outer fltk::Window than this is the position of
  the top edge of the \e contents (not the frame) on the screen.
*/

/*! \fn int Widget::w() const
  Width of the widget in pixels.
*/

/*! \fn int Widget::h() const
  Height of the widget in pixels.
*/

/*! \fn uchar Widget::type() const

  8-bit identifier that controls how widget works.
  This value had to be provided for Forms compatibility, but
  you can use it for any purpose you want (mostly for "bad object
  oriented programming" where you insert some subclass functionality
  into the base class). Widget subclasses may store values in the
  range 0-99 here (larger values are reserved for use by FLTK).

  The fltk::PackedGroup widget uses the low bit of the type() of
  each child to indicate HORIZONTAL (1) or VERTICAL (0).

  For portability FLTK does not use RTTI (Run Time Typing Infomation)
  internally (you are free to use it, though). If you don't have RTTI you can
  use the clumsy FLTK mechanisim, by having type() use a unique
  value.  These unique values must be greater than the symbol
  Widget::RESERVED_TYPE (which is 100).  Look through the header
  files for Widget::RESERVED_TYPE to find an unused number.  If you
  make a subclass of fltk::Window you must use Widget::WINDOW_TYPE+n
  (\e n must be in the range 1 to 7) so that is_window() will work, if you
  make a subclass of fltk::Group you must use Widget::GROUP_TYPE+n
  (\e n must be in the range 1 to 7) so that is_group() will work.  */

/*! \fn bool Widget::is_group() const
  Returns true for subclasses of fltk::Group. If so you can cast it to
  a group with <tt>(fltk::Group*)(widget)</tt>. This is done by using
  type(), but if your compiler supports RTTI you may want to safer
  <tt>dynamic_cast&lt;fltk::Group*&gt;(widget)</tt>.
*/

/*! \fn bool Widget::is_window() const
  Returns true for subclasses of fltk::Window. If so you can cast it
  to a window with <tt>(fltk::Window*)(widget)</tt>. This is done by
  using type(), but if your compiler supports RTTI you may want to safer
  <tt>dynamic_cast&lt;fltk::Window*&gt;(widget)</tt>. If this is true,
  is_group() is also true.
*/

/*! \fn Flags Widget::flags() const

  Each Widget, and most drawing functions, take a bitmask of
  flags that indicate the current state and exactly how to draw
  things. See \ref flags for values.
*/

/*! void Widget::align(Flags);

  Forces the values of all the fltk::ALIGN_* flags to the passed
  value. This determines how the label is printed next to or inside
  the widget. The default value is fltk::ALIGN_CENTER, which centers
  the label. The value can be any of the ALIGN \ref flags or'd together.
*/

/*! \fn void Widget::when(uchar)

  Flags indicating when to do the callback(). This field is in the
  base class so that you can scan a panel and do_callback() on all the
  ones that don't do their own callbacks in response to an "OK"
  button.

  The following constants can be used, their exact meaning depends on
  the widget's implementation:
  - fltk::WHEN_NEVER - Never call the callback (0). 
  - fltk::WHEN_CHANGED - Do the callback each time the widget's value
  is changed by the user (many callbacks may be done as the user drags
  the mouse)
  - fltk::WHEN_RELEASE - Each keystroke that modifies the value, or
  when the mouse is released and the value has changed, causes the
  callback (some widgets do not implement this and act like
  fltk::WHEN_CHANGED)
  - fltk::WHEN_RELEASE_ALWAYS - Each recognized keystroke and the
  mouse being released will cause the callback, even if the value did
  not change. (some widgets do not implement this and act like
  fltk::WHEN_RELEASE)
  - fltk::WHEN_ENTER_KEY - Do the callback when the user presses the
  ENTER key and the value has chagned (used by fltk::Input and
  fltk::Browser.
  - fltk::WHEN_ENTER_KEY_ALWAYS - Do the callback when the user
  presses the ENTER key, even if the value has not changed.
  -fltk::WHEN_ENTER_KEY_CHANGED - Do the callback when the user
  presses the ENTER key and each time the value changes.

*/

////////////////////////////////////////////////////////////////

/*! \defgroup layout

  When a widget resized or moved (or when it is initially created),
  flags are set in it to indicate the layout is damaged. This will
  cause the virtual functionlayout() to be called later on, just
  before fltk attempts to draw the windows on the screen.

  This is useful because often calculating the new layout is quite
  expensive, this expense is thus deferred until the user will
  actually see the new size.

  Some Group widgets such as fltk::PackedGroup will also use the
  virtual layout() function to find out how big a widget should be.
  Widgets are allowed to change their own dimensions in layout().  The
  only rule is that if nothing except the x/y position is changed and
  layout() is called a second time, they must not change size again.
  Usually a widget will alter only one of w() or h(), and usually it
  will only make itself larger, not smaller. Most widgets only set
  w() or h() if one or both of them are zero.

  \{
*/

/*! Virtual function to respond to layout_damage(), it should
  calculate the correct size of this widget and all it's children.
  This function is called by fltk or by the layout() method in other
  widgets. User programs should not call it.

  A widget is allowed to alter it's own size in a layout() method, to
  indicate a size that the data will fit in. A parent widget is then
  expected to rearrange itself to accomodate the new size. This may
  mean it will move the widget and thus layout() will be called again.

  You can look at layout_damage() to find out why this is being called.

  If you subclass this, you must call the base class version! The base
  class version sets layout_damage() to zero.
*/
void Widget::layout() {
  layout_damage_ = 0;
}

/*! Returns h() but if the current value is zero it calls layout()
  before returning the value.  Using these calls allows a widget to
  delay the calculation of size until it is needed. */
int Widget::height() {
  if (!h_) layout();
  return h_;
}

/*! Returns w() but if the current value is zero it calls layout()
  before returning the value.  Using these calls allows a widget to
  delay the calculation of size until it is needed. */
int Widget::width() {
  if (!w_) layout();
  return w_;
}

/*! Change the size or position of the widget. Nothing is done if the
  passed size and position are the same as before. If there is a
  change then relayout() is called so that the virtual function
  layout() is called before the next draw().  */
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

/*! \fn void Widget::position(int x, int y)
  Same as resize(x,y,w(),h())
*/

/*! \fn void Widget::size(int w, int h)
  Same as resize(x(),y(),w,h)
*/

/*! Same as relayout(LAYOUT_DAMAGE), indicates that data inside the
  widget may have changed, but the size did not change. This flag is
  also on when the widget is initially created.
*/
void Widget::relayout() {
  relayout(LAYOUT_DAMAGE);
}

/* Cause layout() to be called later. Turns on the specified flags in
   layout_damage(), and turns on LAYOUT_CHILD in all parents of this
   widget. \a flags cannot be zero, the maaning of the flags is listed
   under \ref layout.
*/
void Widget::relayout(uchar flags) {
  //if (!(flags & ~layout_damage_)) return;
  layout_damage_ |= flags;
  for (Widget* w = this->parent(); w; w = w->parent())
    w->layout_damage_ |= LAYOUT_CHILD;
  fltk::damage(1); // make flush() do something
}

/*! \fn uchar Widget::layout_damage() const

  The 'or' of all the calls to relayout() or resize() done since the
  last time layout() was called.
  For more information see the \ref layout chapter.

  A typical layout function does not care about the widget moving, an
  easy way to skip it is as follows:

\code
MyClass::layout() {
  if (!(layout_damage() & ~LAYOUT_XY)) return;
  do_expensive_layout();
  redraw();
}
\endcode
*/

/*! \fn void Widget::layout_damage(uchar c)
  Directly change the value returned by layout_damage(). */

/* \} */ // end of layout section of documentation

////////////////////////////////////////////////////////////////

/*! \defgroup damage

  When redrawing your widgets you should look at the damage bits to
  see what parts of your widget need redrawing. The handle() method
  can then set individual damage bits to limit the amount of drawing
  that needs to be done:

  \code
MyClass::handle(int event) {
  ...
  if (change_to_part1) damage(1);
  if (change_to_part2) damage(2);
  if (change_to_part3) damage(4);
}

MyClass::draw() {
  if (damage() & fltk::DAMAGE_ALL) {
    ... draw frame/box and other static stuff ...
  }
  if (damage() & (fltk::DAMAGE_ALL | 1)) draw_part1();
  if (damage() & (fltk::DAMAGE_ALL | 2)) draw_part2();
  if (damage() & (fltk::DAMAGE_ALL | 4)) draw_part3();
}
  \endcode

  Except for DAMAGE_ALL, each widget is allowed to assign any meaning
  to any of the bits it wants. The enumerations are just to provide
  suggested meanings.

  \{
*/

/*! \var fltk::DAMAGE_CHILD
  A child of this group widget needs to be
  redrawn (non-group widgets can use this bit for their own purposes).
*/
/*! \var fltk::DAMAGE_CHILD_LABEL
  An outside label of this widget needs
  to be redrawn. This is handled (and this bit is cleared) by the
  parent group.
*/
/*! \var fltk::DAMAGE_EXPOSE
  Damage caused by damage() or by expose
  events from the operating system. If this and fltk::DAMAGE_ALL is on
  the widget should draw every pixel inside it's region.
*/
/*! \var fltk::DAMAGE_ALL
  This bit is set by redraw() and indicates that
  all of the widget (but not "holes" where the background shows
  through) needs to be redraw.
*/
/*! \var fltk::DAMAGE_VALUE
  A widget may use this to indicate that the displayed value has changed. */
/*! \var fltk::DAMAGE_PUSHED
  A widget may use this to indicate that the user has pushed or released
  a button. */
/*! \var fltk::DAMAGE_SCROLL
  A widget may use this to indicate that
  the displayed data has scrolled moved horizontally and/or vertically. */
/*! \var fltk::DAMAGE_OVERLAY
  Same value as fltk::DAMAGE_SCROLL. */
/*! \var fltk::DAMAGE_HIGHLIGHT
  A widget may use this to indicate that
  the mouse has entered/exited part of the widget. */
/*! \var fltk::DAMAGE_CONTENTS
  Same as fltk::DAMAGE_EXPOSE but if fltk::DAMAGE_ALL is off a widget can
  use this for it's own purposes. */

/*! \fn uchar Widget::damage() const

  The 'or' of all the calls to redraw() done since the last
  draw(). Cleared to zero after draw() is called.
*/

/*! \fn void Widget::set_damage(uchar c)

  Directly change the value returned by damage(). Note that this
  \e replaces the value, it does not turn bits on. Use redraw()
  to turn bits on.
*/

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

/*! Same as redraw(DAMAGE_ALL). This bit is used by most widgets to
  indicate that they should not attempt any incremental update, and
  should instead completely draw themselves.
*/
void Widget::redraw() {
  redraw(DAMAGE_ALL);
}

/*! Indicates that draw() should be called, and turns on the given
  bits in damage(). At least these bits, and possibly others, will
  still be on when draw() is called. */
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

/*! Indicates that the label() should be redrawn. This does nothing
  if there is no label. If it is an outside label (see align()) then
  the parent() is told to redraw it. Otherwise redraw() is called.
*/
void Widget::redraw_label() {
  if (!label() && !image()) return;
  // inside label redraws the widget:
  if (!(flags()&15) || (flags() & ALIGN_INSIDE)) redraw();
  // outside label requires a marker flag and damage to parent:
  else redraw(DAMAGE_CHILD_LABEL);
}

/** Causes a redraw if highlighting changes.

    Calls redraw(DAMAGE_HIGHLIGHT) if this widget is active
    and has a non-zero highlight_color(). This is designed to be
    called in response to ENTER and EXIT events.
*/
void Widget::redraw_highlight() {
  if (takesevents() && active_r() && highlight_color())
    redraw(DAMAGE_HIGHLIGHT);
}

// If a draw() method sets this then the calling group assummes it
// modified the current clip region to clip out the area covered by
// the widget.
extern Widget* fl_did_clipping;

/*!
  Fltk calls this virtual function to draw the widget, after setting
  up the graphics (current window, xy translation, etc) so that any
  drawing functions will go into this widget.

  User code should not call this! You probably want to call redraw().

  The default version calls draw_box() and draw_label(), thus drawing
  the box() to fill the widget and putting the label() and image()
  inside it to fill it, unless the align() flags are set to put it
  outside.

  Information on how to write your own version is <a
  href=subclassing.html#draw>here</a>.
*/
void Widget::draw()
{
  if (box() == NO_BOX) {
    // check for completely blank widgets. We must not clip to their
    // area because it will break lots of programs that assumme these
    // can overlap any other widgets:
    if (!image() && (!label() ||
		     align() != ALIGN_CENTER && !(align()&ALIGN_INSIDE))) {
      fl_did_clipping = this;
      return;
    }
    // draw the background behind the invisible widget:
    draw_background();
  } else {
    draw_box();
  }
  draw_label();
}

/* \} */ // end of damage section of documentation

////////////////////////////////////////////////////////////////

/*! \addtogroup events
  \{
*/

/*! Handle an \ref events(event). Returns non-zero if the
  widget understood and used the event.

  The default version returns true for
  fltk::ENTER, fltk::LEAVE, and fltk::MOVE over any opaque widgets
  (ones where box() is not NO_BOX). This is done so you can put tooltips
  on the base widget.

  Information on how to write your own version of handle() is can
  be found under \ref events.

  If you want to send an event to a widget you do not want to call
  this, you probably want to call send() which will do some extra
  processing before and after the event is handled.
*/
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

int fl_pushed_dx;
int fl_pushed_dy;

/*! Wrapper for handle(). This should be called to send
  events. It does a few things:
  - It adjusts event_x/y to be relative to the widget
    (It is the caller's responsibility to see if the
    mouse is pointing at the widget).
  - It makes sure the widget is active and/or visible if the event
    requres this.
  - If this is not the fltk::belowmouse() widget then it changes
    fltk::MOVE into fltk::ENTER and turns fltk::DND_DRAG into
    fltk::DND_ENTER. If this \e is the fltk::belowmouse() widget then
    the opposite conversion is done.
  - For move, focus, and push events if handle() returns true it sets
    the fltk::belowmouse() or fltk::focus() or fltk::pushed() widget to
    reflect this.
*/
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

/*! Tries to make this widget be the keyboard focus widget, by first
  sending it an fltk::FOCUS event, and if it returns non-zero, setting
  fltk::focus() to this widget. You should use this method to assign
  the focus to a widget. Returns true if the widget accepted the
  focus.  */
bool Widget::take_focus() {
  if (focused()) return true;
  if (!takesevents() || !handle(FOCUS)) return false;
  if (!contains(fltk::focus())) fltk::focus(this);
  return true;
}

static void widget_timeout(void* data) {
  ((Widget*)data)->handle(TIMEOUT);
}

/*! Call handle(TIMEOUT) at the given time in the future. This will
  happen exactly once. To make it happen repeatedly, call repeat_timeout()
  from inside handle(TIMEOUT).
*/
void Widget::add_timeout(float time) {
  fltk::add_timeout(time, widget_timeout, this);
}
/*! Call handle(TIMEOUT) at the given time interval since the last timeout.
  This will produce much more accurate time intervals than add_timeout().
*/
void Widget::repeat_timeout(float time) {
  fltk::repeat_timeout(time, widget_timeout, this);
}
/*! Cancel any and all pending handle(TIMEOUT) callbacks. */
void Widget::remove_timeout() {
  fltk::remove_timeout(widget_timeout, this);
}

////////////////////////////////////////////////////////////////

#include <ctype.h>

/*! Test to see if the current KEY or SHORTCUT event matches a
  shortcut() value.
  A shortcut is a key number such as fltk::SpaceKey or'd with shift
  flags such as fltk::SHIFT.

  This returns true if event_state() and event_key() match this
  value exactly.

  This will also return true in some other cases designed to make
  it easier to match what the user expects for shortcuts:
  - Only META, ALT, SHIFT, and CTRL must be "off".  A zero in the
    other shift flags indicates "dont care". For instance we normally
    don't care is SCROLL_LOCK or BUTTON1 is on, but you can require
    them to be on by adding them to the shortcut. But you cannot
    require them to be off (you can test that before calling this,
    though).
  - If the key is an uppercase letter like 'A', it matches the letter
    key whether or not SHIFT is held down. This is due to the common
    standard of displaying uppercase letters in menu shortcuts. You
    can explicitly require the SHIFT key to be held down by using
    SHIFT+'A' or SHIFT+'a'.
  - A lowercase letter means that SHIFT <i>must not be held down</i>.
    This allows you to put both uppercase and lowercase shortcuts into
    the same menu.
  - If the key is an ASCII character it will ignore SHIFT and match
    against fltk::event_text()[0]. This will allow you to put '#' or
    SHIFT+'#' into the shortcut, rather than the SHIFT+'3' that would
    normally be required. It also means that '3' will match both the
    main keyboard and the keypad 3.
  - The character '\\r' will match either Enter key on the keyboard,
    this is the only way to match both of them. This does not match
    the user typing CTRL+M.
  - Control letters are matched by using CTRL+'A'. '\\1' will not
    match it, because the CTRL key is held down. CTRL+'\\1' would
    work, but there is no reason to do that.
  - CTRL+'[' matches the user holding down CTRL and hitting '[', it
    <i>does not</i> match the Esc key. There is no way to match both
    methods of typing ^[ with one shortcut.

*/
bool fltk::test_shortcut(int shortcut) {
  if (!shortcut) return false;

  int shift = event_state();
  // see if any required shift flags are off:
  if ((shortcut&shift) != (shortcut&0x7fff0000)) return false;
  // record shift flags that are wrong:
  int mismatch = (shortcut^shift)&0x7fff0000;
  // these three must always be correct:
  if (mismatch&(META|ALT|CTRL)) return false;

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

/*! Test to see if the current KEY or SHORTCUT event matches a shortcut
    specified with &x in the label.

    This will match if the character after the first '&' matches the
    event_text()[0]. Case is ignored. The caller may want to
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

/*! Same as fltk::test_shortcut(shortcut()) || test_label_shortcut().

    This can be used by simple button type widgets to trigger a callback
    on a shortcut for either complex shortcut() values or for &x in
    the label.
*/
bool Widget::test_shortcut() const {
  return fltk::test_shortcut(shortcut()) || test_label_shortcut();
}

/*! \} */ // end of events chapter of documentation

////////////////////////////////////////////////////////////////

/*! \fn bool Widget::active() const
  Returns the active state of this widget. The widget is only really
  active if all parents are active, use active_r() to test this.
*/

/*! Returns whether the widget is active. THis is true if active() is
  true for this and all parent widgets. An inactive widget does not
  get any events, but it does get redrawn. */
bool Widget::active_r() const {
  for (const Widget* o = this; o; o = o->parent())
    if (!o->active()) return false;
  return true;
}

/*! If active() is false, this turns it on. If active_r() is now true
  send() an fltk::ACTIVATE event. */
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

/*! If active() is true, this turns it off. If active_r() was true
  send() an fltk::DEACTIVATE event. */
void Widget::deactivate() {
  if (active_r()) {
    set_flag(INACTIVE);
    redraw_label(); redraw();
    handle(DEACTIVATE);
  } else {
    set_flag(INACTIVE);
  }
}

/*! \fn bool Widget::output() const
  This flag is similar to !active() except it does not change how the
  widget is drawn. The widget will not recieve any events. This is
  useful for making scrollbars or buttons that work as displays rather
  than input devices.

  Set or clear this flag with set_output() and clear_output().
*/

/*! bool Widget::visible() const
  Returns true if the widget is visible (flags() & INVISIBLE is false)
*/

/*! Returns true if the widget and all of its parents are visible. Only
  if this is true can the user see the widget. */
bool Widget::visible_r() const {
  for (const Widget* o = this; o; o = o->parent())
    if (!o->visible()) return false;
  return true;
}

/*! If visible() is false, turn it on. If visible_r() is then true, send()
  a fltk::SHOW event. */
void Widget::show() {
  if (!visible()) {
    clear_flag(INVISIBLE);
    if (visible_r()) {
      redraw_label(); redraw();
      handle(SHOW);
    }
  }
}

/*! If visible() is true, turn it off. If visible_r() was true then
  send() a fltk::HIDE event, and redraw() the parent if necessary. */
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

/*! bool Widget::takesevents() const
  This is the same as (active() && visible() && !output()) but faster.
  send() uses this to decide whether or not to call handle() for most
  events.
*/

/*! bool Widget::click_to_focus() const
  If this flag is set then if this widget returns true for an
  fltk::PUSH event then fltk will attempt to give it the focus (by
  calling take_focus(), so it will work if this widget also returns
  true for fltk::FOCUS events). By default fltk only turns this on on
  certain widgets such as fltk::Input. Turning this on on all widgets
  will make the user interface match Windows more closely.
*/

/*! bool Widget::changed() const

  The default callback() turns this flag on. This can be used to find
  what widgets have had their value changed by the user, for instance
  in response to an "OK" button.

  Most widgets turn this flag off when they do the callback, and when
  the program sets the stored value.
*/

/*! bool Widget::value() const
  A true/false flag used by fltk::Button to indicate the current state
  and by "parent" items in a hierarchial fltk::Browser to indicate if
  they are open. Many widgets will draw pushed-in or otherwise
  indicate that this flag is on.
*/

/*! bool Widget::selected() const
  A true/false flag used to mark widgets currently selected in
  fltk::Menu and fltk::Browser widgets. Some widgets will draw with
  much different colors if this is on.  */

/*!  Returns true if this is equal to fltk::pushed(), meaning it has
  responded to an fltk::PUSH event and the mouse is still held
  down. Using this function avoids the need to include the <fltk/Fl.h>
  header file. */
bool Widget::pushed() const {return this == fltk::pushed();}

/*!  Returns true if this is equal to fltk::focus(), meaning it has
  the keyboard focus and fltk::KEY events will be sent to this
  widget. Using this function avoids the need to include the
  <fltk/Fl.h> header file. */
bool Widget::focused() const {return this == fltk::focus();}

/*!  Returns true if this is equal to fltk::belowmouse(), meaning it
  has the keyboard focus and fltk::MOVE or fltk::PUSH events will be
  sent to this widget. Using this function avoids the need to include
  the <fltk/Fl.h> header file. */
bool Widget::belowmouse() const {return this == fltk::belowmouse();}

//
// End of "$Id: Fl_Widget.cxx,v 1.110 2004/01/21 09:18:10 spitzak Exp $".
//
