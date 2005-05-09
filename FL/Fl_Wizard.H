//
// "$Id$"
//
// Fl_Wizard widget definitions.
//
// Copyright 1999-2005 by Easy Software Products.
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

//
// Include necessary header files...
//

#ifndef _Fl_Wizard_H_
#  define _Fl_Wizard_H_

#  include <FL/Fl_Group.H>


//
// Fl_Wizard class...
//

/** The Fl_Wizard widget is based off the Fl_Tabs widget, but instead of displaying
 * tabs it only changes "tabs" under program control. Its primary purpose is to
 * support "wizards" that step a user through configuration or troubleshooting tasks.
 *
 * As with Fl_Tabs, wizard panes are composed of child (usually Fl_Group) widgets.
 * Navigation buttons must be added separately. */
class FL_EXPORT Fl_Wizard : public Fl_Group
{
  Fl_Widget *value_;

  void draw();

  public:

    /** The constructor creates the Fl_Wizard widget at the specified position and size. */
  Fl_Wizard(int x, int y, int w, int h, const char *label = 0);

    /** This method shows the next child of the wizard. If the last child is already
     * visible, this function does nothing. */
  void		next();
    /** This method shows the previous child of the wizard. If the first child is
     * already visible, this function does nothing. */
  void		prev();
    /** Gets the child widget that is visible. */
  Fl_Widget	*value();
    /** Sets the child widget that is visible. */
  void		value(Fl_Widget *);
};

#endif // !_Fl_Wizard_H_

//
// End of "$Id$".
//
