//
// "$Id: fl_rounded_box.cxx,v 1.23 2004/01/06 06:43:02 spitzak Exp $"
//
// Rounded box drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/draw.h>
using namespace fltk;

static void rbox(int x, int y, int w, int h, Color fill, Color line) {
  // figure out diameter of circles for corners:
  float d = w|1;
  if (h < w) d = h|1;
  if (d > 31) d = 31;
  
  //d -= .5;
  float X = x;
  float Y = y;
  w--; h--;
  addarc(X, Y, d, d, 90, 180);
  addarc(X, Y+h-d, d, d, 180, 270);
  addarc(X+w-d, Y+h-d, d, d, 270, 360);
  addarc(X+w-d, Y, d, d, 0, 90);
  setcolor(fill);
  fillstrokepath(line);
}

class RoundedBox : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style, Flags f) const {
    Color bg, fg; style->boxcolors(f, bg, fg);
    rbox(x, y, w, h, bg, fg);
  }
  const BoxInfo* boxinfo() const {
    static BoxInfo b = {1,1,2,2,0};
    return &b;
  }
  RoundedBox(const char* n) : Box(n) {}
};
static RoundedBox roundedBox("rounded");
/*! \ingroup boxes
  Round-cornered rectangle with a black border.
*/
Box* const fltk::ROUNDED_BOX = &roundedBox;

class RShadowBox : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style, Flags f) const {
    w -= 3; h -= 3;
    // draw shadow:
    rbox(x+3, y+3, w, h, GRAY33, GRAY33);
    // draw the box:
    roundedBox.draw(x, y, w, h, style, f);
  }
  const BoxInfo* boxinfo() const {
    static BoxInfo b = {1,1,5,5,0};
    return &b;
  }
  RShadowBox(const char* n) : Box(n) {}
};
static RShadowBox rshadowBox("rshadow");
/*! \ingroup boxes
  Round-cornered rectangle with a black border and gray shadow.
*/
Box* const fltk::RSHADOW_BOX = &rshadowBox;

class RFlatBox : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style, Flags f) const {
    Color bg, fg; style->boxcolors(f, bg, fg);
    rbox(x, y, w, h, bg, bg);
  }
  const BoxInfo* boxinfo() const {
    static BoxInfo b = {7,7,14,14,0};
    return &b;
  }
  RFlatBox(const char* n) : Box(n) {}
};
static RFlatBox rflatBox("rflat");
/*! \ingroup boxes
  Round-cornered rectangle with no border.
*/
Box* const fltk::RFLAT_BOX = &rflatBox;

//
// End of "$Id: fl_rounded_box.cxx,v 1.23 2004/01/06 06:43:02 spitzak Exp $".
//
