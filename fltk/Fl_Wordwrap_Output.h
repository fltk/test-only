//
// "$Id: Fl_Wordwrap_Output.h,v 1.2 2002/01/20 07:37:15 spitzak Exp $"
//
// Multi line output header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Wordwrap_Output_H
#define Fl_Wordwrap_Output_H

#include "Fl_Output.h"

class FL_API Fl_Wordwrap_Output : public Fl_Output {
public:
    Fl_Wordwrap_Output(int x,int y,int w,int h,const char *l = 0)
	: Fl_Output(x,y,w,h,l) {type(WORDWRAP);}
};

#endif

//
// End of "$Id: Fl_Wordwrap_Output.h,v 1.2 2002/01/20 07:37:15 spitzak Exp $".
//
