//
// "$Id: Fl_Gl_Choice.H,v 1.3 2000/04/24 05:09:41 carl Exp $"
//
// OpenGL definitions for the Fast Light Tool Kit (FLTK).
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

// This is a private fltk header file.  The structures and functions
// described here are subject to change in future versions.
//
// An Fl_Gl_Choice structure stores all the system-dependent stuff needed
// to implement an fltk OpenGL "mode".  For X this is a visual and colormap,
// and for Win32 this is a PixelFormat.
//
// find() returns a new Fl_Gl_Choice, or an old one if it has already
// been called before with the same mode.  If the system cannot do that
// choice a null pointer is returned.
//
// The second argument to find() is a glX attribute list.  This is
// currently used to get an overlay visual on X.  This argument is
// ignored on win32 or other non-glX systems.

#ifndef Fl_Gl_Choice_H
#define Fl_Gl_Choice_H

#ifdef WIN32
# include <windows.h>
# include <FL/gl.h>
# define GLXContext HGLRC
#else
# include <GL/glx.h>
#endif

// one of these structures is returned:
class Fl_Gl_Choice {
  int mode;
  const int *alist;
  Fl_Gl_Choice *next;
public:
#ifdef WIN32
  PIXELFORMATDESCRIPTOR pfd;
  int pixelFormat;
#else
  XVisualInfo *vis;
  Colormap colormap;
#endif
  static Fl_Gl_Choice *find(int mode, const int * = 0);
};

extern GLXContext fl_first_context; // used to make all contexts share
extern GLXContext fl_current_context;

class Fl_Window;

#ifdef WIN32
// This must be called before fl_set_gl_context works:
HDC fl_private_dc(Fl_Window*, Fl_Gl_Choice *g);
#endif

void fl_set_gl_context(Fl_Window*, GLXContext);
void fl_no_gl_context();

#endif

//
// End of "$Id: Fl_Gl_Choice.H,v 1.3 2000/04/24 05:09:41 carl Exp $".
//
