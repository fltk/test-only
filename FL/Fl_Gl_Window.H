//
// "$Id$"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#ifndef Fl_Gl_Window_H
#define Fl_Gl_Window_H

#include <fltk3/GlWindow.h>
#include "Fl_Window.H"

#ifndef GLContext
typedef void* GLContext; // actually a GLXContext or HGLDC
#endif


class Fl_Gl_Choice; // structure to hold result of glXChooseVisual


class FL_EXPORT Fl_Gl_Window : public Fl_Window {

public:
  
  Fl_Gl_Window() { /* empty */ }

  Fl_Gl_Window(int X, int Y, int W, int H, const char *l=0) {
    _p = new fltk3::GlWindow(X, Y, W, H, l);
    _p->wrapper(this);
  }
  
  Fl_Gl_Window(int W, int H, const char *l=0) {
    _p = new fltk3::GlWindow(W, H, l);
    _p->wrapper(this);
  }
  
  void show(int a, char **b) {
    ((fltk3::GlWindow*)_p)->show(a, b);
  }
  
  void flush() {
    ((fltk3::GlWindow*)_p)->flush();
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT(GlWindow, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)

  FLTK3_WRAPPER_VCALLS_OBJECT(GlWindow, hide(), hide(), Hide)

  FLTK3_WRAPPER_VCALLS_OBJECT(GlWindow, show(), show(), Show)

  FLTK3_WRAPPER_VCALLS_OBJECT_INT(GlWindow, handle(int event), handle(event), Handle)
    
  char valid() const {
    return ((fltk3::GlWindow*)_p)->valid();
  }
  
  void valid(char v) {
    ((fltk3::GlWindow*)_p)->valid(v);
  }
  
  void invalidate() {
    ((fltk3::GlWindow*)_p)->invalidate();
  }
  
  char context_valid() const {
    return ((fltk3::GlWindow*)_p)->context_valid();
  }
  
  void context_valid(char v) {
    ((fltk3::GlWindow*)_p)->context_valid(v);
  }
  
  static int can_do(int m) {
    return fltk3::GlWindow::can_do(m);
  }
  
  static int can_do(const int *m) {
    return fltk3::GlWindow::can_do(m);
  }
  
  int can_do() {
    return ((fltk3::GlWindow*)_p)->can_do();
  }
  
  Fl_Mode mode() const {
    return (Fl_Mode)((fltk3::GlWindow*)_p)->mode();
  }
  
  int mode(int a) {
    return ((fltk3::GlWindow*)_p)->mode(a);
  }
  
  int mode(const int *a) {
    return ((fltk3::GlWindow*)_p)->mode(a);
  }
  
  void* context() const {
    return ((fltk3::GlWindow*)_p)->context();
  }
  
  void context(void *a, int destroy_flag = 0) {
    ((fltk3::GlWindow*)_p)->context(a, destroy_flag);
  }
  
  void make_current() {
    ((fltk3::GlWindow*)_p)->make_current();
  }
  
  void swap_buffers() {
    ((fltk3::GlWindow*)_p)->swap_buffers();
  }
  
  void ortho() {
    ((fltk3::GlWindow*)_p)->ortho();
  }
  
  int can_do_overlay() {
    return ((fltk3::GlWindow*)_p)->can_do_overlay();
  }
  
  void redraw_overlay() {
    ((fltk3::GlWindow*)_p)->redraw_overlay();
  }
  
  void hide_overlay() {
    ((fltk3::GlWindow*)_p)->hide_overlay();
  }
  
  void make_overlay_current() {
    ((fltk3::GlWindow*)_p)->make_overlay_current();
  }
  
  virtual Fl_Gl_Window* as_gl_window() {return this;}

protected:

  FLTK3_WRAPPER_VCALLS_OBJECT(GlWindow, draw(), draw(), Draw)
  
};

#endif

//
// End of "$Id$".
//
