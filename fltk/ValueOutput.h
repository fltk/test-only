//
// "$Id: ValueOutput.h,v 1.2 2004/01/06 06:43:02 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// End of "$Id: ValueOutput.h,v 1.2 2004/01/06 06:43:02 spitzak Exp $".
//
