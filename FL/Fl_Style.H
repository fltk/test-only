//
// "$Id: Fl_Style.H,v 1.9 1999/11/07 08:11:23 bill Exp $"
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

typedef void (*Fl_Glyph)(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype = FL_NORMAL_BOX);

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
  Fl_Style*	child;
  Fl_Style*	next;
  Fl_Style**	previous;
  int		mbf;	// Which entries are different from the parent's ones

  void setp(const void**, const void*);
  void setp(const void**, const void*, int mask);
  void seti(unsigned*, unsigned);
  void seti(unsigned*, unsigned, int mask);
  void set_box(Fl_Boxtype v)		{setp((const void**)&box, v);}
  void set_glyph_box(Fl_Boxtype v)	{setp((const void**)&glyph_box, v);}
  void set_box2(Fl_Boxtype v)		{setp((const void**)&glyph_box, v);}
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

  void add_child(Fl_Style* s) const;

  // find a default style by its string ID
  static Fl_Style* find(const char* name);

  // set the FLTK default style
  static void revert();
};

typedef void (*Fl_Style_Reverter)(Fl_Style*);

// Create a global object of this type with correct parameter to define
// a new style class.  Plugins can search this to find the styles used by
// a program:
class FL_API Fl_Style_Definer {
public:
  const char* name;
  Fl_Style* style;
  Fl_Style_Reverter revert;
  Fl_Style_Definer* next;
  static Fl_Style_Definer* first;
  Fl_Style_Definer(char* n, Fl_Style& s, Fl_Style_Reverter r = 0) : name(n), style(&s), revert(r), next(first) {
    first = this; if (r) r(&s);}
};

FL_API void fl_glyph(int, int,int,int,int, Fl_Color, Fl_Color, Fl_Flags, Fl_Boxtype = FL_NORMAL_BOX);

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
  FL_GLYPH_FASTARROW,
  FL_GLYPH_MEDIUMARROW,
  FL_GLYPH_SLOWARROW,
  FL_GLYPH_VNSLIDER,
  FL_GLYPH_HNSLIDER
};

#endif

//
// End of "$Id: Fl_Style.H,v 1.9 1999/11/07 08:11:23 bill Exp $".
//
