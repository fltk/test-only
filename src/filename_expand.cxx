//
// "$Id: filename_expand.cxx,v 1.12 2004/01/20 07:27:28 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/filename.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) && !defined(__CYGWIN__)
#else
# include <unistd.h>
# include <pwd.h>
#endif

#if defined(_WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
static inline bool isdirsep(char c) {return c=='/' || c=='\\';}
#else
#define isdirsep(c) ((c)=='/')
#endif

/*!
  Expand a file name by substuting environment variables and
  home directories.  Returns true if any changes were made.

  - The text is split into "words" at each slash.
  - Replaces words of the form $X with getenv(X) (leaving it as $X if
    the variable does not exist)
  - Replaces ~X with users X's home directory, if user X exists (Unix only)
  - Replaces just ~ with $HOME
  - If a replacement is made and it starts with a slash, it deletes
    everything before it.

  The result is put into \a to, which must have space for 1024
  characters. \a to and \a from may be the same buffer.
*/
bool filename_expand(char *to, const char *from) {

  char temp[FL_PATH_MAX];
  // do a strlcpy():
  char* a = temp;
  const char* end = from;
  while (a < temp+FL_PATH_MAX-1 && *end) *a++ = *end++;
  *a = 0;
  end = a;

  bool ret = false;

  for (a=temp; a<end; ) {	// for each slash component
    char *e; for (e=a; e<end && !isdirsep(*e); e++); // find next slash
    const char *value = 0; // this will point at substitute value
    switch (*a) {
    case '~':	// a home directory name
      if (e <= a+1) {	// current user's directory
	value = getenv("HOME");
#ifndef _WIN32
      } else {	// another user's directory
	struct passwd *pwd;
	char t = *e; *(char *)e = 0; 
        pwd = getpwnam(a+1); 
        *(char *)e = t;
	    if (pwd) value = pwd->pw_dir;
#endif
      }
      break;
    case '$':		/* an environment variable */
      {char t = *e; *(char *)e = 0; value = getenv(a+1); *(char *)e = t;}
      break;
    }
    if (value) {
      // substitutions that start with slash delete everything before them:
      if (isdirsep(value[0])) a = temp;
#if defined(_WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
      // also if it starts with "A:"
      if (value[0] && value[1]==':') a = temp;
#endif
      int t = strlen(value); if (isdirsep(value[t-1])) t--;
      end = a+t+(end-e);
      if (end > temp+FL_PATH_MAX-1) {
	t -= end-(temp+FL_PATH_MAX-1);
	end = temp+FL_PATH_MAX-1;
      }
      memmove(a+t, e, end+1-(a+t));
      memcpy(a, value, t);
      ret = true;
    } else {
      a = e+1;
#if defined(_WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
      if (*e == '\\') {*e = '/'; ret = true;} // only forward slashes allowed
#endif
    }
  }
  if (ret || to != from) strcpy(to, temp);
  return ret;
}

//
// End of "$Id: filename_expand.cxx,v 1.12 2004/01/20 07:27:28 spitzak Exp $".
//
