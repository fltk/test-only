//
// "$Id: Fl_Fly_Button.cxx,v 1.7 1999/11/10 12:21:51 bill Exp $"
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

static void revert(Fl_Style* s) {
  s->box = FL_HIGHLIGHT_DOWN_BOX;
  s->highlight_color = FL_LIGHT1;
}

Fl_Named_Style Fl_Fly_Button::default_style("Fly_Button", revert);

//
// End of "$Id: Fl_Fly_Button.cxx,v 1.7 1999/11/10 12:21:51 bill Exp $".
//
