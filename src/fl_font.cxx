//
// "$Id: fl_font.cxx,v 1.29 2001/07/23 09:50:05 spitzak Exp $"
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

// Static variables containing the currently selected font & size:
Fl_Font fl_font_;
unsigned fl_size_;

void
fl_font(Fl_Font f, unsigned s) { fl_font_renderer->font(f, s); }

void
fl_draw(const char* str, int n, int x, int y) {
  fl_font_renderer->draw(str, n, x, y);
}

void
fl_draw(const char* str, int x, int y) { fl_draw(str, strlen(str), x, y); }

int
fl_height() { return fl_font_renderer->height(); }

int
fl_descent() { return fl_font_renderer->descent(); }

int
fl_width(const char* c, int n) { return fl_font_renderer->width(c, n); }

int
fl_width(const char* c) { return fl_width(c, strlen(c)); }

int
fl_width(uchar c) { return fl_width((char *)&c, 1); }

#ifdef WIN32
#include "fl_font_win32.cxx"
#else
#include "fl_font_x.cxx"
#endif

//
// End of "$Id: fl_font.cxx,v 1.29 2001/07/23 09:50:05 spitzak Exp $".
//
