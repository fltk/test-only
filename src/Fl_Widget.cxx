//
// "$Id: Fl_Widget.cxx,v 1.67 2000/08/20 04:31:38 spitzak Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include <string.h> // for strdup
#include <stdlib.h> // free

////////////////////////////////////////////////////////////////
// Duplicate the Forms queue for all callbacks from widgets.  The
// widget is added to the queue and readqueue returns it:

#define QUEUE_SIZE 20

static Fl_Widget *obj_queue[QUEUE_SIZE];
static int obj_head, obj_tail;

void Fl_Widget::default_callback(Fl_Widget* o, void*) {
#if 0
  // This is necessary for strict forms compatability but is confusing.
  // Use the parent's callback if this widget does not have one.
  for (Fl_Widget *p = o->parent(); p; p = p->parent())
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

Fl_Widget *Fl::readqueue() {
  if (obj_tail==obj_head) return 0;
  Fl_Widget *o = obj_queue[obj_tail++];
  if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  return o;
}
    
////////////////////////////////////////////////////////////////

Fl_Widget::Fl_Widget(int X, int Y, int W, int H, const char* L) {
  style_	= default_style;
  parent_	= 0;
  callback_	= default_callback;
  user_data_ 	= 0;
  label_	= L;
  image_	= 0;
  tooltip_	= 0;
  shortcut_	= 0;
  flags_	= 0;
  x_ = X; y_ = Y; w_ = W; h_ = H;
  type_		= 0;
  damage_	= FL_DAMAGE_LAYOUT|FL_DAMAGE_ALL;
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
// Damage:

void Fl_Widget::layout() {
  damage_ &= ~FL_DAMAGE_LAYOUT;
}

int Fl_Widget::height() {
  if (!h_) layout();
  return h_;
}

int Fl_Widget::width() {
  if (!w_) layout();
  return w_;
}

int Fl_Widget::resize(int X, int Y, int W, int H) {
  if (x_ == X && y_ == Y && w_ == W && h_ == H) return 0;
  x_ = X; y_ = Y; w_ = W; h_ = H;
  relayout();
  return 1;
}

void Fl_Widget::relayout() {
  damage_ |= FL_DAMAGE_LAYOUT;
  for (Fl_Widget* w = this->parent(); w; w = w->parent())
    w->damage_ |= FL_DAMAGE_LAYOUT;
  Fl::damage(FL_DAMAGE_LAYOUT);
}

void Fl_Widget::damage_label() {
  if (!label()) return;
  // ignore inside label:
  if (!(flags()&15) || (flags() & FL_ALIGN_INSIDE)) return;
  // outside label requires a marker flag and damage to parent:
  damage_ |= FL_DAMAGE_CHILD_LABEL;
  if (parent()) parent()->damage(FL_DAMAGE_CHILD);
}

void Fl_Widget::redraw() {
  damage(FL_DAMAGE_ALL);
}

void Fl_Widget::damage(uchar flags) {
  if (is_window()) {
    // damage entire window by deleting the region:
    Fl_X* i = Fl_X::i((Fl_Window*)this);
    if (!i) return; // window not mapped, so ignore it
    if (i->region) {XDestroyRegion(i->region); i->region = 0;}
    damage_ |= flags;
    Fl::damage(FL_DAMAGE_CHILD);
  } else {
    // damage only the rectangle covered by a child widget:
    damage(flags, x(), y(), w(), h());
  }
}

void Fl_Widget::damage(uchar flags, int X, int Y, int W, int H) {
  Fl_Widget* window = this;
  // mark all parent widgets between this and window with FL_DAMAGE_CHILD:
  while (!window->is_window()) {
    window->damage_ |= flags;
    window = window->parent();
    if (!window) return;
    flags = FL_DAMAGE_CHILD;
  }
  Fl_X* i = Fl_X::i((Fl_Window*)window);
  if (!i) return; // window not mapped, so ignore it

  if (X<=0 && Y<=0 && W>=window->w() && H>=window->h()) {
    // if damage covers entire window delete region:
    window->damage(flags);
    return;
  }

  // clip the damage to the window and quit if none:
  if (X < 0) {W += X; X = 0;}
  if (Y < 0) {H += Y; Y = 0;}
  if (W > window->w()-X) W = window->w()-X;
  if (H > window->h()-Y) H = window->h()-Y;
  if (W <= 0 || H <= 0) return;

  if (window->damage() & ~FL_DAMAGE_LAYOUT) {
    // if we already have damage we must merge with existing region:
    if (i->region) {
#ifndef WIN32
      XRectangle R;
      R.x = X; R.y = Y; R.width = W; R.height = H;
      XUnionRectWithRegion(&R, i->region, i->region);
#else
      Region R = XRectangleRegion(X, Y, W, H);
      CombineRgn(i->region, i->region, R, RGN_OR);
      XDestroyRegion(R);
#endif
    }
  } else {
    // create a new region:
    if (i->region) XDestroyRegion(i->region);
    i->region = XRectangleRegion(X,Y,W,H);
  }
  window->damage_ |= flags;
  Fl::damage(FL_DAMAGE_CHILD);
}

////////////////////////////////////////////////////////////////
// Events:

int Fl_Widget::handle(int event) {
  // Must return 1 for tooltips and mouse tracking to work.  A
  // subclass can return zero to make itself "invisible" so that
  // it does not affect highlighting.
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
  case FL_MOVE:
    return 1;
  default:
    return 0;
  }
}

int Fl_Widget::take_focus() {
  if (focused()) return 1;
  if (!takesevents() || !handle(FL_FOCUS)) return 0;
  if (!contains(Fl::focus())) Fl::focus(this);
  return 1;
}

void Fl_Widget::activate() {
  if (!active()) {
    clear_flag(FL_INACTIVE);
    if (active_r()) {
      damage_label(); redraw();
      handle(FL_ACTIVATE);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void Fl_Widget::deactivate() {
  if (active_r()) {
    set_flag(FL_INACTIVE);
    damage_label(); redraw();
    handle(FL_DEACTIVATE);
    throw_focus();
  } else {
    set_flag(FL_INACTIVE);
  }
}

int Fl_Widget::active_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->active()) return 0;
  return 1;
}

void Fl_Widget::show() {
  if (!visible()) {
    clear_flag(FL_INVISIBLE);
    if (visible_r()) {
      damage_label(); redraw();
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

int Fl_Widget::visible_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->visible()) return 0;
  return 1;
}

// return true if widget is inside (or equal to) this:
// Returns false for null widgets.
int Fl_Widget::contains(const Fl_Widget *o) const {
  for (; o; o = o->parent_) if (o == this) return 1;
  return 0;
}

int Fl_Widget::pushed() const {return this == Fl::pushed();}

int Fl_Widget::focused() const {return this == Fl::focus();}

int Fl_Widget::belowmouse() const {return this == Fl::belowmouse();}

////////////////////////////////////////////////////////////////

// Code to test and parse fltk shortcut numbers.
//
// A shortcut is a keysym or'd with shift flags.  In the simplest
// sense a shortcut is matched if the shift state is exactly as
// given and the key returning that keysym is pressed.
//
// To make it easier to match some things it is more complex:
//
// Only FL_META, FL_ALT, FL_SHIFT, and FL_CTRL must be "off".  A
// zero in the other shift flags indicates "dont care".
//
// It also checks against the first character of Fl::event_text(),
// and zero for FL_SHIFT means "don't care".
// This allows punctuation shortcuts like "#" to work (rather than
// calling it "shift+3")

// Test against an arbitrary shortcut:
int Fl::test_shortcut(int shortcut) {
  if (!shortcut) return 0;

  int shift = Fl::event_state();
  // see if any required shift flags are off:
  if ((shortcut&shift) != (shortcut&0x7fff0000)) return 0;
  // record shift flags that are wrong:
  int mismatch = (shortcut^shift)&0x7fff0000;
  // these three must always be correct:
  if (mismatch&(FL_META|FL_ALT|FL_CTRL)) return 0;

  int key = shortcut & 0xffff;

  // if shift is also correct, check for exactly equal keysyms:
  if (!(mismatch&(FL_SHIFT)) && key == Fl::event_key()) return 1;

  // try matching ascii, ignore shift:
  if (key == Fl::event_text()[0]) return 1;

  // kludge so that Ctrl+'_' works (as opposed to Ctrl+'^_'):
  if ((shift&FL_CTRL) && key >= 0x3f && key <= 0x5F
      && Fl::event_text()[0]==(key^0x40)) return 1;
  return 0;
}

// Test against shortcut() and possibly against a &x shortcut in the label:

int Fl_Widget::test_shortcut() const {

  if (Fl::test_shortcut(shortcut())) return 1;

  if (flags()&FL_NO_SHORTCUT_LABEL) return 0;

  char c = Fl::event_text()[0];
  const char* label = this->label();
  if (!c || !label) return 0;
  for (;;) {
    if (!*label) return 0;
    if (*label++ == '&') {
      if (*label == '&') label++;
      else if (*label == c) return 2;
      else return 0;
    }
  }
}

////////////////////////////////////////////////////////////////
// Drawing methods (designed to be called from a draw() implementation):

// Draw the surrounding box of a normal widget:
Fl_Flags Fl_Widget::draw_box() const {
  Fl_Flags f = flags();
  if (!active_r()) f |= FL_INACTIVE;
  box()->draw(this, x(),y(),w(),h(), f);
  return f;
}

// Draw the box and label as for an Fl_Button, return flags to pass to
// draw_label:
Fl_Flags Fl_Widget::draw_button() const {
  return draw_button(flags());
}

// This version is used to override the setting of FL_VALUE:
Fl_Flags Fl_Widget::draw_button(Fl_Flags flags) const {
  // only use the pushed-in color if the user has explicitly set it
  // on this widget:
  if ((flags&FL_VALUE) && style_->selection_color)
    flags |= FL_SELECTED;
  if (!active_r())
    flags |= FL_INACTIVE;
  else if (belowmouse())
    flags |= FL_HIGHLIGHT;
  if (focused())
    flags |= FL_FOCUSED;
  // We need to erase the focus rectangle for FL_NO_BOX buttons, such
  // as checkmarks:
  else if (box()==FL_NO_BOX && (damage()&FL_DAMAGE_HIGHLIGHT)) {
    fl_clip(x(), y(), w(), h());
    parent()->draw_group_box();
    fl_pop_clip();
  }
  draw_box(x(), y(), w(), h(), flags);
  return flags;
}

// Draw the background behind text/recessed area:
Fl_Flags Fl_Widget::draw_text_box() const {
  return draw_text_box(x(),y(),w(),h());
}

Fl_Flags Fl_Widget::draw_text_box(int x, int y, int w, int h) const {
  Fl_Flags f = draw_text_frame(x,y,w,h);
  text_box()->inset(x,y,w,h); //if (w <= 0 || h <= 0) return;
  fl_color(text_background());
  fl_rectf(x,y,w,h);
  return f;
}

// Draw only the edge of the text/recessed area, but no interior:
Fl_Flags Fl_Widget::draw_text_frame() const {
  return draw_text_frame(x(),y(),w(),h());
}

Fl_Flags Fl_Widget::draw_text_frame(int x, int y, int w, int h) const {
  Fl_Flags f = flags();
  if (!active_r()) f |= FL_INACTIVE;
  text_box()->draw(this, x,y,w,h, f|FL_FRAME_ONLY);
  return f;
}

// Return the color to draw images on buttons:
Fl_Color Fl_Widget::glyph_color(Fl_Flags flags) const
{
  if (flags & FL_INACTIVE)
    return fl_inactive(text_color());
  else if (flags & FL_SELECTED)
    return selection_text_color();
  else if (flags & FL_HIGHLIGHT) {
    Fl_Color c = highlight_label_color();
    if (c) return c;
  }
  return text_color();
}

// Return the color to draw buttons:
Fl_Color Fl_Widget::box_color(Fl_Flags flags) const
{
  if (flags & FL_SELECTED)
    return selection_color();
  else if (flags & FL_HIGHLIGHT) {
    Fl_Color c = highlight_color();
    if (c) return c;
  }
  return color();
}

// Call the draw method, handle the clip out
void Fl_Widget::draw_n_clip()
{
  if (!(box()->fills_rectangle() ||
	image() && (flags()&FL_ALIGN_TILED) &&
	(!(flags()&15) || (flags() & FL_ALIGN_INSIDE)))) {
    fl_clip(x(), y(), w(), h());
    parent()->draw_group_box();
    fl_pop_clip();
  }
  draw();
  fl_clip_out(x(), y(), w(), h());
}

//
// End of "$Id: Fl_Widget.cxx,v 1.67 2000/08/20 04:31:38 spitzak Exp $".
//
