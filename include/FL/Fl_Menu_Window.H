//
// "$Id$"
//
// Menu window header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#ifndef Fl_Menu_Window_H
#define Fl_Menu_Window_H

#include <fltk3/MenuWindow.h>
#include "Fl_Single_Window.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Menu_Window, MenuWindow)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Menu_Window, MenuWindow)
FLTK3_WRAPPER_INTERFACE_WINDOW(Fl_Menu_Window, MenuWindow)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Menu_Window : public Fl_Single_Window {
  
public:
  
  FLTK3_WINDOW_VCALLS(Fl_Menu_Window, MenuWindow)
  
  void erase() {
    ((fltk3::MenuWindow*)_p)->erase();
  }
  
  void flush() {
    ((fltk3::MenuWindow*)_p)->flush();
  }
    
  unsigned int overlay() {
    return ((fltk3::MenuWindow*)_p)->overlay();
  }

  void set_overlay() {
    ((fltk3::MenuWindow*)_p)->set_overlay();
  }

  void clear_overlay() {
    ((fltk3::MenuWindow*)_p)->clear_overlay();
  }

  Fl_Menu_Window() { /* empty */ }
  
  Fl_Menu_Window(int W, int H, const char *l = 0) {
    _p = new fltk3::MenuWindow_I(W, H, l);
    _p->wrapper(this);
  }
  
  Fl_Menu_Window(int X, int Y, int W, int H, const char *l = 0) {
    _p = new fltk3::MenuWindow_I(X, Y, W, H, l);
    _p->wrapper(this);
  }
};

#endif

//
// End of "$Id$".
//
