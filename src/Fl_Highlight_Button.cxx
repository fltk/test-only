//
// "$Id: Fl_Highlight_Button.cxx,v 1.7 2001/07/23 09:50:04 spitzak Exp $"
//
// Highlight button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Highlight_Button.h>

static void revert(Fl_Style* s) {
  s->box = FL_HIGHLIGHT_BOX;
  s->highlight_color = FL_LIGHT1;
}
static Fl_Named_Style style("Highlight_Button", revert, &Fl_Highlight_Button::default_style);
Fl_Named_Style* Fl_Highlight_Button::default_style = &::style;

Fl_Highlight_Button::Fl_Highlight_Button(int x,int y,int w,int h,const char *l)
  : Fl_Button(x,y,w,h,l)
{
  style(default_style);
}

//
// End of "$Id: Fl_Highlight_Button.cxx,v 1.7 2001/07/23 09:50:04 spitzak Exp $".
//
