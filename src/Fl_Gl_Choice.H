/
// "$Id$
/
// OpenGL definitions for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2001 by Bill Spitzak and others
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
// Please report all bugs and problems to "fltk-bugs@easysw.com"
/

// Internal interface to set up OpenGL
/
// A "Fl_Gl_Choice" is created from an OpenGL mode and holds informatio
// necessary to create a window (on X) and to create an OpenGL "context
// (on both X and Win32)
/
// fl_create_gl_context takes a window (necessary only on Win32) and a
// Fl_Gl_Choice and returns a new OpenGL context. All contexts shar
// display lists with each other
/
// On X another fl_create_gl_context is provided to create it for an
// X visual
/
// fl_set_gl_context makes the given OpenGL context current and make
// it draw into the passed window. It tracks the current one contex
// to avoid calling the context switching code when the same contex
// is used, though it is a mystery to me why the GLX/WGL librarie
// don't do this themselves..
/
// fl_no_gl_context clears that cache so the next fl_set_gl_context i
// guaranteed to work
/
// fl_delete_gl_context destroys the context
/
// This code is used by Fl_Gl_Window, gl_start(), and gl_visual(

#ifndef Fl_Gl_Choice_
#define Fl_Gl_Choice_

// Warning: whatever GLContext is defined to must take exactly the sam
// space in a structure as a void*!!
#ifdef WIN3
#  include <FL/gl.h
#  define GLContext HGLR
#elif defined(__APPLE__
#  include <OpenGL/gl.h
#  include <AGL/agl.h
#  define GLContext AGLContex
#els
#  include <GL/glx.h
#  define GLContext GLXContex
#endi

// Describes crap needed to create a GLContext
class Fl_Gl_Choice 
  int mode
  const int *alist
  Fl_Gl_Choice *next
public
#ifdef WIN3
  int pixelformat;	// the visual to us
  PIXELFORMATDESCRIPTOR pfd; // some wgl calls need this thin
#elif defined(__APPLE__
  AGLPixelFormat pixelformat
#els
  XVisualInfo *vis;	// the visual to us
  Colormap colormap;	// a colormap for that visua
#endi
  // Return one of these structures for a given gl mode
  // The second argument is a glX attribute list, and is used if mode i
  // zero.  This is not supported on Win32
  static Fl_Gl_Choice *find(int mode, const int *)
}

class Fl_Window

#ifdef WIN3

GLContext fl_create_gl_context(Fl_Window*, const Fl_Gl_Choice*, int layer=0)

#elif defined(__APPLE__

GLContext fl_create_gl_context(Fl_Window*, const Fl_Gl_Choice*, int layer=0)

#els

GLContext fl_create_gl_context(XVisualInfo* vis)

static inlin
GLContext fl_create_gl_context(Fl_Window*, const Fl_Gl_Choice* g) 
  return fl_create_gl_context(g->vis)


#endi

void fl_set_gl_context(Fl_Window*, GLContext)
void fl_no_gl_context()
void fl_delete_gl_context(GLContext)

#endi

/
// End of "$Id$"
/
