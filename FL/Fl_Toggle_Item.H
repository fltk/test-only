//
// "$Id: Fl_Toggle_Item.H,v 1.1 2000/02/14 11:32:36 bill Exp $"
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

#ifndef Fl_Toggle_Item_H
#define Fl_Toggle_Item_H

#include "Fl_Item.H"

class FL_API Fl_Toggle_Item : public Fl_Item {
public:
  Fl_Toggle_Item(const char* l = 0) : Fl_Item(l) {type(FL_TOGGLE_ITEM);}
};

#endif
