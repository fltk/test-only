//
// "$Id: syscolors.cxx,v 1.1 2001/02/21 06:15:45 clip Exp $"
//
// Theme plugin file for FLTK
//
// Copyright 1999 Bill Spitzak and others.
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

// syscolors.cxx

// This theme is designed to be used with other themes to allow them to
// use the system colors.

#include <FL/Fl_Style.H>

extern "C"
int fltk_plugin() {
  fl_get_system_colors();

  return 0;
}

//
// End of "$Id: syscolors.cxx,v 1.1 2001/02/21 06:15:45 clip Exp $".
//
