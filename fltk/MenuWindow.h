//
// "$Id: MenuWindow.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// A MenuWindow tries to tell the system to display it as fast as possible
// and without any borders. On X this does override-redirect. Earlier
// versions would also try to use overlay planes, though that is
// disabled currently.
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

#ifndef fltk_MenuWindow_h
#define fltk_MenuWindow_h

#include "Window.h"

namespace fltk {

class FL_API MenuWindow : public Window {
  enum {NO_OVERLAY = 0x08000000};

public:

  virtual void create();
  virtual void flush();
  virtual void destroy();

  int overlay() {return !(flags()&NO_OVERLAY);}
  void set_overlay() {clear_flag(NO_OVERLAY);}
  void clear_overlay() {set_flag(NO_OVERLAY);}
  ~MenuWindow();
  MenuWindow(int W, int H, const char *l = 0) : Window(W,H,l) {}
  MenuWindow(int X, int Y, int W, int H, const char *l = 0) : Window(X,Y,W,H,l) {}

};

}

#endif

//
// End of "$Id: MenuWindow.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $".
//
