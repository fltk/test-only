//
// "$Id: fl_sysinfo.cxx,v 1.2 2001/03/09 20:39:26 robertk Exp $"
//
// Code to get windowing system specific info for FLTK.
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#include <FL/Fl.H>

int fl_sysinfo::screen_width = 0;
int fl_sysinfo::screen_height = 0;
int fl_sysinfo::screen_depth = 0;
int fl_sysinfo::screen_width_mm = 0;
int fl_sysinfo::screen_height_mm = 0;
int fl_sysinfo::screen_dpi_x = 0;
int fl_sysinfo::screen_dpi_y = 0;

#ifdef WIN32
#include "fl_sysinfo_win32.cxx"
#else
#include "fl_sysinfo_x.cxx"
#endif

//
// End of "$Id: fl_sysinfo.cxx,v 1.2 2001/03/09 20:39:26 robertk Exp $".
//
