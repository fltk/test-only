//
// "$Id: fl_font_x.cxx,v 1.2 2001/02/21 06:15:45 clip Exp $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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

#include <FL/Fl.H>
#include <FL/x.H>
#include "Fl_FontSize.H"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void* fl_xfont;
static GC font_gc;

static void
set_current_fontsize(Fl_FontSize* f) {
  if (f != fl_fontsize) {
    fl_fontsize = f;
    fl_xfont = f->font;
    font_gc = 0;
  }
}

////////////////////////////////////////////////////////////////
// Things you can do once the font+size has been selected:

static void
x11_font_draw(const char *str, int n, int x, int y) {
  if (font_gc != fl_gc) {
    // I removed this, the user MUST set the font before drawing: (was)
    // if (!fl_xfont) fl_font(FL_HELVETICA, FL_NORMAL_SIZE);
    font_gc = fl_gc;
    XSetFont(fl_display, fl_gc, ((XFontStruct*)fl_xfont)->fid);
  }
  XDrawString(fl_display, fl_window, fl_gc, x+fl_x_, y+fl_y_, str, n);
}

void
fl_draw(const char* str, int n, int x, int y) {
  fl_font_renderer->draw(str, n, x, y);
}

void
fl_draw(const char* str, int x, int y) { fl_draw(str, strlen(str), x, y); }

static int
x11_font_height() {
  return (((XFontStruct*)fl_xfont)->ascent + ((XFontStruct*)fl_xfont)->descent);
}

int
fl_height() { return fl_font_renderer->height(); }

static int
x11_font_descent() { return ((XFontStruct*)fl_xfont)->descent; }

int
fl_descent() { return fl_font_renderer->descent(); }

static int
x11_font_width(const char *c, int n) {
  return XTextWidth((XFontStruct*)fl_xfont, c, n);
}

int
fl_width(const char* c, int n) { return fl_font_renderer->width(c, n); }

int
fl_width(const char* c) { return fl_width(c, strlen(c)); }

#if 0
// CET - why do it this way?
static int
x11_font_cwidth(uchar c) {
#if 1
  XCharStruct* p = ((XFontStruct*)fl_xfont)->per_char;
  if (p) {
    int a = ((XFontStruct*)fl_xfont)->min_char_or_byte2;
    int b = ((XFontStruct*)fl_xfont)->max_char_or_byte2 - a;
    int x = c-a;
    if (x >= 0 && x <= b) return p[x].width;
  }
  return ((XFontStruct*)fl_xfont)->min_bounds.width;
#else
  return XTextWidth((XFontStruct*)fl_xfont, &c, 1);
#endif
}
#endif

int
fl_width(uchar c) { return fl_width((char *)&c, 1); }

static void
x11_font_clip(Region) {} // handled by X clipping region

// return dash number N, or pointer to ending null if none:
static const char *
font_word(const char* p, int n) {
  while (*p) {if (*p=='-') {if (!--n) break;} p++;}
  return p;
}

static void *
x11_font_load(const char *name, const char */*encoding*/, int /*size*/) {
  void* font = XLoadQueryFont(fl_display, name);
  if (!font) {
    Fl::warning("bad font: %s", name);
    font = XLoadQueryFont(fl_display, "fixed"); // if fixed fails we crash
  }
  return font;
}

static void
x11_font_unload(void *font) { XFreeFont(fl_display, (XFontStruct*)font); }

Fl_FontSize::Fl_FontSize(const char* name) {
  font = fl_font_renderer->load(name, fl_encoding_, fl_size_);
  encoding = 0;
#if HAVE_GL
  listbase = 0;
#endif
}

#if 0 // this is never called!

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
// The rest of this file is the enormous amount of crap you have to
// do to get a font & size out of X.  To select a font+size, all
// matchine X fonts are listed with XListFonts, and then the first
// of the following is found and used:
//	pixelsize == size
//	pixelsize == 0 (which indicates a scalable font)
//	the largest pixelsize < size
//	the smallest pixelsize > size
// I have not been able to find any other method than a search
// that will reliably return a bitmap version of the font if one is
// available at the correct size.  This is because X will not return
// it unless you fill in all the garbage fields (swidth, etc)
// correctly.  What a pita!
// Fltk uses pixelsize, not "pointsize".  This is what everybody wants!

// Static variable for the default encoding:
const char *fl_encoding_ = "iso8859-1";

void
fl_font(Fl_Font font, unsigned size) {
  if (font == fl_font_ && size == fl_size_ &&
      (!fl_fontsize->encoding || !strcmp(fl_fontsize->encoding, fl_encoding_)))
    return;
  fl_font_ = font; fl_size_ = size;

  Fl_FontSize* f;
  // search the FontSize we have generated already:
  for (f = font->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size
        && (!f->encoding || !strcmp(f->encoding, fl_encoding_))) {
      set_current_fontsize(f); return;
    }

  // now search the XListFonts results:
  fl_open_display();
  if (!font->xlist) {
    Fl_Font_* t = (Fl_Font_*)font; // cast away const
    t->xlist = XListFonts(fl_display, t->name_, 100, &(t->n));
    if (!t->xlist) {	// use variable if no matching font...
      t->first = f = new Fl_FontSize("variable");
      f->minsize = 0;
      f->maxsize = 32767;
      set_current_fontsize(f); return;
    }
  }

  char* name = font->xlist[0];
  unsigned ptsize = 0;	// best one found so far
  unsigned matchedlength = 32767;
  char namebuffer[1024];	// holds scalable font name
  const char* found_encoding = 0;
  int m = font->n; if (m<0) m = -m;

  for (int n=0; n < m; n++) {

    char* thisname = font->xlist[n];
    // check for matching encoding
    const char* c = font_word(thisname, 13);
    if (*c++ && !strcmp(c, fl_encoding_)) {
      // yes, encoding matches
      if (!found_encoding) ptsize = 0; // force it to choose this
      found_encoding = c;
    } else {
      if (found_encoding) continue;
    }
    c = font_word(thisname,7);
    unsigned thissize = *c ? atoi(++c) : 32767;
    unsigned thislength = strlen(thisname);
    if (thissize == size && thislength < matchedlength) {
      // exact match, use it:
      name = thisname;
      ptsize = size;
      matchedlength = thislength;
    } else if (!thissize && ptsize!=size) {
      // whoa!  A scalable font!  Use unless exact match found:
      int l = c-thisname;
      memcpy(namebuffer,thisname,l);
      // print the pointsize into it:
      if (size>=100) namebuffer[l++] = size/100+'0';
      if (size>=10) namebuffer[l++] = (size/10)%10+'0';
      namebuffer[l++] = (size%10)+'0';
      while (*c == '0') c++;
      strcpy(namebuffer+l,c);
      name = namebuffer;
      ptsize = size;
    } else if (!ptsize ||	// no fonts yet
               thissize < ptsize && ptsize > size || // current font too big
               thissize > ptsize && thissize <= size // current too small
      ) {
      name = thisname; ptsize = thissize;
      matchedlength = thislength;
    }
  }

  if (ptsize != size) { // see if we already found this unscalable font:
    for (f = font->first; f; f = f->next) {
      if (f->minsize <= ptsize && f->maxsize >= ptsize) {
	if (f->minsize > size) f->minsize = size;
	if (f->maxsize < size) f->maxsize = size;
	set_current_fontsize(f); return;
      }
    }
  }

  // okay, we definately have some name, make the font:
  f = new Fl_FontSize(name);
  f->encoding = found_encoding;
  if (ptsize < size) {f->minsize = ptsize; f->maxsize = size;}
  else {f->minsize = size; f->maxsize = ptsize;}
  f->next = font->first;
  ((Fl_Font_*)font)->first = f;
  set_current_fontsize(f);

}

// Change the encoding in use now. This runs the font search again with
// the new encoding.
void fl_encoding(const char* f) {
  if (f != fl_encoding_) {
    fl_encoding_ = f;
    if (fl_font_) fl_font(fl_font_, fl_size_);
  }
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:  bold:       italic:
Fl_Font_
fl_fonts[] = {
{"-*-helvetica-medium-r-normal--*",	fl_fonts+1, fl_fonts+2},
{"-*-helvetica-bold-r-normal--*", 	fl_fonts+1, fl_fonts+3},
{"-*-helvetica-medium-o-normal--*",	fl_fonts+3, fl_fonts+2},
{"-*-helvetica-bold-o-normal--*",	fl_fonts+3, fl_fonts+3},
{"-*-courier-medium-r-normal--*",	fl_fonts+5, fl_fonts+6},
{"-*-courier-bold-r-normal--*",		fl_fonts+5, fl_fonts+7},
{"-*-courier-medium-o-normal--*",	fl_fonts+7, fl_fonts+6},
{"-*-courier-bold-o-normal--*",		fl_fonts+7, fl_fonts+7},
{"-*-times-medium-r-normal--*",		fl_fonts+9, fl_fonts+10},
{"-*-times-bold-r-normal--*",		fl_fonts+9, fl_fonts+11},
{"-*-times-medium-i-normal--*",		fl_fonts+11,fl_fonts+10},
{"-*-times-bold-i-normal--*",		fl_fonts+11,fl_fonts+11},
{"-*-symbol-*",				fl_fonts+12,fl_fonts+12},
{"-*-lucidatypewriter-medium-r-normal-sans-*", fl_fonts+14,fl_fonts+14},
{"-*-lucidatypewriter-bold-r-normal-sans-*", fl_fonts+14,fl_fonts+14},
{"-*-*zapf dingbats-*",			fl_fonts+15,fl_fonts+15},
};

static Fl_Font_Renderer
x11_renderer = {
  x11_font_load, x11_font_unload, x11_font_height,
  x11_font_descent, x11_font_width, x11_font_draw,
  x11_font_clip
};

Fl_Font_Renderer *fl_font_renderer = &x11_renderer;

//
// End of "$Id: fl_font_x.cxx,v 1.2 2001/02/21 06:15:45 clip Exp $"
//
