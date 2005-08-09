//
// "$Id$"
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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <config.h>

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

// callback for monochrome 1 byte per pixel:
static const uchar* mcb1(void*v, int x, int y, int w, uchar* buf) {
  xpm_data& d = *(xpm_data*)v;
  const uchar* p = d.data[y]+x;
  uchar* b = buf;
  for (int X=w; X--;) *b++ = *(uchar*)(d.colors+*p++);//>>24;
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

// callback for monochrome 1 byte per pixel:
static const uchar* mcb1(void*v, int x, int y, int w, uchar* buf) {
  xpm_data& d = *(xpm_data*)v;
  const uchar* p = d.data[y]+x;
  uchar* b = buf;
  for (int X=w; X--;) *b++ = *(uchar*)(d.colors+*p++);//>>24;
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

int fltk::draw_xpm(const char*const* di, int x, int y) {
  xpm_data d;
  if (!measure_xpm(di, d.w, d.h)) return 0;
  const uchar*const* data = (const uchar*const*)(di+1);
  int transparent_index = -1;
  bool monochrome = true;

  if (ncolors < 0) {	// fltk (non standard) compressed colormap
    ncolors = -ncolors;
    const uchar *p = *data++;
    // if first color is ' ' it is transparent (put it later to make
    // it not be transparent):
    if (p[0] == ' ') {
      d.colors[32] = 0;
      transparent_index = ' ';
      ncolors--;
      monochrome = false;
      p += 4;
    }
    // read all the rest of the colors:
    for (int i=0; i < ncolors; i++) {
      uchar* c = (uchar*)&d.colors[p[0]];
#ifdef U64
      *(U64*)c = 0;
#if WORDS_BIGENDIAN
      c += 4;
#endif
#endif
      if (p[0]!=p[1] || p[1]!=p[2]) monochrome = false;
      c[0] = p[1];
      c[1] = p[2];
      c[2] = p[3];
      c[3] = 0xff;
      p += 4;
    }
  } else {	// normal XPM colormap with names
    if (chars_per_pixel>1) {
      memset(d.byte1, 0, sizeof(d.byte1));
      monochrome = false;
    }
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
	if (!*p) break;
	previous_word = p;
	while (*p && !isspace(*p)) p++;
	if (what == 'c') break;
      }
#ifdef U64
      *(U64*)c = 0;
#if WORDS_BIGENDIAN
      c += 4;
#endif
#endif
      Color C;
      if (*p) {
	char buf[30];
	if (p-previous_word > 29) p = previous_word+29;
	memcpy(buf, previous_word, p-previous_word);
	buf[p-previous_word] = 0;
	C = color(buf);
      } else {
	C = color((const char*)previous_word);
      }
      if (C) {
	c[0] = uchar(C>>24);
	c[1] = uchar(C>>16);
	c[2] = uchar(C>>8);
	if (c[0] != c[1] || c[0] != c[2]) monochrome = false;
	c[3] = 0xff;
      } else { // assume "None" or "#transparent" for any errors
	transparent_index = index;
	c[0] = c[1] = c[2] = c[3] = 0;
	monochrome = false;
      }
    }
  }
  d.data = data;
  if (monochrome)
    drawimage(mcb1, &d, MASK, Rectangle(x, y, d.w, d.h));
  else
    drawimage(chars_per_pixel==1 ? cb1 : cb2, &d,
	      transparent_index >= 0 ? RGBA : RGBx,
	      Rectangle(x, y, d.w, d.h));
  if (chars_per_pixel > 1) for (int i = 0; i < 256; i++) delete[] d.byte1[i];
  return 1;
}

//
// End of "$Id$".
//
