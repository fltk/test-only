//
// "$Id$"
//
// MacOS keyboard state routines for the Fast Light Tool Kit (FLTK).
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

// Return the current state of a key.  Keys are named by fltk symbols,
// which are actually X keysyms.  So this has to translate to macOS
// symbols.

#include <fltk3/run.h>
#include <fltk3/x.H>
#include <config.h>

// convert an FLTK (X) keysym to a MacOS symbol:
// See also the inverse converter in Fl_mac.cxx
// This table is in numeric order by FLTK symbol order for binary search:

static const struct {unsigned short vk, fltk;} vktab[] = {
  { 49, ' ' }, { 39, '\'' }, { 43, ',' }, { 27, '-' }, { 47, '.' }, { 44, '/' }, 
  { 29, '0' }, { 18, '1'  }, { 19, '2'  }, { 20, '3'  }, 
  { 21, '4' }, { 23, '5'  }, { 22, '6'  }, { 26, '7'  }, 
  { 28, '8' }, { 25, '9'  }, { 41, ';'  }, { 24, '='  },
  {  0, 'A' }, { 11, 'B'  }, {  8, 'C'  }, {  2, 'D'  }, 
  { 14, 'E' }, {  3, 'F'  }, {  5, 'G'  }, {  4, 'H'  }, 
  { 34, 'I' }, { 38, 'J'  }, { 40, 'K'  }, { 37, 'L'  }, 
  { 46, 'M' }, { 45, 'N'  }, { 31, 'O'  }, { 35, 'P'  }, 
  { 12, 'Q' }, { 15, 'R'  }, {  1, 'S'  }, { 17, 'T'  }, 
  { 32, 'U' }, {  9, 'V'  }, { 13, 'W'  }, {  7, 'X'  }, 
  { 16, 'Y' }, {  6, 'Z'  }, 
  { 33, '[' }, { 30, ']' }, { 50, '`' },  { 42, '|' },
  { 51, fltk3::BackSpaceKey }, { 48, fltk3::TabKey }, { 36, fltk3::EnterKey }, { 127, fltk3::PauseKey },
  { 107, fltk3::ScrollLockKey }, { 53, fltk3::EscapeKey }, { 0x73, fltk3::HomeKey }, { 123, fltk3::LeftKey },
  { 126, fltk3::UpKey }, { 124, fltk3::RightKey }, { 125, fltk3::DownKey }, { 0x74, fltk3::PageUpKey },
  { 0x79, fltk3::PageDownKey },  { 119, fltk3::EndKey }, { 0x71, fltk3::PrintKey }, { 127, fltk3::InsertKey },
  { 0x6e, fltk3::MenuKey }, { 114, fltk3::HelpKey }, /*{ 0x47, fltk3::NumLockKey },*/
  { 76, fltk3::KeypadEnter }, { 67, fltk3::Keypad+'*' }, { 69, fltk3::Keypad+'+'}, { 78, fltk3::Keypad+'-' }, { 65, fltk3::Keypad+'.' }, { 75, fltk3::Keypad+'/' }, 
  { 82, fltk3::Keypad+'0' }, { 83, fltk3::Keypad+'1' }, { 84, fltk3::Keypad+'2' }, { 85, fltk3::Keypad+'3' }, 
  { 86, fltk3::Keypad+'4' }, { 87, fltk3::Keypad+'5' }, { 88, fltk3::Keypad+'6' }, { 89, fltk3::Keypad+'7' }, 
  { 91, fltk3::Keypad+'8' }, { 92, fltk3::Keypad+'9' }, { 81, fltk3::Keypad+'=' }, 
  { 0x7a, fltk3::FKey+1 }, { 0x78, fltk3::FKey+2  }, { 0x63, fltk3::FKey+3  }, { 0x76, fltk3::FKey+4  }, 
  { 0x60, fltk3::FKey+5 }, { 0x61, fltk3::FKey+6  }, { 0x62, fltk3::FKey+7  }, { 0x64, fltk3::FKey+8  }, 
  { 0x65, fltk3::FKey+9 }, { 0x6D, fltk3::FKey+10 }, { 0x67, fltk3::FKey+11 }, { 0x6f, fltk3::FKey+12 }, 
  { 56, fltk3::LeftShiftKey }, { 56, fltk3::RightShiftKey }, { 59, fltk3::LeftControlKey }, { 59, fltk3::RightControlKey }, 
  { 57, fltk3::CapsLockKey }, { 55, fltk3::LeftMetaKey }, { 55, fltk3::RightMetaKey },
  { 58, fltk3::LeftAltKey }, { 58, fltk3::RightAltKey }, /*{ 0x75, fltk3::DeleteKey },*/ { 0x47, fltk3::DeleteKey },
};

static int fltk2mac(int fltk) {
  int a = 0;
  int b = sizeof(vktab)/sizeof(*vktab);
  while (a < b) {
    int c = (a+b)/2;
    if (vktab[c].fltk == fltk) return vktab[c].vk;
    if (vktab[c].fltk < fltk) a = c+1; else b = c;
  }
  return 127;
}

//: returns true, if that key was pressed during the last event
int fltk3::event_key(int k) {
  return get_key(k);
}

#include <stdio.h>

//: returns true, if that key is pressed right now
int fltk3::get_key(int k) {
  KeyMap foo;
  GetKeys(foo);
#ifdef MAC_TEST_FOR_KEYCODES
 static int cnt = 0;
 if (cnt++>1024) {
  cnt = 0;
  printf("%08x %08x %08x %08x\n", (ulong*)(foo)[3], (ulong*)(foo)[2], (ulong*)(foo)[1], (ulong*)(foo)[0]);
 }
#endif
  unsigned char *b = (unsigned char*)foo;
  // KP_Enter can be at different locations for Powerbooks vs. desktop Macs
  if (k==fltk3::KeypadEnter) {
    return (((b[0x34>>3]>>(0x34&7))&1)||((b[0x4c>>3]>>(0x4c&7))&1));
  }
  int i = fltk2mac(k);
  return (b[i>>3]>>(i&7))&1;
}

//
// End of "$Id$".
//
