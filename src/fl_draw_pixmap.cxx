//
// "$Id: fl_draw_pixmap.cxx,v 1.23 2004/09/05 21:40:41 spitzak Exp $"
//
// Pixmap drawing code for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// Implemented without using the xpm library (which I can't use because
// it interferes with the color cube used by drawimage).
// Current implementation is cheap and slow, and works best on a full-color
// display.  Transparency is not handled, and colors are dithered to
// the color cube.  Color index is achieved by adding the id
// characters together!  Also mallocs a lot of temporary memory!
// Notice that there is no pixmap file interface.  This is on purpose,
// as I want to discourage programs that require support files to work.
// All data needed by a program ui should be compiled in!!!

#include <fltk/Color.h>
#include <fltk/draw.h>
#include <config.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
using namespace fltk;

static int ncolors, chars_per_pixel;

int fltk::measure_xpm(const char*const* data, int &w, int &h) {
  int i = sscanf(data[0],"%d %d %d %d",&w,&h,&ncolors,&chars_per_pixel);
  if (i<4 || w<=0 || h<=0 ||
      chars_per_pixel!=1 && chars_per_pixel!=2) return w=0;
  return 1;
}

#ifdef U64

// The callback from drawimage to get a row of data passes this:
struct xpm_data {
  int w, h;
  const uchar*const* data;
  union {
    U64 colors[256];
    U64* byte1[256];
  };
};

// callback for 1 byte per pixel:
static const uchar* cb1(void*v, int x, int y, int w, uchar* buf) {
  xpm_data& d = *(xpm_data*)v;
  const uchar* p = d.data[y]+x;
  U64* q = (U64*)buf;
  for (int X=(w+1)/2; X--; p += 2) {
#if WORDS_BIGENDIAN
    *q++ = (d.colors[p[0]]<<32) | d.colors[p[1]];
#else
    *q++ = (d.colors[p[1]]<<32) | d.colors[p[0]];
#endif
  }
  return buf;
}

// callback for 2 bytes per pixel:
static const uchar* cb2(void*v, int x, int y, int w, uchar* buf) {
  xpm_data& d = *(xpm_data*)v;
  const uchar* p = d.data[y]+2*x;
  U64* q = (U64*)buf;
  for (int X=(w+1)/2; X--;) {
    U64* colors = d.byte1[*p++];
    int index = *p++;
    U64* colors1 = d.byte1[*p++];
    int index1 = *p++;
#if WORDS_BIGENDIAN
    *q++ = (colors[index]<<32) | colors1[index1];
#else
    *q++ = (colors1[index1]<<32) | colors[index];
#endif
  }
  return buf;
}

#else

// The callback from drawimage to get a row of data passes this:
struct xpm_data {
  int w, h;
  const uchar*const* data;
  union {
    U32 colors[256];
    U32* byte1[256];
  };
};

// callback for 1 byte per pixel:
static const uchar* cb1(void*v, int x, int y, int w, uchar* buf) {
  xpm_data& d = *(xpm_data*)v;
  const uchar* p = d.data[y]+x;
  U32* q = (U32*)buf;
  for (int X=w; X--;) *q++ = d.colors[*p++];
  return buf;
}

// callback for 2 bytes per pixel:
static const uchar* cb2(void*v, int x, int y, int w, uchar* buf) {
  xpm_data& d = *(xpm_data*)v;
  const uchar* p = d.data[y]+2*x;
  U32* q = (U32*)buf;
  for (int X=w; X--;) {
    U32* colors = d.byte1[*p++];
    *q++ = colors[*p++];
  }
  return buf;
}

#endif

static uchar **mask_bitmap;

void fltk::set_mask_bitmap(uchar **ppBitmap)
{
	mask_bitmap = ppBitmap;
}

Color fg_kludge;

int fltk::draw_xpm(const char*const* di, int x, int y, Color bg) {
  xpm_data d;
  if (!measure_xpm(di, d.w, d.h)) return 0;
  const uchar*const* data = (const uchar*const*)(di+1);
  int transparent_index = -1;

  if (ncolors < 0) {	// fltk (non standard) compressed colormap
    ncolors = -ncolors;
    const uchar *p = *data++;
    // if first color is ' ' it is transparent (put it later to make
    // it not be transparent):
    if (*p == ' ') {
      uchar* c = (uchar*)&d.colors[32];
#ifdef U64
      *(U64*)c = 0;
#if WORDS_BIGENDIAN
      c += 4;
#endif
#endif
      transparent_index = ' ';
      split_color(bg, c[0], c[1], c[2]); c[3] = 0;
      p += 4;
      ncolors--;
    }
    // read all the rest of the colors:
    for (int i=0; i < ncolors; i++) {
      uchar* c = (uchar*)&d.colors[*p++];
#ifdef U64
      *(U64*)c = 0;
#if WORDS_BIGENDIAN
      c += 4;
#endif
#endif
      *c++ = *p++;
      *c++ = *p++;
      *c++ = *p++;
      *c = 0xff;
    }
  } else {	// normal XPM colormap with names
    uchar f[3];
    uchar b[3];
    if (fg_kludge) {
      split_color(fg_kludge, f[0], f[1], f[2]);
      split_color(bg, b[0], b[1], b[2]);
    }
    if (chars_per_pixel>1) memset(d.byte1, 0, sizeof(d.byte1));
    for (int i=0; i<ncolors; i++) {
      const uchar* p = *data++;
      // the first 1 or 2 characters are the color index:
      int index = *p++;
      uchar* c;
      if (chars_per_pixel>1) {
#ifdef U64
	U64* colors = d.byte1[index];
	if (!colors) colors = d.byte1[index] = new U64[256];
#else
	U32* colors = d.byte1[index];
	if (!colors) colors = d.byte1[index] = new U32[256];
#endif
	c = (uchar*)&colors[*p];
	index = (index<<8)+*p++;
      } else {
	c = (uchar *)&d.colors[index];
      }
      // look for "c word", or last word if none:
      const uchar *previous_word = p;
      for (;;) {
	while (*p && isspace(*p)) p++; uchar what = *p++;
	while (*p && !isspace(*p)) p++;
	while (*p && isspace(*p)) p++;
	if (!*p) {p = previous_word; break;}
	if (what == 'c') break;
	previous_word = p;
	while (*p && !isspace(*p)) p++;
      }
#ifdef U64
      *(U64*)c = 0;
#if WORDS_BIGENDIAN
      c += 4;
#endif
#endif
      Color C = color((const char*)p);
      if (C) {
	c[0] = uchar(C>>24);
	c[1] = uchar(C>>16);
	c[2] = uchar(C>>8);
	if (fg_kludge) {
	  c[0] = (c[0]*b[0]+(255-c[0])*f[0])/255;
	  c[1] = (c[1]*b[1]+(255-c[1])*f[1])/255;
	  c[2] = (c[2]*b[2]+(255-c[2])*f[2])/255;
	}
	c[3] = 0xff;
      } else { // assume "None" or "#transparent" for any errors
	// this should be transparent...
	transparent_index = index;
	c[0] = c[1] = c[2] = c[3] = 0;
      }
    }
  }
  d.data = data;

  // build the mask bitmap used by xpmImage:
  if (mask_bitmap && transparent_index >= 0) {
    // search for usage of the transparent index, if none we act like
    // the image is opaque (which avoids some buggy code on X/Win32):
    int y;
    for (y = 0; y < d.h; y++) {
      const uchar* p = data[y];
      if (chars_per_pixel <= 1) {
	for (int x = 0; x < d.w; x++)
	  if (*p++ == transparent_index) goto GOTIT;
      } else {
	for (int x = 0; x < d.w; x++) {
	  int index = *p++;
	  index = (index<<8) | (*p++);
	  if (index == transparent_index) goto GOTIT;
	}
      }
    }
    goto NO_MASK;
  GOTIT:
    int W = (d.w+7)/8;
    uchar* bitmap = new uchar[W * d.h];
    *mask_bitmap = bitmap;
    for (y = 0; y < d.h; y++) {
      const uchar* p = data[y];
      if (chars_per_pixel <= 1) {
	for (int x = 0; x < W; x++) {
	  int b = (*p++ != transparent_index);
	  if (*p++ != transparent_index) b |= 2;
	  if (*p++ != transparent_index) b |= 4;
	  if (*p++ != transparent_index) b |= 8;
	  if (*p++ != transparent_index) b |= 16;
	  if (*p++ != transparent_index) b |= 32;
	  if (*p++ != transparent_index) b |= 64;
	  if (*p++ != transparent_index) b |= 128;
	  *bitmap++ = b;
	}
      } else {
	for (int x = 0; x < W; x++) {
	  int b = 0;
	  for (int i = 0; i < 8; i++) {
	    int index = *p++;
	    index = (index<<8) | (*p++);
	    if (index != transparent_index) b |= (1<<i);
	  }
	  *bitmap++ = b;
	}
      }
    }
  }
 NO_MASK:

  drawimage(chars_per_pixel==1 ? cb1 : cb2, &d, RGB, x, y, d.w, d.h, 4);
  if (chars_per_pixel > 1) for (int i = 0; i < 256; i++) delete[] d.byte1[i];
  return 1;
}

//
// End of "$Id: fl_draw_pixmap.cxx,v 1.23 2004/09/05 21:40:41 spitzak Exp $".
//
