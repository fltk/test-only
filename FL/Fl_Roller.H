/
// "$Id$
/
// Roller header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Roller_
#define Fl_Roller_

#ifndef Fl_Valuator_
#include "Fl_Valuator.H
#endi

/** The Fl_Roller widget is a "dolly" control commonly used to move
 * 3D objects.
 *   \image html Fl_Roller.gi
 *
class FL_EXPORT Fl_Roller : public Fl_Valuator 
protected
  void draw()
public
  int handle(int)
    /** Creates a new Fl_Roller widget using the given position, size, an
     * label string. The default boxtype is \c FL_NO_BOX. *
  Fl_Roller(int X,int Y,int W,int H,const char* L=0)
}

#endi

/
// End of "$Id$"
/
