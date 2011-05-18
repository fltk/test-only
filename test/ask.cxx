//
// "$Id$"
//
// Standard dialog test program for the Fast Light Tool Kit (FLTK).
//
// Demonstrates how to use readqueue to see if a button has been
// pushed, and to see if a window has been closed, thus avoiding
// the need to define callbacks.
//
// This also demonstrates how to trap attempts by the user to
// close the last window by overriding Fl::exit
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

#include <stdio.h>
#include <string.h>
#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Input.h>
#include <fltk3/Button.h>
#include <fltk3/ReturnButton.h>

#include <fltk3/ask.h>
#include <stdlib.h>

void update_input_text(fltk3::Widget* o, const char *input) {
  if (input) {
    o->copy_label(input);
    o->redraw();
  }
}

void rename_me(fltk3::Widget*o) {
  const char *input = fltk3::input("Input:", o->label());
  update_input_text(o, input);
}

void rename_me_pwd(fltk3::Widget*o) {
  const char *input = fltk3::password("Input PWD:", o->label());
  update_input_text(o, input);
}

void window_callback(fltk3::Widget*, void*) {
  int hotspot = fltk3::message_hotspot();
  fltk3::message_hotspot(0);
  fltk3::message_title("note: no hotspot set for this dialog");
  int rep = fltk3::choice("Are you sure you want to quit?", 
		      "Cancel", "Quit", "Dunno");
  fltk3::message_hotspot(hotspot);
  if (rep==1) 
    exit(0);
  else if (rep==2)
    fltk3::message("Well, maybe you should know before we quit.");
}

int main(int argc, char **argv) {
  char buffer[128] = "Test text";
  char buffer2[128] = "MyPassword";

// this is a test to make sure automatic destructors work.  Pop up
// the question dialog several times and make sure it doesn't crash.
// fc: added more fltk3::ask common dialogs for test cases purposes.

  fltk3::DoubleWindow window(200, 105);
  fltk3::ReturnButton b(20, 10, 160, 35, buffer); b.callback(rename_me);
  fltk3::Button b2(20, 50, 160, 35, buffer2); b2.callback(rename_me_pwd);
  window.end();
  window.resizable(&b);
  window.show(argc, argv);

// Also we test to see if the exit callback works:
  window.callback(window_callback);

// set default message window title
  // fltk3::message_title_default("Default Window Title");

  return fltk3::run();
}
    
//
// End of "$Id$".
//
