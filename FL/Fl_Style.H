//
// "$Id: Fl_Style.H,v 1.21 1999/11/21 06:23:16 carl Exp $"
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
typedef int (*Fl_Theme_Handler)(int);

struct Fl_Named_Style;

struct FL_API Fl_Style {
  Fl_Boxtype	box;
  Fl_Boxtype	glyph_box; // needed for all Light Buttons & sliders
  Fl_Glyph	glyph;
  Fl_Font	label_font;
  Fl_Font	text_font;
  Fl_Labeltype	label_type;
  // All items after color must be sizeof(int).  All items before must
  // be sizeof(void*)!  This allows the style inheritance to use tacky
  // code to copy things.
  Fl_Color	color;
  Fl_Color	label_color;
  Fl_Color	selection_color;
  Fl_Color	selection_text_color;
  Fl_Color	off_color;
  Fl_Color	highlight_color;
  Fl_Color	highlight_label_color;
  Fl_Color	text_color;
  unsigned	label_size;
  unsigned	text_size;
  unsigned	leading;

  Fl_Style* parent;

  void  (*revertfunc)(Fl_Style*);
  int dynamic() const {return !revertfunc;}

  Fl_Style(); // creates a dynamic() style

  void setp(const void** p, const void* v) { *p = v; }
  void seti(unsigned* p, unsigned v) { *p = v; }

  void set_box(Fl_Boxtype v)		{setp((const void**)&box, v);}
  void set_glyph_box(Fl_Boxtype v)	{setp((const void**)&glyph_box, v);}
  void set_glyph(Fl_Glyph v)	        {setp((const void**)&glyph, (const void*)v);}
  void set_label_font(Fl_Font v)	{setp((const void**)&label_font, v);}
  void set_text_font(Fl_Font v)		{setp((const void**)&text_font, v);}
  void set_label_type(Fl_Labeltype v)	{setp((const void**)&label_type, v);}
  void set_color(Fl_Color v)		{seti(&color, v);}
  void set_up_color(Fl_Color v)		{seti(&color, v);}
  void set_label_color(Fl_Color v)	{seti(&label_color, v);}
  void set_selection_color(Fl_Color v)	{seti(&selection_color, v);}
  void set_selection_text_color(Fl_Color v) {seti(&selection_text_color, v);}
  void set_on_color(Fl_Color v)		{seti(&selection_color, v);}
  void set_down_color(Fl_Color v)	{seti(&selection_color, v);}
  void set_off_color(Fl_Color v)	{seti(&off_color, v);}
  void set_highlight_color(Fl_Color v)	{seti(&highlight_color, v);}
  void set_highlight_label_color(Fl_Color v) {seti(&highlight_label_color, v);}
  void set_text_color(Fl_Color v)	{seti(&text_color, v);}
  void set_label_size(unsigned v)	{seti(&label_size, v);}
  void set_text_size(unsigned v)	{seti(&text_size, v);}
  void set_leading(unsigned v)		{seti(&leading, v);}

  static void revert(); // reverts the entire world of styles to defaults

  static int draw_boxes_inactive; // should boxes ever be drawn inactive?
  static int inactive_menu_hack; // Microsoft sucks
  static double inactive_color_weight; // how much weight to FL_GRAY for inactive colors

  static void start(char* name);	// start a new class of style or use a previously started one

  // find a default style by its string ID
  static Fl_Named_Style* find(const char* name);
};

FL_API void fl_theme_handler(Fl_Theme_Handler handler);
FL_API Fl_Theme_Handler fl_theme_handler();

FL_API void fl_glyph(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_default(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_adjuster(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_check(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_choice(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_arrow(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_light(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_return(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_slider(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);
FL_API void fl_glyph_menu(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype);


enum Fl_Glyphtype {	// glyph types
  // zero is reserved to draw an empty box
  FL_GLYPH_CHECK = 1,
  FL_GLYPH_RADIO,
  FL_GLYPH_LIGHT,
  FL_GLYPH_UP,
  FL_GLYPH_DOWN,
  FL_GLYPH_LEFT,
  FL_GLYPH_RIGHT,
  FL_GLYPH_VSLIDER,
  FL_GLYPH_HSLIDER,
  FL_GLYPH_VNSLIDER,
  FL_GLYPH_HNSLIDER,
  FL_GLYPH_CHOICE,
  FL_GLYPH_FASTARROW,
  FL_GLYPH_MEDIUMARROW,
  FL_GLYPH_SLOWARROW,
  FL_GLYPH_LEFTARROW,
  FL_GLYPH_RIGHTARROW,
  FL_GLYPH_2LEFTARROW,
  FL_GLYPH_2RIGHTARROW,
  FL_GLYPH_RETURN
};

FL_API int fl_kde1();
FL_API int fl_kde1_colors();
FL_API int fl_motif();
FL_API int fl_windows();
FL_API int fl_windows_colors();

struct FL_API Fl_Named_Style : public Fl_Style {
  const char* name;
  Fl_Style** pdefault_style;
  Fl_Named_Style* next;
  static Fl_Named_Style* first;

  Fl_Named_Style(const char* name, void (*revertfunc)(Fl_Style*)=0, Fl_Style** pdefault_style = 0);
  Fl_Named_Style() { }	// Empty constructor used by Fl_Style::start only
};

#endif

//
// End of "$Id: Fl_Style.H,v 1.21 1999/11/21 06:23:16 carl Exp $".
//
