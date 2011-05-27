//
// "$Id$"
//
// Group header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#include "Fl_Widget.H"

class FL_EXPORT Fl_Group : public Fl_Widget {
#if 0 // FIXME: 123
  int navigation(int);
  Fl_Group(const Fl_Group&);
  Fl_Group& operator=(const Fl_Group&);
#endif
  
protected:  
  Fl_Group() {}

#if 0 // FIXME: 123
  void draw();
  void draw_child(Fl_Widget& widget) const;
  void draw_children();
  void draw_outside_label(const Fl_Widget& widget) const ;
  void update_child(Fl_Widget& widget) const;
  int  *sizes();
#endif
  
public:
  Fl_Group(int,int,int,int, const char * = 0);

  void end() {
    ((fltk3::Group*)_p)->end();
  }
  
#if 0 // FIXME: 123
  int handle(int);
  void begin();
  static Fl_Group *current();
  static void current(Fl_Group *g);
  int children() const {return children_;}
  Fl_Widget* child(int n) const {return array()[n];}
  int find(const Fl_Widget*) const;
  int find(const Fl_Widget& o) const {return find(&o);}
  Fl_Widget* const* array() const;
  void resize(int,int,int,int);
  virtual ~Fl_Group();
  void add(Fl_Widget&);
  void add(Fl_Widget* o) {add(*o);}
  void insert(Fl_Widget&, int i);
  void insert(Fl_Widget& o, Fl_Widget* before) {insert(o,find(before));}
  void remove(int index);
  void remove(Fl_Widget&);
  void remove(Fl_Widget* o) {remove(*o);}
  void clear();
#endif
  
  void resizable(Fl_Widget& o) {
    ((fltk3::Group*)_p)->resizable((fltk3::Widget&)(*o._p));
  }
  
  void resizable(Fl_Widget* o) {
    ((fltk3::Group*)_p)->resizable((fltk3::Widget*)(o->_p));
  }
  
  Fl_Widget* resizable() const {
    return fltk3::_3to1_widget(((fltk3::Group*)_p)->resizable());
  }
  
#if 0 // FIXME: 123
  void add_resizable(Fl_Widget& o) {resizable_ = &o; add(o);}
  void init_sizes();
  void clip_children(int c) { if (c) set_flag(CLIP_CHILDREN); else clear_flag(CLIP_CHILDREN); }
  unsigned int clip_children() { return (flags() & CLIP_CHILDREN) != 0; }
  virtual Fl_Group* as_group() { return this; }
  void focus(Fl_Widget* W) {W->take_focus();}
  Fl_Widget* & _ddfdesign_kludge() {return resizable_;}
  void forms_end();
#endif
  
};

class FL_EXPORT Fl_End {
#if 0 // FIXME: 123
public:
  /** All it does is calling Fl_Group::current()->end() */
  Fl_End() {Fl_Group::current()->end();}
#endif
};

#endif

//
// End of "$Id$".
//
