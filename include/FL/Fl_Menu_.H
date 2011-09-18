//
// "$Id$"
//
// Menu base class header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu__H
#define Fl_Menu__H

#include <fltk3/Menu_.h>
#include "Fl_Widget.H"
#include "Fl_Menu_Item.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Menu_, Menu_)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Menu_, Menu_)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Menu_ : public Fl_Widget {

public:
  
  FLTK3_WIDGET_VCALLS(Menu_)

  Fl_Menu_() { /* empty */ }
  
  Fl_Menu_(int x, int y, int w, int h, const char *label=0) {
    _p = new fltk3::Menu__I(x, y, w, h, label);
    _p->wrapper(this);
  }

  int item_pathname(char *name, int namelen, const Fl_Menu_Item *finditem=0) const {
    return ((fltk3::Menu_*)_p)->item_pathname(name, namelen, (const fltk3::MenuItem*)finditem);
  }
  
  const Fl_Menu_Item* picked(const Fl_Menu_Item *i) {
    return (Fl_Menu_Item*) ((fltk3::Menu_*)_p)->picked( (fltk3::MenuItem*)i );
  }
  
  const Fl_Menu_Item* find_item(const char *name) {
    return (Fl_Menu_Item*) ((fltk3::Menu_*)_p)->find_item(name);
  }
  
  const Fl_Menu_Item* find_item(Fl_Callback *cb) {
    return (Fl_Menu_Item*) ((fltk3::Menu_*)_p)->find_item( (fltk3::Callback*)cb );
  }
  
  int find_index(const char *name) const {
    return ((fltk3::Menu_*)_p)->find_index(name);
  }
  
  int find_index(const Fl_Menu_Item *item) const {
    return ((fltk3::Menu_*)_p)->find_index( (fltk3::MenuItem*)item );
  }
  
  int find_index(Fl_Callback *cb) const {
    return ((fltk3::Menu_*)_p)->find_index( (fltk3::Callback*)cb );
  }
                                           
  const Fl_Menu_Item* test_shortcut() {
    return (Fl_Menu_Item*) ((fltk3::Menu_*)_p)->test_shortcut();
  }
                                           
  void global() {
    ((fltk3::Menu_*)_p)->global();
  }
                                             
  const Fl_Menu_Item *menu() const {
    return (Fl_Menu_Item*)((fltk3::Menu_*)_p)->menu();
  }
  
  void menu(const Fl_Menu_Item *m) {
    ((fltk3::Menu_*)_p)->menu( (fltk3::MenuItem*)m );
  }
  
  void copy(const Fl_Menu_Item *m, void* user_data = 0) {
    ((fltk3::Menu_*)_p)->copy( (fltk3::MenuItem*)m, user_data );
  }
  
  int insert(int a, const char *b, int c, Fl_Callback *d, void *e=0, int f=0) {
    return ((fltk3::Menu_*)_p)->insert(a, b, c, (fltk3::Callback*)d, e, f);
  }
  
  int add(const char *a, int b, Fl_Callback *c, void *d=0, int e=0) {
    return ((fltk3::Menu_*)_p)->add(a, b, (fltk3::Callback*)c, d, e);
  }
  
  int add(const char* a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) {
    return ((fltk3::Menu_*)_p)->add(a, b, (fltk3::Callback*)c, d, e);
  }
  
  int insert(int index, const char* a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) {
    return ((fltk3::Menu_*)_p)->insert(index, a, b, (fltk3::Callback*)c, d, e);
  }
  
  int add(const char *a) {
    return ((fltk3::Menu_*)_p)->add(a);
  }
  
  int size() const {
    return ((fltk3::Menu_*)_p)->size();
  }
  
  void size(int W, int H) { 
    ((fltk3::Menu_*)_p)->size(W, H);
  }
  
  void clear() {
    ((fltk3::Menu_*)_p)->clear();
  }
  
  int clear_submenu(int index) {
    return ((fltk3::Menu_*)_p)->clear_submenu(index);
  }
  
  void replace(int a, const char *b) {
    ((fltk3::Menu_*)_p)->replace(a, b);
  }
  
  void remove(int a) {
    ((fltk3::Menu_*)_p)->remove(a);
  }
  
  void shortcut(int i, int s) {
    ((fltk3::Menu_*)_p)->shortcut(i, s);
  }
  
  void mode(int i,int fl) {
    ((fltk3::Menu_*)_p)->mode(i, fl);
  }
  
  int  mode(int i) const {
    return ((fltk3::Menu_*)_p)->mode(i);
  }
  
  const Fl_Menu_Item *mvalue() const {
    return (const Fl_Menu_Item*) ((const fltk3::Menu_*)_p)->mvalue();
  }
  
  int value() const {
    return ((fltk3::Menu_*)_p)->value();
  }
  
  int value(const Fl_Menu_Item *m) {
    return ((fltk3::Menu_*)_p)->value( (fltk3::MenuItem*)m );
  }
  
  int value(int i) {
    return ((fltk3::Menu_*)_p)->value(i);
  }
  
  const char *text() const {
    return ((fltk3::Menu_*)_p)->text();
  }
  
  const char *text(int i) const {
    return ((fltk3::Menu_*)_p)->text(i);
  }
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::Menu_*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::Menu_*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::Menu_*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::Menu_*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::Menu_*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::Menu_*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  
  Fl_Boxtype down_box() const {
    return fltk3::_3to1_boxtype( ((fltk3::Menu_*)_p)->down_box() );
  }
  
  void down_box(Fl_Boxtype b) {
    ((fltk3::Menu_*)_p)->down_box( fltk3::_1to3_boxtype(b) );
  }
  
  Fl_Color down_color() const {
    return fltk3::_3to1_color( ((fltk3::Menu_*)_p)->down_color() );
  }
  
  void down_color(unsigned c) {
    ((fltk3::Menu_*)_p)->down_color( fltk3::_1to3_color(c) );
  }
  
};

#endif

//
// End of "$Id$".
//
