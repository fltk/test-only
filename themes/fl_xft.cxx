//
// "$Id: fl_xft.cxx,v 1.2 2001/07/10 08:14:39 clip Exp $"
//
// Plugin file for FLTK
//
// Copyright 1999 Bill Spitzak and others.
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

// Plugin for fltk to use the Xft library for font rendering.  This enables
// your fonts to display anti-aliased and slow just like Windows!
//
// This plugin only works if
// 1. You have XFree86 4
// 2. You have the render extension
// 3. Your X device driver supports the render extension
// 4. You have libXft
// 5. Your libXft has freetype2 support compiled in
// 6. You have the freetype2 library
//
// Distributions that have XFree86 4.0.3 or later should have all of this...


#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Font.H>
#include <FL/x.H>
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int min_antialias_size = 16;

struct FontSize {
  const char *name;
  const char *encoding;
  unsigned size;
  void *font;

  FontSize *next;

  FontSize(const char *n, const char *e, int s) {
    name = strdup(n); encoding = strdup(e); size = s;
    font = fl_font_renderer->load(name, encoding, size);
  }

  ~FontSize() { free((void *)name); free((void *)encoding); }
};

static FontSize *fontsize;

static void
xft_font(Fl_Font font, unsigned size) {
//printf("xft_font(Fl_Font font = %p, unsigned size = %u)\n", font, size);
//printf("  font->name_ = \"%s\"\n", font->name_);
//printf("  fl_encoding_ = \"%s\"\n", fl_encoding_);

//printf("here1: %s\n", font->name_);
  if (font == fl_font_ && size == fl_size_ /*&&
      !strcasecmp(fontsize->encoding, fl_encoding_)*/)
    return;
//printf("here2\n");
  fl_font_ = font; fl_size_ = size;

  FontSize* f;
  // search the fontsizes we have generated already
  for (f = (FontSize *)font->first; f; f = f->next) {
//printf("  f->encoding = %s\n", f->encoding);
    if (f->size == size /*&&
        !strcasecmp(f->encoding, fl_encoding_)*/)
      break;
  }
//printf("here3\n");
  if (!f) {
//f = new FontSize("courier", "iso8859-1", 12);
    f = new FontSize(font->name_, fl_encoding_, size);

    f->next = (FontSize *)font->first;
    ((Fl_Font_*)font)->first = f;
//printf("here7\n");
  }

  if (f != fontsize) {
    fontsize = f;
    fl_xfont = f->font;
  }
//printf("here8\n");
//    fl_xfont = fl_font_renderer->load("courier", "iso8859-1", 12);
}

static void *
xft_font_load(const char *name, const char *encoding, int size) {
//printf("xft_font_load(const char *name = \"%s\", const char *encoding = \"%s\", int size = %d)\n",
//       name, encoding, size);

  int slant = XFT_SLANT_ROMAN;
  int weight = XFT_WEIGHT_MEDIUM;
  Bool antialias = 1;

  char *n = strdup(name);
  int l = strlen(n);
  if (l > 7 && !strcasecmp(n+l-7, " italic")) {
    *(n+l-7) = (char)0;
    slant = XFT_SLANT_ITALIC;
  }

  l = strlen(n);
  if (l > 5 && !strcasecmp(n+l-5, " bold")) {
    *(n+l-5) = (char)0;
    weight = XFT_WEIGHT_BOLD;
  }

  if (size < min_antialias_size) antialias = 0;

  // this call is extremely slow...
  XftFont *font = XftFontOpen(fl_display, fl_screen,
                              XFT_FAMILY, XftTypeString, n,
                              XFT_SLANT, XftTypeInteger, slant,
                              XFT_WEIGHT, XftTypeInteger, weight,
                              XFT_SIZE, XftTypeDouble, (double)size,
                              XFT_ENCODING, XftTypeString, encoding,
                              XFT_ANTIALIAS, XftTypeBool, antialias,
                              0);
  free(n);

  return font;
}

static void
xft_font_unload(void *font) {
//printf("xft_font_unload(void *font = %p)\n", font);
  XftFontClose(fl_display, (XftFont *)font);
}

static XftDraw *draw = 0;

static int
xft_font_height() { return ((XftFont *)fl_xfont)->height; }

static int
xft_font_descent() { return ((XftFont *)fl_xfont)->descent; }

static int
xft_font_width(const char *str, int n) {
//printf("xft_font_width(const char *, int)\n");
  XGlyphInfo i;
  XftTextExtents8(fl_display, (XftFont *)fl_xfont, (XftChar8 *)str, n, &i);
  return i.xOff;
}

Region region = 0;
static void
xft_font_draw(const char *str, int n, int x, int y) {
//printf("xft_font_draw(const char *, int, int, int)\n");
  if (!draw)
    draw = XftDrawCreate(fl_display, fl_window, fl_visual->visual, fl_colormap);
  else
    XftDrawChange(draw, fl_window);

  if (region && !XEmptyRegion(region)) XftDrawSetClip(draw, region);

  uchar r, g, b;
  fl_get_color(fl_color_, r, g, b);
  XRenderColor fc = { r<<8, g<<8, b<<8, 0xffff };
  XftColor color;
  XftColorAllocValue(fl_display, fl_visual->visual, fl_colormap, &fc, &color);

  XftDrawString8(draw, &color, (XftFont *)fl_xfont, x+fl_x_, y+fl_y_,
                    (XftChar8 *)str, n);

  XftColorFree(fl_display, fl_visual->visual, fl_colormap, &color);
}

static void
xft_font_clip(Region r) {
  if (region) XDestroyRegion(region);
  region = XCreateRegion();
  if (r) XUnionRegion(r, region, region);
}

extern "C" {
static int sort_function(const void *aa, const void *bb) {
  return strcasecmp(((Fl_Font)aa)->name_, ((Fl_Font)bb)->name_);
}
}

int
xft_font_list(Fl_Font*& arrayp) {
  static Fl_Font *font_array = 0;
  static int num_fonts = 0;

  if (font_array) { arrayp = font_array; return num_fonts; }

  XftFontSet *fs;
  char *name;
  Fl_Font_ *f;
  fs = XftListFonts(fl_display, fl_screen, 0, XFT_FAMILY, 0);
  num_fonts = fs->nfont;
  font_array = (Fl_Font *)calloc(num_fonts, sizeof(Fl_Font *));
  for (int i = 0; i < num_fonts; i++) {
    if (XftPatternGetString(fs->fonts[i], XFT_FAMILY, 0, &name) == XftResultMatch) {
      f = new Fl_Font_;
      f->name_ = strdup(name);
      f->bold_ = f;
      f->italic_ = f;
      f->first = 0;
      font_array[i] = f;
    }
  }
  XftFontSetDestroy(fs);

  qsort(font_array, num_fonts, sizeof(Fl_Font), sort_function);

  arrayp = font_array;
  return num_fonts;
}

static Fl_Font_
xft_fonts[] = {
{"sans",                   xft_fonts+1, xft_fonts+2},
{"sans bold",              xft_fonts+1, xft_fonts+3},
{"sans italic",            xft_fonts+3, xft_fonts+2},
{"sans bold italic",       xft_fonts+3, xft_fonts+3},
{"mono",                     xft_fonts+5, xft_fonts+6},
{"mono bold",                xft_fonts+5, xft_fonts+7},
{"mono italic",              xft_fonts+7, xft_fonts+6},
{"mono bold italic",         xft_fonts+7, xft_fonts+7},
{"serif",                       xft_fonts+9, xft_fonts+10},
{"serif bold",                  xft_fonts+9, xft_fonts+11},
{"serif italic",                xft_fonts+11,xft_fonts+10},
{"serif bold italic",           xft_fonts+11,xft_fonts+11},
{"symbol",                      xft_fonts+12,xft_fonts+12},
{"lucidatypewriter",            xft_fonts+14,xft_fonts+14},
{"lucidatypewriter bold",       xft_fonts+14,xft_fonts+14},
{"zapf dingbats",               xft_fonts+15,xft_fonts+15},
};

static Fl_Font_Renderer
xft_renderer = {
  xft_font, xft_font_load, xft_font_unload, xft_font_height, xft_font_descent,
  xft_font_width, xft_font_draw, xft_font_clip, xft_font_list, xft_fonts
};


int
fl_xft() {
  if (fl_display) {
    fprintf(stderr, "Cannot start Xft plugin: Display previously opened.\n");
    return 1;
  }

  if (!getenv("DISPLAY")) {
    fprintf(stderr, "Cannot start Xft plugin: DISPLAY not set.\n");
    return 2;
  }

  fl_open_display();

  if (!XftDefaultHasRender(fl_display)) {
    fprintf(stderr, "Cannot start Xft plugin: Display does not "
                    "have render capability.\n");
    return 3;
  }

  fl_font_renderer = &xft_renderer;

  char temp[4];
  if (!fl_getconf("xft plugin/minimum antialias size", temp, sizeof(temp)))
    min_antialias_size = atol(temp);

  return 0;
}


//
// End of "$Id: fl_xft.cxx,v 1.2 2001/07/10 08:14:39 clip Exp $"
//
