//
// "$Id$"
//
// MacOS font selection routines for the Fast Light Tool Kit (FLTK).
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

#include <fltk/x.h>
#include <fltk/Font.h>
#include <ctype.h>
#include <stdlib.h>
#include <fltk/utf.h>
#include <fltk/math.h>
#include <fltk/string.h>
#include <fltk/Window.h>

#include <Carbon/Carbon.h>

using namespace fltk;

// One of these is made for each combination of size + encoding:
struct FontSize {
  FontSize *next;	// linked list for a single Font
  char *q_name;
  int size, minsize, maxsize;
  short ascent, descent, q_width;
  ATSFontRef font;
  FontSize(const char* xfontname, int size);
  ~FontSize();
};

// The public-visible fltk::Font structures are actually imbedded in
// this larger structure which points at the the above list
struct IFont {
  fltk::Font f;
  int attribute_mask; // all attributes that can be turned on
  FontSize* first;
};

// We store the attributes in neat blocks of 2^n:
fltk::Font* fltk::Font::plus(int x) {
  IFont* font = (IFont*)this;
  x &= font->attribute_mask & ~attributes_;
  return &((font+x)->f);
}

const char* fltk::Font::system_name() {
  return name_;
}

static FontSize* current;

ATSFontRef fltk::xfont() { return current->font; }

// face = italic, bold, 0, italic|bold

FontSize::FontSize(const char* name, int Size) {
  current = this;
  next = 0;
  q_name = newstring(name);
  size = Size;
  ascent = Size*3/4;
  descent = Size-ascent;
  q_width = Size*2/3;
  minsize = maxsize = Size;
  // Using ATS to get the genral Glyph size information
  CFStringRef cfname = CFStringCreateWithCString(0L, q_name, kCFStringEncodingASCII);
  font = ATSFontFindFromName(cfname, kATSOptionFlagsDefault);
  if (font) {
    ATSFontMetrics m = { 0 };
    ATSFontGetHorizontalMetrics(font, kATSOptionFlagsDefault, &m);
    if (m.avgAdvanceWidth) q_width = int(m.avgAdvanceWidth*size);
    // playing with the offsets a little to make standard sizes fit
    if (m.ascent) ascent  = int(m.ascent*size-0.5f);
    if (m.descent) descent = -int(m.descent*size-1.5f);
  }
  CFRelease(cfname);
}

FontSize::~FontSize() {
  if (current == this) current = 0;
  delete[] q_name;
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:
static IFont fonts [] = {
  {{"Arial",	0},	3,	0},
  {{"Arial Bold",	1},	3,	0},
  {{"Arial Italic",	2},	3,	0},
  {{"Arial Bold Italic",	3},	3,	0},
  {{"Courier New",0},	3,	0},
  {{"Courier New Bold",1},	3,	0},
  {{"Courier New Italic",2},	3,	0},
  {{"Courier New Bold Italic",3},	3,	0},
  {{"Times New Roman",	0},	3,	0},
  {{"Times New Roman Bold",	1},	3,	0},
  {{"Times New Roman Italic",	2},	3,	0},
  {{"Times New Roman Bold Italic",	3},	3,	0},
  {{"Symbol",	0},	0,	0},
  {{"Monaco",	0},	1,	0},
  {{"Andale Mono",	1},	1,	0},
  {{"Webdings",	0},	0,	0}
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

////////////////////////////////////////////////////////////////

// For fltk::list_fonts(), make a new font, and optionally the bold and
// italic subfonts:
Font* fl_make_font(const char* name, int attrib) {
  // see if it is one of our built-in fonts and return it:
  int j; for (j = 0; j < 16; j++) {
    if (fonts[j].f.attributes_ == attrib &&
	!strcasecmp(fonts[j].f.name_, name)) return &(fonts[j].f);
  }
  // no, lets create some fonts:
  IFont* newfont = new IFont[4];
  newfont[0].f.name_ = newstring(name);
  for (j = 0; j < 4; j++) {
    newfont[j].f.name_ = newfont[0].f.name_;
    newfont[j].f.attributes_ = attrib|j;
    newfont[j].attribute_mask = 3;
    newfont[j].first = 0;
  }
  return &(newfont[0].f);
}

////////////////////////////////////////////////////////////////
// Public interface:

// int fltk::xfont() {return current->font;}

const char* fltk::Font::current_name() {
  return current_font_->name_;
}

// we need to decode the encoding somehow!
//static int charset = DEFAULT_CHARSET;

void fltk::setfont(Font* font, float psize) {

  // only integers supported right now, perphaps Quartz does any size:
  psize = int(psize+.5);
  unsigned size = unsigned(psize);

  FontSize* f;
  bool getmetrics = false;
  if (font == current_font_ && psize == current_size_
      /* && current->charset == charset */) f = current;
  else {
    current_font_ = font; current_size_ = psize;
    // search the fontsizes we have generated already:
    for (f = ((IFont*)font)->first; f; f = f->next)
      if (f->size == (int)size /*&& f->charset == charset*/) break;
    if (!f) {
      f = new FontSize(font->name_, size);
      f->next = ((IFont*)font)->first;
      ((IFont*)font)->first = f;
      getmetrics = true;
    }
    current = f;
  }
  /* //+++
  TextFont(f->font);	//: select font into current QuickDraw GC
  TextFace(f->face);
  TextSize(f->size);
  if (getmetrics) {
    //: get the true metrics for the currnet GC. This fails on multiple
    // monitors with different dpi's!, but actually it should work because
    // we should be selecting font sizes by pixels, not points!
    GetFontInfo(&(f->fi));
    //FontMetrics(&(f->mr));
  } */
  if (f) {
    CGContextSelectFont(quartz_gc, f->q_name, (float)f->size, 
                        kCGEncodingMacRoman);
  }
}

float fltk::getascent()  { return current->ascent; }
float fltk::getdescent() { return current->descent; }

#define WCBUFLEN 256

float fltk::getwidth(const char* text, int n) {
  if (!quartz_gc) {
    Window *w = Window::first();
    if (w) w->make_current();
    if (!quartz_gc) return -1;
  }
  CGContextSetTextDrawingMode(quartz_gc, kCGTextInvisible);
  CGContextShowTextAtPoint(quartz_gc, 0, 0, text, n);
  CGContextSetTextDrawingMode(quartz_gc, kCGTextFill);
  CGPoint p = CGContextGetTextPosition(quartz_gc);
  return p.x;
}

void fltk::drawtext_transformed(const char *text, int n, float x, float y) {
  //+++ convert font encoding into Mac text
  CGContextShowTextAtPoint(quartz_gc, x, y, text, n);
}

//
// End of "$Id$".
//
