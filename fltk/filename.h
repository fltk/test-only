// "$Id$"

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
 *
 * These functions are not in the fltk namespace because they really
 * should not be part of fltk. They are used by the file chooser.
 */

#ifndef fltk_filename_h
#define fltk_filename_h

#include "FL_API.h"

/*! \addtogroup utilities
  \{ */

FL_API const char* filename_normalize(char* output, int length, const char* input, const char* directory=0);
inline char* filename_normalize(char* o, int l, char* i, const char* d=0) {return (char*)(filename_normalize(o,l,(const char*)i,d));}
FL_API const char *filename_name(const char *);
inline char* filename_name(char* a) {return (char*)(filename_name((const char*)a));}
FL_API const char *filename_ext(const char *);
inline char* filename_ext(char* a) {return (char*)(filename_ext((const char*)a));}
FL_API bool filename_match(const char *, const char *pattern); // glob match
FL_API bool filename_isdir(const char*);
FL_API double filename_size(const char *); // return size of file
FL_API long int filename_mtime(const char *); // return modification time

/*! \} */

////////////////////////////////////////////////////////////////
// dirent (what a pain)...

#if defined(__WATCOMC__)

# include <sys/types.h>
# include "direct.h"

#elif defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
// Dummy version used on win32 that just holds a name:

struct dirent {char d_name[1];};

#elif defined(__linux)
// Newest Linux libc is broken when it emulates the 32-bit dirent, it
// generates errors when the data like the inode number does not fit, even
// though we are not going to look at anything other than the name. This
// code seems to force the 64-bit version to be used:

# ifndef _GNU_SOURCE
#  define _GNU_SOURCE
# endif
# include <features.h>
# include <sys/types.h>
# include <dirent.h>
# define dirent dirent64
# define scandir scandir64

#else
// warning: on some systems (very few nowadays?) <dirent.h> may not exist.
// The correct information is in one of these three files:
//  #include <sys/ndir.h>
//  #include <sys/dir.h>
//  #include <ndir.h>
// plus you must do the following #define:
//  #define dirent direct
// I recommend you create a /usr/include/dirent.h containing the correct info

# include <sys/types.h>
# include <dirent.h>

#endif

/*! \addtogroup utilities */
FL_API int filename_list(const char *d, struct dirent ***);

#ifndef PATH_MAX
# ifdef _MAX_PATH
#  define PATH_MAX _MAX_PATH
# else
#  define PATH_MAX 1024
# endif
#endif

#endif

// End of "$Id$".
