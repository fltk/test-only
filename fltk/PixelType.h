// "$Id: PixelType.h,v 1.1 2005/04/02 03:20:15 spitzak Exp $"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

  Notice that the order of the bytes in memory of ARGB32 or RGB32 is
  a,r,g,b on a little-endian machine and b,g,r,a on a big-endian
  machine. Due to the use of these types by Windows, this is often
  the fastest form of data, if you have a choice. To convert an
  fltk::Color to RGB32, shift it right by 8 (for ARGB32 shift the
  alpha left 24 and or it in).

  More types may be added in the future. The set is as minimal as
  possible while still covering the types I have actually encountered.
*/
enum PixelType {
  UNDEFINED	= -1,//!< for dynamic Image construction purpose
  MASK	= 0,	//!< 1 byte of inverted mask, filled with current color
  MONO	= 1,	//!< 1 byte of gray scale
  RGB8	= 2,	//!< 1 byte of color scale with palette
  RGB15 = 3,	//!< 3x5 bits r/g/b + 1 bit unused
  RGB16	= 4,	//!< 16 bits r+g+b 
  RGB	= 5, 	//!< bytes in r,g,b,r,g,b... order
  RGBx	= 6,	//!< bytes in r,g,b,a,r,g,b,a... order, a byte is ignored
  RGBA	= 7,	//!< bytes in r,g,b,a,r,g,b,a... order
  RGB32 = 8,	//!< 32-bit words containiing 0xaarrggbb (aa is ignored)
  ARGB32= 9	//!< 32-bit words containing 0xaarrggbb
};
#define NBPIXELTYPES  10

/**
   Turn a PixelType into the number of bytes needed to hold a pixel.
*/
  //! depth in byte from a pixeltype
  inline int depth(PixelType t) {return (t<0? 0 : t<3? 1 : t<5? 2 : t==5? 3 : 4);}

  //! default pixeltype from bpp, note that it returns the most common value that may not be unique
  inline PixelType pixel_type_bpp(int bpp, bool hasPal=false, bool hasAlpha=false) {
      return bpp <9  ? hasPal ? RGB8 : MONO : bpp <16 ? bpp==15 ? RGB15 : RGB16 :
	    bpp <25 ? RGB : hasAlpha ? ARGB32 : RGB32;}
/*! \} */

}

#endif
