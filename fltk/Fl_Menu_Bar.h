//
// "$Id: Fl_Menu_Bar.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Menu bar header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#ifndef Fl_Menu_Bar_H
#define Fl_Menu_Bar_H

#include "Fl_Menu_.h"

/**

   This widget provides a standard menubar interface. Usually you will put 
   this widget along the top edge of your window. The height of the widget
   should be 30 for the menu titles to draw correctly with the default font. 
   Each child widget (typically an Fl_Item_Group puts a single item into 
   the menu bar. If the child is a group, the pressing on it will bring up
   a pull-down menu of the children of it. Sub-sub menus and lower pop up 
   to the right of the submenus.

   \image menubar.gif

   If a child is not a group (for instance if it is a plain Fl_Item) then
   clicking on it will pick it as though it was a menu item. This allows 
   you to put "buttons" into the menubar and thus the menubar can act like
   a "toolbar" in other toolkits, or you can mix the "tools" with "menus" 
   into the same bar.

   When the user picks an item off the menu, the item's callback is done
   with the menubar as the Fl_Widget* argument. If the item does not have
   a callback the menubar's callback is done instead. 

   Submenus will also pop up in response to shortcuts indicated by putting
   a '&' character in the name field of the menu item. If you put 
   a '&' character in a top-level "button" then the shortcut picks it. 
   The '&' character in submenus is ignored until the menu is popped up. 

   Typing the shortcut() of any of the menu items will cause callbacks 
   exactly the same as when you pick the item with the mouse. 

*/
class FL_API Fl_Menu_Bar : public Fl_Menu_ {
public:
  /**
   Creates a new Fl_Menu_Bar widget using the given position, size, and 
   label string. The default boxtype is FL_UP_BOX. 

   The constructor sets menu() to NULL. See Fl_Menu_ for the methods to 
   set or change the menu. 

   labelsize(), labelfont(), and labelcolor() are used to control how
   the menubar items are drawn. They are initialized from the Fl_Menu 
   static variables, but you can change them if desired. 

   label() is ignored unless you change align() to put it outside the menubar. 
  */
  Fl_Menu_Bar(int x,int y,int w,int h,const char *l=0);
  static Fl_Named_Style* default_style;
  int handle(int);
protected:
  void draw();
private:
  int highlight_, last_;
};

#endif

//
// End of "$Id: Fl_Menu_Bar.h,v 1.2 2002/10/29 00:37:23 easysw Exp $".
//
