//
// "$Id$"
//
// Menu type header file for the Fast Light Tool Kit (FLTK).
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

#ifndef FLUID3_MENU_TYPE_H
#define FLUID3_MENU_TYPE_H

#include "Fl_Widget_Type.h"

#include <fltk3/Menu_.h>
#include <fltk3/MenuButton.h>
#include <fltk3/Choice.h>
#include <fltk3/InputChoice.h>
#include <fltk3/Window.h>
#include <fltk3/MenuBar.h>


extern fltk3::MenuItem menu_item_type_menu[];

class Fl_Menu_Item_Type : public Fl_Widget_Type {
public:
  fltk3::MenuItem* subtypes() {return menu_item_type_menu;}
  const char* type_name() {return "MenuItem";}
  const char* alt_type_name() {return "Fl_Menu_Item";}
  Fl_Type* make();
  int is_menu_item() const {return 1;}
  int is_button() const {return 1;} // this gets shortcut to work
  fltk3::Widget* widget(int,int,int,int) {return 0;}
  Fl_Widget_Type* _make() {return 0;}
  const char* menu_name(int& i);
  int flags();
  void write_static();
  void write_item();
  void write_code1();
  void write_code2();
  int pixmapID() { return 16; }
};


class Fl_Submenu_Type : public Fl_Menu_Item_Type {
public:
  fltk3::MenuItem* subtypes() {return 0;}
  const char* type_name() {return "Submenu";}
  const char* alt_type_name() {return "Submenu";}
  int is_parent() const {return 1;}
  int is_button() const {return 0;} // disable shortcut
  Fl_Type* make();
  // changes to submenu must propagate up so build_menu is called
  // on the parent Fl_Menu_Type:
  void add_child(Fl_Type*a, Fl_Type*b) {parent->add_child(a,b);}
  void move_child(Fl_Type*a, Fl_Type*b) {parent->move_child(a,b);}
  void remove_child(Fl_Type*a) {parent->remove_child(a);}
  int pixmapID() { return 18; }
};


class Fl_Menu_Type : public Fl_Widget_Type {
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
    fltk3::Menu_ *myo = (fltk3::Menu_*)(w==4 ? ((Fl_Widget_Type*)this->factory)->o : this->o);
    switch (w) {
      case 4:
      case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
      case 1: myo->textfont(f); break;
      case 2: myo->textsize(s); break;
      case 3: myo->textcolor(c); break;
    }
    return 1;
  }
public:
  int is_menu_button() const {return 1;}
  int is_parent() const {return 1;}
  int menusize;
  virtual void build_menu();
  Fl_Menu_Type() : Fl_Widget_Type() {menusize = 0;}
  ~Fl_Menu_Type() {
    if (menusize) delete[] (fltk3::MenuItem*)(((fltk3::Menu_*)o)->menu());
  }
  void add_child(Fl_Type*, Fl_Type*) {build_menu();}
  void move_child(Fl_Type*, Fl_Type*) {build_menu();}
  void remove_child(Fl_Type*) {build_menu();}
  Fl_Type* click_test(int x, int y);
  void write_code2();
  void copy_properties();
};


extern fltk3::MenuItem button_type_menu[];


class Fl_Menu_Button_Type : public Fl_Menu_Type {
  fltk3::MenuItem *subtypes() {return button_type_menu;}
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Widget_Type::ideal_size(w, h);
    w += 2 * ((o->labelsize() - 3) & ~1) + o->labelsize() - 4;
    h = (h / 5) * 5;
    if (h < 15) h = 15;
    if (w < (15 + h)) w = 15 + h;
  }
  virtual const char *type_name() {return "fltk3::MenuButton";}
  virtual const char *alt_type_name() {return "Fl_Menu_Button";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    return new fltk3::MenuButton(X,Y,W,H,"menu");}
  Fl_Widget_Type *_make() {return new Fl_Menu_Button_Type();}
  int pixmapID() { return 26; }
};


extern fltk3::MenuItem dummymenu[];


class Fl_Choice_Type : public Fl_Menu_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Widget_Type::ideal_size(w, h);
    int w1 = o->h() - fltk3::box_dh(o->box());
    if (w1 > 20) w1 = 20;
    w1 = (w1 - 4) / 3;
    if (w1 < 1) w1 = 1;
    w += 2 * w1 + o->labelsize() - 4;
    h = (h / 5) * 5;
    if (h < 15) h = 15;
    if (w < (15 + h)) w = 15 + h;
  }
  virtual const char *type_name() {return "fltk3::Choice";}
  virtual const char *alt_type_name() {return "Fl_Choice";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    fltk3::Choice *myo = new fltk3::Choice(X,Y,W,H,"choice:");
    myo->menu(dummymenu);
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Choice_Type();}
  int pixmapID() { return 15; }
};


class Fl_Input_Choice_Type : public Fl_Menu_Type {
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
    fltk3::InputChoice *myo = (fltk3::InputChoice*)(w==4 ? ((Fl_Widget_Type*)this->factory)->o : this->o);
    switch (w) {
      case 4:
      case 0: f = (fltk3::Font)myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
      case 1: myo->textfont(f); break;
      case 2: myo->textsize(s); break;
      case 3: myo->textcolor(c); break;
    }
    return 1;
  }
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::InputChoice *myo = (fltk3::InputChoice *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() - 6;
    w = o->w() - 20 - fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + 20 + fltk3::box_dw(o->box());
    if (h < 15) h = 15;
    if (w < (15 + h)) w = 15 + h;
  }
  virtual const char *type_name() {return "fltk3::InputChoice";}
  virtual const char *alt_type_name() {return "Fl_Input_Choice";}
  virtual Fl_Type* click_test(int,int);
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    fltk3::InputChoice *myo = new fltk3::InputChoice(X,Y,W,H,"input choice:");
    myo->menu(dummymenu);
    myo->value("input");
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Input_Choice_Type();}
  virtual void build_menu();
  int pixmapID() { return 15; }
  void copy_properties();
};


class Fl_Menu_Bar_Type : public Fl_Menu_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    w = o->window()->w();
    h = ((o->labelsize() + fltk3::box_dh(o->box()) + 4) / 5) * 5;
    if (h < 15) h = 15;
  }
  virtual const char *type_name() {return "fltk3::MenuBar";}
  virtual const char *alt_type_name() {return "Fl_Menu_Bar";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {return new fltk3::MenuBar(X,Y,W,H);}
  Fl_Widget_Type *_make() {return new Fl_Menu_Bar_Type();}
  int pixmapID() { return 17; }
};


#endif

//
// End of "$Id$".
//
