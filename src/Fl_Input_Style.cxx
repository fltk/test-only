//
// "$Id: Fl_Input_Style.cxx,v 1.1 1999/10/22 03:11:21 vincent Exp $"
//
// Style of Fl_Input for the Fast Light Tool Kit (FLTK).
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

// The style is defined in a separate file so that the rest of Fl_Input
// does not get always linked in ...

#include <FL/Fl_Input.H>

// Fl_Input, the box, color, and selection color are different:

Fl_Style Fl_Input::default_style = {
  FL_THIN_DOWN_BOX, // box DIFF
  fl_glyph,	// glyphs
  FL_HELVETICA,	// label_font
  FL_HELVETICA,	// text_font
  FL_NORMAL_LABEL, // label_type
  FL_WHITE,	// color DIFF
  FL_NO_COLOR,	// label_color
  FL_BLUE_SELECTION_COLOR, // selection_color DIFF
  FL_WHITE,	// selection_text_color
  FL_NO_COLOR,	// off_color
  FL_NO_COLOR,	// highlight_color
  FL_NO_COLOR,	// highlight_label_color
  FL_NO_COLOR,	// text_color
  FL_NORMAL_SIZE,// label_size
  FL_NORMAL_SIZE,// text_size
};

Fl_Style_Definer fl_input_style_definer("input", Fl_Input::default_style);

//
// End of "$Id: Fl_Input_Style.cxx,v 1.1 1999/10/22 03:11:21 vincent Exp $".
//
