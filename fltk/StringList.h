//
// "$Id: StringList.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// List that can be used in a Menu (and Browser) where each item
// displays one string from an array.  It is pretty much impossible to
// describe a menu in less space than this.
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#ifndef fltk_StringList_h
#define fltk_StringList_h

#include "Menu.h"

namespace fltk {

class FL_API StringList : public List {
  const char* const * array;
  int children_;
public:
  virtual int children(const Menu*, const int* indexes, int level);
  virtual Widget* child(const Menu*, const int* indexes, int level);
  StringList(const char*const* a, int n) : array(a), children_(n) {}
  StringList(const char*const* a); // null-terminated array
  StringList(const char* S); // nul-seperated string, double nul at end
};

}

#endif
