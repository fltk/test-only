/
// "$Id$
/
// OpenGL header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// You must include this instead of GL/gl.h to get the Microsof
// APIENTRY stuff included (from <windows.h>) prior to the OpenG
// header files
/
// This file also provides "missing" OpenGL functions, an
// gl_start() and gl_finish() to allow OpenGL to be used in any windo
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef FL_gl_
#  define FL_gl_

#  include "Enumerations.H" // for color name
#  ifdef WIN3
#    include <windows.h
#  endi
#  ifndef APIENTR
#    if defined(__CYGWIN__
#      define APIENTRY __attribute__ ((__stdcall__)
#    els
#      define APIENTR
#    endi
#  endi

#  ifdef __APPLE_
#    include <OpenGL/gl.h
#  els
#    include <GL/gl.h
#  endi

FL_EXPORT void gl_start()
FL_EXPORT void gl_finish()

FL_EXPORT void gl_color(Fl_Color)
inline void gl_color(int c) {gl_color((Fl_Color)c);} // back compatabilit

FL_EXPORT void gl_rect(int x,int y,int w,int h)
inline void gl_rectf(int x,int y,int w,int h) {glRecti(x,y,x+w,y+h);

FL_EXPORT void gl_font(int fontid, int size)
FL_EXPORT int  gl_height()
FL_EXPORT int  gl_descent()
FL_EXPORT double gl_width(const char *)
FL_EXPORT double gl_width(const char *, int n)
FL_EXPORT double gl_width(uchar)

FL_EXPORT void gl_draw(const char*)
FL_EXPORT void gl_draw(const char*, int n)
FL_EXPORT void gl_draw(const char*, int x, int y)
FL_EXPORT void gl_draw(const char*, float x, float y)
FL_EXPORT void gl_draw(const char*, int n, int x, int y)
FL_EXPORT void gl_draw(const char*, int n, float x, float y)
FL_EXPORT void gl_draw(const char*, int x, int y, int w, int h, Fl_Align)
FL_EXPORT void gl_measure(const char*, int& x, int& y)

FL_EXPORT void gl_draw_image(const uchar *, int x,int y,int w,int h, int d=3, int ld=0)

#endif // !FL_gl_

/
// End of "$Id$"
/
