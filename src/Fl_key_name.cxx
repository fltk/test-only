//
// "$Id: Fl_key_name.cxx,v 1.7 2002/12/09 04:52:27 spitzak Exp $"
//
// Turn a fltk (X) keysym + fltk shift flags into a human-readable string.
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

#include <fltk/events.h>
#include <fltk/Widget.h>
#include <fltk/draw.h>
#include <ctype.h>
#include <string.h>
#ifndef _WIN32
#include <fltk/x.h>
#endif
using namespace fltk;

// This table must be in numeric order by fltk (X) keysym number.
// On X the table is much shorter as it is only the names that
// are not returned correctly by XKeysymToString().
struct Keyname {int key; const char* name;};
static Keyname table[] = {
#ifdef _WIN32 // if not X
  {BackSpaceKey, "Backspace"},
  {TabKey,	"Tab"},
  {ClearKey,	"Clear"},
#endif
  {ReturnKey,	"Enter"}, // X says "Return"
#ifdef _WIN32 // if not X
  {PauseKey,	"Pause"},
  {ScrollLockKey,"ScrollLock"},
  {EscapeKey,	"Escape"},
  {HomeKey,	"Home"},
  {LeftKey,	"Left"},
  {UpKey,	"Up"},
  {RightKey,	"Right"},
  {DownKey,	"Down"},
#endif
  {PageUpKey,	"PageUp"}, // X says "Prior"
  {PageDownKey,	"PageDown"}, // X says "Next"
#ifdef _WIN32 // if not X
  {EndKey,	"End"},
  {PrintKey,	"Print"},
  {InsertKey,	"Insert"},
  {MenuKey,	"Menu"},
  {NumLockKey,	"NumLock"},
  {KeypadEnter,	"KeypadEnter"},
  {LeftShiftKey,"LeftShift"},
  {RightShiftKey,"RightShift"},
  {LeftControlKey, "LeftControl"},
  {RightControlKey,"RightControl"},
  {CapsLockKey,	"CapsLock"},
  {LeftAltKey,	"LeftAlt"},
  {RightAltKey,	"RightAlt"},
#endif
  {LeftCommandKey,	"LeftCommand"}, // X says "Super_L"
  {RightCommandKey,	"RightCommand"}, // X says "Super_R"
#ifdef _WIN32 // if not X
  {DeleteKey,	"Delete"}
#endif
};

const char* fltk::key_name(int shortcut) {
  static char buf[20];
  char *p = buf;
  if (!shortcut) {*p = 0; return buf;}
  if (shortcut & COMMAND) {strcpy(p,"Command+"); p += 5;}
  if (shortcut & ALT) {strcpy(p,"Alt+"); p += 4;}
  if (shortcut & SHIFT) {strcpy(p,"Shift+"); p += 6;}
  if (shortcut & CTRL) {strcpy(p,"Ctrl+"); p += 5;}
  int key = shortcut & 0xFFFF;

  // binary search the table for a match:
  int a = 0;
  int b = sizeof(table)/sizeof(*table);
  const char* q = 0;
  while (a < b) {
    int c = (a+b)/2;
    if (table[c].key == key) {q = table[c].name; break;}
    if (table[c].key < key) a = c+1;
    else b = c;
  }
  if (!q) {
#ifdef _WIN32 // if not X
    if (key >= F0Key && key <= LastFunctionKey) {
      *p++ = 'F';
      if (key > F9Key) *p++ = (key-F0Key)/10+'0';
      *p++ = (key-F0Key)%10 + '0';
      *p = 0;
      return buf;
    }
    if (key >= Keypad && key <= KeypadLast) {
      // mark keypad keys with Keypad prefix
      strcpy(p,"Keypad"); p += 6;
      *p++ = uchar(key & 127);
      *p = 0;
      return buf;
    }
#else
    if (key <= 32 || key >= 0x100) q = XKeysymToString(key);
#endif
  }
  if (q) {
    if (p == buf) return q;
    strcpy(p, q);
    return buf;
  }
  // if all else fails use the keysym as a character:
  *p++ = uchar(key);
  *p = 0;
  return buf;
}

//
// End of "$Id: Fl_key_name.cxx,v 1.7 2002/12/09 04:52:27 spitzak Exp $"
//
