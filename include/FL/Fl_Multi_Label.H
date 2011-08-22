//
// "$Id$"
//
// Multi-label header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//  - struct is cloned, not wrapped
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef Fl_Multi_Label_H
#define Fl_Multi_Label_H

#include <fltk3/MultiLabel.h>
#include "Enumerations.H"

class Fl_Widget;
struct Fl_Menu_Item;

struct FL_EXPORT Fl_Multi_Label : public fltk3::MultiLabel {
  
  void label(Fl_Widget *w) {
    fltk3::MultiLabel::label( fltk3::_1to3_widget(w) );
  }
  
  void label(Fl_Menu_Item *w) {
    fltk3::MultiLabel::label( (fltk3::MenuItem*)w );
  }

};

#endif

//
// End of "$Id$".
//
