//
// "$Id: Fl_Overlay_Window.cxx,v 1.15 2001/07/24 07:48:23 spitzak Exp $"
//
// Overlay window code for the Fast Light Tool Kit (FLTK).
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

// A window using double-buffering and able to draw an overlay
// on top of that.  Uses the hardware to draw the overlay if
// possible, otherwise it just draws in the front buffer.

#include <config.h>
#include <fltk/Fl.h>
#include <fltk/Fl_Overlay_Window.h>
#include <fltk/fl_draw.h>
#include <fltk/x.h>

void Fl_Overlay_Window::flush() {
#if BOXX_OVERLAY_BUGS
  if (overlay_ && overlay_ != this && overlay_->shown()) {
    // all drawing to windows hidden by overlay windows is ignored, fix this
    XUnmapWindow(fl_display, fl_xid(overlay_));
    Fl_Double_Window::flush(0);
    XMapWindow(fl_display, fl_xid(overlay_));
    return;
  }
#endif
  int erase_overlay = (damage()&FL_DAMAGE_OVERLAY);
  set_damage(damage()&~FL_DAMAGE_OVERLAY);
  Fl_Double_Window::flush(erase_overlay);
  if (overlay_ == this) draw_overlay();
}

void Fl_Overlay_Window::layout() {
  Fl_Double_Window::layout();
  if (overlay_ && overlay_!=this) {
    overlay_->resize(0,0,w(),h());
    overlay_->layout();
  }
}

#if !HAVE_OVERLAY

int Fl_Overlay_Window::can_do_overlay() {return 0;}

void Fl_Overlay_Window::redraw_overlay() {
  overlay_ = this;
  set_damage(damage()|FL_DAMAGE_OVERLAY);
  Fl::damage(FL_DAMAGE_CHILD);
}

#else

extern XVisualInfo *fl_find_overlay_visual();
extern XVisualInfo *fl_overlay_visual;
extern Colormap fl_overlay_colormap;
extern unsigned long fl_transparent_pixel;
static GC gc;	// the GC used by all X windows
extern uchar fl_overlay; // changes how fl_color(x) works

class _Fl_Overlay : public Fl_Window {
  friend class Fl_Overlay_Window;
  void flush();
  void create();
public:
  _Fl_Overlay(int x, int y, int w, int h) :
    Fl_Window(x,y,w,h) {set_flag(FL_INACTIVE);}
};

int Fl_Overlay_Window::can_do_overlay() {
  return fl_find_overlay_visual() != 0;
}

void _Fl_Overlay::create() {
  Fl_X::create(this, fl_overlay_visual, fl_overlay_colormap, int(fl_transparent_pixel));
  // find the outermost window to tell wm about the colormap:
  Fl_Window *w = window();
  for (;;) {Fl_Window *w1 = w->window(); if (!w1) break; w = w1;}
  XSetWMColormapWindows(fl_display, fl_xid(w), &(Fl_X::i(this)->xid), 1);
}

void _Fl_Overlay::flush() {
  fl_window = fl_xid(this);
  if (!gc) gc = XCreateGC(fl_display, fl_xid(this), 0, 0);
  fl_gc = gc;
  fl_overlay = 1;
  Fl_Overlay_Window *w = (Fl_Overlay_Window *)parent();
  Fl_X *i = Fl_X::i(this);
  if (damage() != FL_DAMAGE_EXPOSE) XClearWindow(fl_display, fl_xid(this));
  fl_clip_region(i->region); i->region = 0;
  w->draw_overlay();
  fl_overlay = 0;
}

void Fl_Overlay_Window::redraw_overlay() {
  if (!fl_display) return; // this prevents fluid -c from opening display
  if (!overlay_) {
    if (can_do_overlay()) {
      overlay_ = new _Fl_Overlay(0,0,w(),h());
      add_resizable(*overlay_);
      overlay_->show();
    } else {
      overlay_ = this;	// fake the overlay
    }
  }
  if (shown()) {
    if (overlay_ == this) {
      set_damage(damage()|FL_DAMAGE_OVERLAY);
      Fl::damage(FL_DAMAGE_CHILD);
    } else {
      overlay_->redraw();
    }
  }
}

#endif

//
// End of "$Id: Fl_Overlay_Window.cxx,v 1.15 2001/07/24 07:48:23 spitzak Exp $".
//
