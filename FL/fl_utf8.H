//
// "$Id$"
//
// Unicode to UTF-8 conversion functions header.
//
//                   Copyright (c) 2003, O'ksi'D
//                      All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//      Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//      Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//      Neither the name of O'ksi'D nor the names of its contributors
//      may be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//

/*** NOTE : all functions are LIMITED to 24 bits Unicode values !!! ***/

#ifndef fl_utf8_h
#define fl_utf8_h
#include <stdio.h>

#ifndef FL_EXPORT
#if defined(FL_DLL) && defined(_MSC_VER)
#  ifdef FL_LIBRARY
#    define FL_EXPORT   __declspec(dllexport)
#  else
#    define FL_EXPORT   __declspec(dllimport)
#  endif /* FL_LIBRARY */
#else
#  define FL_EXPORT
#endif /* FL_DLL && _MSC_VER */
#endif /* FL_EXPORT */



#if __MACOS__
#  include <wchar.h>
#  include <stat.h>
#  define xchar wchar_t:
#else
#  include <sys/types.h>
#  include <sys/stat.h>
#  ifndef _WIN32 
#    ifndef NANO_X
#      ifndef DJGPP
#        if HAVE_XUTF8
#          include <libXutf8/Xutf8.h>
#        endif
#        include <X11/Xlocale.h>
#        include <X11/Xlib.h>
#        include <locale.h>
#      endif
#    endif
#  endif
#  if defined(WIN32)
#    include <ctype.h>
#    define xchar wchar_t
#  else
#    define xchar unsigned short
#  endif
#endif

#  ifdef __cplusplus
extern "C" {
#  endif

FL_EXPORT int fl_utf2ucs( const unsigned char *buf, int len, unsigned int *ucs);
FL_EXPORT int fl_ucs2utf(unsigned int ucs, char *buf);
FL_EXPORT int fl_utflen(const unsigned char *buf, int len);
FL_EXPORT int fl_utf_nb_char(const unsigned char *buf, int len);
FL_EXPORT int fl_utf_strncasecmp(const char *s1, const char *s2, int n);
FL_EXPORT int fl_utf_strcasecmp(const char *s1, const char *s2);
FL_EXPORT int fl_tolower(unsigned int ucs);
FL_EXPORT int fl_toupper(unsigned int ucs);
FL_EXPORT int fl_utf_tolower(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_utf_toupper(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_utf2unicode(const unsigned char *str, int len, xchar *buf);
FL_EXPORT int fl_unicode2utf(const xchar *str, int len, char *buf);
FL_EXPORT int fl_utf2latin1(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_latin12utf(const unsigned char *str, int len, char *buf);
FL_EXPORT unsigned int fl_nonspacing(unsigned int ucs);
FL_EXPORT char* fl_utf2mbcs(const char * s);
FL_EXPORT char* fl_mbcs2utf(const char *s);

FL_EXPORT int fl_is_nt4(void);
FL_EXPORT int fl_chmod(const char* f, int mode);
FL_EXPORT int fl_access(const char* f, int mode);
FL_EXPORT int fl_stat( const char *path, struct stat *buffer );
FL_EXPORT char* fl_getcwd( char *buf, int maxlen);
FL_EXPORT FILE *fl_fopen(const char *f, const char *mode);
FL_EXPORT int fl_system(const char* f);
FL_EXPORT int fl_execvp(const char *file, char *const *argv);
FL_EXPORT int fl_open(const char* f, int o, int p);
#define fl_open(a, b) fl_open(a, b, -1);
FL_EXPORT int fl_unlink(const char *f);
FL_EXPORT int fl_rmdir(const char *f);
FL_EXPORT char* fl_getenv(const char *name);
FL_EXPORT int fl_mkdir(const char* f, int mode);

#  ifdef __cplusplus
}
#  endif

#endif
 
//
// End of "$Id$".
//

