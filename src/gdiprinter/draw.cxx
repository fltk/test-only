/
// "$Id$
/
// Text drawings for WIN32 GDI printing device for the Fast Light Tool Kit (FLTK)
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
// Please report all bugs and problems to http://www.fltk.org/str.ph
/

#include <FL/Fl_GDI_Printer.H
#include "../win/Fl_Win_Display.H
extern Fl_Win_Display fl_disp

void Fl_GDI_Printer::draw(const char* str, int n, int x, int y)
  fl_disp.draw(str, n, x, y)
}

/
// End of "$Id$
/
