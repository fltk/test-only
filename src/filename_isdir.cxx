/
// "$Id$
/
// Directory detection routines for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

// Used by fl_file_choose

#include "flstring.h
#include <sys/types.h
#include <sys/stat.h
#include <ctype.h
#include <FL/filename.H


#if defined(WIN32) || defined(__EMX__) && !defined(__CYGWIN__
static inline int isdirsep(char c) {return c=='/' || c=='\\';
#els
#define isdirsep(c) ((c)=='/'
#endi

int fl_filename_isdir(const char* n) 
  struct stat	s

#ifdef WIN3
  char		fn[1024]
  int		length
  // This workaround brought to you by the fine folks at Microsoft
  // (read lots of sarcasm in that...
  length = strlen(n)
  if (length < (int)(sizeof(fn) - 1)) 
    if (length < 4 && isalpha(n[0]) && n[1] == ':' &
        (isdirsep(n[2]) || !n[2])) 
      // Always use D:/ for drive letter
      fn[0] = n[0]
      strcpy(fn + 1, ":/")
      n = fn
    } else if (length > 0 && isdirsep(n[length - 1])) 
      // Strip trailing slash from name..
      length --
      memcpy(fn, n, length)
      fn[length] = '\0'
      n = fn
    
  
#endi

  return !stat(n, &s) && (s.st_mode&0170000)==0040000


/
// End of "$Id$"
/
