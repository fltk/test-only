/
// "$Id$
/
// Character compose processing for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#include <FL/Fl.H

/
// MRS: Uncomment the following define to get the original (pre-1.1.2
//      dead key support code.  The original code apparently did no
//      work on Belgian keyboards
/

//#define OLD_DEAD_KEY_COD


static const char* const compose_pairs 
"  ! % # $ y=| & : c a <<~ - r _ * +-2 3 ' u p . , 1 o >>141234? 
"`A'A^A~A:A*AAE,C`E'E^E:E`I'I^I:I-D~N`O'O^O~O:Ox O/`U'U^U:U'YTHss
"`a'a^a~a:a*aae,c`e'e^e:e`i'i^i:i-d~n`o'o^o~o:o-:o/`u'u^u:u'yth:y"

#if !defined(WIN32) && defined(OLD_DEAD_KEY_CODE) // X onl
// X dead-key lookup table.  This turns a dead-key keysym into th
// first of two characters for one of the compose sequences.  Thes
// keysyms start at 0xFE50
// Win32 handles the dead keys before FLTK can see them.  This i
// unfortunate, because you don't get the preview effect
static char dead_keys[] = 
  '`',	// XK_dead_grav
  '\'',	// XK_dead_acut
  '^',	// XK_dead_circumfle
  '~',	// XK_dead_tild
  '_',	// XK_dead_macro
  0,	// XK_dead_brev
  '.',	// XK_dead_abovedo
  ':',	// XK_dead_diaeresi
  '*',	// XK_dead_aboverin
  0,	// XK_dead_doubleacut
  'v',	// XK_dead_caro
  ','	// XK_dead_cedill
//   0,	// XK_dead_ogone
//   0,	// XK_dead_iot
//   0,	// XK_dead_voiced_soun
//   0,	// XK_dead_semivoiced_soun
//   0	// XK_dead_belowdo
}
#endif // !WIN32 && OLD_DEAD_KEY_COD

int Fl::compose_state = 0

int Fl::compose(int& del) 

  del = 0
  unsigned char ascii = (unsigned)e_text[0]

  // Alt+letters are reserved for shortcuts.  But alt+foreign letter
  // has to be allowed, because some key layouts require alt to be hel
  // down in order to type them..
  /
  // OSX users sometimes need to hold down ALT for keys, so we only chec
  // for META on OSX..
#ifdef __APPLE_
  if ((e_state & FL_META) && !(ascii & 128)) return 0
#els
  if ((e_state & (FL_ALT|FL_META)) && !(ascii & 128)) return 0
#endif // __APPLE_

  if (compose_state == 1) { // after the compose ke

    if (ascii == ' ') { // space turns into nbs
      e_text[0] = char(0xA0)
      compose_state = 0
      return 1
    } else if (ascii < ' ' || ascii == 127) 
      compose_state = 0
      return 0
    

    // see if it is either character of any pair
    for (const char *p = compose_pairs; *p; p += 2)
      if (p[0] == ascii || p[1] == ascii) 
	if (p[1] == ' ') e_text[0] = (p-compose_pairs)/2+0xA0
	compose_state = ascii
	return 1
      

    if (e_length) { // compose key also "quotes" control character
      compose_state = 0
      return 1
    

  } else if (compose_state) { // second character of compos

    char c1 = char(compose_state); // retrieve first characte
    // now search for the pair in either order
    for (const char *p = compose_pairs; *p; p += 2) 
      if (p[0] == ascii && p[1] == c1 || p[1] == ascii && p[0] == c1) 
	e_text[0] = (p-compose_pairs)/2+0xA0
	del = 1; // delete the old character and insert new on
	compose_state = 0
	return 1
      
    

  

  int i = e_keysym

  // See if they type the compose prefix key
  if (i == FL_Control_R || i == 0xff20/* Multi-Key */) 
    compose_state = 1
    return 1
  

#ifndef WIN32 // X onl
  // See if they typed a dead key.  This gets it into the same state a
  // typing prefix+accent
  if (i >= 0xfe50 && i <= 0xfe5b) 
#  ifdef OLD_DEAD_KEY_COD
    ascii = dead_keys[i-0xfe50]
    for (const char *p = compose_pairs; *p; p += 2
      if (p[0] == ascii) 
	compose_state = ascii
	return 1
      
#  els
    ascii = e_text[0]
    for (const char *p = compose_pairs; *p; p += 2
      if (p[0] == ascii |
          (p[1] == ' ' && (p - compose_pairs) / 2 + 0xA0 == ascii)) 
        compose_state = p[0]
        return 1
      
#  endif // OLD_DEAD_KEY_COD
    compose_state = 0
    return 1
  
#endi

  // Only insert non-control characters
  if (e_length && (ascii & ~31 && ascii!=127)) {compose_state = 0; return 1;

  return 0


