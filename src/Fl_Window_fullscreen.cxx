//
// "$Id: Fl_Window_fullscreen.cxx,v 1.13 2002/03/09 21:25:36 spitzak Exp $"
//
// Fullscreen window support for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/x.h>

// Neither the X or Win32 version will successfully hide the taskbar.
// I would like it, but maybe that should be a third state of the window.
// On both systems it looks like it is much harder to do it.

void Fl_Window::fullscreen() {
  const Fl_Screen_Info& info = Fl::info();
#ifndef _WIN32
  // Most X window managers will not place the window where we want it unless
  // the border is turned off. And most (all except Irix 4DWM, as far as I
  // can tell) will ignore attempts to change the border unless the window
  // is unmapped. Telling the window manager about the border changing
  // is done by i->sendxjunk().
  if (shown()) XUnmapWindow(fl_display,i->xid);
  clear_border();
#endif
  resize(info.x, info.y, info.w, info.h);
#ifndef _WIN32
  if (shown()) {
    layout();
    i->sendxjunk();
    XMapWindow(fl_display,i->xid);
  }
#endif
}

void Fl_Window::fullscreen_off(int X,int Y,int W,int H) {
#ifndef _WIN32
  if (shown()) XUnmapWindow(fl_display,i->xid);
  clear_flag(Fl_Window::FL_NOBORDER);
#endif
  resize(X, Y, W, H);
#ifndef _WIN32
  if (shown()) {
    layout();
    i->sendxjunk();
    XMapWindow(fl_display,i->xid);
  }
#endif
}

//
// End of "$Id: Fl_Window_fullscreen.cxx,v 1.13 2002/03/09 21:25:36 spitzak Exp $".
//
