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

// Only a limited set of possible X servers is supported:
// Pixmaps must have 8, 16, or 32 bits_per_pixel (visual depth may be less)
// If display is not TrueColor, only 8 bits is supported
// 32 bits must be arranged rgba, argb, bgra, or abgr.

#include <fltk/error.h>
#include <fltk/Image.h>
#include "XColorMap.h"
using namespace fltk;

/// Converter functions:

static int dir;		// direction-alternator
static int ri,gi,bi;	// saved error-diffusion value

#if USE_COLORMAP
////////////////////////////////////////////////////////////////
// 8-bit converter with error diffusion

static void rgb_to_8(const uchar *from, uchar *to, int w, int delta) {
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

static void bgr_to_8(const uchar *from, uchar *to, int w, int delta) {
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
}

static void mono_to_8(const uchar *from, uchar *to, int w, int delta) {
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

static void rgb_to_16(const uchar *from, uchar *to, int w, int delta) {
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

static void bgr_to_16(const uchar *from, uchar *to, int w, int delta) {
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
}

static void mono_to_16(const uchar *from,uchar *to,int w, int delta) {
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

// special-case the 5r6g5b layout used by XFree86:

static void rgb_to_565(const uchar *from, uchar *to, int w, int delta) {
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

static void bgr_to_565(const uchar *from, uchar *to, int w, int delta) {
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
    r = (r&7)+from[2]; if (r>255) r = 255;
    g = (g&3)+from[1]; if (g>255) g = 255;
    b = (b&7)+from[0]; if (b>255) b = 255;
    OUTASSIGN(((r&0xf8)<<8) + ((g&0xfc)<<3) + (b>>3));
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
}

static void mono_to_565(const uchar *from,uchar *to,int w, int delta) {
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
  int r=ri;
  for (;; from += d, t += td) {
    r = (r&7) + *from; if (r > 255) r = 255;
    OUTASSIGN((r>>3) * 0x841);
    if (!--w) break;
  }
  ri = r;
}

////////////////////////////////////////////////////////////////
// 32bit TrueColor converters on a 32 or 64-bit machine:

#ifdef U64
#define STORETYPE U64
#if WORDS_BIGENDIAN
#define INNARDS32(f) \
  U64 *t = (U64*)to; \
  int w1 = (w+1)/2; \
  for (;;from += delta) {U64 i = f; from += delta; *t++ = (i<<32)|(f); if (!--w1) break;}
#else
#define INNARDS32(f) \
  U64 *t = (U64*)to; \
  int w1 = (w+1)/2; \
  for (;; from += delta) {U64 i = f; from += delta; *t++ = ((U64)(f)<<32)|i; if (!--w1) break;}
#endif
#else
#define STORETYPE U32
#define INNARDS32(f) \
  U32 *t = (U32*)to; for (; w--; from += delta) *t++ = f
#endif

// These are named as though the source is rgb order.

static void to_rgbx(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32((unsigned(from[0])<<24)+(from[1]<<16)+(from[2]<<8));
}

static void to_xbgr(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32((from[0])+(from[1]<<8)+(from[2]<<16));
}

static void to_xrgb(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32((from[0]<<16)+(from[1]<<8)+(from[2]));
}

static void to_bgrx(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32((from[0]<<8)+(from[1]<<16)+(unsigned(from[2])<<24));
}

static void to_rrrx(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32(unsigned(*from) * 0x1010100U);
}

static void to_xrrr(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32(*from * 0x10101U);
}

// arbitrary bitmasks, are there any systems that need this?

static void
rgb_to_32(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32(
    (from[0]<<fl_redshift)+(from[1]<<fl_greenshift)+(from[2]<<fl_blueshift));
}

static void
bgr_to_32(const uchar *from, uchar *to, int w, int delta) {
  INNARDS32(
    (from[2]<<fl_redshift)+(from[1]<<fl_greenshift)+(from[0]<<fl_blueshift));
}

static void
mono_to_32(const uchar *from,uchar *to,int w, int delta) {
  INNARDS32(
    (*from << fl_redshift)+(*from << fl_greenshift)+(*from << fl_blueshift));
}

////////////////////////////////////////////////////////////////

static XImage i;	// template used to pass info to X
static int bytes_per_pixel;
static int scanline_add;
static int scanline_mask;
static void (*rgb_converter)(const uchar *from, uchar *to, int w, int delta);
static void (*bgr_converter)(const uchar *from, uchar *to, int w, int delta);
static void (*mono_converter)(const uchar *from, uchar *to, int w, int delta);

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
  if (n < sizeof(STORETYPE)) n = sizeof(STORETYPE);
  scanline_add = n-1;
  scanline_mask = -n;

#if USE_COLORMAP
  if (bytes_per_pixel == 1) {
    rgb_converter = rgb_to_8;
    bgr_converter = bgr_to_8;
    mono_converter = mono_to_8;
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
      rgb_converter = rgb_to_565;
      bgr_converter = bgr_to_565;
      mono_converter = mono_to_565;
    } else {
      rgb_converter = rgb_to_16;
      bgr_converter = bgr_to_16;
      mono_converter = mono_to_16;
    }
    break;

  case 4:
    if ((::i.byte_order!=0) != WORDS_BIGENDIAN)
      {rs = 24-rs; gs = 24-gs; bs = 24-bs;}
    if (rs == 0 && gs == 8 && bs == 16) {
      rgb_converter = to_xbgr;
      bgr_converter = to_xrgb;
      mono_converter = to_xrrr;
    } else if (rs == 16 && gs == 8 && bs == 0) {
      rgb_converter = to_xrgb;
      bgr_converter = to_xbgr;
      mono_converter = to_xrrr;
    } else if (rs == 24 && gs == 16 && bs == 8) {
      rgb_converter = to_rgbx;
      bgr_converter = to_bgrx;
      mono_converter = to_rrrx;
    } else if (rs == 8 && gs == 16 && bs == 24) {
      rgb_converter = to_bgrx;
      bgr_converter = to_rgbx;
      mono_converter = to_rrrx;
    } else {
      ::i.byte_order = WORDS_BIGENDIAN;
      rgb_converter = rgb_to_32;
      bgr_converter = bgr_to_32;
      mono_converter = mono_to_32;
    }
    break;

  default:
    fatal("Can't do %d bits_per_pixel",i.bits_per_pixel);
  }
  //printf("bytes per pixel %d, byte order %d\n",bytes_per_pixel,::i.byte_order);
}

////////////////////////////////////////////////////////////////
// alpha pre-processing:

// call this second...
static void (*final_convert)(const uchar *from, uchar *to, int w, int delta);

// the current colors, swapped to be in the same order as the source:
static uchar fg[3];
static uchar bg[3];

// Mask just interpolates the fg/bg. It could use the mask but it does
// not because I need it to be fast...
static void mask_convert(const uchar* from, uchar* to, int w, int delta)
{
  uchar buffer[3*w];
  uchar* bp = buffer;
  for (int i = 0; i < w; i++) {
    uchar c = *from; from += delta;
    for (int j = 0; j < 3; j++)
      *bp++ = (c*bg[j]+(255-c)*fg[j])>>8;
  }
  final_convert(buffer, to, w, 3);
}

static uchar* alphabuffer;
static int alphabuffersize;
static uchar* alphapointer;
static int alpha_increment;
static bool mixbg;

static void doalpha(const uchar* alpha, const uchar* from, uchar* to, int w, int delta) {
  uchar buffer[3*w];
  uchar* bp = buffer;
  uchar* ap = alphapointer;
  uchar aaccum = 0;
  uchar amask = 1;
  for (int i = 0; i < w; i++, from+=delta, alpha += delta) {
    uchar a = *alpha;
    if (a == 255) {
      *bp++ = from[0];
      *bp++ = from[1];
      *bp++ = from[2];
      aaccum |= amask;
    } else if (a) {
      *bp++ = from[0]+((bg[0]*(255-a))>>8);
      *bp++ = from[1]+((bg[1]*(255-a))>>8);
      *bp++ = from[2]+((bg[2]*(255-a))>>8);
      aaccum |= amask;
      mixbg = true;
    } else {
      *bp++ = 0;
      *bp++ = 0;
      *bp++ = 0;
    }
    if (amask == 0x80) {
      *ap++ = aaccum; aaccum = 0; amask = 1;
    } else {
      amask <<= 1;
    }
  }
  *ap = aaccum;
  alphapointer += alpha_increment;
  final_convert(buffer, to, w, 3);
}

static void prealpha(const uchar* from, uchar* to, int w, int delta) {
  doalpha(from, from+1, to, w, delta);
}

static void postalpha(const uchar* from, uchar* to, int w, int delta) {
  doalpha(from+3, from, to, w, delta);
}

static void domask(const uchar* alpha, const uchar* from, uchar* to, int w, int delta) {
  static int aerror;
  uchar* ap = alphapointer;
  uchar aaccum = 0;
  uchar amask = 1;
  int a = aerror;
  for (int i = 0; i < w; i++, alpha += delta) {
    a += *alpha;
    if (a > 127) {
      aaccum |= amask;
      a -= 255;
    }
    if (amask == 0x80) {
      *ap++ = aaccum; aaccum = 0; amask = 1;
    } else {
      amask <<= 1;
    }
  }
  aerror = a;
  *ap = aaccum;
  alphapointer += alpha_increment;
  final_convert(from, to, w, delta);
}

static void premask(const uchar* from, uchar* to, int w, int delta) {
  domask(from, from+1, to, w, delta);
}

static void postmask(const uchar* from, uchar* to, int w, int delta) {
  domask(from+3, from, to, w, delta);
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
static void putimage(int x, int y, int w, int h, bool bitmask) {
  if (bitmask) {
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
		    int delta, int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  Rectangle r(r1); transform(r);
  Rectangle cr(r); if (!intersect_with_clip(cr)) return;

  int dx = cr.x()-r.x();
  int dy = cr.y()-r.y();
  int w = cr.w();
  int h = cr.h();

  if (!bytes_per_pixel) figure_out_visual();
  i.width = w;
  i.height = h;

  void (*conv)(const uchar *from, uchar *to, int w, int delta);

  static char flip[12] = {0,0,1,0,0,1,1,0,0,1,1,0};

  switch (type) {
  case MONO:
    conv = mono_converter;
    break;
  case BGR:
    conv = bgr_converter;
    break;
  case RGB:
    conv = rgb_converter;
    break;
  case MASK:
    conv = mask_convert;
    final_convert = rgb_converter;
    goto J1;
  default:
    if (type >= RGBM)
      conv = (type&1) ? premask : postmask;
    else
      conv = (type&1) ? prealpha : postalpha;
  J1:
    if (flip[type]) {
      final_convert = bgr_converter;
      split_color(getcolor(), fg[2],fg[1],fg[0]);
      split_color(getbgcolor(), bg[2],bg[1],bg[0]);
    } else {
      final_convert = rgb_converter;
      split_color(getcolor(), fg[0],fg[1],fg[2]);
      split_color(getbgcolor(), bg[0],bg[1],bg[2]);
    }
    alpha_increment = (w+7)>>3;
    if (alpha_increment*h > alphabuffersize) {
      delete[] alphabuffer;
      alphabuffersize = alpha_increment*h;
      alphabuffer = new uchar[alphabuffersize];
    }
    alphapointer = alphabuffer;
    mixbg = false;
    break;
  }

#if 1
  // Direct-dump RGB or BGR data if it is already laid out correctly.
  // This assummes the server will ignore the 4th byte,
  // that it works when the image is not word-aligned,
  // and it works if bytes_per_line is negative.
  // This seems to all work on my XFree86 setup
  if (buf &&
      delta == 4 &&
#if WORDS_BIGENDIAN
      (conv == to_rgbx || conv == to_xrgb)
#else
      (conv == to_xbgr || conv == to_bgrx)
#endif
      && !(linedelta&scanline_add)) {
    i.data = (char *)(buf+delta*dx+linedelta*dy);
    if (conv == to_xrgb || conv == to_bgrx) i.data++;
    i.bytes_per_line = linedelta;
    XPutImage(xdisplay,xwindow,gc, &i, 0, 0, cr.x(), cr.y(), w, h);
    return;
  }
#endif

  int linesize = ((w*bytes_per_pixel+scanline_add)&scanline_mask)/sizeof(STORETYPE);
  int blocking = h;
  static STORETYPE *buffer;	// our storage, always word aligned
  static long buffer_size;
  {int size = linesize*h;
  if (size > MAXBUFFER) {
    size = MAXBUFFER;
    blocking = MAXBUFFER/linesize;
  }
  if (size > buffer_size) {
    delete[] buffer;
    buffer_size = size;
    buffer = new STORETYPE[size];
  }}
  i.data = (char *)buffer;
  i.bytes_per_line = linesize*sizeof(STORETYPE);
  if (buf) {
    buf += delta*dx+linedelta*dy;
    for (int j=0; j<h; ) {
      STORETYPE *to = buffer;
      int k;
      for (k = 0; j<h && k<blocking; k++, j++) {
	conv(buf, (uchar*)to, w, delta);
	buf += linedelta;
	to += linesize;
      }
      putimage(cr.x(), cr.y()+j-k, w, k, type >= RGBA);
    }
  } else {
    STORETYPE* linebuf = new STORETYPE[(r1.w()*delta+(sizeof(STORETYPE)-1))/sizeof(STORETYPE)];
    for (int j=0; j<h; ) {
      STORETYPE *to = buffer;
      int k;
      for (k = 0; j<h && k<blocking; k++, j++) {
	const uchar* ret = cb(userdata, dx, dy+j, w, (uchar*)linebuf);
	conv(ret, (uchar*)to, w, delta);
	to += linesize;
      }
      putimage(cr.x(), cr.y()+j-k, w, k, type >= RGBA);
    }
    delete[] linebuf;
  }

  if (fl_current_Image) {
    if (type==MASK) {
      DrawImageHelper::setmaskflags();
    } else if (type >= RGBA) {
      DrawImageHelper::setalpha(w,h);
    }
  }
}

#define DITHER_FILLRECT (xvisual->depth <= 16)

//
// End of "$Id$"
//
