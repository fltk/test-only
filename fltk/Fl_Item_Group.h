//
// "$Id: Fl_Item_Group.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Widget designed to be a nested list in a menu or browser.
// Code is in Fl_Item.cxx
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

#ifndef Fl_Item_Group_H
#define Fl_Item_Group_H

#include "Fl_Menu_.h"

/**

   This widget describes a set of items that are to be put inside a Fl_Menu or
   Fl_Browser widget. It indicates the title of a submenu, or a level of 
   hierarchy in the browser. Any child widgets are the items in that submenu, 
   or the items under this parent in the browser. 

   If this widget is told to draw, it draws just like Fl_Item draws. See that
   for more details. The triangle indicating a submenu is not drawn by this, 
   it is drawn by the parent menu. 

   Because this is an Fl_Menu_ subclass, you can also call popup() and add() 
   and other methods to manipulate the items inside it. 

   In a browser, the value() indicates if the widget is open or not. In a 
   multi-browser, the selected() indicates if the widget is currently 
   selected. 

*/
class FL_API Fl_Item_Group : public Fl_Menu_ {
public:
  void draw();
  void layout();
  /**
   Unlike other widgets the constructor does not take any dimensions, 
   since it is assummed the container widget will size this correctly. 
  */
  Fl_Item_Group(const char* label = 0);
};

#endif
