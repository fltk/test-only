//
// "$Id: filename_match.cxx,v 1.10 2002/03/26 18:00:35 spitzak Exp $"
//
// Pattern matching routines for the Fast Light Tool Kit (FLTK).
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

/* Adapted from Rich Salz. */
#include <fltk/filename.h>
#include <ctype.h>

#define CASE_INSENSITIVE 1

bool filename_match(const char *s, const char *p) {
  int nesting;

  for (;;) {
    switch(*p++) {

    case '?' :	// match any single character
      if (!*s++) return false;
      break;

    case '*' :	// match 0-n of any characters
      if (!*p) return true; // do trailing * quickly
      while (!filename_match(s, p)) if (!*s++) return false;
      return true;

    case '[': {	// match one character in set of form [abc-d] or [^a-b]
      if (!*s) return false;
      bool reverse = (*p=='^' || *p=='!'); if (reverse) p++;
      bool matched = false;
      char last = 0;
      while (*p) {
	if (*p=='-' && last) {
	  if (*s <= *++p && *s >= last ) matched = true;
	  last = 0;
	} else {
	  if (*s == *p) matched = true;
	}
	last = *p++;
	if (*p==']') break;
      }
      if (matched == reverse) return false;
      s++; p++;}
    break;

    case '{' : // {pattern1|pattern2|pattern3}
    NEXTCASE:
    if (filename_match(s,p)) return true;
    for (nesting = 0;;) {
      switch (*p++) {
      case '\\': if (*p) p++; break;
      case '{': nesting++; break;
      case '}': if (!nesting--) return false; break;
      case '|': case ',': if (nesting==0) goto NEXTCASE;
      case 0: return false;
      }
    }
    case '|':	// skip rest of |pattern|pattern} when called recursively
    case ',':
      for (nesting = 0; *p && nesting >= 0;) {
	switch (*p++) {
	case '\\': if (*p) p++; break;
	case '{': nesting++; break;
	case '}': nesting--; break;
	}
      }
      break;
    case '}':
      break;

    case 0:	// end of pattern
      return !*s;

#if CASE_INSENSITIVE
    case '\\':	// quote next character
      if (*p) p++;
      if (*s++ != *(p-1)) return false;
      break;
    default:
      if (tolower(*s) != tolower(*(p-1))) return false;
      s++;
#else
    case '\\':	// quote next character
      if (*p) p++;
    default  :
      if (*s++ != *(p-1)) return false;
      break;
#endif
    }
  }
}

//
// End of "$Id: filename_match.cxx,v 1.10 2002/03/26 18:00:35 spitzak Exp $".
//
