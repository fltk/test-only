//
// "$Id: Fl_Style.H,v 1.32 2000/06/10 04:10:20 carl Exp $"
//
// Style structure used by Fl_Widgets
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

#ifndef Fl_Style_h
#define Fl_Style_h

#include "Enumerations.H"
//#include "Fl_Color.H"
#include "Fl_Boxtype.H"
#include "Fl_Labeltype.H"
#include "Fl_Font.H"

typedef void (*Fl_Glyph)(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype = FL_UP_BOX);

struct FL_API Fl_Named_Style;

struct FL_API Fl_Style {
  Fl_Boxtype	box;
  Fl_Boxtype	text_box;
  Fl_Glyph	glyph;
  Fl_Font	label_font;
  Fl_Font	text_font;
  Fl_Labeltype	label_type;
  Fl_Color	color;
  Fl_Color	text_background;
  Fl_Color	label_color;
  Fl_Color	selection_color;
  Fl_Color	selection_text_color;
  Fl_Color	highlight_color;
  Fl_Color	highlight_label_color;
  Fl_Color	text_color;
  unsigned	label_size;
  unsigned	text_size;
  unsigned	leading;

  static int	draw_boxes_inactive;
  static int	scrollbar_width;
  static int	scrollbar_align;

  Fl_Style* parent;

  void  (*revertfunc)(Fl_Style*);
  int dynamic() const {return !revertfunc;}

  Fl_Style(); // creates a dynamic() style

  static void revert();                 // use FLTK default style
  static void start(const char* name);	// start a new class of style or use a previously started one

  // find a default style by its string ID
  static Fl_Named_Style* find(const char* name);
};

FL_API void fl_glyph(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);

enum Fl_Glyphtype {	// glyph types understood by fl_glyph()
  // the numbers 0-99 are reserved for private glyph functions
  FL_GLYPH_DEFAULT = 100,
  FL_GLYPH_CHECK,
  FL_GLYPH_ROUND,
  FL_GLYPH_UP,
  FL_GLYPH_DOWN,
  FL_GLYPH_LEFT,
  FL_GLYPH_RIGHT,
  FL_GLYPH_VSLIDER,
  FL_GLYPH_HSLIDER,
  FL_GLYPH_VNSLIDER,
  FL_GLYPH_HNSLIDER,
  FL_GLYPH_LIGHT,
  FL_GLYPH_CHOICE
};

struct FL_API Fl_Named_Style : public Fl_Style {
  const char* name;
  Fl_Named_Style* next;
  static Fl_Named_Style* first;
  Fl_Named_Style** back_pointer; // used by Fl_Style::start
  Fl_Named_Style(const char* name, void (*revert)(Fl_Style*), Fl_Named_Style**);
};

#endif

//
// End of "$Id: Fl_Style.H,v 1.32 2000/06/10 04:10:20 carl Exp $".
//
