//
// "$Id: fl_sysinfo_x.cxx,v 1.3 2001/03/12 00:49:03 spitzak Exp $"
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
#include <FL/x.H>

#include <stdlib.h>
#include <limits.h>

void
fl_sysinfo::update() {
  // we need to open the display for this
  if (!fl_display) fl_open_display();

  // get screen width and height
  fl_sysinfo::screen_width = DisplayWidth(fl_display, fl_screen);
  fl_sysinfo::screen_height = DisplayHeight(fl_display, fl_screen);
  fl_sysinfo::screen_depth = fl_visual->depth;
  fl_sysinfo::screen_width_mm = DisplayWidthMM(fl_display, fl_screen);
  fl_sysinfo::screen_height_mm = DisplayHeightMM(fl_display, fl_screen);
  fl_sysinfo::screen_dpi_x = fl_sysinfo::screen_width_mm ? 
		(int)(((float)fl_sysinfo::screen_width / 
			   (float)fl_sysinfo::screen_width_mm) * 25.4)  : 75;
  fl_sysinfo::screen_dpi_y = fl_sysinfo::screen_height_mm ? 
		(int)(((float)fl_sysinfo::screen_height / 
				(float)fl_sysinfo::screen_height_mm) * 25.4)  : 75;

}

//
// End of "$Id: fl_sysinfo_x.cxx,v 1.3 2001/03/12 00:49:03 spitzak Exp $".
//
