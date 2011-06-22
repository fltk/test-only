//
// "$Id: Dial.h 7981 2010-12-08 23:53:04Z greg.ercolano $"
//
// Dial header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::Dial widget . */

#ifndef Fltk3_Dial_H
#define Fltk3_Dial_H

#include "Valuator.h"
#include <fltk3/Wrapper.h>


class Fl_Dial;


namespace fltk3 {
  
  // values for type():
  const uchar NORMAL_DIAL = 0;	/**< type() for dial variant with dot */
  const uchar LINE_DIAL   = 1;	/**< type() for dial variant with line */
  const uchar FILL_DIAL   = 2;	/**< type() for dial variant with filled arc */
  
  /**
   The fltk3::Dial widget provides a circular dial to control a
   single floating point value.
   <P ALIGN=CENTER>\image html dial.png 
   \image latex dial.png "fltk3::Dial" width=4cm
   Use type() to set the type of the dial to:
   <UL>
   <LI>fltk3::NORMAL_DIAL - Draws a normal dial with a knob. </LI>
   <LI>fltk3::LINE_DIAL - Draws a dial with a line. </LI>
   <LI>fltk3::FILL_DIAL - Draws a dial with a filled arc. </LI>
   </UL>
   
   */
  class FLTK3_EXPORT Dial : public fltk3::Valuator {

    friend class ::Fl_Dial;
    
    short a1,a2;
    
  protected:
    
    // these allow subclasses to put the dial in a smaller area:
    void draw(int X, int Y, int W, int H);
    int handle(int event, int X, int Y, int W, int H);
    void draw();
    
  public:
    
    int handle(int);
    /**
     Creates a new fltk3::Dial widget using the given position, size,
     and label string. The default type is fltk3::NORMAL_DIAL.
     */
    Dial(int x,int y,int w,int h, const char *l = 0);
    /** 
     Sets Or gets the angles used for the minimum and maximum values.  The default
     values are 45 and 315 (0 degrees is straight down and the angles
     progress clockwise).  Normally angle1 is less than angle2, but if you
     reverse them the dial moves counter-clockwise.
     */
    short angle1() const {return a1;}
    /** See short angle1() const */
    void angle1(short a) {a1 = a;}
    /** See short angle1() const */
    short angle2() const {return a2;}
    /** See short angle1() const */
    void angle2(short a) {a2 = a;}
    /** See short angle1() const */
    void angles(short a, short b) {a1 = a; a2 = b;}
    
  };
  
}

#endif

//
// End of "$Id: Dial.h 7981 2010-12-08 23:53:04Z greg.ercolano $".
//
