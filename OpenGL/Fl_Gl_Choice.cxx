//
// "$Id$"
//
// OpenGL visual selection code for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#if HAVE_GL

#include "GlChoice.h"
#include <fltk/visual.h>
#include <stdlib.h>

using namespace fltk;

static GlChoice* first;

GlChoice* GlChoice::find(int mode) {
  GlChoice* g;
  
  for (g = first; g; g = g->next) if (g->mode == mode) return g;

#ifdef _WIN32

  // Replacement for ChoosePixelFormat() that finds one with an overlay
  // if possible:
  HDC dc = getDC();
  int pixelFormat = 0;
  PIXELFORMATDESCRIPTOR chosen_pfd;
  for (int i = 1; ; i++) {
    PIXELFORMATDESCRIPTOR pfd;
    if (!DescribePixelFormat(dc, i, sizeof(pfd), &pfd)) break;
    // continue if it does not satisfy our requirements:
    if (~pfd.dwFlags & (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL)) continue;
    if (pfd.iPixelType != ((mode&INDEXED_COLOR)?1:0)) continue;
    if ((mode & ALPHA_BUFFER) && !pfd.cAlphaBits) continue;
    if ((mode & ACCUM_BUFFER) && !pfd.cAccumBits) continue;
    if ((!(mode & DOUBLE_BUFFER)) != (!(pfd.dwFlags & PFD_DOUBLEBUFFER))) continue;
    if ((!(mode & STEREO)) != (!(pfd.dwFlags & PFD_STEREO))) continue;
    if ((mode & DEPTH_BUFFER) && !pfd.cDepthBits) continue;
    if ((mode & STENCIL_BUFFER) && !pfd.cStencilBits) continue;
    // see if better than the one we have already:
    if (pixelFormat) {
      // offering overlay is better:
      if (!(chosen_pfd.bReserved & 15) && (pfd.bReserved & 15)) {}
      // otherwise more bit planes is better:
      else if (chosen_pfd.cColorBits < pfd.cColorBits) {}
      else continue;
    }
    pixelFormat = i;
    chosen_pfd = pfd;
  }
  if (!pixelFormat) return 0;

#elif defined(__APPLE__)

  // warning: the Quartz version should probably use Core GL (CGL) instead of AGL
  const int *blist;
  int list[32];
   
  int n = 0;
  if (mode & INDEXED_COLOR) {
    list[n++] = AGL_BUFFER_SIZE;
    list[n++] = 8; // glut tries many sizes, but this should work...
  } else {
    list[n++] = AGL_RGBA;
    list[n++] = AGL_GREEN_SIZE;
    list[n++] = (mode & RGB24_COLOR) ? 8 : 1;
    if (mode & ALPHA_BUFFER) {
      list[n++] = AGL_ALPHA_SIZE;
      list[n++] = (mode & RGB24_COLOR) ? 8 : 1;
    }
    if (mode & ACCUM_BUFFER) {
      list[n++] = AGL_ACCUM_GREEN_SIZE;
      list[n++] = 1;
      if (mode & ALPHA_BUFFER) {
        list[n++] = AGL_ACCUM_ALPHA_SIZE;
        list[n++] = 1;
      }
    }
  }
  if (mode & DOUBLE_BUFFER) {
    list[n++] = AGL_DOUBLEBUFFER;
  }
  if (mode & DEPTH_BUFFER) {
    list[n++] = AGL_DEPTH_SIZE; list[n++] = 24;
  }
  if (mode & STENCIL_BUFFER) {
    list[n++] = AGL_STENCIL_SIZE; list[n++] = 1;
  }
# ifdef AGL_STEREO
  if (mode & STEREO) {
    list[n++] = AGL_STEREO;
  }
# endif
  list[n] = AGL_NONE;
  blist = list;

  open_display();
  AGLPixelFormat fmt = aglChoosePixelFormat(NULL, 0, (GLint*)blist);
  if (!fmt) return 0;

#else

  int list[32];
  int n = 0;
  if (mode & INDEXED_COLOR) {
    list[n++] = GLX_BUFFER_SIZE;
    list[n++] = 8; // glut tries many sizes, but this should work...
  } else {
    list[n++] = GLX_RGBA;
    list[n++] = GLX_GREEN_SIZE;
    const int bits = (mode & RGB24_COLOR) ? 8 : 1;
    list[n++] = bits;
    if (mode & ALPHA_BUFFER) {
      list[n++] = GLX_ALPHA_SIZE;
      list[n++] = bits;
    }
    if (mode & ACCUM_BUFFER) {
      list[n++] = GLX_ACCUM_GREEN_SIZE;
      list[n++] = bits;
      if (mode & ALPHA_BUFFER) {
	list[n++] = GLX_ACCUM_ALPHA_SIZE;
	list[n++] = bits;
      }
    }
  }
  if (mode & DOUBLE_BUFFER) {
    list[n++] = GLX_DOUBLEBUFFER;
  }
  if (mode & DEPTH_BUFFER) {
    list[n++] = GLX_DEPTH_SIZE; list[n++] = 1;
  }
  if (mode & STENCIL_BUFFER) {
    list[n++] = GLX_STENCIL_SIZE; list[n++] = 1;
  }
  if (mode & STEREO) {
    list[n++] = GLX_STEREO;
  }
#if defined(GLX_VERSION_1_1) && defined(GLX_SGIS_multisample)
  if (mode & MULTISAMPLE) {
    list[n++] = GLX_SAMPLES_SGIS;
    list[n++] = 4; // value Glut uses
  }
#endif
  list[n] = 0;
    
  open_display();
  XVisualInfo* vis = glXChooseVisual(xdisplay, xscreen, list);
  if (!vis) {
# if defined(GLX_VERSION_1_1) && defined(GLX_SGIS_multisample)
    if (mode&MULTISAMPLE) return find(mode&~MULTISAMPLE);
# endif
    return 0;
  }

#endif

  g = new GlChoice;
  g->mode = mode;
  g->next = first;
  first = g;

#ifdef _WIN32
  g->pixelFormat = pixelFormat;
  g->pfd = chosen_pfd;
#elif defined(__APPLE__)
  g->pixelformat = fmt;
#else
  g->vis = vis;

  if (/*MaxCmapsOfScreen(ScreenOfDisplay(xdisplay,xscreen))==1 && */
      vis->visualid == xvisual->visualid &&
      !getenv("MESA_PRIVATE_CMAP"))
    g->colormap = xcolormap;
  else
    g->colormap = XCreateColormap(xdisplay, RootWindow(xdisplay,xscreen),
				  vis->visual, AllocNone);
#endif

  return g;
}

static GLContext first_context;

#if USE_X11

GLContext fltk::create_gl_context(XVisualInfo* vis) {
  GLContext context = glXCreateContext(xdisplay, vis, first_context, 1);
  if (!first_context) first_context = context;
  return context;
}

#elif defined(_WIN32)

GLContext fltk::create_gl_context(const Window* window, const GlChoice* g, int layer) {
  CreatedWindow* i = CreatedWindow::find(window);
  SetPixelFormat(i->dc, g->pixelFormat, &g->pfd);
  GLContext context =
    layer ? wglCreateLayerContext(i->dc, layer) : wglCreateContext(i->dc);
  if (context) {
    if (first_context) wglShareLists(first_context, context);
    else first_context = context;
  }
  return context;
}

#elif defined(__APPLE__)

// warning: the Quartz version should probably use Core GL (CGL) instead of AGL
GLContext fltk::create_gl_context(const Window* window, const GlChoice* g, int layer) {
  GLContext context;
  context = aglCreateContext(g->pixelformat, first_context);
  if (!context) return 0;
  if (!first_context) first_context = context;
  return context;
}

#endif

GLContext fl_current_glcontext;
static const Window* cached_window;

void fltk::set_gl_context(const Window* window, GLContext context) {
  if (context != fl_current_glcontext || window != cached_window) {
    fl_current_glcontext = context;
    cached_window = window;
#if USE_X11
    glXMakeCurrent(xdisplay, xid(window), context);
#elif defined(_WIN32)
    wglMakeCurrent(CreatedWindow::find(window)->dc, context);
#elif defined(__APPLE__)
  // warning: the Quartz version should probably use Core GL (CGL) instead of AGL
    if ( window->parent() ) { //: resize our GL buffer rectangle
      //++ this gets called a lot if we have more than one GL buffer... .
      fltk::Rectangle r(*window);
      for (Widget* p = window->parent(); p->parent(); p = p->parent())
	r.move(p->x(), p->y());
      Rect wrect; GetWindowPortBounds( xid(window), &wrect );
      GLint rect[] = { r.x(), wrect.bottom-wrect.top-r.b(), r.w(), r.h() };
      aglSetInteger( context, AGL_BUFFER_RECT, rect );
      aglEnable( context, AGL_BUFFER_RECT );
    }
    aglSetDrawable(context, GetWindowPort( xid(window) ) );
    aglSetCurrentContext(context);
#endif
  }
}

void fltk::no_gl_context() {
#if USE_X11
  glXMakeCurrent(xdisplay, 0, 0);
#elif defined(_WIN32)
  wglMakeCurrent(0, 0);
#elif defined(__APPLE__)
  // warning: the Quartz version should probably use Core GL (CGL) instead of AGL
  if (fl_current_glcontext) aglSetCurrentContext(0);
#endif
  fl_current_glcontext = 0;
  cached_window = 0;
}

void fltk::delete_gl_context(GLContext context) {
  if (fl_current_glcontext == context) no_gl_context();
  if (context != first_context) {
#if USE_X11
    glXDestroyContext(xdisplay, context);
#elif defined(_WIN32)
    wglDeleteContext(context);
#elif defined(__APPLE__)
    // warning: the Quartz version should probably use Core GL (CGL) instead of AGL
    aglSetCurrentContext( NULL );
    aglSetDrawable( context, NULL );
    aglDestroyContext( context );
#endif
  }
}

#endif

//
// End of "$Id$".
//
