// "$Id$"

/* Copyright 1998-2005 by Bill Spitzak and others.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Please report all bugs and problems to "fltk-bugs@fltk.org".
 */

/** \addtogroup utilities

  FLTK provides some functions that it uses internally that are
  necessary for portablity, or convienent for writing code.  These are
  \e not in the fltk:: namespace and do not have "fl_" in their names,
  because in theory they should not be part of fltk, but instead
  provided by the system.

  Note to money-grubbing capitalists: the source code for the
  utility functions is PUBLIC DOMAIN and may be reused for any purpose
  whatsoever (such as, I hope, putting them in your system's libraries!)

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
    the result to.
  - \a length is the size of \a output. No more than \a length-1 characters
    are written, plus a trailing nul.
  - \a input is the initial filename.
  - \a directory is the directory that filename is relative to. If
    this is NULL then the current directory is gotten from the OS.

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
fltk::filename_normalize(char* output, int length, const char* input, const char* pwd)
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

/**
  Back-compatability version of filename_normalize().
  This did not let you specify the size of the buffer, it did a useless
  copy in the common case that there was no change in the name.
  It also did not do the home-directory prefix.
*/
bool fltk::filename_absolute(char *output, const char *input, const char* pwd) {
  char temp[PATH_MAX];
  if (input == output) {strlcpy(temp, input, PATH_MAX); input = temp;}
  const char* t = fltk::filename_normalize(output, PATH_MAX, input, pwd);
  if (t != output) {
    strlcpy(output, input, PATH_MAX);
    return false;
  }
  return true;
}

/**
  Back-compatability function. Expands home directories (the old one
  would also expand $environment variables, but that is no longer
  supported).
*/
bool fltk::filename_expand(char *output, const char *input) {
    if (*input == '~') return fltk::filename_absolute(output, input, 0);
  if (output != input) strlcpy(output, input, PATH_MAX);
  return false;
}

//
// End of "$Id$".
//
