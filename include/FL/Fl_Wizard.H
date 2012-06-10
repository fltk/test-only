//
// "$Id$"
//
// Fl_Wizard widget definitions.
//
// Copyright 1999-2012 by Easy Software Products.
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

#ifndef _Fl_Wizard_H_
#define _Fl_Wizard_H_

#include <fltk3/WizardGroup.h>
#include <FL/Fl_Group.H>


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Wizard, WizardGroup)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Wizard, WizardGroup)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Wizard : public Fl_Group {

public:

  FLTK3_WIDGET_VCALLS(Fl_Wizard, WizardGroup)

  Fl_Wizard(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::WizardGroup_I(_ctor_dx(x), _ctor_dy(y), w, h, label);
    _p->wrapper(this);
  }

  void next() {
    ((fltk3::WizardGroup*)_p)->next();
  }
  
  void prev() {
    ((fltk3::WizardGroup*)_p)->prev();
  }
  
  Fl_Widget *value() {
    return fltk3::_3to1_widget( ((fltk3::WizardGroup*)_p)->value() );
  }
  
  void value(Fl_Widget *w) {
    ((fltk3::WizardGroup*)_p)->value( fltk3::_1to3_widget(w) );
  }
  
};


#endif // !_Fl_Wizard_H_

//
// End of "$Id$".
//
