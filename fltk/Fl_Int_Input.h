//
// "$Id: Fl_Int_Input.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Integer input header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Int_Input_H
#define Fl_Int_Input_H

#include "Fl_Float_Input.h"

/**
   The Fl_Int_Input class is a subclass of Fl_Input that only allows the 
   user to type decimal digits (or hex numbers of the form 0xaef). Besides
   editing the text the user can use the up/down arrow keys to change the
   digits. 

   You may want a Fl_Value_Input widget instead. It has value() methods 
   that take and return double values rather than strings. 

   If you change when() to FL_WHEN_ENTER_KEY the callback is only done when
   the user hits the up/down arrow keys or when the user types the Enter key.
   This may be more useful than the default setting of FL_WHEN_CHANGED which 
   can make the callback happen when partially-edited numbers are in 
   the field.
*/
class FL_API Fl_Int_Input : public Fl_Float_Input {
public:
  /**
   Creates a new Fl_Int_Input widget using the given position, 
   size, and label string. 
  */
  Fl_Int_Input(int x,int y,int w,int h,const char *l = 0)
    : Fl_Float_Input(x,y,w,h,l) {type(INT);}
};

#endif

//
// End of "$Id: Fl_Int_Input.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
