//
// "$Id: Fl_Multiline_Input.H,v 1.4.2.3.2.2.2.2 2003/11/19 02:46:11 matthiaswm Exp $"
//
// Multiline input header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef Fl_Multiline_Input_H
#define Fl_Multiline_Input_H

#include "Fl_Input.H"

/** This input field displays '\\n' characters as new lines rather than ^J,
 * and accepts the Return, Tab, and up and down arrow keys. This is for 
 * editing multiline text.
 *
 * This is far from the nirvana of text editors, and is probably only good
 * for small bits of text, 10 lines at most. I think FLTK can be used to 
 * write a powerful text editor, but it is not going to be a built-in 
 * feature. Powerful text editors in a toolkit are a big source of bloat. 
 * \see Fl_Text_Editor, Fl_Text_Display, Fl_Text_Buffer
 */
class Fl_Multiline_Input : public Fl_Input {
public:
      /** Creates a new Fl_Multiline_Input widget using the given position, 
       * size, and label string. The default boxtype is \c FL_DOWN_BOX. */
    Fl_Multiline_Input(int X,int Y,int W,int H,const char *l = 0)
	: Fl_Input(X,Y,W,H,l) {type(FL_MULTILINE_INPUT);}
};

#endif

//
// End of "$Id: Fl_Multiline_Input.H,v 1.4.2.3.2.2.2.2 2003/11/19 02:46:11 matthiaswm Exp $".
//
