/* "$Id$"
 *
 * Copyright 1998-2005 by Bill Spitzak and others.
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

/* This file is designed to work on Windows, Linux, and BSD systems.
   It may need to be edited to work on other systems. Please try
   to do this by adding #if statements so this file remains portable.

   Some versions of fltk wrote this file using autoconf. I never liked
   this because I could not share the header file between systems, so
   I have reverted to a constant version.
*/

#ifndef fltk_string_h
#define fltk_string_h

#if 1
# include <string.h>
#endif

#if 1 /* for va_list */
# include <stdarg.h>
#endif

#if 1 /* for sprintf, vsprintf, snprintf and vsnprintf */
# include <stdio.h>
#endif

#if 0
# include <stdlib.h>
#endif

#if 0
# include <ctype.h>
/* Unixware defines these macros in above header for the obsolete BSD
   functions, get rid of them as it prevents you making a variable
   named "index"! */
# ifdef index
#  undef index
# endif
# ifdef rindex
#  undef rindex
# endif
#endif

#if 0
# include <strings.h>
#endif

#if 0 /*defined(__MWERKS__)*/
/* MetroWerks' CodeWarrior put some functions in <extras.h> but that
   file does not play well with others, so we don't include it. */
# include <extras.h>
#endif

/* Windows has equivalent functions, but being Microsoft they added
   gratuitoius changes to the names to stop code from being portable: */
#if (defined(_WIN32) && !defined(__CYGWIN__)) || defined(__EMX__)
# define strcasecmp(s,t)	stricmp(s, t)
# define strncasecmp(s,t,n)	strnicmp(s, t, n)
# define vsnprintf		_vsnprintf
# define snprintf		_snprintf
#endif

/*! \addtogroup utilities
  \{ */

#ifdef __cplusplus
extern "C" {
#endif

#include "FL_API.h"

FL_API extern char* newstring(const char *);

#if defined(DOXYGEN) || defined(__MWERKS__)
FL_API extern int strcasecmp(const char *, const char *);
#endif

#if defined(DOXYGEN) || defined(__MWERKS__)
FL_API extern int strncasecmp(const char *, const char *, size_t);
#endif

#if defined(DOXYGEN)
FL_API extern int snprintf(char *, size_t, const char *, ...);
#endif

#if defined(DOXYGEN)
FL_API extern int vsnprintf(char *, size_t, const char *, va_list ap);
#endif

#if defined(DOXYGEN) || !defined(__BSD__)
FL_API extern size_t strlcat(char *, const char *, size_t);
#endif

#if defined(DOXYGEN) || !defined(__BSD__)
FL_API extern size_t strlcpy(char *, const char *, size_t);
#endif

#ifdef __cplusplus
}
#endif

/*! \} */

#endif

/*
 * End of "$Id$".
 */
