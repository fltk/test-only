//
// "$Id: Fl_get_key_mac.cxx,v 1.3 2002/12/09 04:52:27 spitzak Exp $"
//
// MacOS keyboard state routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

// Return the current state of a key.  Keys are named by fltk symbols,
// which are actually X keysyms.  So this has to translate to macOS
// symbols.

#include <fltk/events.h>
#include <fltk/x.h>

// convert an FLTK (X) keysym to a MacOS symbol:
// See also the inverse converter in mac.cxx
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
  { 51, BackSpace }, { 48, Tab }, { 36, Enter }, { 127, Pause },
  { 107, ScrollLockKey }, { 53, Escape }, { 0x73, Home }, { 123, Left },
  { 126, Up }, { 124, Right }, { 125, Down }, { 0x74, Page_Up },
  { 0x79, Page_Down },  { 119, End }, { 0x71, Print }, { 127, Insert },
  { 127, MenuKey }, { 114, Help }, { 0x47, Num_Lock }, 
  { 76, Keypad_Enter }, { 67, Keypad+'*' }, { 69, Keypad+'+'}, { 78, Keypad+'-' }, { 65, Keypad+'.' }, { 75, Keypad+'/' }, 
  { 82, Keypad+'0' }, { 83, Keypad+'1' }, { 84, Keypad+'2' }, { 85, Keypad+'3' }, 
  { 86, Keypad+'4' }, { 87, Keypad+'5' }, { 88, Keypad+'6' }, { 89, Keypad+'7' }, 
  { 91, Keypad+'8' }, { 92, Keypad+'9' }, { 81, Keypad+'=' }, 
  { 0x7a, F+1 }, { 0x78, F+2  }, { 0x63, F+3  }, { 0x76, F+4  }, 
  { 0x60, F+5 }, { 0x61, F+6  }, { 0x62, F+7  }, { 0x64, F+8  }, 
  { 0x65, F+9 }, { 0x6D, F+10 }, { 0x67, F+11 }, { 0x6f, F+12 }, 
  { 56, Shift_L }, { 56, Shift_R }, { 55, Control_L }, { 55, Control_R }, 
  { 57, Caps_Lock }, { 59, Meta_L }, { 59, Meta_R },
  { 58, Alt_L }, { 58, Alt_R },
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
int event_key(int k) {
  return get_key(k);
}

#include <stdio.h>

//: returns true, if that key is pressed right now
int get_key(int k) {
  KeyMap foo;
  GetKeys(foo);
#ifdef MAC_TEST_FOR_KEYCODES
 static int cnt = 0;
 if (cnt++>1024) {
  cnt = 0;
  printf("%08x %08x %08x %08x\n", (ulong*)(foo)[3], (ulong*)(foo)[2], (ulong*)(foo)[1], (ulong*)(foo)[0]);
 }
#endif
  int i = fltk2mac(k);
  unsigned char *b = (unsigned char*)foo;
  return (b[i>>3]>>(i&7))&1;
}

//
// End of "$Id: Fl_get_key_mac.cxx,v 1.3 2002/12/09 04:52:27 spitzak Exp $".
//
