//
// "$Id: x.h,v 1.7 2002/03/26 18:00:34 spitzak Exp $"
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

// This header file defines public interfaces to fltk that are specific
// to the X version. Many of these symbols are replicated in header files
// for other operating systems (win32.h, mac.h) but the definition of
// the symbols is different, and usage may be somewhat different.
//
// You should avoid using this header file in your software if at all
// possible. No fltk header file is allowed to include this.

#ifndef Fl_X_H
#define Fl_X_H

#include "Fl_Color.h"

#ifdef _WIN32
# include "win32.h"
#else

////////////////////////////////////////////////////////////////
// Try to get the parts of Xlib.h included while avoiding warnings:

#if defined(_ABIN32) || defined(_ABI64) // fix for broken SGI Irix X .h files
# pragma set woff 3322
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#if defined(_ABIN32) || defined(_ABI64)
# pragma reset woff 3322
#endif

#include <X11/Xatom.h>

// you should define USE_XFT 1 if you want to call Xft:
#if USE_XFT
# include <X11/Xft/Xft.h>
#else
 typedef struct _XftDraw XftDraw;
#endif

////////////////////////////////////////////////////////////////
// constant info about the X server connection:

extern FL_API void	fl_open_display();
extern FL_API void	fl_open_display(Display*);
extern FL_API void	fl_close_display();

extern FL_API Display*	fl_display;
extern FL_API Window	fl_message_window;
extern FL_API int	fl_screen;
extern FL_API XVisualInfo* fl_visual;
extern FL_API Colormap	fl_colormap;

////////////////////////////////////////////////////////////////
// event handling:

// feed events into fltk by setting fl_xevent and calling fl_handle:
extern FL_API XEvent	fl_xevent;
extern FL_API bool	fl_handle();

// set by last fl_xevent with a timestamp:
extern FL_API ulong	fl_event_time;

////////////////////////////////////////////////////////////////
// drawing functions:

extern FL_API GC	fl_gc;
extern FL_API struct Fl_Drawable* fl_drawable;
extern FL_API Window	fl_window;
extern FL_API XFontStruct*	fl_xfont();
extern FL_API ulong	fl_pixel; // ==fl_xpixel(fl_color())
extern FL_API ulong	fl_xpixel(Fl_Color i);
extern FL_API void	fl_clip_region(Region);
extern FL_API Region	fl_clip_region();
extern FL_API Region	XRectangleRegion(int x, int y, int w, int h);

////////////////////////////////////////////////////////////////
// This class contains the id of a window or offscreen image, and
// the necessary "context" stuff needed to draw into it. Because
// of differences in how these things are created & destroyed, and
// the desire to have the id have a longer lifetime than this object,
// intelligent constructors and destructors are not implemented.

class FL_API Fl_Drawable {
 public:
  Window xid;
  XftDraw* draw;
  Fl_Drawable() : draw(0) {}
  Fl_Drawable(Pixmap p) : xid(p), draw(0) {}
  void create(int w, int h) {
    xid = XCreatePixmap(fl_display, fl_window, w, h, fl_visual->depth);
  }
  void copy(int x, int y, int w, int h, int src_x, int src_y) {
    XCopyArea(fl_display, xid, fl_window, fl_gc, src_x, src_y, w, h, x, y);
  }
  void free_gc();
  void destroy() {
    if (xid) {free_gc(); XFreePixmap(fl_display, xid); xid = 0;}
  }
  void make_current() {fl_drawable = this; fl_window = xid;}
};

////////////////////////////////////////////////////////////////
// Offscreen images are identified and stored as only an xid. Drawing
// into them is surrounded by macros that create a temporary Fl_Drawable.
// Because the Fl_Drawable is destroyed this is best if drawing is only
// done once, if you want to draw repeatedly you should use an Fl_Drawable.

#define fl_create_offscreen(w,h) \
  XCreatePixmap(fl_display, fl_window, w, h, fl_visual->depth)

#define fl_begin_offscreen(id) \
  {Fl_Drawable* _sd = fl_drawable; \
  Fl_Drawable _nd(id); \
  _nd.make_current(); \
  int _sx = fl_x_; int _sy = fl_y_; fl_x_ = fl_y_ = 0; \
  fl_push_no_clip()

#define fl_end_offscreen() \
  _nd.free_gc(); _sd->make_current(); fl_pop_clip(); fl_y_ = _sy; fl_x_ = _sx;}

#define fl_copy_offscreen(x,y,w,h,id,srcx,srcy) \
  XCopyArea(fl_display, id, fl_window, fl_gc, srcx, srcy, w, h, x, y)

#define fl_delete_offscreen(id) XFreePixmap(fl_display, id)

////////////////////////////////////////////////////////////////
// Binary images, created from in-memory data:

static inline Pixmap fl_create_bitmap(const uchar* data, int w, int h) {
  return XCreateBitmapFromData(fl_display, fl_window, (char*)data,(w+7)&-8, h);
}
#define fl_delete_bitmap(id) XFreePixmap(fl_display, id)

////////////////////////////////////////////////////////////////
#ifdef Fl_Window_H // only include this if <fltk/Fl_Window.h> was included

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

class FL_API Fl_X : public Fl_Drawable {
public:
  Fl_Drawable backbuffer;
  Fl_Window *window;
  Region region;
  void expose(int x, int y, int w, int h);
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
Fl_Window* fl_find(Window xid);

#endif //Fl_Window_H
////////////////////////////////////////////////////////////////

#endif	// not _WIN32
#endif	//Fl_X_H

//
// End of "$Id: x.h,v 1.7 2002/03/26 18:00:34 spitzak Exp $".
//
