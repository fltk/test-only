//
// "$Id: Fl_Gl_Choice.cxx,v 1.9 2001/02/20 06:59:49 spitzak Exp $"
//
// OpenGL visual selection code for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#if HAVE_GL

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include <stdlib.h>

#include "Fl_Gl_Choice.H"

static Fl_Gl_Choice *first;
GLXContext fl_first_context;

// this assummes one of the two arguments is zero:
// We keep the list system in Win32 to stay compatible and interpret
// the list later...
Fl_Gl_Choice* Fl_Gl_Choice::find(int mode, const int* alist) {

  Fl_Gl_Choice* g;
  for (g = first; g; g = g->next)
    if (g->mode == mode && g->alist == alist) 
      return g;

#ifdef WIN32    

  // Replacement for ChoosePixelFormat() that finds one with an overlay
  // if possible:
  if (!fl_gc) fl_gc = GetDC(0); fl_window = 0; // Why are we messing with the root window?
  int pixelFormat = 0;
  PIXELFORMATDESCRIPTOR chosen_pfd;
  for (int i = 1; ; i++) {
    PIXELFORMATDESCRIPTOR pfd;
    if (!DescribePixelFormat(fl_gc, i, sizeof(pfd), &pfd)) break;
    // continue if it does not satisfy our requirements:
    if (~pfd.dwFlags & (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL)) continue;
    if (pfd.iPixelType != ((mode&FL_INDEX)?1:0)) continue;
    if ((mode & FL_ALPHA) && !pfd.cAlphaBits) continue;
    if ((mode & FL_ACCUM) && !pfd.cAccumBits) continue;
    if ((!(mode & FL_DOUBLE)) != (!(pfd.dwFlags & PFD_DOUBLEBUFFER))) continue;
    if ((!(mode & FL_STEREO)) != (!(pfd.dwFlags & PFD_STEREO))) continue;
    if ((mode & FL_DEPTH) && !pfd.cDepthBits) continue;
    if ((mode & FL_STENCIL) && !pfd.cStencilBits) continue;
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

#else

  int list[32];

  if (!alist) {
    alist = list;
    int n = 0;
    if (mode & FL_INDEX) {
      list[n++] = GLX_BUFFER_SIZE;
      list[n++] = 8; // glut tries many sizes, but this should work...
    } else {
      list[n++] = GLX_RGBA;
      list[n++] = GLX_GREEN_SIZE;
      list[n++] = (mode & FL_RGB8) ? 8 : 1;
      if (mode & FL_ALPHA) {
	list[n++] = GLX_ALPHA_SIZE;
	list[n++] = 1;
      }
      if (mode & FL_ACCUM) {
	list[n++] = GLX_ACCUM_GREEN_SIZE;
	list[n++] = 1;
	if (mode & FL_ALPHA) {
	  list[n++] = GLX_ACCUM_ALPHA_SIZE;
	  list[n++] = 1;
	}
      }
    }
    if (mode & FL_DOUBLE) {
      list[n++] = GLX_DOUBLEBUFFER;
    }
    if (mode & FL_DEPTH) {
      list[n++] = GLX_DEPTH_SIZE; list[n++] = 1;
    }
    if (mode & FL_STENCIL) {
      list[n++] = GLX_STENCIL_SIZE; list[n++] = 1;
    }
    if (mode & FL_STEREO) {
      list[n++] = GLX_STEREO;
    }
#if defined(GLX_VERSION_1_1) && defined(GLX_SGIS_multisample)
    if (mode & FL_MULTISAMPLE) {
      list[n++] = GLX_SAMPLES_SGIS;
      list[n++] = 4; // value Glut uses
    }
#endif
    list[n] = 0;
  }
    
  fl_open_display();
  XVisualInfo *vis = glXChooseVisual(fl_display, fl_screen, (int*)alist);
  if (!vis) {
# if defined(GLX_VERSION_1_1) && defined(GLX_SGIS_multisample)
    if (mode&FL_MULTISAMPLE) return find(mode&~FL_MULTISAMPLE,0);
# endif
    return 0;
  }

#endif

  g = new Fl_Gl_Choice;
  g->mode = mode;
  g->alist = alist;
  g->next = first;
  first = g;

#ifdef WIN32
  g->pfd = chosen_pfd;
  g->pixelFormat = pixelFormat;
#else
  g->vis = vis;
  if (/*MaxCmapsOfScreen(ScreenOfDisplay(fl_display,fl_screen))==1 && */
      vis->visualid == fl_visual->visualid &&
      !getenv("MESA_PRIVATE_CMAP"))
    g->colormap = fl_colormap;
  else
    g->colormap = XCreateColormap(fl_display, RootWindow(fl_display,fl_screen),
				  vis->visual, AllocNone);
#endif

  return g;
}

#ifdef WIN32

HDC fl_private_dc(const Fl_Window* w, Fl_Gl_Choice *g) {
  Fl_X* i = Fl_X::i(w);
  if (!i->private_dc) {
    // Hmmm.  This seems wrong.  I believe this will actually do the opposite
    // of what is intended and use a common DC.  FLTK now uses private DCs by
    // default for all windows anyway.
    // i->private_dc = GetDCEx(i->xid, 0, DCX_CACHE);
    i->private_dc = GetDC(i->xid);
    SetPixelFormat(i->private_dc, g->pixelFormat, &g->pfd);
#if USE_COLORMAP
    if (fl_palette) SelectPalette(i->private_dc, fl_palette, FALSE);
#endif
  }
  return i->private_dc;
}

#endif

static GLXContext cached_context;

static const Fl_Window* cached_window;

void fl_set_gl_context(const Fl_Window* w, GLXContext c) {
  if (c != cached_context || w != cached_window) {
    cached_context = c;
    cached_window = w;
#ifdef WIN32
    wglMakeCurrent(Fl_X::i(w)->private_dc, c);
#else
    glXMakeCurrent(fl_display, fl_xid(w), c);
#endif
  }
}

void fl_no_gl_context() {
  if (cached_context) {
#ifdef WIN32
    wglMakeCurrent(0, 0);
#else
    glXMakeCurrent(fl_display, 0, 0);
#endif
  }
  cached_context = 0;
  cached_window = 0;
}

#endif

//
// End of "$Id: Fl_Gl_Choice.cxx,v 1.9 2001/02/20 06:59:49 spitzak Exp $".
//
