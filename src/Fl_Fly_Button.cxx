//
// "$Id: Fl_Fly_Button.cxx,v 1.4 1999/11/01 02:21:32 carl Exp $"
//
// Fly button widget for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Fly_Button.H>
#include <FL/fl_draw.H>

Fl_Fly_Button::Fl_Fly_Button(int x,int y,int w,int h,const char *l)
  : Fl_Button(x,y,w,h,l)
{
  style(default_style);
}

Fl_Style Fl_Fly_Button::default_style = {
  FL_HIGHLIGHT_DOWN_BOX,// box
  0,                    // glyph_box
  0,		        // glyphs
  0,		        // label_font
  0,		        // text_font
  0,		        // label_type
  0,		        // color
  0,		        // label_color
  0,    	        // selection_color / on_color
  0,		        // selection_text_color
  0,    	        // off_color
  0                     // highlight color
};

static Fl_Style_Definer x("highlight button", Fl_Fly_Button::default_style);

//
// End of "$Id: Fl_Fly_Button.cxx,v 1.4 1999/11/01 02:21:32 carl Exp $".
//
