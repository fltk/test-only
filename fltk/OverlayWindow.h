//
// "$Id: OverlayWindow.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// Window that has the ability to use hardware overlays to draw a
// quickly-changing display over it's contents. Simulated using
// double-buffering if there is no hardware overlay. This is used
// by fluid to draw the red outlines over the display.
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#ifndef fltk_Overlay_Window_h
#define fltk_Overlay_Window_h

#include "DoubleBufferWindow.h"

namespace fltk {

class FL_API OverlayWindow : public DoubleBufferWindow {

public:

  virtual void draw_overlay() = 0;
  virtual void layout();
  int can_do_overlay();
  void redraw_overlay();
  OverlayWindow(int W, int H, const char *l=0)
    : DoubleBufferWindow(W,H,l) {overlay_ = 0;}
  OverlayWindow(int X, int Y, int W, int H, const char *l=0)
    : DoubleBufferWindow(X,Y,W,H,l) {overlay_ = 0;}

protected:

  virtual void flush();

private:

  Window* overlay_;

};

}

#endif

//
// End of "$Id: OverlayWindow.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $".
//
