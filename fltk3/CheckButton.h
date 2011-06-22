//
// "$Id: CheckButton.h 7981 2010-12-08 23:53:04Z greg.ercolano $"
//
// Check button header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::CheckButton widget . */

#ifndef Fltk3_Check_Button_H
#define Fltk3_Check_Button_H

#include "LightButton.h"

class Fl_Check_Button;

namespace fltk3 {
  
  /**
   \class fltk3::CheckButton
   \brief A button with an "checkmark" to show its status.
   
   \image html fltk3::CheckButton.png
   \image latex fltk3::CheckButton.png  "fltk3::CheckButton" width=4cm
   Buttons generate callbacks when they are clicked by the user. You control
   exactly when and how by changing the values for type() and when().
   
   The fltk3::CheckButton subclass displays its "ON" state by showing a "checkmark"
   rather than drawing itself pushed in.
   
   \todo Refactor fltk3::CheckButton doxygen comments (add color() info etc?)
   \todo Generate fltk3::CheckButton.gif with visible checkmark.
   */
  class FLTK3_EXPORT CheckButton : public fltk3::LightButton {
    friend class ::Fl_Check_Button;
  public:
    CheckButton(int X, int Y, int W, int H, const char *L = 0);
  };
  
} // namespace

#endif

//
// End of "$Id: CheckButton.h 7981 2010-12-08 23:53:04Z greg.ercolano $".
//
