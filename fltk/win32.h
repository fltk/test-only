//
// "$Id: win32.h,v 1.10 2002/07/01 15:28:19 spitzak Exp $"
//
// _WIN32 header file for the Fast Light Tool Kit (FLTK).
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

// Do not directly include this file, instead use <fltk/x.h>.  It will
// include this file if _WIN32 is defined.  This is to encourage
// portability of even the system-specific code...

#ifndef FL_WIN32_H
#define FL_WIN32_H

#include <windows.h>
#include <winuser.h>
// In some of the distributions, the gcc header files are missing some stuff:
#ifndef LPMINMAXINFO
# define LPMINMAXINFO MINMAXINFO*
#endif
#ifndef VK_LWIN
# define VK_LWIN 0x5B
# define VK_RWIN 0x5C
# define VK_APPS 0x5D
#endif

////////////////////////////////////////////////////////////////
// Emulate X somewhat:

typedef HWND Window;
typedef POINT XPoint;
struct FL_API XRectangle {int x, y, width, height;};
typedef HRGN Region;
typedef HBITMAP Pixmap;

inline Region XRectangleRegion(int x, int y, int w, int h) {
    return CreateRectRgn(x, y, x+w, y+h);
}
inline void XDestroyRegion(Region r) {DeleteObject(r);}
inline void XClipBox(Region r, XRectangle* rect) {
    RECT win_rect; GetRgnBox(r, &win_rect);
    rect->x = win_rect.left;
    rect->y = win_rect.top;
    rect->width = win_rect.right-win_rect.left;
    rect->height = win_rect.bottom-win_rect.top;
}
#define XDestroyWindow(a,b) DestroyWindow(b)
#define XMapWindow(a,b)	ShowWindow(b, SW_RESTORE)
#define XUnmapWindow(a,b) ShowWindow(b, SW_HIDE)

////////////////////////////////////////////////////////////////
// constant information about the display:

#define fl_open_display()
extern FL_API HINSTANCE	fl_display;
extern FL_API HPALETTE	fl_palette; // non-zero only on 8-bit displays!

////////////////////////////////////////////////////////////////
// event handling:

extern FL_API MSG fl_msg;

////////////////////////////////////////////////////////////////
// drawing functions:

extern FL_API HDC	fl_gc;
inline FL_API HDC	fl_getDC() {return fl_gc ? fl_gc : GetDC(0);}
extern FL_API HFONT	fl_xfont();
extern FL_API TEXTMETRIC* fl_textmetric();
extern FL_API COLORREF	fl_colorref;
extern FL_API COLORREF	fl_wincolor(Fl_Color i);
extern FL_API HPEN	fl_pen;
extern FL_API HPEN	fl_create_pen();
extern FL_API HBRUSH	fl_brush;
extern FL_API void	fl_clip_region(Region);
extern FL_API Region	fl_clip_region();

////////////////////////////////////////////////////////////////
// This class is an offscreen image that you plan to draw to repeatedly.
// It contains "context" information that may be expensive or impossible
// to recreate each time for drawing. On some systems this is a base
// class for Fl_X, which describes a window. Because
// of differences in how these things are created & destroyed, and
// the desire to have the id have a longer lifetime than this object,
// intelligent constructors and destructors are not implemented.

FL_API HDC fl_makeDC(HBITMAP);
FL_API void fl_load_identity();

class FL_API Fl_Drawable {
 public:
  HWND xid;
  HDC dc;
  Fl_Drawable() {}
  Fl_Drawable(HBITMAP p) : xid((HWND)p), dc(fl_makeDC(p)) {}
  void create(int w, int h) {
    HBITMAP bitmap = CreateCompatibleBitmap(fl_gc, w, h);
    xid = (HWND)bitmap;
    dc = fl_makeDC(bitmap);
  }
  void copy(int x, int y, int w, int h, int src_x, int src_y) {
    BitBlt(fl_gc, x, y, w, h, dc, src_x, src_y, SRCCOPY);
  }
  void free_gc() {
    if (dc) {if (fl_gc == dc) fl_gc = 0; DeleteDC(dc); dc = 0;}
  }
  void destroy() {
    if (xid) {free_gc(); DeleteObject((HBITMAP)xid); xid = 0;}
  }
  void make_current() {fl_gc = dc; fl_load_identity();}
};

////////////////////////////////////////////////////////////////
// This is an offscreen image that is designed to be drawn into
// exactly once and then repeatedly used as a source for copy. The
// object is expected to fit into a void* space in the Fl_Image
// structure. Drawing into them is surrounded by macros that save
// the current graphics state in local variables and create a
// temporary drawing context.

#define fl_create_offscreen(w, h) CreateCompatibleBitmap(fl_gc, w, h)

#define fl_begin_offscreen(id) \
  {fl_push_matrix(); \
  HDC _sdc = fl_gc; \
  Fl_Drawable _nd(id); \
  _nd.make_current(); \
  fl_push_no_clip()

#define fl_end_offscreen() \
  _nd.free_gc(); fl_gc = _sdc; fl_pop_clip(); fl_pop_matrix();}

FL_API void fl_copy_offscreen(int x,int y,int w,int h,HBITMAP id,int srcx,int srcy);

#define fl_delete_offscreen(bitmap) DeleteObject(bitmap);

////////////////////////////////////////////////////////////////
// Binary images, created from in-memory data:

extern FL_API Pixmap fl_create_bitmap(const uchar* bitmap, int w, int h);

#define fl_delete_bitmap(bitmap) DeleteObject((HBITMAP)(bitmap));

////////////////////////////////////////////////////////////////
#ifdef Fl_Window_H // only include this if <fltk/Fl_Window.h> was included

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

class FL_API Fl_X : public Fl_Drawable {
public:
  Fl_Drawable backbuffer;
  Fl_Window* window;
  Region region;
  void expose(int x, int y, int w, int h);
  Fl_X *next;
  bool wait_for_expose;
  HCURSOR cursor;
  static Fl_X* first;
  static Fl_X* i(const Fl_Window* window) {return window->i;}
  static int borders(const Fl_Window* w, int& dx, int& dy, int& dw, int& dh);
  void set_minmax(LPMINMAXINFO minmax);
  static void create(Fl_Window*);
};

// convert xid <-> Fl_Window:
inline Window fl_xid(const Fl_Window* window) {return Fl_X::i(window)->xid;}
FL_API Fl_Window* fl_find(Window xid);

extern FL_API HCURSOR fl_default_cursor;

#endif //Fl_Window_H
////////////////////////////////////////////////////////////////

#endif

//
// End of "$Id: win32.h,v 1.10 2002/07/01 15:28:19 spitzak Exp $".
//
