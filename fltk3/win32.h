//
// "$Id: win32.h 8375 2011-02-05 16:55:04Z AlbrechtS $"
//
// WIN32 header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Do not directly include this file, instead use <fltk3/x.h>.  It will
// include this file if WIN32 is defined.  This is to encourage
// portability of even the system-specific code...

#ifndef FLTK3_DOXYGEN
#ifndef Fltk3_X_H
#  error "Never use <fltk3/win32.h> directly; include <fltk3/x.h> instead."
#endif // !Fltk3_X_H

#include <windows.h>

namespace fltk3 {
  typedef ::HRGN Region;
}
typedef HWND Window;
typedef POINT XPoint;

#include <fltk3/Window.h>

// this part is included only when compiling the FLTK library or if requested explicitly
#if defined(FL_LIBRARY) || defined(FLTK3_INTERNALS) 

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
struct XRectangle {int x, y, width, height;};
extern fltk3::Region XRectangleRegion(int x, int y, int w, int h);
inline void XDestroyRegion(fltk3::Region r) {DeleteObject(r);}
inline void XClipBox(fltk3::Region r,XRectangle* rect) {
    RECT win_rect; GetRgnBox(r,&win_rect);
    rect->x=win_rect.left;
    rect->y=win_rect.top;
    rect->width=win_rect.right-win_rect.left;
    rect->height=win_rect.bottom-win_rect.top;
}
#define XDestroyWindow(a,b) DestroyWindow(b)
#define XMapWindow(a,b) ShowWindow(b, SW_RESTORE)
#define XUnmapWindow(a,b) ShowWindow(b, SW_HIDE)

// this object contains all win32-specific stuff about a window:
// Warning: this object is highly subject to change!
class FLTK3_EXPORT Fl_X {
public:
  // member variables - add new variables only at the end of this block
  Window xid;
  HBITMAP other_xid; // for double-buffered windows
  fltk3::Window* w;
  fltk3::Region region;
  Fl_X *next;
  int wait_for_expose;
  HDC private_dc; // used for OpenGL
  HCURSOR cursor;
  HDC saved_hdc;  // saves the handle of the DC currently loaded
  // static variables, static functions and member functions
  static Fl_X* first;
  static Fl_X* i(const fltk3::Window* w) {return w->i;}
  static int fake_X_wm(const fltk3::Window* w,int &X, int &Y,
		                 int &bt,int &bx,int &by);
  void setwindow(fltk3::Window* wi) {w=wi; wi->i=this;}
  void flush() {w->flush();}
  void set_minmax(LPMINMAXINFO minmax);
  void mapraise();
  static Fl_X* make(fltk3::Window*);
};
extern FLTK3_EXPORT HCURSOR fl_default_cursor;
extern FLTK3_EXPORT UINT fl_wake_msg;
extern FLTK3_EXPORT char fl_override_redirect; // hack into fltk3::Window::make_xid()
extern FLTK3_EXPORT int fl_background_pixel;  // hack into fltk3::Window::make_xid()
extern FLTK3_EXPORT HPALETTE fl_palette; // non-zero only on 8-bit displays!
extern FLTK3_EXPORT void fl_release_dc(HWND w, HDC dc);
extern FLTK3_EXPORT void fl_save_dc( HWND w, HDC dc);

inline Window fl_xid(const fltk3::Window* w) { Fl_X *temp = Fl_X::i(w); return temp ? temp->xid : 0; }

#else
FLTK3_EXPORT Window fl_xid_(const fltk3::Window* w);
#define fl_xid(w) fl_xid_(w)
#endif // FL_LIBRARY || FLTK3_INTERNALS

FLTK3_EXPORT fltk3::Window* fl_find(Window xid);
FLTK3_EXPORT void fltk3::clip_region(fltk3::Region);

// most recent fltk3::color() or fl_rgbcolor() points at one of these:
extern FLTK3_EXPORT struct Fl_XMap {
  COLORREF rgb;	// this should be the type the RGB() macro returns
  HPEN pen;	// pen, 0 if none created yet
  int brush;	// ref to solid brush, 0 if none created yet
} *fl_current_xmap;
inline COLORREF fl_RGB() {return fl_current_xmap->rgb;}
inline HPEN fl_pen() {return fl_current_xmap->pen;}
FLTK3_EXPORT HBRUSH fl_brush(); // allocates a brush if necessary
FLTK3_EXPORT HBRUSH fl_brush_action(int); // now does the real work

extern FLTK3_EXPORT HINSTANCE fl_display;
extern FLTK3_EXPORT Window fl_window;
extern FLTK3_EXPORT HDC fl_gc;
extern FLTK3_EXPORT MSG fl_msg;
extern FLTK3_EXPORT HDC fl_GetDC(Window);
extern FLTK3_EXPORT HDC fl_makeDC(HBITMAP);

// off-screen pixmaps: create, destroy, draw into, copy to window
typedef HBITMAP fltk3::Offscreen;
#define fl_create_offscreen(w, h) \
  CreateCompatibleBitmap( (fl_gc ? fl_gc : fl_GetDC(0) ) , w, h)

# define fl_begin_offscreen(b) \
   HDC _sgc=fl_gc; Window _sw=fl_window; \
   fltk3::SurfaceDevice *_ss = fltk3::SurfaceDevice::surface(); fltk3::DisplayDevice::display_device()->set_current(); \
   fl_gc=fl_makeDC(b); int _savedc = SaveDC(fl_gc); fl_window=(HWND)b; fltk3::push_no_clip()

# define fl_end_offscreen() \
   fltk3::pop_clip(); RestoreDC(fl_gc, _savedc); DeleteDC(fl_gc); _ss->set_current(); fl_window=_sw; fl_gc = _sgc


FLTK3_EXPORT void fl_copy_offscreen(int x,int y,int w,int h,HBITMAP pixmap,int srcx,int srcy);
FLTK3_EXPORT void fl_copy_offscreen_with_alpha(int x,int y,int w,int h,HBITMAP pixmap,int srcx,int srcy);
#define fl_delete_offscreen(bitmap) DeleteObject(bitmap)

// Bitmap masks
namespace fltk3 {
  typedef HBITMAP Bitmask;
}

extern FLTK3_EXPORT fltk3::Bitmask fl_create_bitmask(int w, int h, const uchar *data);
extern FLTK3_EXPORT fltk3::Bitmask fl_create_alphamask(int w, int h, int d, int ld, const uchar *data);
extern FLTK3_EXPORT void fl_delete_bitmask(fltk3::Bitmask bm);

// Dummy function to register a function for opening files via the window manager...
inline void fl_open_callback(void (*)(const char *)) {}

extern FLTK3_EXPORT int fl_parse_color(const char* p, uchar& r, uchar& g, uchar& b);
#endif // FLTK3_DOXYGEN
//
// End of "$Id: win32.h 8375 2011-02-05 16:55:04Z AlbrechtS $".
//
