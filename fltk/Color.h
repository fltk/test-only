//
// "$Id$"
//
// Color value. These are 32-bit unsigned numbers with RGB as the
// upper 3 bytes. The lowest-order byte is treated as an "index"
// for back compatabilty, or as an "alpha", depending on context.
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
// Please report all bugs and problems to:
//
//     http://www.fltk.org/str.php
//

#ifndef fltk_Color_h
#define fltk_Color_h

#include "FL_API.h"
#include "Flags.h"

namespace fltk {

/*! \addtogroup color
  \{ */

typedef unsigned Color;

/*! Symbolic names for some of the indexed colors.

  The 24-entry "gray ramp" is modified by fltk::set_background() so
  that the color fltk::GRAY75 is the background color, and the others
  are a nice range from black to white. These are used to draw box
  edges. The gray levels are chosen to be evenly spaced, listed here
  is the actual 8-bit and decimal gray level assigned by default.
  Also listed here is the letter used for fltk::FrameBox and the old
  fltk1.1 names used for these levels.

  The remiander of the colormap is a 5x8x5 color cube. This cube is
  used to dither images on 8-bit screens X colormaps to reduce the
  number of colors used.
*/
enum {
  NO_COLOR	= 0, /*!< Black, empty place holder in Style */

  FREE_COLOR	= 16, /*!< Starting from index 16 is the FREE_COLOR area */
  NUM_FREE_COLOR= 16, /*!< Number of free color slots starting from index FREE_COLOR */

		      // Hex   Decimal FrameBox Old names
  GRAY00	= 32, /*!< 00	.00	A       fltk1: GRAY0 GRAY_RAMP */
  GRAY05	= 33, /*!< 0d	.05	B */
  GRAY10	= 34, /*!< 1a	.10	C */
  GRAY15	= 35, /*!< 26	.15	D */
  GRAY20	= 36, /*!< 31	.19	E */
  GRAY25	= 37, /*!< 3d	.24	F */
  GRAY30	= 38, /*!< 48	.28	G */
  GRAY33	= 39, /*!< 55	.33	H	fltk1: DARK3 */
  GRAY35	= 40, /*!< 5f	.37	I */
  GRAY40	= 41, /*!< 6a	.42	J	(18% gray card) */
  GRAY45	= 42, /*!< 75	.46	K */
  GRAY50	= 43, /*!< 80	.50	L */
  GRAY55	= 44, /*!< 8a	.54	M */
  GRAY60	= 45, /*!< 95	.58	N	fltk1: DARK2 */
  GRAY65	= 46, /*!< a0	.63	O */
  GRAY66	= 47, /*!< aa	.67	P	fltk1: DARK1 INACTIVE_COLOR */
  GRAY70	= 48, /*!< b5	.71	Q */
  GRAY75	= 49, /*!< c0	.75	R	fltk1: GRAY SELECTION_COLOR */
  GRAY80	= 50, /*!< cb	.80	S */
  GRAY85	= 51, /*!< d5	.84	T	fltk1: LIGHT1 */
  //unnamed entry	   e0	.88	U
  GRAY90	= 53, /*!< ea	.92	V	fltk1: LIGHT2 */
  GRAY95	= 54, /*!< f5	.96	W */
  GRAY99	= 55, /*!< ff  1.00	X	fltk1: LIGHT3 */

  BLACK		= 0x38, /*!< Corner of color cube */
  RED		= 0x58, /*!< Corner of color cube */
  GREEN		= 0x3f, /*!< Corner of color cube */
  YELLOW	= 0x5f, /*!< Corner of color cube */
  BLUE		= 0xd8, /*!< Corner of color cube */
  MAGENTA	= 0xf8, /*!< Corner of color cube */
  CYAN		= 0xdf, /*!< Corner of color cube */
  WHITE		= 0xff, /*!< Corner of color cube */

  WINDOWS_BLUE	= 0x88 /*!< default selection_color */
};

inline Color color(unsigned char r, unsigned char g, unsigned char b) {
  return Color((r<<24)+(g<<16)+(b<<8)); }
inline Color color(unsigned char g) {
  return Color(g*0x1010100u); }
FL_API Color color(const char*);
FL_API Color lerp(Color c0, Color c1, float f);
FL_API Color inactive(Color);
FL_API Color inactive(Color, Flags f);
FL_API Color contrast(Color fg, Color bg);
FL_API void split_color(Color c, unsigned char& r, unsigned char& g, unsigned char& b);
FL_API void set_color_index(Color index, Color);
FL_API Color get_color_index(Color index);
FL_API void set_background(Color);
FL_API Color nearest_index(Color);

/* \} */
}

#endif

//
// End of "$Id$".
//
