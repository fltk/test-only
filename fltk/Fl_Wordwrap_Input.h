//
// "$Id: Fl_Wordwrap_Input.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Wordwrap input header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Wordwrap_Input_H
#define Fl_Wordwrap_Input_H

#include "Fl_Input.h"

// This class is entirely inline.  If that changes, add FL_API to its declaration
/**

   This input field displays '\n' characters as new lines rather
   than ^J, and accepts the Return, Tab, and up and down arrow keys,
   and wraps long lines to fit into paragraphs that fit in the window.
   This is useful for editing small blocks of text, but you may want 
   Fl_Text_Editor instead.

*/
class Fl_Wordwrap_Input : public Fl_Input {
public:
   /**
      Creates a new Fl_Wordwrap_Input widget using the 
      given position, size, and label string. 
   */
    Fl_Wordwrap_Input(int x,int y,int w,int h,const char *l = 0)
	: Fl_Input(x,y,w,h,l) {type(WORDWRAP);}
};

#endif

//
// End of "$Id: Fl_Wordwrap_Input.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
