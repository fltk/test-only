/
// "$Id$
/
// Round clock header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Round_Clock_
#define Fl_Round_Clock_

#include "Fl_Clock.H

class Fl_Round_Clock : public Fl_Clock 
public
    Fl_Round_Clock(int x,int y,int w,int h, const char *l = 0
	: Fl_Clock(x,y,w,h,l) {type(FL_ROUND_CLOCK); box(FL_NO_BOX);
}

#endi

/
// End of "$Id$"
/
