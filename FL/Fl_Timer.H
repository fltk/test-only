/
// "$Id$
/
// Timer header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Timer_
#define Fl_Timer_

#ifndef Fl_Widget_
#include "Fl_Widget.H
#endi

// values for type()
#define FL_NORMAL_TIMER		
#define FL_VALUE_TIMER		
#define FL_HIDDEN_TIMER		

/** This is provided only to emulate the Forms Timer widget. It works b
 * making a timeout callback every 1/5 second. This is wasteful an
 * inaccurate if you just want something to happen a fixed time in th
 * future. You should directly call Fl::add_timeout() instead. *
class FL_EXPORT Fl_Timer : public Fl_Widget 
  static void stepcb(void *)
  void step()
  char on, direction_
  double delay, total
  long lastsec,lastusec
protected
  void draw()
public
  int handle(int)
    /** Creates a new Fl_Timer widget using the given type, position, size
     * and label string. The type parameter can be any of the followin
     * symbolic constants
     * \arg \c FL_NORMAL_TIMER - The timer just does the callback an
     *       displays the string "Timer" in the widget
     * \arg \c FL_VALUE_TIMER - The timer does the callback and displays th
     *      current timer value in the widget
     * \arg \c FL_HIDDEN_TIMER - The timer just does the callback and doe
     *      not display anything. *
  Fl_Timer(uchar t,int x,int y,int w,int h, const char *l)
    /** Destroys the timer and removes the timeout. *
  ~Fl_Timer()
    /** Sets the current timer value. *
  void value(double timer)
    /** Gets the current timer value. *
  double value() const {return delay>0.0?delay:0.0;
    /** Gets the direction of the timer. *
  char direction() const {return direction_;
    /** Sets the direction of the timer. If the direction is zero then th
     * timer will count up, otherwise it will count down from the initia
     * value(). *
  void direction(char d) {direction_ = d;
    /** Gets whether the timer is suspended. *
  char suspended() const {return !on;
    /** Sets whether the timer is suspended. *
  void suspended(char d)
}

#endi

/
// End of "$Id$"
/

