//
// "$Id: Fl_Secret_Input.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Secret input header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Secret_Input_H
#define Fl_Secret_Input_H

#include "Fl_Input.h"

/**

   The Fl_Secret_Input class is a subclass of Fl_Input that displays 
   its input as a string of asterisks. Attempts to cut, copy, or drag 
   text out of the field are also prevented. This subclass is usually
   used to recieve passwords and other "secret" information. 

*/
class FL_API Fl_Secret_Input : public Fl_Input {
public:
   /**
      Creates a new Fl_Secret_Input widget using the given position,
      size, and label string. 
   */
    Fl_Secret_Input(int x,int y,int w,int h,const char *l = 0)
	: Fl_Input(x,y,w,h,l) {type(SECRET);}
};

#endif

//
// End of "$Id: Fl_Secret_Input.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
