//
// "$Id: gl.h,v 1.12 2004/03/05 08:17:01 spitzak Exp $"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef gl_draw_H
#define gl_draw_H

#include "Color.h"
#include "Flags.h"

#ifdef _WIN32
# include <windows.h>
# include <GL/gl.h>
# undef DELETE
# undef ERROR
# undef IN
# undef OUT
# undef POINT
# undef far
# undef max
# undef min
# undef near
#elif defined(__APPLE__)
# include <OpenGL/gl.h>
#else
# ifndef APIENTRY
#  if defined(__CYGWIN__)
#   define APIENTRY __attribute__ ((__stdcall__))
#  else
#   define APIENTRY
#  endif
# endif
# include <GL/gl.h>
#endif

namespace fltk {

struct Font;

/*! \addtogroup opengl
  \ingroup drawing
  \{ */
FL_GL_API void glstart();
FL_GL_API void glfinish();

FL_GL_API void glsetcolor(Color);

FL_GL_API void glstrokerect(int x,int y,int w,int h);
inline void glfillrect(int x,int y,int w,int h) {glRecti(x,y,x+w,y+h);}

FL_GL_API void glsetfont(Font* f, float size);
FL_GL_API float glgetascent();
FL_GL_API float glgetdescent();
FL_GL_API float glgetwidth(const char *);
FL_GL_API float glgetwidth(const char *, int n);

FL_GL_API void gldrawtext(const char*);
FL_GL_API void gldrawtext(const char*, int n);
FL_GL_API void gldrawtext(const char*, float x, float y, float z = 0);
FL_GL_API void gldrawtext(const char*, int n, float x, float y, float z = 0);

FL_GL_API void gldrawimage(const uchar *, int x,int y,int w,int h, int d=3, int ld=0);
/*! \} */

}
#endif

//
// End of "$Id: gl.h,v 1.12 2004/03/05 08:17:01 spitzak Exp $".
//
