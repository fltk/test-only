//
// "$Id: Fl_get_key.cxx,v 1.21 2004/07/25 23:22:14 spitzak Exp $"
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

#include <config.h>
#if !USE_X11
# ifdef _WIN32
#  include "Fl_get_key_win32.cxx"
# elif defined(__APPLE__)
#  include "Fl_get_key_mac.cxx"
# else
#  error
# endif
#else // X11 version:

// Return the current state of a key.  This is the X version.  I identify
// keys (mostly) by the X keysym.  So this turns the keysym into a keycode
// and looks it up in the X key bit vector, which x.C keeps track of.

#include <fltk/events.h>
#include <fltk/x.h>

extern char fl_key_vector[32]; // in x.C

/*! 
  Returns true if the given key was held down (or pressed) during the
  last event. This is constant until the next event is read from the
  server. The possible values for the key are listed under fltk::SpaceKey.

  On Win32 event_key_state(KeypadEnter) does not work.
*/
bool fltk::event_key_state(int keysym) {
  if (keysym > 0 && keysym <= 8)
    return event_state(BUTTON(keysym)) != 0;
  int keycode;
  // Newer X servers have somehow mapped both Meta and Alt to the Alt
  // key, and put Super on the Windows key. Detect and work around this:
  // Even newer X servers are further screwed up. What the hell are
  // they thinking??
  if (keysym == LeftMetaKey) {
    keycode = XKeysymToKeycode(xdisplay, 0xffeb); // try XK_Super_L
    if (keycode) goto DONE;
    keycode = XKeysymToKeycode(xdisplay, F0Key+13); // try F13 (!)
    if (keycode && !XKeysymToKeycode(xdisplay, F0Key+14)) goto DONE;
  } else if (keysym == RightMetaKey) {
    keycode = XKeysymToKeycode(xdisplay, 0xffec); // try XK_Super_R
    if (keycode) goto DONE;
    keycode = XKeysymToKeycode(xdisplay, 0xff20); // try XK_Multi_key
    if (keycode) goto DONE;
  } else if (keysym == RightAltKey) {
    keycode = XKeysymToKeycode(xdisplay, 0xff7e); // try XK_Mode_switch
    if (keycode) goto DONE;
  }    
  keycode = XKeysymToKeycode(xdisplay, keysym);
  if (!keycode) {
#ifdef __sgi
    // get some missing PC keyboard keys:
    if (keysym == LeftMetaKey) keycode = 147;
    else if (keysym == RightMetaKey) keycode = 148;
    else if (keysym == MenuKey) keycode = 149;
    else
#endif
      keycode = keysym & 0xff; // undo the |0x8000 done to unknown keycodes
  }
 DONE:
  return (fl_key_vector[keycode/8] & (1 << (keycode%8))) != 0;
}

/*!
  Returns true if the given key is held down now. Under X this
  requires a round-trip to the server and is much slower than
  fltk::event_key_state(int). The values to pass are described
  under fltk::SpaceKey.

  On Win32 fltk::get_key_state(fltk::KeypadEnter) does not work. 
*/
bool fltk::get_key_state(int k) {
  open_display();
  XQueryKeymap(xdisplay, fl_key_vector);
  return event_key_state(k);
}

#endif

//
// End of "$Id: Fl_get_key.cxx,v 1.21 2004/07/25 23:22:14 spitzak Exp $".
//
