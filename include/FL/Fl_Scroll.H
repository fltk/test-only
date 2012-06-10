//
// "$Id$"
//
// Scroll header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef Fl_Scroll_H
#define Fl_Scroll_H

#include <fltk3/ScrollGroup.h>
#include "Fl_Group.H"
#include "Fl_Scrollbar.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Scroll, ScrollGroup)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Scroll, ScrollGroup)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Scroll : public Fl_Group {

protected:

  void bbox(int &x, int &y, int &w, int &h) {
    ((fltk3::ScrollGroup_I*)_p)->bbox(x, y, w, h);
  }
  
public:

  FLTK3_WIDGET_VCALLS(Fl_Scroll, ScrollGroup)

  // FIXME: 123 - Fl_Scrollbar scrollbar;
  
  // FIXME: 123 - Fl_Scrollbar hscrollbar;

  Fl_Scroll() {}
  
  Fl_Scroll(int X,int Y,int W,int H,const char*l=0) {
    _p = new fltk3::ScrollGroup_I(_ctor_dx(X), _ctor_dy(Y), W, H, l);
    _p->wrapper(this);
  }

  enum { // values for type()
    HORIZONTAL = 1,
    VERTICAL = 2,
    BOTH = 3,
    ALWAYS_ON = 4,
    HORIZONTAL_ALWAYS = 5,
    VERTICAL_ALWAYS = 6,
    BOTH_ALWAYS = 7
  };

  int xposition() const {
    return ((fltk3::ScrollGroup*)_p)->xposition();
  }
  
  int yposition() const {
    return ((fltk3::ScrollGroup*)_p)->yposition();
  }
  
  void scroll_to(int x, int y) {
    ((fltk3::ScrollGroup*)_p)->scroll_to(x, y);
  }
  
  void clear() {
    ((fltk3::ScrollGroup*)_p)->clear();
  }
  
  int scrollbar_size() const {
    return ((fltk3::ScrollGroup*)_p)->scrollbar_size();
  }

  void scrollbar_size(int size) {
    ((fltk3::ScrollGroup*)_p)->scrollbar_size(size);
  }   
  
  Fl_Scrollbar *vertical_scrollbar() {
    fltk3::Scrollbar *s3 = &((fltk3::ScrollGroup*)_p)->scrollbar;
    if (!s3) 
      return 0L;
    Fl_Scrollbar *s1 = (Fl_Scrollbar*)s3->wrapper();
    if (!s1) {
      s1 = new Fl_Scrollbar();
      s1->_p = s3;
      s3->wrapper(s1);
    }
    return s1;
  }

  Fl_Scrollbar *horizontal_scrollbar() {
    fltk3::Scrollbar *s3 = &((fltk3::ScrollGroup*)_p)->hscrollbar;
    if (!s3) 
      return 0L;
    Fl_Scrollbar *s1 = (Fl_Scrollbar*)s3->wrapper();
    if (!s1) {
      s1 = new Fl_Scrollbar();
      s1->_p = s3;
      s3->wrapper(s1);
    }
    return s1;
  }

};


#endif

//
// End of "$Id$".
//
