/*
 * "$Id: vsnprintf.h,v 1.3 2000/06/12 09:01:35 carl Exp $"
 *
 * vsnprintf() function for the Fast Light Tool Kit (FLTK).
 *
 * Emulates this call on systems that lack it (pretty much everything
 * except glibc systems).
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
 * Copyright 1998-1999 by Bill Spitzak and others.
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <FL/Fl_Export.H>

FL_API int fl_vsnprintf(char* str, size_t size, const char* fmt, va_list ap);
FL_API int fl_snprintf(char* str, size_t size, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

/*
 * End of "$Id: vsnprintf.h,v 1.3 2000/06/12 09:01:35 carl Exp $".
 */

