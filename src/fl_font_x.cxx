//
// "$Id: fl_font_x.cxx,v 1.16 2002/12/10 02:01:01 easysw Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/draw.h>
#include <fltk/error.h>
#include <fltk/x.h>
#include <fltk/math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "IFont.h"
using namespace fltk;

// One of these is created for each font+size+encoding combination we use:
struct FontSize {
  FontSize* next;	// linked list for a single Font
  XFontStruct* font;
  const char* encoding;
  FontSize(const char* xfontname, const char*);
  const char* name;
  unsigned minsize;	// smallest point size that should use this
  unsigned maxsize;	// largest point size that should use this
  //  ~FontSize();
};

// We store the attributes in neat blocks of 2^n:
fltk::Font* fltk::Font::plus(int x) {
  IFont* font = (IFont*)this;
  if (x & BOLD) font = font->bold;
  if (x & ITALIC) font = font->italic;
  return &(font->f);
}

const char* fltk::Font::system_name() {
  return ((IFont*)this)->system_name;
}

static FontSize *current;
static GC font_gc; // which gc the font was set in last time

XFontStruct* fltk::xfont() {return current->font;}

const char* fltk::Font::current_name() {return current->name;}

FontSize::FontSize(const char* name, const char* nname) {
  this->name = nname ? nname : name;
  font = XLoadQueryFont(xdisplay, name);
  if (!font) {
    warning("bad font: %s", name);
    font = XLoadQueryFont(xdisplay, "fixed"); // if fixed fails we crash
  }
  encoding = 0;
}

#if 0 // this is never called!
FontSize::~FontSize() {
  if (this == current) current = 0;
  XFreeFont(xdisplay, font);
}
#endif

////////////////////////////////////////////////////////////////
// Things you can do once the font+size has been selected:

// Placeholder function, Xft version needs to free something:
void fltk::Drawable::free_gc() {draw = 0;}

void fltk::drawtext_transformed(const char *str, int n, float x, float y) {
  if (font_gc != gc) {
    // I removed this, the user MUST set the font before drawing: (was)
    // if (!current) setfont(HELVETICA, NORMAL_SIZE);
    font_gc = gc;
    XSetFont(xdisplay, gc, current->font->fid);
  }
  XDrawString(xdisplay, xwindow, gc,
	      int(floorf(x+.5f)),
	      int(floorf(y+.5f)), str, n);
}

float fltk::getascent()  { return current->font->ascent; }
float fltk::getdescent() { return current->font->descent; }

float fltk::getwidth(const char *c, int n) {
  return XTextWidth(current->font, c, n);
}

////////////////////////////////////////////////////////////////
// The rest of this file is the enormous amount of crap you have to
// do to get a font & size out of X.  To select a font+size, all
// matchine X fonts are listed with XListFonts, and then the first
// of the following is found and used:
//
//	pixelsize == size
//	pixelsize == 0 (which indicates a scalable font)
//	the largest pixelsize < size
//	the smallest pixelsize > size
//
// If any fonts match the encoding() then the search is limited
// to those matching fonts. Otherwise all fonts are searched and thus
// a random encoding is chosen.
//
// I have not been able to find any other method than a search
// that will reliably return a bitmap version of the font if one is
// available at the correct size.  This is because X will not use a
// bitmap font unless all the extra fields are filled in correctly.
//
// Fltk uses pixelsize, not "pointsize".  This is what everybody wants!

// return dash number N, or pointer to ending null if none:
static const char *
font_word(const char* p, int n) {
  while (*p) {if (*p=='-') {if (!--n) break;} p++;}
  return p;
}

void fltk::setfont(Font* font, float psize) {
  FontSize* f = current;
  IFont* t = (IFont*)font;

  // only integers supported right now (this can be improved):
  psize = int(psize+.5);
  unsigned size = unsigned(psize);

  // See if the current font is correct:
  if (font == current_font_ && psize == current_size_ &&
      (f->encoding==encoding_ || !strcmp(f->encoding, encoding_)))
    return;
  current_font_ = font; current_size_ = psize;

  // search the FontSize we have generated already:
  for (f = t->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size
        && (f->encoding==encoding_ ||
	    !f->encoding || !strcmp(f->encoding, encoding_))) {
      goto DONE;
    }

  // run XListFonts if it has not been done yet:
  if (!t->xlist) {
    open_display();
    t->xlist = XListFonts(xdisplay, t->system_name, 100, &(t->n));
    if (!t->xlist || t->n<=0) {	// use variable if no matching font...
      t->first = f = new FontSize("variable",0);
      f->minsize = 0;
      f->maxsize = 32767;
      goto DONE;
    }
  }

  // now search the XListFonts results:
  {char* name = t->xlist[0];
  unsigned ptsize = 0;	// best one found so far
  char namebuffer[1024];	// holds scalable font name
  bool found_encoding = false;
  int m = t->n; if (m<0) m = -m;
  char* nname = 0;

  for (int n=0; n < m; n++) {

    char* thisname = t->xlist[n];
    // get the encoding field:
    const char* this_encoding = font_word(thisname, 13);
    if (*this_encoding++ && !strcmp(this_encoding, encoding_)) {
      // forget any wrong encodings when we find a correct one:
      if (!found_encoding) ptsize = 0;
      found_encoding = true;
    } else {
      // ignore later wrong encodings after we find a correct one:
      if (found_encoding) continue;
    }
    // get the pixelsize field:
    const char* c = font_word(thisname,7);
    unsigned thissize = *c ? atoi(++c) : 32767;
    if (thissize == size) {
      // Use an exact match:
      name = nname = thisname;
      ptsize = size;
      if (found_encoding) break;
    } else if (!thissize && ptsize!=size) {
      // Use a scalable font if no exact match found:
      int l = c-thisname;
      memcpy(namebuffer,thisname,l);
      // print the pointsize into it:
      if (size>=100) namebuffer[l++] = size/100+'0';
      if (size>=10) namebuffer[l++] = (size/10)%10+'0';
      namebuffer[l++] = (size%10)+'0';
      while (*c == '0') c++;
      strcpy(namebuffer+l,c);
      name = namebuffer;
      nname = thisname;
      ptsize = size;
    } else if (!ptsize ||	// no fonts yet
               thissize < ptsize && ptsize > size || // current font too big
               thissize > ptsize && thissize <= size // current too small
      ) {
      // Use the nearest fixed size font:
      name = nname = thisname;
      ptsize = thissize;
    }
  }

  // If we didn't find an exact match, search the list to see if we already
  // found this font:
  if (ptsize != size || !found_encoding) {
    for (f = t->first; f; f = f->next) {
      if (f->minsize <= ptsize && f->maxsize >= ptsize &&
	  (!found_encoding || !strcmp(f->encoding, encoding_))) {
	if (f->minsize > size) f->minsize = size;
	if (f->maxsize < size) f->maxsize = size;
	goto DONE;
      }
    }
  }

  // okay, we definately have some name, make the font:
  f = new FontSize(name,nname);
  // we pretend it has the current encoding even if it does not, so that
  // it is quickly matched when searching for it again with the same
  // encoding:
  f->encoding = encoding_;
  if (ptsize < size) {f->minsize = ptsize; f->maxsize = size;}
  else {f->minsize = size; f->maxsize = ptsize;}
  f->next = t->first;
  t->first = f;}
 DONE:
  if (f != current) {
    current = f;
    font_gc = 0;
  }
}

// Change the encoding to use for the next font selection.
void fltk::set_encoding(const char* f) {
  encoding_ = f;
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:
static IFont fonts [] = {
  {{"helvetica",0}, "-*-helvetica-medium-r-normal--*",	fonts+1, fonts+2},
  {{"helvetica",1}, "-*-helvetica-bold-r-normal--*", 	fonts+1, fonts+3},
  {{"helvetica",2}, "-*-helvetica-medium-o-normal--*",	fonts+3, fonts+2},
  {{"helvetica",3}, "-*-helvetica-bold-o-normal--*",	fonts+3, fonts+3},
  {{"courier", 0},  "-*-courier-medium-r-normal--*",	fonts+5, fonts+6},
  {{"courier", 1},  "-*-courier-bold-r-normal--*",	fonts+5, fonts+7},
  {{"courier", 2},  "-*-courier-medium-o-normal--*",	fonts+7, fonts+6},
  {{"courier", 3},  "-*-courier-bold-o-normal--*",	fonts+7, fonts+7},
  {{"times", 0},    "-*-times-medium-r-normal--*",	fonts+9, fonts+10},
  {{"times", 1},    "-*-times-bold-r-normal--*",	fonts+9, fonts+11},
  {{"times", 2},    "-*-times-medium-i-normal--*",	fonts+11,fonts+10},
  {{"times", 3},    "-*-times-bold-i-normal--*",	fonts+11,fonts+11},
  {{"symbol", 0},   "-*-symbol-*",			fonts+12,fonts+12},
  {{"lucidatypewriter", 0}, "-*-lucidatypewriter-medium-r-normal-sans-*", fonts+14,fonts+14},
  {{"lucidatypewriter", 1}, "-*-lucidatypewriter-bold-r-normal-sans-*", fonts+14,fonts+14},
  {{"zapf dingbats", 0}, "-*-*zapf dingbats-*",		fonts+15,fonts+15},
};

fltk::Font* const fltk::HELVETICA 		= &(fonts[0].f);
fltk::Font* const fltk::HELVETICA_BOLD		= &(fonts[1].f);
fltk::Font* const fltk::HELVETICA_ITALIC	= &(fonts[2].f);
fltk::Font* const fltk::HELVETICA_BOLD_ITALIC	= &(fonts[3].f);
fltk::Font* const fltk::COURIER 		= &(fonts[4].f);
fltk::Font* const fltk::COURIER_BOLD		= &(fonts[5].f);
fltk::Font* const fltk::COURIER_ITALIC		= &(fonts[6].f);
fltk::Font* const fltk::COURIER_BOLD_ITALIC	= &(fonts[7].f);
fltk::Font* const fltk::TIMES 			= &(fonts[8].f);
fltk::Font* const fltk::TIMES_BOLD		= &(fonts[9].f);
fltk::Font* const fltk::TIMES_ITALIC		= &(fonts[10].f);
fltk::Font* const fltk::TIMES_BOLD_ITALIC	= &(fonts[11].f);
fltk::Font* const fltk::SYMBOL_FONT		= &(fonts[12].f);
fltk::Font* const fltk::SCREEN_FONT		= &(fonts[13].f);
fltk::Font* const fltk::SCREEN_BOLD_FONT	= &(fonts[14].f);
fltk::Font* const fltk::ZAPF_DINGBATS		= &(fonts[15].f);

// Turn an old integer into a predefined font:
fltk::Font* fltk::font(int i) {return &(fonts[i%16].f);}

//
// End of "$Id: fl_font_x.cxx,v 1.16 2002/12/10 02:01:01 easysw Exp $"
//
