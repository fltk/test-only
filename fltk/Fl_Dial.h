//
// "$Id: Fl_Dial.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Dial header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_Dial_H
#define Fl_Dial_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.h"
#endif

/**

   The Fl_Dial widget provides a circular dial to control a 
   single floating point value. 

  \image Fl_Dial.gif

*/
class FL_API Fl_Dial : public Fl_Valuator {

public:
   /**
     Values for type():
     - NORMAL - Draws a normal dial with a knob. 
     - LINE   - Draws a dial with a line. 
     - FILL   - Draws a dial with a filled arc. 
   */
  enum {NORMAL = 0, LINE, FILL};
  int handle(int);
  /**
   Creates a new Fl_Dial widget using the given position, size, 
   and label string. The default type is FL_NORMAL_DIAL. 
  */
  Fl_Dial(int x,int y,int w,int h, const char *label = 0);
  static Fl_Named_Style* default_style;
  /*@{*/
  /**
   Sets the angles used for the minimum and maximum values. 
   The default values are 45 and 315 (0 degrees is straight 
   down and the angles progress clockwise). Normally angle1 is 
   less than angle2, but if you reverse them the dial moves
   counter-clockwise.
  */
  short angle1() const {return a1;}
  void angle1(short a) {a1 = a;}
  short angle2() const {return a2;}
  void angle2(short a) {a2 = a;}
  void angles(short a, short b) {a1 = a; a2 = b;}
  /*@}*/
protected:

  void draw();

private:

  short a1,a2;

};

#ifndef FLTK_2
#define FL_NORMAL_DIAL	Fl_Dial::NORMAL
#define FL_LINE_DIAL	Fl_Dial::LINE
#define FL_FILL_DIAL	Fl_Dial::FILL
#endif

#endif

//
// End of "$Id: Fl_Dial.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
