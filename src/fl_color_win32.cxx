//
// "$Id: fl_color_win32.cxx,v 1.23 2000/07/21 00:31:52 clip Exp $"
//
// WIN32 color functions for the Fast Light Tool Kit (FLTK).
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

// The fltk "colormap".  This allows ui colors to be stored in 8-bit
// locations, and provides a level of indirection so that global color
// changes can be made.  Not to be confused with the X colormap, which
// I try to hide completely.

// SGI compiler seems to have problems with unsigned char arguments
// being used to index arrays.  So I always copy them to an integer
// before use.

#include <config.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/win32.H>
#include <FL/fl_draw.H>

static unsigned fl_cmap[256] = {
#include "fl_cmap.h" // this is a file produced by "cmap.cxx":
};

// The current color:
Fl_Color	fl_color_;
COLORREF	fl_colorref;
FL_API HPEN	fl_pen;
FL_API HBRUSH	fl_brush;
HPALETTE	fl_palette;

void fl_color(Fl_Color i) {
  fl_color_ = i;
  int index;
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
    index = i;
    unsigned c = fl_cmap[i];
    rgb = RGB(uchar(c>>24), uchar(c>>16), uchar(c>>8));
  }
#if USE_COLORMAP
  if (fl_palette) rgb = PALETTEINDEX(index);
#endif

  HPEN newpen = CreatePen(PS_SOLID, 1, rgb);
  if (!newpen) {
    // CET - FIXME - remove this debug fprintf()?
    fprintf(stderr, "fl_color(): Could not create GDI pen object.\n");
    return;
  }
  HBRUSH newbrush = CreateSolidBrush(rgb);
  if (!newbrush) {
    // CET - FIXME - remove this debug fprintf()?
    fprintf(stderr, "fl_color(): Could not create GDI brush object.\n");
    DeleteObject(newpen);
    return;
  }
  HPEN oldpen = (HPEN)SelectObject(fl_gc, newpen); // this returns the old pen
  if (oldpen) DeleteObject(oldpen);
  HBRUSH oldbrush = (HBRUSH)SelectObject(fl_gc, newbrush); // this returns the old brush
  if (oldbrush) DeleteObject(oldbrush);

  fl_colorref = rgb;
  fl_pen = newpen;
  fl_brush = newbrush;
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
// End of "$Id: fl_color_win32.cxx,v 1.23 2000/07/21 00:31:52 clip Exp $".
//
