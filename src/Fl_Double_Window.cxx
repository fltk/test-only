//
// "$Id: Fl_Double_Window.cxx,v 1.18 2000/04/10 06:45:44 bill Exp $"
//
// Double-buffered window code for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/x.H>
#include <FL/fl_draw.H>

// On systems that support double buffering "naturally" the base
// Fl_Window class will probably do double-buffer and this subclass
// does nothing.

#if USE_XDBE

#include <X11/extensions/Xdbe.h>

static int use_xdbe;

static int can_xdbe() {
  static int tried;
  if (!tried) {
    tried = 1;
    int event_base, error_base;
    if (!XdbeQueryExtension(fl_display, &event_base, &error_base)) return 0;
    Drawable root = RootWindow(fl_display,fl_screen);
    int numscreens = 1;
    XdbeScreenVisualInfo *a = XdbeGetVisualInfo(fl_display,&root,&numscreens);
    if (!a) return 0;
    for (int j = 0; j < a->count; j++)
      if (a->visinfo[j].visual == fl_visual->visualid
	  /*&& a->visinfo[j].perflevel > 0*/) {use_xdbe = 1; break;}
    XdbeFreeVisualInfo(a);
  }
  return use_xdbe;
}
#endif

void Fl_Double_Window::create() {
#ifndef WIN32
  // don't set the background pixel
  Fl_X::create(this, fl_visual, fl_colormap, -1);
#else
  Fl_Window::create();
#endif
}

#ifdef WIN32

// Code used to switch output to an off-screen window.  See macros in
// win32.H which save the old state in local variables.

HDC fl_makeDC(HBITMAP bitmap) {
  HDC new_gc = CreateCompatibleDC(fl_gc);
  SetTextAlign(new_gc, TA_BASELINE|TA_LEFT);
  SetBkMode(new_gc, TRANSPARENT);
#if USE_COLORMAP
  if (fl_palette) SelectPalette(new_gc, fl_palette, FALSE);
#endif
  SelectObject(new_gc, bitmap);
  return new_gc;
}

void fl_copy_offscreen(int x,int y,int w,int h,HBITMAP bitmap,int srcx,int srcy) {
  HDC new_gc = CreateCompatibleDC(fl_gc);
  SelectObject(new_gc, bitmap);
  BitBlt(fl_gc, x, y, w, h, new_gc, srcx, srcy, SRCCOPY);
  DeleteDC(new_gc);
}

extern void fl_restore_clip();

#endif

// Fl_Overlay_Window relies on flush(1) copying the back buffer to the
// front everywhere, even if damage() == 0, thus erasing the overlay,
// and leaving the clip region set to the entire window.

void Fl_Double_Window::flush() {flush(0);}

void Fl_Double_Window::flush(int eraseoverlay) {
  make_current(); // make sure fl_gc is non-zero
  Fl_X *i = Fl_X::i(this);
  if (!i->other_xid) {
#if USE_XDBE
    if (can_xdbe()) i->other_xid =
      XdbeAllocateBackBufferName(fl_display, fl_xid(this), XdbeUndefined);
    else
#endif
      i->other_xid = fl_create_offscreen(w(), h());
    set_damage(FL_DAMAGE_ALL);
  }
#if USE_XDBE
  if (use_xdbe) {
    // if this is true, copy rather than swap so back buffer is preserved:
    int copy = (i->region || eraseoverlay);
    if (i->backbuffer_bad) { // make sure we do a complete redraw...
      if (i->region) {XDestroyRegion(i->region); i->region = 0;}
      set_damage(FL_DAMAGE_ALL);
    }
    if (damage()) {
      fl_clip_region(i->region); i->region = 0;
      fl_window = i->other_xid;
      draw();
      fl_window = i->xid;
    }
    if (!copy) {
      XdbeSwapInfo s;
      s.swap_window = fl_xid(this);
      s.swap_action = XdbeUndefined;
      XdbeSwapBuffers(fl_display, &s, 1);
      i->backbuffer_bad = 1;
      return;
    }
    // otherwise just use normal copy from back to front:
    i->backbuffer_bad = 0; // which won't destroy the back buffer...
  } else
#endif
  if (damage() & ~FL_DAMAGE_EXPOSE) {
    fl_clip_region(i->region); i->region = 0;
#ifdef WIN32
    HDC _sgc = fl_gc;
    fl_gc = fl_makeDC(i->other_xid);
    fl_restore_clip(); // duplicate region into new gc
    draw();
    DeleteDC(fl_gc);
    fl_gc = _sgc;
#else // X:
    fl_window = i->other_xid;
    draw();
    fl_window = i->xid;
#endif
  }
  if (eraseoverlay) fl_clip_region(0);
  // on Irix (at least) it is faster to reduce the area copied to
  // the current clip region:
  int X,Y,W,H; fl_clip_box(0,0,w(),h(),X,Y,W,H);
  fl_copy_offscreen(X, Y, W, H, i->other_xid, X, Y);
}

void Fl_Double_Window::maybe_free_backbuffer(int W, int H) {
#if USE_XDBE
  if (!use_xdbe) {
#endif
    Fl_X* i = Fl_X::i(this);
    if (i && i->other_xid && (W != w() || H != h())) {
      fl_delete_offscreen(i->other_xid);
      i->other_xid = 0;
    }
#if USE_XDBE
  }
#endif
}

void Fl_Double_Window::layout() {
  maybe_free_backbuffer(ow(),oh());
  Fl_Window::layout();
}

void Fl_Double_Window::destroy() {
  Fl_X* i = Fl_X::i(this);
  if (i && i->other_xid) {
#if USE_XDBE
    if (!use_xdbe)
#endif
      fl_delete_offscreen(i->other_xid);
  }
  Fl_Window::destroy();
}

Fl_Double_Window::~Fl_Double_Window() {
  destroy();
}

//
// End of "$Id: Fl_Double_Window.cxx,v 1.18 2000/04/10 06:45:44 bill Exp $".
//
