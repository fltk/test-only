//
// "$Id: fl_sysinfo_win32.cxx,v 1.1 2001/03/08 07:39:06 clip Exp $"
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


#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES   104
#endif

#ifndef WHEEL_PAGESCROLL
#define WHEEL_PAGESCROLL        (UINT_MAX) /* Scroll one page */
#endif

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

  // grab mousewheel stuff from Windows
  UINT delta;
  SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, (PVOID)&delta, 0);
  if (delta == WHEEL_PAGESCROLL) Fl_Style::mousewheel_delta = INT_MAX;
  else Fl_Style::mousewheel_delta = (int)delta;
}

//
// End of "$Id: fl_sysinfo_win32.cxx,v 1.1 2001/03/08 07:39:06 clip Exp $".
//
