//
// "$Id: Fl_Style.cxx,v 1.1 1999/11/14 08:42:48 bill Exp $"
//
// Code for managing Fl_Style structures.
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
#include <string.h>

// Copying a style pointer from another widget is not safe if that
// style is dynamic() because it may change or be deleted.  This makes
// another dynamic() copy if necessary.  Returns true if this new copy
// is made (no code uses this return value right now).

int Fl_Widget::copy_style(const Fl_Style* t) {
  if (style_ == t) return 0;
  if (style_ && style_->dynamic()) delete style_;
  if (!t->dynamic()) {style_ = t; return 0;}
  Fl_Style* newstyle = new Fl_Style;
  newstyle->parent = (Fl_Style*)t;
  style_ = newstyle;
  return 1;
}

// Retrieve values from a style, using parent's value if not in child:

unsigned Fl_Widget::geti(const unsigned* a) const {
  int i = a-(const unsigned*)&style_->color;
  for (const Fl_Style* s = style_; s; s = s->parent)
    if (*((unsigned*)(&s->color+i))) return *((unsigned*)(&s->color+i));
  return 0;
}

void* Fl_Widget::getp(const void* const* a) const {
  int i = a-(const void* const*)&style_->box;
  for (const Fl_Style* s = style_; s; s = s->parent)
    if (*((void**)(&s->box+i))) return *((void**)(&s->box+i));
  return 0;
}

// Widgets set their own attributes by (possibly) creating a unique copy
// of their current style and setting it there.  Because this copy does
// not have any children the recursive search is not needed:

Fl_Style* fl_unique_style(const Fl_Style* & pointer) {
  if (pointer->dynamic()) return (Fl_Style*)pointer;
  Fl_Style* newstyle = new Fl_Style;
  newstyle->parent = (Fl_Style*)pointer;
  pointer = newstyle;
  return newstyle;
}

void Fl_Widget::setp(const void* const * p, const void* v) {
  int d = p-(const void**)&style_->box;
  Fl_Style* s = fl_unique_style(style_);
  *((const void**)&s->box + d) = v;
}

void Fl_Widget::seti(const unsigned * p, unsigned v) {
  int d = p-(unsigned*)&style_->color;
  Fl_Style* s = fl_unique_style(style_);
  *((unsigned*)&s->color + d) = v;
}

// Named styles provide a list that can be searched by theme plugins.
// The "revert" function is mostly provided to make it easy to initialize
// the fields even though C++ does not allow a structure constant.
// It is alos used to undo theme changes.

Fl_Style* Fl_Style::first = 0;

static void plainrevert(Fl_Style*) {}

Fl_Style::Fl_Style(const char* n, void (*revert)(Fl_Style*)) {
  memset((void*)this, 0, sizeof(*this));
  parent = &Fl_Widget::default_style; // revert may want to change this
  if (n) { name = n; next = first; first = this;}
  if (revert) { revertfunc = revert; revert(this); }
  else revertfunc = plainrevert;
}

// This constructor is used to create dynamic() styles for widgets that
// change their own attributes:
Fl_Style::Fl_Style() {
  memset((void*)this, 0, sizeof(*this));
}

//
// End of "$Id: Fl_Style.cxx,v 1.1 1999/11/14 08:42:48 bill Exp $".
//
