//
// "$Id: fl_font_xft.cxx,v 1.13 2002/12/10 09:43:26 spitzak Exp $"
//
// Copyright 2001 Bill Spitzak and others.
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

// Draw fonts using Keith Packard's Xft and Xrender extension. Yow!
// Many thanks to Carl for making the original version of this.
//
// This code is included in fltk if it is compiled with -DUSE_XFT=1
// It should also be possible to compile this file as a seperate
// shared library, and by using LD_PRELOAD you can insert it between
// any fltk program and the fltk shared library.
//
// This plugin only requires libXft to work. Contrary to popular
// belief there is no need to have freetype, or the Xrender extension
// available to use this code. You will just get normal Xlib fonts
// (Xft calls them "core" fonts) The Xft algorithims for choosing
// these is about as good as the fltk ones (I hope to fix it so it is
// exactly as good...), plus it can cache it's results and share them
// between programs, so using this should be a win in all cases. Also
// it should be obvious by comparing this file and font_x.cxx that
// it is a lot easier to program to Xft than to Xlib.
//
// To actually get antialiasing you need the following:
//
// 1. You have XFree86 4
// 2. You have the XRender extension
// 3. Your X device driver supports the render extension
// 4. You have libXft
// 5. Your libXft has freetype2 support compiled in
// 6. You have the freetype2 library
//
// Distributions that have XFree86 4.0.3 or later should have all of this...
//
// Unlike some other Xft packages, I tried to keep this simple and not
// to work around the current problems in Xft by making the "patterns"
// complicated. I belive doing this defeats our ability to improve Xft
// itself. You should edit the ~/.xftconfig file to "fix" things, there
// are several web pages of information on how to do this.

#include <fltk/events.h>
#include <fltk/Font.h>
#include <fltk/draw.h>
#include <fltk/math.h>

#include <fltk/x.h>
#define Window XWindow
#include <X11/Xft/Xft.h>
#undef Window

// define some symbols missing from some Xft header files:
#ifndef XFT_MINSPACE
#define XFT_MINSPACE        "minspace"  /* Bool use minimum line spacing */
#endif
#ifndef XFT_MATRIX
#define XFT_MATRIX          "matrix"    /* XftMatrix */
typedef struct _XftMatrix {
    double xx, xy, yx, yy;
} XftMatrix;
#endif                                                        
#include <string.h>
#include <stdlib.h>

using namespace fltk;

// One of these is made for each combination of size + encoding:
struct FontSize {
  FontSize *next;	// linked list for this Fontdesc
  XftFont* font;
  const char* encoding;
  float size;
  //~FontSize();
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

static XftFont* fontopen(const char* name, int attributes, bool core) {
  int weight = XFT_WEIGHT_MEDIUM;
  if (attributes&BOLD) weight = XFT_WEIGHT_BOLD;
  int slant = XFT_SLANT_ROMAN;
  if (attributes&ITALIC) slant = XFT_SLANT_ITALIC;
  // this call is extremely slow...
  return XftFontOpen(xdisplay, xscreen,
		     XFT_FAMILY, XftTypeString, name,
		     XFT_WEIGHT, XftTypeInteger, weight,
		     XFT_SLANT, XftTypeInteger, slant,
		     XFT_ENCODING, XftTypeString, encoding_,
		     XFT_PIXEL_SIZE, XftTypeDouble, (double)current_size_,
		     core ? XFT_CORE : 0, XftTypeBool, true,
		     XFT_RENDER, XftTypeBool, false,
		     0);
}

void fltk::setfont(fltk::Font* font, float size) {
  // Reduce the number of sizes by rounding to various multiples:
  size = rint(10*size)/10;
  if (font == current_font_ && size == current_size_ &&
      !strcasecmp(current->encoding, encoding_))
    return;
  current_font_ = font; current_size_ = size;
  FontSize* f;
  // search the fontsizes we have generated already
  for (f = ((IFont*)font)->first; f; f = f->next) {
    if (f->size == size && !strcasecmp(f->encoding, encoding_))
      break;
  }
  if (!f) {
    f = new FontSize();
    f->encoding = encoding_;
    f->size = current_size_;
    f->font = fontopen(font->name_, font->attributes_, false);
    f->next = ((IFont*)font)->first;
    ((IFont*)font)->first = f;
  }
  current = f;
}

#if 0 // this is never called!
FontSize::~FontSize() {
  if (this == current) current = 0;
  XftFontClose(xdisplay, font);
}
#endif

// This call is used by gl_setfont to get a bitmapped font. Xft actually does
// a pretty good job of selecting X fonts...
XFontStruct* fltk::xfont() {
  if (current->font->core) return current->font->u.core.font;
  static XftFont* xftfont;
  if (xftfont) XftFontClose (xdisplay, xftfont);
  xftfont = fontopen(current_font_->name_, current_font_->attributes_, true);
  return xftfont->u.core.font;
}

const char* fltk::Font::current_name() {
  return current_font_->name_;
}

float fltk::getascent()  { return current->font->ascent; }
float fltk::getdescent() { return current->font->descent; }

float fltk::getwidth(const char *str, int n) {
  XGlyphInfo i;
  XftTextExtents8(xdisplay, current->font, (XftChar8 *)str, n, &i);
  return i.xOff;
}

////////////////////////////////////////////////////////////////

#if USE_OVERLAY
// Currently Xft does not work with colormapped visuals, so this probably
// does not work unless you have a true-color overlay.
extern bool fl_overlay;
extern Colormap fl_overlay_colormap;
extern XVisualInfo* fl_overlay_visual;
#endif

extern int fl_clip_state_number;
static int clip_state_number = 0; // which clip we did last
static XftDraw* clipped_draw = 0;  // the XftDraw we did it to

void fltk::Drawable::free_gc() {
  if (draw) {
    XftDrawDestroy(draw);
    draw = 0;
    clipped_draw = 0;
  }
};

void fltk::drawtext_transformed(const char *str, int n, float x, float y) {
  XftDraw* draw = drawable->draw;

  if (!draw) {
#if USE_OVERLAY
    if (overlay)
      draw =
	XftDrawCreate(xdisplay, drawable->xid,
		      overlay_visual->visual, overlay_colormap);
    else
#endif
    draw =
      XftDrawCreate(xdisplay, drawable->xid,
		    xvisual->visual, xcolormap);
    Region region = clip_region();
    if (region) XftDrawSetClip(draw, region);
    clip_state_number = fl_clip_state_number;
    clipped_draw = drawable->draw = draw;
  } else if (clip_state_number!=fl_clip_state_number || draw!=clipped_draw) {
    clip_state_number = fl_clip_state_number;
    clipped_draw = draw;
    XftDrawSetClip(draw, clip_region());
  }

  // Use fltk's color allocator, copy the results to match what
  // XftCollorAllocValue returns:
  static XftColor color;
  color.pixel = current_xpixel;
  uchar r,g,b; split_color(getcolor(), r,g,b);
  color.color.red   = r*0x101;
  color.color.green = g*0x101;
  color.color.blue  = b*0x101;
  color.color.alpha = 0xffff;

  XftDrawString8(draw, &color, current->font,
		 int(floorf(x+.5f)), int(floorf(y+.5f)),	
		 (XftChar8 *)str, n);
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:
static IFont fonts [] = {
  {{"sans",	0},	3,	0},
  {{"sans",	1},	3,	0},
  {{"sans",	2},	3,	0},
  {{"sans",	3},	3,	0},
  {{"mono",	0},	3,	0},
  {{"mono",	1},	3,	0},
  {{"mono",	2},	3,	0},
  {{"mono",	3},	3,	0},
  {{"serif",	0},	3,	0},
  {{"serif",	1},	3,	0},
  {{"serif",	2},	3,	0},
  {{"serif",	3},	3,	0},
  {{"symbol",	0},	0,	0},
  {{"screen",	0},	1,	0},
  {{"screen",	1},	1,	0},
  {{"dingbats",	0},	0,	0}
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
// The rest of this is for listing fonts:

extern "C" {
static int sort_function(const void *aa, const void *bb) {
  fltk::Font* a = *(fltk::Font**)aa;
  fltk::Font* b = *(fltk::Font**)bb;
  int ret = strcasecmp(a->name_, b->name_); if (ret) return ret;
  return a->attributes_ - b->attributes_;
}}

int fltk::list_fonts(fltk::Font**& arrayp) {
  static fltk::Font** font_array = 0;
  static int num_fonts = 0;

  if (font_array) { arrayp = font_array; return num_fonts; }

  XftFontSet *fs;
  char *name;
  open_display();
  fs = XftListFonts(xdisplay, xscreen, 0, XFT_FAMILY, 0);
  num_fonts = fs->nfont;
  font_array = (fltk::Font**)calloc(num_fonts, sizeof(fltk::Font *));
  for (int i = 0; i < num_fonts; i++) {
    if (XftPatternGetString(fs->fonts[i], XFT_FAMILY, 0, &name)
	== XftResultMatch) {
      // Make a block of 4 for bold, italic combinations
      IFont* newfont = new IFont[4];
      newfont[0].f.name_ = strdup(name);
      for (int j = 0; j < 4; j++) {
	newfont[j].f.name_ = newfont[0].f.name_;
	newfont[j].f.attributes_ = j;
	newfont[j].attribute_mask = 3;
	newfont[j].first = 0;
      }
      font_array[i] = &(newfont->f);
    }
  }
  XftFontSetDestroy(fs);

  qsort(font_array, num_fonts, sizeof(*font_array), sort_function);

  arrayp = font_array;
  return num_fonts;
}

////////////////////////////////////////////////////////////////

extern "C" {
static int int_sort(const void *aa, const void *bb) {
  return (*(int*)aa)-(*(int*)bb);
}
}

// Return all the point sizes supported by this font:
// Suprisingly enough Xft works exactly like fltk does and returns
// the same list. Except there is no way to tell if the font is scalable.
int fltk::Font::sizes(int*& sizep) {
  open_display();
  XftFontSet* fs = XftListFonts(xdisplay, xscreen,
				XFT_FAMILY, XftTypeString, name_, 0,
				XFT_PIXEL_SIZE, 0);
  static int* array = 0;
  static int array_size = 0;
  if (fs->nfont >= array_size) {
    delete[] array;
    array = new int[array_size = fs->nfont+1];
  }
  array[0] = 0; int j = 1; // claim all fonts are scalable by putting a 0 in
  for (int i = 0; i < fs->nfont; i++) {
    double v;
    if (XftPatternGetDouble(fs->fonts[i], XFT_PIXEL_SIZE, 0, &v)
	== XftResultMatch)
      array[j++] = int(v);
  }
  qsort(array+1, j-1, sizeof(int), int_sort);
  XftFontSetDestroy(fs);
  sizep = array;
  return j;
}

////////////////////////////////////////////////////////////////
// Return all the encodings for this font:

int fltk::Font::encodings(const char**& arrayp) {
  open_display();
  // we need to keep the previous return value around so the strings are
  // not destroyed.
  static XftFontSet* fs;
  if (fs) XftFontSetDestroy(fs);
  fs = XftListFonts(xdisplay, xscreen,
		    XFT_FAMILY, XftTypeString, name_, 0,
		    XFT_ENCODING, 0);
  static const char** array = 0;
  static int array_size = 0;
  if (fs->nfont > array_size) {
    delete[] array;
    array = new (const char*)[array_size = fs->nfont];
  }
  int j = 0;
  for (int i = 0; i < fs->nfont; i++) {
    char* v;
    if (XftPatternGetString(fs->fonts[i], XFT_ENCODING, 0, &v) == XftResultMatch) {
      array[j++] = v;
    }
  }
  arrayp = array;
  return j;
}

//
// End of "$Id: fl_font_xft.cxx,v 1.13 2002/12/10 09:43:26 spitzak Exp $"
//
