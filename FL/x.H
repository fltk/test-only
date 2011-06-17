//
// "$Id$"
//
// X11 header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
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

#ifndef _FL_X_H_
#define _FL_X_H_

#include <fltk3/x.h>
#include "Enumerations.H"

typedef fltk3::Region Fl_Region;

#if 0 // FIXME: 123

FL_EXPORT void fl_open_display();
FL_EXPORT void fl_open_display(Display*);
FL_EXPORT void fl_close_display();
extern FL_EXPORT Display *fl_display;
extern FL_EXPORT int fl_screen;
extern FL_EXPORT XVisualInfo *fl_visual;
extern FL_EXPORT Colormap fl_colormap;
extern FL_EXPORT GC fl_gc;
extern FL_EXPORT Window fl_window;
FL_EXPORT ulong fl_xpixel(Fl_Color i);
FL_EXPORT ulong fl_xpixel(uchar r, uchar g, uchar b);
FL_EXPORT void fl_clip_region(Fl_Region);
FL_EXPORT Fl_Region fl_clip_region();
FL_EXPORT int fl_handle(const XEvent&);
extern FL_EXPORT const XEvent* fl_xevent;
extern FL_EXPORT ulong fl_event_time;
typedef ulong Fl_Offscreen;
#   define fl_create_offscreen(w,h) \
  XCreatePixmap(fl_display, \
	      (Fl_Surface_Device::surface()->class_name() == Fl_Display_Device::class_id ? \
	      fl_window : fl_xid(Fl::first_window()) ) , \
	      w, h, fl_visual->depth)
// begin/end are macros that save the old state in local variables:
#    define fl_begin_offscreen(pixmap) \
  Window _sw=fl_window; fl_window=pixmap; \
  Fl_Surface_Device *_ss = Fl_Surface_Device::surface(); Fl_Display_Device::display_device()->set_current(); \
  fl_push_no_clip()
#    define fl_end_offscreen() \
  fl_pop_clip(); fl_window = _sw; _ss->set_current()

extern void fl_copy_offscreen(int x, int y, int w, int h, Fl_Offscreen pixmap, int srcx, int srcy);
#    define fl_delete_offscreen(pixmap) XFreePixmap(fl_display, pixmap)
typedef ulong Fl_Bitmask;
extern FL_EXPORT Fl_Bitmask fl_create_bitmask(int w, int h, const uchar *data);
extern FL_EXPORT Fl_Bitmask fl_create_alphamask(int w, int h, int d, int ld, const uchar *data);
extern FL_EXPORT void fl_delete_bitmask(Fl_Bitmask bm);
#if defined(FL_LIBRARY) || defined(FL_INTERNALS)
extern FL_EXPORT Window fl_message_window;
extern FL_EXPORT void *fl_xftfont;
FL_EXPORT Fl_Region XRectangleRegion(int x, int y, int w, int h); // in fl_rect.cxx

class Fl_XFont_On_Demand
{
public:
  Fl_XFont_On_Demand(XFontStruct* p = NULL) : ptr(p) { }
  Fl_XFont_On_Demand& operator=(const Fl_XFont_On_Demand& x)
  { ptr = x.ptr;  return *this; }
  Fl_XFont_On_Demand& operator=(XFontStruct* p)
  { ptr = p;  return *this; }
  XFontStruct* value();
  operator XFontStruct*() { return value(); }
  XFontStruct& operator*() { return *value(); }
  XFontStruct* operator->() { return value(); }
  bool operator==(const Fl_XFont_On_Demand& x) { return ptr == x.ptr; }
  bool operator!=(const Fl_XFont_On_Demand& x) { return ptr != x.ptr; }
private:
  XFontStruct *ptr;
};
extern FL_EXPORT Fl_XFont_On_Demand fl_xfont;

class FL_EXPORT Fl_X {
public:
  Window xid;
  Window other_xid;
  Fl_Window *w;
  Fl_Region region;
  Fl_X *next;
  char wait_for_expose;
  char backbuffer_bad; // used for XDBE
  static Fl_X* first;
  static Fl_X* i(const Fl_Window* wi) {return wi->i;}
  void setwindow(Fl_Window* wi) {w=wi; wi->i=this;}
  void sendxjunk();
  static void make_xid(Fl_Window*,XVisualInfo* =fl_visual, Colormap=fl_colormap);
  static Fl_X* set_xid(Fl_Window*, Window);
  // kludges to get around protection:
  void flush() {w->flush();}
  static void x(Fl_Window* wi, int X) {wi->x(X);}
  static void y(Fl_Window* wi, int Y) {wi->y(Y);}
};

extern FL_EXPORT char fl_override_redirect; // hack into Fl_X::make_xid()
extern FL_EXPORT int fl_background_pixel;  // hack into Fl_X::make_xid()

inline Window fl_xid(const Fl_Window* w) { return Fl_X::i(w)->xid; }

#else

extern Window fl_xid_(const Fl_Window* w);
#define fl_xid(w) fl_xid_(w)

#endif // FL_LIBRARY || FL_INTERNALS

FL_EXPORT Fl_Window* fl_find(Window xid);

inline void fl_open_callback(void (*)(const char *)) {}

extern FL_EXPORT int fl_parse_color(const char* p, uchar& r, uchar& g, uchar& b);

#endif

#endif

//
// End of "$Id$".
//
