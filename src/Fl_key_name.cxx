//
// "$Id: Fl_key_name.cxx,v 1.5 2001/11/29 17:39:30 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/fl_draw.h>
#include <ctype.h>
#include <string.h>
#ifndef _WIN32
#include <fltk/x.h>
#endif

// This table must be in numeric order by fltk (X) keysym number.
// On X the table is much shorter as it is only the names that
// are not returned correctly by XKeysymToString().
struct Keyname {int key; const char* name;};
static Keyname table[] = {
#ifdef _WIN32 // if not X
  {FL_BackSpace, "Backspace"},
  {FL_Tab,	"Tab"},
  {FL_Clear,	"Clear"},
#endif
  {FL_Enter,	"Enter"}, // X says "Return"
#ifdef _WIN32 // if not X
  {FL_Pause,	"Pause"},
  {FL_Scroll_Lock, "Scroll_Lock"},
  {FL_Escape,	"Escape"},
  {FL_Home,	"Home"},
  {FL_Left,	"Left"},
  {FL_Up,	"Up"},
  {FL_Right,	"Right"},
  {FL_Down,	"Down"},
#endif
  {FL_Page_Up,	"Page_Up"}, // X says "Prior"
  {FL_Page_Down,"Page_Down"}, // X says "Next"
#ifdef _WIN32 // if not X
  {FL_End,	"End"},
  {FL_Print,	"Print"},
  {FL_Insert,	"Insert"},
  {FL_Menu,	"Menu"},
  {FL_Num_Lock,	"Num_Lock"},
  {FL_KP_Enter,	"KP_Enter"},
  {FL_Shift_L,	"Shift_L"},
  {FL_Shift_R,	"Shift_R"},
  {FL_Control_L,"Control_L"},
  {FL_Control_R,"Control_R"},
  {FL_Caps_Lock,"Caps_Lock"},
  {FL_Alt_L,	"Alt_L"},
  {FL_Alt_R,	"Alt_R"},
#endif
  {FL_Win_L,	"Win_L"}, // X says "Super_L"
  {FL_Win_R,	"Win_R"}, // X says "Super_R"
#ifdef _WIN32 // if not X
  {FL_Delete,	"Delete"}
#endif
};

const char* Fl::key_name(int shortcut) {
  static char buf[20];
  char *p = buf;
  if (!shortcut) {*p = 0; return buf;}
  if (shortcut & FL_WIN) {strcpy(p,"Win+"); p += 5;}
  if (shortcut & FL_ALT) {strcpy(p,"Alt+"); p += 4;}
  if (shortcut & FL_SHIFT) {strcpy(p,"Shift+"); p += 6;}
  if (shortcut & FL_CTRL) {strcpy(p,"Ctrl+"); p += 5;}
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
    if (key >= FL_F(0) && key <= FL_F_Last) {
      *p++ = 'F';
      if (key > FL_F(9)) *p++ = (key-FL_F(0))/10+'0';
      *p++ = (key-FL_F(0))%10 + '0';
      *p = 0;
      return buf;
    }
    if (key >= FL_KP(0) && key <= FL_KP_Last) {
      // mark keypad keys with KP_ prefix
      strcpy(p,"KP_"); p += 3;
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
// End of "$Id: Fl_key_name.cxx,v 1.5 2001/11/29 17:39:30 spitzak Exp $"
//
