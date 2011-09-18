//
// "$Id$"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
// FLTK 123 wrapper finished
//  - we only wrap those functions that have version specific arguments
//
// You must include this instead of GL/gl.h to get the Microsoft
// APIENTRY stuff included (from <windows.h>) prior to the OpenGL
// header files.
//
// This file also provides "missing" OpenGL functions, and
// gl_start() and gl_finish() to allow OpenGL to be used in any window
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

#ifndef FL_gl_H
#define FL_gl_H

#include <fltk3gl/gl.h>
#include "Enumerations.H" // for color names

#  ifdef __APPLE__
#    include <OpenGL/gl.h>
#  else
#    include <GL/gl.h>
#  endif

// void gl_start();
// void gl_finish();
// void gl_rect(int x,int y,int w,int h);
// void gl_rectf(int x,int y,int w,int h) {glRecti(x,y,x+w,y+h);}
// void gl_font(int fontid, int size);
// int  gl_height();
// int  gl_descent();
// double gl_width(const char *);
// double gl_width(const char *, int n);
// double gl_width(uchar);
// void gl_draw(const char*);
// void gl_draw(const char*, int n);
// void gl_draw(const char*, int x, int y);
// void gl_draw(const char*, float x, float y);
// void gl_draw(const char*, int n, int x, int y);
// void gl_draw(const char*, int n, float x, float y);
// void gl_measure(const char*, int& x, int& y);
// void gl_draw_image(const uchar *, int x,int y,int w,int h, int d=3, int ld=0);
// inline void gl_color(int c);

// inline void gl_color(Fl_Color i) { gl_color(fltk3::_1to3_color(i)); }

inline void gl_draw(const char *s, int x, int y, int w, int h, Fl_Align a) {
  gl_draw(s, x, y, w, h, fltk3::_1to3_align(a));
}

#ifdef __APPLE__
// void gl_texture_pile_height(int max);
// int gl_texture_pile_height();
#endif

#endif // !FL_gl_H

//
// End of "$Id$".
//
