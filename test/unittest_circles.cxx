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
//------- test the circle drawing capabilities of this implementation ----------
//
class CircleTest : public fltk3::Box {
public: 
  static fltk3::Widget *create() { 
    return new CircleTest(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H);
  }
  CircleTest(int x, int y, int w, int h) : fltk3::Box(x, y, w, h) {
    label("testing int drawing of circles and ovals (fltk3::arc, fltk3::pie)\n"
          "No red lines should be visible. "
          "If you see bright red pixels, the circle drawing alignment is off. "
          "If you see dark red pixels, your system supports anti-aliasing "
          "which should be of no concern. "
          "The green rectangles should not be overwritten by circle drawings.");
    align(fltk3::ALIGN_INSIDE|fltk3::ALIGN_BOTTOM|fltk3::ALIGN_LEFT|fltk3::ALIGN_WRAP);
    box(fltk3::BORDER_BOX);
  }
  void draw() {
    fltk3::Box::draw();
    int a = 10, b = 10; fltk3::color(fltk3::BLACK); fltk3::rect(a, b, 100, 100);
    // test fltk3::arc for full circles
    fltk3::color(fltk3::GREEN); fltk3::rect(a+ 9, b+ 9, 33, 33);
    fltk3::color(fltk3::RED); fltk3::xyline(a+24, b+10, a+27); fltk3::xyline(a+24, b+40, a+27);
    fltk3::yxline(a+10, b+24, b+27); fltk3::yxline(a+40, b+24, b+27);
    fltk3::color(fltk3::BLACK); fltk3::arc(a+10, b+10, 31, 31, 0.0, 360.0);
    // test fltk3::arc segmet 1
    fltk3::color(fltk3::GREEN); fltk3::rect(a+54, b+ 4, 43, 43);
    fltk3::rect(a+54, b+4, 18, 18); fltk3::rect(a+79, b+29, 18, 18);
    fltk3::color(fltk3::RED); fltk3::point(a+55, b+30); fltk3::point(a+70, b+45);
    fltk3::point(a+80, b+5); fltk3::point(a+95, b+20);
    fltk3::color(fltk3::BLACK); fltk3::arc(a+65, b+ 5, 31, 31, -35.0, 125.0);
    // test fltk3::arc segmet 2
    fltk3::color(fltk3::BLACK); fltk3::arc(a+55, b+15, 31, 31, 145.0, 305.0);
    // test fltk3::pie for full circles
    fltk3::color(fltk3::RED); fltk3::xyline(a+24, b+60, a+27); fltk3::xyline(a+24, b+90, a+27);
    fltk3::yxline(a+10, b+74, b+77); fltk3::yxline(a+40, b+74, b+77);
    fltk3::color(fltk3::GREEN); fltk3::rect(a+ 9, b+59, 33, 33);
    fltk3::color(fltk3::BLACK); fltk3::pie(a+10, b+60, 31, 31, 0.0, 360.0);
    // test fltk3::pie segmet 1
    fltk3::color(fltk3::GREEN); fltk3::rect(a+54, b+54, 43, 43);
    fltk3::rect(a+54, b+54, 18, 18); fltk3::rect(a+79, b+79, 18, 18);
    fltk3::point(a+79, b+71); fltk3::point(a+71, b+79);
    fltk3::color(fltk3::RED); fltk3::point(a+55, b+80); fltk3::point(a+70, b+95);
    fltk3::point(a+80, b+55); fltk3::point(a+95, b+70);
    fltk3::point(a+81, b+69); fltk3::point(a+69, b+81);
    fltk3::color(fltk3::BLACK); fltk3::pie(a+65, b+55, 31, 31, -30.0, 120.0);
    // test fltk3::pie segmet 2
    fltk3::color(fltk3::BLACK); fltk3::pie(a+55, b+65, 31, 31, 150.0, 300.0);
    //---- oval testing (horizontal squish)
    a +=120; b += 0; fltk3::color(fltk3::BLACK); fltk3::rect(a, b, 100, 100);
    fltk3::color(fltk3::GREEN);
    fltk3::rect(a+19, b+9, 63, 33); fltk3::rect(a+19, b+59, 63, 33);
    fltk3::color(fltk3::BLACK);
    fltk3::arc(a+20, b+10, 61, 31, 0, 360); fltk3::pie(a+20, b+60, 61, 31, 0, 360);
    //---- oval testing (horizontal squish)
    a += 120; b += 0; fltk3::color(fltk3::BLACK); fltk3::rect(a, b, 100, 100);
    fltk3::color(fltk3::GREEN);
    fltk3::rect(a+9, b+19, 33, 63); fltk3::rect(a+59, b+19, 33, 63);
    fltk3::color(fltk3::BLACK);
    fltk3::arc(a+10, b+20, 31, 61, 0, 360); fltk3::pie(a+60, b+20, 31, 61, 0, 360);
  }
};

UnitTest circle("circles and arcs", CircleTest::create);

//
// End of "$Id$"
//
