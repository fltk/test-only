//
// "$Id: Fl_FontSize.H,v 1.2 1999/08/20 08:32:26 bill Exp $"
//
// Font definitions for the Fast Light Tool Kit (FLTK).
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

// Two internal fltk data structures:
//
// Fl_Fontdesc: an entry into the fl_font() table.  There is one of these
// for each fltk font number.
//
// Fl_FontSize: a structure for an actual system font, with junk to
// help choose it and info on character sizes.  Each Fl_Fontdesc has a
// linked list of these.  These are created the first time each system
// font/size combination is used.

#ifndef FL_FONT_
#define FL_FONT_

#include <FL/Fl_Font.H>
#include <FL/x.H>

class Fl_FontSize {
public:
  Fl_FontSize *next;	// linked list for this Fl_Fontdesc
#ifndef WIN32
  const char* encoding;
  XFontStruct* font;	// X font information
  FL_EXPORT Fl_FontSize(const char* xfontname);
#else
  HFONT fid;
  int width[256];
  TEXTMETRIC metr;
  FL_EXPORT Fl_FontSize(const char* fontname, int size);
#endif
  unsigned minsize;	// smallest point size that should use this
  unsigned maxsize;	// largest point size that should use this
#if HAVE_GL
  unsigned int listbase;// base of display list, 0 = none
#endif
  FL_EXPORT ~Fl_FontSize();
};

#ifndef WIN32
// functions for parsing X font names:
FL_EXPORT const char* fl_font_word(const char *p, int n);
FL_EXPORT char *fl_find_fontsize(char *name);
#endif

#endif

//
// End of "$Id: Fl_FontSize.H,v 1.2 1999/08/20 08:32:26 bill Exp $".
//
