//
// "$Id: KDE1.cxx,v 1.1 1999/11/08 22:22:03 carl Exp $"
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

// KDE theme

#include "../src/fl_kde1.cxx"

extern "C" fltk_theme(int, char**);
int fltk_theme(int, char**) {
  int r = fl_kde1();

  Fl::use_themes = 1; // turn themes back on, but not schemes

  return r;
}

//
// End of "$Id: KDE1.cxx,v 1.1 1999/11/08 22:22:03 carl Exp $".
//
