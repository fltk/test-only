//
// "$Id: Fl_Group.H,v 1.30 2001/02/16 22:55:45 robertk Exp $"
//
// Group header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Group_H
#define Fl_Group_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

class FL_API Fl_Group : public Fl_Widget {
public:

  int children() const {return children_;}
  Fl_Widget* child(int n) const {return array_[n];}

  void draw();
  void draw_n_clip();
  void draw_group_box() const;
  void layout();
  int handle(int);

  void begin() {current_ = this;}
  void end() {current_ = (Fl_Group*)parent();}
  static Fl_Group *current() {return current_;}
  static void current(Fl_Group *g) {current_ = g;}

  int find(const Fl_Widget*) const;
  int find(const Fl_Widget& o) const {return find(&o);}

  Fl_Group(int,int,int,int, const char * = 0);
  virtual ~Fl_Group();
  void add(Fl_Widget&);
  void add(Fl_Widget* o) {add(*o);}
  void insert(Fl_Widget&, int index);
  void insert(Fl_Widget& o, Fl_Widget* before) {insert(o,find(before));}
  void remove(int index);
  void remove(Fl_Widget& o) {remove(find(o));}
  void remove(Fl_Widget* o) {remove(find(*o));}
  void replace(int index, Fl_Widget&);
  void replace(Fl_Widget& old, Fl_Widget& o) {replace(find(old),o);}
  void clear();

  void resizable(Fl_Widget& o) {resizable_ = &o;}
  void resizable(Fl_Widget* o) {resizable_ = o;}
  Fl_Widget* resizable() const {return resizable_;}
  void add_resizable(Fl_Widget& o) {resizable_ = &o; add(o);}
  void init_sizes();

  void focus(int i) {focus_ = i;}
  void focus(Fl_Widget* w) {focus(find(w));}
  int focus() const {return focus_;}
  static int navigation_key();

  // back compatability function:
  Fl_Widget* & _ddfdesign_kludge() {return resizable_;}
  friend FL_FORMS_API void fl_end_group(); // forms emulation function
  void fix_old_positions();

protected:

  void draw_child(Fl_Widget&) const;
  void update_child(Fl_Widget&) const;
  void draw_outside_label(Fl_Widget&) const ;
  int* sizes();
  int ox() const {return ox_;}
  int oy() const {return oy_;}
  int ow() const {return ow_;}
  int oh() const {return oh_;}
  void set_old_size() {ox_=x();oy_=y();ow_=w();oh_=h();}
  int send(int, Fl_Widget&);

private:

  int children_;
  int focus_;
  Fl_Widget** array_;

  Fl_Widget* resizable_;
  int *sizes_; // remembered initial sizes of children
  int ox_,oy_,ow_,oh_; // old size, used by layout

  static Fl_Group *current_;

};

// This dummy class can be used in constructors to set the parent
// group to any desired value before creating child widget instances.
class FL_API Fl_End {
public:
  Fl_End() {Fl_Group::current()->end();}
  Fl_End(Fl_Group* g) {Fl_Group::current(g);}
};

#endif

//
// End of "$Id: Fl_Group.H,v 1.30 2001/02/16 22:55:45 robertk Exp $".
//
