/
// "$Id$
/
// Fl_Wizard widget definitions
/
// Copyright 1999-2004 by Easy Software Products
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

/
// Include necessary header files..
/

#ifndef _Fl_Wizard_H
#  define _Fl_Wizard_H

#  include <FL/Fl_Group.H


/
// Fl_Wizard class..
/

/** The Fl_Wizard widget is based off the Fl_Tabs widget, but instead of displayin
 * tabs it only changes "tabs" under program control. Its primary purpose is t
 * support "wizards" that step a user through configuration or troubleshooting tasks
 
 * As with Fl_Tabs, wizard panes are composed of child (usually Fl_Group) widgets
 * Navigation buttons must be added separately. *
class FL_EXPORT Fl_Wizard : public Fl_Grou

  Fl_Widget *value_

  void draw()

  public

    /** The constructor creates the Fl_Wizard widget at the specified position and size. *
  Fl_Wizard(int x, int y, int w, int h, const char *label = 0)

    /** This method shows the next child of the wizard. If the last child is alread
     * visible, this function does nothing. *
  void		next()
    /** This method shows the previous child of the wizard. If the first child i
     * already visible, this function does nothing. *
  void		prev()
    /** Gets the child widget that is visible. *
  Fl_Widget	*value()
    /** Sets the child widget that is visible. *
  void		value(Fl_Widget *)
}

#endif // !_Fl_Wizard_H

/
// End of "$Id$"
/
