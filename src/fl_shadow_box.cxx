//
// "$Id: fl_shadow_box.cxx,v 1.16 2002/12/09 04:52:30 spitzak Exp $"
//
// Shadow box drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/draw.h>
using namespace fltk;

#define SIZE 3

class ShadowBox : public Box {
public:
  void draw(int x, int y, int w, int h, Color color, Flags f) const
  {
    w-=SIZE; h-=SIZE;
    if (!(f & INVISIBLE)) {
      setcolor(color);
      fillrect(x+1,y+1,w-2,h-2);
    }
    setcolor(GRAY33);
    fillrect(x+SIZE, y+h,  w, SIZE);
    fillrect(x+w,  y+SIZE, SIZE,  h);
    setcolor(inactive(BLACK,f));
    strokerect(x,y,w,h);
  }
  ShadowBox(const char* n) : Box(n) {
    dx_ = dy_ = 1; dw_ = dh_ = 2+SIZE;
    fills_rectangle_ = 0;
  }
};
static ShadowBox shadowBox(0);
Box* const fltk::SHADOW_BOX = &shadowBox;

//
// End of "$Id: fl_shadow_box.cxx,v 1.16 2002/12/09 04:52:30 spitzak Exp $".
//
