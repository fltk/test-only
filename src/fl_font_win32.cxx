//
// "$Id: fl_font_win32.cxx,v 1.9.2.3.2.4.2.2 2003/11/07 03:47:24 easysw Exp $"
//
// WIN32 font selection routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#include <FL/fl_utf8.H>
#include "Fl_Font.H"
#include <stdio.h>


Fl_FontSize::Fl_FontSize(const char* name, int size) {
  int weight = FW_NORMAL;
  int italic = 0;
  switch (*name++) {
  case 'I': italic = 1; break;
  case 'P': italic = 1;
  case 'B': weight = FW_BOLD; break;
  case ' ': break;
  default: name--;
  }
  fid = CreateFont(
    -size, // negative makes it use "char size"
    0,	            // logical average character width 
    0,	            // angle of escapement 
    0,	            // base-line orientation angle 
    weight,
    italic,
    FALSE,	        // underline attribute flag 
    FALSE,	        // strikeout attribute flag 
    DEFAULT_CHARSET,    // character set identifier 
    OUT_DEFAULT_PRECIS,	// output precision 
    CLIP_DEFAULT_PRECIS,// clipping precision 
    DEFAULT_QUALITY,	// output quality 
    DEFAULT_PITCH,	// pitch and family 
    name	        // pointer to typeface name string 
    );
  if (!fl_gc) fl_GetDC(0);
  SelectObject(fl_gc, fid);
  GetTextMetrics(fl_gc, &metr);
//  BOOL ret = GetCharWidthFloat(fl_gc, metr.tmFirstChar, metr.tmLastChar, font->width+metr.tmFirstChar);
// ...would be the right call, but is not implemented into Window95! (WinNT?)
  //GetCharWidth(fl_gc, 0, 255, width);
  int i;
  for (i = 0; i < 64; i++) width[i] = NULL;
#if HAVE_GL
  listbase = 0;
  for (i = 0; i < 64; i++) glok[i] = 0;
#endif
  minsize = maxsize = size;
}

Fl_FontSize* fl_fontsize;

Fl_FontSize::~Fl_FontSize() {
#if HAVE_GL
// Delete list created by gl_draw().  This is not done by this code
// as it will link in GL unnecessarily.  There should be some kind
// of "free" routine pointer, or a subclass?
// if (listbase) {
//  int base = font->min_char_or_byte2;
//  int size = font->max_char_or_byte2-base+1;
//  int base = 0; int size = 256;
//  glDeleteLists(listbase+base,size);
// }
#endif
  if (this == fl_fontsize) fl_fontsize = 0;
  DeleteObject(fid);
  int i;
  for (i = 0; i < 64; i++) free(width[i]);
}

////////////////////////////////////////////////////////////////

// WARNING: if you add to this table, you must redefine FL_FREE_FONT
// in Enumerations.H & recompile!!
static Fl_Fontdesc built_in_table[] = {
{" Arial"},
{"BArial"},
{"IArial"},
{"PArial"},
{" Courier New"},
{"BCourier New"},
{"ICourier New"},
{"PCourier New"},
{" Times New Roman"},
{"BTimes New Roman"},
{"ITimes New Roman"},
{"PTimes New Roman"},
{" Symbol"},
{" Terminal"},
{"BTerminal"},
{" Wingdings"},
};

Fl_Fontdesc* fl_fonts = built_in_table;

static Fl_FontSize* find(int fnum, int size) {
  
  Fl_Fontdesc* s = fl_fonts+fnum;
  if (!s->name) s = fl_fonts; // use 0 if fnum undefined
  Fl_FontSize* f;
  for (f = s->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size) return f;
  f = new Fl_FontSize(s->name, size);
  f->next = s->first;
  s->first = f;
  return f;
}

////////////////////////////////////////////////////////////////
// Public interface:

FL_EXPORT int fl_font_ = 0;
FL_EXPORT int fl_size_ = 0;
//static HDC font_gc;

void Fl_Fltk::font(int fnum, int size) {
  if (fnum == fl_font_ && size == fl_size_) return;
  fl_font_ = fnum; fl_size_ = size;
  fl_fontsize = find(fnum, size);
}

int Fl_Fltk::height() {
  return (fl_fontsize->metr.tmAscent + fl_fontsize->metr.tmDescent);
}

int Fl_Fltk::descent() {
  return fl_fontsize->metr.tmDescent;
}


// Unicode string buffer
static xchar *wstr = NULL;
static int wstr_len	= 0;

double Fl_Fltk::width(const char* c, int n) {
  int i = 0;
  double w = 0;
  while (i < n) {
    unsigned int ucs;
    int l = fl_utf2ucs((const unsigned char*)c + i, n - i, &ucs);
    if (l < 1) l = 1; 
    i += l;
    if (!fl_nonspacing(ucs)) {
      w += Fl_Fltk::width(ucs);
    }
  }
  return w;
}

double Fl_Fltk::width(unsigned int c) { 
  unsigned int r;
  r = (c & 0xFC00) >> 10;
  if (!fl_fontsize->width[r]) {
     	SelectObject(fl_gc, fl_fontsize->fid);
     	fl_fontsize->width[r] = (int*) malloc(sizeof(int) * 0x0400);
        SIZE s;
	unsigned short i = 0, ii = r * 0x400;
	for (; i < 0x400; i++) {
		GetTextExtentPoint32W(fl_gc, (WCHAR*)&ii, 1, &s);
		fl_fontsize->width[r][i] = s.cx;
		ii++;
	} 
  }
  return (double) fl_fontsize->width[r][c & 0x03FF];
}

void Fl_Fltk::draw(const char* str, int n, int x, int y) {
  int i = 0;
  int lx = 0;
  COLORREF oldColor = SetTextColor(fl_gc, fl_RGB());
  SelectObject(fl_gc, fl_fontsize->fid);
  while (i < n) {
    unsigned int u;
    unsigned short ucs;
    int l = fl_utf2ucs((const unsigned char*)str + i, n - i, &u);
    if (fl_nonspacing(u)) {
	x -= lx;
    } else {
        lx = (int) Fl_Fltk::width(u);
    }
    ucs = u; 
    if (l < 1) l = 1;
    i += l;
    TextOutW(fl_gc, x, y, (WCHAR*)&ucs, 1);
    x += lx;
  }
  SetTextColor(fl_gc, oldColor);
}

void Fl_Fltk::rtl_draw(const char* c, int n, int x, int y) {
  int wn;
  int i = 0;
  int lx = 0;
  if (n > wstr_len) {
    wstr = (xchar*) realloc(wstr, sizeof(xchar) * n);
    wstr_len = n;
  }
  wn = fl_utf2unicode((const unsigned char *)c, n, wstr);
  COLORREF oldColor = SetTextColor(fl_gc, fl_RGB());
  SelectObject(fl_gc, fl_fontsize->fid);
  while (i < wn) {
    lx = (int) Fl_Fltk::width(wstr[i]);
    x -= lx;
    TextOutW(fl_gc, x, y, (WCHAR*)wstr + i, 1);
	if (fl_nonspacing(wstr[i])) {
		x += lx;
	}
    i++;
  }
  SetTextColor(fl_gc, oldColor);
}

//
// End of "$Id: fl_font_win32.cxx,v 1.9.2.3.2.4.2.2 2003/11/07 03:47:24 easysw Exp $".
//
