//
// "$Id: filename_isdir.cxx,v 1.5 2000/01/07 22:58:54 mike Exp $"
//
// Directory detection routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// Used by fl_file_chooser

#include <config.h>
#include <FL/filename.H>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

int filename_isdir(const char* n) {
  struct stat s;
#if defined(WIN32) || defined(__EMX__)
  // Append a slash to the pathname if we have L:, since WIN32 is horribly
  // stupid about that...
  char pathname[1024];
  if (strlen(n) == 2 && n[1] == ':') {
    snprintf(pathname, sizeof(pathname), "%s/", n);
    n = pathname;
  }
#endif // WIN32 || __EMX__

  return !stat(n, &s) && (s.st_mode&0170000)==0040000;
}

//
// End of "$Id: filename_isdir.cxx,v 1.5 2000/01/07 22:58:54 mike Exp $".
//
