//
// "$Id: filename_isdir.cxx,v 1.11 2002/12/10 02:00:56 easysw Exp $"
//
// Directory detection routines for the Fast Light Tool Kit (FLTK).
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

// Used by file_chooser

#include <config.h>
#include <fltk/filename.h>
#include <sys/stat.h>
#include <string.h>

bool filename_isdir(const char* name) {
  struct stat s;
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

  return !stat(name, &s) && (s.st_mode&0170000)==0040000;
}

//
// End of "$Id: filename_isdir.cxx,v 1.11 2002/12/10 02:00:56 easysw Exp $".
//
