/*
 * "$Id: utf.h,v 1.2 2004/06/22 08:28:54 spitzak Exp $"
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

/*! \addtogroup utilities
  \{ */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

FL_API extern unsigned utf8decode(const char*, const char* end, int* len);
FL_API extern int utf8encode(unsigned, char*);
FL_API extern int utf8valid(const char*, const char* end);
FL_API extern const char* utf8fwd(const char*, const char* start, const char* end);
FL_API extern const char* utf8back(const char*, const char* start, const char* end);
FL_API extern int utf8len(char);

FL_API extern unsigned*		utf8to32(const char*, int, int*);
FL_API extern unsigned short*	utf8to16(const char*, int, int*);
FL_API extern char*		utf8to8(const char*, int, int*);
FL_API extern char*		utf8from32(const unsigned*, int, int*);
FL_API extern char*		utf8from16(const unsigned short*, int, int*);
FL_API extern char*		utf8from8(const char*, int, int*);
FL_API extern void		utf8free(void*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*! \} */

#endif
