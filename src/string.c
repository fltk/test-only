/*
 * "$Id: string.c,v 1.4 2003/04/20 03:17:51 easysw Exp $"
 *
 * BSD string functions for the Fast Light Tool Kit (FLTK).
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

#include <stdio.h>
#include <stdlib.h>
#include <fltk/string.h>


/*
 * 'fltk_strdup()' - Duplicate a string.
 */

#ifndef HAVE_STRDUP
char 	*			/* O - New string pointer */
fltk_strdup(const char *s)	/* I - String to duplicate */
{
  char	*t;			/* New string pointer */
  int n;			/* size to allocate */
  if (!s) return 0;
  n = strlen(s)+1;
  t = malloc(n);
  if (t) memcpy(t, s, n);
  return t;
}
#endif /* !HAVE_STRDUP */


/*
 * 'fltk_strcasecmp()' - Do a case-insensitive comparison.
 */

#ifndef HAVE_STRCASECMP
int				/* O - Result of comparison (<0, 0, or >0) */
fltk_strcasecmp(const char *s,	/* I - First string */
                const char *t)	/* I - Second string */
{
  for (;;)
  {
    int x = tolower(*s) - tolower(*t);
    if (x) return x;
    if (!*s) return 0;
    s ++;
    t ++;
  }
}
#endif /* !HAVE_STRCASECMP */

/*
 * 'fltk_strncasecmp()' - Do a case-insensitive comparison on up to N chars.
 */

#ifndef HAVE_STRNCASECMP
int				/* O - Result of comparison (<0, 0, or >0) */
fltk_strncasecmp(const char *s,	/* I - First string */
                 const char *t,	/* I - Second string */
		 size_t     n)	/* I - Maximum number of characters to compare */
{
  while (n--)
  {
    int x = tolower(*s) - tolower(*t);
    if (x) return x;
    if (!*s) return 0;
    s ++;
    t ++;
  }
  return 0;
}
#endif /* !HAVE_STRNCASECMP */


#if !HAVE_STRLCAT
/*!
  Appends src to string dst of size siz (unlike strncat, siz is the
  full size of dst, not space left).  At most siz-1 characters
  will be copied.  Always NUL terminates (unless siz == 0).
  Returns strlen(initial dst) + strlen(src); if retval >= siz,
  truncation occurred.
*/
size_t				/* O - strlen(dst)+strlen(src) */
fltk_strlcat(char *dst,		/* I - Destination buffer */
	     const char *src,	/* I - String to append to dst */
	     size_t siz)	/* I - sizeof(dst) */
{
  char *d = dst;
  const char *s = src;
  size_t n = siz;
  size_t dlen;

  /* Find the end of dst and adjust bytes left but don't go past end */
  while (*d != '\0' && n-- != 0)
    d++;
  dlen = d - dst;
  n = siz - dlen;

  if (n == 0)
    return(dlen + strlen(s));
  while (*s != '\0') {
    if (n != 1) {
      *d++ = *s;
      n--;
    }
    s++;
  }
  *d = '\0';

  return(dlen + (s - src));	/* count does not include NUL */
}
#endif /* !HAVE_STRLCAT */

#if !HAVE_STRLCPY
/*!
  Copy src to string dst of size siz.  At most siz-1 characters
  will be copied.  Always NUL terminates (unless siz == 0).
  Returns strlen(src); if retval >= siz, truncation occurred.
*/
size_t				/* O - strlen(src) */
fltk_strlcpy(char *dst,		/* I - Destination buffer */
	     const char *src,	/* I - String to put in dst */
	     size_t siz)	/* I - sizeof(dst) */
{
  char *d = dst;
  const char *s = src;
  size_t n = siz;

  /* Copy as many bytes as will fit */
  if (n != 0 && --n != 0) {
    do {
      if ((*d++ = *s++) == 0)
	break;
    } while (--n != 0);
  }

  /* Not enough room in dst, add NUL and traverse rest of src */
  if (n == 0) {
    if (siz != 0)
      *d = '\0';		/* NUL-terminate dst */
    while (*s++)
      ;
  }

  return(s - src - 1);	/* count does not include NUL */
}
#endif /* !HAVE_STRLCPY */


/*
 * End of "$Id: string.c,v 1.4 2003/04/20 03:17:51 easysw Exp $".
 */
