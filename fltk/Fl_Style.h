//
// "$Id: Fl_Style.h,v 1.4 2002/01/20 07:37:15 spitzak Exp $"
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

#include "Enumerations.h"
//#include "Fl_Color.h"
#include "Fl_Boxtype.h"
#include "Fl_Labeltype.h"
#include "Fl_Font.h"

class FL_API Fl_Widget;

typedef void (*Fl_Glyph)(const Fl_Widget*, int type,
			 int,int,int,int,
			 Fl_Flags);

struct FL_API Fl_Named_Style;

struct FL_API Fl_Style {
  Fl_Boxtype    box;
  Fl_Boxtype    button_box;
  Fl_Glyph      glyph;
  Fl_Font       label_font;
  Fl_Font       text_font;
  Fl_Labeltype  label_type;
  Fl_Color      color;
  Fl_Color      button_color;
  Fl_Color      label_color;
  Fl_Color      selection_color;
  Fl_Color      selection_text_color;
  Fl_Color      highlight_color;
  Fl_Color      highlight_label_color;
  Fl_Color      text_color;
  unsigned      label_size;
  unsigned      text_size;
  unsigned      leading;

  // various user preferences
  static bool   draw_boxes_inactive;
  static bool   draw_sliders_pushed;
  static unsigned scrollbar_width;
  static Fl_Flags scrollbar_align;
  static int	wheel_scroll_lines;

  const Fl_Style* parent;

  void  (*revertfunc)(Fl_Style*);
  bool dynamic() const {return !revertfunc;}

  Fl_Style(); // creates a dynamic() style

  static void revert();                 // use FLTK default style
  static void start(const char* name);	// start a new class of style or use a previously started one

  // find a default style by its string ID
  static Fl_Named_Style* find(const char* name);
};

FL_API void fl_glyph(const Fl_Widget*, int type,
		     int,int,int,int,
		     Fl_Flags);

enum Fl_Glyphtype {	// glyph types understood by fl_glyph()
  // the numbers 0-99 are reserved for private glyph functions
  FL_GLYPH_DEFAULT = 100,
  FL_GLYPH_CHECK,
  FL_GLYPH_ROUND,
  FL_GLYPH_UP,
  FL_GLYPH_DOWN,
  FL_GLYPH_LEFT,
  FL_GLYPH_RIGHT,
  FL_GLYPH_UP_BUTTON,
  FL_GLYPH_DOWN_BUTTON,
  FL_GLYPH_LEFT_BUTTON,
  FL_GLYPH_RIGHT_BUTTON,
  FL_GLYPH_VSLIDER,
  FL_GLYPH_HSLIDER,
  FL_GLYPH_VNSLIDER,
  FL_GLYPH_HNSLIDER,
  FL_GLYPH_CHOICE
};

struct FL_API Fl_Named_Style : public Fl_Style {
  const char* name;
  Fl_Named_Style* next;
  static Fl_Named_Style* first;
  Fl_Named_Style** back_pointer; // used by Fl_Style::start
  Fl_Named_Style(const char* name, void (*revert)(Fl_Style*), Fl_Named_Style**);
};

FL_API void fl_get_system_colors();

#endif

//
// End of "$Id: Fl_Style.h,v 1.4 2002/01/20 07:37:15 spitzak Exp $".
//
