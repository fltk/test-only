//
// "$Id: Fl_FileInput.H,v 1.1 2000/01/08 22:14:13 vincent Exp $"
//
// Fl_FileInput definitions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1997-1999 by Easy Software Products.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef _FL_FILEINPUT_H_
#  define _FL_FILEINPUT_H_

//
// Include necessary header files...
//

#  include <FL/Fl_Input.H>


//
// (world's shortest) FileInput class...
//

class FL_API Fl_FileInput : public Fl_Input	//// File input widget
{
  public:

  Fl_FileInput(int x, int y, int w, int h, const char *l = 0) :
      Fl_Input(x, y, w, h, l) {}
  int handle(int);
};

#endif // !_FL_FILEINPUT_H_

//
// End of "$Id: Fl_FileInput.H,v 1.1 2000/01/08 22:14:13 vincent Exp $".
//
