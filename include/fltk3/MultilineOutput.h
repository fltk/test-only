//
// "$Id$"
//
// Multi line output header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::MultilineOutput widget . */

#ifndef Fltk3_Multiline_Output_H
#define Fltk3_Multiline_Output_H

#include "Output.h"


namespace fltk3 {
  
  /**
   This widget is a subclass of fltk3::Output that displays multiple
   lines of text. It also displays tab characters as whitespace to the
   next column.
   
   Note that this widget does not support scrollbars, or per-character
   color control.
   
   If you are presenting large amounts of read-only text 
   and need scrollbars, or full color control of characters,
   then use fltk3::TextDisplay. If you want to display HTML text,
   use fltk3::HelpView.
   */
  class FLTK3_EXPORT MultilineOutput : public fltk3::Output {
  public:
    /**
     Creates a new fltk3::MultilineOutput widget using the given
     position, size, and label string. The default boxtype is fltk3::DOWN_BOX
     
     Inherited destructor destroys the widget and any value associated with it.
     */
#if defined(FL_DLL)	// implementation in src/Fl_Input.cxx
    MultilineOutput(int X,int Y,int W,int H,const char *l = 0);
#else
    MultilineOutput(int X,int Y,int W,int H,const char *l = 0)
    : fltk3::Output(X,Y,W,H,l) {type(fltk3::MULTILINE_OUTPUT);}
#endif
  };
  
}

#endif

//
// End of "$Id$".
//
