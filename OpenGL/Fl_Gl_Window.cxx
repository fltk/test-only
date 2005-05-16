//
// "$Id$"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <config.h>
#if HAVE_GL

#include "GlChoice.h"
#include <fltk/GlWindow.h>
#include <fltk/damage.h>
#include <fltk/error.h>
#include <fltk/visual.h>
#include <fltk/layout.h>
#include <fltk/run.h>
#include <stdlib.h>
#include <string.h>
using namespace fltk;

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
#define SWAP 1		// assumme garbage is in back buffer
#define USE_COPY 2	// use glCopyPixels to imitate COPY behavior (default)
#define COPY 3		// unchanged
#define NODAMAGE 4	// unchanged even by X expose() events

static char SWAP_TYPE = 0; // 0 = determine it from environment variable

////////////////////////////////////////////////////////////////

bool GlWindow::can_do(int a) {
  return GlChoice::find(a) != 0;
}

void GlWindow::create() {
  if (!gl_choice) {
    gl_choice = GlChoice::find(mode_);
    if (!gl_choice) {error("Insufficient GL support"); return;}
  }
#ifdef WIN32
  Window::create();
#elif USE_QUARTZ
  Window::create();
#else
  CreatedWindow::create(this, gl_choice->vis, gl_choice->colormap, -1);
  //if (overlay && overlay != this) ((GlWindow*)overlay)->show();
  fltk::flush(); glXWaitGL(); glXWaitX();
#endif
}

void GlWindow::invalidate() {
  valid(0);
#ifdef WIN32
  ;
#elif USE_QUARTZ
  ;
#else
  if (overlay) ((GlWindow*)overlay)->valid(0);
#endif
}

bool GlWindow::mode(int m) {
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

void GlWindow::make_current() {
#if USE_QUARTZ
  if (!gl_choice) {
    gl_choice = GlChoice::find(mode_);
    if (!gl_choice) {error("Insufficient GL support"); return;}
  }
#endif
  if (!context_) {
    mode_ &= ~NON_LOCAL_CONTEXT;
    context_ = create_gl_context(this, gl_choice);
    valid(0);
  }
  set_gl_context(this, context_);
}

void GlWindow::ortho() {
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

void GlWindow::swap_buffers() {
#ifdef _WIN32
#if USE_GL_OVERLAY
  // Do not swap the overlay, to match GLX:
  wglSwapLayerBuffers(CreatedWindow::find(this)->dc, WGL_SWAP_MAIN_PLANE);
#else
  SwapBuffers(CreatedWindow::find(this)->dc);
#endif
#elif USE_QUARTZ
  aglSwapBuffers((AGLContext)context_);
#else
  glXSwapBuffers(xdisplay, xid(this));
#endif
}

void GlWindow::draw_swap() {
  draw();
  if (!(mode_ & NO_AUTO_SWAP)) swap_buffers();
}

#if USE_GL_OVERLAY && defined(_WIN32)
int fl_overlay_depth = 0;
bool fl_overlay;
#endif

void GlWindow::flush() {
  uchar save_valid = valid_;
  uchar save_damage = damage();

  CreatedWindow* i = CreatedWindow::find(this);
#if USE_GL_OVERLAY && defined(_WIN32)
  if (save_damage == DAMAGE_OVERLAY && overlay && overlay != this && !i->region) goto OVERLAY_ONLY;
#endif

  make_current();

  if (mode_ & DOUBLE_BUFFER) {

    glDrawBuffer(GL_BACK);

    if (!SWAP_TYPE) {
      SWAP_TYPE = SWAP; //USE_COPY;
      const char* c = getenv("GL_SWAP_TYPE");
      if (c) switch (c[0]) {
      case 'U' : SWAP_TYPE = USE_COPY; break;
      case 'C' : SWAP_TYPE = COPY; break;
      case 'N' : SWAP_TYPE = NODAMAGE; break;
      default  : SWAP_TYPE = SWAP; break;
      }
    }

    if (SWAP_TYPE == NODAMAGE) {

      // don't draw if only overlay damage or expose events:
      if (save_damage != DAMAGE_OVERLAY || !save_valid)
	draw_swap();
      else
	swap_buffers();

    } else if (SWAP_TYPE == COPY) {

      // don't draw if only the overlay is damaged:
      if (save_damage != DAMAGE_OVERLAY || i->region || !save_valid)
	draw_swap();
      else
	swap_buffers();

    } else if (SWAP_TYPE == USE_COPY && overlay == this) {
      // If we are faking the overlay, use CopyPixels to act like
      // SWAP_TYPE == COPY.  Otherwise overlay redraw is way too slow.
      // don't draw if only the overlay is damaged:
      if (damage1_ || save_damage != DAMAGE_OVERLAY || i->region || !save_valid) draw();
      // we use a seperate context for the copy because rasterpos must be 0
      // and depth test needs to be off:
      static GLContext ortho_context = 0;
      static GlWindow* ortho_window = 0;
      if (!ortho_context) {
	ortho_context = create_gl_context(this, gl_choice);
	save_valid = 0;
      }
      set_gl_context(this, ortho_context);
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

      damage1_ = save_damage;
      set_damage(DAMAGE_ALL);
      draw();
      if (overlay == this) draw_overlay();
      if (!(mode_ & NO_AUTO_SWAP)) swap_buffers();
      goto NO_OVERLAY;

    }

    if (overlay==this) { // fake overlay in front buffer
      glDrawBuffer(GL_FRONT);
      draw_overlay();
      glDrawBuffer(GL_BACK);
      glFlush();
    }
  NO_OVERLAY:;

  } else {	// single-buffered context is simpler:

    draw();
    if (overlay == this) draw_overlay();
    glFlush();
  }

#if USE_GL_OVERLAY && defined(_WIN32)
 OVERLAY_ONLY:
  // Draw into hardware overlay planes if they are damaged:
  if (overlay && overlay != this
      && (i->region || save_damage&DAMAGE_OVERLAY || !save_valid)) {
#if SGI320_BUG
    // SGI 320 messes up overlay with user-defined cursors:
    bool fixcursor = false;
    if (i->cursor && i->cursor != fl_default_cursor) {
      fixcursor = true; // make it restore cursor later
      SetCursor(0);
    }
#endif
    set_gl_context(this, (GLContext)overlay);
    if (fl_overlay_depth) wglRealizeLayerPalette(i->dc, 1, TRUE);
    glDisable(GL_SCISSOR_TEST);
    if (!(mode_&NO_ERASE_OVERLAY)) glClear(GL_COLOR_BUFFER_BIT);
    fl_overlay = true;
    valid(save_valid);
    draw_overlay();
    fl_overlay = false;
    wglSwapLayerBuffers(i->dc, WGL_SWAP_OVERLAY1);
#if SGI320_BUG
    if (fixcursor) SetCursor(i->cursor);
#endif
  }
#endif
  valid(1);
}

void GlWindow::layout() {
  if (layout_damage() & LAYOUT_WH) {
    valid(0);
#if USE_QUARTZ
    no_gl_context(); // because the BUFFER_RECT may change
#endif
#ifndef _WIN32
    if (!resizable() && overlay && overlay != this)
      ((GlWindow*)overlay)->resize(0,0,w(),h());
#endif
  }
  Window::layout();
}

void GlWindow::context(void* v, bool destroy_flag) {
  if (context_ && !(mode_&NON_LOCAL_CONTEXT)) delete_gl_context(context_);
  context_ = (GLContext)v;
  if (destroy_flag) mode_ &= ~NON_LOCAL_CONTEXT;
  else mode_ |= NON_LOCAL_CONTEXT;
}    

void GlWindow::destroy() {
  context(0);
#if USE_GL_OVERLAY
  if (overlay && overlay != this) {
#ifdef _WIN32
    delete_gl_context((GLContext)overlay);
#endif
    overlay = 0;
  }
#endif
  Window::destroy();
}

GlWindow::~GlWindow() {
  destroy();
}

void GlWindow::init() {
  end(); // we probably don't want any children
  mode_ = DEPTH_BUFFER | DOUBLE_BUFFER;
  context_ = 0;
  gl_choice = 0;
  overlay = 0;
  damage1_ = 0;
}

void GlWindow::draw_overlay() {}

#endif

//
// End of "$Id$".
//
