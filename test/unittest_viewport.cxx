//
// "$Id$"
//
// Unit tests for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#include "unittests.h"

#include <fltk3/Box.h>
#include <fltk3/draw.h>

//
//------- test viewport clipping ----------
//
class ViewportTest : public fltk3::Widget {
public: 
  static fltk3::Widget *create() {
    return new ViewportTest(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H);
  }
  ViewportTest(int x, int y, int w, int h) : fltk3::Widget(x, y, w, h) { 
    label("Testing Viewport Alignment\n\n"
          "Only green lines should be visible.\n"
          "If red lines are visible in the corners of this window,\n"
          "your viewport alignment and clipping is off.\n"
          "If there is a space between the green lines and the window border,\n"
          "the viewport is off, but some clipping may be working.\n"
          "Also, your window size may be off to begin with.");
    align(fltk3::ALIGN_INSIDE|fltk3::ALIGN_CENTER|fltk3::ALIGN_WRAP);
    box(fltk3::BORDER_BOX);
  }
  void show() {
    fltk3::Widget::show();
    mainwin->testAlignment(1);
  }
  void hide() {
    fltk3::Widget::hide();
    mainwin->testAlignment(0);
  }
};

UnitTest viewport("viewport test", ViewportTest::create);

//
// End of "$Id$
//
