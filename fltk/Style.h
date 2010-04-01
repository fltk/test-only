//
// "$Id: Style.h 6233 2008-09-14 07:54:06Z spitzak $"
//
// Style structure used by Widgets
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#include <fltk3/Enumerations.H>
#include "Color.h"

namespace fltk {

/// \name fltk/Style.h
//@{

  class FL_API Widget;
  class FL_API Rectangle;
  class FL_API Symbol;
  
  // typedef Symbol Box;
  
  // In FLTK2, the Boxtype is actually a class that implements the box drawing.
  // This is very nice and should be ported to FLTK3. For now, we emulate the
  // API by creating a bunch of simplified Box classes. Interestingly, there are 
  // no "frame" styles! I wonder why they were taken out?
  
  // we are still missing a lookup table that will convert the fltk3::Boxtype
  // back into a pointer to the correct Box class.
  
  class Box { 
    fltk3::Boxtype _bt;
  public:
    Box(fltk3::Boxtype b) : _bt(b) { }
    fltk3::Boxtype bt() { return _bt; }
  };

# define FLTK_DEFINE_BOX_TYPE(a) Box _##a(fltk3::a); FL_API Box* const a = &_##a;
# define FLTK_DEFINE_BOX_TYPE2(a, b) Box _##a(fltk3::b); FL_API Box* const a = &_##a;
  
  FLTK_DEFINE_BOX_TYPE(UP_BOX)
  FLTK_DEFINE_BOX_TYPE(DOWN_BOX)
  FLTK_DEFINE_BOX_TYPE(THIN_UP_BOX)
  FLTK_DEFINE_BOX_TYPE(THIN_DOWN_BOX)
  FLTK_DEFINE_BOX_TYPE(ENGRAVED_BOX)
  FLTK_DEFINE_BOX_TYPE(EMBOSSED_BOX)
  FLTK_DEFINE_BOX_TYPE(BORDER_BOX)
  FLTK_DEFINE_BOX_TYPE(FLAT_BOX)
  FLTK_DEFINE_BOX_TYPE2(HIGHLIGHT_UP_BOX, GTK_UP_BOX)
  FLTK_DEFINE_BOX_TYPE2(HIGHLIGHT_DOWN_BOX, GTK_DOWN_BOX)
  FLTK_DEFINE_BOX_TYPE(ROUND_UP_BOX)
  FLTK_DEFINE_BOX_TYPE(ROUND_DOWN_BOX)
  FLTK_DEFINE_BOX_TYPE(DIAMOND_UP_BOX)
  FLTK_DEFINE_BOX_TYPE(DIAMOND_DOWN_BOX)
  FLTK_DEFINE_BOX_TYPE(NO_BOX)
  FLTK_DEFINE_BOX_TYPE(SHADOW_BOX)
  FLTK_DEFINE_BOX_TYPE(ROUNDED_BOX)
  FLTK_DEFINE_BOX_TYPE(RSHADOW_BOX)
  FLTK_DEFINE_BOX_TYPE(RFLAT_BOX)
  FLTK_DEFINE_BOX_TYPE(OVAL_BOX)
  FLTK_DEFINE_BOX_TYPE(OSHADOW_BOX)
  FLTK_DEFINE_BOX_TYPE(OFLAT_BOX)
  FLTK_DEFINE_BOX_TYPE(BORDER_FRAME)
  FLTK_DEFINE_BOX_TYPE(PLASTIC_UP_BOX)
  FLTK_DEFINE_BOX_TYPE(PLASTIC_DOWN_BOX)

# undef FLTK_DEFINE_BOX_TYPE
  
struct Font;
/*
The values below are currently emulated. In a future version of FLTK3, we 
really should port the FLTK2 way of handling fonts to FLTK3.
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
*/
  const Fl_Font HELVETICA             = fltk3::HELVETICA;
  const Fl_Font HELVETICA_BOLD        = fltk3::HELVETICA_BOLD;
  const Fl_Font HELVETICA_ITALIC      = fltk3::HELVETICA_ITALIC;
  const Fl_Font HELVETICA_BOLD_ITALIC = fltk3::HELVETICA_BOLD_ITALIC;
  const Fl_Font COURIER               = fltk3::COURIER;
  const Fl_Font COURIER_BOLD          = fltk3::COURIER_BOLD;
  const Fl_Font COURIER_ITALIC        = fltk3::COURIER_ITALIC;
  const Fl_Font COURIER_BOLD_ITALIC   = fltk3::COURIER_BOLD_ITALIC;
  const Fl_Font TIMES                 = fltk3::TIMES;
  const Fl_Font TIMES_BOLD            = fltk3::TIMES_BOLD;
  const Fl_Font TIMES_ITALIC          = fltk3::TIMES_ITALIC;
  const Fl_Font TIMES_BOLD_ITALIC     = fltk3::TIMES_BOLD_ITALIC;
  const Fl_Font SYMBOL_FONT           = fltk3::SYMBOL_FONT;
  const Fl_Font SCREEN_FONT           = fltk3::SCREEN_FONT;
  const Fl_Font SCREEN_BOLD_FONT      = fltk3::SCREEN_BOLD_FONT;
  const Fl_Font ZAPF_DINGBATS         = fltk3::ZAPF_DINGBATS;
  
  
  
/*
 The values below are currently emulated. In a future version of FLTK3, we 
 really should port the FLTK2 way of handling labeltypes to FLTK3.
class LabelType;
extern FL_API LabelType* const NO_LABEL;
extern FL_API LabelType* const NORMAL_LABEL;
extern FL_API LabelType* const SYMBOL_LABEL; // same as NORMAL_LABEL
extern FL_API LabelType* const SHADOW_LABEL;
extern FL_API LabelType* const ENGRAVED_LABEL;
extern FL_API LabelType* const EMBOSSED_LABEL;
 */
  enum LabelType {
    NO_LABEL        = fltk3::NO_LABEL,
    NORMAL_LABEL    = fltk3::NORMAL_LABEL,
    SYMBOL_LABEL    = fltk3::NORMAL_LABEL,
    SHADOW_LABEL    = fltk3::SHADOW_LABEL,
    ENGRAVED_LABEL  = fltk3::ENGRAVED_LABEL,
    EMBOSSED_LABEL  = fltk3::EMBOSSED_LABEL
  };
  
  
class Style;

class FL_API Style {
 public:
  // Everything is public for various back-compatability hacks:
  const Style* parent_;
  Box*		box_;
  Box*		buttonbox_;
  Symbol*	glyph_;
  Font*		labelfont_;
  Font*		textfont_;
  LabelType*	labeltype_;
  Color		color_;
  Color		textcolor_;
  Color		selection_color_;
  Color		selection_textcolor_;
  Color		buttoncolor_;
  Color		labelcolor_;
  Color		highlight_color_;
  Color		highlight_textcolor_;
  float		labelsize_;
  float		textsize_;
  float		leading_;
  unsigned char	scrollbar_align_;
  unsigned char	scrollbar_width_;
  bool		dynamic_;
  // global settings:
  static bool	hide_underscore_;
  static bool   draw_boxes_inactive_;
  static int	wheel_scroll_lines_;

  // Get functions, which search parents if value is zero:
  Box*		box()		const;
  Box*		buttonbox()	const;
  Symbol*	glyph()		const;
  Font*		labelfont()	const;
  Font*		textfont()	const;
  LabelType*	labeltype()	const;
  Color		color()		const;
  Color		textcolor()	const;
  Color		selection_color() const;
  Color		selection_textcolor() const;
  Color		buttoncolor()	const;
  Color		labelcolor()	const;
  Color		highlight_color() const;
  Color		highlight_textcolor() const;
  float		labelsize()	const;
  float		textsize()	const;
  float		leading()	const;
  unsigned char	scrollbar_align() const;
  unsigned char	scrollbar_width() const;

  bool		hide_underscore() const {return hide_underscore_;}
  bool		draw_boxes_inactive() const {return draw_boxes_inactive_;}
  int		wheel_scroll_lines() const {return wheel_scroll_lines_;}

  // Set functions:
  void box		(Box* v)	{box_ = v;		}
  void buttonbox	(Box* v)	{buttonbox_ = v;	}
  void glyph		(Symbol* v)	{glyph_ = v; 		}
  void labelfont	(Font* v)	{labelfont_ = v;	}
  void textfont		(Font* v)	{textfont_ = v;		}
  void labeltype	(LabelType* v)	{labeltype_ = v; 	}
  void color		(Color v)	{color_ = v; 		}
  void textcolor	(Color v)	{textcolor_ = v; 	}
  void selection_color	(Color v)	{selection_color_ = v;	}
  void selection_textcolor(Color v)	{selection_textcolor_ = v;}
  void buttoncolor	(Color v)	{buttoncolor_ = v; 	}
  void labelcolor	(Color v)	{labelcolor_ = v; 	}
  void highlight_color	(Color v)	{highlight_color_ = v; 	}
  void highlight_textcolor(Color v)	{highlight_textcolor_ = v;}
  void labelsize	(float v)	{labelsize_ = v;	}
  void textsize		(float v)	{textsize_ = v;		}
  void leading		(float v)	{leading_ = v;		}
  void scrollbar_align	(unsigned char v) {scrollbar_align_ = v;}	
  void scrollbar_width	(unsigned char v) {scrollbar_width_ = v;}	

  void hide_underscore	(bool v)	{hide_underscore_ = v;	}
  void draw_boxes_inactive(bool v)	{draw_boxes_inactive_ = v;}
  void wheel_scroll_lines(int v)	{wheel_scroll_lines_ = v;}

  Style();
  bool dynamic() const {return dynamic_;}

  static Style* find(const char* name);
};

struct FL_API NamedStyle : public Style {
  const char* name;
  void  (*revertfunc)(Style*);
  NamedStyle** back_pointer; // used by StyleSet
  static NamedStyle* first;
  NamedStyle* next;
  NamedStyle(const char* name, void (*revert)(Style*), NamedStyle** backptr);
};

extern "C" {typedef bool (*Theme)();}
extern FL_API Theme theme_;
inline Theme theme() {return theme_;}
inline void theme(Theme f) {theme_ = f;}
FL_API void load_theme();
FL_API void reload_theme();
FL_API bool reset_theme();

}

extern "C" FL_API bool fltk_theme();

//@}

#endif

//
// End of "$Id: Style.h 6233 2008-09-14 07:54:06Z spitzak $".
//
