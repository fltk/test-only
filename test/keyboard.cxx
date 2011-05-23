//
// "$Id$"
//
// Keyboard/event test program for the Fast Light Tool Kit (FLTK).
//
// Continuously display FLTK's event state.
//
// Known bugs:
//
// X insists on reporting the state *before* the shift key was
// pressed, rather than after, on shift key events.  I fixed this for
// the mouse buttons, but it did not seem worth it for shift.
//
// X servers do not agree about any shift flags after except shift, ctrl,
// lock, and alt.  They may also not agree about the symbols for the extra
// keys Micro$oft put on the keyboard.
//
// On IRIX the backslash key does not work.  A bug in XKeysymToKeycode?
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


#include "keyboard_ui.h"

#include <string.h>


// these are used to identify which buttons are which:
void key_cb(fltk3::Button*, void*) {}
void shift_cb(fltk3::Button*, void*) {}
void wheel_cb(fltk3::Dial*, void*) {}

// this is used to stop Esc from exiting the program:
int handle(int e) {
  return (e == fltk3::SHORTCUT); // eat all keystrokes
}

int MyWindow::handle(int msg) {
  if (msg==fltk3::MOUSEWHEEL)
  {
    roller_x->value( roller_x->value() + fltk3::e_dx * roller_x->step() );
    roller_y->value( roller_y->value() + fltk3::e_dy * roller_y->step() );
    return 1;
  }
  return 0;
}

struct keycode_table{int n; const char* text;} table[] = {
  {fltk3::EscapeKey,      "fltk3::EscapeKey"},
  {fltk3::BackSpaceKey,   "fltk3::BackSpaceKey"},
  {fltk3::TabKey,         "fltk3::TabKey"},
  {fltk3::EnterKey,       "fltk3::EnterKey"},
  {fltk3::PrintKey,       "fltk3::PrintKey"},
  {fltk3::ScrollLockKey,  "fltk3::ScrollLockKey"},
  {fltk3::PauseKey,       "fltk3::PauseKey"},
  {fltk3::InsertKey,      "fltk3::InsertKey"},
  {fltk3::HomeKey,        "fltk3::HomeKey"},
  {fltk3::PageUpKey,      "fltk3::PageUpKey"},
  {fltk3::DeleteKey,      "fltk3::DeleteKey"},
  {fltk3::EndKey,         "fltk3::EndKey"},
  {fltk3::PageDownKey,    "fltk3::PageDownKey"},
  {fltk3::LeftKey,        "fltk3::LeftKey"},
  {fltk3::UpKey,          "fltk3::UpKey"},
  {fltk3::RightKey,       "fltk3::RightKey"},
  {fltk3::DownKey,        "fltk3::DownKey"},
  {fltk3::ShiftLKey,      "fltk3::ShiftLKey"},
  {fltk3::ShiftRKey,      "fltk3::ShiftRKey"},
  {fltk3::ControlLKey,    "fltk3::ControlLKey"},
  {fltk3::ControlRKey,    "fltk3::ControlRKey"},
  {fltk3::CapsLockKey,    "fltk3::CapsLockKey"},
  {fltk3::AltLKey,        "fltk3::AltLKey"},
  {fltk3::AltRKey,        "fltk3::AltRKey"},
  {fltk3::MetaLKey,       "fltk3::MetaLKey"},
  {fltk3::MetaRKey,       "fltk3::MetaRKey"},
  {fltk3::MenuKey,        "fltk3::MenuKey"},
  {fltk3::HelpKey,        "fltk3::HelpKey"},
  {fltk3::NumLockKey,     "fltk3::NumLockKey"},
  {fltk3::KPEnterKey,     "fltk3::KPEnterKey"}
};

int main(int argc, char** argv) {
  fltk3::add_handler(handle);
  MyWindow *window = make_window();
  window->show(argc,argv);
  while (fltk3::wait()) {
    const char *str;
    
    // update all the buttons with the current key and shift state:
    for (int i = 0; i < window->children(); i++) {
      fltk3::Widget* b = window->child(i);
      if (b->callback() == (fltk3::Callback*)key_cb) {
	int i = b->argument();
	if (!i) i = b->label()[0];
        fltk3::Button *btn = ((fltk3::Button*)b);
        int state = fltk3::event_key(i);
        if (btn->value()!=state)
	  btn->value(state);
      } else if (b->callback() == (fltk3::Callback*)shift_cb) {
	int i = b->argument();
        fltk3::Button *btn = ((fltk3::Button*)b);
        int state = fltk3::event_state(i);
        if (btn->value()!=state)
	  btn->value(state);
      }
    }

    // figure out the keyname:
    char buffer[100];
    const char *keyname = buffer;
    int k = fltk3::event_key();
    if (!k)
      keyname = "0";
    else if (k < 256) {
      sprintf(buffer, "'%c'", k);
    } else if (k > fltk3::FKey && k <= fltk3::FLastKey) {
      sprintf(buffer, "fltk3::FKey+%d", k - fltk3::FKey);
    } else if (k >= fltk3::KPKey && k <= fltk3::KPLastKey) {
      sprintf(buffer, "fltk3::KPKey+'%c'", k-fltk3::KPKey);
    } else if (k >= fltk3::MouseButton && k <= fltk3::MouseButton+7) {
      sprintf(buffer, "fltk3::MouseButton+%d", k-fltk3::MouseButton);
    } else {
      sprintf(buffer, "0x%04x", k);
      for (int i = 0; i < int(sizeof(table)/sizeof(*table)); i++)
	if (table[i].n == k) {keyname = table[i].text; break;}
    }
    if (strcmp(key_output->value(), keyname))
      key_output->value(keyname);

    str = fltk3::event_text();
    if (strcmp(text_output->value(), str))
      text_output->value(str);
  }
  return 0;
}

//
// End of "$Id$".
//
