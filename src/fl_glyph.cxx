//
// "$Id: fl_glyph.cxx,v 1.14 1999/12/08 17:40:34 bill Exp $"
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
  switch(t) {
    case FL_GLYPH_UP: {
      box->draw(x,y,w,h, bc, f);
      fl_color(fc);
      x += 2; y += 2; w -= 4; h -= 4;
      int w1 = (w-1)|1; // use odd sizes only
      int X1 = x+w1/2;
      int W1 = w1/3;
      int Y1 = y+w1/2+W1/2;
      fl_polygon(X1, Y1-W1, X1+W1, Y1, X1-W1, Y1);
      break;
    }
    case FL_GLYPH_DOWN: {
      box->draw(x,y,w,h, bc, f);
      x += 2; y += 2; w -= 4; h -= 4;
      fl_color(fc);
      int w1 = (w-1)|1; // use odd sizes only
      int X1 = x+w1/2;
      int W1 = w1/3;
      int Y1 = y+h-w1/2-W1/2-1;
      fl_polygon(X1, Y1+W1, X1-W1, Y1, X1+W1, Y1);
      break;
    }
    case FL_GLYPH_LEFT: {
      box->draw(x,y,w,h, bc, f);
      x += 2; y += 2; w -= 4; h -= 4;
      fl_color(fc);
      int w1 = (h-1)|1; // use odd sizes only
      int Y1 = y+w1/2;
      int W1 = w1/3;
      int X1 = x+w1/2+W1/2;
      fl_polygon(X1-W1, Y1, X1, Y1-W1, X1, Y1+W1);
      break;
    }
    case FL_GLYPH_RIGHT: {
      box->draw(x,y,w,h, bc, f);
      fl_color(fc);
      x += 2; y += 2; w -= 4; h -= 4;
      int w1 = (h-1)|1; // use odd sizes only
      int Y1 = y+w1/2;
      int W1 = w1/3;
      int X1 = x+w-w1/2-W1/2-1;
      fl_polygon(X1+W1, Y1, X1, Y1+W1, X1, Y1-W1);
      break;
    }
    case FL_GLYPH_CHECK: {
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
      break;
    }
    case FL_GLYPH_RADIO: {
      h = (h+1)&(~1); // even only
      if (box != FL_NO_BOX)
	FL_ROUND_DOWN_BOX->draw(x,y,h,h, bc, f&~(FL_VALUE|FL_FOCUSED));
      if (f & FL_VALUE) {
        fl_color(fc);
        int d = h/4; // box != FL_NO_BOX ? h/4 : 0; //h/5;
        fl_pie(x+d,y+d,h-d-d-1,h-d-d-1,0,360);
      }
      break;
    }
    case FL_GLYPH_VNSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int d = (h-4)/2;
      FL_THIN_DOWN_BOX->draw(x+2, y+d, w-4, h-2*d, fc);
      break;
    }
    case FL_GLYPH_HNSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int d = (w-4)/2;
      FL_THIN_DOWN_BOX->draw(x+d, y+2, w-2*d, h-4, fc);
      break;
    }
    default:
      box->draw(x,y,w,h, bc, f);
  }
}

//
// End of "$Id: fl_glyph.cxx,v 1.14 1999/12/08 17:40:34 bill Exp $".
//
