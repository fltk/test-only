//
// "$Id: fl_sysinfo_win32.cxx,v 1.3 2001/03/12 00:49:03 spitzak Exp $"
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
#include <FL/Fl_Style.H>

#include <windows.h>
#include <limits.h>


#ifndef ENUM_CURRENT_SETTINGS
#define ENUM_CURRENT_SETTINGS ((DWORD)-1)
#endif


void
fl_sysinfo::update() {
  // grab screen info from Windows
  DEVMODE mode;
  EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &mode);
  fl_sysinfo::screen_width = mode.dmPelsWidth;
  fl_sysinfo::screen_height = mode.dmPelsHeight;
  fl_sysinfo::screen_depth = mode.dmBitsPerPel;

  // get display dimension info
  HDC gc = CreateDC("DISPLAY", NULL, NULL, NULL);
  if(gc) {
	fl_sysinfo::screen_width_mm = GetDeviceCaps(gc, HORZSIZE);
	fl_sysinfo::screen_height_mm = GetDeviceCaps(gc, VERTSIZE);
	fl_sysinfo::screen_dpi_x = GetDeviceCaps(gc, LOGPIXELSX);
	fl_sysinfo::screen_dpi_y = GetDeviceCaps(gc, LOGPIXELSY);
    DeleteDC(gc);
  } else {
	fl_sysinfo::screen_dpi_x = 75;
	fl_sysinfo::screen_dpi_y = 75;
	fl_sysinfo::screen_width_mm = fl_sysinfo::screen_width / 75;
	fl_sysinfo::screen_height_mm = fl_sysinfo::screen_height / 75;
  }

}

//
// End of "$Id: fl_sysinfo_win32.cxx,v 1.3 2001/03/12 00:49:03 spitzak Exp $".
//
