//
// "$Id: Fl_display.cxx,v 1.11 2002/12/15 10:42:53 spitzak Exp $"
//
// Display function for the Fast Light Tool Kit (FLTK).
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

// Startup method to set what display to use.  Using setenv makes
// programs that are exec'd use the same display.

// This sets the environment variable on non-X systems, in case they
// can pay attention to it. Unfortunatly it also does some
// uglification that X needs: it adds the "0.0" to the end if it
// appears to be a machine name. It also appears that putenv is
// missing on some versions of Windows so I commented it all out. Sigh.

#include <fltk/run.h>
#include <stdlib.h>
#include <string.h>

void fltk::display(const char *d) {
#ifndef _WIN32
  char *e = new char[strlen(d)+13];
  strcpy(e,"DISPLAY=");
  strcpy(e+8,d);
  for (char *c = e+8; *c!=':'; c++) if (!*c) {strcpy(c,":0.0"); break;}
  putenv(e);
#endif
}

//
// End of "$Id: Fl_display.cxx,v 1.11 2002/12/15 10:42:53 spitzak Exp $".
//
