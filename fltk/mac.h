//
// "$Id: mac.h,v 1.3 2003/01/05 07:40:29 spitzak Exp $"
//
// Mac header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

// Do not directly include this file, instead use <FL/x.h>.  It will
// include this file if "__APPLE__" is defined and "USE_X11" is UNdefined.
// This is to encourage portability of even the system-specific code...

#ifndef fltk_mac_h
#define fltk_mac_h

// Standard MacOS Carbon API includes...
#include <Carbon/Carbon.h>

// Now make some fixes to the headers...
#undef check			// Dunno where this comes from...

////////////////////////////////////////////////////////////////
// Emulate X somewhat:

typedef WindowPtr XWindow;
struct XPoint { int x, y; };
struct XRectangle {int x, y, width, height;};
typedef RgnHandle Region;
typedef GWorldPtr Pixmap;

inline Region XRectangleRegion(int x, int y, int w, int h) {
  Region R = NewRgn();
  SetRectRgn(R, x, y, x+w, y+h);
  return R;
}
inline void XDestroyRegion(Region r) {DisposeRgn(r);}
#define XDestroyWindow(a,b) DisposeWindow(b)
#define XMapWindow(a,b) ShowWindow(b)
#define XUnmapWindow(a,b) HideWindow(b)

namespace fltk {

////////////////////////////////////////////////////////////////
// constant information about the display:

extern FL_API void	open_display();
extern FL_API void	close_display();
//extern FL_API void	xdisplay;
extern FL_API Handle	system_menu;
extern FL_API class Fl_Sys_Menu_Bar *sys_menu_bar;

////////////////////////////////////////////////////////////////
// event handling:

// we want some way to access last message from system

////////////////////////////////////////////////////////////////
// drawing functions:

extern FL_API void	clip_region(Region);
extern FL_API Region	clip_region();

////////////////////////////////////////////////////////////////
// This class is an offscreen image that you plan to draw to repeatedly.
// It contains "context" information that may be expensive or impossible
// to recreate each time for drawing. On some systems this is a base
// class for Fl_X, which describes a window. Because
// of differences in how these things are created & destroyed, and
// the desire to have the id have a longer lifetime than this object,
// intelligent constructors and destructors are not implemented.

FL_API void load_identity();
FL_API void push_no_clip();
FL_API GWorldPtr create_offscreen(int w, int h);
FL_API void copy_offscreen(int x,int y,int w,int h, GWorldPtr, int srcx,int srcy);
FL_API void make_current(GWorldPtr xid);

class FL_API Drawable {
 public:
  GWorldPtr xid;
  Drawable() {}
  Drawable(GWorldPtr p) : xid(p) {}
  void create(int w, int h)  {xid = fltk::create_offscreen(w,h);}
  void copy(int x, int y, int w, int h, int src_x, int src_y) {
    fltk::copy_offscreen(x,y,w,h,xid,src_x,src_y);
  }
  void free_gc() {}
  void destroy() {DisposeGWorld(xid);}
  void make_current() {fltk::make_current(xid);}
};

////////////////////////////////////////////////////////////////
// This is an offscreen image that is designed to be drawn into
// exactly once and then repeatedly used as a source for copy. The
// object is expected to fit into a void* space in the Fl_Image
// structure. Drawing into them is surrounded by macros that save
// the current graphics state in local variables and create a
// temporary drawing context.

#define fl_create_offscreen(w,h) fltk::create_offscreen(w,h)

#define fl_begin_offscreen(id) \
  {::fltk::push_matrix(); \
  GrafPtr prevPort; GDHandle prevGD; GetGWorld(&prevPort, &prevGD); \
  ::fltk::make_current(id); \
  ::fltk::push_no_clip()

#define fl_end_offscreen() \
  SetGWorld(prevPort, prevGD); \
  ::fltk::pop_clip(); ::fltk::pop_matrix();}

#define fl_copy_offscreen fltk::copy_offscreen

#define fl_delete_offscreen(id) DisposeGWorld(id);

////////////////////////////////////////////////////////////////
// This is a binary offscreen image created from in-memory data. This is used
// as an alpha mask by Image on systems that don't support alpha
// blending.

extern Pixmap create_bitmap(const uchar* data, int w, int h);

static inline void delete_bitmap(Pixmap id) {DisposeGWorld(id);}

////////////////////////////////////////////////////////////////
#ifdef fltk_Window_h // only include this if <fltk/Window.h> was included

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

class FL_API CreatedWindow {
public:
  XWindow xid; // used by main windows
  Drawable backbuffer;
  Window* window;
  Region region; // damage region
  void expose(int x, int y, int w, int h);
  CreatedWindow* next;
  Region subRegion; // region which clips out children
  CreatedWindow *children, *brother;
  bool wait_for_expose;
  CursHandle cursor;
  const Widget* cursor_for;
  static CreatedWindow* first;
  static CreatedWindow* find(const Window* window) {return window->i;}
  static int borders(const Window* w, int& dx, int& dy, int& dw, int& dh);
};

// convert xid <-> Window:
inline XWindow xid(const Window*w) {return CreatedWindow::find(w)->xid;}
Window* find(XWindow xid);

extern CursHandle default_cursor;

#endif //Fl_Window_H
////////////////////////////////////////////////////////////////

}

#endif

//
// End of "$Id: mac.h,v 1.3 2003/01/05 07:40:29 spitzak Exp $".
//

