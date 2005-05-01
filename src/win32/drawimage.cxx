//
// "$Id$"
//
// _WIN32 image drawing code for the Fast Light Tool Kit (FLTK).
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

// I hope a simple and portable method of drawing color and monochrome
// images.  To keep this simple, only a single storage type is
// supported: 8 bit unsigned data, byte order RGB, and pixels are
// stored packed into rows with the origin at the top-left.  It is
// possible to alter the size of pixels with the "delta" argument, to
// add alpha or other information per pixel.  It is also possible to
// change the origin and direction of the image data by messing with
// the "delta" and "linedelta", making them negative, though this may
// defeat some of the shortcuts in translating the image for X.

// Unbelievably (since it conflicts with how most PC software works)
// MicroSoft picked a bottom-up and BGR storage format for their
// DIB images.  I'm pretty certain there is a way around this, but
// I can't find any other than the brute-force method of drawing
// each line as a seperate image.  This may also need to be done
// if the delta is any amount other than 1, 3, or 4.

////////////////////////////////////////////////////////////////
//
// WARNING: This file SHOULDN'T be used directly!
//   - It's included from /src/drawimage.cxx
//
////////////////////////////////////////////////////////////////

#include <fltk/Color.h>
#include <fltk/Font.h>
#include <fltk/draw.h>
#include <config.h>
#include <stdio.h>
#include <windows.h>

using namespace fltk;

#if defined(__MINGW32__) || defined(__CYGWIN__) // || defined(__BORLANDC__)
// AlphaBlend IS declared in these but only when WINVER is >= 500
extern "C" {
  WINGDIAPI BOOL  WINAPI AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
}
# define AC_SRC_ALPHA		  0x01
#endif

#define MAXBUFFER 0x40000 // 256k

#undef USE_COLORMAP
#define USE_COLORMAP 0
#if USE_COLORMAP

// error-diffusion dither into the fltk colormap
static void dither(uchar* to, const uchar* from, int w, int delta) {
  static int ri, gi, bi, dir;
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
    int rr = r*5/256;
    r -= rr*255/4;
    g += from[1]; if (g < 0) g = 0; else if (g>255) g = 255;
    int gg = g*8/256;
    g -= gg*255/7;
    b += from[2]; if (b < 0) b = 0; else if (b>255) b = 255;
    int bb = b*5/256;
    b -= bb*255/4;
    *to = uchar(BLACK+(bb*5+rr)*8+gg);
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
}

// error-diffusion dither into the fltk colormap
static void monodither(uchar* to, const uchar* from, int w, int delta) {
  static int ri, gi, bi, dir;
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
    int rr = r*5/256;
    r -= rr*255/4;
    g += from[0]; if (g < 0) g = 0; else if (g>255) g = 255;
    int gg = g*8/256;
    g -= gg*255/7;
    b += from[0]; if (b < 0) b = 0; else if (b>255) b = 255;
    int bb = b*5/256;
    b -= bb*255/4;
    *to = uchar(BLACK+(bb*5+rr)*8+gg);
    if (!--w) break;
  }
  ri = r;
}

#endif // USE_COLORMAP

extern bool fl_drawing_offscreen;

static void innards(const uchar *buf, PixelType type,
		    const fltk::Rectangle& r1,
		    int delta, int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  fltk::Rectangle r(r1); transform(r);
  fltk::Rectangle cr(r); if (!intersect_with_clip(cr)) return;

  int x = cr.x();
  int y = cr.y();
  int w = cr.w();
  int h = cr.h();
  int X = r.x();
  int Y = r.y();
  int W = r1.w();
  if (buf) buf += (x-X)*delta + (y-Y)*linedelta;

  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = w;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  static U32* buffer;
  int blocking = h;
  {int size = 4*w*h;
  if (size > MAXBUFFER) {
    size = MAXBUFFER;
    blocking = MAXBUFFER/(4*w);
  }
  static long buffer_size;
  if (size > buffer_size) {
    delete[] buffer;
    buffer_size = size;
    buffer = new U32[(size+3)/4];
  }}

  static U32* line_buffer;
  if (!buf) {
    int size = W*delta;
    static int line_buf_size;
    if (size > line_buf_size) {
      delete[] line_buffer;
      line_buf_size = size;
      line_buffer = new U32[(size+3)/4];
    }
  }

  // needed for MASK:
  uchar mr,mg,mb;
  fltk::split_color(fltk::getcolor(),mr,mg,mb);

  int ypos = y;
  for (int j=0; j<h; ) {
    int k;
    uchar *to = (uchar*)buffer;
    for (k = 0; j<h && k<blocking; k++, j++) {
      const uchar* from;
      if (!buf) { // run the converter:
        from = cb(userdata, x-X, y-Y+j, w, (uchar*)line_buffer);
      } else {
        from = buf;
        buf += linedelta;
      }
      int i;
      switch (type) {
      case MONO:
	for (i=w; i--; from += delta, to += 4) {
	  to[0] = to[1] = to[2] = *from; to[3] = 0xff;
	}
	break;
      case MASK:
	for (i=w; i--; from += delta, to += 4) {
	  uchar v = *from;
	  to[0] = (mb*v)>>8;
	  to[1] = (mg*v)>>8;
	  to[2] = (mr*v)>>8;
	  to[3] = v;
	}
	break;
      case BGR:
	for (i=w; i--; from += delta, to += 4) {
	  to[0] = from[0];
	  to[1] = from[1];
	  to[2] = from[2];
	  to[3] = 0xff;
	}
	break;
      case RGB:
	for (i=w; i--; from += delta, to += 4) {
	  to[0] = from[2];
	  to[1] = from[1];
	  to[2] = from[0];
	  to[3] = 0xff;
	}
	break;
      case RGBA:
	for (i=w; i--; from += delta, to += 4) {
	  to[0] = from[2];
	  to[1] = from[1];
	  to[2] = from[0];
	  to[3] = from[3];
	}
	break;
      case ABGR:
	for (i=w; i--; from += delta, to += 4) {
	  to[0] = from[1];
	  to[1] = from[2];
	  to[2] = from[3];
	  to[3] = from[0];
	}
	break;
      }
    }
    bmi.bmiHeader.biHeight = -k;
    bmi.bmiHeader.biSizeImage = k*4*w;
    if (fl_drawing_offscreen || type == MONO || type == RGB || type == BGR) {
      SetDIBitsToDevice(dc, x, ypos, w, k, 0, 0, 0, k,
			(LPSTR)buffer,
			&bmi,
			DIB_RGB_COLORS
			);
    } else {
      HBITMAP rgb = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);
      HDC new_dc = CreateCompatibleDC(dc);
      SelectObject(new_dc, rgb);
      SetDIBitsToDevice(new_dc, 0, 0, w, k, 0, 0, 0, k,
			(LPSTR)buffer,
			&bmi,
			DIB_RGB_COLORS
			);
      BLENDFUNCTION m_bf;
      m_bf.BlendOp = AC_SRC_OVER;
      m_bf.BlendFlags = 0;
      m_bf.AlphaFormat = 1; //AC_SRC_ALPHA;
      m_bf.SourceConstantAlpha = 0xFF;
      AlphaBlend(dc, x, ypos, w, k,
		 new_dc, 0, 0, w, k, m_bf);
      DeleteDC(new_dc);
      DeleteObject(rgb);
    }
    ypos += k;
  }
}

#if USE_COLORMAP
#define DITHER_FILLRECT (xpalette!=0)
#else
#define DITHER_FILLRECT false
#endif

//
// End of "$Id$".
//
