//
// "$Id: fl_oval_box.cxx,v 1.22 2002/12/10 01:46:18 easysw Exp $"
//
// Oval box drawing code for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//


// Less-used box types are in seperate files so they are not linked
// in if not used.

#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/draw.h>
using namespace fltk;

class OvalBox : public Box {
public:
  void draw(int x, int y, int w, int h, Color color, Flags f) const {
    addellipse(x, y, w-1, h-1);
    setcolor(color);
    fillstrokepath(inactive(BLACK,f));
  }
  OvalBox(const char* n) : Box(n) {
    dx_ = dy_ = 1; dw_ = dh_ = 2;
    fills_rectangle_ = 0;
  }
};
static OvalBox ovalBox(0);
Box* const fltk::OVAL_BOX = &ovalBox;

class OvalShadowBox : public Box {
public:
  void draw(int x, int y, int w, int h, Color color, Flags f) const {
    w-=3; h-=3;
    addellipse(x+3, y+3, w, h);
    setcolor(GRAY33);
    fillpath();
    ovalBox.draw(x, y, w, h, color, f);
  }
  OvalShadowBox(const char* n) : Box(n) {
    dx_ = dy_ = 1; dw_ = dh_ = 5;
    fills_rectangle_ = 0;
  }
};
static OvalShadowBox ovalShadowBox(0);
Box* const fltk::OSHADOW_BOX = &ovalShadowBox;

class OvalFlatBox : public Box {
public:
  void draw(int x, int y, int w, int h, Color color, Flags) const {
    addellipse(x, y, w-1, h-1);
    setcolor(color);
    fillpath();
  }
  OvalFlatBox(const char* n) : Box(n) {
    dx_ = dy_ = dw_ = dh_ = 0;
    fills_rectangle_ = 0;
  }
};
static OvalFlatBox ovalFlatBox(0);
Box* const fltk::OFLAT_BOX = &ovalFlatBox;


//
// End of "$Id: fl_oval_box.cxx,v 1.22 2002/12/10 01:46:18 easysw Exp $".
//
