//
// "$Id$"
//
// Forms free widget routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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

// Emulation of the Forms "free" widget.
// This emulation allows the free demo to run, and has allowed
// me to port several other programs, but it is in no way
// complete.

#include <fltk3/run.h>
#include <fltk3/Fl_Free.H>

void Fl_Free::step(void *v) {
  Fl_Free *f = (Fl_Free *)v;
  int old_event = fltk3::e_number;
  f->handle(fltk3::e_number == FL_STEP);
  fltk3::e_number = old_event;
  fltk3::add_timeout(.01,step,v);
}

/**
  Create a new Fl_Free widget with type, position, size, label and handler.
  \param[in] t type
  \param[in] X, Y, W, H position and size
  \param[in] L widget label
  \param[in] hdl handler function

  The constructor takes both the type and the handle function. The handle
  function should be declared as follows:
  \code
  int handle_function(fltk3::Widget *w,
                      int       event,
     	              float     event_x,
    		      float     event_y,
    		      char      key)
  \endcode
  This function is called from the handle() method in response to most
  events, and is called by the draw() method.

  The event argument contains the event type:
  \code
  // old event names for compatibility:
  #define FL_MOUSE		fltk3::DRAG
  #define FL_DRAW		0
  #define FL_STEP		9
  #define FL_FREEMEM		12
  #define FL_FREEZE		FL_UNMAP
  #define FL_THAW		FL_MAP
  \endcode
*/
Fl_Free::Fl_Free(uchar t,int X, int Y, int W, int H,const char *L,
		 FL_HANDLEPTR hdl) :
fltk3::Widget(X,Y,W,H,L) {
  type(t);
  hfunc = hdl;
  if (t == FL_SLEEPING_FREE) set_flag(INACTIVE);
  if (t == FL_CONTINUOUS_FREE || t == FL_ALL_FREE)
    fltk3::add_timeout(.01,step,this);
}

/**
  The destructor will call the handle function with the event FL_FREE_MEM.
*/
Fl_Free::~Fl_Free() {
  fltk3::remove_timeout(step,this);
  hfunc(this,FL_FREEMEM,0,0,0);
}

void Fl_Free::draw() {hfunc(this,FL_DRAW,0,0,0);}

int Fl_Free::handle(int e) {
  char key = fltk3::event_key();
  switch (e) {
  case fltk3::FOCUS:
    if (type()!=FL_INPUT_FREE && type()!=FL_ALL_FREE) return 0;
    break;
  case fltk3::PUSH:
  case fltk3::DRAG:
  case fltk3::RELEASE:
    key = 4-fltk3::event_button();
    break;
  case fltk3::SHORTCUT:
    return 0;
  }
  if (hfunc(this, e, float(fltk3::event_x()), float(fltk3::event_y()), key)) do_callback();
  return 1;
}

//
// End of "$Id$".
//
