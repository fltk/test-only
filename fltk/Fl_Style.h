//
// "$Id: Fl_Style.h,v 1.8 2002/03/10 23:10:23 spitzak Exp $"
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

extern "C" {typedef bool (*Fl_Theme)();}

struct FL_API Fl_Style {
  Fl_Boxtype    box;
  Fl_Boxtype    button_box;
  Fl_Boxtype	focus_box;
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

  // global settings:
  static bool   draw_boxes_inactive;
  static unsigned scrollbar_width;
  static Fl_Flags scrollbar_align;
  static int	wheel_scroll_lines;

  const Fl_Style* parent;

  void  (*revertfunc)(Fl_Style*);
  bool dynamic() const {return !revertfunc;}

  Fl_Style(); // creates a dynamic() style

  static Fl_Style* find(const char* name);
  static void revert();
  static Fl_Theme theme() {return theme_;}
  static void theme(Fl_Theme f) {theme_ = f;}
  static Fl_Theme load_theme(const char*);
  static void load_theme();
  static void reload_theme();
  static const char* scheme() {return scheme_;}
  static void scheme(const char* f) {scheme_ = f;}

private:
  static Fl_Theme theme_;
  static const char* scheme_;
};

struct FL_API Fl_Named_Style : public Fl_Style {
  const char* name;
  Fl_Named_Style* next;
  static Fl_Named_Style* first;
  Fl_Named_Style** back_pointer; // used by Fl_Style_Set
  Fl_Named_Style(const char* name, void (*revert)(Fl_Style*), Fl_Named_Style**);
};

enum Fl_Glyphtype {	// glyph types understood by Fl_Widget::default_glyph()
  // the numbers 0-99 are reserved for private glyph functions
  FL_GLYPH_BOX = 100,
  FL_GLYPH_UP,
  FL_GLYPH_DOWN,
  FL_GLYPH_LEFT,
  FL_GLYPH_RIGHT,
  FL_GLYPH_UP_BUTTON,
  FL_GLYPH_DOWN_BUTTON,
  FL_GLYPH_LEFT_BUTTON,
  FL_GLYPH_RIGHT_BUTTON
};

extern "C" FL_API bool fltk_theme();
FL_API bool fl_get_system_colors();
FL_API const char* fl_find_config_file(char* out, int size, const char* name);

#endif

//
// End of "$Id: Fl_Style.h,v 1.8 2002/03/10 23:10:23 spitzak Exp $".
//
