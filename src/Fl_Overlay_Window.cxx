//
// "$Id: Fl_Overlay_Window.cxx,v 1.20 2002/12/10 01:46:17 easysw Exp $"
//
// Overlay window code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

// A window using double-buffering and able to draw an overlay
// on top of that.  Uses the hardware to draw the overlay if
// possible, otherwise it just draws in the front buffer.

#include <fltk/OverlayWindow.h>
#include <fltk/damage.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <config.h>

using namespace fltk;

void OverlayWindow::flush() {
#if BOXX_OVERLAY_BUGS
  if (overlay_ && overlay_ != this && overlay_->shown()) {
    // all drawing to windows hidden by overlay windows is ignored, fix this
    XUnmapWindow(xdisplay, xid(overlay_));
    DoubleBufferWindow::flush(0);
    XMapWindow(xdisplay, xid(overlay_));
    return;
  }
#endif
  bool erase_overlay = (damage()&DAMAGE_OVERLAY) != 0;
  set_damage(damage()&~DAMAGE_OVERLAY);
  DoubleBufferWindow::flush(erase_overlay);
  if (overlay_ == this) draw_overlay();
  clip_region(0); // turn off any clip it left on
}

void OverlayWindow::layout() {
  DoubleBufferWindow::layout();
  if (overlay_ && overlay_!=this) {
    overlay_->resize(0,0,w(),h());
    overlay_->layout();
  }
}

#if !USE_OVERLAY

int OverlayWindow::can_do_overlay() {return 0;}

void OverlayWindow::redraw_overlay() {
  overlay_ = this;
  set_damage(damage()|DAMAGE_OVERLAY);
  redraw(DAMAGE_CHILD);
}

#else

extern XVisualInfo *fl_find_overlay_visual();
extern XVisualInfo *fl_overlay_visual;
extern Colormap fl_overlay_colormap;
extern unsigned long fl_transparent_pixel;
static GC ovlgc;	// the GC used by all X windows
extern uchar fl_overlay; // changes how color(x) works

class _Overlay : public Window {
  friend class fltk::OverlayWindow;
  void flush();
  void create();
public:
  _Overlay(int x, int y, int w, int h) :
    Window(x,y,w,h) {set_flag(INACTIVE);}
};

int OverlayWindow::can_do_overlay() {
  return fl_find_overlay_visual() != 0;
}

void _Overlay::create() {
  CreatedWindow::create(this, fl_overlay_visual, fl_overlay_colormap,
			int(fl_transparent_pixel));
  // find the outermost window to tell wm about the colormap:
  Window *w = window();
  for (;;) {Window *w1 = w->window(); if (!w1) break; w = w1;}
  XSetWMColormapWindows(xdisplay, xid(w), &(CreatedWindow::find(this)->xid),1);
}

void _Overlay::flush() {
  xwindow = xid(this);
  if (!ovlgc) ovlgc = XCreateGC(xdisplay, xwindow, 0, 0);
  fltk::gc = ovlgc;
  fl_overlay = true;
  OverlayWindow *w = (OverlayWindow *)parent();
  current_ = w;
  CreatedWindow *i = CreatedWindow::find(this);
  if (damage() == DAMAGE_EXPOSE) {
    clip_region(i->region); i->region = 0;
    w->draw_overlay();
    clip_region(0);
  } else {
    XClearWindow(xdisplay, xid(this));
    w->draw_overlay();
  }
  fl_overlay = false;
}

void OverlayWindow::redraw_overlay() {
  if (!xdisplay) return; // this prevents fluid -c from opening display
  if (!overlay_) {
    if (can_do_overlay()) {
      overlay_ = new _Overlay(0,0,w(),h());
      add_resizable(*overlay_);
      overlay_->show();
    } else {
      overlay_ = this;	// fake the overlay
    }
  }
  if (shown()) {
    if (overlay_ == this) {
      set_damage(damage()|DAMAGE_OVERLAY);
      redraw(DAMAGE_CHILD);
    } else {
      overlay_->redraw();
    }
  }
}

#endif

//
// End of "$Id: Fl_Overlay_Window.cxx,v 1.20 2002/12/10 01:46:17 easysw Exp $".
//
