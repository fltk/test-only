//
// "$Id: fl_font.cxx,v 1.16 1999/09/14 17:52:43 carl Exp $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
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

// Select fonts from the fltk font table.

#ifdef WIN32
#include "fl_font_win32.cxx"
#else

#include <config.h>
#include <FL/Fl_Font.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include "Fl_FontSize.H"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Fl_FontSize* fl_font_;
XFontStruct* fl_xfont;
static GC font_gc;

void fl_font(Fl_FontSize* s) {
  if (s != fl_font_) {
    fl_font_ = s;
    fl_xfont = s->font;
    font_gc = 0;
  }
}

// return dash number N, or pointer to ending null if none:
const char* fl_font_word(const char* p, int n) {
  while (*p) {if (*p=='-') {if (!--n) break;} p++;}
  return p;
}

Fl_FontSize::Fl_FontSize(const char* name) {
  font = XLoadQueryFont(fl_display, name);
  if (!font) {
//    Fl::warning("bad font: %s", name);
    font = XLoadQueryFont(fl_display, "fixed"); // if fixed fails we crash
  }
  encoding = 0;
  if (*name == '-') {
    const char* c = fl_font_word(name, 13);
    if (*c++) encoding = c;
  }
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
  if (this == fl_font_) fl_font_ = 0;
  XFreeFont(fl_display, font);
}
#endif

////////////////////////////////////////////////////////////////
// fl_font(f,size,encoding):
// This works by searching all the XListFonts that matched the wildcard
// name for the best matching size and encoding.  This is a quite lengthy
// process!

// return a pointer to a number we think is "point size":
char* fl_find_fontsize(char* name) {
  char* c = name;
  // for standard x font names, try after 7th dash:
  if (*c == '-') {
    c = (char*)fl_font_word(c,7);
    if (*c++ && isdigit(*c)) return c;
  }
  char* r = 0;
  // find last set of digits:
  for (c++;* c; c++) if (isdigit(*c) && !isdigit(*(c-1))) r = c;
  return r;
}

void fl_font(const Fl_Font s, unsigned size, const char* encoding) {
  static Fl_Font curfont;
  if (s == curfont) { // see if it is the current font...
      Fl_FontSize* f = fl_font_;
      if (f->minsize <= size && f->maxsize >= size &&
	  (!f->encoding || !strcmp(f->encoding, encoding))) return;
  }
  curfont = s;

  Fl_FontSize* f;
  // search the fonts we have generated already:
  for (f = s->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size
	&& (!f->encoding || !strcmp(f->encoding, encoding))) {
      fl_font(f); return;
    }

  // now search the XListFonts results:
  fl_open_display();
  if (!s->xlist) {
    Fl_Font_* t = (Fl_Font_*)s; // cast away const
    t->xlist = XListFonts(fl_display, t->name_, 100, &(t->n));
    if (!t->xlist) {	// use fixed if no matching font...
      t->first = f = new Fl_FontSize("fixed");
      f->minsize = 0;
      f->maxsize = 32767;
      fl_font(f); return;
    }
  }
  // search for largest <= font size:
  char* name = s->xlist[0];
  unsigned ptsize = 0;	// best one found so far
  unsigned matchedlength = 32767;
  char namebuffer[1024];	// holds scalable font name
  int found_encoding = 0;
  int m = s->n; if (m<0) m = -m;
  for (int n=0; n < m; n++) {

    char* thisname = s->xlist[n];
    if (*thisname == '-') {
      // check for matching encoding
      const char* c = fl_font_word(thisname, 13);
      if (*c++ && !strcmp(c, encoding)) {
	// yes, encoding matches
	if (!found_encoding) ptsize = 0; // force it to choose this
	found_encoding = 1;
      } else {
	if (found_encoding) continue;
      }
    }
    char* c = fl_find_fontsize(thisname);
    unsigned thissize = c ? atoi(c) : 32767;
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
#if 1 // this works if you don't want stdio
      if (size>=100) namebuffer[l++] = size/100+'0';
      if (size>=10) namebuffer[l++] = (size/10)%10+'0';
      namebuffer[l++] = (size%10)+'0';
#else
      //for some reason, sprintf fails to return the right value under Solaris.
      l += sprintf(namebuffer+l,"%d",size);
#endif
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
    for (f = s->first; f; f = f->next) {
      if (f->minsize <= ptsize && f->maxsize >= ptsize) {
	if (f->minsize > size) f->minsize = size;
	if (f->maxsize < size) f->maxsize = size;
	fl_font(f); return;
      }
    }
  }

  // okay, we definately have some name, make the font:
  f = new Fl_FontSize(name);
  if (ptsize < size) {f->minsize = ptsize; f->maxsize = size;}
  else {f->minsize = size; f->maxsize = ptsize;}
  f->next = s->first;
  ((Fl_Font_*)s)->first = f;
  fl_font(f);

}

// back-compatable selector that uses default encoding:

const char *fl_encoding = "iso8859-1";

void fl_font(const Fl_Font f, unsigned size) {
  fl_font(f ? f : FL_HELVETICA, size, fl_encoding);
}

////////////////////////////////////////////////////////////////
// Things you can do once the font+size has been selected:

int fl_height() {
  return (fl_xfont->ascent + fl_xfont->descent);
}

int fl_descent() {
  return fl_xfont->descent;
}

double fl_width(const char* c) {
  XCharStruct* p = fl_xfont->per_char;
  if (!p) return strlen(c)*fl_xfont->min_bounds.width;
  int a = fl_xfont->min_char_or_byte2;
  int b = fl_xfont->max_char_or_byte2 - a;
  int w = 0;
  while (*c) {
    int x = *(uchar*)c++ - a;
    if (x >= 0 && x <= b) w += p[x].width;
    else w += fl_xfont->min_bounds.width;
  }
  return w;
}

double fl_width(const char* c, int n) {
  XCharStruct* p = fl_xfont->per_char;
  if (!p) return n*fl_xfont->min_bounds.width;
  int a = fl_xfont->min_char_or_byte2;
  int b = fl_xfont->max_char_or_byte2 - a;
  int w = 0;
  while (n--) {
    int x = *(uchar*)c++ - a;
    if (x >= 0 && x <= b) w += p[x].width;
    else w += fl_xfont->min_bounds.width;
  }
  return w;
}

double fl_width(uchar c) {
  XCharStruct* p = fl_xfont->per_char;
  if (p) {
    int a = fl_xfont->min_char_or_byte2;
    int b = fl_xfont->max_char_or_byte2 - a;
    int x = c-a;
    if (x >= 0 && x <= b) return p[x].width;
  }
  return fl_xfont->min_bounds.width;
}

void fl_draw(const char* str, int n, int x, int y) {
  if (!fl_xfont) fl_font(0, 14); // Perhaps this should be configured somewhere?
  if (font_gc != fl_gc) {
    font_gc = fl_gc;
    XSetFont(fl_display, fl_gc, fl_xfont->fid);
  }
  XDrawString(fl_display, fl_window, fl_gc, x, y, str, n);
}

void fl_draw(const char* str, int x, int y) {
  fl_draw(str, strlen(str), x, y);
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:  bold:       italic:
Fl_Font_ fl_fonts[] = {
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

#endif

//
// End of "$Id: fl_font.cxx,v 1.16 1999/09/14 17:52:43 carl Exp $".
//
