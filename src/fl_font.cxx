//
// "$Id: fl_font.cxx,v 1.38 2002/09/18 05:51:46 spitzak Exp $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <fltk/Fl_Font.h>
#include <fltk/fl_draw.h>

// Static variables containing the currently selected font, size, encoding:
Fl_Font fl_font_;
float fl_size_;
const char *fl_encoding_ = "iso8859-1";
  
#ifdef _WIN32
# include "fl_font_win32.cxx"
#elif (defined(__APPLE__) && !USE_X11)
# include "fl_font_mac.cxx"
#else
# if USE_XFT
#  include "fl_font_xft.cxx"
# else
#  include "fl_font_x.cxx"
# endif
#endif

void
fl_draw(const char* str, float x, float y) {
  fl_draw(str, strlen(str), x, y);
}

void
fl_draw(const char* str, int n, float x, float y) {
  fl_transform(x,y);
  fl_transformed_draw(str, n, x, y);
}

float
fl_width(const char* c) { return fl_width(c, strlen(c)); }

//
// End of "$Id: fl_font.cxx,v 1.38 2002/09/18 05:51:46 spitzak Exp $".
//
