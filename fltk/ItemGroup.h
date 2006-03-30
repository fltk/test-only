//
// "$Id$"
//
// Widget designed to be a nested list in a menu or browser. This
// copies the drawing and style code from Item. I did not modify the
// base Menu class this way because the style inheritance would mess
// up the styles of MenuButton and MenuBar. Code is in Item.cxx
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#ifndef fltk_ItemGroup_h
#define fltk_ItemGroup_h

#include "Menu.h"

namespace fltk {

class FL_API ItemGroup : public Menu {
public:
  void draw();
  void layout();
  int handle(int);
  ItemGroup(const char* label = 0);
  ItemGroup(const char* label,int shortcut,Callback *callback=0,void *user_data_=0, int flags=0);

private:
  void init();
};

}

#endif
