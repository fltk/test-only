//
// "$Id: fl_rounded_box.cxx,v 1.8 1999/11/01 02:21:39 carl Exp $"
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

static void rflat_draw(Fl_Boxtype, int x, int y, int w, int h,
		       Fl_Color c, Fl_Flags)
{
  fl_color(c); rbox(1, x, y, w, h); rbox(0, x, y, w, h);
}
const Fl_Boxtype_ fl_rflat_box = {
  rflat_draw, 0, 0, 0, 7,7,14,14,
};

static void rounded_draw(Fl_Boxtype, int x, int y, int w, int h,
			 Fl_Color c, Fl_Flags f)
{
  fl_color(c); rbox(1, x, y, w, h);
  fl_color((f&FL_INACTIVE) ? FL_INACTIVE_COLOR : FL_NO_COLOR);
  rbox(0, x, y, w, h);
}
const Fl_Boxtype_ fl_rounded_box = {
  rounded_draw, 0, 0, 0, 1,1,2,2,
};

static void rshadow_draw(Fl_Boxtype b, int x, int y, int w, int h,
			 Fl_Color c, Fl_Flags f)
{
  w-=BW; h-=BW;
  // draw shadow:
  fl_color(FL_DARK3);
  rbox(1, x+BW, y+BW, w, h);
  rbox(0, x+BW, y+BW, w, h);
  // draw the box:
  rounded_draw(b, x, y, w, h, c, f);
}
const Fl_Boxtype_ fl_rshadow_box = {
  rshadow_draw, 0, 0, 0, 1,1,2+BW,2+BW,
};

//
// End of "$Id: fl_rounded_box.cxx,v 1.8 1999/11/01 02:21:39 carl Exp $".
//
