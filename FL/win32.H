//
// "$Id: win32.H,v 1.27 2000/01/10 06:31:04 bill Exp $"
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
void fl_clip_region(Region);
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

#include "Fl_Window.H"

// this object contains all win32-specific stuff about a window:
// Warning: this object is highly subject to change!
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
  static int borders(const Fl_Window* w,
					 int& dx, int& dy, int& dw, int& dh);
  void set_minmax(LPMINMAXINFO minmax);
  static Fl_X* create(Fl_Window*);
};
extern FL_API HCURSOR fl_default_cursor;
inline Window fl_xid(const Fl_Window*w) {return Fl_X::i(w)->xid;}
Fl_Window* fl_find(Window xid);

// most recent fl_color() or fl_rgbcolor() points at one of these:
extern FL_API struct FL_API Fl_XMap {
  COLORREF rgb;	// this should be the type the RGB() macro returns
  HPEN pen;	// pen, 0 if none created yet
  int brush;	// ref to solid brush, 0 if none created yet
} *fl_current_xmap;
inline COLORREF fl_RGB() {return fl_current_xmap->rgb;}
inline HPEN fl_pen() {return fl_current_xmap->pen;}
HBRUSH fl_brush(); // allocates a brush if necessary

extern FL_API HINSTANCE fl_display;
extern FL_API HDC fl_gc;
extern FL_API Window fl_window;
extern FL_API HFONT fl_xfont;
extern FL_API HPALETTE fl_palette; // non-zero only on 8-bit displays!
extern FL_API HDC fl_GetDC(Window);
extern FL_API MSG fl_msg;

// off-screen pixmaps: create, destroy, draw into, copy to window
#define Fl_Offscreen HBITMAP
#define fl_create_offscreen(w, h) CreateCompatibleBitmap(fl_gc, w, h)

extern FL_API HDC fl_makeDC(HBITMAP);
#define fl_begin_offscreen(b) \
  HDC _sgc=fl_gc; Window _sw=fl_window; \
  fl_gc=fl_makeDC(b); fl_window=(HWND)b; fl_push_no_clip()

#define fl_end_offscreen() \
  fl_pop_clip(); DeleteDC(fl_gc); fl_window=_sw; fl_gc = _sgc

void fl_copy_offscreen(int x,int y,int w,int h,HBITMAP pixmap,int srcx,int srcy);
#define fl_delete_offscreen(bitmap) DeleteObject(bitmap);

// binary images, created from in-memory data:
// See Fl_Image::_draw() for code that uses these.
extern FL_API ulong fl_create_bitmap(const uchar* bitmap, int w, int h);
#define fl_delete_bitmap(bitmap) DeleteObject((HBITMAP)(bitmap));

#endif

//
// End of "$Id: win32.H,v 1.27 2000/01/10 06:31:04 bill Exp $".
//
