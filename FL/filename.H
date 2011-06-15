/*
 * "$Id$"
 *
 * Filename header file for the Fast Light Tool Kit (FLTK).
 * FLTK 123 wrapper started
 *
 * Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef FL_FILENAME_H
#define FL_FILENAME_H

// FIXME: 123 - this is bad because now we can only include this file form a c++ file!
#include <fltk3/filename.h>
#include "Fl_Export.H"

#define FL_PATH_MAX 2048

inline const char *fl_filename_name(const char * filename) {
  return fltk3::filename_name(filename);
}

inline const char *fl_filename_ext(const char *buf) {
  return fltk3::filename_ext(buf);
}

inline char *fl_filename_setext(char *to, int tolen, const char *ext) {
  return fltk3::filename_setext(to, tolen, ext);
}

inline int fl_filename_expand(char *to, int tolen, const char *from) {
  return fltk3::filename_expand(to, tolen, from);
}

inline int fl_filename_absolute(char *to, int tolen, const char *from) {
  return fltk3::filename_absolute(to, tolen, from);
}

inline int fl_filename_relative(char *to, int tolen, const char *from) {
  return fltk3::filename_relative(to, tolen, from);
}

inline int fl_filename_match(const char *name, const char *pattern) {
  return fltk3::filename_match(name, pattern);
}

inline int fl_filename_isdir(const char *name) {
  return fltk3::filename_isdir(name);
}

inline char *fl_filename_setext(char *to, const char *ext) { 
  return fltk3::filename_setext(to, FL_PATH_MAX, ext); 
}

inline int fl_filename_expand(char *to, const char *from) { 
  return fltk3::filename_expand(to, FL_PATH_MAX, from); 
}

inline int fl_filename_absolute(char *to, const char *from) { 
  return fltk3::filename_absolute(to, FL_PATH_MAX, from); 
}

inline int fl_filename_relative(char *to, int tolen, const char *from, const char *cwd) {
  return fltk3::filename_relative(to, tolen, from, cwd);
}

inline int fl_filename_relative(char *to, const char *from) { 
  return fltk3::filename_relative(to, FL_PATH_MAX, from); 
}

inline int fl_alphasort(struct dirent **a, struct dirent **b) {
  return fltk3::alphasort(a, b);
}

inline int fl_casealphasort(struct dirent **a, struct dirent **b) {
  return fltk3::alphasort(a, b);
}

inline int fl_casenumericsort(struct dirent **a, struct dirent **b) {
  return fltk3::alphasort(a, b);
}

inline int fl_numericsort(struct dirent **a, struct dirent **b) {
  return fltk3::alphasort(a, b);
}

typedef fltk3::FileSortF Fl_File_Sort_F;

FL_EXPORT int fl_filename_list(const char *d, struct dirent ***l,
                               Fl_File_Sort_F *s = fl_numericsort);
FL_EXPORT void fl_filename_free_list(struct dirent ***l, int n);

FL_EXPORT int	fl_open_uri(const char *uri, char *msg = (char *)0,
		            int msglen = 0);

int _fl_filename_isdir_quick(const char *name);

#  endif /* __cplusplus */


#endif /* FL_FILENAME_H */

/*
 * End of "$Id$".
 */
