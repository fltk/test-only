//
// "$Id: Fl_Output.cxx,v 1.13 1999/09/25 22:27:08 vincent Exp $"
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
// Used to display output.

#include <FL/Fl.H>
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>

void Fl_Output::draw() {
  if (damage() & FL_DAMAGE_ALL) draw_frame();
  Fl_Input_::drawtext(x()+box()->dx(), y()+box()->dy(),
		      w()-box()->dw(), h()-box()->dh());
}

int Fl_Output::handle(int event) {
  if (event == FL_ENTER) return 1;
  if (event == FL_FOCUS) return 0;
  return Fl_Input_::handletext(event,
			       x()+box()->dx(), y()+box()->dy(),
			       w()-box()->dw(), h()-box()->dh());
}

// Fl_Output defaults to the same as Fl_Input and is a child of it:
Fl_Style Fl_Output::default_style = {
};

#include <FL/Fl_Input.H>
Fl_Output::Fl_Output(int x, int y, int w, int h, const char *l)
  : Fl_Input_(x, y, w, h, l)
{
  style(&Fl_Input::default_style); // so that Fl_Output's style will be a 
                                   // child of Fl_Input's style
  style(&default_style);
}

// dummy widgets to make sure the style is initialised
// we declare them together to be sure they will be initialised in this order !
Fl_Input fl_init_input_style(0,0,0,0,0); 
Fl_Output fl_init_output_style(0,0,0,0,0);

Fl_Style_Definer fl_output_style_definer("output", Fl_Output::default_style);


//
// End of "$Id: Fl_Output.cxx,v 1.13 1999/09/25 22:27:08 vincent Exp $".
//
