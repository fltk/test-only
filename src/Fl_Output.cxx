//
// "$Id: Fl_Output.cxx,v 1.36 2002/12/10 02:00:44 easysw Exp $"
//
// Output widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Output.h>
#include <fltk/draw.h>

using namespace fltk;

bool Output::replace(int, int, const char*, int) {
  // By making this function return false, all editing is disallowed:
  return false;
}

int Output::handle(int event) {
  // The Input may grab the focus if sent a shortcut, prevent this:
  if (event == SHORTCUT) return 0;
  // You can't drag&drop on it either:
  if (event == DND_ENTER) return 0;
  // Don't return 3 for FOCUS, return 1 instead:
  if (Input::handle(event)) {
    // clicking or selecting text does the callback:
    if (event == RELEASE) do_callback();
    return 1;
  }
  return 0;
}

// Output has it's own style so the color can be set to gray like
// some themes want:
static NamedStyle style("Output", 0, &Output::default_style);
NamedStyle* Output::default_style = &::style;

Output::Output(int x, int y, int w, int h, const char *l)
  : Input(x, y, w, h, l)
{
  style(default_style);
}

//
// End of "$Id: Fl_Output.cxx,v 1.36 2002/12/10 02:00:44 easysw Exp $".
//
