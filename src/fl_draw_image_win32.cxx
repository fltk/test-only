//
// "$Id: fl_draw_image_win32.cxx,v 1.25 2005/01/24 08:07:54 spitzak Exp $"
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

#include <fltk/Color.h>
#include <stdio.h>
using namespace fltk;

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

static void innards(const uchar *buf, PixelType type,
		    const Rectangle& r1,
		    int delta, int linedelta,
		    DrawImageCallback cb, void* userdata)
{
#if USE_COLORMAP
  char indexed = (xpalette != 0);
#endif

  Rectangle r(r1); transform(r);
  Rectangle cr(r); if (!intersect_with_clip(cr)) return;
  if (!linedelta) linedelta = r1.w()*delta;
  int x = cr.x();
  int y = cr.y();
  int w = cr.w();
  int h = cr.h();
  int X = r.x();
  int Y = r.y();
  int W = r1.w();
  if (buf) buf += (x-X)*delta + (y-Y)*linedelta;

  static U32 bmibuffer[256+12];
  BITMAPINFO &bmi = *((BITMAPINFO*)bmibuffer);
  if (!bmi.bmiHeader.biSize) {
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
  }

  // -1 == none
  // 0  == indexed
  // 1  == mono
  static int current_cmap = -1;

#if USE_COLORMAP
  if (indexed) {
    if(current_cmap != 0) {
      current_cmap = 0;
      for (short i=0; i<256; i++) {
        *((short*)(bmi.bmiColors)+i) = i;
      }
    }
  } else
#endif
    bool mono = (type == LUMINANCE);
  if (mono) {
    if(current_cmap != 1) {
      current_cmap = 1;
      for (int i=0; i<256; i++) {
        bmi.bmiColors[i].rgbBlue = (uchar)i;
        bmi.bmiColors[i].rgbGreen = (uchar)i;
        bmi.bmiColors[i].rgbRed = (uchar)i;
        bmi.bmiColors[i].rgbReserved = (uchar)i;
      }
    }
  }
  
#if USE_COLORMAP
  int pixelsize = mono|indexed ? 1 : 4;
#else
  int pixelsize = mono ? 1 : 4;
#endif

  int linesize = pixelsize * w;
  if ((linesize % 4) != 0) {
    linesize += 4 - (linesize % 4);
  }

  bmi.bmiHeader.biWidth = w;
  bmi.bmiHeader.biBitCount = pixelsize*8;

  static U32* buffer;
  int blocking = h;
  {int size = linesize*h;
  if (size > MAXBUFFER) {
    size = MAXBUFFER;
    blocking = MAXBUFFER/linesize;
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
#if USE_COLORMAP
      if (indexed) {
        if (mono)
          monodither(to, from, w, delta);
        else 
          dither(to, from, w, delta);
        to += w;
      } else
#endif
        if (mono) {
          for (int i=w; i--; from += delta) *to++ = *from;
        } else {
          for (int i=w; i--; from += delta, to += pixelsize) {
            if(delta == 4) {
              // Premultiply for AlphaBlend
              int a = from[3];
              float afactor = (float)a / (float)0xff;
              to[0] = (uchar)(float(from[2]) * afactor);
	      to[1] = (uchar)(float(from[1]) * afactor);
	      to[2] = (uchar)(float(from[0]) * afactor);
	      to[3] = a;
            } else {
	      to[0] = from[2];
	      to[1] = from[1];
	      to[2] = from[0];
              to[3] = 0xFF; // alpha channel is totally opaque
            }
          }
        }
    }
    bmi.bmiHeader.biHeight = -k;
    bmi.bmiHeader.biSizeImage = k*linesize;
#if 1
    SetDIBitsToDevice(dc, x, ypos, w, k, 0, 0, 0, k,
		      (LPSTR)buffer,
		      &bmi,
# if USE_COLORMAP
		      indexed ? DIB_PAL_COLORS : DIB_RGB_COLORS
# else
		      DIB_RGB_COLORS
# endif
		      );
#else
    // We have to do all this conversion stuff because SetDIBitsToDevice does 
    // not do alpha blending. The easiest way to use the win32 alpha blending 
    // is to use the DrawIconEx function. It does all the things necessary to 
    // draw alpha blended images.
      // This code was contributed to fltk but I have not seen it do any
      // actual alpha compositing. Maybe somebody can figure out what is
      // wrong?
    HBITMAP b = CreateDIBitmap(dc,
			       (BITMAPINFOHEADER*)&head,
			       CBM_INIT,
			       (LPSTR)buffer,
			       (BITMAPINFO*)&head,
			       DIB_RGB_COLORS); 

    // we must create a temporary single bit bitmap to satisfy the icon 
    // creating constraints, even though it isn't used.
    HBITMAP hMonoBitmap = CreateBitmap(w,h,1,1,NULL);

    ICONINFO ii;
    ii.fIcon = TRUE;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = b; //hMonoBitmap;
    ii.hbmColor = b;
    HICON imageIcon = ::CreateIconIndirect(&ii);
    DrawIconEx(dc, x, ypos, imageIcon, w, k, 0, NULL, DI_NORMAL);
    // Clean up all the mess
    ::DestroyIcon(imageIcon);
    ::DeleteObject(hMonoBitmap);
    ::DeleteObject(b);
#endif
    ypos += k;
  }
}

#if USE_COLORMAP
#define DITHER_FILLRECT (xpalette!=0)
#else
#define DITHER_FILLRECT false
#endif

//
// End of "$Id: fl_draw_image_win32.cxx,v 1.25 2005/01/24 08:07:54 spitzak Exp $".
//
