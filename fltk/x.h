//
// "$Id: x.h,v 1.2 2001/07/29 21:52:43 spitzak Exp $"
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

#include "Fl_Color.h"

#ifdef _WIN32

#include "win32.h"

#else

#if defined(_ABIN32) || defined(_ABI64) // fix for broken SGI Irix X .h files
#pragma set woff 3322
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#if defined(_ABIN32) || defined(_ABI64)
#pragma reset woff 3322
#endif
#include <X11/Xatom.h>

void fl_open_display();
void fl_open_display(Display*);
void fl_close_display();

// constant info about the X server connection:
extern FL_API Display *fl_display;
extern FL_API Window fl_message_window;
extern FL_API int fl_screen;
extern FL_API XVisualInfo *fl_visual;
extern FL_API Colormap fl_colormap;

// drawing functions:
extern FL_API GC fl_gc;
extern FL_API Window fl_window;
extern FL_API void *fl_xfont;
ulong fl_xpixel(Fl_Color i);
void fl_clip_region(Region);
Region XRectangleRegion(int x, int y, int w, int h); // in fl_rect.cxx

// feed events into fltk:
int fl_handle(const XEvent&);

// you can use these in Fl::add_handler() to look at events:
extern FL_API const XEvent* fl_xevent;
extern FL_API ulong fl_event_time;

// off-screen pixmaps: create, destroy, draw into, copy to window:
#define fl_create_offscreen(w,h) \
  XCreatePixmap(fl_display, fl_window, w, h, fl_visual->depth)
// begin/end are macros that save the old state in local variables:
#define fl_begin_offscreen(pixmap) \
  Window _sw = fl_window; fl_window = pixmap; \
  int _sx = fl_x_; int _sy = fl_y_; fl_x_ = fl_y_ = 0; \
  fl_push_no_clip()
#define fl_end_offscreen() \
  fl_pop_clip(); fl_y_ = _sy; fl_x_ = _sx; fl_window = _sw

#define fl_copy_offscreen(x,y,w,h,pixmap,srcx,srcy) \
  XCopyArea(fl_display, pixmap, fl_window, fl_gc, srcx, srcy, w, h, x, y)
#define fl_delete_offscreen(pixmap) XFreePixmap(fl_display, pixmap)

// binary images, created from in-memory data:
// See Fl_Image::_draw() for code that uses these.
static inline Pixmap fl_create_bitmap(const uchar* bitmap, int w, int h) {
  return XCreateBitmapFromData(fl_display, fl_window,
			       (const char*)bitmap, (w+7)&-8, h);
}
#define fl_delete_bitmap(bitmap) XFreePixmap(fl_display, bitmap)

#ifdef Fl_Window_H // only include this if <fltk/Fl_Window.h> was included

// this object contains all X-specific stuff about a window:
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

class FL_API Fl_X {
public:
  Window xid;
  Window other_xid;
  Fl_Window *window;
  Region region;
  Fl_X *next;
  bool wait_for_expose;
  bool backbuffer_bad; // used for XDBE
  static Fl_X* first;
  static Fl_X* i(const Fl_Window* window) {return window->i;}
  void sendxjunk();
  static void create(Fl_Window*,
		     XVisualInfo*, Colormap,
		     int background = -1);
};

// convert xid <-> Fl_Window:
inline Window fl_xid(const Fl_Window*w) {return Fl_X::i(w)->xid;}
inline Window fl_other_xid(const Fl_Window*w) {return Fl_X::i(w)->other_xid;}
Fl_Window* fl_find(Window xid);

#endif	//Fl_Window_H
#endif	// not _WIN32
#endif	//Fl_X_H

//
// End of "$Id: x.h,v 1.2 2001/07/29 21:52:43 spitzak Exp $".
//
