//
// "$Id$"
//
// Adjuster widget header file for the Fast Light Tool Kit (FLTK).
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

// 3-button "slider", made for Nuke

#ifndef Fl_Adjuster_H
#define Fl_Adjuster_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif

class Fl_Widget_Style;

/**
 * \brief A widget to quickly and precisely change numeric values using only the mouse.
 *
 * The Fl_Adjuster widget was inspired from Prisms, and has proven to be very useful for
 * values that need a large dynamic range.
 *
 *   \image html adjuster1.gif
 * 
 * When you press a button and drag to the right the value increases. When you drag to 
 * the left it decreases. The largest button adjusts by 100 * step(), the next by 
 * 10 * step() and that smallest button by step(). Clicking on the buttons increments 
 * by 10 times the amount dragging by a pixel does. Shift + click decrements by 10 times
 * the amount. */
class FL_EXPORT Fl_Adjuster : public Fl_Valuator {
  int drag;
  int ix;
  int soft_;
  uchar highlight__; // indicates that particular button should be highlighted

protected:
  void draw();
  int handle(int);
  void value_damage();
public:
  static Style * default_style();
    /** Creates a new Fl_Adjuster widget using the given position, size, and label string. 
     * It looks best if one of the dimensions is 3 times the other. */
  Fl_Adjuster(int X,int Y,int W,int H,const char *label=0);
    /** If "soft" is turned on, the user is allowed to drag the value outside the range. 
     * If they drag the value to one of the ends, let go, then grab again and continue to 
     * drag, they can get to any value. Default is one. */
  void soft(int s) {soft_ = s;}
  int soft() const {return soft_;}
};

#endif

//
// End of "$Id$".
//
