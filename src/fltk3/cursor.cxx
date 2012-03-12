//
// "$Id$"
//
// Mouse cursor support for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

// Change the current cursor.
// Under X the cursor is attached to the X window.  I tried to hide
// this and pretend that changing the cursor is a drawing function.
// This avoids a field in the fltk3::Window, and I suspect is more
// portable to other systems.

#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/x.h>
#if !defined(WIN32) && !defined(__APPLE__)
#  include <X11/cursorfont.h>
#endif
#include <fltk3/draw.h>

/**
  Sets the cursor for the current window to the specified shape and colors.
  The cursors are defined in the <fltk3/enumerations.h> header file. 
  */
void fltk3::cursor(fltk3::Cursor c, fltk3::Color fg, fltk3::Color bg) {
  if (fltk3::first_window()) fltk3::first_window()->cursor(c,fg,bg);
}
/** 
    Sets the default window cursor as well as its color.

    For back compatibility only.
*/
void fltk3::Window::default_cursor(fltk3::Cursor c, fltk3::Color fg, fltk3::Color bg) {
//  if (c == fltk3::CURSOR_DEFAULT) c = fltk3::CURSOR_ARROW;

  cursor_default = c;
  cursor_fg      = fg;
  cursor_bg      = bg;

  cursor(c, fg, bg);
}

#ifdef WIN32

#  ifndef IDC_HAND
#    define IDC_HAND	MAKEINTRESOURCE(32649)
#  endif // !IDC_HAND

void fltk3::Window::cursor(fltk3::Cursor c, fltk3::Color c1, fltk3::Color c2) {
  if (!shown()) return;
  // the cursor must be set for the top level window, not for subwindows
  fltk3::Window *w = window(), *toplevel = this;
  while (w) { toplevel = w; w = w->window(); }
  if (toplevel != this) { toplevel->cursor(c, c1, c2); return; }
  // now set the actual cursor
  if (c == fltk3::CURSOR_DEFAULT) {
    c = cursor_default;
  }
  if (c > fltk3::CURSOR_NESW) {
    i->cursor = 0;
  } else if (c == fltk3::CURSOR_DEFAULT) {
    i->cursor = fl_default_cursor;
  } else {
    LPSTR n;
    switch (c) {
    case fltk3::CURSOR_ARROW:	n = IDC_ARROW; break;
    case fltk3::CURSOR_CROSS:	n = IDC_CROSS; break;
    case fltk3::CURSOR_WAIT:	n = IDC_WAIT; break;
    case fltk3::CURSOR_INSERT:	n = IDC_IBEAM; break;
    case fltk3::CURSOR_HELP:	n = IDC_HELP; break;
    case fltk3::CURSOR_HAND: {
          OSVERSIONINFO osvi;

          // Get the OS version: Windows 98 and 2000 have a standard
	  // hand cursor.
          memset(&osvi, 0, sizeof(OSVERSIONINFO));
          osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
          GetVersionEx(&osvi);

          if (osvi.dwMajorVersion > 4 ||
  	      (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion > 0 &&
  	       osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)) n = IDC_HAND;
          else n = IDC_UPARROW;
	} break;
    case fltk3::CURSOR_MOVE:	n = IDC_SIZEALL; break;
    case fltk3::CURSOR_N:
    case fltk3::CURSOR_S:
    case fltk3::CURSOR_NS:		n = IDC_SIZENS; break;
    case fltk3::CURSOR_NE:
    case fltk3::CURSOR_SW:
    case fltk3::CURSOR_NESW:	n = IDC_SIZENESW; break;
    case fltk3::CURSOR_E:
    case fltk3::CURSOR_W:
    case fltk3::CURSOR_WE:		n = IDC_SIZEWE; break;
    case fltk3::CURSOR_SE:
    case fltk3::CURSOR_NW:
    case fltk3::CURSOR_NWSE:	n = IDC_SIZENWSE; break;
    default:			n = IDC_NO; break;
    }
    i->cursor = LoadCursor(NULL, n);
  }
  SetCursor(i->cursor);
}

#elif defined(__APPLE__)

#ifdef __BIG_ENDIAN__
# define E(x) x
#elif defined __LITTLE_ENDIAN__
// Don't worry. This will be resolved at compile time
# define E(x) (x>>8)|((x<<8)&0xff00)
#else
# error "Either __LITTLE_ENDIAN__ or __BIG_ENDIAN__ must be defined"
#endif

CGContextRef Fl_X::help_cursor_image(void)
{
  int w = 20, h = 20;
  fltk3::Offscreen off = fltk3::QuartzGraphicsDriver::create_offscreen_with_alpha(w, h);
  fl_begin_offscreen(off);
  CGContextSetRGBFillColor( (CGContextRef)off, 0,0,0,0);
  fltk3::rectf(0,0,w,h);
  fltk3::color(fltk3::BLACK);
  fltk3::font(fltk3::COURIER_BOLD, 20);
  fltk3::draw("?", 1, h-1);
  fl_end_offscreen();
  return (CGContextRef)off;
}

CGContextRef Fl_X::none_cursor_image(void)
{
  int w = 20, h = 20;
  fltk3::Offscreen off = fltk3::QuartzGraphicsDriver::create_offscreen_with_alpha(w, h);
  fl_begin_offscreen(off);
  CGContextSetRGBFillColor( (CGContextRef)off, 0,0,0,0);
  fltk3::rectf(0,0,w,h);
  fl_end_offscreen();
  return (CGContextRef)off;
}

CGContextRef Fl_X::watch_cursor_image(void)
{
  int w, h, r = 5;
  w = 2*r+6;
  h = 4*r;
  fltk3::Offscreen off = fltk3::QuartzGraphicsDriver::create_offscreen_with_alpha(w, h);
  fl_begin_offscreen(off);
  CGContextSetRGBFillColor( (CGContextRef)off, 0,0,0,0);
  fltk3::rectf(0,0,w,h);
  CGContextTranslateCTM( (CGContextRef)off, w/2, h/2);
  fltk3::color(fltk3::WHITE);
  fltk3::circle(0, 0, r+1);
  fltk3::color(fltk3::BLACK);
  fltk3::rectf(int(-r*0.7), int(-r*1.7), int(1.4*r), int(3.4*r));
  fltk3::rectf(r-1, -1, 3, 3);
  fltk3::color(fltk3::WHITE);
  fltk3::pie(-r, -r, 2*r, 2*r, 0, 360);
  fltk3::color(fltk3::BLACK);
  fltk3::circle(0,0,r);
  fltk3::xyline(0, 0, int(-r*.7));
  fltk3::xyline(0, 0, 0, int(-r*.7));
  fl_end_offscreen();
  return (CGContextRef)off;
}

CGContextRef Fl_X::nesw_cursor_image(void)
{
  int c = 7, r = 2*c;
  int w = r, h = r;
  fltk3::Offscreen off = fltk3::QuartzGraphicsDriver::create_offscreen_with_alpha(w, h);
  fl_begin_offscreen(off);
  CGContextSetRGBFillColor( (CGContextRef)off, 0,0,0,0);
  fltk3::rectf(0,0,w,h);
  CGContextTranslateCTM( (CGContextRef)off, 0, h);
  CGContextScaleCTM( (CGContextRef)off, 1, -1);
  fltk3::color(fltk3::BLACK);
  fltk3::polygon(0, 0, c, 0, 0, c);
  fltk3::polygon(r, r, r, r-c, r-c, r);
  fltk3::line_style(fltk3::SOLID, 2, 0);
  fltk3::line(0,1, r,r+1);
  fltk3::line_style(fltk3::SOLID, 0, 0);
  fl_end_offscreen();
  return (CGContextRef)off;
}

CGContextRef Fl_X::nwse_cursor_image(void)
{
  int c = 7, r = 2*c;
  int w = r, h = r;
  fltk3::Offscreen off = fltk3::QuartzGraphicsDriver::create_offscreen_with_alpha(w, h);
  fl_begin_offscreen(off);
  CGContextSetRGBFillColor( (CGContextRef)off, 0,0,0,0);
  fltk3::rectf(0,0,w,h);
  CGContextTranslateCTM( (CGContextRef)off, 0, h);
  CGContextScaleCTM( (CGContextRef)off, 1, -1);
  fltk3::color(fltk3::BLACK);
  fltk3::polygon(r-1, 0, r-1, c, r-1-c, 0);
  fltk3::polygon(-1, r, c-1, r, -1, r-c);
  fltk3::line_style(fltk3::SOLID, 2, 0);
  fltk3::line(r-1,1, -1,r+1);
  fltk3::line_style(fltk3::SOLID, 0, 0);
  fl_end_offscreen();
  return (CGContextRef)off;
}

void fltk3::Window::cursor(fltk3::Cursor c, fltk3::Color, fltk3::Color) {
  if (c == fltk3::CURSOR_DEFAULT) {
    c = cursor_default;
  }
  if (i) i->set_cursor(c);
}

#else

// I like the MSWindows resize cursors, so I duplicate them here:

#define CURSORSIZE 16
#define HOTXY 7
static struct TableEntry {
  uchar bits[CURSORSIZE*CURSORSIZE/8];
  uchar mask[CURSORSIZE*CURSORSIZE/8];
  ::Cursor cursor;
} table[] = {
  {{	// fltk3::CURSOR_NS
   0x00, 0x00, 0x80, 0x01, 0xc0, 0x03, 0xe0, 0x07, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00},
   {
   0x80, 0x01, 0xc0, 0x03, 0xe0, 0x07, 0xf0, 0x0f, 0xf0, 0x0f, 0xc0, 0x03,
   0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xf0, 0x0f,
   0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01}},
  {{	// fltk3::CURSOR_EW
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10,
   0x0c, 0x30, 0xfe, 0x7f, 0xfe, 0x7f, 0x0c, 0x30, 0x08, 0x10, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x1c, 0x38,
   0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0x1c, 0x38, 0x18, 0x18,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
  {{	// fltk3::CURSOR_NWSE
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x38, 0x00, 0x78, 0x00,
   0xe8, 0x00, 0xc0, 0x01, 0x80, 0x03, 0x00, 0x17, 0x00, 0x1e, 0x00, 0x1c,
   0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {
   0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0xfc, 0x00, 0x7c, 0x00, 0xfc, 0x00,
   0xfc, 0x01, 0xec, 0x03, 0xc0, 0x37, 0x80, 0x3f, 0x00, 0x3f, 0x00, 0x3e,
   0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00}},
  {{	// fltk3::CURSOR_NESW
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x1c, 0x00, 0x1e,
   0x00, 0x17, 0x80, 0x03, 0xc0, 0x01, 0xe8, 0x00, 0x78, 0x00, 0x38, 0x00,
   0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3e, 0x00, 0x3f,
   0x80, 0x3f, 0xc0, 0x37, 0xec, 0x03, 0xfc, 0x01, 0xfc, 0x00, 0x7c, 0x00,
   0xfc, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00}},
  {{0}, {0}} // fltk3::CURSOR_NONE & unknown
};

void fltk3::Window::cursor(fltk3::Cursor c, fltk3::Color fg, fltk3::Color bg) {
  if (!shown()) return;
  ::Cursor xc;
  int deleteit = 0;
  if (c == fltk3::CURSOR_DEFAULT) {
    c  = cursor_default;
    fg = cursor_fg;
    bg = cursor_bg;
  }

  if (!c) {
    xc = None;
  } else {
    if (c >= fltk3::CURSOR_NS) {
      TableEntry *q = (c > fltk3::CURSOR_NESW) ? table+4 : table+(c-fltk3::CURSOR_NS);
      if (!(q->cursor)) {
	XColor dummy = { 0 };
	::Pixmap p = XCreateBitmapFromData(fl_display,
	  RootWindow(fl_display, fl_screen), (const char*)(q->bits),
	  CURSORSIZE, CURSORSIZE);
	::Pixmap m = XCreateBitmapFromData(fl_display,
	  RootWindow(fl_display, fl_screen), (const char*)(q->mask),
	  CURSORSIZE, CURSORSIZE);
	q->cursor = XCreatePixmapCursor(fl_display, p,m,&dummy, &dummy,
					HOTXY, HOTXY);
	XFreePixmap(fl_display, m);
	XFreePixmap(fl_display, p);
      }
      xc = q->cursor;
    } else {
      xc = XCreateFontCursor(fl_display, (c-1)*2);
      deleteit = 1;
    }
    XColor fgc;
    uchar r,g,b;
    fltk3::get_color(fg,r,g,b);
    fgc.red = r<<8; fgc.green = g<<8; fgc.blue = b<<8;
    XColor bgc;
    fltk3::get_color(bg,r,g,b);
    bgc.red = r<<8; bgc.green = g<<8; bgc.blue = b<<8;
    XRecolorCursor(fl_display, xc, &fgc, &bgc);
  }
  XDefineCursor(fl_display, fl_xid(this), xc);
  if (deleteit) XFreeCursor(fl_display, xc);
}

#endif

//
// End of "$Id$".
//
