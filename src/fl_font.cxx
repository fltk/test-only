//
// "$Id: fl_font.cxx,v 1.14 1999/08/18 08:02:19 bill Exp $"
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

Fl_FontSize::Fl_FontSize(const char* name) {
  font = XLoadQueryFont(fl_display, name);
  if (!font) {
//    Fl::warning("bad font: %s", name);
    font = XLoadQueryFont(fl_display, "fixed"); // if fixed fails we crash
  }
#if HAVE_GL
  listbase = 0;
#endif
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
  XFreeFont(fl_display, font);
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

#define MAXSIZE 32767

// return dash number N, or pointer to ending null if none:
const char* fl_font_word(const char* p, int n) {
  while (*p) {if (*p=='-') {if (!--n) break;} p++;}
  return p;
}

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
  for (c++;* c; c++)
    if (isdigit(*c) && !isdigit(*(c-1))) r = c;
  return r;
}

const char *fl_encoding = "iso8859-1";

// return true if this matches Fl::encoding:
int fl_correct_encoding(const char* name) {
  if (*name != '-') return 0;
  const char* c = fl_font_word(name,13);
  return (*c++ && !strcmp(c,fl_encoding));
}

// locate or create an Fl_FontSize for a given Fl_Font_ and size:
static Fl_FontSize* find(Fl_Font s, unsigned size) {
  if (!s || !s->name_) s = fl_fonts; // use font 0 if still undefined
  Fl_FontSize* f;
  for (f = s->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size) return f;
  fl_open_display();
  if (!s->xlist) {
    Fl_Font_* t = (Fl_Font_*)s; // cast away const
    t->xlist = XListFonts(fl_display, t->name_, 100, &(t->n));
    if (!t->xlist) {	// use fixed if no matching font...
      t->first = new Fl_FontSize("fixed");
      t->first->minsize = 0;
      t->first->maxsize = 32767;
      return t->first;
    }
  }
  // search for largest <= font size:
  char* name = s->xlist[0];
  unsigned ptsize = 0;	// best one found so far
  unsigned matchedlength = 32767;
  char namebuffer[1024];	// holds scalable font name
  bool found_encoding = false;
  int m = s->n; if (m<0) m = -m;
  for (int n=0; n < m; n++) {

    char* thisname = s->xlist[n];
    if (fl_correct_encoding(thisname)) {
      if (!found_encoding) ptsize = 0; // force it to choose this
      found_encoding = true;
    } else {
      if (found_encoding) continue;
    }
    char* c = fl_find_fontsize(thisname);
    unsigned thissize = c ? atoi(c) : MAXSIZE;
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
	return f;
      }
    }
  }

  // okay, we definately have some name, make the font:
  f = new Fl_FontSize(name);
  if (ptsize < size) {f->minsize = ptsize; f->maxsize = size;}
  else {f->minsize = size; f->maxsize = ptsize;}
  f->next = s->first;
  ((Fl_Font_*)s)->first = f;
  return f;

}

////////////////////////////////////////////////////////////////
// Public interface:

Fl_Font fl_font_;
unsigned fl_size_;
XFontStruct* fl_xfont;
static GC font_gc;

void fl_font(Fl_Font f, unsigned size) {
  if (f == fl_font_ && size == fl_size_) return;
  fl_font_ = f; fl_size_ = size;
  Fl_FontSize* s = find(f, size);
  if (s != fl_fontsize) {
    fl_fontsize = s;
    fl_xfont = s->font;
    font_gc = 0;
  }
}

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

#endif

//
// End of "$Id: fl_font.cxx,v 1.14 1999/08/18 08:02:19 bill Exp $".
//
