//
// "$Id: fl_diamond_box.cxx,v 1.19 2001/01/02 00:20:28 clip Exp $"
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

#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <string.h>

extern void fl_to_inactive(const char* s, char* to);

void Fl_Diamond_Box::draw(int x, int y, int w, int h,
			  Fl_Color c, Fl_Flags f) const
{
  w &= -2;
  h &= -2;
  int x1 = x+w/2;
  int y1 = y+h/2;
  const char* s = (f & FL_VALUE) ? down->data() : data();
  char buf[26]; if (f&FL_INACTIVE && Fl_Style::draw_boxes_inactive) {
    fl_to_inactive(s, buf); s = buf;}
  if (!(f & FL_FRAME_ONLY)) {
    // draw the interior, assumming the edges are the same thickness
    // as the normal square box:
    int d = strlen(s)/4;
    if (w > 2*d && h > 2*d) {
      fl_color(c);
      fl_polygon(x+d, y1, x1,y+d, x+w-d,y1, x1,y+h-d);
    }
  }
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
}

void Fl_Diamond_Box::draw(const Fl_Widget* widget,
			int x, int y, int w, int h, Fl_Flags f) const {
  draw(x,y,w,h, widget->get_box_color(f), f);
}

Fl_Diamond_Box::Fl_Diamond_Box(const char* n, const char* s, const Fl_Frame_Box* d)
  : Fl_Frame_Box(n, s, d)
{
  fills_rectangle_ = 0;
}

const Fl_Diamond_Box fl_diamond_up_box(0, "2WWMMPPAA", &fl_diamond_down_box);
const Fl_Diamond_Box fl_diamond_down_box(0, "2AAWWMMTT", &fl_diamond_up_box);

//
// End of "$Id: fl_diamond_box.cxx,v 1.19 2001/01/02 00:20:28 clip Exp $".
//
