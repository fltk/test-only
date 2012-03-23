//
// "$Id$"
//
// Value output header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::ValueOutput widget . */

#ifndef Fltk3_Value_Output_H
#define Fltk3_Value_Output_H

#include "Valuator.h"


namespace fltk3 {
  
  /**
   The fltk3::ValueOutput widget displays a floating point value.
   If step() is not zero, the user can adjust the value by
   dragging the mouse left and right.  The left button moves one step()
   per pixel, the middle by 10 * step(), and the right button by 
   100 * step().
   <P>This is much lighter-weight than 
   fltk3::ValueInput because it contains no text editing code or
   character buffer. </P>
   <P ALIGN=CENTER>\image html fltk3::ValueOutput.png 
   \image latex  fltk3::ValueOutput.png "fltk3::ValueOutput" width=4cm
   */
  class FLTK3_EXPORT ValueOutput : public fltk3::Valuator {

    uchar soft_;
    
  protected:
    void draw();
    
  public:
    int handle(int);
    ValueOutput(int x,int y,int w,int h,const char *l=0);
    
    /**
     If "soft" is turned on, the user is allowed to drag the value outside
     the range.  If they drag the value to one of the ends, let go, then
     grab again and continue to drag, they can get to any value.  Default is
     one.
     */
    void soft(uchar s) {soft_ = s;}
    /**
     If "soft" is turned on, the user is allowed to drag the value outside
     the range.  If they drag the value to one of the ends, let go, then
     grab again and continue to drag, they can get to any value.  Default is
     one.
     */
    uchar soft() const {return soft_;}
    
  };
  
}

#endif

//
// End of "$Id$".
//
