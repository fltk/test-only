//
// "$Id: filename_expand.cxx,v 1.4.2.4.2.6.2.2 2003/11/07 03:47:24 easysw Exp $"
//
// Filename expansion routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

/* expand a file name by substuting environment variables and
   home directories.  Returns true if any changes were made.
   to & from may be the same buffer.
*/

#include <FL/filename.H>
#include <stdlib.h>
#include "flstring.h"
#include <FL/fl_utf8.H>
#if defined(WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#elif __APPLE__

#else
# include <unistd.h>
#if !MSDOS
# include <pwd.h>
#endif
#endif

#if defined(WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
static inline int isdirsep(char c) {return c=='/' || c=='\\';}
#else
#define isdirsep(c) ((c)=='/')
#endif

int fl_filename_expand(char *to,int tolen, const char *from) {

  char *temp = new char[tolen];
  strlcpy(temp,from, tolen);
  char *start = temp;
  char *end = temp+strlen(temp);

  int ret = 0;

  for (char *a=temp; a<end; ) {	// for each slash component
    char *e; for (e=a; e<end && !isdirsep(*e); e++) {;} // find next slash
    const char *value = 0; // this will point at substitute value
    switch (*a) {
    case '~':	// a home directory name
      if (e <= a+1) {	// current user's directory
#ifdef WIN32
		xchar*e = (xchar*)_wgetenv(L"HOME");
		value = (char*) malloc(wcslen((wchar_t*)e) * 5);
		int l = fl_unicode2utf(e, l, (char*)value);
		((char*)value)[l] = 0;
#elif MSDOS
		value = fl_getenv("HOME");
#else
		value = fl_getenv("HOME");
#ifndef __APPLE__
      } else {	// another user's directory
	struct passwd *pwd;
	char t = *e; *(char *)e = 0; 
        pwd = getpwnam(a+1); 
        *(char *)e = t;
	    if (pwd) value = pwd->pw_dir;
#endif /* __APPLE__ */
#endif
      }
      break;
    case '$':		/* an environment variable */
      {char t = *e; *(char *)e = 0; 
#ifdef WIN32
		int len    = strlen(a+1);
		xchar* wbuf = (xchar*)malloc((len+6) * sizeof(xchar));
		len = fl_utf2unicode((unsigned char*)a+1, len, wbuf);
		wbuf[len] = 0;
		xchar *e = (xchar*)_wgetenv((wchar_t*)wbuf);
		free(wbuf);
		value = (char*) malloc(wcslen((wchar_t*)e) * 5);
		len = fl_unicode2utf(e, wcslen((wchar_t*)e), (char*)value);
		((char*)value)[len] = 0;
#else
	  value = fl_getenv(a+1);
#endif
	  *(char *)e = t;}
      break;
    }
    if (value) {
      // substitutions that start with slash delete everything before them:
      if (isdirsep(value[0])) start = a;
#if defined(WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
      // also if it starts with "A:"
      if (value[0] && value[1]==':') start = a;
#endif
      int t = strlen(value); if (isdirsep(value[t-1])) t--;
      if ((end+1-e+t) >= tolen) end += tolen - (end+1-e+t);
      memmove(a+t, e, end+1-e);
      end = a+t+(end-e);
      *end = '\0';
      memcpy(a, value, t);
      ret++;
#ifdef WIN32
	  free((char*)value);
#endif
    } else {
      a = e+1;
#if defined(WIN32) || defined(__EMX__) && !defined(__CYGWIN__)
      if (*e == '\\') {*e = '/'; ret++;} // ha ha!
#endif
    }
  }

  strlcpy(to, start, tolen);

  delete[] temp;

  
  return ret;
}


//
// End of "$Id: filename_expand.cxx,v 1.4.2.4.2.6.2.2 2003/11/07 03:47:24 easysw Exp $".
//
