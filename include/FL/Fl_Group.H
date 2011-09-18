//
// "$Id$"
//
// Group header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef Fl_Group_H
#define Fl_Group_H

#include <fltk3/Group.h>
#include "Fl_Widget.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Group, Group)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Group, Group)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Group : public Fl_Widget {

protected:  
  
  Fl_Group() { /* empty */ }

  void draw_child(Fl_Widget& widget) const {
    ((fltk3::Group_I*)_p)->draw_child(*(fltk3::_1to3_widget(&widget)));
  }
  
  void draw_children() {
    ((fltk3::Group_I*)_p)->draw_children();
  }
  
  void draw_outside_label(const Fl_Widget& widget) const {
    ((fltk3::Group_I*)_p)->draw_outside_label(*(fltk3::_1to3_widget((Fl_Widget*)&widget)));
  }
  
  void update_child(Fl_Widget& widget) const {
    ((fltk3::Group_I*)_p)->update_child(*(fltk3::_1to3_widget(&widget)));
  }
  
  int *sizes() {
    return ((fltk3::Group_I*)_p)->sizes();
  }
  
public:
  
  FLTK3_WIDGET_VCALLS(Group)

  Fl_Group(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::Group_I(x, y, w, h, label);
    _p->wrapper(this);
  }

  void begin() {
    ((fltk3::Group*)_p)->begin();
  }
    
  void end() {
    ((fltk3::Group*)_p)->end();
  }
  
  static Fl_Group *current() {
    return fltk3::_3to1_group( fltk3::Group::current() );
  }
  
  static void current(Fl_Group *g) {
    fltk3::Group::current(fltk3::_1to3_group(g));
  }
  
  int children() const {
    return ((fltk3::Group*)_p)->children();
  }
  
  Fl_Widget* child(int n) const {
    return fltk3::_3to1_widget( ((fltk3::Group*)_p)->child(n) );
  }
  
  int find(const Fl_Widget *w) const {
    return ((fltk3::Group*)_p)->find(fltk3::_1to3_widget((Fl_Widget*)w));
  }
  
  int find(const Fl_Widget& o) const {
    return ((fltk3::Group*)_p)->find(*(fltk3::_1to3_widget((Fl_Widget*)&o)));
  }
  
  // FIXME: 123 - can't traslate easily! Fl_Widget* const* array() const;
  
  void add(Fl_Widget &o) {
    ((fltk3::Group*)_p)->find(*(fltk3::_1to3_widget(&o)));
  }
  
  void add(Fl_Widget* o) {
    ((fltk3::Group*)_p)->add(fltk3::_1to3_widget(o));
  }
  
  void insert(Fl_Widget &o, int i) {
    ((fltk3::Group*)_p)->insert(*(fltk3::_1to3_widget(&o)), i);
  }
  
  void insert(Fl_Widget& o, Fl_Widget* before) {
    ((fltk3::Group*)_p)->insert(*(fltk3::_1to3_widget(&o)), fltk3::_1to3_widget((Fl_Widget*)before));
  }
  
  void remove(int index) {
    ((fltk3::Group*)_p)->remove(index);
  }
  
  void remove(Fl_Widget &o) {
    ((fltk3::Group*)_p)->remove(*(fltk3::_1to3_widget(&o)));
  }
  
  void remove(Fl_Widget* o) {
    ((fltk3::Group*)_p)->remove(fltk3::_1to3_widget(o));
  }
  
  void clear() {
    ((fltk3::Group*)_p)->clear();
  }
  
  void resizable(Fl_Widget& o) {
    ((fltk3::Group*)_p)->resizable(*(fltk3::_1to3_widget(&o)));
  }
  
  void resizable(Fl_Widget* o) {
    ((fltk3::Group*)_p)->resizable(fltk3::_1to3_widget(o));
  }
  
  Fl_Widget* resizable() const {
    return fltk3::_3to1_widget(((fltk3::Group*)_p)->resizable());
  }
  
  void add_resizable(Fl_Widget& o) {
    ((fltk3::Group*)_p)->add_resizable(*(fltk3::_1to3_widget(&o)));
  }
  
  void init_sizes() {
    ((fltk3::Group*)_p)->init_sizes();
  }
  
  void clip_children(int c) {
    ((fltk3::Group*)_p)->clip_children(c);
  }
  
  unsigned int clip_children() {
    return ((fltk3::Group*)_p)->clip_children();
  }
  
  virtual Fl_Group* as_group() { return this; }
  
  void focus(Fl_Widget* W) {
    ((fltk3::Group*)_p)->remove(fltk3::_1to3_widget(W));
  }
  
  void forms_end() {
    ((fltk3::Group*)_p)->forms_end();
  }
  
};


class FL_EXPORT Fl_End {
public:
  Fl_End() { Fl_Group::current()->end(); }
};


#endif

//
// End of "$Id$".
//
