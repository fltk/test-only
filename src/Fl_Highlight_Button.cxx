//
// "$Id: Fl_Highlight_Button.cxx,v 1.12 2003/02/02 10:39:23 spitzak Exp $"
//
// Highlight button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/events.h>
#include <fltk/HighlightButton.h>

using namespace fltk;

static void revert(Style* s) {
  //s->color = GRAY75;
  s->box = HIGHLIGHT_UP_BOX;
  s->highlight_color = GRAY85;
}
static NamedStyle style("Highlight_Button", revert, &HighlightButton::default_style);
NamedStyle* HighlightButton::default_style = &::style;

HighlightButton::HighlightButton(int x,int y,int w,int h,const char *l)
  : Button(x,y,w,h,l)
{
  style(default_style);
}

//
// End of "$Id: Fl_Highlight_Button.cxx,v 1.12 2003/02/02 10:39:23 spitzak Exp $".
//
