/
// "$Id$
/
// Overlay window header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Overlay_Window_
#define Fl_Overlay_Window_

#include "Fl_Double_Window.H

/** This window provides double buffering and also the ability to draw th
 * "overlay" which is another picture placed on top of the main image. Th
 * overlay is designed to be a rapidly-changing but simple graphic such a
 * a mouse selection box. Fl_Overlay_Window uses the overlay planes provide
 * by your graphics hardware if they are available
 
 * If no hardware support is found the overlay is simulated by drawin
 * directly into the on-screen copy of the double-buffered window, an
 * "erased" by copying the backbuffer over it again. This means the overla
 * will blink if you change the image in the window. *
class FL_EXPORT Fl_Overlay_Window : public Fl_Double_Window 
  friend class _Fl_Overlay
    /** You must subclass Fl_Overlay_Window and provide this method. It i
     * just like a draw() method, except it draws the overlay. The overla
     * will have already been "cleared" when this is called. You can us
     * any of the routines described in<FL/fl_draw.H>. *
  virtual void draw_overlay() = 0
  Fl_Window *overlay_
public
  void show()
  void flush()
  void hide()
  void resize(int,int,int,int)
  ~Fl_Overlay_Window()
  int can_do_overlay()
    /** Call this to indicate that the overlay data has changed and needs t
     * be redrawn. The overlay will be clear until the first time this i
     * called, so if you want an initial display you must call this afte
     * calling show(). *
  void redraw_overlay()
  Fl_Overlay_Window(int W, int H, const char *l=0
    : Fl_Double_Window(W,H,l) {overlay_ = 0; force_doublebuffering_=1; image(0); 
    /** Creates a new Fl_Overlay_Window widget using the given position
     * size, and label (title) string. *
  Fl_Overlay_Window(int X, int Y, int W, int H, const char *l=0
    : Fl_Double_Window(X,Y,W,H,l) {overlay_ = 0; force_doublebuffering_=1; image(0); 
  void show(int a, char **b) {Fl_Double_Window::show(a,b);
}

#endi

/
// End of "$Id$"
/
