/
// "$Id$
/
// Dial header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Dial_
#define Fl_Dial_

#ifndef Fl_Valuator_
#include "Fl_Valuator.H
#endi

// values for type()
#define FL_NORMAL_DIAL	
#define FL_LINE_DIAL	
#define FL_FILL_DIAL	

/** The Fl_Dial widget provides a circular dial to control a singl
 * floating point value.
 *   \image html dial.gi
 * type(uchar) supports the following dials
 * \arg \c FL_NORMAL_DIAL - Draws a normal dial with a knob
 * \arg \c FL_LINE_DIAL - Draws a dial with a line
 * \arg \c FL_FILL_DIAL - Draws a dial with a filled arc
 *
class FL_EXPORT Fl_Dial : public Fl_Valuator 

  short a1,a2

protected

  // these allow subclasses to put the dial in a smaller area
  void draw(int, int, int, int)
  int handle(int, int, int, int, int)
  void draw()

public

  int handle(int)
    /** Creates a new Fl_Dial widget using the given position, size, an
     * label string. The default type is \c FL_NORMAL_DIAL. *
  Fl_Dial(int x,int y,int w,int h, const char *l = 0)
  short angle1() const {return a1;
  void angle1(short a) {a1 = a;
  short angle2() const {return a2;
  void angle2(short a) {a2 = a;
    /** Sets the angles used for the minimum and maximum values. The defaul
     * values are 45 and 315 (0 degrees is straight down and the angle
     * progress clockwise). Normally angle1 is less than angle2, but i
     * you reverse them the dial moves counter-clockwise. *
  void angles(short a, short b) {a1 = a; a2 = b;

}

#endi

/
// End of "$Id$"
/
