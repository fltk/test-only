//
// "$Id: fl_font_win32.cxx,v 1.33 2001/07/16 19:38:18 robertk Exp $"
//
// WIN32 font selection routines for the Fast Light Tool Kit (FLTK).
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

#include <FL/x.H>
#include "Fl_FontSize.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Fl_FontSize *fl_fontsize;

static void *
win_font_load(const char *name, const char *encoding, int size) {
  int weight = FW_NORMAL;
  int italic = 0;
  // may be efficient, but this is non-obvious
  switch (*name++) {
  case 'I': italic = 1; break;
  case 'P': italic = 1;
  case 'B': weight = FW_BOLD; break;
  case ' ': break;
  default: name--;
  }

  HFONT font = CreateFont(
    -size,	    // use "char size"
    0,		    // logical average character width
    0,		    // angle of escapement
    0,		    // base-line orientation angle
    weight,
    italic,
    FALSE,		// underline attribute flag
    FALSE,		// strikeout attribute flag
    (int)encoding,	// character set identifier
    OUT_DEFAULT_PRECIS, // output precision
    CLIP_DEFAULT_PRECIS,// clipping precision
    DEFAULT_QUALITY,	// output quality
    DEFAULT_PITCH,	// pitch and family
    name		// pointer to typeface name string
  );

  HDC screen =	GetDC(0);
  SelectObject(screen, font);
  GetTextMetrics(screen, &fl_fontsize->metr);
  //BOOL ret = GetCharWidthFloat(screen, metr.tmFirstChar, metr.tmLastChar, font->width+metr.tmFirstChar);
  //...would be the right call, but is not implemented into Window95! (WinNT?)
  GetCharWidth(screen, 0, 255, fl_fontsize->width);
  ReleaseDC(0, screen);

  return (void *)font;
}


Fl_FontSize::Fl_FontSize(const char* name, int size, int charset) {
  fl_fontsize = this;
  font = fl_font_renderer->load(name, (char *)charset, size);
#if HAVE_GL
  listbase = 0;
#endif
  minsize = maxsize = size;
  encoding = charset;
}

static void
win_font_unload(void *font) { DeleteObject((HFONT)font); }

#if HAVE_GL
Fl_FontSize::~Fl_FontSize() {
// Delete list created by gl_draw().  This is not done by this code
// as it will link in GL unnecessarily.  There should be some kind
// of "free" routine pointer, or a subclass?
// if (listbase) {
//  int base = font->min_char_or_byte2;
//  int size = font->max_char_or_byte2-base+1;
//  int base = 0; int size = 256;
//  glDeleteLists(listbase+base,size);
// }
  if (this == fl_fontsize) fl_fontsize = 0;
  fl_font_renderer->unload(font);
}
#endif

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:	bold:	    italic:
static Fl_Font_ win_fonts[] = {
{" Arial",				win_fonts+1, win_fonts+2},
{"BArial",				win_fonts+1, win_fonts+3},
{"IArial",				win_fonts+3, win_fonts+2},
{"PArial",				win_fonts+3, win_fonts+3},
{" Courier New",			win_fonts+5, win_fonts+6},
{"BCourier New",			win_fonts+5, win_fonts+7},
{"ICourier New",			win_fonts+7, win_fonts+6},
{"PCourier New",			win_fonts+7, win_fonts+7},
{" Times New Roman",			win_fonts+9, win_fonts+10},
{"BTimes New Roman",			win_fonts+9, win_fonts+11},
{"ITimes New Roman",			win_fonts+11,win_fonts+10},
{"PTimes New Roman",			win_fonts+11,win_fonts+11},
{" Symbol",				win_fonts+12,win_fonts+12},
{" Terminal",				win_fonts+14,win_fonts+14},
{"BTerminal",				win_fonts+14,win_fonts+14},
{" Wingdings",				win_fonts+15,win_fonts+15},
};

////////////////////////////////////////////////////////////////
// Public interface:

void *fl_xfont;

// Static variable for the default encoding:
const char *fl_encoding_ = (const char*)DEFAULT_CHARSET;

static void
win_font(Fl_Font font, unsigned size) {
  if (font == fl_font_ && size == fl_size_ &&
      fl_fontsize->encoding == (int)fl_encoding_) return;
  fl_font_ = font; fl_size_ = size;

  Fl_FontSize* f;
  // search the fontsizes we have generated already:
  for (f = (Fl_FontSize *)font->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size &&
	f->encoding == (int)fl_encoding_) break;
  if (!f) {
    f = new Fl_FontSize(font->name_, size, (int)fl_encoding_);
    f->next = (Fl_FontSize *)font->first;
    ((Fl_Font_*)font)->first = f;
  }
  if (f != fl_fontsize) {
    fl_fontsize = f;
    fl_xfont = f->font;
  }
}

static int
win_font_height() {
  return (fl_fontsize->metr.tmAscent + fl_fontsize->metr.tmDescent);
}

static int
win_font_descent() { return fl_fontsize->metr.tmDescent; }

// Change the encoding in use now. This runs the font search again with
// the new encoding.
void fl_encoding(const char* f) {
  if (f != fl_encoding_) {
    fl_encoding_ = f;
    if (fl_font_) fl_font(fl_font_, fl_size_);
  }
}

static int
win_font_width(const char* c, int n) {
  int w = 0;
  // Just adding the characters won't work on all Windows versions
  // since some of them (NT/2000/?) do kerning.
  //while (n--) w += fl_fontsize->width[uchar(*c++)];
  HDC screen =  GetDC(0);
  SelectObject(screen, (HFONT)fl_fontsize->font);
  SIZE size;
  if(GetTextExtentPoint32(screen, c, n, &size))
	  w = size.cx;
  ReleaseDC(0, screen);
  return w;
}

static void
win_font_draw(const char *str, int n, int x, int y) {
  SetTextColor(fl_gc, fl_colorref);
  SelectObject(fl_gc, (HFONT)fl_fontsize->font);
  TextOut(fl_gc, x+fl_x_, y+fl_y_, str, n);
}

static void
win_font_clip(Region) {} // handled by regular windows clipping

static Fl_Font_Renderer
win_renderer = {
  win_font, win_font_load, win_font_unload, win_font_height, win_font_descent,
  win_font_width, win_font_draw, win_font_clip, 0, win_fonts
};

Fl_Font_Renderer *fl_font_renderer = &win_renderer;



//
// End of "$Id: fl_font_win32.cxx,v 1.33 2001/07/16 19:38:18 robertk Exp $".
//
