//
// "$Id: Fl_Clock.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Clock header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Clock_H
#define Fl_Clock_H

#include <fltk/Fl_Widget.h>

/**
   An Fl_Clock_Output can be used to display a program-supplied time.
*/
class FL_API Fl_Clock_Output : public Fl_Widget {
public:
  enum {SQUARE = 0, ANALOG = 0, ROUND, DIGITAL};
  Fl_Clock_Output(int x,int y,int w,int h, const char *l = 0);

  /*@{*/
  /**
   The first two forms of value set the displayed time to the given UNIX time 
   value or specific hours, minutes, and seconds. 

   The third form of value returns the displayed time in seconds since 
   the UNIX epoch (January 1, 1970). 
  */
  void value(ulong v);	// set to this Unix time
  void value(int,int,int);	// set hour, minute, second
  ulong value() const {return value_;}
  /*@}*/

  /** Returns the current hour (0 to 23). */
  int hour() const {return hour_;}
  /** Returns the current minute (0 to 59). */
  int minute() const {return minute_;}
  /** Returns the current second (0 to 60, 60 = leap second). */
  int second() const {return second_;}
protected:
  void draw(int, int, int, int);
  void draw();
private:
  int hour_, minute_, second_;
  ulong value_;
  void drawhands(Fl_Color,Fl_Color); // part of draw
};

// a Fl_Clock displays the current time always by using a timeout:
/**

  This widget provides a round analog clock display and is provided for 
  Forms compatibility. It installs a 1-second timeout callback using 
  Fl::add_timeout(). 

  \image clock.gif

*/
class FL_API Fl_Clock : public Fl_Clock_Output {
public:
  int handle(int);
  void update();
  /**
   Creates a new Fl_Clock widget using the given position, size, and 
   label string. The default boxtype is FL_NO_BOX. 
  */
  Fl_Clock(int x,int y,int w,int h, const char *l = 0);
  static Fl_Named_Style* default_style;
  /**
   The destructor also deletes all the children. This allows a whole tree 
   to be deleted at once, without having to keep a pointer to all the 
   children in the user code. A kludge has been done so the Fl_Clock and 
   all of it's children can be automatic (local) variables, but you must 
   declare the Fl_Clockfirst, so that it is destroyed last. 
  */
  ~Fl_Clock();
};

#ifndef FLTK_2
#define FL_SQUARE_CLOCK		Fl_Clock_Output::SQUARE
#define FL_ROUND_CLOCK		Fl_Clock_Output::ROUND
#define FL_ANALOG_CLOCK		Fl_Clock_Output::SQUARE
#define FL_DIGITAL_CLOCK	Fl_Clock_Oupput::DIGITAL
#endif

#endif

//
// End of "$Id: Fl_Clock.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
