//
// "$Id: Fl_Menu_Button.h,v 1.5 2002/10/29 00:37:23 easysw Exp $"
//
// Menu button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu_Button_H
#define Fl_Menu_Button_H

#include "Fl_Menu_.h"

/**

   This is a button that when pushed pops up a menu (or hierarchy of menus)
   defined by an array of Fl_Menu_Item objects. 

   \image menu_button.gif

   Normally any mouse button will pop up a menu and it is lined up below 
   the button as shown in the picture. However an Fl_Menu_Button may also
   control a pop-up menu. This is done by setting the type() , see below. 

   The menu will also pop up in response to shortcuts indicated by putting 
   a '&' character in the label(). 

   Typing the shortcut() of any of the menu items will cause callbacks 
   exactly the same as when you pick the item with the mouse. 
   The '&' character in menu item names are only looked at when the menu is
   popped up, however. 

   When the user picks an item off the menu, the item's callback is done 
   with the menu_button as the Fl_Widget* argument. If the item does not 
   have a callback the menu_button's callback is done instead. 

*/
class FL_API Fl_Menu_Button : public Fl_Menu_ {
public:
  // values for type:
   /**
   If type() is zero a normal menu button is produced.
   If it is nonzero then this is a pop-up menu. The bits 
   in type() indicate what mouse buttons pop up the menu. 
   For convienece the constants 
   Fl_Menu_Button::POPUP1, POPUP2, POPUP3, POPUP12, POPUP13, POPUP23, and POPUP123
   are defined. Fl_Menu_Button::POPUP3 is usually what you want. 
   A popup menu button is invisible and does not interfere with any events 
   other than the mouse button specified (and any shortcuts). The widget can
   be stretched to cover all your other widgets by putting it last in the 
   hierarchy so it is "on top". You can also make several widgets covering 
   different areas for context-sensitive popup menus. 

   The popup menus appear with the cursor pointing at the previously 
   selected item. This is a feature. If you don't like it, do value(0) 
   after the menu items are picked to forget the current item. 

   */
  enum {NORMAL = GROUP_TYPE, POPUP1, POPUP2, POPUP12, POPUP3, POPUP13, POPUP23, POPUP123};
  int handle(int);

  /**
   Act exactly as though the user clicked the button or typed the shortcut key.
   The menu appears, it waits for the user to pick an item, and if they pick 
   one it sets value() and does the callback or sets changed() as described 
   above. The menu item is returned or NULL if the user dismisses the menu. 
  */
  int popup();
  /**
   Creates a new Fl_Menu_Button widget using the given position, size, and 
   label string. The default boxtype is FL_UP_BOX. 

   The constructor sets menu() to NULL. See Fl_Menu_ for the methods to
   set or change the menu.
  */
  Fl_Menu_Button(int x,int y,int w,int h,const char *label =0);
  static Fl_Named_Style* default_style;

protected:
  void draw();

};

#endif

//
// End of "$Id: Fl_Menu_Button.h,v 1.5 2002/10/29 00:37:23 easysw Exp $".
//
