//
// "$Id: Style.h,v 1.4 2003/02/03 02:33:56 spitzak Exp $"
//
// Style structure used by Widgets
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#ifndef fltk_Style_h
#define fltk_Style_h

#include "Color.h"

namespace fltk {

class FL_API Widget;

class Box;
extern FL_API Box* const UP_BOX;
extern FL_API Box* const DOWN_BOX;
extern FL_API Box* const THIN_UP_BOX;
extern FL_API Box* const THIN_DOWN_BOX;
extern FL_API Box* const ENGRAVED_BOX;
extern FL_API Box* const EMBOSSED_BOX;
extern FL_API Box* const BORDER_BOX;
extern FL_API Box* const FLAT_BOX;
extern FL_API Box* const HIGHLIGHT_UP_BOX;
extern FL_API Box* const HIGHLIGHT_DOWN_BOX;
extern FL_API Box* const ROUND_UP_BOX;
extern FL_API Box* const ROUND_DOWN_BOX;
extern FL_API Box* const DIAMOND_UP_BOX;
extern FL_API Box* const DIAMOND_DOWN_BOX;
extern FL_API Box* const NO_BOX;
extern FL_API Box* const SHADOW_BOX;
extern FL_API Box* const ROUNDED_BOX;
extern FL_API Box* const RSHADOW_BOX;
extern FL_API Box* const RFLAT_BOX;
extern FL_API Box* const OVAL_BOX;
extern FL_API Box* const OSHADOW_BOX;
extern FL_API Box* const OFLAT_BOX;
extern FL_API Box* const BORDER_FRAME;
extern FL_API Box* const DOTTED_FRAME;
extern FL_API Box* const PLASTIC_UP_BOX;
extern FL_API Box* const PLASTIC_DOWN_BOX;

struct Font;
FL_API Font* font(const char* name, int attrib = 0);
enum {  // values for attributes:
  BOLD = 1,
  ITALIC = 2,
  BOLD_ITALIC = 3
};
extern FL_API Font* const HELVETICA;
extern FL_API Font* const HELVETICA_BOLD;
extern FL_API Font* const HELVETICA_ITALIC;
extern FL_API Font* const HELVETICA_BOLD_ITALIC;
extern FL_API Font* const COURIER;
extern FL_API Font* const COURIER_BOLD;
extern FL_API Font* const COURIER_ITALIC;
extern FL_API Font* const COURIER_BOLD_ITALIC;
extern FL_API Font* const TIMES;
extern FL_API Font* const TIMES_BOLD;
extern FL_API Font* const TIMES_ITALIC;
extern FL_API Font* const TIMES_BOLD_ITALIC;
extern FL_API Font* const SYMBOL_FONT;
extern FL_API Font* const SCREEN_FONT;
extern FL_API Font* const SCREEN_BOLD_FONT;
extern FL_API Font* const ZAPF_DINGBATS;
FL_API Font* font(int);

class LabelType;
extern FL_API LabelType* const NO_LABEL;
extern FL_API LabelType* const NORMAL_LABEL;
extern FL_API LabelType* const SYMBOL_LABEL; // same as NORMAL_LABEL
extern FL_API LabelType* const SHADOW_LABEL;
extern FL_API LabelType* const ENGRAVED_LABEL;
extern FL_API LabelType* const EMBOSSED_LABEL;

typedef void (*GlyphStyle)(const Widget*, int type,
			  int,int,int,int,
			  Flags);

extern "C" {typedef bool (*Theme)();}

struct FL_API Style {
  const Style* parent;
  void  (*revertfunc)(Style*);

  Box*		box;
  Box*		buttonbox;
  Box*		focusbox;
  GlyphStyle	glyph;
  Font*		labelfont;
  Font*		textfont;
  LabelType*	labeltype;
  Color		color;
  Color		textcolor;
  Color		selection_color;
  Color		selection_textcolor;
  Color		buttoncolor;
  Color		labelcolor;
  Color		highlight_color;
  Color		highlight_labelcolor;
  float		labelsize;
  float		textsize;
  float		leading;
  unsigned char	scrollbar_align;
  unsigned char	scrollbar_width;

  // global settings:
  static bool   draw_boxes_inactive;
  static int	wheel_scroll_lines;

  Style(); // creates a dynamic() style
  bool dynamic() const {return !revertfunc;}

  static Style* find(const char* name);
  static void revert();
  static Theme theme() {return theme_;}
  static void theme(Theme f) {theme_ = f;}
  static Theme load_theme(const char*);
  static void load_theme();
  static void reload_theme();
  static const char* scheme() {return scheme_;}
  static void scheme(const char* f) {scheme_ = f;}

private:
  static Theme theme_;
  static const char* scheme_;
};

struct FL_API NamedStyle : public Style {
  const char* name;
  NamedStyle* next;
  static NamedStyle* first;
  NamedStyle** back_pointer; // used by StyleSet
  NamedStyle(const char* name, void (*revert)(Style*), NamedStyle**);
};

enum GlyphNumber {	// glyph types understood by Widget::default_glyph()
  // the numbers 0-99 are reserved for private glyph functions
  GLYPH_BOX = 100,
  GLYPH_UP,
  GLYPH_DOWN,
  GLYPH_LEFT,
  GLYPH_RIGHT,
  GLYPH_UP_BUTTON,
  GLYPH_DOWN_BUTTON,
  GLYPH_LEFT_BUTTON,
  GLYPH_RIGHT_BUTTON
};

FL_API bool get_system_colors();
FL_API const char* find_config_file(char* out, int size, const char* name);
FL_API void redraw();

}

extern "C" FL_API bool fltk_theme();

#endif

//
// End of "$Id: Style.h,v 1.4 2003/02/03 02:33:56 spitzak Exp $".
//
