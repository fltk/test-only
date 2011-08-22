//
// "$Id$"
//
// Fl_Help_Dialog dialog for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper complete
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

#ifndef Fl_Help_Dialog_H
#define Fl_Help_Dialog_H

#include <fltk3/HelpDialog.h>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Help_View.H>

class FL_EXPORT Fl_Help_Dialog : public fltk3::HelpDialog {

public:

  Fl_Help_Dialog() 
  : fltk3::HelpDialog() 
  {
  }

  void textsize(Fl_Fontsize s) {
    fltk3::HelpDialog::textsize(fltk3::_1to3_fontsize(s));
  }

  Fl_Fontsize textsize() {
    return fltk3::_3to1_fontsize( fltk3::HelpDialog::textsize() );
  }
};
#endif

//
// End of "$Id$".
//
