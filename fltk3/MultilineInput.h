//
// "$Id: MultilineInput.h 8068 2010-12-20 07:48:59Z greg.ercolano $"
//
// Multiline input header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::MultilineInput widget . */


#ifndef Fltk3_Multiline_Input_H
#define Fltk3_Multiline_Input_H

#include "Input.h"

namespace fltk3 {
  
  /**
   This input field displays '\n' characters as new lines rather than ^J,
   and accepts the Return, Tab, and up and down arrow keys.  This is for
   editing multiline text.
   
   This is far from the nirvana of text editors, and is probably only
   good for small bits of text, 10 lines at most. Note that this widget
   does not support scrollbars or per-character color control.
   
   If you are presenting large amounts of text and need scrollbars
   or full color control of characters, you probably want fltk3::TextEditor
   instead.
   
   In FLTK 1.3.x, the default behavior of the 'Tab' key was changed
   to support consistent focus navigation. To get the older FLTK 1.1.x
   behavior, set fltk3::Input_::tab_nav() to 0. Newer programs should consider using
   fltk3::TextEditor.
   */
  class FLTK3_EXPORT MultilineInput : public fltk3::Input {
  public:
    /**
     Creates a new fltk3::MultilineInput widget using the given
     position, size, and label string. The default boxtype is fltk3::DOWN_BOX.
     
     Inherited destructor destroys the widget and any value associated with it.
     */
#if defined(FL_DLL)	// implementation in src/Fl_Input.cxx
    MultilineInput(int X,int Y,int W,int H,const char *l = 0);
#else
    MultilineInput(int X,int Y,int W,int H,const char *l = 0)
    : fltk3::Input(X,Y,W,H,l) {type(fltk3::MULTILINE_INPUT);}
#endif
  };
  
}

#endif

//
// End of "$Id: MultilineInput.h 8068 2010-12-20 07:48:59Z greg.ercolano $".
//
