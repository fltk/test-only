//
// "$Id$"
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
#include <fltk/string.h>
#include <stdlib.h>
#if defined(_WIN32) && !defined(__CYGWIN__)
# include <direct.h>
//# define getcwd(a,b) _getdcwd(0,a,b)
#else
# include <unistd.h>
# ifdef __EMX__
#  define getcwd _getcwd2
# endif
#endif

#if defined(_WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
static inline bool isdirsep(char c) {return c=='/' || c=='\\';}
#else
#define isdirsep(c) ((c)=='/')
#endif

/*!
  Return the filename \a from with but expanded to a full path name
  by prepending the current directory or by prepending the "home"
  directory if the string starts with '~'.

  If there is no change \a from is returned. Otherwise perhaps the
  suffix of \a from is returned, or the result is copied to the \a
  buffer and a pointer to that is returned. \a len is the size of the
  buffer, the string is truncated to use at most len-1 characters and
  a null is always added. \a from and \a buffer must not be the same
  memory!

  On Unix only strings that start with a '/' are returned unchanged.
  On Windows any string starting with '/', '\\', or "x:" is returned
  unchanged.

  Leading "./" sequences are removed, and "../" sequences are removed
  as well as the matching trailing part of the prefixed directory.
  Technically this is incorrect if symbolic links are used but this
  matches the behavior of many programs.

  To expand a filename starting with ~ in the current directory
  you must start it with "./~".

  The optional \a pwd is a value to use for the current directory.
  If provided it is used and no ~ expansion is done.
*/
const char*
filename_normalize(const char* from, char* buffer, int length, const char* pwd)
{
  const char* prefix = 0;
  int prefixlen = 0;
  if (!pwd && from[0] == '~') {
    prefix = getenv("HOME");
    if (prefix && *prefix) {
      prefixlen = strlen(prefix);
      from++;
      if (isdirsep(*from)) {
	from++;
      } else if (*from) {
	// another user. Fake it for now by assumming it is at the same
	// level as the current user and has that user's name. The real
	// real way is to call getpwnam(name):
	while (prefixlen > 0 && !isdirsep(prefix[--prefixlen]));
      }
    }
  } else if (isdirsep(from[0]) /*|| from[0] == '|' // for tcl pipes? */
#if defined(_WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
	     || from[0] && from[1]==':'
#endif
	     ) {
    ;
  } else {
    // current directory
    if (pwd) prefix = pwd;
    else if ((prefix = getenv("PWD")));
    else prefix = getcwd(buffer, length);
    if (prefix) prefixlen = strlen(prefix);
  }
  while (*from == '.') {
    if (isdirsep(from[1])) {
      from += 2;
    } else if (from[1] == '.' && isdirsep(from[2])) {
      if (!prefixlen) break;
      while (prefixlen > 0 && !isdirsep(prefix[--prefixlen]));
      from += 3;
    } else break;
  }
  if (!prefix) return from;
  if (prefixlen > length-2) prefixlen = length-2;
  if (prefix != buffer) memcpy(buffer, prefix, prefixlen);
  if (!prefixlen || !isdirsep(prefix[prefixlen-1])) buffer[prefixlen++] = '/';
  strlcpy(buffer+prefixlen, from, length-prefixlen);
  return buffer;
}

/*! Back-compatability version of filename_normalize().
  This did not let you specify the size of the buffer, it did a useless
  copy in the common case that there was no change in the name.
  It also did not do the home-directory prefix.
*/
bool filename_absolute(char *to, const char *from, const char* pwd) {
  char temp[FL_PATH_MAX];
  if (from == to) {strlcpy(temp, from, FL_PATH_MAX); from = temp;}
  const char* t = filename_normalize(from, to, FL_PATH_MAX, pwd);
  if (t != to) {
    strlcpy(to, from, FL_PATH_MAX);
    return false;
  }
  return true;
}
  
//
// End of "$Id$".
//
