//
// "$Id: fl_plastic_box.cxx,v 1.1 2002/02/10 22:57:49 spitzak Exp $"
//
// "Plastic" drawing routines for the Fast Light Tool Kit (FLTK).
//
// These box types provide a cross between Aqua and KDE buttons; kindof
// like translucent plastic buttons...
//
// Copyright 2001-2002 by Michael Sweet.
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

#include <fltk/Fl_Boxtype.h>
#include <fltk/Fl_Style.h>
#include <fltk/fl_draw.h>
#include <string.h>

extern void fl_to_inactive(const char* s, char* to);

static inline Fl_Color shade_color(uchar gc, Fl_Color bc) {
  return fl_color_average(gc+(FL_GRAY_RAMP-'A'), bc, 0.75f);
}

#if 0 // this code is not used in fltk 2.0
static void shade_frame(int x, int y, int w, int h, const char *c, Fl_Color bc) {
  uchar *g = fl_gray_ramp();
  int b = strlen(c) / 4 + 1;

  for (x += b, y += b, w -= 2 * b + 1, h -= 2 * b + 1; b > 1; b --)
  {
    // Draw lines around the perimeter of the button, 4 colors per
    // circuit.
    fl_color(shade_color(*c++, bc));
    fl_line(x, y + h + b, x + w - 1, y + h + b, x + w + b - 1, y + h);
    fl_color(shade_color(*c++, bc));
    fl_line(x + w + b - 1, y + h, x + w + b - 1, y, x + w - 1, y - b);
    fl_color(shade_color(*c++, bc));
    fl_line(x + w - 1, y - b, x, y - b, x - b, y);
    fl_color(shade_color(*c++, bc));
    fl_line(x - b, y, x - b, y + h, x, y + h + b);
  }
}
#endif

void Fl_Plastic_Box::draw(int x, int y, int w, int h, Fl_Color bc, Fl_Flags f) const
{
  const char* c = (f & FL_VALUE) ? down->data() : data();
  char buf[26]; if (f&FL_INACTIVE && Fl_Style::draw_boxes_inactive) {
    fl_to_inactive(c, buf); c = buf;}

  int		i, j;
  int		clen = strlen(c) - 1;
  int		chalf = clen / 2;
  int		cstep = 1;

  if (h < (w * 2)) {
    // Horizontal shading...
    if (clen >= h) cstep = 2;

    for (i = 0, j = 0; j < chalf; i ++, j += cstep) {
      // Draw the top line and points...
      fl_color(shade_color(c[i], bc));
      fl_line(x + 1, y + i, x + w - 1, y + i);

      fl_color(shade_color(c[i] - 2, bc));
      fl_point(x, y + i + 1);
      fl_point(x + w - 1, y + i + 1);

      // Draw the bottom line and points...
      fl_color(shade_color(c[clen - i], bc));
      fl_line(x + 1, y + h - 1 - i, x + w - 1, y + h - 1 - i);

      fl_color(shade_color(c[clen - i] - 2, bc));
      fl_point(x, y + h - i);
      fl_point(x + w - 1, y + h - i);
    }

    // Draw the interior and sides...
    i = chalf / cstep;

    fl_color(shade_color(c[chalf], bc));
    fl_rectf(x + 1, y + i, w - 2, h - 2 * i);

    fl_color(shade_color(c[chalf] - 2, bc));
    fl_line(x, y + i, x, y + h - i);
    fl_line(x + w - 1, y + i, x + w - 1, y + h - i);
  } else {
    // Vertical shading...
    if (clen >= w) cstep = 2;

    for (i = 0, j = 0; j < chalf; i ++, j += cstep) {
      // Draw the left line and points...
      fl_color(shade_color(c[i], bc));
      fl_line(x + i, y + 1, x + i, y + h - 1);

      fl_color(shade_color(c[i] - 2, bc));
      fl_point(x + i + 1, y);
      fl_point(x + i + 1, y + h - 1);

      // Draw the right line and points...
      fl_color(shade_color(c[clen - i], bc));
      fl_line(x + w - 1 - i, y + 1, x + w - 1 - i, y + h - 1);

      fl_color(shade_color(c[clen - i] - 2, bc));
      fl_point(x + w - 1 - i, y);
      fl_point(x + w - 1 - i, y + h - 1);
    }

    // Draw the interior, top, and bottom...
    i = chalf / cstep;

    fl_color(shade_color(c[chalf], bc));
    fl_rectf(x + i, y + 1, w - 2 * i, h - 2);

    fl_color(shade_color(c[chalf - 2], bc));
    fl_line(x + i, y, x + w - i, y);
    fl_line(x + i, y + h - 1, x + w - i, y + h);
  }
}

Fl_Plastic_Box::Fl_Plastic_Box(const char* n, const char* s, const Fl_Frame_Box* d)
  : Fl_Frame_Box(n, s, d)
{
  fills_rectangle_ = 0;
}

const Fl_Plastic_Box fl_plastic_down_box(0, "STUVWWWVT");
const Fl_Plastic_Box fl_plastic_up_box(0, "TXSPPQQRSSTTUVS", &fl_plastic_down_box);

// up_frame =  "MNFKKLNO"
// down_frame = "LLRRTTLL"

//
// End of "$Id: fl_plastic_box.cxx,v 1.1 2002/02/10 22:57:49 spitzak Exp $".
//
