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
//------- test the line drawing capabilities of this implementation ----------
//
class LineTest : public fltk3::Widget {
public: 
  static fltk3::Widget *create() {
    return new LineTest(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H);
  }
  LineTest(int x, int y, int w, int h) : fltk3::Widget(x, y, w, h) {
    label("testing the integer based fltk3::line calls\n"
	  "No red pixels should be visible.\n"
	  "If you see bright red pixels, the line drawing alignment is off,\n"
	  "or the last pixel in a line does not get drawn.\n"
	  "If you see dark red pixels, anti-aliasing must be switched off.");
    align(fltk3::ALIGN_INSIDE|fltk3::ALIGN_BOTTOM|fltk3::ALIGN_LEFT|fltk3::ALIGN_WRAP);
    box(fltk3::BORDER_BOX);
  }
  void draw() {
    fltk3::Widget::draw();
    int a = 10, b = 10; fltk3::color(fltk3::BLACK); fltk3::rect(a, b, 100, 100);
    // testing fltk3::xyline(x, y, x1)
    fltk3::color(fltk3::RED); fltk3::point(a+10, b+10); fltk3::point(a+20, b+10);
    fltk3::color(fltk3::BLACK); fltk3::xyline(a+10, b+10, a+20);
    // testing fltk3::xyline(x, y, x1, y2);
    fltk3::color(fltk3::RED); fltk3::point(a+10, b+20); fltk3::point(a+20, b+20);
    fltk3::point(a+20, b+30);
    fltk3::color(fltk3::BLACK); fltk3::xyline(a+10, b+20, a+20, b+30);
    // testing fltk3::xyline(x, y, x1, y2, x3);
    fltk3::color(fltk3::RED); fltk3::point(a+10, b+40); fltk3::point(a+20, b+40);
    fltk3::point(a+20, b+50); fltk3::point(a+30, b+50);
    fltk3::color(fltk3::BLACK); fltk3::xyline(a+10, b+40, a+20, b+50, a+30);
    //+++ add testing for the fltk3::yxline commands!
    // testing fltk3::loop(x,y, x,y, x,y, x, y)
    fltk3::color(fltk3::RED); fltk3::point(a+60, b+60); fltk3::point(a+90, b+60);
    fltk3::point(a+60, b+90); fltk3::point(a+90, b+90);
    fltk3::color(fltk3::BLACK);
    fltk3::loop(a+60, b+60, a+90, b+60, a+90, b+90, a+60, b+90);
  }
};

UnitTest lines("drawing lines", LineTest::create);

//
// End of "$Id$"
//
