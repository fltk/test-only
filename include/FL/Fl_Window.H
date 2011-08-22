//
// "$Id$"
//
// Window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Window_H
#define Fl_Window_H

#include <fltk3/Wrapper.h>
#include <fltk3/Window.h>

#include "Fl_Group.H"

#define FL_WINDOW 0xF0		///< window type id all subclasses have type() >= this
#define FL_DOUBLE_WINDOW 0xF1   ///< double window type id

class Fl_X;

class FL_EXPORT Fl_Window : public Fl_Group {

protected:
  
  void flush() {
    ((fltk3::Window*)_p)->flush();
  }
  
  void force_position(int force) {
    ((fltk3::Window*)_p)->force_position(force);
  }
  
  int force_position() const {
    return ((fltk3::Window*)_p)->force_position();
  }
  
public:

  FLTK3_WRAPPER_VCALLS_OBJECT(Window, draw_overlay(), draw_overlay(), DrawOverlay)
  
  Fl_Window() { /* empty */ }
  
  Fl_Window(int w, int h, const char* label = 0) {
    _p = new fltk3::Window(w, h, label);
    _p->wrapper(this);
  }
  
  Fl_Window(int x, int y, int w, int h, const char* label = 0) {
    _p = new fltk3::Window(x, y, w, h, label);
    _p->wrapper(this);
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Window, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Window, hide(), hide(), Hide)
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Window, show(), show(), Show)
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Window, draw(), draw(), Draw)
  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Window, handle(int event), handle(event), Handle)
  
  void border(int b) {
    ((fltk3::Window*)_p)->border(b);
  }
  
  void clear_border() {
    ((fltk3::Window*)_p)->clear_border();
  }
  
  unsigned int border() const {
    return ((fltk3::Window*)_p)->border();
  }
  
  void set_override() {
    ((fltk3::Window*)_p)->set_override();
  }
  
  unsigned int override() const {
    return ((fltk3::Window*)_p)->override();
  }
  
  void set_modal() {
    ((fltk3::Window*)_p)->set_modal();
  }
  
  unsigned int modal() const {
    return ((fltk3::Window*)_p)->modal();
  }
  
  void set_non_modal() {
    ((fltk3::Window*)_p)->set_non_modal();
  }
  
  unsigned int non_modal() const {
    return ((fltk3::Window*)_p)->non_modal();
  }
  
  void set_menu_window() {
    ((fltk3::Window*)_p)->set_menu_window();
  }
  
  unsigned int menu_window() const {
    return ((fltk3::Window*)_p)->menu_window();
  }
  
  void set_tooltip_window() {
    ((fltk3::Window*)_p)->set_tooltip_window();
  }
  
  unsigned int tooltip_window() const {
    return ((fltk3::Window*)_p)->tooltip_window();
  }
  
  void hotspot(int x, int y, int offscreen = 0) {
    ((fltk3::Window*)_p)->hotspot(x, y, offscreen);
  }
  
  void hotspot(const Fl_Widget *w, int offscreen = 0) {
    ((fltk3::Window*)_p)->hotspot(fltk3::_1to3_widget((Fl_Widget*)w), offscreen);
  }
  
  void hotspot(const Fl_Widget& p, int offscreen = 0) {
    ((fltk3::Window*)_p)->hotspot(*fltk3::_1to3_widget((Fl_Widget*)&p), offscreen);
  }
  
  void free_position() {
    ((fltk3::Window*)_p)->free_position();
  }
  
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0, int g=0) {
    ((fltk3::Window*)_p)->size_range(a, b, c, d, e, f, g);
  }
  
  const char* label() const {
    return ((fltk3::Window*)_p)->label();
  }
  
  const char* iconlabel() const	{
    return ((fltk3::Window*)_p)->iconlabel();
  }
  
  void label(const char *t) {
    ((fltk3::Window*)_p)->label(t);
  }
  
  void iconlabel(const char *t) {
    ((fltk3::Window*)_p)->iconlabel(t);
  }
  
  void label(const char* a, const char* b) {
    ((fltk3::Window*)_p)->label(a, b);
  }
  
  void copy_label(const char* a) {
    ((fltk3::Window*)_p)->copy_label(a);
  }
  
  static void default_xclass(const char *c) {
    fltk3::Window::default_xclass(c);
  }
  
  static const char *default_xclass() {
    return fltk3::Window::default_xclass();
  }
  
  const char* xclass() const {
    return ((fltk3::Window*)_p)->xclass();
  }
  
  void xclass(const char* c) {
    ((fltk3::Window*)_p)->xclass(c);
  }
  
  const void* icon() const {
    return ((fltk3::Window*)_p)->icon();
  }
  
  void icon(const void * ic) {
    ((fltk3::Window*)_p)->icon(ic);
  }
  
  int shown() {
    return ((fltk3::Window*)_p)->shown();
  }
  
  void show(int argc, char **argv) {
    ((fltk3::Window*)_p)->show(argc, argv);
  }
  
  void fullscreen() {
    ((fltk3::Window*)_p)->fullscreen();
  }
  
  void fullscreen_off(int x, int y, int w, int h) {
    ((fltk3::Window*)_p)->fullscreen_off(x, y, w, h);
  }
  
  void iconize() {
    ((fltk3::Window*)_p)->iconize();
  }
    
  int x_root() const {
    return ((fltk3::Window*)_p)->y_root();
  }
  
  int y_root() const {
    return ((fltk3::Window*)_p)->y_root();
  }
  
  static Fl_Window *current() {
    return fltk3::_3to1_window( fltk3::Window::current() );
  }
  
  void make_current() {
    ((fltk3::Window*)_p)->make_current();
  }
  
  virtual Fl_Window* as_window() { return this; }
  
  void cursor(Fl_Cursor c, Fl_Color a=FL_BLACK, Fl_Color b=FL_WHITE) {
    ((fltk3::Window*)_p)->cursor( fltk3::_1to3_cursor(c), fltk3::_1to3_color(a), fltk3::_1to3_color(b));
  }
  
  void default_cursor(Fl_Cursor c, Fl_Color a=FL_BLACK, Fl_Color b=FL_WHITE) {
    ((fltk3::Window*)_p)->default_cursor(fltk3::_1to3_cursor(c), fltk3::_1to3_color(a), fltk3::_1to3_color(b));
  }
  
  static void default_callback(Fl_Window *w, void* v) {
    fltk3::Window::default_callback(fltk3::_1to3_window(w), v);
  }
  
  int decorated_w() {
    return ((fltk3::Window*)_p)->decorated_w();
  }
  
  int decorated_h() {
    return ((fltk3::Window*)_p)->decorated_h();
  }

};

#endif

//
// End of "$Id$".
//
