//
// "$Id: fl_draw_image_win32.cxx,v 1.9 2001/07/23 09:50:05 spitzak Exp $"
//
// WIN32 image drawing code for the Fast Light Tool Kit (FLTK).
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

#define MAXBUFFER 0x40000 // 256k

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
    int rr = r*FL_NUM_RED/256;
    r -= rr*255/(FL_NUM_RED-1);
    g += from[1]; if (g < 0) g = 0; else if (g>255) g = 255;
    int gg = g*FL_NUM_GREEN/256;
    g -= gg*255/(FL_NUM_GREEN-1);
    b += from[2]; if (b < 0) b = 0; else if (b>255) b = 255;
    int bb = b*FL_NUM_BLUE/256;
    b -= bb*255/(FL_NUM_BLUE-1);
    *to = uchar(FL_COLOR_CUBE+(bb*FL_NUM_RED+rr)*FL_NUM_GREEN+gg);
    if (!--w) break;
  }
  ri = r; gi = g; bi = b;
}

// error-diffusion dither into the fltk colormap
static void monodither(uchar* to, const uchar* from, int w, int delta) {
  static int ri,dir;
  int r=ri;
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
    r += *from; if (r < 0) r = 0; else if (r>255) r = 255;
    int rr = r*FL_NUM_GRAY/256;
    r -= rr*255/(FL_NUM_GRAY-1);
    *to = uchar(FL_GRAY_RAMP+rr);
    if (!--w) break;
  }
  ri = r;
}

#endif // USE_COLORMAP

static void innards(const uchar *buf, int X, int Y, int W, int H,
		    int delta, int linedelta, int mono,
		    Fl_Draw_Image_Cb cb, void* userdata)
{
#if USE_COLORMAP
  char indexed = (fl_palette != 0);
#endif

  if (!linedelta) linedelta = W*delta;

  int x, y, w, h;
  fl_clip_box(X,Y,W,H,x,y,w,h);
  if (w<=0 || h<=0) return;
  if (buf) buf += (x-X)*delta + (y-Y)*linedelta;
  X += fl_x_; x += fl_x_;
  Y += fl_y_; y += fl_y_;

  static U32 bmibuffer[256+12] = {0};
  BITMAPINFO &bmi = *((BITMAPINFO*)bmibuffer);
  if (!bmi.bmiHeader.biSize) {
    bmi.bmiHeader.biSize = sizeof(bmi)-4; // does it use this to determine type?
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
  }
#if USE_COLORMAP
  if (indexed) {
    for (int i=0; i<256; i++) {
      *((short*)(bmi.bmiColors)+i) = i;
    }
  } else
#endif
  if (mono) {
    for (int i=0; i<256; i++) {
      bmi.bmiColors[i].rgbBlue = i;
      bmi.bmiColors[i].rgbGreen = i;
      bmi.bmiColors[i].rgbRed = i;
      bmi.bmiColors[i].rgbReserved = i;
    }
  }
  bmi.bmiHeader.biWidth = w;
#if USE_COLORMAP
  bmi.bmiHeader.biBitCount = mono|indexed ? 8 : 24;
  int pixelsize = mono|indexed ? 1 : 3;
#else
  bmi.bmiHeader.biBitCount = mono ? 8 : 24;
  int pixelsize = mono ? 1 : 3;
#endif
  int linesize = (pixelsize*w+3)&~3;
  
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
  bmi.bmiHeader.biHeight = blocking;
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
  for (int j=0; j<h; ) {
    int k;
    for (k = 0; j<h && k<blocking; k++, j++) {
      const uchar* from;
      if (!buf) { // run the converter:
	cb(userdata, x-X, y-Y+j, w, (uchar*)line_buffer);
	from = (uchar*)line_buffer;
      } else {
	from = buf;
	buf += linedelta;
      }
      uchar *to = (uchar*)buffer+(blocking-k-1)*linesize;
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
	for (int i=w; i--; from += delta, to += 3) {
	  uchar r = from[0];
	  to[0] = from[2];
	  to[1] = from[1];
	  to[2] = r;
        }
      }
    }
    SetDIBitsToDevice(fl_gc, x, y+j-k, w, k, 0, 0, 0, k,
		      (LPSTR)((uchar*)buffer+(blocking-k)*linesize),
		      &bmi,
#if USE_COLORMAP
		      indexed ? DIB_PAL_COLORS : DIB_RGB_COLORS
#else
		      DIB_RGB_COLORS
#endif
		      );
  }
}

#if USE_COLORMAP
#define DITHER_RECTF fl_palette
#else
#define DITHER_RECTF false
#endif

//
// End of "$Id: fl_draw_image_win32.cxx,v 1.9 2001/07/23 09:50:05 spitzak Exp $".
//
