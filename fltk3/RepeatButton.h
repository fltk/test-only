//
// "$Id: RepeatButton.h 7903 2010-11-28 21:06:39Z matt $"
//
// Repeat button header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
 fltk3::RepeatButton widget . */

#ifndef Fltk3_Repeat_Button_H
#define Fltk3_Repeat_Button_H

#include "run.h"
#include "Button.h"

namespace fltk3 {
  
  /**
   The fltk3::RepeatButton is a subclass of fltk3::Button that
   generates a callback when it is pressed and then repeatedly generates
   callbacks as long as it is held down.  The speed of the repeat is fixed
   and depends on the implementation.
   */
  class FLTK3_EXPORT RepeatButton : public Button {
    static void repeat_callback(void *);
  public:
    int handle(int);
    /**
     Creates a new fltk3::RepeatButton widget using the given
     position, size, and label string. The default boxtype is fltk3::UP_BOX.
     Deletes the button.
     */
    RepeatButton(int X,int Y,int W,int H,const char *l=0)
    : Button(X,Y,W,H,l) {}
    void deactivate() {
      remove_timeout(repeat_callback,this);
      Button::deactivate();
    }
  };
  
}

#endif

//
// End of "$Id: RepeatButton.h 7903 2010-11-28 21:06:39Z matt $".
//
