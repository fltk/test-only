//
// "$Id: Fl_compose.cxx,v 1.12 2001/11/29 17:39:29 spitzak Exp $"
//
// Character compose processing for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

// Before searching anything the following conversions are made:
// '"', ';' -> ":"     "/" -> "|"    "=",'_' -> "-"

// This table starts at character 0xA0 (non-breaking space)
// The characters may be typed in either order after the compose key.
// If the second character is a space then only the first character
// needs to be typed.
// I changed these slightly from fltk 1.0 to match X compose
// sequences in cases when my version did not use the same characters
// as the X sequence. Comments show the original versions.

static const char compose_pairs[] = {
  "  "	// nbsp
  "! "	// inverted !
  "c|"	// cent		 (was "% ")
  "l-"	// pound	 (was "# ")
  "xo"	// currency	 (was "$ ")
  "y-"	// yen
  "| "	// broken bar
  "s "	// section	 (was "& ", X uses "so")
  ": "	// dieresis
  "c "	// copyright	 (X uses "co")
  "a "	// superscript a (X uses "a-")
  "<<"	// <<
  "-,"	// not sign	 (was "~ ")
  "- "	// hyphen
  "r "	// registered	 (X uses "ro")
  "--"	// macron	 (was "_ ", X uses "-^")
  "0 "	// superscript 0 (degree, was "* ", X uses "0^")
  "+-"	// plusminus
  "2 "	// superscript 2 (X uses "2^")
  "3 "	// superscript 3 (X uses "3^")
  "' "	// acute
  "u "	// mu
  "p "	// paragraph
  ". "	// centered dot
  ", "	// cedilla
  "1 "	// superscript 1 (X uses "1^")
  "o "	// superscript o (X uses "o-")
  ">>"	// >>
  "14"	// 1/4
  "12"	// 1/2
  "34"	// 3/4
  "? "	// inverted ?
  "`A"
  "'A"
  "^A"
  "~A"
  ":A"
  "*A"
  "AE"
  ",C"
  "`E"
  "'E"
  "^E"
  ":E"
  "`I"
  "'I"
  "^I"
  ":I"
  "-D"
  "~N"
  "`O"
  "'O"
  "^O"
  "~O"
  ":O"
  "x "	// multiply
  "O|"
  "`U"
  "'U"
  "^U"
  ":U"
  "'Y"
  "TH"
  "ss"
  "`a"
  "'a"
  "^a"
  "~a"
  ":a"
  "*a"
  "ae"
  ",c"
  "`e"
  "'e"
  "^e"
  ":e"
  "`i"
  "'i"
  "^i"
  ":i"
  "-d"
  "~n"
  "`o"
  "'o"
  "^o"
  "~o"
  ":o"
  "-:"	// divide
  "o|"
  "`u"
  "'u"
  "^u"
  ":u"
  "'y"
  "th"
  ":y"
};

#ifndef _WIN32 // X only
// X dead-key lookup table.  This turns a dead-key keysym into the
// first of two characters for one of the compose sequences.  These
// keysyms start at 0xFE50.
// Win32 handles the dead keys before fltk can see them.  This is
// unfortunate, because you don't get the preview effect.
static const char dead_keys[] = {
  '`',	// XK_dead_grave
  '\'',	// XK_dead_acute
  '^',	// XK_dead_circumflex
  '~',	// XK_dead_tilde
  '_',	// XK_dead_macron
  0,	// XK_dead_breve
  '.',	// XK_dead_abovedot
  ':',	// XK_dead_diaeresis
  '*',	// XK_dead_abovering
  0,	// XK_dead_doubleacute
  'v',	// XK_dead_caron
  ','	// XK_dead_cedilla
//   0,	// XK_dead_ogonek
//   0,	// XK_dead_iota
//   0,	// XK_dead_voiced_sound
//   0,	// XK_dead_semivoiced_sound
//   0	// XK_dead_belowdot
};
#endif

int Fl::compose_state;

bool Fl::compose(int& del) {

  del = 0;
  char ascii = e_text[0];
  if      (ascii == '"' || ascii == ';') ascii = ':';
  else if (ascii == '/') ascii = '|';
  else if (ascii == '_' || ascii == '=') ascii = '-';

  // Alt+letters are reserved for shortcuts.  But alt+foreign letters
  // has to be allowed, because some key layouts require alt to be held
  // down in order to type them...
  if ((e_state & (FL_ALT|FL_WIN)) && !(ascii & 128)) return false;

  if (compose_state == 1) { // after the compose key
    
    // see if it is either character of any pair:
    for (const char *p = compose_pairs; *p; p += 2) 
      if (p[0] == ascii || p[1] == ascii) {
	compose_state = ascii;
	// prefer the single-character versions:
	if (p[1] == ' ') {e_text[0] = (p-compose_pairs)/2+0xA0; return true;}
      }
    if (compose_state != 1) return true;

    if (e_length) { // compose key also "quotes" control characters
      compose_state = 0;
      return true;
    }

  } else if (compose_state) { // second character of compose

    char c1 = char(compose_state); // retrieve first character
    // now search for the pair in either order:
    for (const char *p = compose_pairs; *p; p += 2) {
      if (p[0] == ascii && p[1] == c1 || p[1] == ascii && p[0] == c1) {
	e_text[0] = (p-compose_pairs)/2+0xA0;
	del = 1; // delete the old character and insert new one
	compose_state = 0;
	return true;
      }
    }

  }

  int i = e_keysym;

  // See if they type the compose prefix key:
  if (i == FL_Control_R || i == 0xff20/* Multi-Key */) {
    compose_state = 1;
    return true;
  }

#ifndef _WIN32 // X only
  // See if they typed a dead key.  This gets it into the same state as
  // typing prefix+accent:
  if (i >= 0xfe50 && i <= 0xfe5b) {
    ascii = dead_keys[i-0xfe50];
    for (const char *p = compose_pairs; *p; p += 2)
      if (p[0] == ascii) {
      compose_state = ascii;
      return true;
    }
    compose_state = 0;
    return true;
  }
#endif

  // Only insert non-control characters:
  if (e_length && (ascii & ~31 && ascii != 127)) {
    compose_state = 0;
    return true;
  }

  return false;
}

