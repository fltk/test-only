//
// "$Id$"
//
// Widget designed to be a menu or browser item.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fltk_Item_h
#define fltk_Item_h

#ifndef fltk_Widget_h
#include "Widget.h"
#endif

namespace fltk {

class FL_API Item : public Widget {
public:
  enum { // values for type(), should match Button
    NORMAL = 0,
    TOGGLE = RESERVED_TYPE+1,
    RADIO  = RESERVED_TYPE+2
  };
  void draw();
  void layout();
  int handle(int);
  Item(const char* label = 0);
  static NamedStyle* default_style;
  static void set_style(const Style*, bool menubar);
  static void set_style(const Widget* w, bool f) {set_style(w->style(),f);}
  static void clear_style() {set_style(Widget::default_style,false);}
};

}

#endif
