//
// "$Id: win32.H,v 1.39 2001/07/16 19:38:17 robertk Exp $"
//
// WIN32 header file for the Fast Light Tool Kit (FLTK).
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

// Do not directly include this file, instead use <FL/x.H>.  It will
// include this file if WIN32 is defined.  This is to encourage
// portability of even the system-specific code...


#ifndef FL_WIN32_H
#define FL_WIN32_H

#include <windows.h>
#include <winuser.h>
// In some of the distributions, the gcc header files are missing some stuff:
#ifndef LPMINMAXINFO
#define LPMINMAXINFO MINMAXINFO*
#endif
#ifndef VK_LWIN
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#endif

// some random X equivalents
typedef HWND Window;
typedef POINT XPoint;
struct FL_API XRectangle {int x, y, width, height;};
typedef HRGN Region;
typedef HBITMAP Pixmap;

FL_API COLORREF fl_wincolor(Fl_Color i);

FL_API void fl_clip_region(Region);

inline Region XRectangleRegion(int x, int y, int w, int h) {
    return CreateRectRgn(x,y,x+w,y+h);
}
inline void XDestroyRegion(Region r) {DeleteObject(r);}
inline void XClipBox(Region r,XRectangle* rect) {
    RECT win_rect; GetRgnBox(r,&win_rect);
    rect->x=win_rect.left;
    rect->y=win_rect.top;
    rect->width=win_rect.right-win_rect.left;
    rect->height=win_rect.bottom-win_rect.top;
}
#define XDestroyWindow(a,b) DestroyWindow(b)
#define XMapWindow(a,b) ShowWindow(b, SW_RESTORE)
#define XUnmapWindow(a,b) ShowWindow(b, SW_HIDE)

extern FL_API HINSTANCE	fl_display;
extern FL_API HDC	fl_gc;
extern FL_API Window	fl_window;
extern FL_API void	*fl_xfont;
extern FL_API HPALETTE	fl_palette; // non-zero only on 8-bit displays!
extern FL_API COLORREF	fl_colorref;
extern FL_API HPEN	fl_pen;
extern FL_API HPEN	fl_create_pen();
extern FL_API HBRUSH	fl_brush;

extern FL_API HDC fl_GetDC(Window);
extern FL_API MSG fl_msg;

// off-screen pixmaps: create, destroy, draw into, copy to window
#define fl_create_offscreen(w, h) CreateCompatibleBitmap(fl_gc, w, h)

extern FL_API HDC fl_makeDC(HBITMAP);
extern FL_API void swap_fl_coordinates(int, int, int*, int*);
#define fl_begin_offscreen(b) \
  Window _sw = fl_window; fl_window = (HWND)(b); \
  int _sx, _sy; \
  swap_fl_coordinates(0, 0, &_sx, &_sy); \
  HDC _sgc = fl_gc; fl_gc = fl_makeDC(b); fl_push_no_clip()

#define fl_end_offscreen() \
  fl_pop_clip(); DeleteDC(fl_gc); \
  swap_fl_coordinates(_sx, _sy, 0, 0); \
  fl_window=_sw; fl_gc = _sgc

FL_API void fl_copy_offscreen(int x,int y,int w,int h,HBITMAP pixmap,int srcx,int srcy);
#define fl_delete_offscreen(bitmap) DeleteObject(bitmap);

// binary images, created from in-memory data:
// See Fl_Image::_draw() for code that uses these.
extern FL_API Pixmap fl_create_bitmap(const uchar* bitmap, int w, int h);
#define fl_delete_bitmap(bitmap) DeleteObject((HBITMAP)(bitmap));

extern FL_API HCURSOR fl_default_cursor;

#ifdef Fl_Window_H // only include this if <FL/Fl_Window.H> was included

// this object contains all X-specific stuff about a window:
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

class FL_API Fl_X {
public:
  Window xid;
  HBITMAP other_xid; // for double-buffered windows
  Fl_Window* w;
  Region region;
  Fl_X *next;
  int wait_for_expose;
  HDC private_dc; // used for OpenGL
  HCURSOR cursor;
  inline void flush() {w->flush();}
  static Fl_X* first;
  static Fl_X* i(const Fl_Window* w) {return w->i;}
  static int borders(const Fl_Window* w, int& dx, int& dy, int& dw, int& dh);
  void set_minmax(LPMINMAXINFO minmax);
  static Fl_X* create(Fl_Window*);
};
inline Window fl_xid(const Fl_Window*w) {return Fl_X::i(w)->xid;}
FL_API Fl_Window* fl_find(Window xid);


#endif
#endif

//
// End of "$Id: win32.H,v 1.39 2001/07/16 19:38:17 robertk Exp $".
//
