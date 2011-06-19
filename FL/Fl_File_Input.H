//
// "$Id$"
//
// File_Input header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//
// Copyright 1998-2011 by Bill Spitzak and others.
// Original version Copyright 1998 by Curtis Edwards.
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


#ifndef Fl_File_Input_H
#define Fl_File_Input_H

#include <fltk3/FileInput.h>
#include <FL/Fl_Input.H>


class FL_EXPORT Fl_File_Input : public Fl_Input {
  
protected:
  
  FLTK3_WRAPPER_VCALLS_OBJECT(FileInput, draw(), draw(), Draw)
  
public:

  Fl_File_Input() { /* empty */ }
  
  Fl_File_Input(int X, int Y, int W, int H, const char *L=0) {
    _p = new fltk3::FileInput(x, y, w, h, label);
    _p->wrapper(this);
  }

  FLTK3_WRAPPER_VCALLS_OBJECT_INT(FileInput, handle(int event), handle(event), Handle)

  Fl_Boxtype down_box() const { 
    return fltk3::_3to1_boxtype( ((fltk3::FileInput*)_p)->down_box() );
  }

  void down_box(Fl_Boxtype b) { 
    ((fltk3::FileInput*)_p)->down_box(fltk3::_1to3_boxtype(b));
  }

  Fl_Color errorcolor() const { 
    return fltk3::_3to1_color( ((fltk3::FileInput*)_p)->errorcolor() );
  }

  void errorcolor(Fl_Color c) { 
    ((fltk3::FileInput*)_p)->errorcolor( fltk3::_1to3_color(c) );
  }

  int value(const char *str) {
    return ((fltk3::FileInput*)_p)->value(str);
  }
  
  int value(const char *str, int len) {
    return ((fltk3::FileInput*)_p)->value(str, len);
  }

  const char *value() { 
    return ((fltk3::FileInput*)_p)->value();
  }
  
};


#endif // !Fl_File_Input_H


//
// End of "$Id$".
//
