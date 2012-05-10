//
// "$Id$:
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
//------- test the point drawing capabilities of this implementation ----------
//
class PointTest : public fltk3::Box {
public:
  static fltk3::Widget *create() {
    return new PointTest(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H);
  }
  PointTest(int x, int y, int w, int h) : fltk3::Box(x, y, w, h) {
    label("testing the fltk3::point call\n"
	  "You should see four pixels each in black, red, green and blue. "
	  "Make sure that pixels are not anti-aliased (blurred across multiple pixels)!");
    align(fltk3::ALIGN_INSIDE|fltk3::ALIGN_BOTTOM|fltk3::ALIGN_LEFT|fltk3::ALIGN_WRAP);
    box(fltk3::BORDER_BOX);
  }
  void draw() {
    fltk3::Box::draw();
    int a = 10, b = 10; 
    fltk3::color(fltk3::WHITE); fltk3::rectf(a, b, 90, 90);
    fltk3::color(fltk3::BLACK); fltk3::rect(a, b, 90, 90);
    fltk3::point(a+10, b+10); fltk3::point(a+20, b+20);
    fltk3::point(a+10, b+20); fltk3::point(a+20, b+10);
    fltk3::color(fltk3::RED); a = x()+70;
    fltk3::point(a+10, b+10); fltk3::point(a+20, b+20);
    fltk3::point(a+10, b+20); fltk3::point(a+20, b+10);
    fltk3::color(fltk3::GREEN); a = x()+10; b = y()+70;
    fltk3::point(a+10, b+10); fltk3::point(a+20, b+20);
    fltk3::point(a+10, b+20); fltk3::point(a+20, b+10);
    fltk3::color(fltk3::BLUE); a = x()+70;
    fltk3::point(a+10, b+10); fltk3::point(a+20, b+20);
    fltk3::point(a+10, b+20); fltk3::point(a+20, b+10);
  }
};

UnitTest points("drawing points", PointTest::create);

//
// End of "$Id$"
//
