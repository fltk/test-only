//
// "$Id$"
//
// Choice header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
 fltk3::Choice widget . */

#ifndef Fltk3_Choice_H
#define Fltk3_Choice_H

#include "Menu_.h"
#include <fltk3/Wrapper.h>


namespace fltk3 {
  
  /**
   \class fltk3::Choice
   \brief A button that is used to pop up a menu.
   
   This is a button that, when pushed, pops up a menu (or hierarchy of menus)
   defined by an array of fltk3::MenuItem objects.
   Motif calls this an OptionButton.
   
   The only difference between this and a fltk3::MenuButton is that the name of
   the most recent chosen menu item is displayed inside the box, while the
   label is displayed outside the box. However, since the use of this is most
   often to control a single variable rather than do individual callbacks,
   some of the fltk3::MenuButton methods are redescribed here in those terms.
   
   When the user picks an item off the menu the value() is set to that item
   and then the item's callback is done with the menu_button as the
   \c fltk3::Widget* argument. If the item does not have a callback the
   menu_button's callback is done instead.
   
   All three mouse buttons pop up the menu. The Forms behavior of the first
   two buttons to increment/decrement the choice is not implemented.  This
   could be added with a subclass, however.
   
   The menu will also pop up in response to shortcuts indicated by putting
   a '\&' character in the label().  See fltk3::Button::shortcut(int s) for a
   description of this.
   
   Typing the shortcut() of any of the items will do exactly the same as when
   you pick the item with the mouse.  The '\&' character in item names are
   only looked at when the menu is popped up, however.
   
   \image html choice.png
   \image latex choice.png  "fltk3::Choice" width=4cm
   \todo Refactor the doxygen comments for fltk3::Choice changed() documentation.
   
   \li <tt>int fltk3::Widget::changed() const</tt>
   This value is true the user picks a different value. <em>It is turned
   off by value() and just before doing a callback (the callback can turn
   it back on if desired).</em>
   \li <tt>void fltk3::Widget::set_changed()</tt>
   This method sets the changed() flag.
   \li <tt>void fltk3::Widget::clear_changed()</tt>
   This method clears the changed() flag.
   \li <tt>fltk3::Boxtype fltk3::Choice::down_box() const</tt>
   Gets the current down box, which is used when the menu is popped up.
   The default down box type is \c fltk3::DOWN_BOX.
   \li <tt>void fltk3::Choice::down_box(fltk3::Boxtype b)</tt>
   Sets the current down box type to \p b.
   */
  class FLTK3_EXPORT Choice : public fltk3::Menu_ {

  protected:
    
    void draw();
    
  public:
    
    int handle(int);
    
    Choice(int X, int Y, int W, int H, const char *L = 0);
    
    /**
     Gets the index of the last item chosen by the user.
     The index is zero initially.
     */
    int value() const {return Menu_::value();}
    
    int value(int v);
    
    int value(const fltk3::MenuItem* v);
  };
  
} // namespace

#endif

//
// End of "$Id$".
//
