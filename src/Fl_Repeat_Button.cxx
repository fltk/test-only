//
// "$Id: Fl_Repeat_Button.cxx,v 1.9 2002/09/16 00:29:06 spitzak Exp $"
//
// Repeat button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Repeat_Button.h>

#define INITIALREPEAT .5f
#define REPEAT .1f

void Fl_Repeat_Button::repeat_callback(void *v) {
  Fl_Button *b = (Fl_Button*)v;
  Fl::add_timeout(REPEAT,repeat_callback,b);
  b->do_callback();
}

int Fl_Repeat_Button::handle(int event) {
  bool newval;
  switch (event) {
  case FL_HIDE:
  case FL_DEACTIVATE:
  case FL_RELEASE:
    newval = false; goto J1;
  case FL_PUSH:
  case FL_DRAG:
    newval = Fl::event_inside(0, 0, w(), h());
  J1:
    if (value(newval)) {
      if (newval) {
	Fl::add_timeout(INITIALREPEAT,repeat_callback,this);
	do_callback();
      } else {
	Fl::remove_timeout(repeat_callback,this);
      }
    }
    return newval;
  default:
    return Fl_Button::handle(event);
  }
}

//
// End of "$Id: Fl_Repeat_Button.cxx,v 1.9 2002/09/16 00:29:06 spitzak Exp $".
//
