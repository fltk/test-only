//
// "$Id: gl.h,v 1.5 2002/06/17 16:03:52 spitzak Exp $"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef gl_draw_H
#define gl_draw_H

#include "Fl_Color.h"
#include "Fl_Flags.h"
#include "Fl_Font.h"

#ifdef _WIN32
# include <windows.h>
#endif
#ifndef APIENTRY
# if defined(__CYGWIN__)
#  define APIENTRY __attribute__ ((__stdcall__))
# else
#  define APIENTRY
# endif
#endif

#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

FL_GL_API void gl_start();
FL_GL_API void gl_finish();

FL_GL_API void gl_color(Fl_Color);

FL_GL_API void gl_rect(int x,int y,int w,int h);
inline void gl_rectf(int x,int y,int w,int h) {glRecti(x,y,x+w,y+h);}

FL_GL_API void gl_font(Fl_Font f, int size);
FL_GL_API int  gl_height();
FL_GL_API int  gl_descent();
FL_GL_API int  gl_width(const char *);
FL_GL_API int  gl_width(const char *, int n);

FL_GL_API void gl_draw(const char*);
FL_GL_API void gl_draw(const char*, int n);
FL_GL_API void gl_draw(const char*, double x, double y, double z = 0);
FL_GL_API void gl_draw(const char*, int n, double x, double y, double z = 0);

FL_GL_API void gl_draw_image(const uchar *, int x,int y,int w,int h, int d=3, int ld=0);

#endif

//
// End of "$Id: gl.h,v 1.5 2002/06/17 16:03:52 spitzak Exp $".
//
