//
// "$Id$"
//
// Clock header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::Clock, fltk3::ClockOutput widgets . */

#ifndef Fltk3_Clock_H
#define Fltk3_Clock_H

#include "Widget.h"
#include <fltk3/Wrapper.h>


namespace fltk3 {
  
  // values for type:
  const uchar SQUARE_CLOCK = 0;	/**< type() of Square Clock variant */
  const uchar ROUND_CLOCK = 1;	/**< type() of Round Clock variant */
  const uchar ANALOG_CLOCK = SQUARE_CLOCK;	/**< An analog clock is square */
  const uchar DIGITAL_CLOCK = SQUARE_CLOCK; /**< Not yet implemented */
  
  // fabien: Please keep the horizontal formatting of both images in class desc, 
  // don't lose vert. space for nothing!
  
  /**
   \class fltk3::ClockOutput
   \brief This widget can be used to display a program-supplied time.
   
   The time shown on the clock is not updated. To display the current time,
   use fltk3::Clock instead.
   
   \htmlonly <BR>  <table align=CENTER border=1 cellpadding=5 >  
   <caption align=bottom>type() fltk3::SQUARE_CLOCK and fltk3::ROUND_CLOCK </caption> <TR><TD> \endhtmlonly
   \image html clock.png  
   \htmlonly </TD> <TD> \endhtmlonly
   \image html round_clock.png 
   \htmlonly </TD> </TR> </table> \endhtmlonly
   \image latex clock.png "fltk3::SQUARE_CLOCK type" width=4cm
   \image latex round_clock.png "fltk3::ROUND_CLOCK type" width=4cm
   */
  class FLTK3_EXPORT ClockOutput : public Widget {

    int hour_, minute_, second_;
    ulong value_;
    void drawhands(fltk3::Color,fltk3::Color); // part of draw
  protected:
    void draw();
    void draw(int X, int Y, int W, int H);
  public:
    
    ClockOutput(int X, int Y, int W, int H, const char *L = 0);
    
    void value(ulong v);	// set to this Unix time
    
    void value(int H, int m, int s);
    
    /**
     Returns the displayed time.
     Returns the time in seconds since the UNIX epoch (January 1, 1970).
     \see value(ulong)
     */
    ulong value() const {return value_;}
    
    /**
     Returns the displayed hour (0 to 23).
     \see value(), minute(), second()
     */
    int hour() const {return hour_;}
    
    /**
     Returns the displayed minute (0 to 59).
     \see value(), hour(), second()
     */
    int minute() const {return minute_;}
    
    /**
     Returns the displayed second (0 to 60, 60=leap second).
     \see value(), hour(), minute()
     */
    int second() const {return second_;}
  };
  
  // a fltk3::Clock displays the current time always by using a timeout:
  
  /**
   \class fltk3::Clock
   \brief This widget provides a round analog clock display.
   
   fltk3::Clock is provided for Forms compatibility. 
   It installs a 1-second timeout callback using fltk3::add_timeout().
   You can choose the rounded or square type of the clock with type(), see below.
   \htmlonly <BR>  <table align=CENTER border=1 cellpadding=5 >  
   <caption align=bottom>type() fltk3::SQUARE_CLOCK and fltk3::ROUND_CLOCK </caption> <TR><TD> \endhtmlonly
   \image html clock.png  
   \htmlonly </TD> <TD> \endhtmlonly
   \image html round_clock.png 
   \htmlonly </TD> </TR> </table> \endhtmlonly
   \image latex clock.png "fltk3::SQUARE_CLOCK type" width=4cm  
   \image latex round_clock.png "fltk3::ROUND_CLOCK type" width=4cm
   */
  class FLTK3_EXPORT Clock : public ClockOutput {

  public:
    int handle(int);
    
    Clock(int X, int Y, int W, int H,  const char *L = 0);
    
    Clock(uchar t, int X, int Y, int W, int H, const char *L);
    
    ~Clock();
  };
  
} // namespace

#endif

//
// End of "$Id$".
//
