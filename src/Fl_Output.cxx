//
// "$Id: Fl_Output.cxx,v 1.28 2000/08/20 04:31:38 spitzak Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>

int Fl_Output::replace(int, int, const char*, int) {
  return 0;
}

static Fl_Named_Style* style = new Fl_Named_Style("Output", 0, &style);

Fl_Output::Fl_Output(int x, int y, int w, int h, const char *l)
  : Fl_Input(x, y, w, h, l)
{
  style(::style);
}

//
// End of "$Id: Fl_Output.cxx,v 1.28 2000/08/20 04:31:38 spitzak Exp $".
//
