//
// "$Id: Fl_Window_hotspot.cxx,v 1.8 1999/08/16 07:31:23 bill Exp $"
//
// Common hotspot routines for the Fast Light Tool Kit (FLTK).
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

void Fl_Window::hotspot(int X, int Y, int /*offscreen*/) {
  int mx,my; Fl::get_mouse(mx,my);
  X = mx-X; Y = my-Y;
  position(X,Y);
  layout();
}

void Fl_Window::hotspot(const Fl_Widget *o, int offscreen) {
  int X = o->w()/2;
  int Y = o->h()/2;
  while (o != this) {
    X += o->x(); Y += o->y();
    o = o->window();
  }
  hotspot(X,Y,offscreen);
}

//
// End of "$Id: Fl_Window_hotspot.cxx,v 1.8 1999/08/16 07:31:23 bill Exp $".
//
