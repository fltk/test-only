//
// "$Id: fl_color.cxx,v 1.32 2003/06/24 07:10:48 spitzak Exp $"
//
// Color functions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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

#include <fltk/Color.h>
#include <fltk/draw.h>
#include <config.h>
#ifdef _WIN32 // Crap needed before windows.h so StockBrushes work
# ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0500
# endif
#endif
#include <fltk/x.h>

using namespace fltk;

// The fltk "colormap". In fltk 1.0 this allowed the gui colors to be
// stored in 8-bit locations. In fltk 2.0 this is preserved for back
// compatability, but it also serves the purpose of reducing the set
// of colors asked for on colormapped displays and to keep track of
// the assignments of color on such displays. The contents of this
// table are not to be confused with the X colormap, which I try to
// hide completely.

static unsigned cmap[256] = {
#include "fl_cmap.h" // this is a file produced by "cmap.cxx":
};

void fltk::split_color(Color i, uchar& r, uchar& g, uchar& b) {
  if (!(i & 0xFFFFFF00)) i = Color(cmap[i]);
  r = uchar(i>>24);
  g = uchar(i>>16);
  b = uchar(i>>8);
}

Color fltk::nearest_index(Color i) {
  if (!(i & 0xFFFFFF00)) return i;
  uchar r = i>>24;
  uchar g = i>>16;
  uchar b = i>> 8;
  //if (r == g && r == b) return gray_ramp(r*NUM_GRAY/256);
  return Color(BLACK + (b*5/256 * 5 + r*5/256) * 8 + g*8/256);
}

Color fltk::lerp(Color color0, Color color1, float weight) {
  if (weight <= 0) return color0;
  if (weight >= 1) return color1;
  Color rgb0 = get_color_index(color0);
  Color rgb1 = get_color_index(color1);
  if (rgb0 == rgb1) return color0;
  return color(
	(uchar)(((uchar)(rgb1>>24))*weight + ((uchar)(rgb0>>24))*(1-weight)),
	(uchar)(((uchar)(rgb1>>16))*weight + ((uchar)(rgb0>>16))*(1-weight)),
	(uchar)(((uchar)(rgb1>>8))*weight + ((uchar)(rgb0>>8))*(1-weight)));
}

Color fltk::inactive(Color c) {
  return lerp(c, GRAY75, 0.70f);
}

Color fltk::inactive(Color c, Flags f) {
  if (f&INACTIVE) return lerp(c, GRAY75, 0.70f);
  return c;
}

Color fltk::contrast(Color fg, Color bg) {
  Color c1 = get_color_index(fg);
  Color c2 = get_color_index(bg);
  if ((c1^c2)&0x80800000)
    return fg;
  else if (c2&0x80800000)
    return BLACK;
  else
    return WHITE;
}

// Include the code for setting colors on the system and for managing
// system colormaps:

#ifdef _WIN32
# include "fl_color_win32.cxx"
#elif (defined(__APPLE__) && !USE_X11)
# include "fl_color_mac.cxx"
#else
# include "fl_color_x.cxx"
#endif

void fltk::set_color_index(Color i, Color c) {
  if (cmap[i] != c) {
    free_color(i);
    cmap[i] = c;
  }
}

Color fltk::get_color_index(Color i) {
  if (i & 0xFFFFFF00) return i;
  return (Color)cmap[i];
}

//
// End of "$Id: fl_color.cxx,v 1.32 2003/06/24 07:10:48 spitzak Exp $".
//
