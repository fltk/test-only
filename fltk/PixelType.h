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

  The types are described by the order the bytes are in memory. Thus
  RGB means R is stored at a byte with an address of one less than
  where G is stored. Warning: some Windows and X software describes
  the pixels backwards from this. I use this standard because it
  works for arrays of data larger than bytes.

  The Mask types are not supported except on OS/X, they render the
  same as the premultiplied versions on other platforms. This may be
  fixed in the future.
*/
enum PixelType {
  MASK	= 0,	//!< 1 byte: 0xff = fltk::getcolor(), 0 = transparent
  MONO	= 1,	//!< 1 byte: 0xff = fltk::getbgcolor(), 0 = fltk::getcolor() ("inverted" so that a b&w image looks correct in black color)
  BGR 	= 2,	//!< often used by Windows software
  RGB	= 3, 	//!< normal method of storing 3-channel color
  RGBA	= 4,	//!< normal method of storing 4-channel color, array of fltk::Color on a big-endian machine
  ABGR	= 5,	//!< array of fltk::Color on a little-endian machine
  ARGB	= 6,	//!< some Windows software on a big-endian machine
  BGRA	= 7,	//!< often used by Windows software
  RGBM	= 8,	//!< RGBA but not premultiplied (M stands for "mask")
  MBGR	= 9,	//!< ABGR but not premultiplied
  MRGB	=10,	//!< ARGB but not premultiplied
  BGRM	=11	//!< BRGA but not premultiplied
};

/**
   Turn a PixelType into the number of bytes needed to hold a pixel.
*/
inline int depth(PixelType t) {return (t<2 ? 1 : t < 4 ? 3 : 4);}

/*! \} */

}

#endif
