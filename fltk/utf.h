/*
 * "$Id: utf.h,v 1.7 2004/07/29 09:07:52 spitzak Exp $"
 *
 * UTF-8 functions used by fltk
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

#ifndef fltk_utf_h
#define fltk_utf_h

#include "FL_API.h"
#include <stdlib.h>

/*! \addtogroup utilities
  \{ */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

FL_API int	utf8bytes(unsigned ucs);
FL_API int	utf8len(char firstbyte); /* depreciated */

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

/*! \} */

#endif
