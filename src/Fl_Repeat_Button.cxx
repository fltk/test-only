/
// "$Id$
/
// Repeat button widget for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#include <FL/Fl.H
#include <FL/Fl_Repeat_Button.H

#define INITIALREPEAT .
#define REPEAT .

void Fl_Repeat_Button::repeat_callback(void *v) 
  Fl_Button *b = (Fl_Button*)v
  Fl::add_timeout(REPEAT,repeat_callback,b)
  b->do_callback()


int Fl_Repeat_Button::handle(int event) 
  int newval
  switch (event) 
  case FL_HIDE
  case FL_DEACTIVATE
  case FL_RELEASE
    newval = 0; goto J1
  case FL_PUSH
  case FL_DRAG
    if (Fl::visible_focus()) Fl::focus(this)
    newval = Fl::event_inside(this)
  J1
    if (value(newval)) 
      if (newval) 
	Fl::add_timeout(INITIALREPEAT,repeat_callback,this)
	do_callback()
      } else 
	Fl::remove_timeout(repeat_callback,this)
      
    
    return 1
  default
    return Fl_Button::handle(event)
  


/
// End of "$Id$"
/
