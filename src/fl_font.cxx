//
// "$Id: fl_font.cxx,v 1.41 2002/12/10 02:01:00 easysw Exp $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <fltk/Font.h>
#include <fltk/draw.h>

// Static variables containing the currently selected font, size, encoding:
fltk::Font* fltk::current_font_;
float fltk::current_size_;
const char *fltk::encoding_ = "iso8859-1";
  
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
fltk::drawtext(const char* str, float x, float y) {
  drawtext(str, strlen(str), x, y);
}

void
fltk::drawtext(const char* str, int n, float x, float y) {
  transform(x,y);
  drawtext_transformed(str, n, x, y);
}

float
fltk::getwidth(const char* c) { return getwidth(c, strlen(c)); }

// turn a stored font name into a pretty name:
const char* fltk::Font::name() const {
  if (!attributes_) return name_;
  static char *buffer; if (!buffer) buffer = new char[128];
  strcpy(buffer, name_);
  if (attributes_ & BOLD) strcat(buffer, " bold");
  if (attributes_ & ITALIC) strcat(buffer, " italic");
  return buffer;
}

//
// End of "$Id: fl_font.cxx,v 1.41 2002/12/10 02:01:00 easysw Exp $".
//
