//
// "$Id: fl_draw_image.cxx,v 1.22 2004/06/04 08:34:25 spitzak Exp $"
//
// Image drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#if defined(_WIN32) && WINVER<0x0500
# undef WINVER
# define WINVER 0x0500
#endif
#include <fltk/x.h>
#include <fltk/events.h>
#include <fltk/draw.h>

// I hope a simple and portable method of drawing color and monochrome
// images.  To keep this simple, only a single storage type is
// supported: 8 bit unsigned data, byte order RGB, and pixels are
// stored packed into rows with the origin at the top-left.  It is
// possible to alter the size of pixels with the "delta" argument, to
// add alpha or other information per pixel.  It is also possible to
// change the origin and direction of the image data by messing with
// the "delta" and "linedelta", making them negative, though this may
// defeat some of the shortcuts in translating the image for X.

// These files define "innards" and the "DITHER_FILLRECT" macro:
#if USE_X11
# include "fl_draw_image_x.cxx"
#elif defined(_WIN32)
# include "fl_draw_image_win32.cxx"
#elif defined(__APPLE__)
# include "fl_draw_image_mac.cxx"
#endif

using namespace fltk;

/*! \defgroup images Drawing Images
    \ingroup drawing

  If you plan to draw the same image many times, you may want an
  fltk::Image subclass and call draw() on it. The advantage of using
  such an object is that FLTK will cache translated forms of the image (on
  X it uses a server pixmap) and thus redrawing is much faster. In
  addition, on current systems, fltk::Image is the only way to get
  transparency or to draw 1-bit bitmaps.

  The advantage of drawing directly is that it is more intuitive, and
  it is faster if the image data changes more often than it is
  redrawn.

  Currently the image is only affected by the integer portion of the
  current transformation. This may change in future versions!
*/

/*!

  Draw an 8-bit per color RGB or luminance image. The pointer points
  at the byte of red data of the top-left pixel. Data must be in r,g,b
  order. \a x,y are where to put the top-left corner. \a w,h define the
  size of the image. \a d is the delta to add to the pointer between
  pixels, it may be any value greater than 0, or it can be
  negative to flip the image horizontally. \a ld is the delta to add to
  the pointer between lines (if 0 is passed it uses w*d), and may be
  larger than w*d to crop data, or negative to flip the image
  vertically. You can also get 90 degree rotations by clever
  settings of \a d and \a l.

  It is highly recommended that you put fltk::visual(fltk::RGB) at the
  start of your code to force the X visual to one that is nice. This
  probably does nothing on non-X systems and on any modern X system,
  but it is still a good idea.

  If abs(d) is less than 3 a monochrome image is drawn. You can also
  force monochrome by calling drawimagemono().

  The X version does not support some visuals and will abort() if
  this is called. This includes PsuedoColor maps of more than 8 bits.
  Fortunately all such X servers are pretty much gone now.
*/
void fltk::drawimage(const uchar* buf, int x, int y, int w, int h, int d, int l){
  innards(buf,x,y,w,h,d,l,(d<3&&d>-3),0,0);
}

/*!
  Gray scale (1-channel) images may be drawn. Only one 8-bit sample is
  used for each pixel, and on screens with different numbers of bits
  for red, green, and blue only gray colors are used. Setting D
  greater than 1 will let you display one channel of a color image.
*/
void fltk::drawimagemono(const uchar* buf, int x, int y, int w, int h, int d, int l){
  innards(buf,x,y,w,h,d,l,1,0,0);
}

/*! \typedef fltk::DrawImageCallback
  Type of function passed to drawimage(). It must fill in the buffer
  pointed to by the last argument with the pixels to draw for a row.
*/

/*!
  Call the passed function to provide each scan line of the
  image. This lets you generate the image as it is being drawn, or do
  arbitrary decompression of stored data (provided it can be
  decompressed to individual scan lines easily).

  The callback is called with the void* \a data argument (this can be
  used to point at a structure of information about the image), and
  the x, y, and w of the scan line desired from the image. 0,0 is the
  upper-left corner (not the x,y passed to this!). A pointer to a
  buffer to put the data into is passed. You must copy w pixels from
  scanline y, starting at pixel x, to this buffer.

  Due to cropping, less than the whole image may be requested. So x
  may be greater than zero, the first y may be greater than zero, and
  w may be less than W. The buffer is long enough to store the entire
  W * D pixels, this is for convienence with some decompression
  schemes where you must decompress the entire line at once:
  decompress it into the buffer, and then if x is not zero, copy the
  data over so the x'th pixel is at the start of the buffer.

  You can assume the y's will be consecutive, except the first one may
  be greater than zero.

  If \a d is 4 or more, you must fill in the unused bytes with zero.
*/
void fltk::drawimage(DrawImageCallback cb, void* data,
		   int x, int y, int w, int h,int d) {
  innards(0,x,y,w,h,d,0,(d<3&&d>-3),cb,data);
}

/*!
  Similar but force a single channel to be drawn even if d is larger than 1.
*/
void fltk::drawimagemono(DrawImageCallback cb, void* data,
		   int x, int y, int w, int h,int d) {
  innards(0,x,y,w,h,d,0,1,cb,data);
}

#if 0
void fltk::fill_color_rect(int x, int y, int w, int h, Color C) {
  if (!DITHER_FILLRECT) {
    setcolor(C);
    fillrect(x,y,w,h);
  } else {
    uchar c[3];
    split_color(C, c[0], c[1], c[2]);
    innards(c,x,y,w,h,0,0,0,0,0);
  }
}
#endif

//
// End of "$Id: fl_draw_image.cxx,v 1.22 2004/06/04 08:34:25 spitzak Exp $".
//
