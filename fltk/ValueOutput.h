//
// "$Id: ValueOutput.h,v 1.1 2002/12/15 10:42:49 spitzak Exp $"
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

// A ValueOutput is a valuator that displays the number like a text
// box. This is indended for showing the user a number, there is no
// way for the user to change the number. It is much lighter weight
// than using an Output widget for this.  If the user clicks on it
// they select the text.

#ifndef fltk_ValueOutput_h
#define fltk_ValueOutput_h

#include <fltk/Valuator.h>

namespace fltk {

class FL_API ValueOutput : public Valuator {
public:
  void draw();
  int handle(int);
  ValueOutput(int x,int y,int w,int h,const char *l = 0)
    : Valuator(x, y, w, h, l) {align(ALIGN_LEFT);}
};

}

#endif

//
// End of "$Id: ValueOutput.h,v 1.1 2002/12/15 10:42:49 spitzak Exp $".
//
