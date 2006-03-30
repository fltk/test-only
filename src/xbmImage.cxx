// "$Id$"
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

/*! \class fltk::xbmImage

  Image based on a 1-bit bitmap from memory. This matches the very
  first type of image provided with X10 in 1980 or so, and unfortunately
  the only one that draws with any efficiency even today...

  Each byte in the bitmap determines 8 pixels, a 1 bit is opaque and
  a 0 bit is transparent. The high-order bit is the left-most (this
  is inverted from similar data used by Windows). If the width is
  not a multiple of 8 each line starts at the start of the next byte.
*/

// Implementation is in system/Image.cxx!

//
// End of "$Id$".
//
