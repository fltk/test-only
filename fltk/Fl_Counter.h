//
// "$Id: Fl_Counter.h,v 1.4 2002/07/15 05:55:37 spitzak Exp $"
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

// Back compatatility class, this is the same as Fl_Value_Input.

#ifndef Fl_Counter_H
#define Fl_Counter_H

#include <fltk/Fl_Value_Input.h>

class FL_API Fl_Counter : public Fl_Value_Input {
public:
  enum {NORMAL = 0, SIMPLE = 1};
  Fl_Counter(int x,int y,int w,int h,const char *l = 0)
    : Fl_Value_Input(x, y, w, h, l) {align(FL_ALIGN_BOTTOM);}
#ifndef FLTK_2
  float step() const {return Fl_Valuator::step();}
  void step(double a) {Fl_Valuator::step(a);}
  void lstep(double a) {linesize(a);}
  float lstep() const {return linesize();}
  void step(double a,double b) {step(a); lstep(b);}
#endif
};

#ifndef FLTK_2
#define FL_NORMAL_COUNTER	Fl_Counter::NORMAL
#define FL_SIMPLE_COUNTER	Fl_Counter::SIMPLE
#endif

#endif

//
// End of "$Id: Fl_Counter.h,v 1.4 2002/07/15 05:55:37 spitzak Exp $".
//
