//
// "$Id: fl_cursor.cxx,v 1.24 2004/07/19 23:33:05 laza2000 Exp $"
//
// Mouse cursor support for the Fast Light Tool Kit (FLTK).
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

// Change the current cursor.
//
// This needs a lot of work and is commented out for now...
//
// Under X the cursor is attached to the X window.  I tried to hide
// this and pretend that changing the cursor is a drawing function.
// This avoids a field in the Window, and I suspect is more
// portable to other systems.

#include <config.h>
#include <fltk/Cursor.h>
#include <fltk/Window.h>
#include <fltk/x.h>
#include <fltk/draw.h>
#include <fltk/Color.h>

using namespace fltk;

/*! \fn void Widget::cursor(Cursor* c) const

  Change the cursor being displayed on the screen. A widget should do
  this in response to \c fltk::ENTER and \c fltk::MOVE events. FLTK will change
  it back to \c fltk::CURSOR_DEFAULT if the mouse is moved outside this
  widget, unless another widget calls this.

  On X you can mess with the colors by setting the Color variables
  \c fl_cursor_fg and \c fl_cursor_bg to the colors you want, before calling
  this.
*/

/*! \class fltk::Cursor

  Cursor is an opaque system-dependent class. Currently you can only
  use the built-in cursors but a method to turn an Image into a Cursor
  will be added in the future.

  To display a cursor, call Widget::cursor().

  Built-in cursors are:
  - \c fltk::CURSOR_DEFAULT - the default cursor, usually an arrow.
  - \c fltk::CURSOR_ARROW - up-left arrow pointer
  - \c fltk::CURSOR_CROSS - crosshairs
  - \c fltk::CURSOR_WAIT - watch or hourglass
  - \c fltk::CURSOR_INSERT - I-beam
  - \c fltk::CURSOR_HAND - hand / pointing finger
  - \c fltk::CURSOR_HELP - question mark
  - \c fltk::CURSOR_MOVE - 4-pointed arrow
  - \c fltk::CURSOR_NS - up/down arrow
  - \c fltk::CURSOR_WE - left/right arrow
  - \c fltk::CURSOR_NWSE - diagonal arrow
  - \c fltk::CURSOR_NESW - diagonal arrow
  - \c fltk::CURSOR_NO - circle with slash
  - \c fltk::CURSOR_NONE - invisible
*/

#ifdef __sgi
FL_API Color fl_cursor_fg = RED;
#else
FL_API Color fl_cursor_fg = BLACK;
#endif
FL_API Color fl_cursor_bg = WHITE;

#ifdef DOXYGEN
// don't let it print internal stuff
////////////////////////////////////////////////////////////////
#elif USE_X11

struct fltk::Cursor {
  ::Cursor cursor;
  uchar fontid;
  uchar tableid;
};

fltk::Cursor *fltk::cursor(void *raw) {
  fltk::Cursor *c = new fltk::Cursor;
  c->cursor = (::Cursor)raw;
  c->fontid = 0;
  c->tableid = 0;
  return c;
}

static fltk::Cursor arrow = {0,35};
static fltk::Cursor cross = {0,66};
static fltk::Cursor wait = {0,76};
static fltk::Cursor insert = {0,77};
static fltk::Cursor hand = {0,31};
static fltk::Cursor help = {0,47};
static fltk::Cursor move = {0,27};
static fltk::Cursor ns = {0,0,0};
static fltk::Cursor we = {0,0,1};
static fltk::Cursor nwse = {0,0,2};
static fltk::Cursor nesw = {0,0,3};
static fltk::Cursor no = {0,0,4};
static fltk::Cursor none = {0,0,5};

// this probably should be a nicer bitmap:
fltk::Cursor fl_drop_ok_cursor = {0,21};

#define CURSORSIZE 16
#define HOTXY 8
static struct TableEntry {
  uchar bits[CURSORSIZE*CURSORSIZE/8];
  uchar mask[CURSORSIZE*CURSORSIZE/8];
} table[] = {
  {{	// CURSOR_NS
   0x00, 0x00, 0x80, 0x01, 0xc0, 0x03, 0xe0, 0x07, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00},
   {
   0x80, 0x01, 0xc0, 0x03, 0xe0, 0x07, 0xf0, 0x0f, 0xf0, 0x0f, 0xc0, 0x03,
   0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xf0, 0x0f,
   0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01}},
  {{	// CURSOR_EW
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10,
   0x0c, 0x30, 0xfe, 0x7f, 0xfe, 0x7f, 0x0c, 0x30, 0x08, 0x10, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x1c, 0x38,
   0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0x1c, 0x38, 0x18, 0x18,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
  {{	// CURSOR_NWSE
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x38, 0x00, 0x78, 0x00,
   0xe8, 0x00, 0xc0, 0x01, 0x80, 0x03, 0x00, 0x17, 0x00, 0x1e, 0x00, 0x1c,
   0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {
   0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0xfc, 0x00, 0x7c, 0x00, 0xfc, 0x00,
   0xfc, 0x01, 0xec, 0x03, 0xc0, 0x37, 0x80, 0x3f, 0x00, 0x3f, 0x00, 0x3e,
   0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00}},
  {{	// CURSOR_NESW
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x1c, 0x00, 0x1e,
   0x00, 0x17, 0x80, 0x03, 0xc0, 0x01, 0xe8, 0x00, 0x78, 0x00, 0x38, 0x00,
   0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3e, 0x00, 0x3f,
   0x80, 0x3f, 0xc0, 0x37, 0xec, 0x03, 0xfc, 0x01, 0xfc, 0x00, 0x7c, 0x00,
   0xfc, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00}},
  {{	// CURSOR_NO
   0x00, 0x00, 0xc0, 0x03, 0xf0, 0x0f, 0x38, 0x1c, 0x3c, 0x30, 0x7c, 0x30,
   0xe6, 0x60, 0xc6, 0x61, 0x86, 0x63, 0x06, 0x67, 0x0c, 0x3e, 0x0c, 0x3c,
   0x38, 0x1c, 0xf0, 0x0f, 0xc0, 0x03, 0x00, 0x00},
   {
   0xc0, 0x03, 0xf0, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0x7e, 0x7c, 0xfe, 0x78,
   0xff, 0xf1, 0xef, 0xf3, 0xcf, 0xf7, 0x8f, 0xff, 0x1e, 0x7f, 0x3e, 0x7e,
   0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xc0, 0x03}},
  {{0}, {0}} // CURSOR_NONE
};

void Widget::cursor(fltk::Cursor* c) const {
  Window* window = is_window() ? (Window*)this : this->window();
  if (!window) return;
  CreatedWindow* i = CreatedWindow::find(window);
  if (!i) return;
  ::Cursor xcursor;
  if (!c) {
    xcursor = None;
  } else {
    // Figure out the X cursor if not already figured out:
    if (!c->cursor) {
      if (c->fontid) {
	c->cursor = XCreateFontCursor(xdisplay, (c->fontid-1)*2);
      } else {
	TableEntry *q = table+c->tableid;
	XColor dummy;
	Pixmap p = XCreateBitmapFromData(xdisplay,
		RootWindow(xdisplay, xscreen), (const char*)(q->bits),
		CURSORSIZE, CURSORSIZE);
	Pixmap m = XCreateBitmapFromData(xdisplay,
		RootWindow(xdisplay, xscreen), (const char*)(q->mask),
		CURSORSIZE, CURSORSIZE);
	c->cursor = XCreatePixmapCursor(xdisplay, p,m,&dummy, &dummy,
					HOTXY, HOTXY);
	XFreePixmap(xdisplay, m);
	XFreePixmap(xdisplay, p);
      }
      uchar r,g,b;
      XColor fgc;
      split_color(fl_cursor_fg, r,g,b);
      fgc.red = r*0x101;
      fgc.green = g*0x101;
      fgc.blue = b*0x101;
      XColor bgc;
      split_color(fl_cursor_bg, r,g,b);
      bgc.red = r*0x101;
      bgc.green = g*0x101;
      bgc.blue = b*0x101;
      XRecolorCursor(xdisplay, c->cursor, &fgc, &bgc);
    }
    xcursor = c->cursor;
  }
  i->cursor_for = this;
  if (xcursor != i->cursor) {
    i->cursor = xcursor;
    XDefineCursor(xdisplay, i->xid, xcursor);
  }
}

////////////////////////////////////////////////////////////////
#elif defined(_WIN32)

struct fltk::Cursor {
  LPTSTR resource;
  HCURSOR cursor;
};

fltk::Cursor *fltk::cursor(void *raw) {
  fltk::Cursor *c = new fltk::Cursor;
  c->cursor = (HCURSOR)raw;
  c->resource = 0;
  return c;
}

static fltk::Cursor arrow = {TEXT(IDC_ARROW)};
static fltk::Cursor cross = {TEXT(IDC_CROSS)};
static fltk::Cursor wait = {TEXT(IDC_WAIT)};
static fltk::Cursor insert = {TEXT(IDC_IBEAM)};
#ifndef IDC_HAND
# define IDC_HAND IDC_UPARROW
#endif
static fltk::Cursor hand = {TEXT(IDC_HAND)};
static fltk::Cursor help = {TEXT(IDC_HELP)};
static fltk::Cursor move = {TEXT(IDC_SIZEALL)};
static fltk::Cursor ns = {TEXT(IDC_SIZENS)};
static fltk::Cursor we = {TEXT(IDC_SIZEWE)};
static fltk::Cursor nwse = {TEXT(IDC_SIZENWSE)};
static fltk::Cursor nesw = {TEXT(IDC_SIZENESW)};
static fltk::Cursor no = {TEXT(IDC_NO)};
static fltk::Cursor none = {0};

void Widget::cursor(fltk::Cursor* c) const {
  Window* window = is_window() ? (Window*)this : this->window();
  if (!window) return;
  while (window->parent()) window = window->window();
  CreatedWindow* i = CreatedWindow::find(window);
  if (!i) return;
  HCURSOR xcursor;
  if (!c) {
    xcursor = default_cursor;
  } else {
    if (!c->cursor && c->resource) c->cursor = LoadCursor(NULL, c->resource);
    xcursor = c->cursor;
  }
  i->cursor_for = this;
  if (xcursor != i->cursor) {
    i->cursor = xcursor;
    SetCursor(xcursor);
  }
}

////////////////////////////////////////////////////////////////
#elif defined(__APPLE__)

struct fltk::Cursor {
  ::Cursor* cursor;
  int resource;
};

fltk::Cursor *fltk::cursor(void *raw) {
  fltk::Cursor *c = new fltk::Cursor;
  c->cursor = (::Cursor)raw;
  c->resource = 0;
  return c;
}

static ::Cursor crsrHAND =
{
  { 0x0600, 0x0900, 0x0900, 0x0900, 0x09C0, 0x0938, 0x6926, 0x9805,
    0x8801, 0x4801, 0x2002, 0x2002, 0x1004, 0x0804, 0x0408, 0x0408 },
  { 0x0600, 0x0F00, 0x0F00, 0x0F00, 0x0FC0, 0x0FF8, 0x6FFE, 0xFFFF,
    0xFFFF, 0x7FFF, 0x3FFE, 0x3FFE, 0x1FFC, 0x0FFC, 0x07F8, 0x07F8 },
  { 1, 5 } // Hotspot: ( y, x )
};
static fltk::Cursor hand = {&crsrHAND};

static ::Cursor crsrHELP =
{
  { 0x0000, 0x4000, 0x6000, 0x7000, 0x783C, 0x7C7E, 0x7E66, 0x7F06,
    0x7F8C, 0x7C18, 0x6C18, 0x4600, 0x0618, 0x0318, 0x0300, 0x0000 },
  { 0xC000, 0xE000, 0xF000, 0xF83C, 0xFC7E, 0xFEFF, 0xFFFF, 0xFFFF,
    0xFFFE, 0xFFFC, 0xFE3C, 0xEF3C, 0xCF3C, 0x07BC, 0x0798, 0x0380 },
  { 1, 1 }
};
static fltk::Cursor help = {&crsrHELP};

static ::Cursor crsrMOVE =
{
  { 0x0000, 0x0180, 0x03C0, 0x07E0, 0x07E0, 0x1998, 0x399C, 0x7FFE,
    0x7FFE, 0x399C, 0x1998, 0x07E0, 0x07E0, 0x03C0, 0x0180, 0x0000 },
  { 0x0180, 0x03C0, 0x07E0, 0x0FF0, 0x1FF8, 0x3FFC, 0x7FFE, 0xFFFF,
    0xFFFF, 0x7FFE, 0x3FFC, 0x1FF8, 0x0FF0, 0x07E0, 0x03C0, 0x0180 },
  { 8, 8 }
};
static fltk::Cursor move = {&crsrMOVE};

static ::Cursor crsrNS =
{
  { 0x0000, 0x0180, 0x03C0, 0x07E0, 0x0FF0, 0x0180, 0x0180, 0x0180,
    0x0180, 0x0180, 0x0180, 0x0FF0, 0x07E0, 0x03C0, 0x0180, 0x0000 },
  { 0x0180, 0x03C0, 0x07E0, 0x0FF0, 0x1FF8, 0x1FF8, 0x03C0, 0x03C0,
    0x03C0, 0x03C0, 0x1FF8, 0x1FF8, 0x0FF0, 0x07E0, 0x03C0, 0x0180 },
  { 8, 8 }
};
static fltk::Cursor ns = {&crsrNS};

static ::Cursor crsrWE =
{
  { 0x0000, 0x0000, 0x0000, 0x0000, 0x0810, 0x1818, 0x381C, 0x7FFE,
    0x7FFE, 0x381C, 0x1818, 0x0810, 0x0000, 0x0000, 0x0000, 0x0000 },
  { 0x0000, 0x0000, 0x0000, 0x0C30, 0x1C38, 0x3C3C, 0x7FFE, 0xFFFF,
    0xFFFF, 0x7FFE, 0x3C3C, 0x1C38, 0x0C30, 0x0000, 0x0000, 0x0000 },
  { 8, 8 }
};
static fltk::Cursor we = {&crsrWE};

static ::Cursor crsrNWSE =
{
  { 0x0000, 0x7E00, 0x7C00, 0x7800, 0x7C00, 0x6E00, 0x4710, 0x03B0,
    0x01F0, 0x00F0, 0x01F0, 0x03F0, 0x0000, 0x0000, 0x0000, 0x0000 },
  { 0xFF00, 0xFF00, 0xFE00, 0xFC00, 0xFE00, 0xFF18, 0xEFB8, 0xC7F8,
    0x03F8, 0x01F8, 0x03F8, 0x07F8, 0x07F8, 0x0000, 0x0000, 0x0000 },
  { 8, 8 }
};
static fltk::Cursor nwse = {&crsrNWSE};

static ::Cursor crsrNESW =
{
  { 0x0000, 0x03F0, 0x01F0, 0x00F0, 0x01F0, 0x03B0, 0x4710, 0x6E00,
    0x7C00, 0x7800, 0x7C00, 0x7E00, 0x0000, 0x0000, 0x0000, 0x0000 },
  { 0x07F8, 0x07F8, 0x03F8, 0x01F8, 0x03F8, 0xC7F8, 0xEFB8, 0xFF18,
    0xFE00, 0xFC00, 0xFE00, 0xFF00, 0xFF00, 0x0000, 0x0000, 0x0000 },
  { 8, 8 }
};
static fltk::Cursor nesw = {&crsrNESW};

static ::Cursor crsrNONE =
{
  { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
  { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
  { 0, 0 }
};
static fltk::Cursor none = {&crsrNONE};

static ::Cursor crsrARROW =
{
  { 0x0000, 0x4000, 0x6000, 0x7000, 0x7800, 0x7C00, 0x7E00, 0x7F00,
    0x7F80, 0x7C00, 0x6C00, 0x4600, 0x0600, 0x0300, 0x0300, 0x0000 },
  { 0xC000, 0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00, 0xFF80,
    0xFFC0, 0xFFC0, 0xFE00, 0xEF00, 0xCF00, 0x0780, 0x0780, 0x0380 },
  { 1, 1 }
};
static fltk::Cursor arrow = {&crsrARROW};

static fltk::Cursor cross = {0, crossCursor};
static fltk::Cursor wait  = {0, watchCursor};
static fltk::Cursor insert= {0, iBeamCursor};

static ::Cursor crsrNO =
{
  { 0x0000, 0x03c0, 0x0ff0, 0x1c38, 0x303c, 0x307c, 0x60e6, 0x61c6,
    0x6386, 0x6706, 0x3e0c, 0x3c0c, 0x1c38, 0x0ff0, 0x03c0, 0x0000 },
  { 0x03c0, 0x0ff0, 0x1ff8, 0x3ffc, 0x7c7e, 0x78fe, 0xf1ff, 0xf3ef,
    0xf7cf, 0xff8f, 0x7f1e, 0x7e3e, 0x3ffc, 0x1ff8, 0x0ff0, 0x03c0 },
  { 8, 8 }
};
static fltk::Cursor no = {&crsrNO};

/*  Setting the cursor on the Macintosh is very easy, as there is a
    single cursor rather than one per window. The main loop checks to
    see if we are still pointing at the cursor widget and puts the
    cursor back to the default if not. */
void Widget::cursor(fltk::Cursor* c) const {
  CursHandle xcursor;
  if (!c) {
    xcursor = fltk::default_cursor;
  } else {
    if (!c->cursor) c->cursor = *GetCursor(c->resource);
    xcursor = &c->cursor;
  }
  fltk::cursor_for = this;
  if (fltk::current_cursor != xcursor) {
    fltk::current_cursor = xcursor;
    SetCursor(*xcursor);
  }
}

#endif

fltk::Cursor* const fltk::CURSOR_DEFAULT= 0;
fltk::Cursor* const fltk::CURSOR_ARROW	= &arrow;
fltk::Cursor* const fltk::CURSOR_CROSS	= &cross;
fltk::Cursor* const fltk::CURSOR_WAIT	= &wait;
fltk::Cursor* const fltk::CURSOR_INSERT	= &insert;
fltk::Cursor* const fltk::CURSOR_HAND	= &hand;
fltk::Cursor* const fltk::CURSOR_HELP	= &help;
fltk::Cursor* const fltk::CURSOR_MOVE	= &move;
fltk::Cursor* const fltk::CURSOR_NS	= &ns;
fltk::Cursor* const fltk::CURSOR_WE	= &we;
fltk::Cursor* const fltk::CURSOR_NWSE	= &nwse;
fltk::Cursor* const fltk::CURSOR_NESW	= &nesw;
fltk::Cursor* const fltk::CURSOR_NO	= &no;
fltk::Cursor* const fltk::CURSOR_NONE	= &none;

//
// End of "$Id: fl_cursor.cxx,v 1.24 2004/07/19 23:33:05 laza2000 Exp $".
//
