//
// "$Id: x.h,v 1.24 2004/02/05 07:21:20 spitzak Exp $"
//
// X11 header file for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// These are internal fltk symbols that are necessary or useful for
// calling Xlib.  You should include this file if (and ONLY if) you
// need to call Xlib directly.  You should avoid using this header
// file in your software if at all possible.  NO FLTK HEADER FILE IS
// ALLOWED TO INCLUDE THIS.
//
// Most of these symbols are replicated in header files
// for other operating systems (win32.h, mac.h) but the definition of
// the symbols is different, and usage may be somewhat different.

#ifndef fltk_x_h
#define fltk_x_h

#include "draw.h"

#if defined(_WIN32) && !USE_X11
# include "win32.h"

#elif defined(__APPLE__) && !USE_X11
# include "mac.h"

#else
#define USE_X11 1

////////////////////////////////////////////////////////////////
// Try to get the parts of Xlib.h included while avoiding warnings:

#ifndef DOXYGEN
# define Window XWindow

# if defined(_ABIN32) || defined(_ABI64) // fix for broken SGI Irix X .h files
#  pragma set woff 3322
# endif
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# if defined(_ABIN32) || defined(_ABI64)
#  pragma reset woff 3322
# endif

# include <X11/Xatom.h>

# if USE_XFT
#  include <X11/Xft/Xft.h>
# else
 typedef struct _XftDraw XftDraw;
# endif

# undef Window

#if defined(__FreeBSD__) || defined(__APPLE__)
typedef unsigned long	ulong;
typedef unsigned int	uint;
typedef unsigned char	uchar;
#endif

#endif

extern FL_API Region	XRectangleRegion(int x, int y, int w, int h);

namespace fltk {

////////////////////////////////////////////////////////////////
// constant info about the X server connection:

extern FL_API void	open_display();
extern FL_API void	open_display(Display*);
extern FL_API void	close_display();

extern FL_API Display*	xdisplay;
extern FL_API XWindow	message_window;
extern FL_API int	xscreen;
extern FL_API XVisualInfo* xvisual;
extern FL_API Colormap	xcolormap;

////////////////////////////////////////////////////////////////
// event handling:

// feed events into fltk by setting xevent and calling handle:
extern FL_API XEvent	xevent;
extern FL_API bool	handle();

// set by last xevent with a timestamp:
extern FL_API ulong	event_time;

////////////////////////////////////////////////////////////////
// DnD:

extern FL_API XWindow dnd_source_window;
extern FL_API Atom *dnd_source_types;
extern FL_API Atom dnd_type;
extern FL_API Atom dnd_source_action;
extern FL_API Atom dnd_action;

////////////////////////////////////////////////////////////////
// drawing functions:

class Drawable;

extern FL_API GC	gc;
extern FL_API Drawable* drawable;
extern FL_API XWindow	xwindow;
extern FL_API XFontStruct*	xfont();
extern FL_API ulong	current_xpixel;
extern FL_API ulong	xpixel(Color i);
extern FL_API void	clip_region(Region);
extern FL_API Region	clip_region();

////////////////////////////////////////////////////////////////
// This class is an offscreen image that you plan to draw to repeatedly.
// It contains "context" information that may be expensive or impossible
// to recreate each time for drawing. On some systems this is a base
// class for CreatedWindow, which describes a window. Because
// of differences in how these things are created & destroyed, and
// the desire to have the id have a longer lifetime than this object,
// intelligent constructors and destructors are not implemented.

class FL_API Drawable {
 public:
  XWindow xid;
  XftDraw* draw;
  Drawable() : draw(0) {}
  Drawable(Pixmap p) : xid(p), draw(0) {}
  void create(int w, int h) {
    xid = XCreatePixmap(xdisplay, xwindow, w, h, xvisual->depth);
  }
  void copy(int x, int y, int w, int h, int src_x, int src_y) {
    XCopyArea(xdisplay, xid, xwindow, gc, src_x, src_y, w, h, x, y);
  }
  void free_gc();
  void destroy() {
    if (xid) {free_gc(); XFreePixmap(xdisplay, xid); xid = 0;}
  }
  void make_current() {drawable = this; xwindow=xid; load_identity();}
};

////////////////////////////////////////////////////////////////
// This is an offscreen image that is designed to be drawn into
// exactly once and then repeatedly used as a source for copy. The
// object is expected to fit into a void* space in the Image
// structure. Drawing into them is surrounded by macros that save
// the current graphics state in local variables and create a
// temporary drawing context.

#define fl_create_offscreen(w,h) \
  XCreatePixmap(::fltk::xdisplay,::fltk::xwindow, w, h,::fltk::xvisual->depth)

#define fl_begin_offscreen(id) \
  {::fltk::push_matrix(); \
  ::fltk::Drawable* _sd = fltk::drawable; \
  ::fltk::Drawable _nd(id); \
  _nd.make_current(); \
  ::fltk::push_no_clip()

#define fl_end_offscreen() \
  _nd.free_gc(); _sd->make_current();::fltk::pop_clip();::fltk::pop_matrix();}

#define fl_copy_offscreen(x,y,w,h,id,srcx,srcy) \
  XCopyArea(::fltk::xdisplay, id, ::fltk::xwindow, ::fltk::gc, srcx, srcy, w, h, x, y)

#define fl_delete_offscreen(id) XFreePixmap(::fltk::xdisplay, id)

////////////////////////////////////////////////////////////////
// only include this if <fltk/Window.h> was included:
#if defined(fltk_Window_h) || defined(DOXYGEN)

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that xid(Window) can be declared inline:

class FL_API CreatedWindow : public Drawable {
public:
  Drawable backbuffer;
  Window *window;
  Region region;
  void expose(int x, int y, int w, int h);
  CreatedWindow *next;
  bool wait_for_expose;
  bool backbuffer_bad; // used for XDBE
  ::Cursor cursor;
  const Widget* cursor_for;
  static CreatedWindow* first;
  static CreatedWindow* find(const Window* window) {return window->i;}
  void sendxjunk();
  static void create(Window*,
		     XVisualInfo*, Colormap,
		     int background = -1);
  static CreatedWindow* set_xid(Window*, XWindow);
};

// convert xid <-> Window:
inline XWindow xid(const Window*w) {return CreatedWindow::find(w)->xid;}
Window* find(XWindow xid);

#endif // Window_h
////////////////////////////////////////////////////////////////

}

#endif	// not _WIN32
#endif

//
// End of "$Id: x.h,v 1.24 2004/02/05 07:21:20 spitzak Exp $".
//
