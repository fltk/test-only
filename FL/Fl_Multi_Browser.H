//
// "$Id: Fl_Multi_Browser.H,v 1.4.2.3.2.1.2.3 2003/12/02 02:51:44 easysw Exp $"
//
// Multi browser header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Multi_Browser_H
#define Fl_Multi_Browser_H

#include "Fl_Browser.H"

/** The Fl_Multi_Browser class is a subclass of Fl_Browser which lets the
 * user select any set of the lines. The user interface is Macintosh style:
 * clicking an item turns off all the others and selects that one,
 * dragging selects all the items the mouse moves over, and shift + click
 * toggles the items. This is different then how forms did it. Normally
 * the callback is done when the user releases the mouse, but you can
 * change this with when().
 *
 * \see Fl_Browser for methods to add and remove lines from the browser. 
 */
class Fl_Multi_Browser : public Fl_Browser {
public:
      /** Creates a new Fl_Multi_Browser widget using the given position,
       * size, and label string. The default boxtype is \c FL_DOWN_BOX. */
    Fl_Multi_Browser(int x,int y,int w,int h,const char *l=0)
	: Fl_Browser(x,y,w,h,l) {type(FL_MULTI_BROWSER);}
};

#endif

//
// End of "$Id: Fl_Multi_Browser.H,v 1.4.2.3.2.1.2.3 2003/12/02 02:51:44 easysw Exp $".
//
