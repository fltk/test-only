//
// "$Id$"
//
// Select browser header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Select_Browser_H
#define Fl_Select_Browser_H

#include "Fl_Browser.H"

/** The Fl_Select_Browser class is a subclass of Fl_Browser which lets the
 * user select a single item, or no items by clicking on the empty space.
 * As long as the mouse button is held down the item pointed to by it is
 * highlighted. Normally the callback is done when the user presses the
 * mouse, but you can change this with when().
 * 
 * \see Fl_Browser for methods to add and remove lines from the browser 
 */
class Fl_Select_Browser : public Fl_Browser {
public:
      /** Creates a new Fl_Select_Browser widget using the given position,
       * size, and label string. The default boxtype is \c FL_DOWN_BOX. */
    Fl_Select_Browser(int X,int Y,int W,int H,const char *l=0)
	: Fl_Browser(X,Y,W,H,l) {type(FL_SELECT_BROWSER);}
};

#endif

//
// End of "$Id$".
//
