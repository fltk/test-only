//
// "$Id: Shortcut_Button.h,v 1.7 2002/12/10 01:46:15 easysw Exp $"
//
// Shortcut header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Shortcut_Button_H
#define Shortcut_Button_H

#include <fltk/Button.h>

class Shortcut_Button : public fltk::Button {
public:
  int svalue;
  int handle(int);
  void draw();
  Shortcut_Button(int x, int y, int w, int h, const char* l = 0) :
    fltk::Button(x,y,w,h,l) {svalue = 0;}
};

//
// End of "$Id: Shortcut_Button.h,v 1.7 2002/12/10 01:46:15 easysw Exp $".
//
#endif
