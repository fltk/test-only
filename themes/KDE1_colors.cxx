//
// "$Id: KDE1_colors.cxx,v 1.4 2000/03/03 02:45:54 carl Exp $"
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

// Just colors and fonts for KDE1 -- no boxtypes
// This is done by passing an argument to the KDE1 plugin that causes
// it to skip that stuff.

#include <FL/Fl.H>

extern "C" int fltk_theme(int, char**) {
// CET - FIXME - do this properly
  return Fl::theme("KDE1");
}

//
// End of "$Id: KDE1_colors.cxx,v 1.4 2000/03/03 02:45:54 carl Exp $".
//
