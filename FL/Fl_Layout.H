// Layout header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Layout_H
#define Fl_Layout_H

#ifndef Fl_Group_H
#include <FL/Fl_Group.H>
#endif

class Fl_Layout
{
  static Fl_Group* group_[16];
  static int sp_;
  int rc_;
public:

  Fl_Layout():rc_(1){}
  virtual ~Fl_Layout();

  static void push_group(Fl_Group* g) {group_[++sp_]=g;}
  static void pop_group() {sp_--;}
  static Fl_Group* group() {return group_[sp_];}

  static void acquire(Fl_Layout* l) {if (l) l->rc_++;}
  static void release(Fl_Layout* l) {if (l && !--l->rc_) delete l;}
  virtual void perform() = 0;

  int x() const {return group()->x();}
  int y() const {return group()->y();}
  int w() const {return group()->w();}
  int h() const {return group()->h();}
  int ox() const {return group()->ox();}
  int oy() const {return group()->oy();}
  int ow() const {return group()->ow();}
  int oh() const {return group()->oh();}
  Fl_Widget*const* array() const {return group()->array();}
  int children() const {return group()->children();}
  void init_sizes() const {group()->init_sizes();}
  uchar type() const {return group()->type();}
  uchar damage() const {return group()->damage();}
  void damage(uchar d) const {group()->damage(d);}
};

class Fl_Align_Layout:public Fl_Layout
{
  bool vertical_;
  uchar n_to_break_,dw_,dh_;
  Fl_Align align_;
public:

  Fl_Align_Layout(uchar n_to_break = 0,uchar vertical = true,
                  Fl_Align align = FL_ALIGN_LEFT,
                  uchar dw = 0,uchar dh = 0)
    :vertical_(vertical),n_to_break_(n_to_break),dw_(dw),dh_(dh),
     align_(align){}
  void perform();

  bool vertical() const {return vertical_;}
  void vertical(bool v) {vertical_ = v;}

  uchar n_to_break() const {return n_to_break_;}
  void n_to_break(uchar n) {n_to_break_ = n;}

  uchar dw() const {return dw_;}
  void dw(uchar d) {dw_ = d;}
  uchar dh() const {return dh_;}
  void dh(uchar d) {dh_ = d;}

  Fl_Align align() const {return align_;}
  void align(Fl_Align a) {align_ = a;}
};

#endif
