//
// "$Id: Fl_Multi_Browser.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Multi browser header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Multi_Browser_H
#define Fl_Multi_Browser_H

#include "Fl_Browser.h"

// This class is entirely inline.  If that changes, add FL_API to its declaration
/**

   The Fl_Multi_Browser class is a subclass of Fl_Browser which lets the 
   user select any set of the lines. Clicking on an item selects only that
   one. Ctrl+click toggles items on/off. Shift+drag (or shift+arrows) will
   extend selections. Normally the callback is done when any item changes 
   it's state, but you can change this with when(). 
   
   See Fl_Browser for methods to control the display and "current item", 
   and Fl_Menu_ for methods to add and remove lines from the browser. 

   The methods on Fl_Browser for controlling the "value" control item
   with the keyboard focus in a multi-browser. You must use the "select" 
   methods described here to change what items are turned on. 

*/
class Fl_Multi_Browser : public Fl_Browser {
public:
   /**
      Creates a new, empty Fl_Multi_Browser.
   */
    Fl_Multi_Browser(int x,int y,int w,int h,const char *l=0)
	: Fl_Browser(x,y,w,h,l) {type(MULTI);}
};

#endif

//
// End of "$Id: Fl_Multi_Browser.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
