//
// "$Id: Fl_Window_iconize.cxx,v 1.19 2003/11/11 07:36:31 spitzak Exp $"
//
// Window minification code for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Window.h>
#include <fltk/x.h>
using namespace fltk;

extern bool fl_show_iconic; // in x.C/win32.C

/*! Iconifies the window. If you call this when shown() is false it
  will show() it as an icon. If the window is already iconified this
  does nothing.

  Call show() to restore the window. 

  Currently there are only X and Win32 system-specific ways to control
  what is drawn in the icon. You should not rely on window managers
  displaying the icons. */
void Window::iconize() {
  if (!i) {
    fl_show_iconic = true;
    show();
  } else {
#ifdef _WIN32
    ShowWindow(i->xid, SW_SHOWMINNOACTIVE);
#elif (defined(__APPLE__) && !USE_X11)
    CollapseWindow(i->xid, true);
#else
    XIconifyWindow(xdisplay, i->xid, xscreen);
    i->wait_for_expose = true;
#endif
    clear_visible();
  }
}

//
// End of "$Id: Fl_Window_iconize.cxx,v 1.19 2003/11/11 07:36:31 spitzak Exp $".
//
