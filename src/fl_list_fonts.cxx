//
// "$Id: fl_list_fonts.cxx,v 1.11 2001/07/18 19:39:24 clip Exp $"
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#include <FL/fl_draw.H>

#ifdef WIN32
#include "fl_list_fonts_win32.cxx"
#else
#include "fl_list_fonts_x.cxx"
#endif

Fl_Font fl_find_font(const char* name, int attributes /* = 0 */) {
  if (!name || !*name) return 0;
  // find out if the " bold" or " italic" are on the end:
  int length = strlen(name);
  if (length > 7 && !strncasecmp(name+length-7, " italic", 7)) {
    length -= 7; attributes |= FL_ITALIC;
  }
  if (length > 5 && !strncasecmp(name+length-5, " bold", 5)) {
    length -= 5; attributes |= FL_BOLD;
  }
  Fl_Font font = 0;
  // always try the built-in fonts first, becasue fl_list_fonts is *slow*...
  int i; for (i = 0; i < 16; i += (i < 12 ? 4 : 1)) {
    font = fl_fonts()+i;
    const char* fontname = font->name();
    if (!strncasecmp(name, fontname, length) && !fontname[length]) goto GOTIT;
  }
  // now try all the fonts on the server, using a binary search:
  font = 0;
  {Fl_Font* list; int b = fl_list_fonts(list); int a = 0;
  while (a < b) {
    int c = (a+b)/2;
    Fl_Font testfont = list[c];
    const char* fontname = testfont->name();
    int d = strncasecmp(name, fontname, length);
    if (!d) {
      // If we match a prefix of the font return it unless a better match found
      font = testfont;
      if (!fontname[length]) goto GOTIT;
    }
    if (d > 0) a = c+1;
    else b = c;
  }}
  if (!font) return 0;
 GOTIT:
  if (attributes & FL_BOLD) font = font->bold_;
  if (attributes & FL_ITALIC) font = font->italic_;
  return font;
}

//
// End of "$Id: fl_list_fonts.cxx,v 1.11 2001/07/18 19:39:24 clip Exp $".
//
