/* "$Id: utf.h 5197 2006-06-14 07:43:46Z spitzak $"
 *
 * Copyright 1998-2006 by Bill Spitzak and others.
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
/*! \file
  Functions to manipulate UTF-8 strings and convert from/to legacy
  encodings. These functions are \e not in the fltk namespace.
*/

#ifndef fltk_utf_h
#define fltk_utf_h

#include "FL_API.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

FL_API int	utf8bytes(unsigned ucs);

FL_API unsigned	utf8decode(const char*, const char* end, int* len);
FL_API int	utf8encode(unsigned, char*);
FL_API const char* utf8fwd(const char*, const char* start, const char* end);
FL_API const char* utf8back(const char*, const char* start, const char* end);

FL_API unsigned	utf8towc(const char*, unsigned, wchar_t*, unsigned);
FL_API unsigned utf8tomb(const char*, unsigned, char*, unsigned);
FL_API unsigned utf8toa (const char*, unsigned, char*, unsigned);
FL_API unsigned utf8fromwc(char*, unsigned, const wchar_t*, unsigned);
FL_API unsigned utf8frommb(char*, unsigned, const char*, unsigned);
FL_API unsigned utf8froma (char*, unsigned, const char*, unsigned);
FL_API int utf8locale();
FL_API int utf8test(const char*, unsigned);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
