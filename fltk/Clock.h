//
// "$Id: Clock.h 5197 2006-06-14 07:43:46Z spitzak $"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#ifndef fltk_Clock_h
#define fltk_Clock_h

#include "Widget.h"

namespace fltk {

// a ClockOutput can be used to display a program-supplied time:

class FL_API ClockOutput : public Widget {
public:
  enum {SQUARE = 0, ANALOG = 0, ROUND, DIGITAL};
  ClockOutput(int x,int y,int w,int h, const char *l = 0);
  void value(unsigned long v);	// set to this Unix time
  void value(int,int,int);	// set hour, minute, second
  unsigned long value() const {return value_;}
  int hour() const {return hour_;}
  int minute() const {return minute_;}
  int second() const {return second_;}
protected:
  void draw(int, int, int, int);
  void draw();
private:
  int hour_, minute_, second_;
  unsigned long value_;
  void drawhands(Color, Color); // part of draw
};

// a Clock displays the current time always by using a timeout:

class FL_API Clock : public ClockOutput {
public:
  int handle(int);
  void update();
  Clock(int x,int y,int w,int h, const char *l = 0);
  static NamedStyle* default_style;
};

}

#endif

//
// End of "$Id: Clock.h 5197 2006-06-14 07:43:46Z spitzak $".
//
