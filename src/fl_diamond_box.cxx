//
// "$Id: fl_diamond_box.cxx,v 1.28 2003/11/04 08:11:03 spitzak Exp $"
//
// Diamond box code for the Fast Light Tool Kit (FLTK).
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

// A less-used boxtype, it is here so that it is not linked in if
// not used.

// The diamond box draws best if the area is square!

#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/Widget.h>
#include <fltk/draw.h>
#include <string.h>

using namespace fltk;

// Diamond with an edge pattern like FrameBox:
class DiamondBox : public FrameBox {
public:
  void _draw(int,int,int,int, const Style*, Flags=0) const;
  DiamondBox(const char* n, const char* s, const FrameBox* d=0)
    : FrameBox(n, s, d) { }
};

extern void fl_to_inactive(const char* s, char* to);

void DiamondBox::_draw(int x, int y, int w, int h,
		       const Style* style, Flags flags) const
{
  int x1 = x+w/2;
  if (w&1) w--; else {w -= 2; x++;}
  int y1 = y+h/2;
  if (h&1) h--; else {h -= 2; y++;}
  const char* s = (flags & VALUE) ? down->data() : data();
  char buf[26]; if (flags&INACTIVE && style->draw_boxes_inactive()) {
    fl_to_inactive(s, buf); s = buf;}
  const char* t;
  if (*s == '2') {t = s+1; s += 3;} else {t = s+2;}
  while (*s && *t && w > 0 && h > 0) {
    // draw upper-right line:
    setcolor(*s++ + (GRAY00-'A'));
    drawline(x+w, y1, x1, y);
    // draw upper-left line:
    setcolor(*s++ + (GRAY00-'A'));
    drawline(x1, y, x, y1);
    s += 2;
    // draw lower-left line:
    setcolor(*t++ + (GRAY00-'A'));
    drawline(x, y1, x1, y+h);
    // draw lower-right line:
    setcolor(*t++ + (GRAY00-'A'));
    drawline(x1, y+h, x+w, y1);
    t += 2;
    x++; y++; w -= 2; h -= 2;
  }
  if (w > 0 && h > 0 && !(flags & INVISIBLE)) {
    Color bg, fg; style->boxcolors(flags, bg, fg);
    // draw the interior, assumming the edges are the same thickness
    // as the normal square box:
    newpath();
    addvertex(x, y1);
    addvertex(x1, y);
    addvertex(x+w,y1);
    addvertex(x1,y+h);
    setcolor(bg);
    fillstrokepath(bg);
  }
}

static DiamondBox diamondDownBox("diamond_down", "2WWMMPPAA");
Box* const fltk::DIAMOND_DOWN_BOX = &diamondDownBox;
static DiamondBox diamondUpBox("diamond_up", "2AAWWMMTT", &diamondDownBox);
Box* const fltk::DIAMOND_UP_BOX = &diamondUpBox;

//
// End of "$Id: fl_diamond_box.cxx,v 1.28 2003/11/04 08:11:03 spitzak Exp $".
//
