//
// "$Id: Fl_Window_hotspot.cxx,v 1.10 1999/11/10 19:27:32 carl Exp $"
//
// Move windows but keep them on-screen.
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
#include <FL/Fl_Window.H>

#ifdef WIN32
#include <FL/win32.H>
#endif

void Fl_Window::move(int X, int Y) {
#ifdef WIN32
  int dx, dy, dr, db;
  Fl_X::borders(this, dx, dy, dr, db); dr -= dx; db -= dy;
#else
  // guess very thin borders for X (most X window managers will further
  // move the window to be on-screen)
  const int dx = 1;
  const int dy = 20;
  const int dr = 1;
  const int db = 1;
#endif
  int W = Fl::w();
  if (X+w()+dr > W) X = W-dr-w();
  if (X < dx) X = dx;
  if (X+w() > W) X = W-w();
  if (X < 0) X = 0;
  int H = Fl::h();
  if (Y+h()+db > H) Y = H-db-h();
  if (Y < dy) Y = dy;
  if (Y+h() > H) Y = H-h();
  if (Y < 0) Y = 0;
  position(X,Y);
}

void Fl_Window::hotspot(int X, int Y, int offscreen) {
  int mx,my; Fl::get_mouse(mx,my); mx -= X; my -= Y;
  if (offscreen) position(mx, my); else move(mx, my);
}

void Fl_Window::hotspot(const Fl_Widget *o, int offscreen) {
  int X = o->w()/2;
  int Y = o->h()/2;
  while (o != this) {
    X += o->x(); Y += o->y();
    o = o->window();
  }
  hotspot(X, Y, offscreen);
}

//
// End of "$Id: Fl_Window_hotspot.cxx,v 1.10 1999/11/10 19:27:32 carl Exp $".
//
