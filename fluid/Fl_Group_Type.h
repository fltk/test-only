//
// "$Id$"
//
// Group type header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef FLUID3_GROUP_TYPE_H
#define FLUID3_GROUP_TYPE_H

#include "Fl_Widget_Type.h"

#include <fltk3/TabGroup.h>
#include <fltk3/PackedGroup.h>
#include <fltk3/WizardGroup.h>


class igroup : public fltk3::Group {
public:
  void resize(int,int,int,int);
  void full_resize(int X, int Y, int W, int H) { Group::resize(X, Y, W, H); }
  igroup(int X,int Y,int W,int H) : fltk3::Group(X,Y,W,H) {Group::current(0);}
};


class itabs : public fltk3::TabGroup {
public:
  void resize(int,int,int,int);
  void full_resize(int X, int Y, int W, int H) { Group::resize(X, Y, W, H); }
  itabs(int X,int Y,int W,int H) : fltk3::TabGroup(X,Y,W,H) {}
};


class iwizard : public fltk3::WizardGroup {
public:
  void resize(int,int,int,int);
  void full_resize(int X, int Y, int W, int H) { Group::resize(X, Y, W, H); }
  iwizard(int X,int Y,int W,int H) : fltk3::WizardGroup(X,Y,W,H) {}
};


class Fl_Group_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() {return "fltk3::Group";}
  virtual const char *alt_type_name() {return "Fl_Group";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    igroup *g = new igroup(X,Y,W,H); fltk3::Group::current(0); return g;}
  Fl_Widget_Type *_make() {return new Fl_Group_Type();}
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
  int is_parent() const {return 1;}
  int is_group() const {return 1;}
  int pixmapID() { return 6; }

  virtual fltk3::Widget *enter_live_mode(int top=0);
  virtual void leave_live_mode();
  virtual void copy_properties();
};


extern const char pack_type_name[];
extern fltk3::MenuItem pack_type_menu[];


class Fl_Pack_Type : public Fl_Group_Type {
  fltk3::MenuItem *subtypes() {return pack_type_menu;}
public:
  virtual const char *type_name() {return pack_type_name;}
  virtual const char *alt_type_name() {return "Fl_Pack";}
  Fl_Widget_Type *_make() {return new Fl_Pack_Type();}
  int pixmapID() { return 22; }
  void copy_properties();
};


extern const char table_type_name[];


class Fl_Table_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return table_type_name;}
  virtual const char *alt_type_name() {return "Fl_Table";}
  Fl_Widget_Type *_make() {return new Fl_Table_Type();}
  fltk3::Widget *widget(int X,int Y,int W,int H);
  int pixmapID() { return 51; }
  virtual fltk3::Widget *enter_live_mode(int top=0);
  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
};


extern const char tabs_type_name[];


class Fl_Tabs_Type : public Fl_Group_Type {
public:
  virtual void ideal_spacing(int &x, int &y) {
     x = 10;
     fltk3::font(o->labelfont(), o->labelsize());
     y = fltk3::height() + o->labelsize() - 6;
  }
  virtual const char *type_name() {return tabs_type_name;}
  virtual const char *alt_type_name() {return "Fl_Tabs";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    itabs *g = new itabs(X,Y,W,H); fltk3::Group::current(0); return g;}
  Fl_Widget_Type *_make() {return new Fl_Tabs_Type();}
  Fl_Type* click_test(int,int);
  void add_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
  int pixmapID() { return 13; }
  fltk3::Widget *enter_live_mode(int top=0);
};


extern const char scroll_type_name[];
extern fltk3::MenuItem scroll_type_menu[];


class Fl_Scroll_Type : public Fl_Group_Type {
  fltk3::MenuItem *subtypes() {return scroll_type_menu;}
public:
  virtual const char *type_name() {return scroll_type_name;}
  virtual const char *alt_type_name() {return "Fl_Scroll";}
  Fl_Widget_Type *_make() {return new Fl_Scroll_Type();}
  int pixmapID() { return 19; }
  fltk3::Widget *enter_live_mode(int top=0);
  void copy_properties();
};


extern const char tile_type_name[];


class Fl_Tile_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return tile_type_name;}
  virtual const char *alt_type_name() {return "Fl_Tile";}
  Fl_Widget_Type *_make() {return new Fl_Tile_Type();}
  int pixmapID() { return 20; }
  void copy_properties();
};


extern const char wizard_type_name[];


class Fl_Wizard_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return wizard_type_name;}
  virtual const char *alt_type_name() {return "Fl_Wizard";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    iwizard *g = new iwizard(X,Y,W,H); fltk3::Group::current(0); return g;}
  Fl_Widget_Type *_make() {return new Fl_Wizard_Type();}
  int pixmapID() { return 21; }
};


#endif

//
// End of "$Id$".
//
