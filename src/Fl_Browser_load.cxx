/
// "$Id$
/
// File loading routines for the Fast Light Tool Kit (FLTK)
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

#include <FL/Fl.H
#include <FL/Fl_Browser.H
#include <stdio.h

int Fl_Browser::load(const char *filename) 
#define MAXFL_BLINE 102
    char newtext[MAXFL_BLINE]
    int c
    int i
    clear()
    if (!filename || !(filename[0])) return 1
    FILE *fl = fopen(filename,"r")
    if (!fl) return 0
    i = 0
    do 
	c = getc(fl)
	if (c == '\n' || c <= 0 || i>=(MAXFL_BLINE-1)) 
	    newtext[i] = 0
	    add(newtext)
	    i = 0
	} els
	    newtext[i++] = c
    } while (c >= 0)
    fclose(fl)
    return 1


/
// End of "$Id$"
/
