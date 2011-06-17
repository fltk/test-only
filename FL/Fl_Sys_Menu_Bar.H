//
// "$Id$"
//
// MacOS system menu bar header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Sys_Menu_Bar_H
#define Fl_Sys_Menu_Bar_H

#include <fltk3/SysMenuBar.h>
#include "Fl_Menu_Bar.H"
#include "x.H"

#if defined(__APPLE__)

class FL_EXPORT Fl_Sys_Menu_Bar : public Fl_Menu_Bar {
  
protected:
  
  void draw();
  
public:

  Fl_Sys_Menu_Bar(int x,int y,int w,int h,const char *l=0) {
    _p = new fltk3::SysMenuBar(x, y, w, h, l);
    _p->wrapper(this);
  }

  void menu(const Fl_Menu_Item *m) {
    ((fltk3::SysMenuBar*)_p)->menu((fltk3::MenuItem*)m);
  }
  
  int add(const char* label, int shortcut, Fl_Callback *cb, void *user_data=0, int flags=0) {
    return ((fltk3::SysMenuBar*)_p)->add(label, shortcut, cb, user_data, flags);
  }
  
  int insert(int index, const char* label, int shortcut, Fl_Callback *cb, void *user_data=0, int flags=0) {
    return ((fltk3::SysMenuBar*)_p)->insert(index, label, shortcut, cb, user_data, flags);
  }
  
  void remove(int n) {
    ((fltk3::SysMenuBar*)_p)->remove(n);
  }
  
  void replace(int rank, const char *name) {
    ((fltk3::SysMenuBar*)_p)->replace(rank, name);
  }
  
  void clear() {
    ((fltk3::SysMenuBar*)_p)->clear();
  }
  
  int clear_submenu(int index) {
    return ((fltk3::SysMenuBar*)_p)->clear_submenu(index);
  }
  
};

#else

typedef Fl_Menu_Bar Fl_Sys_Menu_Bar;

#endif // defined(__APPLE__) || defined(FL_DOXYGEN)

#endif // Fl_Sys_Menu_Bar_H

//
// End of "$Id$".
//
