//
// "$Id: Fl_Highlight_Button.cxx,v 1.3 2000/01/10 06:31:21 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Highlight_Button.H>

static void revert(Fl_Style* s) {
  s->box = FL_HIGHLIGHT_BOX;
  s->highlight_color = FL_LIGHT1;
}

static Fl_Named_Style* style =new Fl_Named_Style("Highlight_Button",revert,&style);

Fl_Highlight_Button::Fl_Highlight_Button(int x,int y,int w,int h,const char *l)
  : Fl_Button(x,y,w,h,l)
{
  style(::style);
}

//
// End of "$Id: Fl_Highlight_Button.cxx,v 1.3 2000/01/10 06:31:21 bill Exp $".
//
