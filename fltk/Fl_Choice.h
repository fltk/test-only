//
// "$Id: Fl_Choice.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Choice header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Choice_H
#define Fl_Choice_H

#include "Fl_Menu_.h"

/**

   This is a button that when pushed pops up a menu (or hierarchy of menus) 
   defined by an array of child items. The appearance is designed to look 
   like a Windows "combo box" but it is somewhat different in that it does
   not contain a text editor, also the menu pops up with the current item 
   under the cursor, which is immensely easier to use once you get used
   to it. Motif calls this an OptionButton. 

   \image choice.gif

   The user can change the value by popping up the menu by clicking anywhere
   in he widget and moving the cursor to a different item, or by typing up and 
   down arrow keys to cycle amoung the items. Typing the shortcut() of any of 
   the items will also change the value to that item (The '&' character in 
   item names are only looked at when the menu is popped up, however).

   The menu will also pop up in response to shortcuts indicated by putting 
   a '&' character in the label() or to the shortcut() of the Fl_Choice 
   widget itself. The user can then use arrow keys or the mouse to change 
   the selected item. 

   When the user changes the value() the callback is done. 

*/
class FL_API Fl_Choice : public Fl_Menu_ {
public:
  int handle(int);
  
  /**
   Creates a new Fl_Choice widget using the given position, size, and label 
   string. The default boxtype is FL_UP_BOX. 

   The constructor sets menu() to NULL. See Fl_Menu_ for the methods to
   set or change the menu.
  */
  Fl_Choice(int x,int y,int w,int h,const char * label= 0);
  static Fl_Named_Style* default_style;
  /*@{*/
  /**
   The value is the index into the Fl_Menu array of the last item chosen by
   the user. It is zero initially. You can set it as an integer, or set it 
   with a pointer to a menu item. The set routines return non-zero if the
   new value is different than the old one. Changing it causes a redraw(). 
  */
  int value(int v);
  int value() const {return Fl_Menu_::value();}
  /*@}*/
protected:
  void draw();
};

#endif

//
// End of "$Id: Fl_Choice.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
