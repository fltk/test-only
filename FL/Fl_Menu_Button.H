//
// "$Id: Fl_Menu_Button.H,v 1.5.2.3.2.2.2.2 2003/11/22 01:04:39 matthiaswm Exp $"
//
// Menu button header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef Fl_Menu_Button_H
#define Fl_Menu_Button_H

#include "Fl_Menu_.H"

/** This is a button that when pushed pops up a menu (or hierarchy of menus)
 * defined by an array of Fl_Menu_Item objects.
 *   \image html menu_button.gif
 * Normally any mouse button will pop up a menu and it is lined up below the
 * button as shown in the picture. However an Fl_Menu_Button may also
 * control a pop-up menu. This is done by setting the type() , see below.
 *
 * The menu will also pop up in response to shortcuts indicated by putting
 * a '&' character in the label().
 *
 * Typing the shortcut() of any of the menu items will cause callbacks
 * exactly the same as when you pick the item with the mouse. The '&'
 * character in menu item names are only looked at when the menu is popped
 * up, however.
 *
 * When the user picks an item off the menu, the item's callback is done
 * with the menu_button as the Fl_Widget* argument. If the item does not
 * have a callback the menu_button's callback is done instead. 
 *
 * If type() is zero a normal menu button is produced. If it is nonzero 
 * then this is a pop-up menu. The bits in type() indicate what mouse buttons
 * pop up the menu. For convienece the constants \c Fl_Menu_Button::POPUP1, 
 * \c POPUP2, \c POPUP3, \c POPUP12, \c POPUP13, \c POPUP23, and \c POPUP123
 * are defined. \c Fl_Menu_Button::POPUP3 is usually what you want.
 *
 * A popup menu button is invisible and does not interfere with any events
 * other than the mouse button specified (and any shortcuts). The widget
 * can be stretched to cover all your other widgets by putting it last in
 * the hierarchy so it is "on top". You can also make several widgets
 * covering different areas for context-sensitive popup menus.
 *
 * The popup menus appear with the cursor pointing at the previously
 * selected item. This is a feature. If you don't like it, do value(0)
 * after the menu items are picked to forget the current item.
 */
class FL_EXPORT Fl_Menu_Button : public Fl_Menu_ {
protected:
  void draw();
public:
  // values for type:
  enum {POPUP1 = 1, POPUP2, POPUP12, POPUP3, POPUP13, POPUP23, POPUP123};
  int handle(int);
  const Fl_Menu_Item* popup();
    /** Creates a new Fl_Menu_Button widget using the given position, size,
     * and label string. The default boxtype is \c FL_UP_BOX.
     *
     * The constructor sets menu() to \c NULL. See Fl_Menu_ for the methods
     * to set or change the menu. */
  Fl_Menu_Button(int,int,int,int,const char * =0);
};

#endif

//
// End of "$Id: Fl_Menu_Button.H,v 1.5.2.3.2.2.2.2 2003/11/22 01:04:39 matthiaswm Exp $".
//
