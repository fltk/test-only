//
// "$Id: filename_setext.cxx,v 1.9 2004/01/20 07:27:28 spitzak Exp $"
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
#include <string.h>

/*! \defgroup utilities Non-fltk Utility Functions

  FLTK provides some functions that it uses internally that are
  necessary for portablity, or convienent for writing code.  These are
  \e not in the fltk:: namespace and do not have "fl_" in their names,
  because in theory they should not be part of fltk, but instead
  provided by the system.
*/

/*! Does strcpy(filename_ext(buf), ext ? ext : ""). Returns \a buf.

  \a ext is usually a desired extension, starting with period, such
  as ".jpg". If it is null it acts like "".

  If there is no period in \a buf, \a ext is concatenated.

  Otherwise the last period is replaced with the first character of
  \a ext and the rest copied there.
*/
char *filename_setext(char *buf, const char *ext) {
  char *q = (char *)filename_ext(buf);
  if (ext) strcpy(q,ext); else *q = 0;
  return(buf);
}

//
// End of "$Id: filename_setext.cxx,v 1.9 2004/01/20 07:27:28 spitzak Exp $".
//
