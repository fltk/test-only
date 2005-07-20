// "$Id$"
//
// Image drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

// This file is not independently compiled, it is included by drawimage.cxx

// Only a limited set of possible X servers is supported, as I have
// not encountered other versions, and the old code that supported more
// was unnecessarily complex:
//
// Anything with a 8 bits_per_pixel bitmaps (visual depth may be less)
// 16-bit TrueColor with arbitrary layout
// 32-bit TrueColor with ARGB32 layout

// STD C
#include <stdlib.h>
#include <stdio.h>
// FLTK
#include <fltk/error.h>
#include <fltk/Image.h>
#include <fltk/math.h>
#include "XColorMap.h"

using namespace fltk;

/// Converter functions:
static void (*converter[7])(const uchar *from, uchar *to, int w);

static int dir;		// direction-alternator
static int ri,gi,bi;	// saved error-diffusion value

#if USE_COLORMAP
////////////////////////////////////////////////////////////////
// 8-bit converter with error diffusion

static void mono_to_8(const uchar *from, uchar *to, int w) {
  int r=ri, g=gi, b=bi;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1);
    to = to+(w-1);
    d = -1;
    td = -1;
  } else {
    dir = 1;
    d = 1;
    td = 1;
  }
  for (;; from += d, to += td) {
    r += from[0]; if (r < 0) r = 0; else if (r>255) r = 255;
    g += from[0]; if (g < 0) g = 0; else if (g>255) g = 255;
    b += from[0]; if (b < 0) b = 0; else if (b>255) b = 255;
    uchar i = (uchar)(BLACK + (b*5/256 * 5 + r*5/256) * 8 + g*8/256);
    XColorMap& xmap = fl_xmap(i,(uchar)r,(uchar)g,(uchar)b);
    r -= xmap.r;
    g -= xmap.g;
    b -= xmap.b;
    *to = uchar(xmap.pixel);
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
}

static void rgb_to_8d(const uchar *from, uchar *to, int w, int delta) {
  int r=ri, g=gi, b=bi;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1)*delta;
    to = to+(w-1);
    d = -delta;
    td = -1;
  } else {
    dir = 1;
    d = delta;
    td = 1;
  }
  for (;; from += d, to += td) {
    r += from[0]; if (r < 0) r = 0; else if (r>255) r = 255;
    g += from[1]; if (g < 0) g = 0; else if (g>255) g = 255;
    b += from[2]; if (b < 0) b = 0; else if (b>255) b = 255;
    uchar i = (uchar)(BLACK + (b*5/256 * 5 + r*5/256) * 8 + g*8/256);
    XColorMap& xmap = fl_xmap(i,(uchar)r,(uchar)g,(uchar)b);
    r -= xmap.r;
    g -= xmap.g;
    b -= xmap.b;
    *to = uchar(xmap.pixel);
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
}

static void rgb_to_8(const uchar *from, uchar *to, int w) {
  rgb_to_8d(from, to, w, 3);
}

static void rgba_to_8(const uchar *from, uchar *to, int w) {
  rgb_to_8d(from, to, w, 4);
}

static void argb32_to_8(const uchar *from, uchar *to, int w) {
#if WORDS_BIGENDIAN
  rgb_to_8d(from+1, to, w, 4);
#else
  int r=ri, g=gi, b=bi;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1)*4;
    to = to+(w-1);
    d = -4;
    td = -1;
  } else {
    dir = 1;
    d = 4;
    td = 1;
  }
  for (;; from += d, to += td) {
    r += from[2]; if (r < 0) r = 0; else if (r>255) r = 255;
    g += from[1]; if (g < 0) g = 0; else if (g>255) g = 255;
    b += from[0]; if (b < 0) b = 0; else if (b>255) b = 255;
    uchar i = (uchar)(BLACK + (b*5/256 * 5 + r*5/256) * 8 + g*8/256);
    XColorMap& xmap = fl_xmap(i,(uchar)r,(uchar)g,(uchar)b);
    r -= xmap.r;
    g -= xmap.g;
    b -= xmap.b;
    *to = uchar(xmap.pixel);
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
#endif
}

#endif

////////////////////////////////////////////////////////////////
// 16 bit TrueColor converters with error diffusion
// Cray computers have no 16-bit type, so we use character pointers
// (which may be slow)

#ifdef U16
#define OUTTYPE U16
#define OUTSIZE 1
#define OUTASSIGN(v) *t = v
#else
#define OUTTYPE uchar
#define OUTSIZE 2
#define OUTASSIGN(v) int tt=v; t[0] = uchar(tt>>8); t[1] = uchar(tt)
#endif

static void mono_to_16(const uchar *from,uchar *to,int w) {
  OUTTYPE *t = (OUTTYPE *)to;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1);
    t = t+(w-1)*OUTSIZE;
    d = -1;
    td = -OUTSIZE;
  } else {
    dir = 1;
    d = 1;
    td = OUTSIZE;
  }
  uchar mask = fl_redmask & fl_greenmask & fl_bluemask;
  int r=ri;
  for (;; from += d, t += td) {
    r = (r&~mask) + *from; if (r > 255) r = 255;
    uchar m = r&mask;
    OUTASSIGN((
      (m<<fl_redshift)+
      (m<<fl_greenshift)+
      (m<<fl_blueshift)
      ) >> fl_extrashift);
    if (!--w) break;
  }
  ri = r;
}

static void rgb_to_16d(const uchar *from, uchar *to, int w, int delta) {
  OUTTYPE *t = (OUTTYPE *)to;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1)*delta;
    t = t+(w-1)*OUTSIZE;
    d = -delta;
    td = -OUTSIZE;
  } else {
    dir = 1;
    d = delta;
    td = OUTSIZE;
  }
  int r=ri, g=gi, b=bi;
  for (;; from += d, t += td) {
    r = (r&~fl_redmask)  +from[0]; if (r>255) r = 255;
    g = (g&~fl_greenmask)+from[1]; if (g>255) g = 255;
    b = (b&~fl_bluemask) +from[2]; if (b>255) b = 255;
    OUTASSIGN((
      ((r&fl_redmask)<<fl_redshift)+
      ((g&fl_greenmask)<<fl_greenshift)+
      ((b&fl_bluemask)<<fl_blueshift)
      ) >> fl_extrashift);
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
}

static void rgb_to_16(const uchar *from, uchar *to, int w) {
  rgb_to_16d(from, to, w, 3);
}

static void rgba_to_16(const uchar *from, uchar *to, int w) {
  rgb_to_16d(from, to, w, 4);
}

static void argb32_to_16(const uchar *from, uchar *to, int w) {
#if WORDS_BIGENDIAN
  rgb_to_16d(from+1, to, w, 4);
#else
  OUTTYPE *t = (OUTTYPE *)to;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1)*4;
    t = t+(w-1)*OUTSIZE;
    d = -4;
    td = -OUTSIZE;
  } else {
    dir = 1;
    d = 4;
    td = OUTSIZE;
  }
  int r=ri, g=gi, b=bi;
  for (;; from += d, t += td) {
    r = (r&~fl_redmask)  +from[2]; if (r>255) r = 255;
    g = (g&~fl_greenmask)+from[1]; if (g>255) g = 255;
    b = (b&~fl_bluemask) +from[0]; if (b>255) b = 255;
    OUTASSIGN((
      ((r&fl_redmask)<<fl_redshift)+
      ((g&fl_greenmask)<<fl_greenshift)+
      ((b&fl_bluemask)<<fl_blueshift)
      ) >> fl_extrashift);
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
#endif
}

// special-case the 5r6g5b layout used by XFree86:

static void mono_to_565(const uchar *from,uchar *to,int w) {
  OUTTYPE *t = (OUTTYPE *)to;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1);
    t = t+(w-1)*OUTSIZE;
    d = -1;
    td = -OUTSIZE;
  } else {
    dir = 1;
    d = 1;
    td = OUTSIZE;
  }
  int r=ri;
  for (;; from += d, t += td) {
    r = (r&7) + *from; if (r > 255) r = 255;
    OUTASSIGN((r>>3) * 0x841);
    if (!--w) break;
  }
  ri = r;
}

static void rgb_to_565d(const uchar *from, uchar *to, int w, int delta) {
  OUTTYPE *t = (OUTTYPE *)to;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1)*delta;
    t = t+(w-1)*OUTSIZE;
    d = -delta;
    td = -OUTSIZE;
  } else {
    dir = 1;
    d = delta;
    td = OUTSIZE;
  }
  int r=ri, g=gi, b=bi;
  for (;; from += d, t += td) {
    r = (r&7)+from[0]; if (r>255) r = 255;
    g = (g&3)+from[1]; if (g>255) g = 255;
    b = (b&7)+from[2]; if (b>255) b = 255;
    OUTASSIGN(((r&0xf8)<<8) + ((g&0xfc)<<3) + (b>>3));
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
}

static void rgb_to_565(const uchar *from, uchar *to, int w) {
  rgb_to_565d(from, to, w, 3);
}

static void rgba_to_565(const uchar *from, uchar *to, int w) {
  rgb_to_565d(from, to, w, 4);
}

static void argb32_to_565(const uchar *from, uchar *to, int w) {
#if WORDS_BIGENDIAN
  rgb_to_565d(from+1, to, w, 4);
#else
  OUTTYPE *t = (OUTTYPE *)to;
  int d, td;
  if (dir) {
    dir = 0;
    from = from+(w-1)*4;
    t = t+(w-1)*OUTSIZE;
    d = -4;
    td = -OUTSIZE;
  } else {
    dir = 1;
    d = 4;
    td = OUTSIZE;
  }
  int r=ri, g=gi, b=bi;
  for (;; from += d, t += td) {
    r = (r&7)+from[2]; if (r>255) r = 255;
    g = (g&3)+from[1]; if (g>255) g = 255;
    b = (b&7)+from[0]; if (b>255) b = 255;
    OUTASSIGN(((r&0xf8)<<8) + ((g&0xfc)<<3) + (b>>3));
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
#endif
}

////////////////////////////////////////////////////////////////
// 32bit TrueColor converters on a 32 or 64-bit machine:
// I have only seen rgbx32 and xrgb32 orders, or the byte-swapped
// version of them, which is done by flipping the byte order in Image

static void mono_to_32(const uchar *from, uchar *to, int w) {
  U32* t = (U32*)to;
  for (;;) {
    *t++ = *from++ * 0x1010101U;
    if (!--w) break;
  }
}

static void rgb_to_rgbx(const uchar *from, uchar *to, int w) {
  U32* t = (U32*)to;
  for (;; from += 3) {
    *t++ = (from[0]<<24)+(from[1]<<16)+(from[2]<<8)+(from[3]);
    if (!--w) break;
  }
}

#if !WORDS_BIGENDIAN
static void rgba_to_rgbx(const uchar *from, uchar *to, int w) {
  U32* t = (U32*)to;
  for (;; from += 4) {
    *t++ = (from[0]<<24)+(from[1]<<16)+(from[2]<<8)+(from[3]);
    if (!--w) break;
  }
}
#endif

static void argb32_to_rgbx(const uchar *from, uchar *to, int w) {
  U32* t = (U32*)to;
  const U32* f = (U32*)from;
  for (;;) {
    unsigned x = *f++;
    *t++ = (x<<8)|((x>>24)&255);
    if (!--w) break;
  }
}

////////////////////////////////////////////////////////////////

static void rgb_to_xrgb(const uchar *from, uchar *to, int w) {
  U32* t = (U32*)to;
  for (;; from += 3) {
    *t++ = (from[0]<<16)+(from[1]<<8)+(from[2]);
    if (!--w) break;
  }
}

static void rgba_to_xrgb(const uchar *from, uchar *to, int w) {
  U32* t = (U32*)to;
  for (;; from += 4) {
    *t++ = (from[0]<<16)+(from[1]<<8)+(from[2])+(from[3]<<24);
    if (!--w) break;
  }
}

static void direct_32(const uchar *from, uchar *to, int w) {
  memcpy(to, from, w*4);
}

////////////////////////////////////////////////////////////////
// alpha pre-processing:
// These assumme that argb32 is the best intermediate form for image.

// the current colors:
static uchar fg[3];
static uchar bg[3];

#if USE_XFT

#if 0
// Mask with XRender can be done by multiplying fg color by alpha:
static void mask_to_argb32(const uchar* from, uchar* to, int w)
{
  U32* bp = (U32*)to;
  for (int i = 0; i < w; i++) {
    uchar c = 255-*from++;
    *bp++ =
      (c<<24) |
      (((fg[0]*c)<<8)&0xff0000u) |
      ((fg[1]*c)&0xff00u) |
      ((fg[2]*c)>>8);
  }
}
#endif

static void mask_to_32(const uchar *from, uchar *to, int w) {
  U32* t = (U32*)to;
  for (;;) {
    *t++ = ~(*from++ * 0x1010101U);
    if (!--w) break;
  }
}
#endif

// For old Xlib, we build a binary alpha mask, opaque for any non-zero
// alpha, and guess a mixture with the current getbgcolor() is acceptable:

static uchar* alphabuffer;
static int alphabuffersize;
static uchar* alphapointer;
static int alpha_increment;

// true for any transparency other than 255 and 0:
static bool mixbg;

static void mask_converter(const uchar* from, uchar* to, int w)
{
  U32 buffer[w];
  U32* bp = buffer;
  uchar* ap = alphapointer;
  uchar aaccum = 0;
  uchar amask = 1;
  for (int i = 0; i < w; i++) {
    uchar c = *from++;
    if (c==255) {
      *bp++ = 0;
    } else {
      aaccum |= amask;
      if (c) {
	uchar r = (c*bg[0]+(255-c)*fg[0])>>8;
	uchar g = (c*bg[1]+(255-c)*fg[1])>>8;
	uchar b = (c*bg[2]+(255-c)*fg[2])>>8;
	*bp++ = (r<<16)|(g<<8)|b;
	mixbg = true;
      } else {
	*bp++ = (fg[0]<<16)|(fg[1]<<8)|fg[2];
      }
    }
    if (amask == 0x80) {
      *ap++ = aaccum; aaccum = 0; amask = 1;
    } else {
      amask <<= 1;
    }
  }
  *ap = aaccum;
  alphapointer += alpha_increment;
  converter[RGB32]((const uchar*)buffer, to, w);
}

static void rgba_converter(const uchar* from, uchar* to, int w) {
  U32 buffer[w];
  U32* bp = buffer;
  uchar* ap = alphapointer;
  uchar aaccum = 0;
  uchar amask = 1;
  for (int i = 0; i < w; i++) {
    uchar r = *from++;
    uchar g = *from++;
    uchar b = *from++;
    uchar a = *from++;
    if (!a) {
      *bp++ = 0;
    } else {
      aaccum |= amask;
      if (a != 255) {
	r = r+((bg[0]*(255-a))>>8);
	g = g+((bg[1]*(255-a))>>8);
	b = b+((bg[2]*(255-a))>>8);
	mixbg = true;
      }
      *bp++ = (r<<16)|(g<<8)|b;
    }
    if (amask == 0x80) {
      *ap++ = aaccum; aaccum = 0; amask = 1;
    } else {
      amask <<= 1;
    }
  }
  *ap = aaccum;
  alphapointer += alpha_increment;
  converter[RGB32]((const uchar*)buffer, to, w);
}

static void argb32_converter(const uchar* from, uchar* to, int w) {
  U32 buffer[w];
  U32* bp = buffer;
  const U32* f = (const U32*)from;
  uchar* ap = alphapointer;
  uchar aaccum = 0;
  uchar amask = 1;
  for (int i = 0; i < w; i++) {
    U32 c = *f++;
    uchar a = c>>24;
    if (!a) {
      *bp++ = 0;
    } else {
      aaccum |= amask;
      if (a == 255) {
	*bp++ = c;
      } else {
	uchar r = (c>>16)+((bg[0]*(255-a))>>8);
	uchar g = (c>>8)+((bg[1]*(255-a))>>8);
	uchar b = c+((bg[2]*(255-a))>>8);
	*bp++ = (r<<16)|(g<<8)|b;
	mixbg = true;
      }
    }
    if (amask == 0x80) {
      *ap++ = aaccum; aaccum = 0; amask = 1;
    } else {
      amask <<= 1;
    }
  }
  *ap = aaccum;
  alphapointer += alpha_increment;
  converter[RGB32]((const uchar*)buffer, to, w);
}

////////////////////////////////////////////////////////////////

static XImage i;	// this is reused to draw an images
static int bytes_per_pixel;
static int scanline_add;
static int scanline_mask;

#if USE_XFT
extern XRenderPictFormat* fl_rgba_xrender_format;
extern bool fl_get_invert_matrix(XTransform&);
extern bool fl_trivial_transform();

Picture p;
XWindow prevsource;

void fl_xrender_draw_image(XWindow source, fltk::PixelType type,
			   const fltk::Rectangle& from,
			   const fltk::Rectangle& to)
{
  if (source != prevsource) {
    prevsource = source;
    if (p) XRenderFreePicture(xdisplay, p);
    p = XRenderCreatePicture(xdisplay, source, fl_rgba_xrender_format, 0, 0);
    XRenderSetPictureFilter(xdisplay, p, "best", 0, 0);
  }
  XTransform xtransform;
  const bool scaling = fl_get_invert_matrix(xtransform);
  int x,y,r,b; // box to draw
  if (scaling) {
    float X,Y,R,B,tx,ty;
    tx = to.x(); ty = to.y(); transform(tx, ty);
    X = R = tx; Y = B = ty;
    tx = to.r(); ty = to.b(); transform(tx, ty);
    if (tx < X) X = tx; else R = tx;
    if (ty < Y) Y = ty; else B = ty;
    if (xtransform.matrix[0][1]||xtransform.matrix[1][0]) {
      tx = to.x(); ty = to.b(); transform(tx, ty);
      if (tx < X) X = tx; else if (tx > R) R = tx;
      if (ty < Y) Y = ty; else if (ty > B) B = ty;
      tx = to.r(); ty = to.y(); transform(tx, ty);
      if (tx < X) X = tx; else if (tx > R) R = tx;
      if (ty < Y) Y = ty; else if (ty > B) B = ty;
    }
    x = int(floorf(X));
    y = int(floorf(Y));
    r = int(ceilf(R));
    b = int(ceilf(B));
  } else {
    x = to.x(); y = to.y(); transform(x,y);
    r = x+to.w(); b = y+to.h();
  }
  if (to.w() != from.w() || to.h() != from.h()) {
    const float scalex = float(from.w())/to.w();
    const float scaley = float(from.h())/to.h();
    for (int i=0;i<3;i++) {
      xtransform.matrix[0][i] = int(scalex*xtransform.matrix[0][i]);
      xtransform.matrix[1][i] = int(scaley*xtransform.matrix[1][i]);
    }
    xtransform.matrix[0][2] += XDoubleToFixed(from.x()-to.x()*scalex);
    xtransform.matrix[1][2] += XDoubleToFixed(from.y()-to.y()*scaley);
  } else {
    xtransform.matrix[0][2] += XDoubleToFixed(from.x()-to.x());
    xtransform.matrix[1][2] += XDoubleToFixed(from.y()-to.y());
  }
  XRenderSetPictureTransform(xdisplay, p, &xtransform);
  if (type == MASK) {
    XftColor color;
    {color.pixel = current_xpixel;
    uchar r,g,b; split_color(getcolor(), r,g,b);
    color.color.red   = r*0x101;
    color.color.green = g*0x101;
    color.color.blue  = b*0x101;
    color.color.alpha = 0xffff;}
    Picture solid = XftDrawSrcPicture(xftc, &color);
    XRenderComposite(xdisplay, PictOpOver,
		     solid, p, XftDrawPicture(xftc), // src, mask, dest
		     x, y, // src xy (in destination space!)
		     x, y, // mask xy
		     x, y, r-x, b-y); // rectangle to fill
  } else if (type==RGBA || type==ARGB32) {
    XRenderComposite(xdisplay, PictOpOver,
		     p, 0, XftDrawPicture(xftc), // src, mask, dest
		     x, y, // src xy (in destination space!)
		     0, 0, // mask xy
		     x, y, r-x, b-y); // rectangle to fill
  } else {
    XRenderComposite(xdisplay, PictOpSrc,
		     p, 0, XftDrawPicture(xftc), // src, mask, dest
		     x, y, // src xy (in destination space!)
		     0, 0, // mask xy
		     x, y, r-x, b-y); // rectangle to fill
  }
}

#else
# define fl_rgba_xrender_format 0
#endif

static void figure_out_visual() {

  xpixel(BLACK); // make sure figure_out_visual in color.cxx is called

  i.format = ZPixmap;
//i.bitmap_unit = 8;
//i.bitmap_bit_order = MSBFirst;
//i.bitmap_pad = 8;

  converter[MASK] = mask_converter;
  converter[RGBA] = rgba_converter;
  converter[ARGB32] = argb32_converter;

#if USE_XFT
  // See if XRender is going to work.
  // RGBA is run through the generic ARGB32 bitmap format. To allow
  // RGB images to be drawn with XCopyArea or XPutImage I use the
  // bitmap format that matches the visual. This code assummes that
  // format is laid out exactly the same as RGBA.
  fl_rgba_xrender_format =
    XRenderFindStandardFormat(xdisplay, PictStandardARGB32);
  if (fl_rgba_xrender_format) {
    converter[MASK] = mask_to_32;
    converter[RGBA] = rgba_to_xrgb;
    converter[ARGB32] = direct_32;
  }
#endif

  static XPixmapFormatValues *pfvlist;
  static int NUM_pfv;
  if (!pfvlist) pfvlist = XListPixmapFormats(xdisplay,&NUM_pfv);
  XPixmapFormatValues *pfv;
  for (pfv = pfvlist; pfv < pfvlist+NUM_pfv; pfv++)
    if (pfv->depth == xvisual->depth) break;
  i.bits_per_pixel = pfv->bits_per_pixel;

  if (i.bits_per_pixel & 7) bytes_per_pixel = 0; // produce fatal error
  else bytes_per_pixel = i.bits_per_pixel/8;

  unsigned int n = pfv->scanline_pad/8;
  if (pfv->scanline_pad & 7 || (n&(n-1)))
    fatal("Can't do scanline_pad of %d",pfv->scanline_pad);
  scanline_add = n-1;
  scanline_mask = -n;

#if USE_COLORMAP
  if (bytes_per_pixel == 1) {
    i.byte_order = ImageByteOrder(xdisplay);
    converter[MONO] = mono_to_8;
    converter[RGB] = rgb_to_8;
    converter[RGBx] = rgba_to_8;
    converter[RGB32] = argb32_to_8;
    return;
  }
  if (!xvisual->red_mask)
    fatal("Can't do %d bits_per_pixel colormap",i.bits_per_pixel);
#endif

  // otherwise it is a TrueColor visual:

  int rs = fl_redshift;
  int gs = fl_greenshift;
  int bs = fl_blueshift;

  switch (bytes_per_pixel) {

  case 2:
    // All 16-bit TrueColor visuals are supported on any machine with
    // 24 or more bits per integer.
#ifdef U16
    ::i.byte_order = WORDS_BIGENDIAN;
#else
    ::i.byte_order = 1;
#endif
    if (rs == 11 && gs == 6 && bs == 0 && fl_extrashift == 3) {
      converter[MONO] = mono_to_565;
      converter[RGB] = rgb_to_565;
      converter[RGBx] = rgba_to_565;
      converter[RGB32] = argb32_to_565;
    } else {
      converter[MONO] = mono_to_16;
      converter[RGB] = rgb_to_16;
      converter[RGBx] = rgba_to_16;
      converter[RGB32] = argb32_to_16;
    }
    break;

  case 4:
    converter[MONO] = mono_to_32;
    if (rs == 0 && gs == 8 && bs == 16) {
      ::i.byte_order = !WORDS_BIGENDIAN;
      printf("A\n");
      goto RGBX;
    }
    if (rs == 24 && gs == 16 && bs == 8) {
      ::i.byte_order = WORDS_BIGENDIAN;
      printf("B\n");
    RGBX:
      converter[RGB] = rgb_to_rgbx;
#if WORDS_BIGENDIAN
      converter[RGBx] = direct_32;
#else
      converter[RGBx] = rgba_to_rgbx;
#endif
      converter[RGB32] = argb32_to_rgbx;
      break;
    }
    if (rs == 8 && gs == 16 && bs == 24) {
      ::i.byte_order = !WORDS_BIGENDIAN;
      printf("C\n");
      goto XRGB;
    }
    if (rs == 16 && gs == 8 && bs == 0) {
      ::i.byte_order = WORDS_BIGENDIAN;
      printf("D\n");
    XRGB:
      converter[RGB] = rgb_to_xrgb;
      converter[RGBx] = rgba_to_xrgb;
      converter[RGB32] = direct_32;
      break;
    }
    // else fallthrough to failure case:
  default:
    fatal("Can't do %d-bit 0x%x 0x%x 0x%x visual",
	  i.bits_per_pixel, xvisual->red_mask,
	  xvisual->green_mask, xvisual->blue_mask);
  }
  //printf("bytes per pixel %d, byte order %d\n",bytes_per_pixel,::i.byte_order);
}

////////////////////////////////////////////////////////////////

#define MAXBUFFER 0x40000 // 256k

struct PixmapPair {
  XWindow rgb;
  XWindow alpha;
};

extern fltk::Image* fl_current_Image;
class fltk::DrawImageHelper {
public:
  static XWindow create_pixmap(int depth) {
    fl_current_Image->flags = Image::DRAWN|Image::OPAQUE;
    PixmapPair* picture = (PixmapPair*)(fl_current_Image->picture);
    if (!picture->rgb)
      picture->rgb = XCreatePixmap(xdisplay, xwindow,
				   fl_current_Image->w_,
				   fl_current_Image->h_,
				   depth);
    return picture->rgb;
  }
  static void setalpha(int w, int h) {
    fl_current_Image->flags =
      mixbg ? (Image::USES_BG|Image::DRAWN) : Image::DRAWN;
    PixmapPair* picture = (PixmapPair*)(fl_current_Image->picture);
    if (picture->alpha) XFreePixmap(xdisplay, picture->alpha);
    picture->alpha =
      XCreateBitmapFromData(xdisplay, xwindow, (char*)alphabuffer, (w+7)&-8, h);
  }
  static void setalphaflags() {
    fl_current_Image->flags = Image::DRAWN;
  }
  static void setmaskflags() {
    fl_current_Image->flags = Image::DRAWN|Image::MASK;
  }
  static void setfgbgflags() {
    fl_current_Image->flags =
      mixbg ? Image::USES_FG|Image::USES_BG|Image::DRAWN|Image::MASK :
	      Image::USES_FG|Image::DRAWN|Image::MASK;
  }
};

extern void fl_restore_clip();
static void putimage(int x, int y, int w, int h, XWindow target, GC gc) {
  if (alphapointer && !fl_current_Image) {
    XWindow alpha =
      XCreateBitmapFromData(xdisplay, xwindow,
			    (char*)alphabuffer, (w+7)&-8, h);
    alphapointer = alphabuffer;
    XSetClipMask(xdisplay, gc, alpha);
    XSetClipOrigin(xdisplay, gc, x, y);
    XPutImage(xdisplay, target, gc, &i, 0, 0, x, y, w, h);
    XSetClipOrigin(xdisplay, gc, 0, 0);
    fl_restore_clip();
    XFreePixmap(xdisplay, alpha);
  } else {
    XPutImage(xdisplay, target, gc, &i, 0, 0, x, y, w, h);
  }
}

// Combines both the callback and buffer image drawing functions
static void innards(const uchar *buf, PixelType type,
		    const Rectangle& r1,
		    int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  if (!bytes_per_pixel) figure_out_visual();

  const bool hasalpha = type==MASK || type==RGBA || type==ARGB32;

  int dx,dy,x,y,w,h;
  XWindow target;
  i.depth = xvisual->depth;
  GC gc = fltk::gc;

#if USE_XFT
  const bool scaling = !fl_trivial_transform();
  const bool use_xrender = fl_rgba_xrender_format &&
    (fl_current_Image || hasalpha || scaling);
  static GC gc32;
#endif

  if (fl_current_Image) {
    dx = dy = x = y = 0;
    w = r1.w();
    h = r1.h();
#if USE_XFT
    if (use_xrender) {
      i.depth = 32;
      target = DrawImageHelper::create_pixmap(32);
      if (type==MASK) DrawImageHelper::setmaskflags();
      else if (hasalpha) DrawImageHelper::setalphaflags();
      if (!gc32) gc32 = XCreateGC(xdisplay, target, 0, 0);
      gc = gc32;
    } else
#endif
      target = DrawImageHelper::create_pixmap(i.depth);
#if USE_XFT
  } else if (use_xrender) {
    dx = dy = x = y = 0;
    w = r1.w();
    h = r1.h();
    i.depth = 32;
    static XWindow ptarget;
    static int pw, ph;
    if (w != pw || h != ph) {
      if (ptarget) XFreePixmap(xdisplay, ptarget);
      ptarget = XCreatePixmap(xdisplay, xwindow, w, h, 32);
      pw = w; ph = h;
    }
    target = ptarget;
    if (!gc32) gc32 = XCreateGC(xdisplay, target, 0, 0);
    gc = gc32;
#endif
  } else {
    target = fltk::xwindow;
    // because scaling is not supported, I just draw the image centered:
    // This is the rectangle I want to fill:
    Rectangle tr(r1); transform(tr);
    // Center the image in that rectangle:
    Rectangle r(tr, r1.w(), r1.h());
    // Clip image if it is bigger than destination rectangle:
    Rectangle cr(r);
    if (r1.w() >= tr.w()) {cr.x(tr.x()); cr.w(tr.w());}
    if (r1.h() >= tr.h()) {cr.y(tr.y()); cr.h(tr.h());}
    // Clip with current clip region,
    // otherwise the alpha mask will allow it to draw outside:
    if (!intersect_with_clip(cr)) return;
    x = cr.x();
    y = cr.y();
    w = cr.w();
    h = cr.h();
    dx = x-r.x();
    dy = y-r.y();
  }

  const int delta = depth(type);
  if (buf) buf += dx*delta + dy*linedelta;

  i.width = w;
  i.height = h;

  void (*conv)(const uchar *from, uchar *to, int w) = converter[type];

  if (hasalpha && !fl_rgba_xrender_format) {
    split_color(getcolor(), fg[0],fg[1],fg[2]);
    split_color(getbgcolor(), bg[0],bg[1],bg[2]);
    alpha_increment = (w+7)>>3;
    if (alpha_increment*h > alphabuffersize) {
      delete[] alphabuffer;
      alphabuffersize = alpha_increment*h;
      alphabuffer = new uchar[alphabuffersize];
    }
    alphapointer = alphabuffer;
    mixbg = false;
  } else {
    if (type==MASK) {split_color(getcolor(), fg[0],fg[1],fg[2]); mixbg=false;}
    alphapointer = 0;
  }

  // Direct-dump RGB or BGR data if it is already laid out correctly.
  // This assummes the server will ignore the 4th byte,
  // that it works when the image is not word-aligned,
  // and it works if bytes_per_line is negative.
  // This seems to all work on my XFree86 setup
  if (buf && conv==direct_32 && !(linedelta&scanline_add)) {
    i.data = (char *)buf;
    i.bytes_per_line = linedelta;
    XPutImage(xdisplay, target, gc, &i, 0, 0, x, y, w, h);
  } else {
    int linesize = ((w*bytes_per_pixel+scanline_add)&scanline_mask)/4;
    int blocking = h;
    static U32* buffer;	// our storage, always word aligned
    static long buffer_size;
    {int size = linesize*h;
    if (size > MAXBUFFER) {
      size = MAXBUFFER;
      blocking = MAXBUFFER/linesize;
    }
    if (size > buffer_size) {
      delete[] buffer;
      buffer_size = size;
      buffer = new U32[size];
    }}
    i.data = (char *)buffer;
    i.bytes_per_line = linesize*4;
    if (buf) {
      for (int j=0; j<h; ) {
	U32 *to = buffer;
	int k;
	for (k = 0; j<h && k<blocking; k++, j++) {
	  conv(buf, (uchar*)to, w);
	  buf += linedelta;
	  to += linesize;
	}
	putimage(x, y+j-k, w, k, target, gc);
      }
    } else {
      U32* linebuf = new U32[(r1.w()*delta+3)/4];
      for (int j=0; j<h; ) {
	U32* to = buffer;
	int k;
	for (k = 0; j<h && k<blocking; k++, j++) {
	  const uchar* ret = cb(userdata, dx, dy+j, w, (uchar*)linebuf);
	  conv(ret, (uchar*)to, w);
	  to += linesize;
	}
	putimage(x, y+j-k, w, k, target, gc);
      }
      delete[] linebuf;
    }
  }

#if USE_XFT
  if (use_xrender) {
    if (!fl_current_Image)
      fl_xrender_draw_image(target, type, Rectangle(0,0,w,h), r1);
    return;
  }
#endif
  if (fl_current_Image) {
    if (alphapointer) DrawImageHelper::setalpha(w,h);
    if (type==MASK) DrawImageHelper::setfgbgflags();
  }
}

#define DITHER_FILLRECT (xvisual->depth <= 16)

//
// End of "$Id$"
//
