//
// "$Id$"
//
// Xlib font selection code for the Fast Light Tool Kit (FLTK).
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

// Select fonts from the FLTK font table.
#include "../flstring.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include "../Fl_Font.H"

#include <stdio.h>
#include <stdlib.h>

#  if USE_XFT
#    include "font_xft.cxx"
#  else
#    include "font_x.cxx"
#  endif // USE_XFT

char *fl_get_font_xfld(char *buffer, int bufsize, int fnum, int size) {
  static const char *fonts[] = {
    "-*-helvetica-medium-r-normal--",
    "-*-helvetica-bold-r-normal--",
    "-*-helvetica-medium-o-normal--",
    "-*-helvetica-bold-o-normal--",
    "-*-courier-medium-r-normal--",
    "-*-courier-bold-r-normal--",
    "-*-courier-medium-o-normal--",
    "-*-courier-bold-o-normal--",
    "-*-times-medium-r-normal--",
    "-*-times-bold-r-normal--",
    "-*-times-medium-i-normal--",
    "-*-times-bold-i-normal--",
    "-*-symbol-",
    "-*-lucidatypewriter-medium-r-normal-sans-",
    "-*-lucidatypewriter-bold-r-normal-sans-",
    "-*-*zapf dingbats-"
  };

  if (fnum < 0 || fnum >= (int)(sizeof(fonts) / sizeof(fonts[0]))) fnum = 0;

  snprintf(buffer, bufsize,
           "%s%d-*-*-*-*-*-iso10646-1,%s%d-*-*-*-*-*-iso8859-1",
           fonts[fnum], size, fonts[fnum], size);

//  printf("fl_get_font_xfld returning \"%s\"...\n", buffer);

  return (buffer);
}

//
// End of "$Id$".
//
