//
// "$Id: Fl_get_key.cxx,v 1.9 2001/11/29 17:39:29 spitzak Exp $"
//
// Keyboard state routines for the Fast Light Tool Kit (FLTK).
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

#ifdef _WIN32
#include "Fl_get_key_win32.cxx"
#else

// Return the current state of a key.  This is the X version.  I identify
// keys (mostly) by the X keysym.  So this turns the keysym into a keycode
// and looks it up in the X key bit vector, which Fl_x.C keeps track of.

#include <fltk/Fl.h>
#include <fltk/x.h>

extern char fl_key_vector[32]; // in Fl_x.C

bool Fl::event_key(int keysym) {
  if (keysym > FL_Button(0) && keysym <= FL_Button(8))
    return Fl::event_state(FL_BUTTON(keysym-FL_Button(0))) != 0;
  int keycode = XKeysymToKeycode(fl_display, keysym);
  if (!keycode) {
#ifdef __sgi
  // get some missing PC keyboard keys:
    if (keysym == FL_Win_L) keycode = 147;
    else if (keysym == FL_Win_R) keycode = 148;
    else if (keysym == FL_Menu) keycode = 149;
  else
#endif
      keycode = keysym & 0xff; // undo the |0x8000 done to unknown keycodes
  }
  return (fl_key_vector[keycode/8] & (1 << (keycode%8))) != 0;
}

bool Fl::get_key(int k) {
  fl_open_display();
  XQueryKeymap(fl_display, fl_key_vector);
  return event_key(k);
}

#endif

//
// End of "$Id: Fl_get_key.cxx,v 1.9 2001/11/29 17:39:29 spitzak Exp $".
//
