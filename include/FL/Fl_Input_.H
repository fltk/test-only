//
// "$Id$"
//
// Input base class header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Input__H
#define Fl_Input__H

#include <fltk3/Input_.h>
#include "Fl_Widget.H"

#define FL_NORMAL_INPUT		0
#define FL_FLOAT_INPUT		1
#define FL_INT_INPUT		2
#define FL_HIDDEN_INPUT		3
#define FL_MULTILINE_INPUT	4
#define FL_SECRET_INPUT		5
#define FL_INPUT_TYPE		7
#define FL_INPUT_READONLY	8
#define FL_NORMAL_OUTPUT	(FL_NORMAL_INPUT | FL_INPUT_READONLY)
#define FL_MULTILINE_OUTPUT	(FL_MULTILINE_INPUT | FL_INPUT_READONLY)
#define FL_INPUT_WRAP		16
#define FL_MULTILINE_INPUT_WRAP	(FL_MULTILINE_INPUT | FL_INPUT_WRAP)
#define FL_MULTILINE_OUTPUT_WRAP (FL_MULTILINE_INPUT | FL_INPUT_READONLY | FL_INPUT_WRAP)


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Input_, Input_)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Input_, Input_)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Input_ : public Fl_Widget {

protected:

  int word_start(int i) const {
    return ((fltk3::Input__I*)_p)->word_start(i);
  }

  int word_end(int i) const {
    return ((fltk3::Input__I*)_p)->word_end(i);
  }

  int line_start(int i) const {
    return ((fltk3::Input__I*)_p)->line_start(i);
  }
  
  int line_end(int i) const {
    return ((fltk3::Input__I*)_p)->line_end(i);
  }

  void drawtext(int x, int y, int w, int h) {
    ((fltk3::Input__I*)_p)->drawtext(x, y, w, h);
  }

  int up_down_position(int a, int b=0) {
    return ((fltk3::Input__I*)_p)->up_down_position(a, b);
  }

  void handle_mouse(int a, int b, int c, int d, int e=0) {
    ((fltk3::Input__I*)_p)->handle_mouse(a, b, c, d, e);
  }

  int handletext(int a, int b, int c, int d, int e) {
    return ((fltk3::Input__I*)_p)->handletext(a, b, c, d, e);
  }

  void maybe_do_callback() {
    ((fltk3::Input__I*)_p)->maybe_do_callback();
  }

  int xscroll() const {
    return ((fltk3::Input__I*)_p)->xscroll();
  }

  int yscroll() const {
    return ((fltk3::Input__I*)_p)->yscroll();
  }

  void yscroll(int y) { 
    ((fltk3::Input__I*)_p)->yscroll(y);
  }

  int linesPerPage() {
    return ((fltk3::Input__I*)_p)->linesPerPage();
  }

public:

  FLTK3_WIDGET_VCALLS(Input_)
  
  Fl_Input_() { /* empty */ }
  
  Fl_Input_(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::Input__I(x, y, w, h, label);
    _p->wrapper(this);
  }

  int value(const char *v) {
    return ((fltk3::Input_*)_p)->value(v);
  }

  int value(const char *a, int b) {
    return ((fltk3::Input_*)_p)->value(a, b);
  }

  int static_value(const char *a) {
    return ((fltk3::Input_*)_p)->static_value(a);
  }

  int static_value(const char *a, int b) {
    return ((fltk3::Input_*)_p)->static_value(a, b);
  }

  const char* value() const {
    return ((fltk3::Input_*)_p)->value();
  }

  Fl_Char index(int i) const {
    return ((fltk3::Input_*)_p)->index(i);
  }

  int size() const {
    return ((fltk3::Input_*)_p)->size();
  }

  void size(int W, int H) { 
    ((fltk3::Input_*)_p)->size(W, H); 
  }

  int maximum_size() const {
    return ((fltk3::Input_*)_p)->maximum_size();
  }

  void maximum_size(int m) {
    ((fltk3::Input_*)_p)->maximum_size(m);
  }

  int position() const {
    return ((fltk3::Input_*)_p)->position();
  }

  int mark() const {
    return ((fltk3::Input_*)_p)->mark();
  }

  int position(int p, int m) {
    return ((fltk3::Input_*)_p)->position(p, m);
  }

  int position(int p) {
    return ((fltk3::Input_*)_p)->position(p);
  }

  int mark(int m) {
    return ((fltk3::Input_*)_p)->mark(m);
  }

  int replace(int a, int b, const char *c, int d=0) {
    return ((fltk3::Input_*)_p)->replace(a, b, c, d);
  }

  int cut() {
    return ((fltk3::Input_*)_p)->cut();
  }

  int cut(int n) {
    return ((fltk3::Input_*)_p)->cut(n);
  }

  int cut(int a, int b) {
    return ((fltk3::Input_*)_p)->cut(a, b);
  }

  int insert(const char* t, int l=0) {
    return ((fltk3::Input_*)_p)->insert(t, l);
  }

  int copy(int clipboard) {
    return ((fltk3::Input_*)_p)->copy(clipboard);
  }

  int undo() {
    return ((fltk3::Input_*)_p)->undo();
  }

  int copy_cuts() {
    return ((fltk3::Input_*)_p)->copy_cuts();
  }

  int shortcut() const {
    return ((fltk3::Input_*)_p)->shortcut();
  }

  void shortcut(int s) {
    ((fltk3::Input_*)_p)->shortcut(s);
  }

  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::Input_*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::Input_*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::Input_*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::Input_*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::Input_*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::Input_*)_p)->textcolor( fltk3::_1to3_color(col) );
  }

  Fl_Color cursor_color() const {
    return fltk3::_3to1_color( ((fltk3::Input_*)_p)->cursor_color() );
  }

  void cursor_color(Fl_Color n) {
    ((fltk3::Input_*)_p)->cursor_color( fltk3::_1to3_color(n) );
  }

  int input_type() const {
    return ((fltk3::Input_*)_p)->input_type();
  }

  void input_type(int t) { 
    ((fltk3::Input_*)_p)->input_type(t);
  }

  int readonly() const { 
    return ((fltk3::Input_*)_p)->readonly();
  }

  void readonly(int b) { 
    ((fltk3::Input_*)_p)->readonly(b);
  }

  int wrap() const { 
    return ((fltk3::Input_*)_p)->wrap();
  }

  void wrap(int b) { 
    ((fltk3::Input_*)_p)->wrap(b);
  }

  void tab_nav(int val) {
    ((fltk3::Input_*)_p)->tab_nav(val);
  }

  int tab_nav() const {
    return ((fltk3::Input_*)_p)->tab_nav();
  }
  
};


#endif 

//
// End of "$Id$".
//
