//
// "$Id: fl_font_mac.cxx,v 1.7 2004/12/16 18:40:42 spitzak Exp $"
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

using namespace fltk;

//: MeasureText, FontMetrics, WidthTabHandle, GetSysFont, SysFontSize
//: TextSize, TextFont
//: GetFNum (theName: Str255; VAR familyID: Integer);
//: FUNCTION FMSwapFont (inRec: FMInput): FMOutPtr;

// One of these is made for each combination of size + encoding:
struct FontSize {
  FontSize *next;	// linked list for a single Font
  short font, face, size;
  FontInfo fi;
  //FMetricRec mr;
  short width[256];
  FontSize(const char* fontname, int attr, int size);
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

// face = italic, bold, 0, italic|bold

FontSize::FontSize(const char* name, int attr, int size) {
  current = this;

  int n = strlen(name); if (n > 79) n = 79;
  unsigned char fn[80]; fn[0] = n; memcpy(fn+1, name, n);
  GetFNum(fn, &font);

  this->size = size;

  face = 0;
  if (attr&BOLD) face = bold;
  if (attr&ITALIC) face |= italic;

//   FMInput fIn = { font, size, face, 0, 0, { 1, 1}, { 1, 1} };
//   FMOutput *fOut = FMSwapFont(&fIn);
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:
static IFont fonts [] = {
  {{"Arial",	0},	3,	0},
  {{"Arial",	1},	3,	0},
  {{"Arial",	2},	3,	0},
  {{"Arial",	3},	3,	0},
  {{"Courier New",0},	3,	0},
  {{"Courier New",1},	3,	0},
  {{"Courier New",2},	3,	0},
  {{"Courier New",3},	3,	0},
  {{"Times New Roman",	0},	3,	0},
  {{"Times New Roman",	1},	3,	0},
  {{"Times New Roman",	2},	3,	0},
  {{"Times New Roman",	3},	3,	0},
  {{"Symbol",	0},	0,	0},
  {{"Chicago",	0},	1,	0},
  {{"Chicago",	1},	1,	0},
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
  newfont[0].f.name_ = strdup(name);
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
      f = new FontSize(font->name_, font->attributes_, size);
      f->next = ((IFont*)font)->first;
      ((IFont*)font)->first = f;
      getmetrics = true;
    }
    current = f;
  }
  TextFont(f->font);	//: select font into current QuickDraw GC
  TextFace(f->face);
  TextSize(f->size);
  if (getmetrics) {
    //: get the true metrics for the currnet GC. This fails on multiple
    // monitors with different dpi's!, but actually it should work because
    // we should be selecting font sizes by pixels, not points!
    GetFontInfo(&(f->fi));
    //FontMetrics(&(f->mr));
  }
}

float fltk::getascent()  { return current->fi.ascent; }
float fltk::getdescent() { return current->fi.descent; }

#define WCBUFLEN 256

float fltk::getwidth(const char* text, int n) {
  char localbuffer[WCBUFLEN];
  char* buffer = localbuffer;
  char* mallocbuffer = 0;
  int count = utf8toa(text, n, buffer, WCBUFLEN);
  if (count >= WCBUFLEN) {
    buffer = mallocbuffer = new char[count+1];
    count = utf8toa(text, n, buffer, count+1);
  }
  float r = float(TextWidth( buffer, 0, count ));
  delete[] mallocbuffer;
  return r;
}

void fltk::drawtext_transformed(const char *text, int n, float x, float y) {
  char localbuffer[WCBUFLEN];
  char* buffer = localbuffer;
  char* mallocbuffer = 0;
  int count = utf8toa(text, n, buffer, WCBUFLEN);
  if (count >= WCBUFLEN) {
    buffer = mallocbuffer = new char[count+1];
    count = utf8toa(text, n, buffer, count+1);
  }
  MoveTo(int(floorf(x+.5f)), int(floorf(y+.5f)));
  DrawText(buffer, 0, count);
  delete[] mallocbuffer;
}

//
// End of "$Id: fl_font_mac.cxx,v 1.7 2004/12/16 18:40:42 spitzak Exp $".
//
