//
// "$Id: Fl_Input_Style.cxx,v 1.9 1999/11/20 04:42:42 vincent Exp $"
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

// The styles for Fl_Input and Fl_Output are referenced directly by
// some parts of fltk's code.  I put them here so that uses of them does
// not link in the rest of the Fl_Input/Output code.
//
// It is possible that Fl_Output's style should be a child of Fl_Input's.
// This could be done by making it's own revert() function and setting
// parent in it.

#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

static void revert(Fl_Style* s) {
  s->box = FL_THIN_DOWN_BOX;
  s->color = FL_WHITE;
  s->selection_color = FL_BLUE_SELECTION_COLOR;
  s->selection_text_color = FL_WHITE;
  s->off_color = FL_BLACK;
}

Fl_Style* Fl_Input::default_style = new Fl_Named_Style("Input", revert, &Fl_Input::default_style);

Fl_Style* Fl_Output::default_style = new Fl_Named_Style("Output", revert, &Fl_Output::default_style);

//
// End of "$Id: Fl_Input_Style.cxx,v 1.9 1999/11/20 04:42:42 vincent Exp $".
//
