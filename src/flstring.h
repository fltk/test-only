/
 * "$Id$
 
 * Common string header file for the Fast Light Tool Kit (FLTK)
 
 * Copyright 1998-2004 by Bill Spitzak and others
 
 * This library is free software; you can redistribute it and/o
 * modify it under the terms of the GNU Library General Publi
 * License as published by the Free Software Foundation; eithe
 * version 2 of the License, or (at your option) any later version
 
 * This library is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty o
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
 * Library General Public License for more details
 
 * You should have received a copy of the GNU Library General Publi
 * License along with this library; if not, write to the Free Softwar
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
 * USA
 
 * Please report all bugs and problems to "fltk-bugs@fltk.org"
 *

#ifndef flstring_
#  define flstring_

#  include <FL/Fl_Export.H
#  include <config.h
#  include <stdarg.h
#  include <string.h
#  ifdef HAVE_STRINGS_
#    include <strings.h
#  endif /* HAVE_STRINGS_H *
#  include <ctype.h

/
 * Apparently Unixware defines "index" to strchr (!) rather tha
 * providing a proper entry point or not providing the (obsolete
 * BSD function.  Make sure index is not defined..
 *

#  ifdef inde
#    undef inde
#  endif /* index *

#  if defined(WIN32) && !defined(__CYGWIN__
#    define strcasecmp(s,t)	stricmp((s), (t)
#    define strncasecmp(s,t,n)	strnicmp((s), (t), (n)
#  elif defined(__EMX__
#    define strcasecmp(s,t)	stricmp((s), (t)
#    define strncasecmp(s,t,n)	strnicmp((s), (t), (n)
#  endif /* WIN32 *

#  ifdef __cplusplu
extern "C" 
#  endif /* __cplusplus *

/
 * MetroWerks' CodeWarrior put thes "non-standard" functions i
 * <extras.h> which unfortunatly does not play well otherwis
 * when included - to be resolved..
 *

#  if defined(__APPLE__) && defined(__MWERKS__) && defined(_MSL_USING_MW_C_HEADERS
int strcasecmp(const char*,const char*)
int strncasecmp(const char*,const char*,int)
char *strdup(const char*)
#  endi

#  if !HAVE_SNPRINT
FL_EXPORT extern int fl_snprintf(char *, size_t, const char *, ...)
#    define snprintf fl_snprint
#  endif /* !HAVE_SNPRINTF *

#  if !HAVE_VSNPRINT
FL_EXPORT extern int fl_vsnprintf(char *, size_t, const char *, va_list ap)
#    define vsnprintf fl_vsnprint
#  endif /* !HAVE_VSNPRINTF *

/
 * strlcpy() and strlcat() are some really useful BSD string function
 * that work the way strncpy() and strncat() *should* have worked
 *

#  if !HAVE_STRLCA
FL_EXPORT extern size_t fl_strlcat(char *, const char *, size_t)
#    define strlcat fl_strlca
#  endif /* !HAVE_STRLCAT *

#  if !HAVE_STRLCP
FL_EXPORT extern size_t fl_strlcpy(char *, const char *, size_t)
#    define strlcpy fl_strlcp
#  endif /* !HAVE_STRLCPY *

#  ifdef __cplusplu

#  endif /* __cplusplus *
#endif /* !flstring_h *


/
 * End of "$Id$"
 *
