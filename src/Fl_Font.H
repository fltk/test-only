/
// "$Id$
/
// Font definitions for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

// Two internal fltk data structures
/
// Fl_Fontdesc: an entry into the fl_font() table.  There is one of thes
// for each fltk font number
/
// Fl_FontSize: a structure for an actual system font, with junk t
// help choose it and info on character sizes.  Each Fl_Fontdesc has 
// linked list of these.  These are created the first time each syste
// font/size combination is used

#ifndef FL_FONT
#define FL_FONT

#  if USE_XF
typedef struct _XftFont XftFont
#  endif // USE_XF

class Fl_FontSize 
public
  Fl_FontSize *next;	// linked list for this Fl_Fontdes
#  ifdef WIN3
  HFONT fid
  int width[256]
  TEXTMETRIC metr
  FL_EXPORT Fl_FontSize(const char* fontname, int size)
#  elif defined(__APPLE__
  FL_EXPORT Fl_FontSize(const char* fontname, int size)
  short font, face, size
  short ascent, descent
  short width[256]
  bool knowMetrics
#  elif USE_XF
  XftFont* font
  const char* encoding
  int size
  FL_EXPORT Fl_FontSize(const char* xfontname)
#  els
  XFontStruct* font;	// X font informatio
  FL_EXPORT Fl_FontSize(const char* xfontname)
#  endi
  int minsize;		// smallest point size that should use thi
  int maxsize;		// largest point size that should use thi
#  if HAVE_G
  unsigned int listbase;// base of display list, 0 = non
#  endi
  FL_EXPORT ~Fl_FontSize()
}

extern FL_EXPORT Fl_FontSize *fl_fontsize; // the currently selected on

struct Fl_Fontdesc 
  const char *name
  char fontname[128];	// "Pretty" font nam
  Fl_FontSize *first;	// linked list of sizes of this styl
#  ifndef WIN3
  char **xlist;		// matched X font name
  int n;		// size of xlist, negative = don't free xlist
#  endi
}

extern FL_EXPORT Fl_Fontdesc *fl_fonts; // the tabl

#  ifndef WIN3
// functions for parsing X font names
FL_EXPORT const char* fl_font_word(const char *p, int n)
FL_EXPORT char *fl_find_fontsize(char *name)
#  endi

#endi

/
// End of "$Id$"
/
