//
// "$Id: Fl_Item.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
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

/**

   This widget is designed to be put into Fl_Menu and Fl_Browser widgets.
   
   It's draw() method is designed to appear correctly inside menus and 
   browsers, despite inconsistencies in the designs of these things on 
   Windows. By default it does not erase it's background and assumes that 
   if FL_SELECTED is set in the flags it should use the 
   selected_label_color(). You can change this by setting box() or the 
   colors in the style, or by overriding the draw() method, but if you do
   so you will probably have to limit your widget to either menus or 
   browsers, but not both. 

   The browser draws the focus dotted box and the hierarchy indicators, 
   and the shortcut key assigments in menus is drawn by the menu code. 
   You cannot turn these off or modify them, unfortunately I was unable 
   to get these into the child widgets and still replicate the standard 
   Windows UI appearance. 

   Currently you can set type() to Fl_Item::TOGGLE to get a checkmark on 
   the left and Fl_Item::RADIO to make it a radio button. This may change
   in future versions however and you may need to use a subclass for this. 

   In a multi-browser, the selected() indicates if the widget is
   currently selected. 

*/
class FL_API Fl_Item : public Fl_Widget {
public:
  enum { // values for type(), should match Fl_Button
    NORMAL = 0,
    TOGGLE = RESERVED_TYPE+1,
    RADIO  = RESERVED_TYPE+2
  };
  void draw();
  void layout();

  /**
   Unlike other widgets the constructor does not take any dimensions, 
   since it is assummed the container widget will size this correctly. 
  */
  Fl_Item(const char* label = 0);
  static Fl_Named_Style* default_style;
};

#endif
