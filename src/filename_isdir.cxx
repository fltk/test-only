//
// "$Id: filename_isdir.cxx,v 1.21 2004/07/29 09:07:54 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <config.h>
#include <fltk/filename.h>
#include <fltk/utf.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#include <fltk/x.h>
#endif

static struct stat last_stat;
static char *last_statname = 0;
static bool last_result = false;
static int last_op = 0;

// This function makes sure that 'stat' is only called once if the users
// queries different aspects on the same file. 'stat' can be relatively slow.
static bool fill_stat(const char *name, int new_op) {
  if (!last_statname) {
    last_statname = (char*)malloc(FL_PATH_MAX);
    *last_statname = 0;
  }
  if (last_op!=new_op && strcmp(last_statname, name)==0) return last_result;
  strncpy(last_statname, name, FL_PATH_MAX-1);
  last_statname[FL_PATH_MAX-1] = 0;
  char namebuf[FL_PATH_MAX];
  utf8tomb(name, strlen(name), namebuf, FL_PATH_MAX);
  name = namebuf;
#if defined(_WIN32) || defined(__EMX__)
  // _WIN32 apparently thinks A: is not a directory, but A:/ is!
  char buffer[4];
  if (name[0] && name[1]==':' && name[2] == 0) {
    buffer[0] = name[0];
    buffer[1] = ':';
    buffer[2] = '/';
    buffer[3] = 0;
    name = buffer;
  }
#endif // _WIN32 || __EMX__
  last_result = (stat(name, &last_stat)==0);
  return last_result;
}

/*! Returns non-zero if the file exists and is a directory. */
bool filename_isdir(const char* name) {
  if (fill_stat(name, 1)==false) return false;
  return (last_stat.st_mode&0170000)==0040000;
}

/*! Returns the size of the file in bytes. Returns zero if it does not exist.*/
double filename_size(const char* name) {
  if (fill_stat(name, 2)==false) return 0.0;
  return (double)last_stat.st_size;
}

/*! Returns the modification time of the file as a Unix timestamp.
  (number of seconds since the start of 1970 in GMT)
*/
long int filename_mtime(const char *name) {
  if (fill_stat(name, 3)==false) return 0;
  if (last_stat.st_mtime) return last_stat.st_mtime;
  if (last_stat.st_atime) return last_stat.st_atime;
  return last_stat.st_ctime;
}

//
// End of "$Id: filename_isdir.cxx,v 1.21 2004/07/29 09:07:54 spitzak Exp $".
//
