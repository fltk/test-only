//
// "$Id: Fl_Window_hotspot.cxx,v 1.22 2003/11/11 07:36:31 spitzak Exp $"
//
// Move windows but keep them on-screen.
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

#include <fltk/events.h>
#include <fltk/Window.h>
#include <fltk/Monitor.h>
#if defined(_WIN32)
# include <fltk/x.h>
#endif

using namespace fltk;

/*! position() the window so that the mouse is pointing at the center
  of the \a widget, which may be the window itself. If \a offscreen is
  true then the window is allowed to extend off the Monitor (some X
  window managers do not allow this). */
void Window::hotspot(const Widget *widget, bool offscreen) {
  int X = widget->w()/2;
  int Y = widget->h()/2;
  while (widget != this) {
    X += widget->x(); Y += widget->y();
    widget = widget->parent();
  }
  hotspot(X, Y, offscreen);
}

/*! position() the window so that the mouse is pointing at the \a
  cx,cy position. If \a offscreen is true then the window is allowed
  to extend off the Monitor (some X window managers do not allow
  this). */
void Window::hotspot(int cx, int cy, bool offscreen) {
  int X,Y; get_mouse(X,Y); X -= cx; Y -= cy;
  if (!offscreen) {
#ifdef _WIN32
    int dx, dy, dr, db;
    CreatedWindow::borders(this, dx, dy, dr, db); dr -= dx; db -= dy;
#else
    // We have to guess as to the thickness of the X window manager
    // borders. This can be determined by querying the server for the
    // parent window, but I don't bother as most window managers will
    // force the window onscreen anyway.
    const int dx = 1;
    const int dy = 20;
    const int dr = 1;
    const int db = 1;
#endif
    const Monitor& monitor = Monitor::find(X,Y);
    if (X+w()+dr > monitor.r()) X = monitor.r()-dr-w();
    if (X < monitor.x()+dx) X = monitor.x()+dx;
    if (X+w() > monitor.r()) X = monitor.r()-w();
    if (X < monitor.x()) X = monitor.x();
    if (Y+h()+db > monitor.h()) Y = monitor.h()-db-h();
    if (Y < monitor.y()+dy) Y = monitor.y()+dy;
    if (Y+h() > monitor.h()) Y = monitor.h()-h();
    if (Y < monitor.y()) Y = monitor.y();
  }
  position(X,Y);
}

//
// End of "$Id: Fl_Window_hotspot.cxx,v 1.22 2003/11/11 07:36:31 spitzak Exp $".
//
