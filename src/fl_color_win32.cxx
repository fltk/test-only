//
// "$Id: fl_color_win32.cxx,v 1.32 2002/12/10 01:46:18 easysw Exp $"
//
// _WIN32 color functions for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// This file does not compile independently, it is included by color.cxx

// The current color:
Color		fltk::current_color_;
COLORREF	fltk::current_xpixel;
HBRUSH		fltk::current_brush;
HPEN		fltk::current_pen;
HPALETTE	fltk::xpalette;

COLORREF fltk::xpixel(Color i) {
  int index = i;
  COLORREF rgb;
  // this is the same as nearest_color(i) but it also sets rgb:
  if (i & 0xffffff00) { // an rgb color
    uchar r = i>>24;
    uchar g = i>>16;
    uchar b = i>> 8;
    rgb = RGB(r,g,b);
    //if (r == g && r == b) index = GRAY00+r*(GRAY99-GRAY00)/256;
    index = (BLACK + (b*5/256 * 5 + r*5/256) * 8 + g*8/256);
  } else {
    unsigned c = cmap[i];
    rgb = RGB(uchar(c>>24), uchar(c>>16), uchar(c>>8));
  }
#if USE_COLORMAP
  if (xpalette) rgb = PALETTEINDEX(index);
#endif
  return rgb;
}

static int lstyle = 0;
static DWORD dash_pattern[16];
static int dash_pattern_size = 0;
static int line_width = 0;

void fltk::line_style(int style, int width, char* dashes) {
  static DWORD Cap[4]= {PS_ENDCAP_ROUND, PS_ENDCAP_FLAT, PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE};
  static DWORD Join[4]={PS_JOIN_ROUND, PS_JOIN_MITER, PS_JOIN_ROUND, PS_JOIN_BEVEL};
  lstyle = PS_GEOMETRIC | Cap[(style>>8)&3] | Join[(style>>12)&3];
  if (dashes && dashes[0]) {
    lstyle |= PS_USERSTYLE;
    int n; for (n = 0; n < 16 && *dashes;) dash_pattern[n++] = *dashes++;
    dash_pattern_size = n;
  } else {
    dash_pattern_size = 0;
    lstyle |= style & 0xff; // allow them to pass any low 8 bits for style
  }
  line_width = width;
  // fix cards that ignore dash pattern for zero width:
  if (!width && (lstyle || dash_pattern_size)) line_width = 1;
  current_pen = create_pen();
  HPEN oldpen = (HPEN)SelectObject(gc, current_pen);
  if (oldpen) DeleteObject(oldpen);
}

HPEN fltk::create_pen() {
  if (lstyle || line_width || dash_pattern_size) {
    LOGBRUSH penbrush = {BS_SOLID, current_xpixel, 0}; // can this be brush?
    return ExtCreatePen(lstyle, line_width, &penbrush,
			dash_pattern_size, dash_pattern_size?dash_pattern:0);
  } else {
    return CreatePen(PS_SOLID, 1, current_xpixel);
  }
}

void fltk::setcolor(Color i) {
  current_color_ = i;
  current_xpixel = xpixel(i);

  current_brush = CreateSolidBrush(current_xpixel);
  HBRUSH oldbrush = (HBRUSH)SelectObject(gc, current_brush);
  if (oldbrush) DeleteObject(oldbrush);

  current_pen = create_pen();
  HPEN oldpen = (HPEN)SelectObject(gc, current_pen);
  if (oldpen) DeleteObject(oldpen);
}

// Used by setcolor_index
static void free_color(Color) {
  // CET - FIXME - Are there colormapped displays on Windows?
  //               Would this only be used for private colormaps?
}

#if USE_COLORMAP

// 'select_palette()' - Make a color palette for 8-bit displays if necessary
// Thanks to Michael Sweet @ Easy Software Products for this

HPALETTE
fl_select_palette(void)
{
  static char beenhere;
  if (!beenhere) {
    beenhere = 1;

    //if (GetDeviceCaps(gc, BITSPIXEL) > 8) return NULL;
    int nColors = GetDeviceCaps(gc, SIZEPALETTE);
    if (nColors <= 0 || nColors > 256) return NULL;
    // this will try to work on < 256 color screens, but will probably
    // come out quite badly.

    // I lamely try to get this variable-sized object allocated on stack:
    unsigned long foo[(sizeof(LOGPALETTE)+256*sizeof(PALETTEENTRY))/sizeof(unsigned long)+1];
    LOGPALETTE *pPal = (LOGPALETTE*)foo;

    pPal->palVersion    = 0x300;
    pPal->palNumEntries = nColors;

    // Build 256 colors from the standard FLTK colormap...

    for (int i = 0; i < nColors; i ++) {
      pPal->palPalEntry[i].peRed   = (cmap[i] >> 24) & 255;
      pPal->palPalEntry[i].peGreen = (cmap[i] >> 16) & 255;
      pPal->palPalEntry[i].peBlue  = (cmap[i] >>  8) & 255;
      pPal->palPalEntry[i].peFlags = 0;
    };

    // Create the palette:
    xpalette = CreatePalette(pPal);
  }
  if (xpalette) {
    SelectPalette(gc, xpalette, FALSE);
    RealizePalette(gc);
  }
  return xpalette;
}

#endif

//
// End of "$Id: fl_color_win32.cxx,v 1.32 2002/12/10 01:46:18 easysw Exp $".
//
