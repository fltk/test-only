/
// "$Id$
/
// Unicode to UTF-8 conversion functions header
/
//                   Copyright (c) 2003, O'ksi'
//                      All rights reserved
/
// Redistribution and use in source and binary forms, with or withou
// modification, are permitted provided that the following condition
// are met
/
//      Redistributions of source code must retain the above copyrigh
//      notice, this list of conditions and the following disclaimer
/
//      Redistributions in binary form must reproduce the above copyrigh
//      notice, this list of conditions and the following disclaimer in th
//      documentation and/or other materials provided with the distribution
/
//      Neither the name of O'ksi'D nor the names of its contributor
//      may be used to endorse or promote products derived from this softwar
//      without specific prior written permission
/
/
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTOR
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NO
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FO
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNE
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, O
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY O
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDIN
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THI
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
/
/
/

/*** NOTE : all functions are LIMITED to 24 bits Unicode values !!! ***

#ifndef fl_utf8_
#define fl_utf8_
#include <stdio.h

#ifndef FL_EXPOR
#if defined(FL_DLL) && defined(_MSC_VER
#  ifdef FL_LIBRAR
#    define FL_EXPORT   __declspec(dllexport
#  els
#    define FL_EXPORT   __declspec(dllimport
#  endif /* FL_LIBRARY *
#els
#  define FL_EXPOR
#endif /* FL_DLL && _MSC_VER *
#endif /* FL_EXPORT *



#if __MACOS_
#  include <wchar.h
#  include <stat.h
#  define xchar wchar_t
#els
#  include <sys/types.h
#  include <sys/stat.h
#  ifndef _WIN32
#    ifndef NANO_
#      ifndef DJGP
#        if HAVE_XUTF
#          include <libXutf8/Xutf8.h
#        endi
#        include <X11/Xlocale.h
#        include <X11/Xlib.h
#        include <locale.h
#      endi
#    endi
#  endi
#  if defined(WIN32
#    include <ctype.h
#    define xchar wchar_
#  els
#    define xchar unsigned shor
#  endi
#endi

#  ifdef __cplusplu
extern "C" 
#  endi

FL_EXPORT int fl_utf2ucs( const unsigned char *buf, int len, unsigned int *ucs)
FL_EXPORT int fl_ucs2utf(unsigned int ucs, char *buf)
FL_EXPORT int fl_utflen(const unsigned char *buf, int len)
FL_EXPORT int fl_utf_nb_char(const unsigned char *buf, int len)
FL_EXPORT int fl_utf_strncasecmp(const char *s1, const char *s2, int n)
FL_EXPORT int fl_utf_strcasecmp(const char *s1, const char *s2)
FL_EXPORT int fl_tolower(unsigned int ucs)
FL_EXPORT int fl_toupper(unsigned int ucs)
FL_EXPORT int fl_utf_tolower(const unsigned char *str, int len, char *buf)
FL_EXPORT int fl_utf_toupper(const unsigned char *str, int len, char *buf)
FL_EXPORT int fl_utf2unicode(const unsigned char *str, int len, xchar *buf)
FL_EXPORT int fl_unicode2utf(const xchar *str, int len, char *buf)
FL_EXPORT int fl_utf2latin1(const unsigned char *str, int len, char *buf)
FL_EXPORT int fl_latin12utf(const unsigned char *str, int len, char *buf)
FL_EXPORT unsigned int fl_nonspacing(unsigned int ucs)
FL_EXPORT char* fl_utf2mbcs(const char * s)
FL_EXPORT char* fl_mbcs2utf(const char *s)

FL_EXPORT int fl_is_nt4(void)
FL_EXPORT int fl_chmod(const char* f, int mode)
FL_EXPORT int fl_access(const char* f, int mode)
FL_EXPORT int fl_stat( const char *path, struct stat *buffer )
FL_EXPORT char* fl_getcwd( char *buf, int maxlen)
FL_EXPORT FILE *fl_fopen(const char *f, const char *mode)
FL_EXPORT int fl_system(const char* f)
FL_EXPORT int fl_execvp(const char *file, char *const *argv)
FL_EXPORT int fl_open(const char* f, int o, int p)
#define fl_open(a, b) fl_open(a, b, -1)
FL_EXPORT int fl_unlink(const char *f)
FL_EXPORT int fl_rmdir(const char *f)
FL_EXPORT char* fl_getenv(const char *name)
FL_EXPORT int fl_mkdir(const char* f, int mode)

#  ifdef __cplusplu

#  endi

#endi

/
// End of "$Id$"
/

