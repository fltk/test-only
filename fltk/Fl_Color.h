//
// "$Id: Fl_Color.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Color value used by the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Color_h
#define Fl_Color_h

#include "Enumerations.h" // for FL_API and uchar
#include "Fl_Flags.h" // for Fl_Flags & FL_INACTIVE

/**
   Fl_Color is an unsigned integer. 
   The (low) 4 bytes of the color are rgbN. 
   (r is the byte with the highest value, not the first in memory.
   On an Intel or other little-endian machine the first byte in
   memory is N, while on a big-endian machine such as a MIPS it is r) 

   If the rgb is zero, the N is the color "index". This index is 
   used to look up an Fl_Color in an internal table of 255 colors, 
   called the "fltk colormap", shown here. Thus using any integer in 
   the range 1-255 as an Fl_Color will index the colormap. If rgb is 
   not zero the index value is ignored (future code may treat the last
   byte as an "alpha" value). 

   An Fl_Color of zero (FL_NO_COLOR) will draw black but is ambiguous. 
   It is returned as an error value or to indicate portions of a Fl_Style
   that should be inherited, and it is also used as the default label color
   for everything so that changing color zero can be used by the -fg switch.
   You should use FL_BLACK (56) to get black. 

   The entries 1-31 in the colormap are settable by the user program. The 
   advantage of using these over fl_rgb(r,g,b) is that they are reproduced 
   exactly on 8-bit screens (normal rgb colors are selected on 8-bit screens
   by using fl_nearest_color()). Colors 1-15 are preset for back compatability
   but fltk no longer uses these so you can change them. 

   Entries 32-55 of the colormap are a 24-entry "gray ramp". This ramp is 
   modified by the -bg switch or by other code that calls fl_background() so
   that the color FL_GRAY (49) is the background color, and the others are a 
   nice range from black to white. These are used to draw box edges. 

   The remainder of the colormap is a 5x8x5 color cube. This cube is used to 
   dither images or fl_rectf() on 8-bit screens. In addition the 
   symbols FL_BLACK, FL_RED, FL_GREEN, FL_YELLOW, FL_BLUE, FL_MAGENTA, 
   FL_CYAN, FL_WHITE, and FL_BLUE_SELECTION_COLOR index the corners of 
   the cube (these have different values than fltk1.0!). 

*/
typedef unsigned Fl_Color;

enum {
  FL_NO_COLOR		= 0,
  FL_GRAY_RAMP		= 32,
  FL_GRAY		= 49,	// 'R' default color, 75%
  FL_INACTIVE_COLOR	= 39,	// same as fl_inactive(FL_BLACK)
  FL_COLOR_CUBE		= 0x38,
  FL_BLACK		= 0x38,
  FL_RED		= 0x58,
  FL_GREEN		= 0x3f,
  FL_YELLOW		= 0x5f,
  FL_BLUE		= 0xd8,
  FL_MAGENTA		= 0xf8,
  FL_CYAN		= 0xdf,
  FL_WHITE		= 0xff,
#ifndef FLTK_2
  FL_GRAY0		= 32,	// 'A' black
  FL_DARK3		= 39,	// 'H' about 33%
  FL_DARK2		= 45,   // 'N'
  FL_DARK1		= 47,	// 'P' about 66%
  FL_LIGHT1		= 51,	// 'T'
  FL_LIGHT2		= 53,	// 'V'
  FL_LIGHT3		= 55,	// 'X' white
  FL_SELECTION_COLOR	= FL_GRAY, // older selection color
  FL_FREE_COLOR		= 16,
#endif
  FL_BLUE_SELECTION_COLOR = 0x88 // _WIN32-like selection color
};

#define FL_NUM_GRAY	24
inline Fl_Color fl_gray_ramp(int i) {return (Fl_Color)(i+FL_GRAY_RAMP);}

#define FL_NUM_GREEN	8
#define FL_GREEN_DELTA	1
#define FL_NUM_RED	5
#define FL_RED_DELTA	8
#define FL_NUM_BLUE	5
#define FL_BLUE_DELTA	40
inline Fl_Color fl_color_cube(int r, int g, int b) {
  return (Fl_Color)((b*FL_NUM_RED + r) * FL_NUM_GREEN + g + FL_COLOR_CUBE);}
/**
   Inline function to build a color out of individual bytes. 
   The index is set to zero.
*/
inline Fl_Color fl_rgb(unsigned char r, unsigned char g, unsigned char b) {
  return Fl_Color((r<<24)+(g<<16)+(b<<8));}
/**
   Return a named color, returns 0 if the name is not found (on X this
   does Xlookupcolor, on Windows it can only do colors of the form #123456). 
*/
FL_API Fl_Color fl_rgb(const char*);
/**
   Returns a color that is weight*A+(1-weight)*B.
*/
FL_API Fl_Color fl_color_average(Fl_Color, Fl_Color, double weight);
/**
   Returns some color between c and FL_GRAY.
*/
FL_API Fl_Color fl_inactive(Fl_Color c);
/**
   This returns fl_inactive(c) if the bit FL_INACTIVE is turned on in f,
   otherwise c is returned unchanged.
*/
FL_API Fl_Color fl_inactive(Fl_Color c, Fl_Flags f);
/**
   Decides if fg can be seen against bg and returns fg if so. 
   If not this returns either FL_NO_COLOR or FL_WHITE, 
   whichever one is more different than bg. This function is often
   used by fltk for labels and text when no color is specified in the style. 
*/
FL_API Fl_Color fl_contrast(Fl_Color fg, Fl_Color bg);
#ifndef FLTK_2
#define contrast(a,b) fl_contrast(a,b)
#define inactive(a) fl_inactive(a)
#endif
/**
   Turns an indexed color into an rgb color by looking it up in the colormap.
   If c is not an indexed color it is returned unchanged. 
*/
FL_API Fl_Color fl_get_color(Fl_Color c);
/**
   Turns an indexed color into an rgb color if necessary, and then the
   rgb portions of the color are extracted and put in the passed variables. 
*/
FL_API void fl_get_color(Fl_Color c, uchar& r, uchar& g, uchar& b);
/**
   Set entry index in the fltk colormap to the rgb values of v. index 
   must be in the range 1-31. 
*/
FL_API void fl_set_color(Fl_Color c, Fl_Color);
/**
   Tell a colormapped-display that we don't need this indexed color anymore.
   Fltk will free any hardware colormap it allocated so that it can be reused
   by this or another program. 
*/
FL_API void fl_free_color(Fl_Color c);
/**
   Modifies the "gray ramp" in the fltk color map so that the color 
   FL_GRAY is set the the rgb values of the passed color. This is the
   best way to change the overall color of your application, because 
   it sets all the shaded borders and works on 8-bit screens. 
*/
FL_API void fl_background(Fl_Color);
/**
   Turns an rgb color into an indexed color. If c is already an indexed 
   color it is returned unchanged. Otherwise, the closest color from the 
   color cube is returned. 
*/
FL_API Fl_Color fl_nearest_color(Fl_Color);

#endif

//
// End of "$Id: Fl_Color.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
