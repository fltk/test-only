//
// "$Id: x.h,v 1.32 2004/12/18 19:03:02 spitzak Exp $"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

// Declarations of internal fltk symbols that are useful for calling
// Xlib functions. You should include this file if (and ONLY if) you
// need to call Xlib directly.  You should avoid using this header
// file in your software if at all possible.  NO FLTK HEADER FILE IS
// ALLOWED TO INCLUDE THIS.
//
// For historic reasons, this file will also include the correct
// header file for other operating systems. As all three major systems
// now have multiple drawing libraries (Linux has X11/Cairo, Windows
// has GDI32/LongHorn/DirectX, and Mac has Carbon/Aqua/X11, and all
// have OpenGL) not all the symbols in here may be usable with a given
// compilation of fltk.

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

// pragmas are to fix the broken SGI Irix Xlib header files:
# if !defined(__GNUC__) && (defined(_ABIN32) || defined(_ABI64))
#  pragma set woff 3322
# endif
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# if !defined(__GNUC__) && (defined(_ABIN32) || defined(_ABI64))
#  pragma reset woff 3322
# endif

# include <X11/Xatom.h>

# if USE_XFT
#  include <X11/Xft/Xft.h>
# else
 typedef struct _XftDraw XftDraw;
# endif

# if USE_CAIRO
#  include <cairo.h>
# else
 typedef struct cairo cairo_t;
# endif

#include <X11/extensions/XInput.h>
#include <X11/extensions/XI.h>

# undef Window

# if defined(__FreeBSD__) || defined(__APPLE__) || defined(__CYGWIN__)
typedef unsigned long	ulong;
typedef unsigned int	uint;
typedef unsigned char	uchar;
# endif

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

extern FL_API cairo_t*	cc;
extern FL_API GC	gc;
extern FL_API XWindow	xwindow;
extern FL_API ulong	current_xpixel;
extern FL_API ulong	xpixel(Color i);
extern FL_API void	clip_region(Region);
extern FL_API Region	clip_region();

extern FL_API void	draw_into(XWindow);
extern FL_API void	stop_drawing(XWindow);

extern FL_API XFontStruct*	xfont();

////////////////////////////////////////////////////////////////
// only include this if <fltk/Window.h> was included:
#if defined(fltk_Window_h) || defined(DOXYGEN)

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that xid(Window) can be declared inline:

class FL_API CreatedWindow {
public:
  XWindow xid;
  XWindow backbuffer;
  XWindow frontbuffer;
  Window *window;
  Region region;
  void expose(int x, int y, int w, int h);
  CreatedWindow *next;
  bool wait_for_expose;
  bool backbuffer_bad; // used for XDBE
  bool overlay; // true if redraw_overlay was called
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
// End of "$Id: x.h,v 1.32 2004/12/18 19:03:02 spitzak Exp $".
//
