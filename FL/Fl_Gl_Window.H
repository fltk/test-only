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

#if 0 // FIXME: 123

  void show();
  void show(int a, char **b) {Fl_Window::show(a,b);}
  void flush();
  void hide();
  void resize(int,int,int,int);
  int handle(int);
  
#endif
  
  char valid() const {
    return ((fltk3::GlWindow*)_p)->valid();
  }
  
  void show() const { // FIXME: 123 - this is a virtual function!
    ((fltk3::GlWindow*)_p)->show();
  }
  
#if 0 // FIXME: 123

  void valid(char v) {if (v) valid_f_ |= 1; else valid_f_ &= 0xfe;}
  void invalidate();
  char context_valid() const {return valid_f_ & 2;}
  void context_valid(char v) {if (v) valid_f_ |= 2; else valid_f_ &= 0xfd;}
  static int can_do(int m) {return can_do(m,0);}
  static int can_do(const int *m) {return can_do(0, m);}
  int can_do() {return can_do(mode_,alist);}
  Fl_Mode mode() const {return (Fl_Mode)mode_;}
  int mode(int a) {return mode(a,0);}
  int mode(const int *a) {return mode(0, a);}
  void* context() const {return context_;}
  void context(void*, int destroy_flag = 0);
  void make_current();
  void swap_buffers();
  void ortho();
  int can_do_overlay();
  void redraw_overlay();
  void hide_overlay();
  void make_overlay_current();
  virtual Fl_Gl_Window* as_gl_window() {return this;}
  ~Fl_Gl_Window();
  Fl_Gl_Window(int W, int H, const char *l=0) : Fl_Window(W,H,l) {init();}

#endif

protected:

#if 0 // FIXME: 123

  virtual void draw();
  
#endif
  
};

#endif

//
// End of "$Id$".
//
