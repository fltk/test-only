//
// "$Id$"
//
// Carbon line style code for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include "../flstring.h"
#include <stdio.h>
#include "Fl_Carbon_Display.H"

void Fl_Carbon_Display::line_style(int style, int width, char* dashes) {

  // QuickDraw supports pen size and pattern, but no arbitrary line styles.
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
// End of "$Id$".
//
