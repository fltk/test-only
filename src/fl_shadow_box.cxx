//
// "$Id: fl_shadow_box.cxx,v 1.19 2003/11/04 08:11:04 spitzak Exp $"
//
// Shadow box drawing routines for the Fast Light Tool Kit (FLTK).
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

#define SIZE 3

class ShadowBox : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style, Flags f) const
  {
    Color bg, fg; style->boxcolors(f, bg, fg);
    w-=SIZE; h-=SIZE;
    if (w > 2 && h > 2 && !(f & INVISIBLE)) {
      setcolor(bg);
      fillrect(x+1,y+1,w-2,h-2);
    }
    setcolor(GRAY33);
    fillrect(x+SIZE, y+h,  w, SIZE);
    fillrect(x+w,  y+SIZE, SIZE,  h);
    setcolor(fg);
    strokerect(x,y,w,h);
  }
  const BoxInfo* boxinfo() const {
    static BoxInfo b = {1,1,2+SIZE,2+SIZE,0};
    return &b;
  }
  ShadowBox(const char* n) : Box(n) {}
};
static ShadowBox shadowBox("shadow_box");
Box* const fltk::SHADOW_BOX = &shadowBox;

//
// End of "$Id: fl_shadow_box.cxx,v 1.19 2003/11/04 08:11:04 spitzak Exp $".
//
