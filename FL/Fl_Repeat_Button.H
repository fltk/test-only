/
// "$Id$
/
// Repeat button header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Repeat_Button_
#define Fl_Repeat_Button_
#include "Fl.H
#include "Fl_Button.H

/** The Fl_Repeat_Button is a subclass of Fl_Button that generates 
 * callback when it is pressed and then repeatedly generates callbacks
 * as long as it is held down. The speed of the repeat is fixed and
 * depends on the implementation
 *
class FL_EXPORT Fl_Repeat_Button : public Fl_Button 
  static void repeat_callback(void *)
public
  int handle(int)
    /** Creates a new Fl_Repeat_Button widget using the given \a position,
     * \a size, and \a label string. The default boxtype is \c FL_UP_BOX . *
  Fl_Repeat_Button(int X,int Y,int W,int H,const char *label=0
    : Fl_Button(X,Y,W,H,label) {
  void deactivate() 
    Fl::remove_timeout(repeat_callback,this)
    Fl_Button::deactivate()
  
}

#endi

/
// End of "$Id$"
/
