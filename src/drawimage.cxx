//
// "$Id$"
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
# include "x11/drawimage.cxx"
#elif defined(_WIN32)
# include "win32/drawimage.cxx"
#elif defined(__APPLE__)
# include "osx/drawimage.cxx"
#endif

using namespace fltk;

/*! \defgroup images Drawing Images
    \ingroup drawing

  If you plan to draw the same image many times, you may want an
  fltk::Image subclass and call draw() on it. The advantage of using
  such an object is that the image data is stored already-translated
  to whatever form the display needs (and on X at least it is stored
  in the same memory space as the server), so drawing it is much
  faster. (Also currently alpha transparency is broken on X and Windows
  unless you use an Image).

  The advantage of using these calls verses fltk::Image is that it is
  a lot easier to write the program. It also is the fastest way if you
  are only going to draw the image once (for instance a video image
  that is changing, or the output of your painting program that you
  want the user to change).

  Currently the image is only affected by the integer portion of the
  current transformation. This may change in future versions!

  The X version does not support some visuals and will abort() if
  this is called on them. Currently only TrueColor, the rare
  DirectColor, and PseudoColor of less or equal to 8 bits are
  supported. It is recommended that you put fltk::visual(fltk::RGB)
  at the start of your program to indicate that you want TrueColor,
  all known X servers unsupported by FLTK in their default mode
  provide a working full-color mode.
*/

/*!

  Draw an image.

  - \a pointer points at the first byte of the top-left pixel.
  - \a type describes how to interpret the bytes of each pixel.
  - \a rectangle: the image is put in the top-left corner
  and the width and height declare how many pixels the image has.
  - \a delta is how much to add to \a pointer to go 1 pixel to the right
  - \a line_delta is how much to add to \a pointer to go 1 pixel down

  By setting \a line_delta to larger than \a delta*r.w() you can crop a
  picture out of a larger buffer. By setting \a delta to larger than
  the size of the pixel data you can skip extra bytes, such as alpha
  information you don't want, or draw one channel of an rgb image as a
  gray-scale iamge. By setting \a line_delta and/or \a delta negative
  you can get 90 degree rotations and mirror images of the data.

*/
void fltk::drawimage(const uchar* pointer, fltk::PixelType type,
		     const Rectangle& r,
		     int delta, int line_delta) {
  innards(pointer, type, r, delta, line_delta, 0, 0);
}

/*!
  Same except \a line_delta is set to \a r.w() times \a delta, indicating
  the rows are packed together one after another with no gap.

  If you use fltk::RGB make sure your source data really is packed,
  if each row starts word-aligned then you must use the version where
  you pass the line_delta
*/
void fltk::drawimage(const uchar* pointer, fltk::PixelType type,
		     const Rectangle& r,
		     int delta) {
  innards(pointer, type, r, delta, delta*r.w(), 0, 0);
}

/*!
  Same except \a delta is set to the number of bytes used by \a type,
  and \a line_delta is set to \a r.w() times \a delta, indicating
  the rows are packed together one after another with no gap.

  If you use fltk::RGB make sure your source data really is packed,
  if each row starts word-aligned then you must use the version where
  you pass the line_delta
*/
void fltk::drawimage(const uchar* pointer, fltk::PixelType type,
		     const Rectangle& r) {
  innards(pointer, type, r, type&3, (type&3)*r.w(), 0, 0);
}

/*! \typedef fltk::DrawImageCallback

  Type of function passed to drawimage(). It must return a pointer
  to a horizontal row of \a w pixels, starting with the pixel at
  \a x and \a y (relative to the top-left corner of the image, not
  to the coordinate space drawimage() is called in). These pixels
  must be in the format described by \a type passed to drawimage()
  and must be the \a delta apart passed to drawimage().

  \a data is the same as the argument passed to drawimage().
  This can be used to point at a structure of information about
  the image.

  The passed \a buffer contains room for at least the number of
  pixels specified by the width passed to drawimage(). You can use
  this as temporary storage to construct a row of the image, and
  return a pointer to it (or offset \a x into it if desired).

  Due to cropping, less than the whole image may be requested. So the
  callback may get an \a x greater than zero, the first \a y passed to
  it may be greater than zero, and \a x+w may be less than the width
  of the image. The passed buffer is long enough to store the entire
  \a w * \a delta bytes, this is for convienence with some
  decompression schemes where you must decompress the entire line at
  once: decompress it into the buffer, and then if x is not zero,
  shift the data over so the x'th pixel is at the start of the buffer.

*/

/*!

  Call the passed function to provide each scan line of the
  image. This lets you generate the image as it is being drawn, or do
  arbitrary decompression of stored data (provided it can be
  decompressed to individual scan lines easily).

  \a callback is called with the void* \a data argument (this can be
  used to point at a structure of information about the image), the x,
  y, and number of pixels desired from the image, measured from the
  upper-left corner of the image. It is also given a buffer of at
  least \a w pixels that can be used as temporary storage, for
  instance to decompress a line read from a file. You can then return
  a pointer to this buffer, or to somewhere inside it.

  The callback must return n pixels of the format described by \a
  type, \a delta apart from each other.

  The \a xywh rectangle describes the area to draw. The callback is
  called with y values between \a y and \a y+h-1. Due to cropping not
  all pixels may be asked for. You can assumme y will be asked for in
  increasing order.
*/
void fltk::drawimage(DrawImageCallback cb,
		     void* data, fltk::PixelType type,
		     const Rectangle& r, int delta) {
  innards(0, type, r, delta, 0, cb, data);
}

/*! Same except the \a delta is figured out from the \a type. */
void fltk::drawimage(DrawImageCallback cb,
		     void* data, fltk::PixelType type,
		     const Rectangle& r) {
  innards(0, type, r, depth(type), 0, cb, data);
}

#if 0
// obsolete method that used the image dithering to get better color
// chips on 8-bit displays.
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
// End of "$Id$".
//
