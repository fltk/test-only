/*
 * "$Id: vsnprintf.h,v 1.5 2002/12/10 01:46:15 easysw Exp $"
 *
 * vsnprintf() function for the Fast Light Tool Kit (FLTK).
 *
 * Include this file instead of stdio.h to get the snprintf and vsnprintf
 * functions on systems that lack it (pretty much everything except glibc
 * systems). PLEASE FIX FOR SYSTEMS THAT HAVE THESE FUNCTIONS!
 *
 * KNOWN BUGS:
 *
 * Field width & Precision is ignored for %%, %c, and %s.
 *
 * A malicious user who manages to create a %-fmt string that prints
 * more than 99 characters can still overflow the temporary buffer.
 * For instance %110f will overflow.
 *
 * Only handles formats that are both documented in the glibc man page
 * for printf and also handled by your system's sprintf().
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
 * Please report all bugs and problems to "fltk-bugs@easysw.com".
 */

#ifndef fl_vsnprintf_h
#define fl_vsnprintf_h

#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32

# define vsnprintf _vsnprintf
# define snprintf _snprintf

#elif defined(__linux)
/* linux has vsnprintf */

#elif _COMPILER_VERSION>=730
/* newer SGI compilers have vsnprintf */

#else
/* all other machines need our emulation function: */

# define need_fl_vsnprintf 1 /* make the function compile */
# ifdef __cplusplus
   extern "C" {
# endif
# include <sys/types.h>
# define vsnprintf fl_vsnprintf
  int fl_vsnprintf(char* str, size_t size, const char* fmt, va_list ap);
# define snprintf fl_snprintf
  int fl_snprintf(char* str, size_t size, const char* fmt, ...);
# ifdef __cplusplus
   }
# endif

#endif

#endif
/*
 * End of "$Id: vsnprintf.h,v 1.5 2002/12/10 01:46:15 easysw Exp $".
 */

