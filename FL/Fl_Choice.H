//
// "$Id: Fl_Choice.H,v 1.20 2000/09/11 07:29:32 spitzak Exp $"
//
// Choice header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Choice_H
#define Fl_Choice_H

#include "Fl_Menu_.H"

class FL_API Fl_Choice : public Fl_Menu_ {
public:
  int handle(int);
  Fl_Choice(int,int,int,int,const char * = 0);
  int value(const int* indexes, int level);
  int value(int v);
  int value() const {return focus();}
protected:
  void draw();
};

#endif

//
// End of "$Id: Fl_Choice.H,v 1.20 2000/09/11 07:29:32 spitzak Exp $".
//
