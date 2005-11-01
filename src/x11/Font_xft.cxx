//
// "$Id$"
//
// Copyright 2004 Bill Spitzak and others.
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

// Draw fonts using Keith Packard's Xft and Xrender extension. Yow!
// Many thanks to Carl for making the original version of this.
//
// In older versions Xft fell back on using X11 fonts if it could not
// use the Xrender extension. Xft2 sends bitmaps, which actually works
// acceptably even to a remote X server. Unfortunatly it lost the
// ability to return a "similar" X11 font, which broke our code to
// get matching fonts into OpenGL. It would probably be best to fix
// this by having the OpenGL emulation access the bitmaps as well,
// so there would be antialiasing there, too!
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
#include <fltk/string.h>
#include <fltk/utf.h>
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
#include <stdlib.h>

using namespace fltk;

// One of these is made for each combination of size + encoding:
struct FontSize {
  unsigned size10;	// size*10
  XftFont* font;
  const char* encoding;
  XFontStruct* xfont;
  //~FontSize();
};

// The xft font implementation adds the xft name and the above list:
struct IFont {
  fltk::Font f;
  int attribute_mask; // attributes that can still be turned on
  unsigned numsizes;
  FontSize* fontsizes;
};

// We store the attributes in neat blocks of 2^n:
fltk::Font* fltk::Font::plus(int x) {
  IFont* font = (IFont*)this;
  x &= font->attribute_mask;
  return &((font+x)->f);
}

const char* fltk::Font::system_name() {
  return name_;
}

static FontSize* current;

static XftFont* fontopen(const char* name, int attributes, bool core) {
  open_display();
  int weight = XFT_WEIGHT_MEDIUM;
  if (attributes&BOLD) weight = XFT_WEIGHT_BOLD;
  int slant = XFT_SLANT_ROMAN;
  if (attributes&ITALIC) slant = XFT_SLANT_ITALIC;

  return XftFontOpen(xdisplay, xscreen,
		     XFT_FAMILY, XftTypeString, name,
		     XFT_WEIGHT, XftTypeInteger, weight,
		     XFT_SLANT, XftTypeInteger, slant,
		     //XFT_ENCODING, XftTypeString, encoding_,
		     XFT_PIXEL_SIZE, XftTypeDouble, (double)current_size_,
		     core ? XFT_CORE : 0, XftTypeBool, true,
		     XFT_RENDER, XftTypeBool, false,
		     0);
}

void fltk::setfont(fltk::Font* font, float rsize) {
  // Reduce the number of sizes by rounding to various multiples:
  unsigned size10 = unsigned(10*rsize+.5);
#ifndef XFT_MAJOR
  // Older Xft craps out with tiny sizes and returns null for the font
  if (size10 < 20) size10 = 20;
#endif
  if (font == current_font_ && current->size10 == size10 &&
      !strcasecmp(current->encoding, encoding_))
    return;
  current_font_ = font; current_size_ = size10/10.0f;
  // binary search the fontsizes we have generated already
  FontSize* array = ((IFont*)font)->fontsizes;
  unsigned n = ((IFont*)font)->numsizes;
  unsigned a = 0; unsigned b = n;
  while (a < b) {
    unsigned c = (a+b)/2;
    FontSize* f = array+c;
    int d = f->size10-size10;
    if (!d) d = strcasecmp(f->encoding, encoding_);
    if (d < 0) a = c+1;
    else if (d > 0) b = c;
    else {current = f; return;}
  }
  if (!(n&(n+1))) {
    unsigned m = 2*(n+1)-1;
    FontSize* newarray = new FontSize[m];
    memcpy(newarray, array, a*sizeof(FontSize));
    memcpy(newarray+a+1, array+a, (n-a)*sizeof(FontSize));
    delete[] array;
    ((IFont*)font)->fontsizes = array = newarray;
  } else {
    memmove(array+a+1, array+a, (n-a)*sizeof(FontSize));
  }
  ((IFont*)font)->numsizes = n+1;
  FontSize* f = array+a;
  f->size10 = size10;
  f->encoding = encoding_;
  f->font = fontopen(font->name_, font->attributes_, false);
  f->xfont = 0; // figure this out later
  current = f;
}

#if 0 // this is never called!
FontSize::~FontSize() {
  if (this == current) current = 0;
  XftFontClose(xdisplay, font);
}
#endif

// This call is used by gl_setfont to get a bitmapped font. Xft actually did
// a pretty good job of selecting X fonts. Unfortunately Xft2 seems to have
// completely hidden or removed this interface...
XFontStruct* fltk::xfont() {
  if (!current->xfont) {
#if 1 //defined(XFT_MAJOR) && XFT_MAJOR >= 2
    // kludge! Select Xfonts for the fltk built-in ones, uses "variable"
    // for everything else. Assummes the Xfont setup from RedHat 9:
    const char *name = current_font_->name();
    char *myname, xname[1024];
    int slo = 0;
    if (strncmp(name, "sans", 4)==0) {
      myname = "-*-helvetica-%s-%s-normal-*-%d-*-*-*-*-*-*-*";
    } else if (strncmp(name, "mono", 4)==0) {
      myname = "-*-courier-%s-%s-normal-*-%d-*-*-*-*-*-*-*";
    } else if (strncmp(name, "serif", 5)==0) {
      myname = "-*-times-%s-%s-normal-*-%d-*-*-*-*-*-*-*"; slo = 2;
    } else if (strncmp(name, "symbol", 6)==0) {
      myname = "-*-symbol-%s-%s-*-*-%d-*-*-*-*-*-*-*";
    } else if (strncmp(name, "screen", 6)==0) {
      myname = "-*-clean-%s-%s-*-*-%d-*-*-*-*-*-*-*"; slo = 2;
    } else if (strncmp(name, "dingbats", 8)==0) {
      myname = "-*-*zapf dingbats-%s-%s-*-*-%d-*-*-*-*-*-*-*";
    } else {
      myname = "-*-helvetica-%s-%s-normal-*-%d-*-*-*-*-*-*-*";
    }
    static char *wghtLUT[] = { "medium", "bold" };
    static char *slantLUT[] = { "r", "o", "r", "i" };
    sprintf(xname, myname,
	    wghtLUT[(current_font_->attributes_&BOLD)!=0],
	    slantLUT[slo+((current_font_->attributes_&ITALIC)!=0)],
	    int(current_size_+0.5));
    XFontStruct *myFont = XLoadQueryFont(xdisplay, xname);
    if (!myFont) {
      static XFontStruct* some_font = 0;
      if (!some_font) some_font = XLoadQueryFont(xdisplay, "variable");
      myFont = some_font;
    }
    current->xfont = myFont;
#else
    if (current->font->core) {
      current->xfont = current->font->u.core.font;
    } else {
      static XftFont* xftfont;
      if (xftfont) XftFontClose (xdisplay, xftfont);
      // select the "core" version of the font:
      xftfont = fontopen(current_font_->name_,current_font_->attributes_,true);
      current->xfont = xftfont->u.core.font;
    }
#endif
  }
  return current->xfont;
}

const char* fltk::Font::current_name() {
  return current_font_->name_;
}

float fltk::getascent()  { return current->font->ascent; }
float fltk::getdescent() { return current->font->descent; }

// Unfortunatly the Xft UTF-8 interface barfs on error sequences and
// does not print anything. This is very annoying. Alternative version
// here will user our converter to wchar_t and print that, but I would
// rather see Xft fixed.
//
// Older Xft seems to crash xlib on certain lengths. This was a real
// pain to track down!
#define WCBUFLEN 256

float fltk::getwidth(const char *str, int n) {
  XGlyphInfo i;
#if 0
  XftTextExtentsUtf8(xdisplay, current->font, (XftChar8*)str, n, &i);
#else
  wchar_t localbuffer[WCBUFLEN];
  wchar_t* buffer = localbuffer;
  wchar_t* mallocbuffer = 0;
  int count = utf8towc(str, n, buffer, WCBUFLEN);
  if (count >= WCBUFLEN) {
    buffer = mallocbuffer = new wchar_t[count+3];
    count = utf8towc(str, n, buffer, count+3);
  }
  // fix some lengths that cause older Xft to crash (!):
  if ((count&255)==253) buffer[count++] = 0;
  if ((count&255)==254) buffer[count++] = 0;
  XftTextExtents32(xdisplay, current->font, (XftChar32*)buffer, count, &i);
  delete[] mallocbuffer;
#endif
  return i.xOff;
}

////////////////////////////////////////////////////////////////

void fltk::drawtext_transformed(const char *str, int n, float x, float y) {

  // Use fltk's color allocator, copy the results to match what
  // XftCollorAllocValue returns:
  XftColor color;
  color.pixel = current_xpixel;
  uchar r,g,b; split_color(getcolor(), r,g,b);
  color.color.red   = r*0x101;
  color.color.green = g*0x101;
  color.color.blue  = b*0x101;
  color.color.alpha = 0xffff;

#if 0
  XftDrawStringUtf8(xftc, &color, current->font,
		    int(floorf(x+.5f)), int(floorf(y+.5f)),
		    (XftChar8*)str, n);
#else
  wchar_t localbuffer[WCBUFLEN];
  wchar_t* buffer = localbuffer;
  wchar_t* mallocbuffer = 0;
  int count = utf8towc(str, n, buffer, WCBUFLEN);
  if (count >= WCBUFLEN) {
    buffer = mallocbuffer = new wchar_t[count+3];
    count = utf8towc(str, n, buffer, count+3);
  }
  // fix some lengths that cause older Xft to crash (!):
  if ((count&255)==253) buffer[count++] = ' ';
  if ((count&255)==254) buffer[count++] = ' ';
  XftDrawString32(xftc, &color, current->font,
		  int(floorf(x+.5f)), int(floorf(y+.5f)),
		  (XftChar32*)buffer, count);
  delete[] mallocbuffer;
#endif
}

////////////////////////////////////////////////////////////////

// The predefined fonts that fltk has:
static IFont fonts [] = {
  {{"sans",	0},	3,	0},
  {{"sans",	1},	2,	0},
  {{"sans",	2},	1,	0},
  {{"sans",	3},	0,	0},
  {{"mono",	0},	3,	0},
  {{"mono",	1},	2,	0},
  {{"mono",	2},	1,	0},
  {{"mono",	3},	0,	0},
  {{"serif",	0},	3,	0},
  {{"serif",	1},	2,	0},
  {{"serif",	2},	1,	0},
  {{"serif",	3},	0,	0},
  {{"symbol",	0},	0,	0},
  {{"screen",	0},	1,	0},
  {{"screen",	1},	0,	0},
  {{"dingbats",	0},	0,	0}
};

fltk::Font* const fltk::HELVETICA		= &(fonts[0].f);
fltk::Font* const fltk::HELVETICA_BOLD		= &(fonts[1].f);
fltk::Font* const fltk::HELVETICA_ITALIC	= &(fonts[2].f);
fltk::Font* const fltk::HELVETICA_BOLD_ITALIC	= &(fonts[3].f);
fltk::Font* const fltk::COURIER			= &(fonts[4].f);
fltk::Font* const fltk::COURIER_BOLD		= &(fonts[5].f);
fltk::Font* const fltk::COURIER_ITALIC		= &(fonts[6].f);
fltk::Font* const fltk::COURIER_BOLD_ITALIC	= &(fonts[7].f);
fltk::Font* const fltk::TIMES			= &(fonts[8].f);
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

// Apparently the 2nd name is more interesting than the first (?)
static const char* getfamily(FcPattern* p) {
  FcChar8* s;
  if (FcPatternGetString(p, FC_FAMILY, 1, &s) != 0)
    FcPatternGetString(p, FC_FAMILY, 0, &s);
  return (const char*)s;
}

int fltk::list_fonts(fltk::Font**& arrayp) {
  static fltk::Font** font_array = 0;
  static int num_fonts = 0;
  if (font_array) { arrayp = font_array; return num_fonts; }

  // Make sure fontconfig is ready... is this necessary? The docs say it is
  // safe to call it multiple times, so just go for it anyway!
  FcInit();

  // Create a search pattern that will match every font name - I think
  // this does the Right Thing... but am not certain...
  FcPattern* fnt_pattern = FcPatternCreate ();
  // Now list all the families:
  FcObjectSet* fnt_obj_set = FcObjectSetBuild (FC_FAMILY, 0);
  // This gives more info, but it is a pain to deal with:
  //FcObjectSet* fnt_obj_set = FcObjectSetBuild (FC_FAMILY, FC_WEIGHT, FC_SLANT, 0);

  // Hopefully, this is a set of all the fonts...
  FcFontSet* fnt_set = FcFontList (0, fnt_pattern, fnt_obj_set);
  
  // We don't need the fnt_pattern any more, release it
  FcPatternDestroy (fnt_pattern);

  num_fonts = fnt_set->nfont;
  font_array = new fltk::Font*[num_fonts];

  // For each family we make 4 fonts for bold,italic combinations
  // The families could be smaller if there is no bold or italic, but
  // that is a pain to figure out. Besides this is what Windows
  // version does:
  IFont* fonts = new IFont[num_fonts*4];
  for (int i = 0; i < num_fonts; i++) {
    IFont* newfont = fonts+4*i;
    font_array[i] = &(newfont->f);
    FcPattern* p = fnt_set->fonts[i];
    const char* name = strdup(getfamily(p));
    for (int j = 0; j < 4; j++) {
      newfont[j].f.name_ = name;
      newfont[j].f.attributes_ = j;
      newfont[j].attribute_mask = 3-j;
      newfont[j].numsizes = 0;
      newfont[j].fontsizes = 0;
    }
  }
  // Release the fnt_set - we don't need it any more
  FcFontSetDestroy (fnt_set);

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
  int j = 0;
  for (int i = 0; i < fs->nfont; i++) {
    double v;
    if (XftPatternGetDouble(fs->fonts[i], XFT_PIXEL_SIZE, 0, &v)
	== XftResultMatch)
      array[j++] = int(v);
  }
  if (j) {
    qsort(array, j, sizeof(int), int_sort);
  } else {
    array[0] = 0; j = 1; // claim all fonts are scalable by putting a 0 in
  }
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
    array = new const char*[array_size = fs->nfont];
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
// End of "$Id$"
//
