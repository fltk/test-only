//
// "$Id: Fl_Widget.cxx,v 1.61 2000/05/15 05:52:27 bill Exp $"
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
#if 0
  // This would be the logical thing to do, but it crashes composite
  // objects, and greatly slows down the destruction of a tree:
  if (parent_) parent_->remove(this);
#else
  // Instead this is used to prevent double destruction when the parent
  // group is destroyed.  If you don't plan to destroy the group
  // immediately, you better do the remove() yourself!
  parent_ = 0;
#endif
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
  // it does not effect highlighting.
  if (event == FL_ENTER) return 1;
  else return 0;
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
// Drawing methods (designed to be called from a draw() implementation):

// Draw the surrounding box of a normal widget:
void Fl_Widget::draw_box() const {
  box()->draw(x(),y(),w(),h(), color(),
	      active_r() ? FL_NO_FLAGS : FL_INACTIVE);
}

// Draw the surrounding box but no interior:
void Fl_Widget::draw_frame() const {
  box()->draw(x(), y(), w(), h(), color(),
	      active_r() ? FL_FRAME_ONLY : (FL_INACTIVE|FL_FRAME_ONLY));
}

// Draw the surrounding box of a text area widget:
void Fl_Widget::draw_window_box() const {
  window_box()->draw(x(),y(),w(),h(), window_color(),
		     active_r() ? FL_NO_FLAGS : FL_INACTIVE);
}

// Draw the surrounding box but no interior:
void Fl_Widget::draw_window_frame() const {
  window_box()->draw(x(), y(), w(), h(), window_color(),
		     active_r() ? FL_FRAME_ONLY : (FL_INACTIVE|FL_FRAME_ONLY));
}

// Draw the box of a widget that acts as a button, and return the color
// to pass to draw_button_label():
Fl_Color Fl_Widget::draw_button(Fl_Flags flags) const {
  Fl_Color background_color = color();
  Fl_Color foreground_color = label_color();
  if (!active_r()) {
    flags |= FL_INACTIVE;
  } else if (belowmouse()) {
    flags |= FL_HIGHLIGHT;
    Fl_Color c = highlight_color(); if (c) background_color = c;
    c = highlight_label_color(); if (c) foreground_color = c;
  }
  if (flags&FL_VALUE && box() != FL_NO_BOX) {
    background_color = selection_color();
    foreground_color = selection_text_color();
  }
  if (focused()) flags |= FL_FOCUSED;
  // We need to erase the focus rectangle for FL_NO_BOX buttons, such
  // as checkmarks:
  else if (box()==FL_NO_BOX && (damage()&FL_DAMAGE_HIGHLIGHT)) {
    fl_clip(x(), y(), w(), h());
    parent()->draw_group_box();
    fl_pop_clip();
  }
  box()->draw(x(), y(), w(), h(), background_color, flags);
  return foreground_color;
}

void Fl_Widget::draw_glyph(int T,int X,int Y,int W,int H, Fl_Flags flags) const
{
  Fl_Color background_color =
    (flags & FL_VALUE) ? selection_color() : color();
  Fl_Color foreground_color =
    (flags & FL_VALUE) ? selection_text_color() : text_color();
  if (!active_r()) {
    flags |= FL_INACTIVE;
  } else if (flags & FL_HIGHLIGHT) {
    Fl_Color c = highlight_color(); if (c) background_color = c;
    c = highlight_label_color(); if (c) foreground_color = c;
  }
  glyph()(T, X,Y,W,H, background_color, foreground_color, flags, box());
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
// End of "$Id: Fl_Widget.cxx,v 1.61 2000/05/15 05:52:27 bill Exp $".
//
