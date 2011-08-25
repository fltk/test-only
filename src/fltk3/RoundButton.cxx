//
// "$Id$"
//
// Round button for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// A subclass of fltk3::Button that always draws as a round circle.  This
// circle is smaller than the widget size and can be surrounded by
// another box type, for compatibility with Forms.

#include <fltk3/run.h>
#include <fltk3/RoundButton.h>

/**
  Creates a new fltk3::RoundButton widget using the given
  position, size, and label string.
*/
fltk3::RoundButton::RoundButton(int X,int Y,int W,int H, const char *l)
: fltk3::LightButton(X,Y,W,H,l) {
  box(fltk3::NO_BOX);
  down_box(fltk3::ROUND_DOWN_BOX);
  selection_color(fltk3::FOREGROUND_COLOR);
}

//
// End of "$Id$".
//
