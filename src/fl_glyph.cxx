//
// "$Id: fl_glyph.cxx,v 1.19 2000/04/15 04:47:24 carl Exp $"
//
// Glyph drawing code for the Fast Light Tool Kit (FLTK).
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


#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>

void fl_glyph(int t, int x,int y,int w,int h,
	      Fl_Color bc, Fl_Color fc, Fl_Flags f, Fl_Boxtype box)
{
  fc = fl_inactive(fc, f);
  bc = fl_inactive(bc, f);

  // handle special glyphs that don't draw the box:
  switch (t) {
    case FL_GLYPH_CHECK:
      box->draw(x,y,w,h, bc, f & ~(FL_VALUE|FL_FOCUSED));
      if (f & FL_VALUE) {
        fl_color(fc);
        box->inset(x,y,w,h);
        x += 1;
        w = h - 2;
        int d1 = w/3;
        int d2 = w-d1;
        y = y+(h+d2)/2-d1-2;
        fl_line(x, y, x+d1, y+d1, x+w-1, y+d1-d2+1);
        y++;
        fl_line(x, y, x+d1, y+d1, x+w-1, y+d1-d2+1);
        y++;
        fl_line(x, y, x+d1, y+d1, x+w-1, y+d1-d2+1);
      }
      return;

    case FL_GLYPH_ROUND:
      h = (h+1)&(~1); // even only
      if (box != FL_NO_BOX)
	FL_ROUND_DOWN_BOX->draw(x,y,h,h, bc, f&~(FL_VALUE|FL_FOCUSED));
      if (f & FL_VALUE) {
        fl_color(fc);
        int d = h/4; // box != FL_NO_BOX ? h/4 : 0; //h/5;
        fl_pie(x+d,y+d,h-d-d-1,h-d-d-1,0,360);
      }
      return;

  }

  box->draw(x,y,w,h, bc, f);
  box->inset(x,y,w,h);
  fl_color(fc);

  switch(t) {
    case FL_GLYPH_UP: {
      int w1 = w/3; if (w1 < 1) w1 = 1;
      int x1 = x+(w-2*w1-1)/2;
      int y1 = y+(h-w1-1)/2;
      fl_polygon(x1, y1+w1, x1+2*w1, y1+w1, x1+w1, y1);
      break;
    }
    case FL_GLYPH_DOWN: {
      int w1 = w/3; if (w1 < 1) w1 = 1;
      int x1 = x+(w-2*w1-1)/2;
      int y1 = y+(h-w1-1)/2;
      fl_polygon(x1, y1, x1+w1, y1+w1, x1+2*w1, y1);
      break;
    }
    case FL_GLYPH_LEFT: {
      int w1 = h/3; if (w1 < 1) w1 = 1;
      int x1 = x+(w-w1-1)/2;
      int y1 = y+(h-2*w1-1)/2;
      fl_polygon(x1, y1+w1, x1+w1, y1+2*w1, x1+w1, y1);
      break;
    }
    case FL_GLYPH_RIGHT: {
      int w1 = h/3; if (w1 < 1) w1 = 1;
      int x1 = x+(w-w1-1)/2;
      int y1 = y+(h-2*w1-1)/2;
      fl_polygon(x1, y1, x1, y1+2*w1, x1+w1, y1+w1);
      break;
    }
    case FL_GLYPH_VNSLIDER: {
      int d = (h-4)/2;
      FL_THIN_DOWN_BOX->draw(x, y+d, w, h-2*d, fc);
      break;
    }
    case FL_GLYPH_HNSLIDER: {
      int d = (w-4)/2;
      FL_THIN_DOWN_BOX->draw(x+d, y, w-2*d, h, fc);
      break;
    }
    case FL_GLYPH_CHOICE: {
      int H = h/3;
      int Y = y + (h-H)/2;
      FL_UP_BOX->draw(x,Y,w,H, bc, f);
      break;
    }
  }
}

//
// End of "$Id: fl_glyph.cxx,v 1.19 2000/04/15 04:47:24 carl Exp $".
//
