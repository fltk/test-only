//
// "$Id: GlChoice.h,v 1.3 2004/03/25 18:13:17 spitzak Exp $"
//
// OpenGL definitions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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

// Internal interface to set up OpenGL.
//
// A "GlChoice" is created from an OpenGL mode and holds information
// necessary to create a window (on X) and to create an OpenGL "context"
// (on both X and Win32).
//
// create_gl_context takes a window (necessary only on Win32) and an
// GlChoice and returns a new OpenGL context. All contexts share
// display lists with each other.
//
// On X another create_gl_context is provided to create it for any
// X visual.
//
// set_gl_context makes the given OpenGL context current and makes
// it draw into the passed window. It tracks the current one context
// to avoid calling the context switching code when the same context
// is used, though it is a mystery to me why the GLX/WGL libraries
// don't do this themselves...
//
// no_gl_context clears that cache so the next fl_set_gl_context is
// guaranteed to work.
//
// delete_gl_context destroys the context.
//
// This code is used by GlWindow, glStart(), and glVisual()

#ifndef fltk_GlChoice_h
#define fltk_GlChoice_h

#include <config.h>
#include <fltk/Window.h> // force CreatedWindow to be defined by x.h
#include <fltk/x.h>

// Warning: whatever GLContext is defined to must take exactly the same
// space in a structure as a void*!!!
#ifdef _WIN32
# include <fltk/gl.h>
# define GLContext HGLRC
#else
# define Window XWindow
# include <GL/glx.h>
# undef Window
# define GLContext GLXContext
#endif

namespace fltk {

// Describes crap needed to create a GLContext.
class GlChoice {
  int mode;
  GlChoice *next;
public:
#ifdef _WIN32
  int pixelFormat;	// the visual to use
  PIXELFORMATDESCRIPTOR pfd; // some wgl calls need this thing
#else
  XVisualInfo *vis;	// the visual to use
  Colormap colormap;	// a colormap for that visual
#endif
  // Return one of these structures for a given gl mode, or null if impossible:
  static GlChoice *find(int mode);
};

class Window;

#ifdef _WIN32

GLContext create_gl_context(const Window*, const GlChoice*, int layer=0);

#else

GLContext create_gl_context(XVisualInfo* vis);

static inline
GLContext create_gl_context(const Window*, const GlChoice* g) {
  return create_gl_context(g->vis);
}

#endif

void set_gl_context(const Window*, GLContext);
void no_gl_context();
void delete_gl_context(GLContext);

}

#endif

//
// End of "$Id: GlChoice.h,v 1.3 2004/03/25 18:13:17 spitzak Exp $".
//
