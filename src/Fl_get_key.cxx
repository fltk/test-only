//
// "$Id: Fl_get_key.cxx,v 1.15 2002/12/10 01:46:18 easysw Exp $"
//
// Keyboard state routines for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifdef _WIN32
# include "Fl_get_key_win32.cxx"
#elif (defined(__APPLE__) && !USE_X11)
# include "Fl_get_key_mac.cxx"
#else

// Return the current state of a key.  This is the X version.  I identify
// keys (mostly) by the X keysym.  So this turns the keysym into a keycode
// and looks it up in the X key bit vector, which x.C keeps track of.

#include <fltk/events.h>
#include <fltk/x.h>

extern char fl_key_vector[32]; // in x.C

bool fltk::event_key_state(int keysym) {
  if (keysym > 0 && keysym <= 8)
    return event_state(BUTTON(keysym)) != 0;
  int keycode = XKeysymToKeycode(xdisplay, keysym);
  if (!keycode) {
#ifdef __sgi
    // get some missing PC keyboard keys:
    if (keysym == LeftCommandKey) keycode = 147;
    else if (keysym == RightCommandKey) keycode = 148;
    else if (keysym == MenuKey) keycode = 149;
    else
#endif
      keycode = keysym & 0xff; // undo the |0x8000 done to unknown keycodes
  }
  return (fl_key_vector[keycode/8] & (1 << (keycode%8))) != 0;
}

bool fltk::get_key_state(int k) {
  open_display();
  XQueryKeymap(xdisplay, fl_key_vector);
  return event_key_state(k);
}

#endif

//
// End of "$Id: Fl_get_key.cxx,v 1.15 2002/12/10 01:46:18 easysw Exp $".
//
