//
// "$Id: Fl_Output.cxx,v 1.24 2000/04/16 08:31:46 bill Exp $"
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
  if (damage() & FL_DAMAGE_ALL) draw_window_frame();
  int X=x(); int Y=y(); int W=w(); int H=h(); window_box()->inset(X,Y,W,H);
  Fl_Input_::drawtext(X,Y,W,H);
}

int Fl_Output::handle(int event) {
  if (event == FL_ENTER) return 1; // For tooltips
  if (event == FL_FOCUS) return 0;
  int X=x(); int Y=y(); int W=w(); int H=h(); window_box()->inset(X,Y,W,H);
  if (event == FL_PUSH && !focused()) take_focus();
  return Fl_Input_::handletext(event,X,Y,W,H);
}

#include <FL/Fl_Input.H>
Fl_Output::Fl_Output(int x, int y, int w, int h, const char *l)
  : Fl_Input_(x, y, w, h, l)
{
  style(default_style);
}

//
// End of "$Id: Fl_Output.cxx,v 1.24 2000/04/16 08:31:46 bill Exp $".
//
