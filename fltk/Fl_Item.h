//
// "$Id: Fl_Item.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $"
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

// values for type() (should be the same as the _BUTTON symbols)
#define FL_TOGGLE_ITEM	(FL_RESERVED_TYPE+1)
#define FL_RADIO_ITEM	(FL_RESERVED_TYPE+2)

class FL_API Fl_Item : public Fl_Widget {
public:
  void draw();
  void layout();
  Fl_Item(const char* label = 0);
  static Fl_Named_Style* default_style;
};

#endif
