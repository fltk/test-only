//
// "$Id: Fl_Output.cxx,v 1.30 2002/01/23 08:46:01 spitzak Exp $"
//
// Output widget for the Fast Light Tool Kit (FLTK).
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

// This subclass of Fl_Input_ does not allow user to edit the output.
// This is done by making the replace() function refuse all changes.
// Also handle() has been changed so the user must click or tab to
// it to get the focus.

#include <fltk/Fl.h>
#include <fltk/Fl_Output.h>
#include <fltk/fl_draw.h>

bool Fl_Output::replace(int, int, const char*, int) {
  return false;
}

int Fl_Output::handle(int event) {
  // The Fl_Input will grab the focus if sent a shortcut, prevent this:
  if (event == FL_SHORTCUT) return 0;
  return Fl_Input::handle(event);
}

static Fl_Named_Style style("Output", 0, &Fl_Output::default_style);
Fl_Named_Style* Fl_Output::default_style = &::style;

Fl_Output::Fl_Output(int x, int y, int w, int h, const char *l)
  : Fl_Input(x, y, w, h, l)
{
  style(default_style);
}

//
// End of "$Id: Fl_Output.cxx,v 1.30 2002/01/23 08:46:01 spitzak Exp $".
//
