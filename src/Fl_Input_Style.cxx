//
// "$Id: Fl_Input_Style.cxx,v 1.7 1999/11/10 18:06:04 carl Exp $"
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
// Fl_Output is also defined here, output widgets use it as a reference
// so that themes that set input and output to different colors work.
// To avoid problems with inheritance (so that a theme can just change
// input) I need to default to identical appearance, which matches
// how Windoze works anyway...

#include <FL/Fl_Input.H>

static void revert(Fl_Style* s) {
  s->box = FL_THIN_DOWN_BOX;
  s->color = FL_WHITE;
  s->selection_color = FL_BLUE_SELECTION_COLOR;
  s->selection_text_color = FL_WHITE;
  s->off_color = FL_BLACK;
}

Fl_Style Fl_Input::default_style("Input", revert);

#include <FL/Fl_Output.H>

Fl_Style Fl_Output::default_style("Output", revert);

//
// End of "$Id: Fl_Input_Style.cxx,v 1.7 1999/11/10 18:06:04 carl Exp $".
//
