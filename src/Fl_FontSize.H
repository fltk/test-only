//
// "$Id: Fl_FontSize.H,v 1.7 2001/02/16 22:55:45 robertk Exp $"
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

// An Fl_FontSize is a structure describing an actual system font.  On
// both X and Win32 this is a combination of an Fl_Font and a specific
// point size and a specific encoding.  These are created the first
// time each combination is used.

// This is a private object and it is not necessary to replicate this
// on other systems.

#ifndef FL_FONT_
#define FL_FONT_

#include <FL/Fl_Font.H>
#include <FL/x.H>

class FL_API Fl_FontSize {
public:
  Fl_FontSize *next;	// linked list for this Fl_Fontdesc
#ifdef WIN32
  int encoding;
  HFONT fid;
  int width[256];
  TEXTMETRIC metr;
  Fl_FontSize(const char* fontname, int size, int encoding);
#else
  const char* encoding;
  XFontStruct* font;
  Fl_FontSize(const char* xfontname);
#endif
  unsigned minsize;	// smallest point size that should use this
  unsigned maxsize;	// largest point size that should use this
#if HAVE_GL
  unsigned int listbase;// base of display list, 0 = none
#endif
  ~Fl_FontSize();
};

extern FL_API Fl_FontSize *fl_fontsize;

#endif

//
// End of "$Id: Fl_FontSize.H,v 1.7 2001/02/16 22:55:45 robertk Exp $".
//
