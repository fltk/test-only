//
// "$Id: Fl_Choice.H,v 1.5.2.3.2.2.2.5 2004/04/29 02:48:53 easysw Exp $"
//
// Choice header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Choice_H
#define Fl_Choice_H

#include "Fl_Menu_.H"

/** This is a button that when pushed pops up a menu (or hierarchy of
 * menus) defined by an array of Fl_Menu_Item objects. Motif calls this
 * an OptionButton.
 * 
 * The only difference between this and a Fl_Menu_Button is that the 
 * name of the most recent chosen menu item is displayed inside the box,
 * while the label is displayed outside the box. However, since the use
 * of this is most often to control a single variable rather than do 
 * individual callbacks, some of the Fl_Menu_Button methods are 
 * redescribed here in those terms.
 *
 * When the user picks an item off the menu the value() is set to that
 * item and then the callback is done.
 *
 * All three mouse buttons pop up the menu. The Forms behavior of the 
 * first two buttons to increment/decrement the choice is not
 * implemented. This could be added with a subclass, however.
 * 
 * The menu will also pop up in response to shortcuts indicated by 
 * putting a '&' character in the label(). See Fl_Button for a 
 * description of this.
 *
 * Typing the shortcut() of any of the items will do exactly the same as
 * when you pick the item with the mouse. The '&' character in item
 * names are only looked at when the menu is popped up, however.
 *   \image html choice.gif
 */
class FL_EXPORT Fl_Choice : public Fl_Menu_ {
protected:
  void draw();
public:
  int handle(int event);
    /** Creates a new Fl_Choice widget using the given position, size, 
    * and label string. The default boxtype is \c FL_UP_BOX.
    * 
    * The constructor sets menu() to \c NULL. See Fl_Menu_ for the
    * methods to set or change the menu. */
  Fl_Choice(int x,int y,int width,int height,const char *label = 0);
    /** The value is the index into the Fl_Menu array of the last item
     * chosen by the user. It is zero initially. You can set it as an
     * integer, or set it with a pointer to a menu item. The set 
     * routines return non-zero if the new value is different than the 
     * old one. Changing it causes a redraw(). */
  int value(int index);
    /** The value is the index into the Fl_Menu array of the last item
     * chosen by the user. It is zero initially. You can set it as an
     * integer, or set it with a pointer to a menu item. The set 
     * routines return non-zero if the new value is different than the 
     * old one. Changing it causes a redraw(). */
  int value(const Fl_Menu_Item *item);
    /** The value is the index into the Fl_Menu array of the last item
     * chosen by the user. */
  int value() const {return Fl_Menu_::value();}
};

#endif

//
// End of "$Id: Fl_Choice.H,v 1.5.2.3.2.2.2.5 2004/04/29 02:48:53 easysw Exp $".
//
