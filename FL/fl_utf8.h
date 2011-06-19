/* "$Id$"
 *
 * Author: Jean-Marc Lienher ( http://oksid.ch )
 * Copyright 2000-2010 by O'ksi'D.
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
 * Please report all bugs and problems on the following page:
 *
 *     http://www.fltk.org/str.php
 */

/* Merged in some functionality from the fltk-2 version. IMM.
 * The following code is an attempt to merge the functions incorporated in FLTK2
 * with the functions provided in OksiD's fltk-1.1.6-utf8 port
 */

/**
  \file fl_utf8.h
  \brief header for Unicode and UTF8 chracter handling
*/

#ifndef _HAVE_FL_UTF8_HDR_
#define _HAVE_FL_UTF8_HDR_

#include <fltk3/utf8.h>
#include "Fl_Export.H"
#include "fl_types.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#if 0 // FIXME: 123

inline int fl_utf8bytes(unsigned ucs);
inline int fl_utf8len(char c);
inline int fl_utf8len1(char c);
inline int fl_utf_nb_char(const unsigned char *buf, int len);
inline unsigned fl_utf8decode(const char* p, const char* end, int* len);
inline int fl_utf8encode(unsigned ucs, char* buf);
inline const char* fl_utf8fwd(const char* p, const char* start, const char* end);
inline const char* fl_utf8back(const char* p, const char* start, const char* end);
inline unsigned fl_ucs_to_Utf16(const unsigned ucs, unsigned short *dst, const unsigned dstlen);
inline unsigned fl_utf8toUtf16(const char* src, unsigned srclen, unsigned short* dst, unsigned dstlen);
inline unsigned fl_utf8towc(const char *src, unsigned srclen, wchar_t *dst, unsigned dstlen);
inline unsigned fl_utf8fromwc(char *dst, unsigned dstlen, const wchar_t *src, unsigned srclen);
inline unsigned fl_utf8toa (const char *src, unsigned srclen, char *dst, unsigned dstlen);
inline unsigned fl_utf8froma (char *dst, unsigned dstlen, const char *src, unsigned srclen);
inline int fl_utf8locale();
inline int fl_utf8test(const char *src, unsigned len);
inline int fl_wcwidth_(unsigned int ucs);
inline int fl_wcwidth(const char *src);
inline unsigned int fl_nonspacing(unsigned int ucs);
inline unsigned fl_utf8to_mb(const char *src, unsigned srclen, char *dst, unsigned dstlen);
inline char* fl_utf2mbcs(const char *src);
inline unsigned fl_utf8from_mb(char *dst, unsigned dstlen, const char *src, unsigned srclen);
inline int fl_utf_strncasecmp(const char *s1, const char *s2, int n);
inline int fl_utf_strcasecmp(const char *s1, const char *s2);
inline int fl_tolower(unsigned int ucs);
inline int fl_toupper(unsigned int ucs);
inline int fl_utf_tolower(const unsigned char *str, int len, char *buf);
inline int fl_utf_toupper(const unsigned char *str, int len, char *buf);
inline int fl_chmod(const char* f, int mode);
inline int fl_access(const char* f, int mode);
inline int fl_stat( const char *path, struct stat *buffer );
inline char* fl_getcwd( char *buf, int maxlen);

#endif

inline FILE *fl_fopen(const char *f, const char *mode) {
  return fltk3::fopen(f, mode);
}

#if 0 // FIXME: 123

inline int fl_system(const char* f);
inline int fl_execvp(const char *file, char *const *argv);

#endif

inline int fl_open(const char* f, int o, ...) {
  va_list ap;
  va_start(ap, o);
  int ret = fltk3::open(ap, f, o);
  va_end(ap);
  return ret;
}

#if 0 // FIXME: 123

inline int fl_unlink(const char *f);
inline int fl_rmdir(const char *f);
inline char* fl_getenv(const char *name);
inline int fl_mkdir(const char* f, int mode);
inline int fl_rename(const char* f, const char *t);
inline void fl_make_path_for_file( const char *path );
inline char fl_make_path( const char *path );

#ifdef WIN32
inline char *fl_utf8_to_locale(const char *s, int len, unsigned int codepage);
inline char *fl_locale_to_utf8(const char *s, int len, unsigned int codepage);
#endif

#endif

#endif /* _HAVE_FL_UTF8_HDR_ */

/*
 * End of "$Id$".
 */
