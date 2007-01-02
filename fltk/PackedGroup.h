//
// "$Id$"
//
// Group that places all it's child widgets packed against the
// edges. The edge is decided by the PACK_VERTICAL flag stored on
// the child and by whether the child is before or after the
// resizable() child.
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

#ifndef fltk_PackedGroup_h
#define fltk_PackedGroup_h

#include "Group.h"

namespace fltk {

class FL_API PackedGroup : public Group {
  int spacing_;
public:
  enum { // values for type(int), for back-compatability
    NORMAL	= GROUP_TYPE,
    ALL_CHILDREN_VERTICAL = GROUP_TYPE+1
  };
  void layout();
  PackedGroup(int x, int y, int w, int h, const char *l = 0, bool begin=false);
  int spacing() const {return spacing_;}
  void spacing(int i) {spacing_ = i;}
};

}

#endif

//
// End of "$Id$".
//
