//
// "$Id: fl_color.cxx,v 1.28 2002/06/17 16:03:53 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/Fl_Color.h>
#include <fltk/fl_draw.h>

// The fltk "colormap". In fltk 1.0 this allowed the gui colors to be
// stored in 8-bit locations. In fltk 2.0 this is preserved for back
// compatability, but it also serves the purpose of reducing the set
// of colors asked for on colormapped displays and to keep track of
// the assignments of color on such displays. The contents of this
// table are not to be confused with the X colormap, which I try to
// hide completely.

static unsigned fl_cmap[256] = {
#include "fl_cmap.h" // this is a file produced by "cmap.cxx":
};


void fl_set_color(Fl_Color i, Fl_Color c) {
  if (fl_cmap[i] != c) {
    fl_free_color(i);
    fl_cmap[i] = fl_get_color(c);
  }
}

Fl_Color fl_get_color(Fl_Color i) {
  if (i & 0xFFFFFF00) return i;
  return (Fl_Color)fl_cmap[i];
}

void fl_get_color(Fl_Color i, uchar& r, uchar& g, uchar& b) {
  if (!(i & 0xFFFFFF00)) i = Fl_Color(fl_cmap[i]);
  r = uchar(i>>24);
  g = uchar(i>>16);
  b = uchar(i>>8);
}

Fl_Color fl_nearest_color(Fl_Color i) {
  if (!(i & 0xFFFFFF00)) return i;
  uchar r = i>>24;
  uchar g = i>>16;
  uchar b = i>> 8;
  //if (r == g && r == b) return fl_gray_ramp(r*FL_NUM_GRAY/256);
  return fl_color_cube(r*FL_NUM_RED/256,g*FL_NUM_GREEN/256,b*FL_NUM_BLUE/256);
}

Fl_Color fl_color_average(Fl_Color color1, Fl_Color color2, double weight) {
  Fl_Color rgb1 = fl_get_color(color1);
  Fl_Color rgb2 = fl_get_color(color2);
  if (rgb1 == rgb2) return color1;
  return fl_rgb(
	(uchar)(((uchar)(rgb1>>24))*weight + ((uchar)(rgb2>>24))*(1-weight)),
	(uchar)(((uchar)(rgb1>>16))*weight + ((uchar)(rgb2>>16))*(1-weight)),
	(uchar)(((uchar)(rgb1>>8))*weight + ((uchar)(rgb2>>8))*(1-weight)));
}

Fl_Color fl_inactive(Fl_Color c) {
  return fl_color_average(c, FL_GRAY, 0.30);
}

Fl_Color fl_inactive(Fl_Color c, Fl_Flags f) {
  if (f&FL_INACTIVE) return fl_inactive(c);
  return c;
}

Fl_Color fl_contrast(Fl_Color fg, Fl_Color bg) {
  Fl_Color c1 = fl_get_color(fg);
  Fl_Color c2 = fl_get_color(bg);
  if ((c1^c2)&0x80800000)
    return fg;
  else if (c2&0x80800000)
    return FL_BLACK;
  else
    return FL_WHITE;
}

// Include the code for setting colors on the system and for managing
// system colormaps:

#ifdef _WIN32
# include "fl_color_win32.cxx"
#elif defined(__APPLE__)
# include "fl_color_mac.cxx"
#else
# include "fl_color_x.cxx"
#endif

//
// End of "$Id: fl_color.cxx,v 1.28 2002/06/17 16:03:53 spitzak Exp $".
//
