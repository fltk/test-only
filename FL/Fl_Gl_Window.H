//
// "$Id: Fl_Gl_Window.H,v 1.14 2000/06/10 04:10:18 carl Exp $"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
#ifndef Fl_Gl_Window_H
#define Fl_Gl_Window_H

#include "Fl_Window.H"

class FL_GL_API Fl_Gl_Choice; // structure to hold result of glXChooseVisual

class FL_GL_API Fl_Gl_Window : public Fl_Window {

  int mode_;
  const int* alist;
  Fl_Gl_Choice* g;
  void* context; // actually a GLXContext
  char valid_;
  char damage1_; // damage() of back buffer
  void init();

  void *overlay;
  void make_overlay();
  friend class _Fl_Gl_Overlay;

  static int can_do(int, const int *);
  int mode(int, const int *);

protected:

  virtual void create();
  virtual void flush();
  virtual void destroy();

public:

  virtual void draw_overlay();
  virtual void layout();

  char valid() const {return valid_;}
  void valid(char i) {valid_ = i;}
  void invalidate();

  static int can_do(int i) {return can_do(i, 0);}
  static int can_do(const int *i) {return can_do(0, i);}
  int can_do() {return can_do(mode_, alist);}
  Fl_Mode mode() const {return (Fl_Mode)mode_;}
  int mode(int a) {return mode(a, 0);}
  int mode(const int *a) {return mode(0, a);}

  int can_do_overlay();
  void redraw_overlay();
  void hide_overlay();

  void make_current();
  void make_overlay_current();
  void swap_buffers();
  void ortho();

  ~Fl_Gl_Window();
  Fl_Gl_Window(int W, int H, const char *l=0) : Fl_Window(W,H,l) {init();}
  Fl_Gl_Window(int X, int Y, int W, int H, const char *l=0)
    : Fl_Window(X,Y,W,H,l) {init();}
};

FL_GL_API int fl_gl_visual(int, int *alist=0);

#endif

//
// End of "$Id: Fl_Gl_Window.H,v 1.14 2000/06/10 04:10:18 carl Exp $".
//
