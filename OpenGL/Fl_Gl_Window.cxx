//
// "$Id: Fl_Gl_Window.cxx,v 1.28 2001/07/24 16:25:08 clip Exp $"
//
// OpenGL window code for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#if HAVE_GL

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/x.h>
#include "Fl_Gl_Choice.h"
#include <fltk/Fl_Gl_Window.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////

// The symbol SWAP_TYPE defines what is in the back buffer after doing
// a glXSwapBuffers().

// The OpenGl documentation says that the contents of the backbuffer
// are "undefined" after glXSwapBuffers().  However, if we know what
// is in the backbuffers then we can save a good deal of time.  For
// this reason you can define some symbols to describe what is left in
// the back buffer.

// Having not found any way to determine this from glx (or wgl) I have
// resorted to letting the user specify it with an environment variable,
// GL_SWAP_TYPE, it should be equal to one of these symbols:

// contents of back buffer after glXSwapBuffers():
#define UNDEFINED 1 	// anything
#define SWAP 2		// former front buffer (same as unknown)
#define COPY 3		// unchanged
#define NODAMAGE 4	// unchanged even by X expose() events

static char SWAP_TYPE; // 0 = determine it from environment variable

////////////////////////////////////////////////////////////////

bool Fl_Gl_Window::can_do(int a) {
  return Fl_Gl_Choice::find(a) != 0;
}

void Fl_Gl_Window::create() {
  if (!gl_choice) {
    gl_choice = Fl_Gl_Choice::find(mode_);
    if (!gl_choice) {Fl::error("Insufficient GL support"); return;}
  }
#ifndef WIN32
  Fl_X::create(this, gl_choice->vis, gl_choice->colormap, -1);
  //if (overlay && overlay != this) ((Fl_Gl_Window*)overlay)->show();
#else
  Fl_Window::create();
#endif
}

void Fl_Gl_Window::invalidate() {
  valid(0);
#ifndef WIN32
  if (overlay) ((Fl_Gl_Window*)overlay)->valid(0);
#endif
}

bool Fl_Gl_Window::mode(int m) {
  if (m == mode_) return false;
  mode_ = m;
  // destroy context and g:
  if (shown()) {
    bool reshow = visible_r();
    destroy();
    gl_choice = 0;
    if (reshow) {create(); show();}
  }
  return true;
}

#define NON_LOCAL_CONTEXT 0x80000000

void Fl_Gl_Window::make_current() {
  if (!context_) {
    mode_ &= ~NON_LOCAL_CONTEXT;
    context_ = fl_create_gl_context(this, gl_choice);
    valid(0);
  }
  fl_set_gl_context(this, context_);
#if defined(WIN32) && USE_COLORMAP
  if (fl_palette) {
    fl_window = fl_xid(this); fl_gc = GetDC(fl_window);
    SelectPalette(fl_gc, fl_palette, FALSE);
    RealizePalette(fl_gc);
  }
#endif // USE_COLORMAP
  current_ = this;
}

void Fl_Gl_Window::ortho() {
// Alpha NT seems to have a broken OpenGL that does not like negative coords:
#ifdef _M_ALPHA
  glLoadIdentity();
  glViewport(0, 0, w(), h());
  glOrtho(0, w(), 0, h(), -1, 1);
#else
  GLint v[2];
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, v);
  glLoadIdentity();
  glViewport(w()-v[0], h()-v[1], v[0], v[1]);
  glOrtho(w()-v[0], w(), h()-v[1], h(), -1, 1);
#endif
}

void Fl_Gl_Window::swap_buffers() {
#ifdef WIN32
#if HAVE_GL_OVERLAY
  // Do not swap the overlay, to match GLX:
  wglSwapLayerBuffers(Fl_X::i(this)->private_dc, WGL_SWAP_MAIN_PLANE);
#else
  SwapBuffers(Fl_X::i(this)->private_dc);
#endif
#else
  glXSwapBuffers(fl_display, fl_xid(this));
#endif
}

void Fl_Gl_Window::draw_swap() {
  draw();
  if (!(mode_ & FL_NO_AUTO_SWAP)) swap_buffers();
}

#if HAVE_GL_OVERLAY && defined(WIN32)
int fl_overlay_depth = 0;
bool fl_overlay;
#endif

#define SGI320_BUG 1

void Fl_Gl_Window::flush() {
  uchar save_valid = valid_;

#if HAVE_GL_OVERLAY && defined(WIN32)

#if SGI320_BUG
  bool fixcursor = false; // for fixing the SGI 320 bug
#endif

  // Draw into hardware overlay planes if they are damaged:
  if (overlay && overlay != this
      && (damage()&(FL_DAMAGE_OVERLAY|FL_DAMAGE_EXPOSE) || !save_valid)) {
#if SGI320_BUG
    // SGI 320 messes up overlay with user-defined cursors:
    if (Fl_X::i(this)->cursor && Fl_X::i(this)->cursor != fl_default_cursor) {
      fixcursor = true; // make it restore cursor later
      SetCursor(0);
    }
#endif
    fl_set_gl_context(this, (GLContext)overlay);
    if (fl_overlay_depth)
      wglRealizeLayerPalette(Fl_X::i(this)->private_dc, 1, TRUE);
    glDisable(GL_SCISSOR_TEST);
    if (!(mode_ & FL_NO_ERASE_OVERLAY)) glClear(GL_COLOR_BUFFER_BIT);
    fl_overlay = true;
    draw_overlay();
    fl_overlay = false;
    valid(save_valid);
    wglSwapLayerBuffers(Fl_X::i(this)->private_dc, WGL_SWAP_OVERLAY1);
    // if only the overlay was damaged we are done, leave main layer alone:
    if (damage() == FL_DAMAGE_OVERLAY) {
#if SGI320_BUG
      if (fixcursor) SetCursor(Fl_X::i(this)->cursor);
#endif
      return;
    }
  }
#endif

  make_current();

  if (mode_ & FL_DOUBLE) {

    glDrawBuffer(GL_BACK);

    if (!SWAP_TYPE) {
      SWAP_TYPE = UNDEFINED;
      const char* c = getenv("GL_SWAP_TYPE");
      if (c) {
	if (!strcmp(c,"COPY")) SWAP_TYPE = COPY;
	else if (!strcmp(c, "NODAMAGE")) SWAP_TYPE = NODAMAGE;
      }
    }

    if (SWAP_TYPE == NODAMAGE) {

      // don't draw if only overlay damage or expose events:
      if ((damage()&~(FL_DAMAGE_OVERLAY|FL_DAMAGE_EXPOSE)) || !save_valid)
	draw_swap();
      else
	swap_buffers();

    } else if (SWAP_TYPE == COPY) {

      // don't draw if only the overlay is damaged:
      if (damage() != FL_DAMAGE_OVERLAY || !save_valid)
	draw_swap();
      else
	swap_buffers();

    } else { // SWAP_TYPE == UNDEFINED

      // If we are faking the overlay, use CopyPixels to act like
      // SWAP_TYPE == COPY.  Otherwise overlay redraw is way too slow.
      if (overlay == this) {
	// don't draw if only the overlay is damaged:
	if (damage1_ || damage() != FL_DAMAGE_OVERLAY || !save_valid) draw();
	// we use a seperate context for the copy because rasterpos must be 0
	// and depth test needs to be off:
	static GLContext ortho_context = 0;
	static Fl_Gl_Window* ortho_window = 0;
	if (!ortho_context) {
	  ortho_context = fl_create_gl_context(this, gl_choice);
	  save_valid = 0;
	}
	fl_set_gl_context(this, ortho_context);
	if (!save_valid || ortho_window != this) {
	  ortho_window = this;
	  glDisable(GL_DEPTH_TEST);
	  glReadBuffer(GL_BACK);
	  glDrawBuffer(GL_FRONT);
	  glLoadIdentity();
	  glViewport(0, 0, w(), h());
	  glOrtho(0, w(), 0, h(), -1, 1);
	  glRasterPos2i(0,0);
	}
	glCopyPixels(0,0,w(),h(),GL_COLOR);
	make_current(); // set current context back to draw overlay
	damage1_ = 0;

      } else {

	damage1_ = damage();
	set_damage(~0); draw_swap();

      }

    }

    if (overlay==this) { // fake overlay in front buffer
      glDrawBuffer(GL_FRONT);
      draw_overlay();
      glDrawBuffer(GL_BACK);
      glFlush();
    }

  } else {	// single-buffered context is simpler:

    draw();
    if (overlay == this) draw_overlay();
    glFlush();
  }

#if HAVE_GL_OVERLAY && defined(WIN32) && SGI320_BUG
  if (fixcursor) SetCursor(Fl_X::i(this)->cursor);
#endif
  valid(1);
}

void Fl_Gl_Window::layout() {
  valid(0);
#ifndef WIN32
  if (!resizable() && overlay && overlay != this)
    ((Fl_Gl_Window*)overlay)->resize(0,0,w(),h());
#endif
  Fl_Window::layout();
}

void Fl_Gl_Window::context(void* v, bool destroy_flag) {
  if (context_ && !(mode_&NON_LOCAL_CONTEXT)) fl_delete_gl_context(context_);
  context_ = (GLContext)v;
  if (destroy_flag) mode_ &= ~NON_LOCAL_CONTEXT;
  else mode_ |= NON_LOCAL_CONTEXT;
}    

void Fl_Gl_Window::destroy() {
  context(0);
#if HAVE_GL_OVERLAY
  if (overlay && overlay != this) {
#ifdef WIN32
    fl_delete_gl_context((GLContext)overlay);
#endif
    overlay = 0;
  }
#endif
  Fl_Window::destroy();
}

Fl_Gl_Window::~Fl_Gl_Window() {
  destroy();
}

void Fl_Gl_Window::init() {
  end(); // we probably don't want any children
  mode_ = FL_RGB | FL_DEPTH | FL_DOUBLE;
  context_ = 0;
  gl_choice = 0;
  overlay = 0;
  damage1_ = 0;
}

void Fl_Gl_Window::draw_overlay() {}

#endif

//
// End of "$Id: Fl_Gl_Window.cxx,v 1.28 2001/07/24 16:25:08 clip Exp $".
//
