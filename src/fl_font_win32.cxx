//
// "$Id: fl_font_win32.cxx,v 1.43 2002/09/09 01:39:58 spitzak Exp $"
//
// _WIN32 font selection routines for the Fast Light Tool Kit (FLTK).
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

#include <fltk/x.h>
#include <fltk/Fl_Font.h>
#include <ctype.h>
#include <stdlib.h>
#include <fltk/math.h>

class Fl_FontSize {
public:
  Fl_FontSize *next;	// linked list for a single Fl_Font_
  Fl_FontSize *next_all;// linked list so we can destroy em all
  unsigned size;
  int charset;
  int width[256];
  HFONT font;
  TEXTMETRIC metr;
  Fl_FontSize(const char* fontname, int size, int charset);
  ~Fl_FontSize();
};

static Fl_FontSize* fl_fontsize;
static Fl_FontSize* all_fonts;

Fl_FontSize::Fl_FontSize(const char* name, int size, int charset) {
  fl_fontsize = this;

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
    charset,		// character set identifier
    OUT_DEFAULT_PRECIS, // output precision
    CLIP_DEFAULT_PRECIS,// clipping precision
    DEFAULT_QUALITY,	// output quality
    DEFAULT_PITCH,	// pitch and family
    name		// pointer to typeface name string
  );

  HDC dc = fl_getDC();
  SelectObject(dc, font);
  GetTextMetrics(dc, &fl_fontsize->metr);
  //BOOL ret = GetCharWidthFloat(dc, metr.tmFirstChar, metr.tmLastChar, font->width+metr.tmFirstChar);
  //...would be the right call, but is not implemented into Window95! (WinNT?)
  //GetCharWidth(dc, 0, 255, fl_fontsize->width);

  this->font = font;
  this->size = size;
  this->charset = charset;
  next_all = all_fonts;
  all_fonts = this;
}

Fl_FontSize::~Fl_FontSize() {
  if (this == fl_fontsize) fl_fontsize = 0;
  DeleteObject(font);
}

// Deallocate Win32 fonts on exit. Warning: it will crash if you try
// to do any fonts after this, because the pointers are not changed!
void fl_font_rid() {
  for (Fl_FontSize* fontsize = all_fonts; fontsize;) {
    Fl_FontSize* next = fontsize->next;
    delete fontsize;
    fontsize = next;
  }
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:  bold:       italic:
Fl_Font_ fl_fonts[] = {
{" Arial",				fl_fonts+1, fl_fonts+2},
{"BArial", 				fl_fonts+1, fl_fonts+3},
{"IArial",				fl_fonts+3, fl_fonts+2},
{"PArial",				fl_fonts+3, fl_fonts+3},
{" Courier New",			fl_fonts+5, fl_fonts+6},
{"BCourier New",			fl_fonts+5, fl_fonts+7},
{"ICourier New",			fl_fonts+7, fl_fonts+6},
{"PCourier New",			fl_fonts+7, fl_fonts+7},
{" Times New Roman",			fl_fonts+9, fl_fonts+10},
{"BTimes New Roman",			fl_fonts+9, fl_fonts+11},
{"ITimes New Roman",			fl_fonts+11,fl_fonts+10},
{"PTimes New Roman",			fl_fonts+11,fl_fonts+11},
{" Symbol",				fl_fonts+12,fl_fonts+12},
{" Terminal",				fl_fonts+14,fl_fonts+14},
{"BTerminal",				fl_fonts+14,fl_fonts+14},
{" Wingdings",				fl_fonts+15,fl_fonts+15},
};

////////////////////////////////////////////////////////////////
// Public interface:

#define current_font (fl_fontsize->font)
HFONT fl_xfont() {return current_font;}
TEXTMETRIC* fl_textmetric() {return &(fl_fontsize->metr);}

// we need to decode the encoding somehow!
static int charset = DEFAULT_CHARSET;

void fl_font(Fl_Font font, double psize) {

  // only integers supported right now, I think there is a newer
  // interface that takes arbitrary sizes, though...
  psize = int(psize+.5);
  unsigned size = unsigned(psize);

  if (font == fl_font_ && psize == fl_size_ &&
      fl_fontsize->charset == charset) return;
  fl_font_ = font; fl_size_ = psize;

  Fl_FontSize* f;
  // search the fontsizes we have generated already:
  for (f = font->first; f; f = f->next)
    if (f->size == size && f->charset == charset) break;
  if (!f) {
    f = new Fl_FontSize(font->name_, size, charset);
    f->next = font->first;
    ((Fl_Font_*)font)->first = f;
  }
  fl_fontsize = f;
}

double fl_height() {
  return (fl_fontsize->metr.tmAscent + fl_fontsize->metr.tmDescent);
}

double fl_descent() { return fl_fontsize->metr.tmDescent; }
  
double fl_width(const char* c, int n) {
  SIZE size;
  HDC dc = fl_getDC();
  SelectObject(dc, current_font);
  // I think win32 has a fractional version of this:
  GetTextExtentPoint(dc, c, n, &size);
  return size.cx;
}

void fl_transformed_draw(const char *str, int n, double x, double y) {
  SetTextColor(fl_gc, fl_colorref);
  HGDIOBJ oldfont = SelectObject(fl_gc, current_font);
  TextOut(fl_gc, int(floor(x+.5)), int(floor(y+.5)), str, n);
  SelectObject(fl_gc, oldfont);
}

// Change the encoding to use for the next font selection.
// Encodings is NYI. We need a way to translate the ISO encoding names
// to Win32 encoding enumerations. Ie "iso8859-1" turns into ANSI_CHARSET,
// etc.
void fl_encoding(const char* f) {
  if (f != fl_encoding_) {
    fl_encoding_ = f;
    // charset = decode_the_encoding(f);
  }
}

//
// End of "$Id: fl_font_win32.cxx,v 1.43 2002/09/09 01:39:58 spitzak Exp $".
//
