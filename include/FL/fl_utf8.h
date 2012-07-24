/* 
 * "$Id$"
 *
 * Author: Jean-Marc Lienher ( http://oksid.ch )
 * Copyright 2000-2012 by O'ksi'D.
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

#ifndef _HAVE_FL_UTF8_HDR_
#define _HAVE_FL_UTF8_HDR_

#include <fltk3/utf8.h>
#include "Fl_Export.H"
#include "fl_types.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


inline int fl_utf8bytes(unsigned ucs) {
  return fltk3::utf8bytes(ucs);
}

inline int fl_utf8len(char c) {
  return fltk3::utf8len(c);
}

inline int fl_utf8len1(char c) {
  return fltk3::utf8len1(c);
}

inline int fl_utf_nb_char(const unsigned char *buf, int len) {
  return fltk3::utf_nb_char(buf, len);
}

inline unsigned fl_utf8decode(const char* p, const char* end, int* len) {
  return fltk3::utf8decode(p, end, len);
}

inline int fl_utf8encode(unsigned ucs, char* buf) {
  return fltk3::utf8encode(ucs, buf);
}

inline const char* fl_utf8fwd(const char* p, const char* start, const char* end) {
  return fltk3::utf8fwd(p, start, end);
}

inline const char* fl_utf8back(const char* p, const char* start, const char* end) {
  return fltk3::utf8back(p, start, end);
}

inline unsigned fl_ucs_to_Utf16(const unsigned ucs, unsigned short *dst, const unsigned dstlen) {
  return fltk3::ucs_to_Utf16(ucs, dst, dstlen);
}

inline unsigned fl_utf8toUtf16(const char* src, unsigned srclen, unsigned short* dst, unsigned dstlen) {
  return fltk3::utf8toUtf16(src, srclen, dst, dstlen);
}

inline unsigned fl_utf8towc(const char *src, unsigned srclen, wchar_t *dst, unsigned dstlen) {
  return fltk3::utf8towc(src, srclen, dst, dstlen);
}

inline unsigned fl_utf8fromwc(char *dst, unsigned dstlen, const wchar_t *src, unsigned srclen) {
  return fltk3::utf8fromwc(dst, dstlen, src, srclen);
}

inline unsigned fl_utf8toa (const char *src, unsigned srclen, char *dst, unsigned dstlen) {
  return fltk3::utf8toa(src, srclen, dst, dstlen);
}

inline unsigned fl_utf8froma (char *dst, unsigned dstlen, const char *src, unsigned srclen) {
  return fltk3::utf8froma(dst, dstlen, src, srclen);
}

inline int fl_utf8locale() {
  return fltk3::utf8locale();
}

inline int fl_utf8test(const char *src, unsigned len) {
  return fltk3::utf8test(src, len);
}

inline int fl_wcwidth_(unsigned int ucs) {
  return fltk3::wcwidth_(ucs);
}

inline int fl_wcwidth(const char *src) {
  return fltk3::wcwidth(src);
}

inline unsigned int fl_nonspacing(unsigned int ucs) {
  return fltk3::nonspacing(ucs);
}

inline unsigned fl_utf8to_mb(const char *src, unsigned srclen, char *dst, unsigned dstlen) {
  return fltk3::utf8to_mb(src, srclen, dst, dstlen);
}

inline char* fl_utf2mbcs(const char *src) {
  return fltk3::utf2mbcs(src);
}

inline unsigned fl_utf8from_mb(char *dst, unsigned dstlen, const char *src, unsigned srclen) {
  return fltk3::utf8from_mb(dst, dstlen, src, srclen);
}

inline int fl_utf_strncasecmp(const char *s1, const char *s2, int n) {
  return fltk3::strncasecmp(s1, s2, n);
}

inline int fl_utf_strcasecmp(const char *s1, const char *s2) {
  return fltk3::strcasecmp(s1, s2);
}

inline int fl_tolower(unsigned int ucs) {
  return fltk3::tolower(ucs);
}

inline int fl_toupper(unsigned int ucs) {
  return fltk3::toupper(ucs);
}

inline int fl_utf_tolower(const unsigned char *str, int len, char *buf) {
  return fltk3::tolower(str, len, buf);
}

inline int fl_utf_toupper(const unsigned char *str, int len, char *buf) {
  return fltk3::toupper(str, len, buf);
}

inline int fl_chmod(const char* f, int mode) {
  return fltk3::chmod(f, mode);
}

inline int fl_access(const char* f, int mode) {
  return fltk3::access(f, mode);
}

inline int fl_stat( const char *path, struct stat *buffer ) {
  return fltk3::stat(path, buffer);
}

inline char* fl_getcwd( char *buf, int maxlen) {
  return fltk3::getcwd(buf, maxlen);
}

inline FILE *fl_fopen(const char *f, const char *mode) {
  return fltk3::fopen(f, mode);
}

inline int fl_system(const char* f) {
  return fltk3::system(f);
}

inline int fl_execvp(const char *file, char *const *argv) {
  return fltk3::execvp(file, argv);
}

inline int fl_open(const char* f, int o, ...) {
  va_list ap;
  va_start(ap, o);
  int ret = fltk3::vopen(f, o, ap);
  va_end(ap);
  return ret;
}

inline int fl_unlink(const char *f) {
  return fltk3::unlink(f);
}

inline int fl_rmdir(const char *f) {
  return fltk3::rmdir(f);
}

inline char* fl_getenv(const char *name) {
  return fltk3::getenv(name);
}

inline int fl_mkdir(const char* f, int mode) {
  return fltk3::mkdir(f, mode);
}

inline int fl_rename(const char* f, const char *t) {
  return fltk3::rename(f, t);
}

inline void fl_make_path_for_file( const char *path ) {
  fltk3::make_path_for_file(path);
}

inline char fl_make_path( const char *path ) {
  return fltk3::make_path(path);
}

#ifdef WIN32

inline char *fl_utf8_to_locale(const char *s, int len, unsigned int codepage) {
  return fltk3::utf8_to_locale(s, len, codepage);
}

inline char *fl_locale_to_utf8(const char *s, int len, unsigned int codepage) {
  return fltk3::locale_to_utf8(s, len, codepage);
}

#endif

#endif /* _HAVE_FL_UTF8_HDR_ */

/*
 * End of "$Id$".
 */
