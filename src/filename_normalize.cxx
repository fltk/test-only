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

/** \addtogroup utilities

  FLTK provides some functions that it uses internally that are
  necessary for portablity, or convienent for writing code.  These are
  \e not in the fltk:: namespace and do not have "fl_" in their names,
  because in theory they should not be part of fltk, but instead
  provided by the system.
*/

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

/**
  Return the filename \a from expanded to a full "absolute" path name
  by prepending the current directory or by prepending the "home"
  directory if it starts with '~'.
  - \a output is a pointer to a buffer that \a may be used to write
    the result to. It may also return a pointer into \a from.
  - \a length is the size of \a output. No more than n-1 characters
  are written, plus a trailing nul.
  - \a input is the initial filename.
  - \a directory is the directory that filename is relative to. If
  this is NULL thne the current directory is gotten from the OS.

  If there is no change \a input is returned. Otherwise perhaps the
  suffix of \a input is returned, or the result is copied to \a output
  and a pointer to that is returned.

  Leading "./" sequences in \a input are removed, and "../" sequences
  are removed as well as the matching trailing part of the prefixed
  directory.  Technically this is incorrect if symbolic links are used
  but this matches the behavior of most programs.

  To expand a filename starting with ~ in the current directory
  you must start it with "./~".
*/
const char*
filename_normalize(char* output, int length, const char* input, const char* pwd)
{
  const char* prefix = 0;
  int prefixlen = 0;
  if (!pwd && input[0] == '~') {
    prefix = getenv("HOME");
    if (prefix && *prefix) {
      prefixlen = strlen(prefix);
      input++;
      if (isdirsep(*input)) {
	input++;
      } else if (*input) {
	// another user. Fake it for now by assumming it is at the same
	// level as the current user and has that user's name. The real
	// real way is to call getpwnam(name):
	while (prefixlen > 0 && !isdirsep(prefix[--prefixlen]));
      }
    }
  } else if (isdirsep(input[0]) /*|| input[0] == '|' // for tcl pipes? */
#if defined(_WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
	     || input[0] && input[1]==':'
#endif
	     ) {
    ;
  } else {
    // current directory
    if (pwd) prefix = pwd;
    else if ((prefix = getenv("PWD")));
    else prefix = getcwd(output, length);
    if (prefix) prefixlen = strlen(prefix);
  }
  while (*input == '.') {
    if (isdirsep(input[1])) {
      input += 2;
    } else if (input[1] == '.' && isdirsep(input[2])) {
      if (!prefixlen) break;
      while (prefixlen > 0 && !isdirsep(prefix[--prefixlen]));
      input += 3;
    } else break;
  }
  if (!prefix) return input;
  if (prefixlen > length-2) prefixlen = length-2;
  if (prefix != output) memcpy(output, prefix, prefixlen);
  if (!prefixlen || !isdirsep(prefix[prefixlen-1])) output[prefixlen++] = '/';
  strlcpy(output+prefixlen, input, length-prefixlen);
  return output;
}

// Back-compatability with fltk1 functions:

#define FL_PATH_MAX 1024 // all buffers are assummed to be at least this long

/**
  Back-compatability version of filename_normalize().
  This did not let you specify the size of the buffer, it did a useless
  copy in the common case that there was no change in the name.
  It also did not do the home-directory prefix.
*/
bool fl_filename_absolute(char *output, const char *input, const char* pwd) {
  char temp[FL_PATH_MAX];
  if (input == output) {strlcpy(temp, input, FL_PATH_MAX); input = temp;}
  const char* t = filename_normalize(output, FL_PATH_MAX, input, pwd);
  if (t != output) {
    strlcpy(output, input, FL_PATH_MAX);
    return false;
  }
  return true;
}

/**
  Back-compatability function. Expands home directories (the old one
  would also expand $environment variables, but that is no longer
  supported).
*/
bool fl_filename_expand(char *output, const char *input) {
  if (*input == '~') return fl_filename_absolute(output, input, 0);
  if (output != input) strlcpy(output, input, FL_PATH_MAX);
  return false;
}

//
// End of "$Id$".
//
