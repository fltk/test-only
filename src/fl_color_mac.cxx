//
// "$Id: fl_color_mac.cxx,v 1.7 2003/08/25 15:28:47 spitzak Exp $"
//
// MacOS color functions for the Fast Light Tool Kit (FLTK).
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

// Because carbon has a 'current color' in the drawing context this
// is really simple.

Color fltk::current_color_;

void fltk::setcolor(Color i) {
  current_color_ = i;
  // get fltk indexed color:
  if (!(i & 0xFFFFFF00)) i = cmap[i];
  // get the individual colors and put into Mac color structure:
  RGBColor rgb; 
  uchar r = i>>24;
  rgb.red   = (r<<8)|r;
  uchar g = i>>16;
  rgb.green = (g<<8)|g;
  uchar b = i>> 8;
  rgb.blue  = (b<<8)|b;
  RGBForeColor(&rgb);
}

// Used by setcolor_index
static void free_color(Color) {}

// This is here because stoopid Windows requires you to set the color
// and dash pattern in the same code, so they cannot be seperated.
// QuickDraw supports pen size and pattern, but no dash patterns, so
// dashes are faked with stippling patterns.
void fltk::line_style(int style, int width, char* dashes) {
  static Pattern	styles[] = {
    { { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } },	// FL_SOLID
    { { 0xf0, 0xf0, 0xf0, 0xf0, 0x0f, 0x0f, 0x0f, 0x0f } },	// FL_DASH
    { { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 } }	// FL_DOT
  };

  if (!width) width = 1;
  PenSize(width, width);

  style &= 0xff;
  if (style > 2) style = 2;
  PenPat(styles + style);
}

//
// End of "$Id: fl_color_mac.cxx,v 1.7 2003/08/25 15:28:47 spitzak Exp $".
//
