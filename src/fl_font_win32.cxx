//
// "$Id: fl_font_win32.cxx,v 1.47 2002/12/10 02:01:01 easysw Exp $"
//
// _WIN32 font selection routines for the Fast Light Tool Kit (FLTK).
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
#include <fltk/math.h>

using namespace fltk;

// One of these is made for each combination of size + encoding:
struct FontSize {
  FontSize *next;	// linked list for a single Font
  FontSize *next_all;// linked list so we can destroy em all
  HFONT font;
  int charset;
  unsigned size;
  int width[256];
  TEXTMETRIC metr;
  FontSize(const char* fontname, int attr, int size, int charset);
  ~FontSize();
};

// The xft font implementation adds the xft name and the above list:
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

static FontSize *current;
  
// Change the encoding to use for the next font selection.
void fltk::set_encoding(const char* f) {
  encoding_ = f;
}

static FontSize* all_fonts;

FontSize::FontSize(const char* name, int attr, int size, int charset) {
  current = this;

  int weight = (attr&BOLD) ? FW_BOLD : FW_NORMAL;
  int italic = (attr&ITALIC) ? 1 : 0;

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

  HDC dc = getDC();
  SelectObject(dc, font);
  GetTextMetrics(dc, &current->metr);
  //BOOL ret = GetCharWidthFloat(dc, metr.tmFirstChar, metr.tmLastChar, font->width+metr.tmFirstChar);
  //...would be the right call, but is not implemented into Window95! (WinNT?)
  //GetCharWidth(dc, 0, 255, current->width);

  this->font = font;
  this->size = size;
  this->charset = charset;
  next_all = all_fonts;
  all_fonts = this;
}

FontSize::~FontSize() {
  if (this == current) current = 0;
  DeleteObject(font);
}

// Deallocate Win32 fonts on exit. Warning: it will crash if you try
// to do any fonts after this, because the pointers are not changed!
void fl_font_rid() {
  for (FontSize* fontsize = all_fonts; fontsize;) {
    FontSize* next = fontsize->next;
    delete fontsize;
    fontsize = next;
  }
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
  {{"Terminal",	0},	1,	0},
  {{"Terminal",	1},	1,	0},
  {{"Wingdings",0},	0,	0}
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

HFONT fltk::xfont() {return current->font;}
TEXTMETRIC* fltk::textmetric() {return &(current->metr);}

const char* fltk::Font::current_name() {
  return current_font_->name_;
}

// we need to decode the encoding somehow!
static int charset = DEFAULT_CHARSET;

void fltk::setfont(Font* font, float psize) {

  // only integers supported right now, I think there is a newer
  // interface that takes arbitrary sizes, though...
  psize = int(psize+.5);
  unsigned size = unsigned(psize);

  if (font == current_font_ && psize == current_size_ &&
      current->charset == charset) return;
  current_font_ = font; current_size_ = psize;

  FontSize* f;
  // search the fontsizes we have generated already:
  for (f = ((IFont*)font)->first; f; f = f->next)
    if (f->size == size && f->charset == charset) break;
  if (!f) {
    f = new FontSize(font->name_, font->attributes_, size, charset);
    f->next = ((IFont*)font)->first;
    ((IFont*)font)->first = f;
  }
  current = f;
}

float fltk::getascent()  { return current->metr.tmAscent; }
float fltk::getdescent() { return current->metr.tmDescent; }
  
float fltk::getwidth(const char* c, int n) {
  SIZE size;
  HDC dc = getDC();
  SelectObject(dc, current->font);
  // I think win32 has a fractional version of this:
  GetTextExtentPoint(dc, c, n, &size);
  return size.cx;
}

void fltk::drawtext_transformed(const char *str, int n, float x, float y) {
  SetTextColor(gc, current_xpixel);
  HGDIOBJ oldfont = SelectObject(gc, current->font);
  TextOut(gc, int(floorf(x+.5f)), int(floorf(y+.5f)), str, n);
  SelectObject(gc, oldfont);
}

// Change the encoding to use for the next font selection.
// Encodings is NYI. We need a way to translate the ISO encoding names
// to Win32 encoding enumerations. Ie "iso8859-1" turns into ANSI_CHARSET,
// etc.
void fltk::set_encoding(const char* f) {
  if (f != encoding_) {
    encoding_ = f;
    // charset = decode_the_encoding(f);
  }
}

//
// End of "$Id: fl_font_win32.cxx,v 1.47 2002/12/10 02:01:01 easysw Exp $".
//
