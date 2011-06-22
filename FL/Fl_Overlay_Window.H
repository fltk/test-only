//
// "$Id$"
//
// Overlay window header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef Fl_Overlay_Window_H
#define Fl_Overlay_Window_H

#include <fltk3/OverlayWindow.h>
#include "Fl_Double_Window.H"


class FL_EXPORT Fl_Overlay_Window : public Fl_Double_Window {

public:

  FLTK3_WRAPPER_VCALLS_OBJECT(OverlayWindow, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)
  
  FLTK3_WRAPPER_VCALLS_OBJECT(OverlayWindow, hide(), hide(), Hide)
  
  FLTK3_WRAPPER_VCALLS_OBJECT(OverlayWindow, show(), show(), Show)

  FLTK3_WRAPPER_VCALLS_OBJECT(OverlayWindow, draw_overlay(), draw_overlay(), DrawOverlay)
  
  void flush() {
    ((fltk3::OverlayWindow*)_p)->flush();
  }
  
  int can_do_overlay() {
    return ((fltk3::OverlayWindow*)_p)->can_do_overlay();
  }
  
  void redraw_overlay() {
    ((fltk3::OverlayWindow*)_p)->redraw_overlay();
  }

  Fl_Overlay_Window() {}
  
  Fl_Overlay_Window(int W, int H, const char *l=0) {
    _p = new fltk3::OverlayWindow(W, H, l);
    _p->wrapper(this);
  }
  
  Fl_Overlay_Window(int X, int Y, int W, int H, const char *l=0) {
    _p = new fltk3::OverlayWindow(X, Y, W, H, l);
    _p->wrapper(this);
  }
  
  void show(int a, char **b) {
    ((fltk3::OverlayWindow*)_p)->show(a, b);
  }
  
};

#endif

//
// End of "$Id$".
//
