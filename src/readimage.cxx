//
// "$Id: readimage.cxx,v 1.1 2005/02/05 00:26:10 spitzak Exp $"
//
// Copyright 1998-2002 by Bill Spitzak and others.
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
#include <fltk/draw.h>

/** Reads a 2-D image off the current drawing destination. The
    resulting data can be passed to fltk::drawimage() or the 8-bit
    pixels examined or stored by your program.

    The return value is either \a p or NULL if there is some problem
    (such as an inability to read from the current output surface, or
    if the rectangle is empty).

    \a p points to the location to store the first byte of the
    upper-left pixel of the image.

    \a type can be fltk::RGB or fltk::RGBA (possibly other types will
    be supported in the future).

    \a rectangle indicates the position on the surface in the current
    transformation to read from and the width and height of the resulting
    image. What happens when the current transformation is rotated or
    scaled is undefined. If the rectangle extends outside the current
    drawing surface, or into areas obscured by overlapping windows, the
    result in those areas is undefined.
*/
uchar* fltk::readimage(uchar* p, PixelType type, const Rectangle& r) {
  return fltk::readimage(p, type, r, type&3);
}

/** Same except \a delta is how much to add to a pointer into the
    buffer to go to the next pixel. This can be used to skip over
    extra data for each pixel, for instance to read RGB data into
    a 4-byte/pixel buffer. The skipped pixels are left with undefined
    values, do not assumme they are unchanged!
*/
uchar* fltk::readimage(uchar* p, PixelType type, const Rectangle& r, int delta) {
  if (r.empty()) return 0;
  return fltk::readimage(p, type, r, delta, delta*r.w());
}

/** \fn uchar *fltk::readimage(uchar* p, PixelType type, const Rectangle&rectangle, int delta, int linedelta);

    \a p points to the location to store the first byte of the
    upper-left pixel of the image. The caller must allocate this,
    it cannot be NULL.

    \a delta is how much to add to a pointer to advance from one pixel
    to the one to it's right. Any bytes skipped over are left with
    undefined values in them.

    \a linedelta is how much to add to a pointer to advance from one
    pixel to the one below it. Any bytes skipped over are left with
    undefined values in them.

    Negative values for \a delta and \a linedelta can be used to
    produce mirror images or 90 degree rotations of the data.
*/

#if USE_X11
# include "x11/readimage.cxx"
#elif defined(_WIN32)
# include "win32/readimage.cxx"
#elif defined(__APPLE__)
# include "osx/readimage.cxx"
#else

// Function returns null to indicate failure on unsupported platforms:
uchar* fltk::readimage(uchar *, PixelType, const Rectangle&, int,int) {return 0;}

#endif
