//
// "$Id: gl_start.cxx,v 1.14 2004/01/19 21:38:41 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// This does not work with DoubleBufferWindow!  It will try to draw
// into the front buffer.  Depending on the system this will either
// crash or do nothing (when pixmaps are being used as back buffer
// and GL is being done by hardware), work correctly (when GL is done
// with software, such as Mesa), or draw into the front buffer and
// be erased when the buffers are swapped (when double buffer hardware
// is being used)

#include <config.h>
#if HAVE_GL

#include <fltk/Window.h>
#include "GlChoice.h"
#include <fltk/draw.h>
#include <fltk/visual.h>
#include <fltk/gl.h>
using namespace fltk;

#ifdef _WIN32
static GlChoice* gl_choice;
#endif

/*! Use OpenGL and the glX library to choose the visual. This is a
  somewhat different algorithim from fltk::visual(). On some X servers
  OpenGL will crash if the visual is not selected with this.
*/

bool fltk::glVisual(int mode) {
  GlChoice *c = GlChoice::find(mode);
  if (!c) return false;
#ifdef _WIN32
  gl_choice = c;
#else
  xvisual = c->vis;
  xcolormap = c->colormap;
#endif
  return true;
}

static GLContext context;
//static int clip_state_number=-1;
static int pw, ph;

Region XRectangleRegion(int x, int y, int w, int h);

void fltk::glstart() {
  if (!context) {
#ifdef _WIN32
    if (!gl_choice) glVisual(0);
    context = create_gl_context(Window::current(), gl_choice);
#else
    context = create_gl_context(xvisual);
#endif
  }
  set_gl_context(Window::current(), context);
#ifndef _WIN32
  glXWaitX();
#endif
  if (pw != Window::current()->w() || ph != Window::current()->h()) {
    pw = Window::current()->w();
    ph = Window::current()->h();
    glLoadIdentity();
    glViewport(0, 0, pw, ph);
    glOrtho(0, pw, 0, ph, -1, 1);
    glDrawBuffer(GL_FRONT);
  }
//  if (clip_state_number != clip_state_number) {
//    clip_state_number = clip_state_number;
    int x, y, w, h;
    if (clip_box(0, 0, Window::current()->w(), Window::current()->h(),
		 x, y, w, h)) {
      clip_region(XRectangleRegion(x,y,w,h));
      glScissor(x, Window::current()->h()-(y+h), w, h);
      glEnable(GL_SCISSOR_TEST);
    } else {
      glDisable(GL_SCISSOR_TEST);
    }
//  }
}

void fltk::glfinish() {
  glFlush();
#ifndef _WIN32
  glXWaitGL();
#endif
}

#endif

//
// End of "$Id: gl_start.cxx,v 1.14 2004/01/19 21:38:41 spitzak Exp $".
//
