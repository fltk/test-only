//
// "$Id: Fl_Value_Output.h,v 1.2 2002/06/21 06:17:09 spitzak Exp $"
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

#ifndef Fl_Value_Output_H
#define Fl_Value_Output_H

#include <fltk/Fl_Value_Input.h>

class FL_API Fl_Value_Output : public Fl_Value_Input {
public:
  Fl_Value_Output(int x,int y,int w,int h,const char *l = 0)
    : Fl_Value_Input(x, y, w, h, l) {}
  void soft(uchar x) {} // unimplemented, it acts like true allways
  bool soft() const {return true;}
};

#endif

//
// End of "$Id: Fl_Value_Output.h,v 1.2 2002/06/21 06:17:09 spitzak Exp $".
//
