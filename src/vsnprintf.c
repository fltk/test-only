/* "$Id: vsnprintf.c,v 1.15 2004/12/12 22:23:26 spitzak Exp $"
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

#include <config.h>
#include <fltk/string.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !HAVE_SNPRINTF || defined(DOXYGEN)

/*!
  printf a string and set of arguments into an output buffer. At
  most size-1 bytes will be written, and a NUL terminator is
  \e always added. The return value is the number of bytes that
  \e would be written if the string was correctly formatted, if
  this is greater than \a size then you need to reallocate a
  buffer of retval+1 size to get the full result.

  FLTK provides an implementation of this function on the (few)
  systems that don't provide it. Include the <fltk/string.h> header to
  call this portably. FLTK's emulation is rather poor and has the
  following bugs:
  - Field width & Precision is ignored for %%%, %%c, and %%s.
  - A malicious user who manages to create a %%-fmt string that prints
    more than 99 characters can still overflow the temporary buffer.
    For instance %%110f will overflow.
  - Only handles formats that are both documented in the glibc man page
    for printf and also handled by your system's sprintf().
  - Return value is not correct. If the buffer overflows a value
    greater or equal to \a size is returned, but it is only a guess
    about the actual length.

  Windows, Linux, and BSD all have this function so FLTK's emulation
  is not used. Warning: some systems do not return the correct value
  when the buffer overflows. A common alternative is to return -1 or
  \a size.
*/
int snprintf(char* str, size_t size, const char* fmt, ...) {
  int ret;
  va_list ap;
  va_start(ap, fmt);
  ret = vsnprintf(str, size, fmt, ap);
  va_end(ap);
  return ret;
}

#endif

#if !HAVE_VSNPRINTF || defined(DOXYGEN)

/*! See snprintf(). This version takes a va_list so it can be called
  from another function that has a variable argument list.

  FLTK provides an implementation of this function on the (few)
  systems that don't provide it. Include the <fltk/string.h> header to
  call this portably. See snprintf() for bugs with this emulation.
*/
int vsnprintf(char* str, size_t size, const char* fmt, va_list ap) {
  const char* e = str+size-1;
  char* p = str;
  char copy[20];
  char* copy_p;
  char sprintf_out[100];

  while (*fmt && p < e) {
    if (*fmt != '%') {
      *p++ = *fmt++;
    } else {
      fmt++;
      copy[0] = '%';
      for (copy_p = copy+1; copy_p < copy+19;) {
	switch ((*copy_p++ = *fmt++)) {
	case 0:
	  fmt--; goto CONTINUE;
	case '%':
	  *p++ = '%'; goto CONTINUE;
	case 'c':
	  *p++ = va_arg(ap, int);
	  goto CONTINUE;
	case 'd':
	case 'i':
	case 'o':
	case 'u':
	case 'x':
	case 'X':
	  *copy_p = 0;
	  sprintf(sprintf_out, copy, va_arg(ap, int));
	  copy_p = sprintf_out;
	  goto DUP;
	case 'e':
	case 'E':
	case 'f':
	case 'g':
	  *copy_p = 0;
	  sprintf(sprintf_out, copy, va_arg(ap, double));
	  copy_p = sprintf_out;
	  goto DUP;
	case 'p':
	  *copy_p = 0;
	  sprintf(sprintf_out, copy, va_arg(ap, void*));
	  copy_p = sprintf_out;
	  goto DUP;
	case 'n':
	  *(va_arg(ap, int*)) = p-str;
	  goto CONTINUE;
	case 's':
	  copy_p = va_arg(ap, char*);
	  if (!copy_p) copy_p = "NULL";
	DUP:
	  while (*copy_p && p < e) *p++ = *copy_p++;
	  goto CONTINUE;
	}
      }
    }
  CONTINUE:;
  }
  *p = 0;
  return p-str+strlen(fmt);
}

#endif

#ifdef __cplusplus
}
#endif

/*
 * End of "$Id: vsnprintf.c,v 1.15 2004/12/12 22:23:26 spitzak Exp $".
 */

