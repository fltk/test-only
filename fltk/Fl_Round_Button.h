//
// "$Id: Fl_Round_Button.h,v 1.5 2002/10/29 00:37:23 easysw Exp $"
//
// Round button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Round_Button_H
#define Fl_Round_Button_H

#include "Fl_Radio_Button.h"

// Back-compatability, this is a radio button with check-button behavior

// This class is entirely inline.  If that changes, add FL_API to its declaration
/**

   Buttons generate callbacks when they are clicked by the user. 
   You control exactly when and how by changing the values 
   for type() and when(). 

   \image Fl_Round_Button.gif

   The Fl_Round_Button subclass display the "on" state by 
   turning on a light, rather than drawing pushed in. The shape
   of the "light" is initially set to FL_ROUND_DOWN_BOX. The color
   of the light when on is controlled with selection_color(), which
   defaults to FL_RED. 

*/
class Fl_Round_Button : public Fl_Radio_Button {
public:
  /** 
   Creates a new Fl_Round_Button widget using the given 
   position, size, and label string.
  */
  Fl_Round_Button(int x,int y,int w,int h,const char *l)
    : Fl_Radio_Button(x,y,w,h,l) { type(TOGGLE); }
};

#endif

//
// End of "$Id: Fl_Round_Button.h,v 1.5 2002/10/29 00:37:23 easysw Exp $".
//
