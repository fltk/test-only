//
// "$Id: Fl_Widget.cxx,v 1.16 1999/08/28 17:20:57 bill Exp $"
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
  style_	= &default_style;
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
bool Fl_Widget::damage_resize(int X, int Y, int W, int H) {
  if (x() == X && y() == Y && w() == W && h() == H) return false;
  resize(X, Y, W, H);
  redraw();
  return true;
}

bool Fl_Widget::take_focus() {
  if (!takesevents()) return false;
  if (!handle(FL_FOCUS)) return false; // see if it wants it
  if (contains(Fl::focus())) return true; // it called Fl::focus for us
  Fl::focus(this);
  return true;
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

bool Fl_Widget::active_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->active()) return false;
  return true;
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

////////////////////////////////////////////////////////////////
// Styles:

// Styles that are only used by one widget and have no children can
// safely be modified and destroyed.  This is detected with this:
static inline bool unique(const Fl_Style* s) {return s->child == s;}
static inline void make_unique(Fl_Style* s) {s->child = s;}

// Returns a style that can be modified.  This creates a unique
// child of the current style if necessary.  Functions like widget->box(n)
// call this.

Fl_Style* Fl_Widget::wstyle() {
  Fl_Style* oldstyle = (Fl_Style*)style_; // cast away const
  if (unique(oldstyle)) return oldstyle;
  Fl_Style* newstyle = new Fl_Style;
  *newstyle = *oldstyle;
  make_unique(newstyle);
  // insert it as first child into list:
  newstyle->next = oldstyle->child;
  if (newstyle->next) newstyle->next->previous = &(newstyle->next);
  newstyle->previous = &(oldstyle->child);
  oldstyle->child = newstyle;
  style_ = newstyle;
  return newstyle;
}

// Set the style to a structure that the caller owns.  This is assummed
// to be a static style structure and if this is the first time it has
// been used it is inserted into the inheritance tree as a child of the
// current style.  This returns true if this is the first time this
// style has been inserted, this can be used to test to initialize
// the style.

// This is also called by Fl_Menu.cxx to set Fl_Menu_Item styles:
void Fl_Style::add_child(Fl_Style* s) const {
  if (s->previous) return;
  s->next = child;
  if (s->next) s->next->previous = &(s->next);
  s->previous = &(((Fl_Style*)this)->child);
  s->child = 0;
  ((Fl_Style*)this)->child = s;
  // copy all the pointer-sized things:
  void** p = (void**)&s->box;
  void*const* q = (void**)&box;
  void** e = (void**)&s->label_type;
  for (; p <= e; p++,q++) if (!*p) *p = *q;
  // copy all the integer-sized things:
  unsigned *p1 = &s->color;
  const unsigned *q1 = &color;
  unsigned *e1 = &s->text_size;
  for (; p1 <= e1; p1++,q1++) if (!*p1) *p1 = *q1;
}

bool Fl_Widget::style(Fl_Style* s) {
  if (s->previous) {style_ = s; return false;}
  style_->add_child(s);
  style_ = s;
  return true;
}

// Copying a style pointer from another widget is not safe if that
// style is unique() because it may change or be deleted.  This makes
// another unique() copy if necessary.  Returns true if this new copy
// is made (no code uses this return value right now).

bool Fl_Widget::copy_style(const Fl_Style* t) {
  if (!unique(t)) {style_ = t; return false;}
  Fl_Style* parent = (Fl_Style*)t; // cast away const
  Fl_Style* s = new Fl_Style;
  *s = *t;
  make_unique(s);
  s->next = parent->child;
  if (s->next) s->next->previous = &(s->next);
  s->previous = &(parent->child);
  parent->child = s;
  style_ = s;
  return true;
}

// Inherit a pointer-sized thing:

void Fl_Style::setp(const void** p, const void* v) {
  if (*p == v) return;
  const void* old_v = *p;
  *p = v;
  if (!unique(this)) for (Fl_Style* s = child; s; s = s->next) {
    const void** p1 = (const void**)((char*)s + ((char*)p - (char*)this));
    if (*p1 == old_v) s->setp(p1, v);
  }
}
  
// Inherit a integer-sized thing:

void Fl_Style::seti(unsigned* p, unsigned v) {
  if (*p == v) return;
  unsigned old_v = *p;
  *p = v;
  if (!unique(this)) for (Fl_Style* s = child; s; s = s->next) {
    unsigned* p1 = (unsigned*)((char*)s + ((char*)p - (char*)this));
    if (*p1 == old_v) s->seti(p1, v);
  }
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
  if (unique(style_)) {
    Fl_Style* s = (Fl_Style*)style_; // cast away const
    *(s->previous) = s->next;
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
    if (highlight_label_color()) lc = highlight_label_color();
  }
  if (f & FL_VALUE) {
    c = selection_color();
    if (selection_text_color()) lc = selection_text_color();
  }
  box()->draw(x(), y(), w(), h(), c, f);
  return lc;
}

void Fl_Widget::draw_glyph(int T, int X,int Y,int W,int H, Fl_Flags f) const {
  Fl_Color c = color();
  if (!active_r()) {
    f |= FL_INACTIVE;
  } else if (Fl::belowmouse() == this && highlight_color()) {
    f |= FL_HIGHLIGHT;
    c = highlight_color();
  }
  glyph()(T, X,Y,W,H, c, f);
}

////////////////////////////////////////////////////////////////
// Some widgets have imbedded text fields (such as Fl_Value_Slider).
// This is to be discouraged because composite widgets (widgets with
// child widgets) is a better design.  But for back compatability
// they cross reference the Fl_Input/Fl_Output widget styles.  This
// caused problems if Fl_Input/Fl_Output were not intantiated, so I fix
// this by declaring their styles here, already set up as though the
// inheritance tree was built:

#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

Fl_Style Fl_Widget::default_style = {
  FL_NORMAL_BOX,// box
  fl_glyph,	// glyphs
  FL_HELVETICA,	// label_font
  FL_HELVETICA,	// text_font
  FL_NORMAL_LABEL, // label_type
  FL_GRAY,	// color
  FL_NO_COLOR,	// label_color
  FL_GRAY,	// selection_color / down_color / on_color
  FL_NO_COLOR,	// selection_text_color
  FL_NO_COLOR,	// off_color
  FL_NO_COLOR,	// highlight_color
  FL_NO_COLOR,	// highlight_label_color
  FL_NO_COLOR,	// text_color
  FL_NORMAL_SIZE,// label_size
  FL_NORMAL_SIZE,// text_size
  &Fl_Input::default_style,	// child
  0,		// next
  // make the previous pointer be non-zero so that style(this) does
  // not try to insert it into an inheritance tree:
  &(default_style.next) // previous
};

// Fl_Input, the box, color, and selection color are different:

Fl_Style Fl_Input::default_style = {
  FL_THIN_DOWN_BOX, // box DIFF
  fl_glyph,	// glyphs
  FL_HELVETICA,	// label_font
  FL_HELVETICA,	// text_font
  FL_NORMAL_LABEL, // label_type
  FL_WHITE,	// color DIFF
  FL_NO_COLOR,	// label_color
  FL_BLUE_SELECTION_COLOR, // selection_color DIFF
  FL_WHITE,	// selection_text_color
  FL_NO_COLOR,	// off_color
  FL_NO_COLOR,	// highlight_color
  FL_NO_COLOR,	// highlight_label_color
  FL_NO_COLOR,	// text_color
  FL_NORMAL_SIZE,// label_size
  FL_NORMAL_SIZE,// text_size
  &Fl_Output::default_style,	// child
  0,		// next
  &(Fl_Widget::default_style.child) // previous
};

// Fl_Output defaults to the same as Fl_Input and is a child of it:

Fl_Style Fl_Output::default_style = {
  FL_THIN_DOWN_BOX, // box DIFF
  fl_glyph,	// glyphs
  FL_HELVETICA,	// label_font
  FL_HELVETICA,	// text_font
  FL_NORMAL_LABEL, // label_type
  FL_WHITE,	// color DIFF
  FL_NO_COLOR,	// label_color
  FL_BLUE_SELECTION_COLOR, // selection_color DIFF
  FL_WHITE,	// selection_text_color
  FL_NO_COLOR,	// off_color
  FL_NO_COLOR,	// highlight_color
  FL_NO_COLOR,	// highlight_label_color
  FL_NO_COLOR,	// text_color
  FL_NORMAL_SIZE,// label_size
  FL_NORMAL_SIZE,// text_size
  0,		// child
  0,		// next
  &(Fl_Input::default_style.child) // previous
};

//
// End of "$Id: Fl_Widget.cxx,v 1.16 1999/08/28 17:20:57 bill Exp $".
//
