//
// "$Id: mac.h,v 1.2 2002/12/09 04:47:59 spitzak Exp $"
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

#ifndef FL_MAC_H
#define FL_MAC_H

// Standard MacOS Carbon API includes...
#include <Carbon/Carbon.h>

// Now make some fixes to the headers...
#undef check			// Dunno where this comes from...

////////////////////////////////////////////////////////////////
// Emulate X somewhat:

typedef WindowPtr Window;
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

////////////////////////////////////////////////////////////////
// constant information about the display:

extern void fl_open_display();
extern FL_API void *fl_display;
extern FL_API Handle fl_system_menu;
extern FL_API class Fl_Sys_Menu_Bar *fl_sys_menu_bar;

////////////////////////////////////////////////////////////////
// event handling:

// we want some way to access last message from system

////////////////////////////////////////////////////////////////
// drawing functions:

void fl_clip_region(Region);
Region fl_clip_region();

////////////////////////////////////////////////////////////////
// This is an offscreen image that is designed to be drawn into
// exactly once and then repeatedly used as a source for copy. The
// object is expected to fit into a void* space in the Fl_Image
// structure. Drawing into them is surrounded by macros that save
// the current graphics state in local variables and create a
// temporary drawing context.

extern GWorldPtr fl_create_offscreen(int w, int h);

extern void fl_make_current(GWorldPtr id);

#define fl_begin_offscreen(id) \
  {fl_push_matrix(); \
  GrafPtr prevPort; GDHandle prevGD; GetGWorld(&prevPort, &prevGD); \
  fl_make_current(id); \
  fl_push_no_clip()

#define fl_end_offscreen() \
  SetGWorld(prevPort, prevGD); \
  fl_pop_matrix();}

extern void fl_copy_offscreen(int x,int y,int w,int h, GWorldPtr, int srcx,int srcy);

#define fl_delete_offscreen(id) DisposeGWorld(id);

////////////////////////////////////////////////////////////////
// This class is an offscreen image that you plan to draw to repeatedly.
// It contains "context" information that may be expensive or impossible
// to recreate each time for drawing. On some systems this is a base
// class for Fl_X, which describes a window. Because
// of differences in how these things are created & destroyed, and
// the desire to have the id have a longer lifetime than this object,
// intelligent constructors and destructors are not implemented.

class FL_API Fl_Drawable {
 public:
  GWorldPtr xid;
  Fl_Drawable() {}
  Fl_Drawable(GWorldPtr p) : xid(p) {}
  void create(int w, int h)  {xid = fl_create_offscreen(w,h);}
  void copy(int x, int y, int w, int h, int src_x, int src_y) {
    fl_copy_offscreen(x,y,w,h,xid,src_x,src_y);
  }
  void free_gc() {}
  void destroy() {fl_delete_offscreen(xid);}
  void make_current() {fl_make_current(xid);}
};

////////////////////////////////////////////////////////////////
// Binary images, created from in-memory data:

extern FL_API Pixmap fl_create_bitmap(const uchar* bitmap, int w, int h);

#define fl_delete_bitmap(bitmap) DeleteObject((HBITMAP)(bitmap));

////////////////////////////////////////////////////////////////
#ifdef Fl_Window_H // only include this if <fltk/Fl_Window.h> was included

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

class FL_API Fl_X {
public:
  Window xid; // used by main windows
  Fl_Drawable backbuffer;
  Fl_Window* window;
  Region region;
  void expose(int x, int y, int w, int h);
  Fl_X *next;
  Region subRegion;
  Fl_X *children, *brother;
  bool wait_for_expose;
  CursHandle cursor;
  static Fl_X* first;
  static Fl_X* i(const Fl_Window* window) {return window->i;}
  static int borders(const Fl_Window* w, int& dx, int& dy, int& dw, int& dh);
};

// convert xid <-> Fl_Window:
inline Window fl_xid(const Fl_Window* window) {return Fl_X::i(window)->xid;}
FL_API Fl_Window* fl_find(Window xid);

extern CursHandle fl_default_cursor;

#endif //Fl_Window_H
////////////////////////////////////////////////////////////////

#endif // !FL_MAC_H

//
// End of "$Id: mac.h,v 1.2 2002/12/09 04:47:59 spitzak Exp $".
//

