//
// "$Id$"
//
// Box header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::Box widget . */

#ifndef FLTK3_BOX_H
#define FLTK3_BOX_H

#include "Widget.h"


namespace fltk3 { 
  
  class Widget;

  /**
   \brief This widget draws a box with an optional label.
   
   This widget simply draws its box, and possibly it's label. Putting it
   before some other widgets and making it big enough to surround them
   will let you draw a frame around them.
   */
  class FLTK3_EXPORT Box : public fltk3::Widget {
    
  protected:
    void draw();
    
  public:
    /**
     \brief Box constructor.
     
     This constructor sets box() to fltk3::NO_BOX, which means it is invisible. 
     However such widgets are useful as placeholders or 
     fltk3::Group::resizable() values.  To change the box to something visible, 
     use box(n).
     */
    Box(int X, int Y, int W, int H, const char *l=0);
    
    /**
     \brief Box constructor.
     
     This constructor also sets the box type.
     */
    Box(fltk3::Boxtype b, int X, int Y, int W, int H, const char *l);

    virtual int handle(int);
  };
  
} // namespace

#endif

//
// End of "$Id$".
//
