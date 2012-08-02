//
// "$Id$"
//
// Adjuster widget header file for the Fast Light Tool Kit (FLTK).
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

// FLTK 123 complete

/** \file
 fltk3::Adjuster widget 
 */

#ifndef FLTK3_Adjuster_H
#define FLTK3_Adjuster_H

#include "Valuator.h"


namespace fltk3 {
  
  /**
   \brief The Adjuster widget allows for quick numeric manipulation.
   
   The Adjuster widget was inspired by Prisms, and has proven to be very useful
   for values that need a large dynamic range.
   
   \image html adjuster1.png 
   \image latex adjuster1.png "fltk3::Adjuster" width=4cm
   
   When you  press a button and drag to the right the value increases. When you 
   drag to the left it decreases.  The largest button adjusts by <i>100 * 
   step()</i>, the next by <i>10 * step()</i> and that smallest button by 
   <i>step()</i>.  
   
   Clicking on the buttons increments by 10 times the amount dragging by a 
   pixel does. Shift + click decrements by 10 times the amount.
   */
  class FLTK3_EXPORT Adjuster : public Valuator {
    
  private:
    int pDragButton; ///< 1..3, indicating the clicked range
    bool pSoftBoundaries; ///< set if soft boundaries are switched on
    
  protected:
    void draw();
    int handle(int);
    virtual void value_damage();
    
  public:
    
    /**
     \brief Create the Adjuster widget.
     
     Creates a new fltk3::Adjuster widget using the given position, size, and 
     label string. It looks best if one of the dimensions is 3 times the other.
     
     Inherited destructor destroys the Valuator.
     
     \param X, Y, W, H position and size
     \param l label text
     */
    Adjuster(int X,int Y,int W,int H,const char *l=0);
    
    /**
     \brief Set soft boundaries.
     
     If "soft" is turned on, the user is allowed to drag the value outside
     the range.  If they drag the value to one of the ends, let go, then
     grab again and continue to drag, they can get to any value.  Default is
     one.
     
     \param s false to turn off, true to turn on
     \see fltk3::Valuator::range(doube min, double max)
     */
    void soft(bool onOff) { pSoftBoundaries = onOff; }
    
    /**
     \brief Return 1 if soft boundaries are switched on.
     
     \return status of soft boundaries
     \see fltk3::Valuator::range(doube min, double max)
     \see fltk3::Adjuster(int s)
     */
    int soft() const { return pSoftBoundaries; }
  };
  
} // namespace

#endif

//
// End of "$Id$".
//
