//
// "$Id: Fl_Gl_Window.H,v 1.17 2001/07/10 19:26:53 clip Exp $"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_Gl_Window_H
#define Fl_Gl_Window_H

#include "Fl_Window.H"

#ifndef GLContext
typedef void* GLContext; // actually a GLXContext or HGLDC
#endif

class Fl_Gl_Choice; // structure to hold result of glXChooseVisual

enum {
  FL_NO_AUTO_SWAP = 1024,
  FL_NO_ERASE_OVERLAY = 2048
};

class FL_GL_API Fl_Gl_Window : public Fl_Window {

public:

  void create();
  void flush();
  void destroy();
  void layout();

  char valid() const {return valid_;}
  void valid(char i) {valid_ = i;}
  void invalidate();

  int mode() const {return mode_;}
  int mode(int a);
  static int can_do(int);
  int can_do() {return can_do(mode_);}

  void* context() const {return context_;}
  void context(void*, int destroy_flag = false);
  void make_current();
  void swap_buffers();
  void ortho();

  int can_do_overlay();
  void redraw_overlay();
  void hide_overlay();
  void make_overlay_current();

  ~Fl_Gl_Window();
  Fl_Gl_Window(int W, int H, const char *l=0) : Fl_Window(W,H,l) {init();}
  Fl_Gl_Window(int X, int Y, int W, int H, const char *l=0)
    : Fl_Window(X,Y,W,H,l) {init();}

private:

  int mode_;
  Fl_Gl_Choice *gl_choice;
  GLContext context_;
  char valid_;
  char damage1_; // damage() of back buffer
  virtual void draw_overlay();
  void init();

  void *overlay;
  void make_overlay();
  friend class _Fl_Gl_Overlay;

  void draw_swap();
};

#endif

//
// End of "$Id: Fl_Gl_Window.H,v 1.17 2001/07/10 19:26:53 clip Exp $".
//
