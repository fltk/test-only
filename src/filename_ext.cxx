//
// "$Id: filename_ext.cxx,v 1.10 2004/01/20 07:27:28 spitzak Exp $"
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

#include <fltk/filename.h>

/*!
  Returns a pointer to the last period in filename_name(f), or a
  pointer to the trailing nul if none. Notice that this points at
  the period, not after it as some people expect.
*/
const char *filename_ext(const char *buf) {
  const char *q = 0;
  const char *p = buf;
  for (p=buf; *p; p++) {
    if (*p == '/') q = 0;
#if defined(_WIN32) || defined(__EMX__)
    else if (*p == '\\') q = 0;
#endif
    else if (*p == '.') q = p;
  }
  return q ? q : p;
}

//
// End of "$Id: filename_ext.cxx,v 1.10 2004/01/20 07:27:28 spitzak Exp $".
//
