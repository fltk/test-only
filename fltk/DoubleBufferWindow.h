//
// "$Id: DoubleBufferWindow.h,v 1.2 2002/12/10 02:00:29 easysw Exp $"
//
// Double-buffered window. Uses XDBE if possible on X. Uses offscreen
// pixmap on Windows or on X if no XDBE.
//
// Copyright 2002 by Bill Spitzak and others.
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

#ifndef fltk_DoubleBufferWindow_h
#define fltk_DoubleBufferWindow_h

#ifndef fltk_Window_h
# include "Window.h"
#endif

namespace fltk {

class FL_API DoubleBufferWindow : public Window {

public:

  virtual void layout();
  ~DoubleBufferWindow();
  DoubleBufferWindow(int W, int H, const char *l = 0) : Window(W,H,l) {}
  DoubleBufferWindow(int X, int Y, int W, int H, const char *l = 0)
    : Window(X,Y,W,H,l) {}

protected:

  virtual void create();
  virtual void flush();
  void flush(bool eraseoverlay);
  virtual void destroy();

private:

  void free_backbuffer();

};

}

#endif

//
// End of "$Id: DoubleBufferWindow.h,v 1.2 2002/12/10 02:00:29 easysw Exp $".
//
