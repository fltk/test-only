// "$Id: filename_ext.cxx 4115 2005-03-14 04:48:51Z spitzak $"

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

#include <fltk/filename.h>

/*!
  Returns a pointer to the last period in filename_name(f), or a
  pointer to the trailing nul if none. Notice that this points \e at
  the period, not after it!
*/
// returns pointer to the filename, or null if name ends with '/'
#if defined(_WIN32) || defined(__EMX__)
const char *fltk::filename_name(const char *name) {
  const char *p,*q;
  if (!name) return (0);
  q = name;
  if (q[0] && q[1]==':') q += 2; // skip leading drive letter
  for (p = q; *p; p++) if (*p == '/' || *p == '\\') q = p+1;
  return q;
}
#else
// returns pointer to the filename, or null if name ends with '/'
const char *fltk::filename_name(const char *name) {
  const char *p,*q;
  if (!name) return (0);
  for (p=q=name; *p;) if (*p++ == '/') q = p;
  return q;
}
#endif

//
// End of "$Id: filename_ext.cxx 4115 2005-03-14 04:48:51Z spitzak $".
//
