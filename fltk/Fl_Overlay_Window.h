//
// "$Id: Fl_Overlay_Window.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Overlay window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Overlay_Window_H
#define Fl_Overlay_Window_H

#include "Fl_Double_Window.h"

/**

   This window provides double buffering and also the ability to draw the 
   "overlay" which is another picture placed on top of the main image. 
   The overlay is designed to be a rapidly-changing but simple graphic 
   such as a mouse selection box. Fl_Overlay_Window uses the overlay 
   planes provided by your graphics hardware if they are available. 

   If no hardware support is found the overlay is simulated by 
   drawing directly into the on-screen copy of the double-buffered
   window, and "erased" by copying the backbuffer over it again. 
   This means the overlay will blink if you change the image in
   the window. 

*/
class FL_API Fl_Overlay_Window : public Fl_Double_Window {

public:

  /**
    You must subclass Fl_Overlay_Window and provide this method. 
    It is just like a draw() method, except it draws the overlay.
    The overlay will have already been "cleared" when this is called.
    You can use any of the routines described in <fltk/fl_draw.h>. 
  */
  virtual void draw_overlay() = 0;
  virtual void layout();
  int can_do_overlay();
  /**
   Call this to indicate that the overlay data has changed and needs
   to be redrawn. The overlay will be clear until the first time 
   this is called, so if you want an initial display you must call
   this after calling show().
  */
  void redraw_overlay();
  /*@{*/
  /**
   Creates a new Fl_Overlay_Window widget using the given position, size, 
   and label (title) string.
  */
  Fl_Overlay_Window(int W, int H, const char *l=0)
    : Fl_Double_Window(W,H,l) {overlay_ = 0;}
  Fl_Overlay_Window(int X, int Y, int W, int H, const char *l=0)
    : Fl_Double_Window(X,Y,W,H,l) {overlay_ = 0;}
  /*@}*/
protected:

  virtual void flush();

private:

  friend class _Fl_Overlay;
  Fl_Window* overlay_;

};

#endif

//
// End of "$Id: Fl_Overlay_Window.h,v 1.2 2002/10/29 00:37:23 easysw Exp $".
//
