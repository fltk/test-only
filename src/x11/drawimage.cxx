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

#include <fltk/error.h>
#include <fltk/Image.h>
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

// Currently MASK just interpolates the fg/bg, producing a solid rectangle.
// It could instead produce the alpha channel for the transparent part,
// but I needed this fast and I was not drawing except on plain backgrounds.
static void mask_converter(const uchar* from, uchar* to, int w)
{
  U32 buffer[w];
  U32* bp = buffer;
  for (int i = 0; i < w; i++) {
    uchar c = *from++;
    uchar r = (c*bg[0]+(255-c)*fg[0])>>8;
    uchar g = (c*bg[1]+(255-c)*fg[1])>>8;
    uchar b = (c*bg[2]+(255-c)*fg[2])>>8;
    *bp++ = (r<<16)|(g<<8)|b;
  }
  converter[RGB32]((uchar*)buffer, to, w);
}

static uchar* alphabuffer;
static int alphabuffersize;
static uchar* alphapointer;
static int alpha_increment;
static bool mixbg;

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

static XImage i;	// template used to pass info to X
static int bytes_per_pixel;
static int scanline_add;
static int scanline_mask;

static void figure_out_visual() {

  xpixel(BLACK); // make sure figure_out_visual in color.cxx is called

  static XPixmapFormatValues *pfvlist;
  static int NUM_pfv;
  if (!pfvlist) pfvlist = XListPixmapFormats(xdisplay,&NUM_pfv);
  XPixmapFormatValues *pfv;
  for (pfv = pfvlist; pfv < pfvlist+NUM_pfv; pfv++)
    if (pfv->depth == xvisual->depth) break;
  i.format = ZPixmap;
  i.byte_order = ImageByteOrder(xdisplay);
//i.bitmap_unit = 8;
//i.bitmap_bit_order = MSBFirst;
//i.bitmap_pad = 8;
  i.depth = xvisual->depth;
  i.bits_per_pixel = pfv->bits_per_pixel;

  if (i.bits_per_pixel & 7) bytes_per_pixel = 0; // produce fatal error
  else bytes_per_pixel = i.bits_per_pixel/8;

  unsigned int n = pfv->scanline_pad/8;
  if (pfv->scanline_pad & 7 || (n&(n-1)))
    fatal("Can't do scanline_pad of %d",pfv->scanline_pad);
  scanline_add = n-1;
  scanline_mask = -n;

  converter[MASK] = mask_converter;
  converter[RGBA] = rgba_converter;
  converter[ARGB32] = argb32_converter;

#if USE_COLORMAP
  if (bytes_per_pixel == 1) {
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
  static void setalpha(int w, int h) {
    fl_current_Image->flags = mixbg ? (Image::USES_BG|Image::DRAWN) : Image::DRAWN;
    PixmapPair* picture = (PixmapPair*)(fl_current_Image->picture);
    if (picture->alpha) XFreePixmap(xdisplay, picture->alpha);
    picture->alpha =
      XCreateBitmapFromData(xdisplay, xwindow, (char*)alphabuffer, (w+7)&-8, h);
  }
  static void setmaskflags() {
    fl_current_Image->flags = Image::MASK|Image::USES_FG|Image::USES_BG|Image::DRAWN;
  }
};

extern void fl_restore_clip();
static void putimage(int x, int y, int w, int h) {
  if (alphapointer && !fl_current_Image) {
    XWindow alpha =
      XCreateBitmapFromData(xdisplay, xwindow,
			    (char*)alphabuffer, (w+7)&-8, h);
    alphapointer = alphabuffer;
    XSetClipMask(xdisplay, gc, alpha);
    XSetClipOrigin(xdisplay, gc, x, y);
    XPutImage(xdisplay,xwindow,gc, &i, 0, 0, x, y, w, h);
    XSetClipOrigin(xdisplay, gc, 0, 0);
    fl_restore_clip();
    XFreePixmap(xdisplay, alpha);
  } else {
    XPutImage(xdisplay,xwindow,gc, &i, 0, 0, x, y, w, h);
  }
}

// Combines both the callback and buffer image drawing functions
static void innards(const uchar *buf, PixelType type,
		    const Rectangle& r1,
		    int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  Rectangle r(r1); transform(r);
  Rectangle cr(r); if (!intersect_with_clip(cr)) return;

  int dx = cr.x()-r.x();
  int dy = cr.y()-r.y();
  int w = cr.w();
  int h = cr.h();
  const int delta = depth(type);
  if (buf) buf += dx*delta + dy*linedelta;

  if (!bytes_per_pixel) figure_out_visual();
  i.width = w;
  i.height = h;

  void (*conv)(const uchar *from, uchar *to, int w) = converter[type];

  if (type==MASK) {
    split_color(getcolor(), fg[0],fg[1],fg[2]);
    split_color(getbgcolor(), bg[0],bg[1],bg[2]);
    alphapointer = 0;
  } else if (type==RGBA || type==ARGB32) {
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
    alphapointer = 0;
  }

  // Direct-dump RGB or BGR data if it is already laid out correctly.
  // This assummes the server will ignore the 4th byte,
  // that it works when the image is not word-aligned,
  // and it works if bytes_per_line is negative.
  // This seems to all work on my XFree86 setup
  if (buf && conv==direct_32 && !(linedelta&scanline_add)) {
    i.data = (char *)(buf+delta*dx+linedelta*dy);
    i.bytes_per_line = linedelta;
    XPutImage(xdisplay,xwindow,gc, &i, 0, 0, cr.x(), cr.y(), w, h);
    return;
  }

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
    buf += delta*dx+linedelta*dy;
    for (int j=0; j<h; ) {
      U32 *to = buffer;
      int k;
      for (k = 0; j<h && k<blocking; k++, j++) {
	conv(buf, (uchar*)to, w);
	buf += linedelta;
	to += linesize;
      }
      putimage(cr.x(), cr.y()+j-k, w, k);
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
      putimage(cr.x(), cr.y()+j-k, w, k);
    }
    delete[] linebuf;
  }

  if (fl_current_Image) {
    if (type==MASK) {
      DrawImageHelper::setmaskflags();
    } else if (alphapointer) {
      DrawImageHelper::setalpha(w,h);
    }
  }
}

#define DITHER_FILLRECT (xvisual->depth <= 16)

//
// End of "$Id$"
//
