/*
 * "$Id: string.h,v 1.1 2003/03/26 22:05:54 easysw Exp $"
 *
 * Common string header file for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2003 by Bill Spitzak and others.
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

#ifndef fltk_string_h
#  define fltk_string_h

#  include <fltk/FL_API.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>

/*
 * Include the standard string function header file...
 */

#  include <string.h>
#  include <ctype.h>


/*
 * Windows defines functions called "stricmp" and "strnicmp" which
 * work just like "strcasecmp" and "strncasecmp" under UNIX.
 *
 * Also, the DLL C runtime libraries contain snprintf and vsnprintf
 * functions, but not all linkers add the leading underscore when
 * linking, thus the definition.
 */

#  define strcasecmp(s,t)	stricmp((s), (t))
#  define strncasecmp(s,t,n)	strnicmp((s), (t), (n))
#  define vsnprintf		_vsnprintf
#  define snprintf		_snprintf

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */

/*
 * Define the strlcat and strlcpy functions that work the
 * way strncpy and strncat *should* have worked in the
 * first place.
 */

FL_API extern size_t fltk_strlcat(char *, const char *, size_t);
#  define strlcat fltk_strlcat

FL_API extern size_t fltk_strlcpy(char *, const char *, size_t);
#  define strlcpy fltk_strlcpy

#  ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !fltk_string_h */


/*
 * End of "$Id: string.h,v 1.1 2003/03/26 22:05:54 easysw Exp $".
 */
