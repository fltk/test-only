//
// "$Id: fl_color_win32.cxx,v 1.28 2001/07/29 22:04:44 spitzak Exp $"
//
// _WIN32 color functions for the Fast Light Tool Kit (FLTK).
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

// This file does not compile independently, it is included by fl_color.cxx

#include <config.h>
#include <fltk/win32.h>
#include <stdio.h>

// The current color:
Fl_Color	fl_color_;
COLORREF	fl_colorref;
HBRUSH		fl_brush;
HPEN		fl_pen;
HPALETTE	fl_palette;

COLORREF fl_wincolor(Fl_Color i) {
  int index = i;
  COLORREF rgb;
  // this is the same as fl_nearest_color(i) but it also sets rgb:
  if (i & 0xFFFFFF00) {
    uchar r = i>>24;
    uchar g = i>>16;
    uchar b = i>> 8;
    rgb = RGB(r,g,b);
    if (r == g && r == b)
      index = fl_gray_ramp(r*FL_NUM_GRAY/256);
    else
      index =
         fl_color_cube(r*FL_NUM_RED/256,g*FL_NUM_GREEN/256,b*FL_NUM_BLUE/256);
  } else {
    unsigned c = fl_cmap[i];
    rgb = RGB(uchar(c>>24), uchar(c>>16), uchar(c>>8));
  }
#if USE_COLORMAP
  if (fl_palette) rgb = PALETTEINDEX(index);
#endif
  return rgb;
}

static int line_style = 0;
static DWORD dash_pattern[16];
static int dash_pattern_size = 0;
static int line_width = 0;

void fl_line_style(int style, int width, char* dashes) {
  static DWORD Cap[4]= {PS_ENDCAP_ROUND, PS_ENDCAP_FLAT, PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE};
  static DWORD Join[4]={PS_JOIN_ROUND, PS_JOIN_MITER, PS_JOIN_ROUND, PS_JOIN_BEVEL};
  line_style = PS_GEOMETRIC | Cap[(style>>8)&3] | Join[(style>>12)&3];
  if (dashes && dashes[0]) {
    line_style |= PS_USERSTYLE;
    int n; for (n = 0; n < 16 && *dashes;) dash_pattern[n++] = *dashes++;
    dash_pattern_size = n;
  } else {
    dash_pattern_size = 0;
    line_style |= style & 0xff; // allow them to pass any low 8 bits for style
  }
  line_width = width;
  // fix cards that ignore dash pattern for zero width:
  if (!width && (line_style || dash_pattern_size)) line_width = 1;
  HPEN newpen = fl_pen = fl_create_pen();
  if (newpen) {
    HPEN oldpen = fl_pen = (HPEN)SelectObject(fl_gc, newpen);
    if (oldpen) DeleteObject(oldpen);
  } else {
    // CET - FIXME - remove this debug fprintf()?
    fprintf(stderr, "fl_line_style(): Could not create GDI pen object.\n");
  }
}

HPEN fl_create_pen() {
  if (line_style || line_width || dash_pattern_size) {
    LOGBRUSH penbrush = {BS_SOLID, fl_colorref, 0}; // can this be fl_brush?
    return ExtCreatePen(line_style, line_width, &penbrush,
			dash_pattern_size, dash_pattern_size?dash_pattern:0);
  } else {
    return CreatePen(PS_SOLID, 1, fl_colorref);
  }
}

void fl_color(Fl_Color i) {
  fl_color_ = i;
  fl_colorref = fl_wincolor(i);

  HBRUSH newbrush = CreateSolidBrush(fl_colorref);
  if (newbrush) {
    fl_brush = newbrush;
    HBRUSH oldbrush = (HBRUSH)SelectObject(fl_gc, newbrush);
    if (oldbrush) DeleteObject(oldbrush);
  } else {
    // CET - FIXME - remove this debug fprintf()?
    fprintf(stderr, "fl_color(): Could not create GDI brush object.\n");
  }
  HPEN newpen = fl_pen = fl_create_pen();
  if (newpen) {
    HPEN oldpen = fl_pen = (HPEN)SelectObject(fl_gc, newpen);
    if (oldpen) DeleteObject(oldpen);
  } else {
    // CET - FIXME - remove this debug fprintf()?
    fprintf(stderr, "fl_color(): Could not create GDI pen object.\n");
  }
}

void fl_free_color(Fl_Color) {
  // CET - FIXME - Are there colormapped displays on Windows?
  //               Would this only be used for private colormaps?
}

#if USE_COLORMAP

// 'fl_select_palette()' - Make a color palette for 8-bit displays if necessary
// Thanks to Michael Sweet @ Easy Software Products for this

HPALETTE
fl_select_palette(void)
{
  static char beenhere;
  if (!beenhere) {
    beenhere = 1;

    //if (GetDeviceCaps(fl_gc, BITSPIXEL) > 8) return NULL;
    int nColors = GetDeviceCaps(fl_gc, SIZEPALETTE);
    if (nColors <= 0 || nColors > 256) return NULL;
    // this will try to work on < 256 color screens, but will probably
    // come out quite badly.

    // I lamely try to get this variable-sized object allocated on stack:
    ulong foo[(sizeof(LOGPALETTE)+256*sizeof(PALETTEENTRY))/sizeof(ulong)+1];
    LOGPALETTE *pPal = (LOGPALETTE*)foo;

    pPal->palVersion    = 0x300;
    pPal->palNumEntries = nColors;

    // Build 256 colors from the standard FLTK colormap...

    for (int i = 0; i < nColors; i ++) {
      pPal->palPalEntry[i].peRed   = (fl_cmap[i] >> 24) & 255;
      pPal->palPalEntry[i].peGreen = (fl_cmap[i] >> 16) & 255;
      pPal->palPalEntry[i].peBlue  = (fl_cmap[i] >>  8) & 255;
      pPal->palPalEntry[i].peFlags = 0;
    };

    // Create the palette:
    fl_palette = CreatePalette(pPal);
  }
  if (fl_palette) {
    SelectPalette(fl_gc, fl_palette, FALSE);
    RealizePalette(fl_gc);
  }
  return fl_palette;
}

#endif

//
// End of "$Id: fl_color_win32.cxx,v 1.28 2001/07/29 22:04:44 spitzak Exp $".
//
