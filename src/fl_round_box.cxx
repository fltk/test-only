//
// "$Id: fl_round_box.cxx,v 1.28 2002/01/28 08:03:00 spitzak Exp $"
//
// Round box drawing routines for the Fast Light Tool Kit (FLTK).
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

// Box drawing code for an obscure box type.
// These box types are in seperate files so they are not linked
// in if not used.

#include <fltk/Fl_Boxtype.h>
#include <fltk/Fl_Style.h>
#include <fltk/fl_draw.h>
#include <string.h>

enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

// Draw the oval shape. This is ugly because I need to cut the path
// up into individual arcs so the drawing library uses the server
// arc code.

static void lozenge(int which, int x,int y,int w,int h, Fl_Color color)
{
  w--; h--;
  int d = w <= h ? w : h;
  if (d <= 1) return;
  fl_color((Fl_Color)color);
  int what = (which==FILL ? FL_PIE : FL_ARC);
  if (which >= CLOSED) {
    fl_pie(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90, what);
    fl_pie(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270, what);
  } else if (which == UPPER_LEFT) {
    fl_pie(x+w-d, y, d, d, 45, w<=h ? 180 : 90, what);
    fl_pie(x, y+h-d, d, d, w<=h ? 180 : 90, 225, what);
  } else { // LOWER_RIGHT
    fl_pie(x, y+h-d, d, d, 225, w<=h ? 360 : 270, what);
    fl_pie(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45, what);
  }
  if (which == FILL) {
    if (w < h)
      fl_rectf(x, y+d/2, w, h-(d&-2));
    else if (w > h)
      fl_rectf(x+d/2, y, w-(d&-2), h);
  } else {
    if (w < h) {
      if (which != UPPER_LEFT) fl_line(x+w, y+d/2, x+w, y+h-d/2);
      if (which != LOWER_RIGHT) fl_line(x, y+d/2, x, y+h-d/2);
    } else if (w > h) {
      if (which != UPPER_LEFT) fl_line(x+d/2, y+h, x+w-d/2, y+h);
      if (which != LOWER_RIGHT) fl_line(x+d/2, y, x+w-d/2, y);
    }
  }
}

extern void fl_to_inactive(const char* s, char* to);

void Fl_Round_Box::draw(int x, int y, int w, int h,
			Fl_Color c, Fl_Flags f) const
{
  const char* s = (f & FL_VALUE) ? down->data() : data();
  char buf[26]; if (f&FL_INACTIVE && Fl_Style::draw_boxes_inactive) {
    fl_to_inactive(s, buf); s = buf;}
  if (!(f & FL_INVISIBLE)) {
    // draw the interior, assumming the edges are the same thickness
    // as the normal square box:
    int d = strlen(s)/4;
    if (w > 2*d && h > 2*(d-1))
      lozenge(FILL, x+d, y+d-1, w-2*d, h-2*(d-1), c);
  }
  const char* t;
  if (*s == '2') {t = s+1; s += 3;} else {t = s+2;}
  while (*s && *t && w > 0 && h > 0) {
    Fl_Color c1 = *s + (FL_GRAY_RAMP-'A'); s += 4;
    Fl_Color c2 = *t + (FL_GRAY_RAMP-'A'); t += 4;
    lozenge(UPPER_LEFT,  x+1, y,   w-2, h, *s&&*t ? c1 : c);
    lozenge(UPPER_LEFT,  x,   y,   w,   h, c1);
    lozenge(LOWER_RIGHT, x+1, y,   w-2, h, *s&&*t ? c2 : c);
    lozenge(LOWER_RIGHT, x,   y,   w,   h, c2);
    x++; y++; w -= 2; h -= 2;
  }
}

Fl_Round_Box::Fl_Round_Box(const char* n, const char* s, const Fl_Frame_Box* d)
  : Fl_Frame_Box(n, s, d)
{
  fills_rectangle_ = 0;
}

const Fl_Round_Box fl_round_down_box(0, "2WWMMPPAA");
const Fl_Round_Box fl_round_up_box(0, "2AAWWMMTT", &fl_round_down_box);

//
// End of "$Id: fl_round_box.cxx,v 1.28 2002/01/28 08:03:00 spitzak Exp $".
//
