/
// "$Id$
/
// Hold browser header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Hold_Browser_
#define Fl_Hold_Browser_

#include "Fl_Browser.H

/** The Fl_Hold_Browser class is a subclass of Fl_Browser which lets th
 * user select a single item, or no items by clicking on the empty space
 * As long as the mouse button is held down the item pointed to by it i
 * highlighted, and this highlighting remains on when the mouse button i
 * released. Normally the callback is done when the user releases th
 * mouse, but you can change this with when()
 
 * \see Fl_Browser for methods to add and remove lines from the browse
 *
class Fl_Hold_Browser : public Fl_Browser 
public
      /** Creates a new Fl_Hold_Browser widget using the given position
       * size, and label string. The default boxtype is \c FL_DOWN_BOX. *
    Fl_Hold_Browser(int X,int Y,int W,int H,const char *l=0
	: Fl_Browser(X,Y,W,H,l) {type(FL_HOLD_BROWSER);
}

#endi

/
// End of "$Id$"
/
