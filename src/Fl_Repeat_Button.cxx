//
// "$Id$"
//
// Repeat button widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/RepeatButton.h>

#define INITIALREPEAT .5
#define REPEAT .1

void fltk3::RepeatButton::repeat_callback(void *v) {
  fltk3::Button *b = (fltk3::Button*)v;
  fltk3::add_timeout(REPEAT,repeat_callback,b);
  b->do_callback();
}

int fltk3::RepeatButton::handle(int event) {
  FLTK3_OBJECT_VCALLS_WRAPPER_RET(int, handle(event), Handle)
  int newval;
  switch (event) {
  case fltk3::HIDE:
  case fltk3::DEACTIVATE:
  case fltk3::RELEASE:
    newval = 0; goto J1;
  case fltk3::PUSH:
  case fltk3::DRAG:
    if (fltk3::visible_focus()) fltk3::focus(this);
    newval = fltk3::event_inside(this);
  J1:
    if (!active()) 
      newval = 0;
    if (value(newval)) {
      if (newval) {
	fltk3::add_timeout(INITIALREPEAT,repeat_callback,this);
	do_callback();
      } else {
	fltk3::remove_timeout(repeat_callback,this);
      }
    }
    return 1;
  default:
    return Button::handle(event);
  }
}

//
// End of "$Id$".
//
