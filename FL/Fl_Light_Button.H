/
// "$Id$
/
// Lighted button header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Light_Button_
#define Fl_Light_Button_

#include "Fl_Button.H

/** Buttons generate callbacks when they are clicked by the user.
 * You control exactly when and how by changing the values for
 * type() and when()
 *   \image html Fl_Light_Button.gi
 * The Fl_Light_Button subclass display the \a on state by turning
 * on a light, rather than drawing pushed in. The shape of the
 * "light" is initially set to \c FL_DOWN_BOX. The color of the light
 * when on is controlled with selection_color(), which defaults
 * to \c FL_YELLOW. *
class FL_EXPORT Fl_Light_Button : public Fl_Button 
  FL_STYLE


protected
    virtual void draw()




public
   virtual int handle(int event)
      /** Creates a new Fl_Light_Button widget using the given \a position,
       * \a size, and \a label string. *
  Fl_Light_Button(int x,int y,int width,int height,const char *label = 0)
}

#endi

/
// End of "$Id$"
/
