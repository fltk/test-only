//
// "$Id: fl_color.cxx,v 1.24 2000/08/12 07:44:29 spitzak Exp $"
//
// Color functions for the Fast Light Tool Kit (FLTK).
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

// Implementation of fl_color(i)

#ifdef WIN32
#include "fl_color_win32.cxx"
#else

// Also code to look at the X visual and figure out the best way to turn
// a color into a pixel value.

#include <FL/Fl_Color.H>
#include <FL/x.H>
#include <FL/fl_draw.H>
#include "Fl_XColor.H"

////////////////////////////////////////////////////////////////
// figure_out_visual() calculates masks & shifts for generating
// pixels in true-color visuals:

int fl_redbits, fl_greenbits, fl_bluebits;
int fl_redshift, fl_greenshift, fl_blueshift, fl_extrashift;
uchar fl_redmask, fl_greenmask, fl_bluemask;
static uchar beenhere;

static void figure_out_visual() {
  beenhere = 1;
  if (!fl_visual->red_mask || !fl_visual->green_mask || !fl_visual->blue_mask){
#if USE_COLORMAP
    fl_redmask = 0;
    // make sure black & white are allocated:
    fl_xpixel(FL_WHITE);
    fl_xpixel(FL_BLACK);
    return;
#else
    Fl::fatal("Requires TrueColor visual");
#endif
  }

  // get the bit masks into a more useful form:
  int i,j,m;

  for (i = 0, m = 1; m; i++, m<<=1) if (fl_visual->red_mask & m) break;
  for (j = i; m; j++, m<<=1) if (!(fl_visual->red_mask & m)) break;
  fl_redshift = j-8;
  fl_redbits = j-i; if (fl_redbits > 8) fl_redbits = 8;
  fl_redmask = 0xff << (8-fl_redbits);

  for (i = 0, m = 1; m; i++, m<<=1) if (fl_visual->green_mask & m) break;
  for (j = i; m; j++, m<<=1) if (!(fl_visual->green_mask & m)) break;
  fl_greenshift = j-8;
  fl_greenbits = j-i; if (fl_greenbits > 8) fl_greenbits = 8;
  fl_greenmask = 0xff << (8-fl_greenbits);

  for (i = 0, m = 1; m; i++, m<<=1) if (fl_visual->blue_mask & m) break;
  for (j = i; m; j++, m<<=1) if (!(fl_visual->blue_mask & m)) break;
  fl_blueshift = j-8;
  fl_bluebits = j-i; if (fl_bluebits > 8) fl_bluebits = 8;
  fl_bluemask = 0xff << (8-fl_bluebits);

  i = fl_redshift;
  if (fl_greenshift < i) i = fl_greenshift;
  if (fl_blueshift < i) i = fl_blueshift;
  if (i < 0) {
    fl_extrashift = -i;
    fl_redshift -= i; fl_greenshift -= i; fl_blueshift -= i;
  } else {
    fl_extrashift = 0;
  }
}

unsigned fl_cmap[256] = {
#include "fl_cmap.h" // this is a file produced by "cmap.cxx":
};

////////////////////////////////////////////////////////////////
// Return an X pixel value for an fltk color.  For true color this
// is easy.  Most of this code is to handle colormaps, which are
// also assummed to be 8 bits in size or less.  The requested
// color is rounded to the nearest color in fltk's "colormap"
// and then requested from X.  If this fails the current X colormap
// is searched for the nearest match.

#if USE_COLORMAP
#if HAVE_OVERLAY
Fl_XColor fl_xmap[2][256];
uchar fl_overlay;
Colormap fl_overlay_colormap;
XVisualInfo* fl_overlay_visual;
ulong fl_transparent_pixel;
#else
Fl_XColor fl_xmap[1][256];
#endif
#endif

ulong fl_xpixel(Fl_Color i) {
  if (!beenhere) figure_out_visual();

#if USE_COLORMAP
#if HAVE_OVERLAY
  if (fl_redmask && !fl_overlay)
#else
  if (fl_redmask)
#endif
    {
#endif
    // return color for a truecolor visual:
    if (!(i & 0xFFFFFF00)) i = (Fl_Color)fl_cmap[i];
    return
      ((((i>>24)& fl_redmask)  << fl_redshift)+
       (((i>>16)& fl_greenmask)<< fl_greenshift)+
       (((i>>8) & fl_bluemask) << fl_blueshift)
       ) >> fl_extrashift;
#if USE_COLORMAP
    }

  // rest of this is for a colormapped visual (it will work for a true
  // color one but wastes time):

  // translate rgb colors into color index:
  int index = fl_nearest_color(i);

  // see if we have already allocated it:
#if HAVE_OVERLAY
  Fl_XColor &xmap = fl_xmap[fl_overlay][index];
#else
  Fl_XColor &xmap = fl_xmap[0][index];
#endif
  if (!xmap.mapped) {
    // figure out the rgb to ask for.  Use the specified one unless this
    // is an indexed color, in which case we use the colormap entry:
    if (!(i & 0xFFFFFF00)) i = (Fl_Color)fl_cmap[i];
    uchar r = i>>24;
    uchar g = i>>16;
    uchar b = i>> 8;
    fl_allocate_xpixel(xmap, r, g, b);
  }
  return xmap.pixel;
}

// Create an X colormap entry and place it in the given xmap entry:
void fl_allocate_xpixel(Fl_XColor& xmap, uchar r, uchar g, uchar b)
{
#if HAVE_OVERLAY
  Colormap colormap = fl_overlay ? fl_overlay_colormap : fl_colormap;
  static XColor* ac[2];
  XColor*& allcolors = ac[fl_overlay];
  static int nc[2];
  int& numcolors = nc[fl_overlay];
#else
  Colormap colormap = fl_colormap;
  static XColor *allcolors;
  static int numcolors;
#endif

  // I don't try to allocate colors with XAllocColor after the first
  // failure.  It is possible that it will work, since a color
  // may have been freed, but some servers are extremely slow and this
  // avoids a round trip for every color after that:
  if (!numcolors) { // if we have not failed yet:

    // Try XAllocColor:
    XColor xcol;
    xcol.red = r<<8; xcol.green = g<<8; xcol.blue = b<<8;
    if (XAllocColor(fl_display, colormap, &xcol)) {
      xmap.mapped = 1;
      xmap.r = xcol.red>>8;
      xmap.g = xcol.green>>8;
      xmap.b = xcol.blue>>8;
      xmap.pixel = xcol.pixel;
      return;
    }

    // Failed, read the colormap so we can search it:
    // I only read the colormap once.  Again this is due to the slowness
    // of round-trips to the X server, even though other programs may alter
    // the colormap after this and make decisions here wrong:
#if HAVE_OVERLAY
    if (fl_overlay) numcolors = fl_overlay_visual->colormap_size; else
#endif
      numcolors = fl_visual->colormap_size;
    if (!allcolors) allcolors = new XColor[numcolors];
    for (int p = numcolors; p--;) allcolors[p].pixel = p;
    XQueryColors(fl_display, colormap, allcolors, numcolors);
  }

  // Find least-squares match in the colormap:
  int mindist = 0x7FFFFFFF;
  unsigned int bestmatch = 0;
  for (unsigned int n = numcolors; n--;) {
#if HAVE_OVERLAY
    if (fl_overlay && n == fl_transparent_pixel) continue;
#endif
    XColor &a = allcolors[n];
    int d, t;
    t = int(r)-int(a.red>>8); d = t*t;
    t = int(g)-int(a.green>>8); d += t*t;
    t = int(b)-int(a.blue>>8); d += t*t;
    if (d <= mindist) {bestmatch = n; mindist = d;}
  }
  XColor &p = allcolors[bestmatch];

  // It appears to "work" to not call this XAllocColor, which will
  // avoid another round-trip to the server.  But then X does not
  // know that this program "owns" this value, and can (and will)
  // change it when the program that did allocate it exits:
  if (XAllocColor(fl_display, colormap, &p)) {
    xmap.mapped = 1;
    xmap.pixel = p.pixel;
  } else {
    // However, if that XAllocColor fails, I have to give up and
    // assumme the pixel is ok for the duration of the program.  This
    // is due to bugs (?) in the Solaris X and some X terminals
    // where XAllocColor *always* fails when the colormap is full,
    // even if we ask for a color already in it...
    xmap.mapped = 2; // 2 prevents XFreeColor from being called
    xmap.pixel = bestmatch;
  }
  xmap.r = p.red>>8;
  xmap.g = p.green>>8;
  xmap.b = p.blue>>8;
#endif
}

Fl_Color fl_color_;

void fl_color(Fl_Color i) {
  fl_color_ = i;
  XSetForeground(fl_display, fl_gc, fl_xpixel(i));
}

void fl_free_color(Fl_Color i) {
#if USE_COLORMAP
  if (fl_xmap[0][i].mapped) {
    if (fl_xmap[0][i].mapped == 1)
      XFreeColors(fl_display, fl_colormap, &(fl_xmap[0][i].pixel), 1, 0);
    fl_xmap[0][i].mapped = 0;
  }
#if HAVE_OVERLAY
  if (fl_xmap[1][i].mapped) {
    if (fl_xmap[1][i].mapped == 1)
      XFreeColors(fl_display, fl_overlay_colormap, &(fl_xmap[1][i].pixel),1,0);
    fl_xmap[overlay][i].mapped = 0;
  }
#endif
#endif
}

#endif // end of X-specific code

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

//
// End of "$Id: fl_color.cxx,v 1.24 2000/08/12 07:44:29 spitzak Exp $".
//
