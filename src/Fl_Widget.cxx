//
// "$Id: Fl_Widget.cxx,v 1.44 1999/11/21 06:23:27 carl Exp $"
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
#include <FL/Fl_Group.H>

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

int Fl_Widget::handle(int e) { return (e == FL_ENTER) ? 1 : 0; }

Fl_Widget::Fl_Widget(int X, int Y, int W, int H, const char* L) {
  style_	= default_style;
  parent_	= 0;
  callback_	= default_callback;
  user_data_ 	= 0;
  label_	= L;
  image_	= 0;
  tooltip_	= 0;
  flags_	= 0;
  x_ = X; y_ = Y; w_ = W; h_ = H;
  type_		= 0;
  damage_	= 0;
  when_		= FL_WHEN_RELEASE;
  if (Fl_Group::current()) Fl_Group::current()->add(this);
}

void Fl_Widget::layout() {
  damage_ &= ~FL_DAMAGE_LAYOUT;
}

void Fl_Widget::resize(int X, int Y, int W, int H) {
  x_ = X; y_ = Y; w_ = W; h_ = H;
  damage_ |= FL_DAMAGE_LAYOUT;
  for (Fl_Widget* w = this->parent(); w; w = w->parent())
    w->damage_ |= FL_DAMAGE_LAYOUT;
}

// this is useful for parent widgets to call to resize children:
int Fl_Widget::damage_resize(int X, int Y, int W, int H) {
  if (x() == X && y() == Y && w() == W && h() == H) return 0;
  resize(X, Y, W, H);
  redraw();
  return 1;
}

int Fl_Widget::take_focus() {
  if (!takesevents()) return 0;
  if (!handle(FL_FOCUS)) return 0; // see if it wants it
  if (contains(Fl::focus())) return 1; // it called Fl::focus for us
  Fl::focus(this);
  return 1;
}

void Fl_Widget::activate() {
  if (!active()) {
    clear_flag(FL_INACTIVE);
    if (active_r()) {
      damage(FL_DAMAGE_ALL|FL_DAMAGE_CHILD_LABEL);
      handle(FL_ACTIVATE);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void Fl_Widget::deactivate() {
  if (active_r()) {
    set_flag(FL_INACTIVE);
    damage(FL_DAMAGE_ALL|FL_DAMAGE_CHILD_LABEL);
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
      damage(FL_DAMAGE_ALL|FL_DAMAGE_CHILD_LABEL);
      handle(FL_SHOW);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
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

// When a widget is destroyed it can destroy unique styles:

Fl_Widget::~Fl_Widget() {
#if 0
  // this causes the color chooser (and perhaps other composite objects)
  // to crash, and also slows down destruction.
  if (parent_) parent_->remove(this);
#else
  // the original fltk hack, to prevent double destruction if the parent
  // group is destroyed.  Note that if you don't plan to destroy the group
  // you better do the remove() yourself!
  parent_ = 0;
#endif
  throw_focus();
  if (style_->dynamic()) {
    Fl_Style* s = (Fl_Style*)style_; // cast away const
    delete s;
  }
}

////////////////////////////////////////////////////////////////

// Draw the surrounding box of a normal widget:
void Fl_Widget::draw_box() const {
  Fl_Flags f = active_r() ? FL_NO_FLAGS : FL_INACTIVE;
  box()->draw(x(), y(), w(), h(), color(), f);
}

// Draw the surrounding box but no interior:
void Fl_Widget::draw_frame() const {
  Fl_Flags f = active_r() ? FL_FRAME_ONLY : (FL_INACTIVE|FL_FRAME_ONLY);
  box()->draw(x(), y(), w(), h(), color(), f);
}

// Draw the box of a widget that acts as a button, and return the color
// to pass to draw_button_label():
Fl_Color Fl_Widget::draw_button() const {
  Fl_Flags f = flags();
  Fl_Color c = color();
  Fl_Color lc = label_color();
  if (!active_r()) {
    f |= FL_INACTIVE;
    // lc = fl_inactive(lc); don't do this, draw_button_label() will do it
  } else if (Fl::belowmouse() == this && highlight_color()) {
    f |= FL_HIGHLIGHT;
    c = highlight_color();
    lc = highlight_label_color();
  }
  if (f & FL_VALUE) {
    c = selection_color();
    lc = selection_text_color();
  }

  box()->draw(x(), y(), w(), h(), c, f);
  return lc;
}

void Fl_Widget::draw_glyph(int T, int X,int Y,int W,int H, Fl_Flags f, Fl_Boxtype b) const {
  if (!b) b = glyph_box();
  Fl_Color bc = off_color(), fc = label_color();
  if (f&FL_VALUE) {
    bc = selection_color();
    fc = selection_text_color();
  }
  if ((f&FL_HIGHLIGHT) && !(f&FL_INACTIVE) && highlight_color()) {
    fc = highlight_label_color();
    bc = highlight_color();
  }
  fc = fl_inactive(fc, f);
  bc = fl_inactive(bc, f);
  glyph()(T, X,Y,W,H, bc, fc, f, b);
}

// Call the draw method, handle the clip out
#include <FL/fl_draw.H>
void Fl_Widget::draw_n_clip()
{
  if (!box()->fills_rectangle() && !(image() && (flags()&FL_ALIGN_TILED) &&
		(!(flags()&15) || (flags() & FL_ALIGN_INSIDE)))) {
    fl_clip(x(), y(), w(), h());
    parent()->draw_group_box();
    fl_pop_clip();
  }
  clear_damage(FL_DAMAGE_ALL);
  draw();
  clear_damage();
  fl_clip_out(x(), y(), w(), h());
}

void fl_glyph_default(int t, int x,int y,int w,int h, Fl_Color bc, Fl_Color fc,
                     Fl_Flags f, Fl_Boxtype box)
{
  box->draw(x,y,w,h, bc, f);
}

//
// End of "$Id: Fl_Widget.cxx,v 1.44 1999/11/21 06:23:27 carl Exp $".
//
