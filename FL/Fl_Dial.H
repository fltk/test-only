//
// "$Id: Fl_Dial.H,v 1.15 2000/01/10 06:31:01 bill Exp $"
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
#include "Fl_Valuator.H"
#endif

// values for type():
#define FL_NORMAL_DIAL	0
#define FL_LINE_DIAL	1
#define FL_FILL_DIAL	2

class FL_API Fl_Dial : public Fl_Valuator {

public:

  int handle(int);
  Fl_Dial(int x,int y,int w,int h, const char *l = 0);
  short angle1() const {return a1;}
  void angle1(short a) {a1 = a;}
  short angle2() const {return a2;}
  void angle2(short a) {a2 = a;}
  void angles(short a, short b) {a1 = a; a2 = b;}

protected:

  void draw();

private:

  short a1,a2;

};

#endif

//
// End of "$Id: Fl_Dial.H,v 1.15 2000/01/10 06:31:01 bill Exp $".
//
