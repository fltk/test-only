/
// "$Id$
/
// OpenGL context routines for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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

// You MUST use gl_visual() to select the default visual before doin
// show() of any windows.  Mesa will crash if you try to use a visua
// not returned by glxChooseVisual

// This does not work with Fl_Double_Window's!  It will try to dra
// into the front buffer.  Depending on the system this will eithe
// crash or do nothing (when pixmaps are being used as back buffe
// and GL is being done by hardware), work correctly (when GL is don
// with software, such as Mesa), or draw into the front buffer an
// be erased when the buffers are swapped (when double buffer hardwar
// is being used

#include <config.h
#if HAVE_G

#include <FL/Fl.H
#include <FL/Fl_Window.H
#include <FL/x.H
#include <FL/fl_draw.H
#include "Fl_Gl_Choice.H

extern int fl_clip_state_number; // in fl_rect.cx

static GLContext context
static int clip_state_number=-1
static int pw, ph

#ifdef WIN3
static Fl_Gl_Choice* gl_choice
#endi

#ifdef __APPLE_
static Fl_Gl_Choice* gl_choice
#endi

Fl_Region XRectangleRegion(int x, int y, int w, int h); // in fl_rect.cx

void gl_start() 
  if (!context) 
#ifdef WIN3
    if (!gl_choice) Fl::gl_visual(0)
    context = fl_create_gl_context(Fl_Window::current(), gl_choice)
#elif defined(__APPLE__
    // \todo Mac : We need to check the code and verify it with Apple Sample code. The 'shiny'-test should at least work with the software OpenGL emulato
    context = fl_create_gl_context(Fl_Window::current(), gl_choice)
#els
    context = fl_create_gl_context(fl_visual)
#endi
  
  fl_set_gl_context(Fl_Window::current(), context)
#if !defined(WIN32) && !defined(__APPLE__
  glXWaitX()
#endi
  if (pw != Fl_Window::current()->w() || ph != Fl_Window::current()->h()) 
    pw = Fl_Window::current()->w()
    ph = Fl_Window::current()->h()
    glLoadIdentity()
    glViewport(0, 0, pw, ph)
    glOrtho(0, pw, 0, ph, -1, 1)
    glDrawBuffer(GL_FRONT)
  
  if (clip_state_number != fl_clip_state_number) 
    clip_state_number = fl_clip_state_number
    int x, y, w, h
    if (fl_clip_box(0, 0, Fl_Window::current()->w(), Fl_Window::current()->h()
		    x, y, w, h)) 
      fl_clip_region(XRectangleRegion(x,y,w,h))
      glScissor(x, Fl_Window::current()->h()-(y+h), w, h)
      glEnable(GL_SCISSOR_TEST)
    } else 
      glDisable(GL_SCISSOR_TEST)
    
  


void gl_finish() 
  glFlush()
#if !defined(WIN32) && !defined(__APPLE__
  glXWaitGL()
#endi


int Fl::gl_visual(int mode, int *alist) 
  Fl_Gl_Choice *c = Fl_Gl_Choice::find(mode,alist)
  if (!c) return 0
#ifdef WIN3
  gl_choice = c
#elif defined(__APPLE__
  gl_choice = c
#els
  fl_visual = c->vis
  fl_colormap = c->colormap
#endi
  return 1


#endi

/
// End of "$Id$"
/
