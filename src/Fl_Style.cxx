//
// "$Id: Fl_Style.cxx,v 1.8 1999/11/29 08:47:01 bill Exp $"
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

Fl_Named_Style* Fl_Named_Style::first;

// Do not change the contents of this ever.  The themes depend on getting
// a known state initially.
static void revert(Fl_Style* s) {
  s->box                   = FL_NORMAL_BOX;
  s->glyph_box             = FL_NORMAL_BOX;
  s->glyph                 = fl_glyph;
  s->label_font            = FL_HELVETICA;
  s->text_font             = FL_HELVETICA;
  s->label_type            = FL_NORMAL_LABEL;
  s->color                 = FL_GRAY;
  s->label_color           = FL_BLACK;
  s->selection_color       = FL_NO_COLOR;
  s->selection_text_color  = FL_BLACK;
  s->off_color             = FL_GRAY;
  s->highlight_color       = FL_NO_COLOR;
  s->highlight_label_color = FL_BLACK;
  s->text_color            = FL_BLACK;
  s->label_size		   = FL_NORMAL_SIZE;
  s->text_size             = FL_NORMAL_SIZE;
  s->leading		   = 2;
  s->parent                = 0;	// this is the topmost style always
}

Fl_Style* Fl_Widget::default_style;

// Copying a style pointer from another widget is not safe if that
// style is dynamic() because it may change or be deleted.  This makes
// another dynamic() copy if necessary.

int Fl_Widget::copy_style(const Fl_Style* t) {
  if (style_ == t) return 0;
  if (style_ && style_->dynamic()) delete (Fl_Style*)style_;
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
// It is also used to undo theme changes.

static void plainrevert(Fl_Style*) {}

Fl_Named_Style::Fl_Named_Style(const char* n, void (*revert)(Fl_Style*), Fl_Style** pds) {
  static bool init = 0;
  memset((void*)this, 0, sizeof(*this));
  if (!init) {
    init = 1;
    Fl_Widget::default_style = new Fl_Named_Style("default", ::revert, &Fl_Widget::default_style);
  }
  parent = Fl_Widget::default_style; // revert may want to change this
  if (revert) { revertfunc = revert; revert(this); }
  else revertfunc = plainrevert;
  next = Fl_Named_Style::first;
  Fl_Named_Style::first = this;
  pdefault_style = pds;
  name = n;
}

// This constructor is used to create dynamic() styles for widgets that
// change their own attributes:
Fl_Style::Fl_Style() {
  memset((void*)this, 0, sizeof(*this));
}

int Fl_Style::draw_boxes_inactive = 1;
int Fl_Style::inactive_menu_hack = 0;
double Fl_Style::inactive_color_weight = 0.33f;

#include <ctype.h>
Fl_Named_Style* Fl_Style::find(const char* name) {
  for (Fl_Named_Style* p = Fl_Named_Style::first; p; p = p->next) {
    const char* a = p->name;
    const char* b = name;
    if (!a) continue;
    for (;;) {
      if (*a == '_') {
	if (*b == ' ' || *b == '_');
	else {a++; continue;}
      } else if (tolower(*a) != tolower(*b)) break;
      if (!*a && !*b) return p;
      a++;
      b++;
    }
  }
  return 0;
}

//
// End of "$Id: Fl_Style.cxx,v 1.8 1999/11/29 08:47:01 bill Exp $".
//
