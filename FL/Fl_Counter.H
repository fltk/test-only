//
// "$Id: Fl_Counter.H,v 1.18 1999/11/10 18:05:54 carl Exp $"
//
// Counter header file for the Fast Light Tool Kit (FLTK).
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

// A numerical value with up/down step buttons.  From Forms.

#ifndef Fl_Counter_H
#define Fl_Counter_H

#include <FL/Fl_Valuator.H>

// values for type():
#define FL_NORMAL_COUNTER	0
#define FL_SIMPLE_COUNTER	1

class FL_API Fl_Counter : public Fl_Valuator {

public:
  int handle(int);
  Fl_Counter(int,int,int,int,const char * = 0);
  ~Fl_Counter();
  void lstep(double a) {lstep_ = a;}
  void step(double a,double b) {Fl_Valuator::step(a); lstep_ = b;}
  void step(double a) {Fl_Valuator::step(a);}

protected:
  void draw();

private:
  static Fl_Style default_style;
  double lstep_;
  uchar mouseobj, highlight, last;
  static void repeat_callback(void *);
  int calc_mouseobj();
  void increment_cb();

};

#endif

//
// End of "$Id: Fl_Counter.H,v 1.18 1999/11/10 18:05:54 carl Exp $".
//
