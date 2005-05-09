//
// "$Id$"
//
// Dial header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

#ifndef Fl_Dial_H
#define Fl_Dial_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif

// values for type():
#define FL_NORMAL_DIAL	0
#define FL_LINE_DIAL	1
#define FL_FILL_DIAL	2

/** The Fl_Dial widget provides a circular dial to control a single
 * floating point value. 
 *   \image html dial.gif
 * type(uchar) supports the following dials:
 * \arg \c FL_NORMAL_DIAL - Draws a normal dial with a knob.
 * \arg \c FL_LINE_DIAL - Draws a dial with a line.
 * \arg \c FL_FILL_DIAL - Draws a dial with a filled arc.
 */
class FL_EXPORT Fl_Dial : public Fl_Valuator {

  short a1,a2;

protected:

  // these allow subclasses to put the dial in a smaller area:
  void draw(int, int, int, int);
  int handle(int, int, int, int, int);
  void draw();

public:

  int handle(int);
    /** Creates a new Fl_Dial widget using the given position, size, and
     * label string. The default type is \c FL_NORMAL_DIAL. */
  Fl_Dial(int x,int y,int w,int h, const char *l = 0);
  short angle1() const {return a1;}
  void angle1(short a) {a1 = a;}
  short angle2() const {return a2;}
  void angle2(short a) {a2 = a;}
    /** Sets the angles used for the minimum and maximum values. The default
     * values are 45 and 315 (0 degrees is straight down and the angles
     * progress clockwise). Normally angle1 is less than angle2, but if
     * you reverse them the dial moves counter-clockwise. */
  void angles(short a, short b) {a1 = a; a2 = b;}

};

#endif

//
// End of "$Id$".
//
