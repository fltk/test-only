//
// "$Id: Counter.h 7981 2010-12-08 23:53:04Z greg.ercolano $"
//
// Counter header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::Counter widget . */

// A numerical value with up/down step buttons.  From Forms.

#ifndef Fltk3_Counter_H
#define Fltk3_Counter_H

#include "Valuator.h"
#include <fltk3/Wrapper.h>

class Fl_Counter;

namespace fltk3 {
  
  // values for type():
  const uchar NORMAL_COUNTER	= 0;	/**< type() for counter with fast buttons */
  const uchar SIMPLE_COUNTER	= 1;	/**< type() for counter without fast buttons */
  
  /**
   Controls a single floating point value with button (or keyboard) arrows.
   Double arrows buttons achieve larger steps than simple arrows.
   \see fltk3::Spinner for value input with vertical step arrows.
   <P align=center>\image html counter.png</P>
   \image latex counter.png "fltk3::Counter" width=4cm
   
   \todo Refactor the doxygen comments for fltk3::Counter type() documentation.
   
   The type of an fltk3::Counter object can be set using type(uchar t) to:
   \li \c fltk3::NORMAL_COUNTER: Displays a counter with 4 arrow buttons.
   \li \c fltk3::SIMPLE_COUNTER: Displays a counter with only 2 arrow buttons.
   */
  class FLTK3_EXPORT Counter : public fltk3::Valuator {

    friend class ::Fl_Counter;
    
    double lstep_;
    uchar mouseobj;
    static void repeat_callback(void *);
    int calc_mouseobj();
    void increment_cb();
    
  protected:
    
    void draw();
    
  public:
    
    int handle(int);
    
    Counter(int X, int Y, int W, int H, const char* L = 0);
    ~Counter();
    
    /**
     Sets the increment for the large step buttons.
     The default value is 1.0.
     \param[in] a large step increment.
     */
    void lstep(double a) {lstep_ = a;}
    
    /**
     Sets the increments for the normal and large step buttons.
     \param[in] a, b normal and large step increments.
     */
    void step(double a,double b) {Valuator::step(a); lstep_ = b;}
    
    /**
     Sets the increment for the normal step buttons.
     \param[in] a normal step increment.
     */
    void step(double a) {Valuator::step(a);}
    
    /**
     Returns the increment for normal step buttons.
     */
    double step() const {return Valuator::step();}    
  };
  
}

#endif

//
// End of "$Id: Counter.h 7981 2010-12-08 23:53:04Z greg.ercolano $".
//
