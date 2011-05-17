//
// "$Id: WizardGroup.h 8306 2011-01-24 17:04:22Z matt $"
//
// Fl_Wizard widget definitions.
//
// Copyright 1999-2010 by Easy Software Products.
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
   Fl_Wizard widget . */

//
// Include necessary header files...
//

#ifndef _Fltk3_Wizard_H_
#  define _Fltk3_Wizard_H_

#  include <fltk3/Group.h>


/**
    This widget is based off the Fl_Tabs
    widget, but instead of displaying tabs it only changes "tabs" under
    program control. Its primary purpose is to support "wizards" that
    step a user through configuration or troubleshooting tasks.

    <P>As with Fl_Tabs, wizard panes are composed of child (usually
    fltk3::Group) widgets. Navigation buttons must be added separately.
*/
class FLTK3_EXPORT Fl_Wizard : public fltk3::Group {

  fltk3::Widget *value_;

  void draw();

  public:

  Fl_Wizard(int, int, int, int, const char * = 0);

  void		next();
  void		prev();
  fltk3::Widget	*value();
  void		value(fltk3::Widget *);
};

#endif // !_Fl_Wizard_H_

//
// End of "$Id: WizardGroup.h 8306 2011-01-24 17:04:22Z matt $".
//
