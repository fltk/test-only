//
// "$Id: Fl_Menu_.H,v 1.29 2000/10/18 17:01:48 spitzak Exp $"
//
// Menu base class header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#ifndef Fl_Menu__H
#define Fl_Menu__H

#include "Fl_Group.H"
#include "Fl_Menu_Item.H"

class Fl_Menu_;

class FL_API Fl_List {
public:
  virtual int children(const Fl_Menu_*, const int* indexes, int level);
  virtual Fl_Widget* child(const Fl_Menu_*, const int* indexes, int level);
  virtual void flags_changed(const Fl_Menu_*, Fl_Widget*);
};

class FL_API Fl_Menu_ : public Fl_Group {

public:

  Fl_Menu_(int,int,int,int, const char* l=0);

  Fl_List* list() const {return list_;}
  void list(Fl_List* l) {list_ = l;}

  int children(const int* indexes, int level) const ;
  int children() const ;
  Fl_Widget* child(const int* indexes, int level) const ;
  Fl_Widget* child(int index) const ;

  Fl_Widget* item() const {return item_;}
  Fl_Widget* item(Fl_Widget* w) {return item_ = w;}
  virtual int goto_item(const int* indexes, int level);

  int popup(int x,int y, const char* title = 0);
  int pulldown(int x,int y,int w,int h, Fl_Widget* t=0, int menubar = 0);
  int handle_shortcut();
  void global();
  void execute();

#ifndef FLTK_2
  // Commented-out methods cannot be emulated.

  int value() const {return focus();}
  void value(int v) {focus(v);}
  int  size() const {return children();}

//const Fl_Menu_Item* test_shortcut();

//Fl_Menu_Item* menu() const;
  void add(const Fl_Menu_Item* m, void* data = 0);
  void copy(const Fl_Menu_Item* m, void* data = 0) {clear(); add(m,data);}
  void menu(const Fl_Menu_Item* m) {copy(m,0);}

// in fltk1.0 this returned an Fl_Menu_Item*:
  Fl_Widget* find(const char* label) const;

//int index(Fl_Menu_Item* m) const { return m - menu_; }
//int index(const char* label) const;

// add() in fltk 1 returned the index, this returns the new widget:
  Fl_Widget* add(const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
  Fl_Widget* replace(const char*, int scut, Fl_Callback*, void* = 0, int = 0);
  Fl_Widget* add(const char*, void*);
  Fl_Widget* add(const char*);
  Fl_Widget* insert(int n, const char* s, void* data = 0);
  // necessary for stupid C++ rules:
  void add(Fl_Widget& o) {Fl_Group::add(o);}
  void add(Fl_Widget* o) {add(*o);}
  void insert(Fl_Widget& o, int n) {Fl_Group::insert(o, n);}

  void replace(int n, const char* s) { child(n)->label(s); }
//void replace(Fl_Menu_Item* m, const char* s) { replace(index(m), s); }
  void replace(const char* l, const char* s) { find(l)->label(s); }
  // necessary for stupid C++ rules:
  void replace(int index, Fl_Widget& o) {Fl_Group::replace(index, o);}
  void replace(Fl_Widget& old, Fl_Widget& o) {Fl_Group::replace(old,o);}

//void remove(Fl_Menu_Item* m) { remove(index(m)); }
  void remove(const char* l) { delete find(l); }
  // necessary for stupid C++ rules:
  void remove(int index) {Fl_Group::remove(index);}
  void remove(Fl_Widget& o) {Fl_Group::remove(o);}
  void remove(Fl_Widget* o) {Fl_Group::remove(o);}

  void shortcut(int i, int s) { child(i)->shortcut(s); }
//void shortcut(Fl_Menu_Item* m, int s) {shortcut(index(m), s);}
  void shortcut(const char* l, int s) { find(l)->shortcut(s); }

//void mode(int i,int x);
//void mode(Fl_Menu_Item* m, int x) {mode(index(m), x);}
//void mode(const char* l, int x) {mode(index(l), x);}
  unsigned mode(int i) const {return child(i)->flags() >> 8;}
//unsigned mode(Fl_Menu_Item* m) const {return mode(index(m));}
  unsigned mode(const char* l) const {return find(l)->flags() >> 8;}

  // in fltk 1.0 these returned/took an Fl_Menu_Item*:
  Fl_Widget* mvalue() {return item();}
  void value(Fl_Widget* o) {item(o);}

  const char *text(int i) const {return i >= 0 ? child(i)->label() : 0;}
  const char *text() const {return item_ ? item_->label() : 0;}

#endif
private:
  Fl_List* list_;
  Fl_Widget* item_;
};

#endif

//
// End of "$Id: Fl_Menu_.H,v 1.29 2000/10/18 17:01:48 spitzak Exp $".
//
