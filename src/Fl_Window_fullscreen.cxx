//
// "$Id: Fl_Window_fullscreen.cxx,v 1.15 2002/12/09 04:52:27 spitzak Exp $"
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

#include <fltk/Window.h>
#include <fltk/ScreenInfo.h>
#include <fltk/events.h>
#include <fltk/x.h>
using namespace fltk;

// Neither the X or Win32 version will successfully hide the taskbar.
// I would like it, but maybe that should be a third state of the window.
// On both systems it looks like it is much harder to do it.

void Window::fullscreen() {
  const ScreenInfo& info = screenInfo();
#ifndef _WIN32
  // Most X window managers will not place the window where we want it unless
  // the border is turned off. And most (all except Irix 4DWM, as far as I
  // can tell) will ignore attempts to change the border unless the window
  // is unmapped. Telling the window manager about the border changing
  // is done by i->sendxjunk(). Order is somewhat peculiar, we need to
  // have the window mapped before calling resize or some window managers
  // (KDE) ignore the positioning information:
  clear_border();
  if (shown()) i->sendxjunk();
#endif
  resize(info.x, info.y, info.w, info.h);
}

void Window::fullscreen_off(int X,int Y,int W,int H) {
#ifndef _WIN32
  clear_flag(Window::NOBORDER);
  if (shown()) i->sendxjunk();
#endif
  resize(X, Y, W, H);
}

//
// End of "$Id: Fl_Window_fullscreen.cxx,v 1.15 2002/12/09 04:52:27 spitzak Exp $".
//
