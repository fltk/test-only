//
// "$Id$"
//
// Shortcut support routines for the Fast Light Tool Kit (FLTK).
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

// Code to test and parse fltk shortcut numbers.
//
// A shortcut is a keysym or'd with shift flags.  In the simplest
// sense a shortcut is matched if the shift state is exactly as
// given and the key returning that keysym is pressed.
//
// To make it easier to match some things it is more complex:
//
// Only fltk3::META, fltk3::ALT, fltk3::SHIFT, and fltk3::CTRL must be "off".  A
// zero in the other shift flags indicates "dont care".
//
// It also checks against the first character of fltk3::event_text(),
// and zero for fltk3::SHIFT means "don't care".
// This allows punctuation shortcuts like "#" to work (rather than
// calling it "shift+3" on a US keyboard)

#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/Button.h>
#include <fltk3/draw.h>
#include <ctype.h>
#include "flstring.h"
#if !defined(WIN32) && !defined(__APPLE__)
#include <fltk3/x.H>
#endif

/**
    Test the current event, which must be an fltk3::KEY or 
    fltk3::SHORTCUT, against a shortcut value (described in 
    fltk3::Button).  Returns non-zero if there is a match.  Not to
    be confused with 
    fltk3::Widget::test_shortcut().
*/
int fltk3::test_shortcut(unsigned int shortcut) {
  if (!shortcut) return 0;

  unsigned int v = shortcut & fltk3::KEY_MASK;
  if (((unsigned)fl_tolower(v))!=v) {
    shortcut |= fltk3::SHIFT;
  }

  int shift = fltk3::event_state();
  // see if any required shift flags are off:
  if ((shortcut&shift) != (shortcut&0x7fff0000)) return 0;
  // record shift flags that are wrong:
  int mismatch = (shortcut^shift)&0x7fff0000;
  // these three must always be correct:
  if (mismatch&(fltk3::META|fltk3::ALT|fltk3::CTRL)) return 0;

  unsigned int key = shortcut & fltk3::KEY_MASK;

  // if shift is also correct, check for exactly equal keysyms:
  if (!(mismatch&(fltk3::SHIFT)) && key == (unsigned)fltk3::event_key()) return 1;

  // try matching utf8, ignore shift:
  unsigned int firstChar = fl_utf8decode(fltk3::event_text(), fltk3::event_text()+fltk3::event_length(), 0);
  if (key==firstChar) return 1;

  // kludge so that Ctrl+'_' works (as opposed to Ctrl+'^_'):
  if ((shift&fltk3::CTRL) && key >= 0x3f && key <= 0x5F
      && firstChar==(key^0x40)) return 1; // firstChar should be within a-z
  return 0;
}

// This table must be in numeric order by fltk (X) keysym number:
struct Keyname {unsigned int key; const char* name;};
#if defined(WIN32)
static Keyname table[] = {
  {' ', "Space"},
  {fltk3::BackSpaceKey, "Backspace"},
  {fltk3::TabKey,	"Tab"},
  {0xff0b/*XK_Clear*/, "Clear"},
  {fltk3::EnterKey,	"Enter"}, // X says "Enter"
  {fltk3::PauseKey,	"Pause"},
  {fltk3::ScrollLockKey, "Scroll_Lock"},
  {fltk3::EscapeKey,	"Escape"},
  {fltk3::HomeKey,	"Home"},
  {fltk3::LeftKey,	"Left"},
  {fltk3::UpKey,	"Up"},
  {fltk3::RightKey,	"Right"},
  {fltk3::DownKey,	"Down"},
  {fltk3::PageUpKey,	"Page_Up"}, // X says "Prior"
  {fltk3::PageDownKey,"Page_Down"}, // X says "Next"
  {fltk3::EndKey,	"End"},
  {fltk3::PrintKey,	"Print"},
  {FL_Insert,	"Insert"},
  {fltk3::MenuKey,	"Menu"},
  {fltk3::NumLockKey,	"Num_Lock"},
  {fltk3::Keypad_Enter,	"KP_Enter"},
  {fltk3::LeftShiftKey,	"Shift_L"},
  {fltk3::RightShiftKey,	"Shift_R"},
  {fltk3::LeftControlKey,"Control_L"},
  {fltk3::RightControlKey,"Control_R"},
  {fltk3::CapsLockKey,"Caps_Lock"},
  {fltk3::LeftMetaKey,	"Meta_L"},
  {fltk3::RightMetaKey,	"Meta_R"},
  {fltk3::LeftAltKey,	"Alt_L"},
  {fltk3::RightAltKey,	"Alt_R"},
  {fltk3::DeleteKey,	"Delete"}
};
#elif defined(__APPLE__) 
static Keyname table[] = {
                                 // v - this column contains utf8 characters
  {' ', "Space"},
  {fltk3::BackSpaceKey,"\xe2\x8c\xab"}, // ⌫  erase to the left
  {fltk3::TabKey,	"\xe2\x87\xa5"}, // ⇥  rightwards arrow to bar
  {0xff0b,      "\xe2\x8c\xa6"}, // ⌦  erase to the right
  {fltk3::EnterKey,	"\xe2\x86\xa9"}, // ↩  leftwards arrow with hook
  {fltk3::PauseKey,	"Pause"},
  {fltk3::ScrollLockKey, "Scroll_Lock"},
  {fltk3::EscapeKey,	"\xe2\x90\x9b"},
  {fltk3::HomeKey,	"\xe2\x86\x96"}, // ↖  north west arrow
  {fltk3::LeftKey,	"\xe2\x86\x90"}, // ←  leftwards arrow
  {fltk3::UpKey,	"\xe2\x86\x91"}, // ↑  upwards arrow
  {fltk3::RightKey,	"\xe2\x86\x92"}, // →  rightwards arrow
  {fltk3::DownKey,	"\xe2\x86\x93"}, // ↓  downwards arrow
  {fltk3::PageUpKey,	"\xe2\x87\x9e"}, // ⇞  upwards arrow with double stroke
  {fltk3::PageDownKey,"\xe2\x87\x9f"}, // ⇟  downward arrow with double stroke
  {fltk3::EndKey,	"\xe2\x86\x98"}, // ↘  south east arrow
  {fltk3::PrintKey,	"Print"},
  {fltk3::InsertKey,	"Insert"},
  {fltk3::MenuKey,	"Menu"},
  {fltk3::NumLockKey,	"Num_Lock"},
  {fltk3::KeypadEnter,	"\xe2\x8c\xa4"}, // ⌤ up arrow head between two horizontal bars
  {fltk3::LeftShiftKey,	"Shift_L"},
  {fltk3::RightShiftKey,	"Shift_R"},
  {fltk3::LeftControlKey,"Control_L"},
  {fltk3::RightControlKey,"Control_R"},
  {fltk3::CapsLockKey,"\xe2\x87\xaa"}, // ⇪  upwards white arrow from bar
  {fltk3::LeftMetaKey,	"Meta_L"},
  {fltk3::RightMetaKey,	"Meta_R"},
  {fltk3::LeftAltKey,	"Alt_L"},
  {fltk3::RightAltKey,	"Alt_R"},
  {fltk3::DeleteKey,	"\xe2\x8c\xa7"}  // ⌧  x in a rectangle box
};
#endif

/**
  Get a human-readable string from a shortcut value.

  Unparse a shortcut value as used by fltk3::Button or fltk3::MenuItem into
  a human-readable string like "Alt+N". This only works if the shortcut
  is a character key or a numbered function key. If the shortcut is
  zero then an empty string is returned. The return value points at
  a static buffer that is overwritten with each call.

  \param [in] shortcut the integer value containing the ascii charcter or extended keystroke plus modifiers
  \return a pointer to a static buffer containing human readable text for the shortcut
  */
const char* fl_shortcut_label(unsigned int shortcut) {
  return fl_shortcut_label(shortcut, 0L);
}

/** 
  Get a human-readable string from a shortcut value.

  \param [in] shortcut the integer value containing the ascii charcter or extended keystroke plus modifiers
  \param [in] eom if this pointer is set, it will receive a pointer to the end of the modifier text
  \return a pointer to a static buffer containing human readable text for the shortcut
  \see fl_shortcut_label(unsigned int shortcut)
  */
const char* fl_shortcut_label(unsigned int shortcut, const char **eom) {
  static char buf[20];
  char *p = buf;
  if (eom) *eom = p;
  if (!shortcut) {*p = 0; return buf;}
  // fix upper case shortcuts
  unsigned int v = shortcut & fltk3::KEY_MASK;
  if (((unsigned)fl_tolower(v))!=v) {
    shortcut |= fltk3::SHIFT;
  }
#ifdef __APPLE__
  //                        this column contains utf8 characters - v
  if (shortcut & fltk3::SHIFT) {strcpy(p,"\xe2\x87\xa7"); p += 3;}  // ⇧  upwards white arrow
  if (shortcut & fltk3::CTRL)  {strcpy(p,"\xe2\x8c\x83"); p += 3;}  // ⌃  up arrowhead
  if (shortcut & fltk3::ALT)   {strcpy(p,"\xe2\x8c\xa5"); p += 3;}  // ⌥  alternative key symbol
  if (shortcut & fltk3::META)  {strcpy(p,"\xe2\x8c\x98"); p += 3;}  // ⌘  place of interest sign
#else
  if (shortcut & fltk3::META) {strcpy(p,"Meta+"); p += 5;}
  if (shortcut & fltk3::ALT) {strcpy(p,"Alt+"); p += 4;}
  if (shortcut & fltk3::SHIFT) {strcpy(p,"Shift+"); p += 6;}
  if (shortcut & fltk3::CTRL) {strcpy(p,"Ctrl+"); p += 5;}
#endif // __APPLE__
  if (eom) *eom = p;
  unsigned int key = shortcut & fltk3::KEY_MASK;
#if defined(WIN32) || defined(__APPLE__) // if not X
  if (key >= fltk3::FKey && key <= fltk3::LastFunctionKey) {
    *p++ = 'F';
    if (key > fltk3::FKey+9) *p++ = (key-fltk3::FKey)/10+'0';
    *p++ = (key-fltk3::FKey)%10 + '0';
  } else {
    // binary search the table for a match:
    int a = 0;
    int b = sizeof(table)/sizeof(*table);
    while (a < b) {
      int c = (a+b)/2;
      if (table[c].key == key) {
        if (p > buf) {
          strcpy(p,table[c].name); 
          return buf;
        } else {
          const char *sp = table[c].name;
          if (eom) *eom = sp;
          return sp;
        }
      }
      if (table[c].key < key) a = c+1;
      else b = c;
    }
    if (key >= fltk3::Keypad && key <= fltk3::KeypadLast) {
      // mark keypad keys with KP_ prefix
      strcpy(p,"KP_"); p += 3;
      *p++ = uchar(key & 127);
    } else {
      // if none found, use the keystroke as a match:
      p += fl_utf8encode(fl_toupper(key), p); 
    }
  }
  *p = 0;
  return buf;
#else
  const char* q;
  if (key == fltk3::EnterKey || key == '\r') q="Enter";  // don't use Xlib's "Return":
  else if (key > 32 && key < 0x100) q = 0;
  else q = XKeysymToString(key);
  if (!q) {
    p += fl_utf8encode(fl_toupper(key), p); 
    *p = 0; 
    return buf;
  }
  if (p > buf) {
    strcpy(p,q); 
    return buf;
  } else {
    if (eom) *eom = q;
    return q;
  }
#endif
}

// Emulation of XForms named shortcuts
#include <stdlib.h>
/**
  Emulation of XForms named shortcuts.
*/
unsigned int fl_old_shortcut(const char* s) {
  if (!s || !*s) return 0;
  unsigned int n = 0;
  if (*s == '#') {n |= fltk3::ALT; s++;}
  if (*s == '+') {n |= fltk3::SHIFT; s++;}
  if (*s == '^') {n |= fltk3::CTRL; s++;}
  if (*s && s[1]) return n | (int)strtol(s,0,0); // allow 0xf00 to get any key
  return n | *s;
}

// Tests for &x shortcuts in button labels:

unsigned int fltk3::Widget::label_shortcut(const char *t) {
  if (!t) return 0;
  for (;;) {
    if (*t==0) return 0;
    if (*t=='&') {
      unsigned int s = fl_utf8decode(t+1, 0, 0);
      if (s==0) return 0;
      else if (s==(unsigned int)'&') t++;
      else return s;
    }
    t++;
  }
}

int fltk3::Widget::test_shortcut(const char *t) {
  #ifdef WIN32
  // on MSWindows, users expect shortcuts to work only when the Alt modifier is pressed
  if (fltk3::event_state(fltk3::ALT)==0) return 0;
  #endif
  if (!t) return 0;
  unsigned int c = fl_utf8decode(fltk3::event_text(), fltk3::event_text()+fltk3::event_length(), 0);
  if (!c) return 0;
  if (c == label_shortcut(t))
    return 1;
  return 0;
}

int fltk3::Widget::test_shortcut() {
  if (!(flags()&SHORTCUT_LABEL)) return 0;
  return test_shortcut(label());
}

//
// End of "$Id$".
//
