//
// "$Id: Fl_Clock.H,v 1.6.2.4.2.2.2.2 2003/11/10 01:32:58 matthiaswm Exp $"
//
// Clock header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef Fl_Clock_H
#define Fl_Clock_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

// values for type:
#define FL_SQUARE_CLOCK		0
#define FL_ROUND_CLOCK		1
#define FL_ANALOG_CLOCK FL_SQUARE_CLOCK
#define FL_DIGITAL_CLOCK FL_SQUARE_CLOCK // nyi

/** an Fl_Clock_Output can be used to display a program-supplied time.
 * \see Fl_Clock
 */
class FL_EXPORT Fl_Clock_Output : public Fl_Widget {
  int hour_, minute_, second_;
  ulong value_;
  void drawhands(Fl_Color,Fl_Color); // part of draw
protected:
  void draw(int, int, int, int);
  void draw();
public:
  Fl_Clock_Output(int x,int y,int w,int h, const char *l = 0);
    /** Set the displayed time to the given UNIX time value */
  void value(ulong v);
    /** Set the displayed time to the given hours, minutes, and seconds. */
  void value(int hours,int minutes,int seconds);
    /** Returns the displayed time in seconds since the UNIX epoch (January 1, 1970). */
  ulong value() const {return value_;}
    /** Returns the current hour (0 to 23). */
  int hour() const {return hour_;}
    /** Returns the current minute (0 to 59). */
  int minute() const {return minute_;}
    /** Returns the current second (0 to 60, 60 = leap second). */
  int second() const {return second_;}
};

// a Fl_Clock displays the current time always by using a timeout:

/** This widget provides a round analog clock display and is provided for Forms 
 * compatibility. It installs a 1-second timeout callback using Fl::add_timeout(). 
 *   \image html clock.gif
 *
 *   \image html round_clock.gif
 */
class FL_EXPORT Fl_Clock : public Fl_Clock_Output {
public:
  int handle(int);
  void update();
    /** Creates a new Fl_Clock widget using the given position, size, and label string. 
     * The default boxtype is \c FL_NO_BOX. */
  Fl_Clock(int x,int y,int w,int h, const char *label = 0);
  Fl_Clock(uchar boxtype,int x,int y,int w,int h, const char *label);
  ~Fl_Clock();
};

#endif

//
// End of "$Id: Fl_Clock.H,v 1.6.2.4.2.2.2.2 2003/11/10 01:32:58 matthiaswm Exp $".
//
