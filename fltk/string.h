/* "$Id: string.h,v 1.3 2005/01/24 17:25:04 spitzak Exp $"
 *
 * Copyright 1998-2004 by Bill Spitzak and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
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

#if defined(DOXYGEN) || defined(__MWERKS__)
FL_API extern int strcasecmp(const char *, const char *);
#endif

#if defined(DOXYGEN) || defined(__MWERKS__)
FL_API extern int strncasecmp(const char *, const char *, size_t);
#endif

#if defined(DOXYGEN) || defined(__MWERKS__)
FL_API extern char *strdup(const char *);
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
 * End of "$Id: string.h,v 1.3 2005/01/24 17:25:04 spitzak Exp $".
 */
