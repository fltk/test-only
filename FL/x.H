//
// "$Id: x.H,v 1.17 1999/09/19 04:58:59 vincent Exp $"
//
// X11 header file for the Fast Light Tool Kit (FLTK).
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

// These are internal fltk symbols that are necessary or useful for
// calling Xlib.  You should include this file if (and ONLY if) you
// need to call Xlib directly.  These symbols may not exist on non-X
// systems.

#ifndef Fl_X_H
#define Fl_X_H

#include "Fl_Color.H"

#ifdef WIN32

#include "win32.H"

#else

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

FL_EXPORT void fl_open_display();
FL_EXPORT void fl_close_display();

// constant info about the X server connection:
FL_IMPORT extern FL_EXPORT Display *fl_display;
FL_IMPORT extern FL_EXPORT int fl_screen;
FL_IMPORT extern FL_EXPORT XVisualInfo *fl_visual;
FL_IMPORT extern FL_EXPORT Colormap fl_colormap;

// drawing functions:
FL_IMPORT extern FL_EXPORT GC fl_gc;
FL_IMPORT extern FL_EXPORT Window fl_window;
FL_IMPORT extern FL_EXPORT XFontStruct* fl_xfont;
FL_EXPORT ulong fl_xpixel(Fl_Color i);
FL_EXPORT void fl_clip_region(Region);
FL_EXPORT Region XRectangleRegion(int x, int y, int w, int h); // in fl_rect.cxx

// feed events into fltk:
FL_EXPORT int fl_handle(const XEvent&);

// you can use these in Fl::add_handler() to look at events:
FL_IMPORT extern FL_EXPORT const XEvent* fl_xevent;
FL_IMPORT extern FL_EXPORT ulong fl_event_time;

// off-screen pixmaps: create, destroy, draw into, copy to window:
#define Fl_Offscreen ulong
#define fl_create_offscreen(w,h) \
  XCreatePixmap(fl_display, fl_window, w, h, fl_visual->depth)
// begin/end are macros that save the old state in local variables:
#define fl_begin_offscreen(pixmap) \
  Window _sw=fl_window; fl_window=pixmap; fl_push_no_clip()
#define fl_end_offscreen() \
  fl_pop_clip(); fl_window = _sw

#define fl_copy_offscreen(x,y,w,h,pixmap,srcx,srcy) \
  XCopyArea(fl_display, pixmap, fl_window, fl_gc, srcx, srcy, w, h, x, y)
#define fl_delete_offscreen(pixmap) XFreePixmap(fl_display, pixmap)

// binary images, created from in-memory data:
// See Fl_Image::_draw() for code that uses these.
FL_IMPORT static inline ulong fl_create_bitmap(const uchar* bitmap, int w, int h) {
  return XCreateBitmapFromData(fl_display, fl_window,
			       (const char*)bitmap, (w+7)&-8, h);
}
#define fl_delete_bitmap(bitmap) XFreePixmap(fl_display, bitmap)

// this object contains all X-specific stuff about a window:
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

#include "Fl_Window.H"

class Fl_X {
public:
  Window xid;
  Window other_xid;
  Fl_Window *w;
  Region region;
  Fl_X *next;
  int wait_for_expose;
  int backbuffer_bad; // used for XDBE
  FL_IMPORT static Fl_X* first;
  FL_IMPORT static Fl_X* i(const Fl_Window* w) {return w->i;}
  void sendxjunk();
  FL_IMPORT static void create(Fl_Window*,
		     XVisualInfo*, Colormap,
		     int background = -1);
};

// convert xid <-> Fl_Window:
inline Window fl_xid(const Fl_Window*w) {return Fl_X::i(w)->xid;}
FL_EXPORT Fl_Window* fl_find(Window xid);

#endif
#endif

//
// End of "$Id: x.H,v 1.17 1999/09/19 04:58:59 vincent Exp $".
//
