//
// "$Id: fl_oval_box.cxx,v 1.25 2004/01/06 06:43:02 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//


// Less-used box types are in seperate files so they are not linked
// in if not used.

#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/draw.h>
using namespace fltk;

class OvalBox : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style, Flags f) const {
    addellipse(x, y, w-1, h-1);
    Color bg, fg; style->boxcolors(f,bg,fg);
    setcolor(bg);
    fillstrokepath(fg);
  }
  const BoxInfo* boxinfo() const {
    static BoxInfo b = {1,1,2,2,0};
    return &b;
  }
  OvalBox(const char* n) : Box(n) {}
};
static OvalBox ovalBox("oval");
/*! \ingroup boxes
  Ellipse with a black border.
*/
Box* const fltk::OVAL_BOX = &ovalBox;

class OvalShadowBox : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style, Flags f) const {
    w-=3; h-=3;
    addellipse(x+3, y+3, w, h);
    setcolor(GRAY33);
    fillpath();
    ovalBox.draw(x, y, w, h, style, f);
  }
  const BoxInfo* boxinfo() const {
    static BoxInfo b = {1,1,5,5,0};
    return &b;
  }
  OvalShadowBox(const char* n) : Box(n) {}
};
static OvalShadowBox ovalShadowBox("oval_shadow");
/*! \ingroup boxes
  Ellipse with a black border and gray shadow.
*/
Box* const fltk::OSHADOW_BOX = &ovalShadowBox;

class OvalFlatBox : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style, Flags f) const {
    addellipse(x, y, w-1, h-1);
    Color bg, fg; style->boxcolors(f,bg,fg);
    setcolor(bg);
    fillpath();
  }
  OvalFlatBox(const char* n) : Box(n) {}
};
static OvalFlatBox ovalFlatBox(0);
/*! \ingroup boxes
  Ellipse with no border.
*/
Box* const fltk::OFLAT_BOX = &ovalFlatBox;


//
// End of "$Id: fl_oval_box.cxx,v 1.25 2004/01/06 06:43:02 spitzak Exp $".
//
