// "$Id: PixelType.h,v 1.1 2005/04/02 03:20:15 spitzak Exp $"
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

#ifndef fltk_PixelType_h
#define fltk_PixelType_h

namespace fltk {
/*! \addtogroup images
  \{ */

/**
  Enumeration describing how colors are stored in an array of bytes
  that is a pixel. This is used as an argument for fltk::drawimage(),
  fltk::readimage(), and fltk::Symbol::readimage().

  The bytes are described in the order they are in memory: RGB means
  red is stored at the first memory address, green at the second, and
  blue at the third. Most image processing software uses this scheme,
  since it is easily extended to data larger than bytes. However be
  warned that some graphics software written for little-endian machines
  may describe the bytes in the opposite order, as they are describing
  the bytes in a 4-byte integer.
*/
enum PixelType {
  // Commented out ones are nyi and will only be done as people need them
  MASK	= 0,	//!< 1 byte: 0xff = fltk::getcolor(), 0 = transparent
  LUMINANCE=1,	//!< 1 byte: 0xff = white, 0 = black
  //BGR = 2,	//!< 3 bytes: b,g,r
  RGB	= 3, 	//!< 3 bytes: r,g,b
  RGBA	= 4	//!< 4 bytes: r,g,b,a.
  //ABGR= 5,	//!< 4 bytes: a,b,g,r.
  //ARGB= 6,	//!< 4 bytes: a,r,g,b.
  //BGRA= 7,	//!< 4 bytes: b,g,r,a.
  //RGBM=8,	//!< 4 bytes: r,g,b,a. Unpremultiplied (M means "mask")
  //MBGR=9,	//!< 4 bytes: a,b,g,r. Unpremultiplied.
  //MRGB=10,	//!< 4 bytes: a,r,g,b. Unpremultiplied.
  //BGRM=11,	//!< 4 bytes: b,g,r,a. Unpremultiplied.
};

/*! \} */

}

#endif
