//
// "$Id: fl_rounded_box.cxx,v 1.17 2002/01/23 08:46:02 spitzak Exp $"
//
// Rounded box drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl_Boxtype.h>
#include <fltk/fl_draw.h>

static void rbox(int x, int y, int w, int h, Fl_Color fill, Fl_Color line) {
  // figure out diameter of circles for corners:
  double d = w*4/5.0;
  double d1 = h*4/5.0;
  if (d1 < d) d = d1;
  if (d > 30) d = 30;
  double X = x;
  double Y = y;
  w--; h--;
  fl_arc(X, Y, d, d, 90, 180);
  fl_arc(X, Y+h-d, d, d, 180, 270);
  fl_arc(X+w-d, Y+h-d, d, d, 270, 360);
  fl_arc(X+w-d, Y, d, d, 0, 90);
  fl_color(fill); fl_fill_stroke(line);
}

void Fl_RFlat_Box::draw(
	int x, int y, int w, int h, Fl_Color color, Fl_Flags) const
{
  rbox(x, y, w, h, color, color);
}
Fl_RFlat_Box::Fl_RFlat_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 7; dw_ = dh_ = 14;
  fills_rectangle_ = 0;
}
const Fl_RFlat_Box fl_rflat_box(0);

void Fl_Rounded_Box::draw(
	int x, int y, int w, int h, Fl_Color color, Fl_Flags f) const
{
  rbox(x, y, w, h, color, fl_inactive(FL_BLACK,f));
}
Fl_Rounded_Box::Fl_Rounded_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1; dw_ = dh_ = 2;
  fills_rectangle_ = 0;
}
const Fl_Rounded_Box fl_rounded_box(0);

void Fl_RShadow_Box::draw(
	int x, int y, int w, int h, Fl_Color color, Fl_Flags f) const
{
  w -= 3; h -= 3;
  // draw shadow:
  rbox(x+3, y+3, w, h, FL_DARK3, FL_DARK3);
  // draw the box:
  fl_rounded_box.draw(x, y, w, h, color, f);
}
Fl_RShadow_Box::Fl_RShadow_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1; dw_ = dh_ = 5;
  fills_rectangle_ = 0;
}
const Fl_RShadow_Box fl_rshadow_box(0);

//
// End of "$Id: fl_rounded_box.cxx,v 1.17 2002/01/23 08:46:02 spitzak Exp $".
//
