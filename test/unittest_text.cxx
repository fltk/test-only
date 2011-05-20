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

#include <fltk3/Box.h>
#include <fltk3/draw.h>

//
// --- fltk3::text_extents() tests -----------------------------------------------
//
class TextExtentsTest : public fltk3::Widget
{
  void DrawTextAndBoxes(const char *txt, int X, int Y) {
    int wo = 0, ho = 0;
    int dx, dy;
    // First, we draw the bounding boxes (fltk3::measure and fltk3::text_extents)
    // draw fltk3::measure() typographical bounding box
    fltk3::measure(txt, wo, ho, 0);
    int desc = fltk3::descent();
    fltk3::color(fltk3::RED);
    fltk3::rect(X, Y-ho+desc, wo, ho);
    // draw fltk3::text_extents() glyph bounding box
    fltk3::text_extents(txt, dx, dy, wo, ho);
    fltk3::color(fltk3::GREEN);
    fltk3::rect(X+dx, Y+dy, wo, ho);
    // Then we draw the text to show how it fits insode each of the two boxes
    fltk3::color(fltk3::BLACK);
    fltk3::draw(txt, X, Y);
  }
public: 
  static fltk3::Widget *create() {
    return new TextExtentsTest(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H);
  }
  TextExtentsTest(int x, int y, int w, int h) : fltk3::Widget(x, y, w, h) {}
  void draw(void) {
    int x0 = x(); // origin is current window position for fltk3::Box
    int y0 = y();
    int w0 = w();
    int h0 = h();
    fltk3::push_clip(x0, y0, w0, h0); // reset local clipping
    {
      // set the background colour - slightly off-white to enhance the green bounding box
      fltk3::color(fltk3::gray_ramp(fltk3::NUM_GRAY - 3));
      fltk3::rectf(x0, y0, w0, h0);

      fltk3::font(fltk3::HELVETICA, 30);
      int xx = x0+55;
      int yy = y0+40;
      DrawTextAndBoxes("!abcdeABCDE\"#A", xx, yy); yy += 50;	// mixed string
      DrawTextAndBoxes("oacs",     xx, yy); xx += 100;		// small glyphs
      DrawTextAndBoxes("qjgIPT",   xx, yy); yy += 50; xx -= 100;	// glyphs with descenders
      DrawTextAndBoxes("````````", xx, yy); yy += 50;		// high small glyphs
      DrawTextAndBoxes("--------", xx, yy); yy += 50;		// mid small glyphs
      DrawTextAndBoxes("________", xx, yy); yy += 50;		// low small glyphs

      fltk3::font(fltk3::HELVETICA, 14);
      fltk3::color(fltk3::RED);  fltk3::draw("fltk3::measure bounding box in RED",       xx, yy); yy += 20;
      fltk3::color(fltk3::GREEN); fltk3::draw("fltk3::text_extents bounding box in GREEN", xx, yy);
      fltk3::color(fltk3::BLACK);
      xx = x0 + 10;  yy += 30;
      fltk3::draw("NOTE: On systems with text anti-aliasing (e.g. OSX Quartz)", xx, yy);
      w0 = h0 = 0; fltk3::measure("NOTE: ", w0, h0, 0);
      xx += w0; yy += h0;
      fltk3::draw("text may leak slightly outside the fltk3::text_extents()", xx, yy);
    }
    fltk3::pop_clip(); // remove the local clip
  }
};

UnitTest textExtents("rendering text", TextExtentsTest::create);

//
// End of "$Id$"
//
