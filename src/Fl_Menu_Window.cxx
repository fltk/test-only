//
// "$Id: Fl_Menu_Window.cxx,v 1.11 2001/07/23 09:50:05 spitzak Exp $"
//
// Menu window code for the Fast Light Tool Kit (FLTK).
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

// This is the window type used by Fl_Menu to make the pop-ups.
// It draws in the overlay planes if possible.

// Also here is the implementation of the mouse & keyboard grab,
// which are used so that clicks outside the program's windows
// can be used to dismiss the menus.

#include <fltk/Fl.h>
#include <fltk/Fl_Menu_Window.h>
#include <fltk/x.h>
#include <fltk/fl_draw.h>
#include <config.h>

// WIN32 note: HAVE_OVERLAY is false
#if HAVE_OVERLAY
extern XVisualInfo *fl_find_overlay_visual();
extern XVisualInfo *fl_overlay_visual;
extern Colormap fl_overlay_colormap;
extern unsigned long fl_transparent_pixel;
static GC gc;	// the GC used by all X windows
extern uchar fl_overlay; // changes how fl_color(x) works
#endif

void Fl_Menu_Window::create() {
#if HAVE_OVERLAY
  if (overlay() && fl_find_overlay_visual()) {
    XInstallColormap(fl_display, fl_overlay_colormap);
    Fl_X::create(this, fl_overlay_visual, fl_overlay_colormap, int(fl_transparent_pixel));
  } else
#endif
    Fl_Single_Window::create();
}

void Fl_Menu_Window::flush() {
#if HAVE_OVERLAY
  if (!fl_overlay_visual || !overlay()) {Fl_Single_Window::flush(); return;}
  Fl_X *i = Fl_X::i(this);
  fl_window = i->xid;
  if (!gc) gc = XCreateGC(fl_display, i->xid, 0, 0);
  fl_gc = gc;
  fl_overlay = 1;
  fl_clip_region(i->region); i->region = 0; current_ = this;
  draw();
  fl_overlay = 0;
#else
  Fl_Single_Window::flush();
#endif
}

void Fl_Menu_Window::erase() {
#if HAVE_OVERLAY
  if (!gc || !shown()) return;
//XSetForeground(fl_display, gc, 0);
//XFillRectangle(fl_display, fl_xid(this), gc, 0, 0, w(), h());
  XClearWindow(fl_display, fl_xid(this));
#endif
}

// Fix the colormap flashing on Maximum Impact Graphics by erasing the
// menu before unmapping it:
void Fl_Menu_Window::destroy() {
  erase();
  Fl_Single_Window::destroy();
}

Fl_Menu_Window::~Fl_Menu_Window() {
  destroy();
}

//
// End of "$Id: Fl_Menu_Window.cxx,v 1.11 2001/07/23 09:50:05 spitzak Exp $".
//
