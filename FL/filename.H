#error header has not been ported to 3.0 yet
/*
 * "$Id$"
 *
 * Filename header file for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2010 by Bill Spitzak and others.
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

const char *fl_filename_name(const char * filename) {
  return fltk3::filename_name(filename);
}

FL_EXPORT const char *fl_filename_ext(const char *buf);
FL_EXPORT char *fl_filename_setext(char *to, int tolen, const char *ext);
FL_EXPORT int fl_filename_expand(char *to, int tolen, const char *from);
FL_EXPORT int fl_filename_absolute(char *to, int tolen, const char *from);
FL_EXPORT int fl_filename_relative(char *to, int tolen, const char *from);
FL_EXPORT int fl_filename_match(const char *name, const char *pattern);
FL_EXPORT int fl_filename_isdir(const char *name);

#  if defined(__cplusplus)

inline char *fl_filename_setext(char *to, const char *ext) { return fl_filename_setext(to, FL_PATH_MAX, ext); }
inline int fl_filename_expand(char *to, const char *from) { return fl_filename_expand(to, FL_PATH_MAX, from); }
inline int fl_filename_absolute(char *to, const char *from) { return fl_filename_absolute(to, FL_PATH_MAX, from); }
FL_EXPORT int fl_filename_relative(char *to, int tolen, const char *from, const char *cwd);
inline int fl_filename_relative(char *to, const char *from) { return fl_filename_relative(to, FL_PATH_MAX, from); }

#  endif


#  if defined(WIN32) && !defined(__CYGWIN__) && !defined(__WATCOMC__)

struct dirent {char d_name[1];};

#  elif defined(__WATCOMC__)
#    include <sys/types.h>
#    include <direct.h>

#  else
#    include <sys/types.h>
#    define fl_dirent_h_cyclic_include
#    include <dirent.h>
#    undef fl_dirent_h_cyclic_include
#  endif

#  if defined (__cplusplus)
extern "C" {
#  endif /* __cplusplus */

FL_EXPORT int fl_alphasort(struct dirent **, struct dirent **);
FL_EXPORT int fl_casealphasort(struct dirent **, struct dirent **);
FL_EXPORT int fl_casenumericsort(struct dirent **, struct dirent **);
FL_EXPORT int fl_numericsort(struct dirent **, struct dirent **);

  typedef int (Fl_File_Sort_F)(struct dirent **, struct dirent **); /**< File sorting function. \see fl_filename_list() */

#  if defined(__cplusplus)
}

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
