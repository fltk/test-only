//
// "$Id: fl_xft.cxx,v 1.1 2001/02/21 06:15:45 clip Exp $"
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
// 3. Your X device driver works with the render extension (NVidia's
//    OpenGL accelerated driver doesn't)
// 4. You have libXft
// 5. Your libXft has freetype2 support compiled in
// 6. You have the freetype2 library
//


#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Font.H>
#include <FL/x.H>

#include <stdio.h>
#include <stdlib.h>

static int min_antialias_size = 16;

static void *
xft_font_load(const char *name, const char */*encoding*/, int size) {
  static int do_once = 0;
  if (!do_once) {
    do_once = 1;
    if (!XftDefaultHasRender(fl_display)) {
      fprintf(stderr, "Cannot start Xft plugin: Display does not "
                      "have render capability.\n");
      abort();
    }
  }

  //XftFont *font = XftFontOpenXlfd(fl_display, fl_screen, name);

  XftPattern *pat = XftXlfdParse(name, False, False);

  XftPatternAddBool(pat, XFT_MINSPACE, 1);
  //XftPatternDel(pat, XFT_PIXEL_SIZE);
  //XftPatternAddInteger(pat, XFT_SIZE, size);
  //printf("size:%d  min:%d\n", size ,min_antialias_size);
  if (size < min_antialias_size)
    XftPatternAddBool(pat, XFT_ANTIALIAS, 0);
  //printf("\ndesired:\n"); XftPatternPrint(pat);
  if (!pat) return 0;
  XftResult result;
  XftPattern *match = XftFontMatch(fl_display, fl_screen, pat, &result);
  XftPatternDestroy(pat);
  if (!match) return 0;
  //printf("\nreceived:\n"); XftPatternPrint(match);

  XftFont *font = XftFontOpenPattern(fl_display, match);
  if (!font) XftPatternDestroy(match);
  //printf("height: %d\n", font->height);

  return font;
}

static void
xft_font_unload(void *font) {
  XftFontClose(fl_display, (XftFont *)font);
}

static XftDraw *draw = 0;

static int
xft_font_height() { return ((XftFont *)fl_xfont)->height; }

static int
xft_font_descent() { return ((XftFont *)fl_xfont)->descent; }

static int
xft_font_width(const char *str, int n) {
  XGlyphInfo i;
  XftTextExtents8(fl_display, (XftFont *)fl_xfont, (XftChar8 *)str, n, &i);
  return i.xOff;
}

Region region = 0;
static void
xft_font_draw(const char *str, int n, int x, int y) {
  if (!draw)
    draw = XftDrawCreate(fl_display, fl_window, fl_visual->visual, fl_colormap);
  else
    XftDrawChange(draw, fl_window);

  if (!XEmptyRegion(region)) XftDrawSetClip(draw, region);

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

static Fl_Font_Renderer
xft_renderer = {
  xft_font_load, xft_font_unload, xft_font_height,
  xft_font_descent, xft_font_width, xft_font_draw,
  xft_font_clip
};


int
fl_xft() {
  if (fl_display) {
    fprintf(stderr, "Cannot start Xft plugin after display has been opened!\n");
    return 1;
  }

  char temp[4];
  if (!fl_getconf("xft plugin/minimum antialias size", temp, sizeof(temp)))
    min_antialias_size = atol(temp);

  fl_font_renderer = &xft_renderer;

  return 0;
}


//
// End of "$Id: fl_xft.cxx,v 1.1 2001/02/21 06:15:45 clip Exp $"
//
