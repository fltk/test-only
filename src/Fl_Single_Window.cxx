/
// "$Id$
/
// Single-buffered window for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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

//	A window with a single-buffered contex
/
//	This is provided for systems where the base class is doubl
//	buffered.  You can turn it off using this subclass in cas
//	your display looks better without it

#include <FL/Fl_Single_Window.H

void Fl_Single_Window::show() {Fl_Window::show();
void Fl_Single_Window::flush() {Fl_Window::flush();

/
// End of "$Id$"
/
