//
// "$Id: gl_start.cxx,v 1.10 2001/07/23 09:50:03 spitzak Exp $"
//
// OpenGL context routines for the Fast Light Tool Kit (FLTK).
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

// You MUST use gl_visual() to select the default visual before doing
// show() of any windows.  Mesa will crash if you try to use a visual
// not returned by glxChooseVisual.

// This does not work with Fl_Double_Window's!  It will try to draw
// into the front buffer.  Depending on the system this will either
// crash or do nothing (when pixmaps are being used as back buffer
// and GL is being done by hardware), work correctly (when GL is done
// with software, such as Mesa), or draw into the front buffer and
// be erased when the buffers are swapped (when double buffer hardware
// is being used)

#include <config.h>
#if HAVE_GL

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include "Fl_Gl_Choice.h"
#include <fltk/fl_draw.h>

#ifdef WIN32
static Fl_Gl_Choice* gl_choice;
#endif

int fl_gl_visual(int mode) {
  Fl_Gl_Choice *c = Fl_Gl_Choice::find(mode);
  if (!c) return 0;
#ifdef WIN32
  gl_choice = c;
#else
  fl_visual = c->vis;
  fl_colormap = c->colormap;
#endif
  return 1;
}

//extern FL_API int fl_clip_state_number; // in fl_rect.C

static GLContext context;
//static int clip_state_number=-1;
static int pw, ph;

Region XRectangleRegion(int x, int y, int w, int h); // in fl_rect.C

void gl_start() {
  if (!context) {
#ifdef WIN32
    if (!gl_choice) fl_gl_visual(0);
    context = fl_create_gl_context(Fl_Window::current(), gl_choice);
#else
    context = fl_create_gl_context(fl_visual);
#endif
  }
  fl_set_gl_context(Fl_Window::current(), context);
#ifndef WIN32
  glXWaitX();
#endif
  if (pw != Fl_Window::current()->w() || ph != Fl_Window::current()->h()) {
    pw = Fl_Window::current()->w();
    ph = Fl_Window::current()->h();
    glLoadIdentity();
    glViewport(0, 0, pw, ph);
    glOrtho(0, pw, 0, ph, -1, 1);
    glDrawBuffer(GL_FRONT);
  }
//  if (clip_state_number != fl_clip_state_number) {
//    clip_state_number = fl_clip_state_number;
    int x, y, w, h;
    if (fl_clip_box(0, 0, Fl_Window::current()->w(), Fl_Window::current()->h(),
		    x, y, w, h)) {
      fl_clip_region(XRectangleRegion(x,y,w,h));
      glScissor(x, Fl_Window::current()->h()-(y+h), w, h);
      glEnable(GL_SCISSOR_TEST);
    } else {
      glDisable(GL_SCISSOR_TEST);
    }
//  }
}

void gl_finish() {
  glFlush();
#ifndef WIN32
  glXWaitGL();
#endif
}

#endif

//
// End of "$Id: gl_start.cxx,v 1.10 2001/07/23 09:50:03 spitzak Exp $".
//
