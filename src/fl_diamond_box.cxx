//
// "$Id: fl_diamond_box.cxx,v 1.23 2002/01/20 07:37:16 spitzak Exp $"
//
// Diamond box code for the Fast Light Tool Kit (FLTK).
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

// A less-used boxtype, it is here so that it is not linked in if
// not used.

// The diamond box draws best if the area is square!

#include <fltk/Fl_Boxtype.h>
#include <fltk/Fl_Style.h>
#include <fltk/Fl_Widget.h>
#include <fltk/fl_draw.h>
#include <string.h>

extern void fl_to_inactive(const char* s, char* to);

void Fl_Diamond_Box::draw(int x, int y, int w, int h,
			  Fl_Color c, Fl_Flags f) const
{
  int x1 = x+w/2;
  if (w&1) w--; else {w -= 2; x++;}
  int y1 = y+h/2;
  if (h&1) h--; else {h -= 2; y++;}
  const char* s = (f & FL_VALUE) ? down->data() : data();
  char buf[26]; if (f&FL_INACTIVE && Fl_Style::draw_boxes_inactive) {
    fl_to_inactive(s, buf); s = buf;}
  const char* t;
  if (*s == '2') {t = s+1; s += 3;} else {t = s+2;}
  while (*s && *t && w > 0 && h > 0) {
    // draw upper-right line:
    fl_color(*s++ + (FL_GRAY_RAMP-'A'));
    fl_line(x+w, y1, x1, y);
    // draw upper-left line:
    fl_color(*s++ + (FL_GRAY_RAMP-'A'));
    fl_line(x1, y, x, y1);
    s += 2;
    // draw lower-left line:
    fl_color(*t++ + (FL_GRAY_RAMP-'A'));
    fl_line(x, y1, x1, y+h);
    // draw lower-right line:
    fl_color(*t++ + (FL_GRAY_RAMP-'A'));
    fl_line(x1, y+h, x+w, y1);
    t += 2;
    x++; y++; w -= 2; h -= 2;
  }
  if (w > 0 && h > 0 && !(f & FL_INVISIBLE)) {
    // draw the interior, assumming the edges are the same thickness
    // as the normal square box:
    fl_newpath();
    fl_vertex(x, y1);
    fl_vertex(x1, y);
    fl_vertex(x+w,y1);
    fl_vertex(x1,y+h);
    fl_color(c);
    fl_fill_stroke(c);
  }
}

Fl_Diamond_Box::Fl_Diamond_Box(const char* n, const char* s, const Fl_Frame_Box* d)
  : Fl_Frame_Box(n, s, d)
{
  fills_rectangle_ = 0;
}

const Fl_Diamond_Box fl_diamond_up_box(0, "2AAWWMMTT", &fl_diamond_down_box);
const Fl_Diamond_Box fl_diamond_down_box(0, "2WWMMPPAA", &fl_diamond_up_box);

//
// End of "$Id: fl_diamond_box.cxx,v 1.23 2002/01/20 07:37:16 spitzak Exp $".
//
