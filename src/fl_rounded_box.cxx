//
// "$Id: fl_rounded_box.cxx,v 1.13 2000/08/10 09:24:32 spitzak Exp $"
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

#include <FL/Fl_Boxtype.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>

#define RN	5
#define RS	15
#define BW	3

static double offset[RN] = { 0.0, 0.07612, 0.29289, 0.61732, 1.0};

static void rbox(int fill, int x, int y, int w, int h) {
  int i;
  int rsx ,rsy, rs;
  rsx = w*2/5; rsy = h*2/5;
  if (rsx > rsy) rs = rsy; else  rs = rsx;
  if (rs > RS) rs = RS;
  rsx = rs; rsy = rs;

  if (fill) fl_begin_polygon(); else fl_begin_loop();
  for (i=0; i<RN; i++)
    fl_vertex(x + offset[RN-i-1]*rsx, y + offset[i] * rsy);
  for (i=0; i<RN; i++)
    fl_vertex(x + offset[i]*rsx, y + h-1 - offset[RN-i-1] * rsy);
  for (i=0; i<RN; i++)
    fl_vertex(x + w-1 - offset[RN-i-1]*rsx, y + h-1 - offset[i] * rsy);
  for (i=0; i<RN; i++)
    fl_vertex(x + w-1 - offset[i]*rsx, y + offset[RN-i-1] * rsy);
  if (fill) fl_end_polygon(); else fl_end_loop();
}

void Fl_RFlat_Box::draw(const Fl_Widget* widget,
			int x, int y, int w, int h, Fl_Flags f) const
{
  fl_color(widget->box_color(f));
  rbox(1, x, y, w, h);
  rbox(0, x, y, w, h);
}
Fl_RFlat_Box::Fl_RFlat_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 7; dw_ = dh_ = 14;
  fills_rectangle_ = 0;
}
const Fl_RFlat_Box fl_rflat_box(0);

void Fl_Rounded_Box::draw(const Fl_Widget* widget,
			  int x, int y, int w, int h, Fl_Flags f) const
{
  fl_color(widget->box_color(f));
  rbox(1, x, y, w, h);
  fl_color(widget->glyph_color(f));
  rbox(0, x, y, w, h);
}
Fl_Rounded_Box::Fl_Rounded_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1; dw_ = dh_ = 2;
  fills_rectangle_ = 0;
}
const Fl_Rounded_Box fl_rounded_box(0);

void Fl_RShadow_Box::draw(const Fl_Widget* widget,
			  int x, int y, int w, int h, Fl_Flags f) const
{
  w-=BW; h-=BW;
  // draw shadow:
  fl_color(FL_DARK3);
  rbox(1, x+BW, y+BW, w, h);
  rbox(0, x+BW, y+BW, w, h);
  // draw the box:
  fl_rounded_box.draw(widget, x, y, w, h, f);
}
Fl_RShadow_Box::Fl_RShadow_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1; dw_ = dh_ = 2+BW;
  fills_rectangle_ = 0;
}
const Fl_RShadow_Box fl_rshadow_box(0);

//
// End of "$Id: fl_rounded_box.cxx,v 1.13 2000/08/10 09:24:32 spitzak Exp $".
//
