//
// "$Id: Fl_Multiline_Input.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Multiline input header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Multiline_Input_H
#define Fl_Multiline_Input_H

#include "Fl_Input.h"

// This class is entirely inline.  If that changes, add FL_API to its declaration
/**

   This input field displays '\n' characters as new lines rather 
   than ^J, and accepts the Return, Tab, and up and down arrow 
   keys. Long lines of text scroll horizontally, you may
   want Fl_Wordwrap_Input instead. 
   
   Because there are no scrollbars, this is probably only 
   good for small bits of text, 10 lines at most. More powerful 
   editing is available from the Fl_Text_Editor widget. 

*/
class Fl_Multiline_Input : public Fl_Input {
public:
   /**
      Creates a new Fl_Multiline_Input widget using the 
      given position, size, and label string.
   */
    Fl_Multiline_Input(int x,int y,int w,int h,const char *l = 0)
	: Fl_Input(x,y,w,h,l) {type(MULTILINE);}
};

#endif

//
// End of "$Id: Fl_Multiline_Input.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
