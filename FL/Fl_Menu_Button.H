//
// "$Id$"
//
// Menu button header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef Fl_Menu_Button_H
#define Fl_Menu_Button_H

#include <fltk3/MenuButton.h>
#include "Fl_Menu_.H"


class FL_EXPORT Fl_Menu_Button : public Fl_Menu_ {
  
protected:
  
  FLTK3_WRAPPER_VCALLS_OBJECT(MenuButton, draw(), draw(), Draw)
  
public:

  enum popup_buttons {
    POPUP1 = 1,
    POPUP2,
    POPUP12,
    POPUP3,
    POPUP13,
    POPUP23,
    POPUP123
  };
  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, MenuButton, handle(int e), handle(e), Handle)
  
  const Fl_Menu_Item* popup() {
    return (Fl_Menu_Item*) ((fltk3::MenuButton*)_p)->popup();
  }
  
  Fl_Menu_Button(int x, int y, int w, int h, const char *label=0) {
    _p = new fltk3::MenuButton(x, y, w, h, label);
    _p->wrapper(this);
  }
  
};

#endif

//
// End of "$Id$".
//
