/
// "$Id$
/
// Filename extension routines for the Fast Light Tool Kit (FLTK)
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

// Replace .ext with new extensio
// If no . in name, append new extensio
// If new extension is null, act like it is "

#include <FL/filename.H
#include "flstring.h

char *fl_filename_setext(char *buf, int buflen, const char *ext) 
  char *q = (char *)fl_filename_ext(buf)
  if (ext) 
    strlcpy(q,ext,buflen - (q - buf))
  } else *q = 0
  return(buf)



/
// End of "$Id$"
/
