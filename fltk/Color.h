//
// "$Id: Color.h,v 1.3 2003/01/15 07:55:19 spitzak Exp $"
//
// Color value. These are 32-bit unsigned numbers with RGB as the
// upper 3 bytes. The lowest-order byte is treated as an "index"
// for back compatabilty, or as an "alpha", depending on context.
//
// Copyright 2002 by Bill Spitzak and others.
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

#ifndef fltk_Color_h
#define fltk_Color_h

#include "FL_API.h"
#include "Flags.h"

namespace fltk {

typedef unsigned Color;

enum {
  NO_COLOR	= 0,

  // The gray ramp. The values shown here are the defaults. But
  // calling set_background() (which a theme will often do) will alter
  // this to a smooth curve such that the entry for GRAY75 is the
  // default background color.
  //
  // Notice that actual luminance is approximatly the decimal
  // value squared.
  //
  // There is a history of hard-to-remember names for these. For
  // fltk 2.0 I provide a symbol for all multiples of 5% and 33%,
  // using the closest available color for each. This works out
  // quite well.
		      // Hex   Decimal FrameBox Old names
  GRAY00	= 32, // 00	.000	A	GRAY0 GRAY_RAMP
  GRAY05	= 33, // 0d	.050	B
  GRAY10	= 34, // 1a	.101	C
  GRAY15	= 35, // 26	.149	D
  GRAY20	= 36, // 31	.192	E
  GRAY25	= 37, // 3d	.239	F
  GRAY30	= 38, // 48	.282	G
  GRAY33	= 39, // 55	.333	H	DARK3
  GRAY35	= 40, // 5f	.372	I
  GRAY40	= 41, // 6a	.416	J	18% gray
  GRAY45	= 42, // 75	.459	K
  GRAY50	= 43, // 80	.502	L
  GRAY55	= 44, // 8a	.541	M
  GRAY60	= 45, // 95	.584	N	DARK2
  GRAY65	= 46, // a0	.627	O
  GRAY66	= 47, // aa	.666	P	DARK1 INACTIVE_COLOR
  GRAY70	= 48, // b5	.710	Q
  GRAY75	= 49, // c0	.753	R	GRAY SELECTION_COLOR
  GRAY80	= 50, // cb	.796	S
  GRAY85	= 51, // d5	.835	T	LIGHT1
		      // e0	.878	U
  GRAY90	= 53, // ea	.917	V	LIGHT2
  GRAY95	= 54, // f5	.961	W
  GRAY99	= 55, // ff    1.000	X	LIGHT3

  // color cube corners:
  BLACK		= 0x38,
  RED		= 0x58,
  GREEN		= 0x3f,
  YELLOW	= 0x5f,
  BLUE		= 0xd8,
  MAGENTA	= 0xf8,
  CYAN		= 0xdf,
  WHITE		= 0xff,

  WINDOWS_BLUE	= 0x88 // default selection_color
};

inline Color color(unsigned char r, unsigned char g, unsigned char b) {
  return Color((r<<24)+(g<<16)+(b<<8));}
inline Color color(unsigned char g) {
  return Color(g*0x1010100u);}
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

}

#endif

//
// End of "$Id: Color.h,v 1.3 2003/01/15 07:55:19 spitzak Exp $".
//
