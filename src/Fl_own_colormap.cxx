//
// "$Id: Fl_own_colormap.cxx,v 1.11 2004/01/19 21:38:41 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/visual.h>
#include <fltk/x.h>
#include <config.h>

#if !USE_X11

// There is probably something relevant to do on MSWindows 8-bit displays
// but I don't know what it is
void fltk::own_colormap() {}

#else

/*!
  Makes FLTK use its own colormap. This may make FLTK display
  better and will reduce conflicts with other programs that want lots
  of colors. However the colors may flash as you move the cursor
  between windows.

  The new colormap has the first 16 entries copied from the default
  one, in an attempt to reduce colormap flashing when you change
  programs.

  This does nothing if the current visual is not colormapped, or
  if the system is not X based. This means this probably does nothing
  on any modern system.

  Use fltk::visual() to set the visual first.
*/
void fltk::own_colormap() {
  open_display();
#if USE_COLORMAP
  switch (xvisual->c_class) {
  case GrayScale :
  case PseudoColor :
  case DirectColor :
    break;
  default:
    return; // don't do anything for non-colormapped visuals
  }
  int i;
  XColor colors[16];
  // Get the first 16 colors from the default colormap...
  for (i = 0; i < 16; i ++) colors[i].pixel = i;
  XQueryColors(xdisplay, xcolormap, colors, 16);
  // Create a new colormap...
  xcolormap = XCreateColormap(xdisplay,
			      RootWindow(xdisplay,xscreen),
			      xvisual->visual, AllocNone);
  // Copy those first 16 colors to our own colormap:
  for (i = 0; i < 16; i ++)
    XAllocColor(xdisplay, xcolormap, colors + i);
#endif
}

#endif

//
// End of "$Id: Fl_own_colormap.cxx,v 1.11 2004/01/19 21:38:41 spitzak Exp $".
//
