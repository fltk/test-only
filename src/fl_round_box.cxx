//
// "$Id: fl_round_box.cxx,v 1.19 1999/11/28 09:19:30 bill Exp $"
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

#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>
#include <string.h>

// A compiler from a certain very large software company will not compile
// the function pointer assignment due to the name conflict with fl_arc.
// This function is to fix that:
// SGI's compiler is broken too
void fl_arc_i(int x,int y,int w,int h,double a1,double a2) {
  fl_arc(x,y,w,h,a1,a2);
}

enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

static void arc(int which, int x,int y,int w,int h, Fl_Color color)
{
  int d = w <= h ? w : h;
  if (d <= 1) return;
  fl_color((Fl_Color)color);
  void (*f)(int,int,int,int,double,double);
  f = (which==FILL) ? fl_pie : fl_arc_i;
  if (which >= CLOSED) {
    f(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270);
  } else if (which == UPPER_LEFT) {
    f(x+w-d, y, d, d, 45, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, 225);
  } else { // LOWER_RIGHT
    f(x, y+h-d, d, d, 225, w<=h ? 360 : 270);
    f(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45);
  }
  if (which == FILL) {
    if (w < h)
      fl_rectf(x, y+d/2, w, h-(d&-2));
    else if (w > h)
      fl_rectf(x+d/2, y, w-(d&-2), h);
  } else {
    if (w < h) {
      if (which != UPPER_LEFT) fl_yxline(x+w-1, y+d/2, y+h-d/2);
      if (which != LOWER_RIGHT) fl_yxline(x, y+d/2, y+h-d/2);
    } else if (w > h) {
      if (which != UPPER_LEFT) fl_xyline(x+d/2, y+h-1, x+w-d/2);
      if (which != LOWER_RIGHT) fl_xyline(x+d/2, y, x+w-d/2);
    }
  }
}

extern void fl_to_inactive(const char* s, char* to);

void Fl_Round_Box::draw(int x, int y, int w, int h,
			Fl_Color c, Fl_Flags f) const
{
  const char* s = (f & FL_VALUE) ? data_from->down->data : data_from->data;
  char buf[26]; if (f&FL_INACTIVE && Fl_Style::draw_boxes_inactive) {
    fl_to_inactive(s, buf); s = buf;}
  if (!(f & FL_FRAME_ONLY)) {
    // draw the interior, assumming the edges are the same thickness
    // as the normal square box:
    int d = strlen(s)/4;
    if (w > 2*d && h > 2*(d-1)) arc(FILL, x+d, y+d-1, w-2*d, h-2*(d-1), c);
  }
  const char* t;
  if (*s == '2') {t = s+1; s += 3;} else {t = s+2;}
  while (*s && *t && w > 0 && h > 0) {
    Fl_Color c1 = *s + (FL_GRAY_RAMP-'A'); s += 4;
    Fl_Color c2 = *t + (FL_GRAY_RAMP-'A'); t += 4;
    arc(UPPER_LEFT,  x+1, y,   w-2, h, *s&&*t ? c1 : c);
    arc(UPPER_LEFT,  x,   y,   w,   h, c1);
    arc(LOWER_RIGHT, x+1, y,   w-2, h, *s&&*t ? c2 : c);
    arc(LOWER_RIGHT, x,   y,   w,   h, c2);
    x++; y++; w -= 2; h -= 2;
  }
}

void Fl_Round_Box::inset(int& x,int& y,int& w,int& h) const {
  data_from->inset(x,y,w,h);
}
int Fl_Round_Box::fills_rectangle() const {return false;}

const Fl_Round_Box fl_round_down_box(0, FL_DOWN_BOX);
const Fl_Round_Box fl_round_up_box(0, FL_UP_BOX);

//
// End of "$Id: fl_round_box.cxx,v 1.19 1999/11/28 09:19:30 bill Exp $".
//
