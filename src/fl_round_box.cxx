//
// "$Id: fl_round_box.cxx,v 1.33 2003/12/13 11:06:53 spitzak Exp $"
//
// Round box drawing routines for the Fast Light Tool Kit (FLTK).
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

// Box drawing code for an obscure box type.
// These box types are in seperate files so they are not linked
// in if not used.

#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/draw.h>
#include <string.h>
using namespace fltk;

// Circle with an edge pattern like FrameBox:
class RoundBox : public FrameBox {
public:
  void _draw(int,int,int,int, const Style*, Flags) const;
  RoundBox(const char* n, const char* s, const FrameBox* d=0)
    : FrameBox(n, s, d) {boxinfo_.fills_rectangle = 0;}
};

enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

// Draw the oval shape. This is ugly because I need to cut the path
// up into individual arcs so the drawing library uses the server
// arc code.

static void lozenge(int which, int x,int y,int w,int h, Color color)
{
  w--; h--;
  int d = w <= h ? w : h;
  if (d <= 1) return;
  setcolor(color);
  int what = (which==FILL ? FILLPIE : STROKEARC);
  if (which >= CLOSED) {
    fillpie(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90, what);
    fillpie(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270, what);
  } else if (which == UPPER_LEFT) {
    fillpie(x+w-d, y, d, d, 45, w<=h ? 180 : 90, what);
    fillpie(x, y+h-d, d, d, w<=h ? 180 : 90, 225, what);
  } else { // LOWER_RIGHT
    fillpie(x, y+h-d, d, d, 225, w<=h ? 360 : 270, what);
    fillpie(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45, what);
  }
  if (which == FILL) {
    if (w < h)
      fillrect(x, y+d/2, w, h-(d&-2));
    else if (w > h)
      fillrect(x+d/2, y, w-(d&-2), h);
  } else {
    if (w < h) {
      if (which != UPPER_LEFT) drawline(x+w, y+d/2, x+w, y+h-d/2);
      if (which != LOWER_RIGHT) drawline(x, y+d/2, x, y+h-d/2);
    } else if (w > h) {
      if (which != UPPER_LEFT) drawline(x+d/2, y+h, x+w-d/2, y+h);
      if (which != LOWER_RIGHT) drawline(x+d/2, y, x+w-d/2, y);
    }
  }
}

extern void fl_to_inactive(const char* s, char* to);

void RoundBox::_draw(int x, int y, int w, int h,
		     const Style* style, Flags f) const
{
  const char* s = (f & VALUE) ? down->data() : data();
  char buf[26]; if (f&INACTIVE && style->draw_boxes_inactive()) {
    fl_to_inactive(s, buf); s = buf;}
  Color bg, fg; style->boxcolors(f, bg, fg);
  if (!(f & INVISIBLE)) {
    // draw the interior, assumming the edges are the same thickness
    // as the normal square box:
    int d = strlen(s)/4;
    if (w > 2*d && h > 2*(d-1))
      lozenge(FILL, x+d, y+d-1, w-2*d, h-2*(d-1), bg);
  }
  const char* t;
  if (*s == '2') {t = s+1; s += 3;} else {t = s+2;}
  while (*s && *t && w > 0 && h > 0) {
    Color c1 = *t + (GRAY00-'A'); t += 4;
    Color c2 = *s + (GRAY00-'A'); s += 4;
    lozenge(UPPER_LEFT,  x+1, y,   w-2, h, *s&&*t ? c1 : bg);
    lozenge(UPPER_LEFT,  x,   y,   w,   h, c1);
    lozenge(LOWER_RIGHT, x+1, y,   w-2, h, *s&&*t ? c2 : bg);
    lozenge(LOWER_RIGHT, x,   y,   w,   h, c2);
    x++; y++; w -= 2; h -= 2;
  }
}

static RoundBox roundDownBox("round_down", "WWMMPPAA");
Box* const fltk::ROUND_DOWN_BOX = &roundDownBox;
static RoundBox roundUpBox("round_up", "AAWWMMTT", &roundDownBox);
Box* const fltk::ROUND_UP_BOX = &roundUpBox;

//
// End of "$Id: fl_round_box.cxx,v 1.33 2003/12/13 11:06:53 spitzak Exp $".
//
