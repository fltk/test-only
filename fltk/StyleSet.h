//
// "$Id: StyleSet.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// Saves and restores all the styles used by Widget constructors. Fluid
// uses this so you can preview a style without fluid's own control
// panels changing. I think this is broken, anybody want to fix it?
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

#ifndef fltk_StyleSet_h
#define fltk_StyleSet_h

#include "Style.h"

namespace fltk {

class FL_API StyleSet {
  NamedStyle* first_style;
  Theme theme;
  const char* scheme;
  Color background;
  void* reserved; // pointer to extra saved stuff
public:
  StyleSet();
  void make_current();
  ~StyleSet();
};

}

#endif
