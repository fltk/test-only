//
// "$Id$"
//
// Browser header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Browser_H
#define Fl_Browser_H

#include <fltk3/Browser.h>
#include "Fl_Browser_.H"
#include "Fl_Image.H"

struct FL_BLINE;


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Browser, Browser)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Browser, Browser)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Browser : public Fl_Browser_ {

protected:

  FLTK3_WRAPPER_VCALLS_WIDGET_RET(void *, Browser_I, item_first() const, item_first(), BrowserItemFirst) 
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(void *, Browser_I, item_next(void *item) const, item_next(item), BrowserItemNext);
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(void *, Browser_I, item_prev(void *item) const, item_prev(item), BrowserItemPrev); 
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(void*, Browser_I, item_last() const, item_last(), BrowserItemLast)  
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Browser_I, item_height(void *item) const, item_height(item), BrowserItemHeight)  
  FLTK3_WRAPPER_VCALLS_WIDGET(Browser_I, item_select(void *item, int val=1), item_select(item, val), BrowserItemSelect)  
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Browser_I, item_selected(void *item) const, item_selected(item), BrowserItemSelected)
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Browser_I, item_width(void *item) const, item_width(item), BrowserItemWidth)
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Browser_I, item_quick_height(void *item) const, item_quick_height(item), BrowserItemQuickHeight)
  FLTK3_WRAPPER_VCALLS_WIDGET(Browser_I, item_draw(void *item, int X, int Y, int W, int H) const, item_draw(item, X, Y, W, H), BrowserItemDraw)
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(const char *, Browser_I, item_text(void *item) const, item_text(item), BrowserItemText)
  FLTK3_WRAPPER_VCALLS_WIDGET(Browser_I, item_swap(void *a, void *b), item_swap(a, b), BrowserItemSwap)
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(void *, Browser_I, item_at(int index) const, item_at(index), BrowserItemAt)
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Browser_I, full_width() const, full_width(), BrowserFullWidth)
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Browser_I, full_height() const, full_height(), BrowserFullHeight)
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Browser_I, incr_height() const, incr_height(), BrowserIncrHeight)

  FL_BLINE* find_line(int line) const  { return 0; }
  FL_BLINE* _remove(int line) { return 0; }
  void insert(int line, FL_BLINE* item) {}
  int lineno(void *item) const { return 0; }
  void swap(FL_BLINE *a, FL_BLINE *b) {}

public:

  FLTK3_WIDGET_VCALLS(Fl_Browser, Browser)

  void remove(int line) {
    ((fltk3::Browser*)_p)->remove(line);
  }
  
  void add(const char* newtext, void* d = 0) {
    ((fltk3::Browser*)_p)->add(newtext, d);
  }
  
  void insert(int line, const char* newtext, void* d = 0) {
    ((fltk3::Browser*)_p)->insert(line, newtext, d);
  }
  
  void move(int to, int from) {
    ((fltk3::Browser*)_p)->move(to, from);
  }
  
  int  load(const char* filename) {
    return ((fltk3::Browser*)_p)->load(filename);
  }
  
  void swap(int a, int b) {
    ((fltk3::Browser*)_p)->swap(a, b);
  }
  
  void clear() {
    ((fltk3::Browser*)_p)->clear();
  }
  
  int size() const { 
    return ((fltk3::Browser*)_p)->size();
  }

  void size(int W, int H) { 
    ((fltk3::Browser*)_p)->size(W, H);
  }
  
  int topline() const {
    return ((fltk3::Browser*)_p)->topline();
  }
  
  enum Fl_Line_Position { TOP, BOTTOM, MIDDLE };
  
  void lineposition(int line, Fl_Line_Position pos) {
    ((fltk3::Browser*)_p)->lineposition(line, (fltk3::Browser::LinePosition)pos);
  }
  
  void topline(int line) { 
    ((fltk3::Browser*)_p)->topline(line);
  }
  
  void bottomline(int line) { 
    ((fltk3::Browser*)_p)->bottomline(line);
  }
  
  void middleline(int line) { 
    ((fltk3::Browser*)_p)->middleline(line);
  }
  
  int select(int line, int val=1) {
    return ((fltk3::Browser*)_p)->select(line, val);
  }
  
  int selected(int line) const {
    return ((fltk3::Browser*)_p)->selected(line);
  }
  
  void show(int line) {
    ((fltk3::Browser*)_p)->show(line);
  }
  
  void hide(int line) {
    ((fltk3::Browser*)_p)->hide(line);
  }
  
  int visible(int line) const {
    return ((fltk3::Browser*)_p)->visible(line);
  }
  
  int value() const {
    return ((fltk3::Browser*)_p)->value();
  }
  
  void value(int line) { 
    ((fltk3::Browser*)_p)->value(line);
  }
  
  const char* text(int line) const {
    return ((fltk3::Browser*)_p)->text(line);
  }
  
  void text(int line, const char* newtext) {
    ((fltk3::Browser*)_p)->text(line, newtext);
  }
  
  void* data(int line) const {
    return ((fltk3::Browser*)_p)->data(line);
  }
  
  void data(int line, void* d) {
    ((fltk3::Browser*)_p)->data(line, d);
  }

  Fl_Browser() { /* empty */ }
  
  Fl_Browser(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::Browser_I(_ctor_dx(X), _ctor_dy(Y), W, H, L);
    _p->wrapper(this);
  }
  
  char format_char() const { 
    return ((fltk3::Browser*)_p)->format_char();
  }
  
  void format_char(char c) { 
    ((fltk3::Browser*)_p)->format_char(c);
  }
  
  char column_char() const { 
    return ((fltk3::Browser*)_p)->column_char();
  }
  
  void column_char(char c) { 
    ((fltk3::Browser*)_p)->column_char(c);
  }
  
  const int* column_widths() const { 
    return ((fltk3::Browser*)_p)->column_widths();
  }
  
  void column_widths(const int* arr) {
    ((fltk3::Browser*)_p)->column_widths(arr);
  }
  
  int displayed(int line) const { 
    return ((fltk3::Browser*)_p)->displayed(line);
  }
  
  void make_visible(int line) {
    ((fltk3::Browser*)_p)->make_visible(line);
  }
  
  void icon(int line, Fl_Image* icon) {
    ((fltk3::Browser*)_p)->icon( line, fltk3::_1to3_image(icon) );
  }
  
  Fl_Image* icon(int line) const {
    return fltk3::_3to1_image( ((fltk3::Browser*)_p)->icon(line) );
  }
  
  void remove_icon(int line) {
    ((fltk3::Browser*)_p)->remove_icon(line);
  }
};

#endif

//
// End of "$Id$".
//
