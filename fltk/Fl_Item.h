//
// "$Id: Fl_Item.h,v 1.2 2002/01/20 07:37:15 spitzak Exp $"
//
// Widget designed to be a menu or browser item.
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#ifndef Fl_Item_H
#define Fl_Item_H

#ifndef Fl_Widget_H
#include "Fl_Widget.h"
#endif

class FL_API Fl_Item : public Fl_Widget {
public:
  enum { // values for type(), should match Fl_Button
    NORMAL = 0,
    TOGGLE = RESERVED_TYPE+1,
    RADIO  = RESERVED_TYPE+2
  };
  void draw();
  void layout();
  Fl_Item(const char* label = 0);
  static Fl_Named_Style* default_style;
};

#endif
