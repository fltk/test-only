//
// "$Id: fl_glyph.cxx,v 1.26 2001/03/11 16:14:30 spitzak Exp $"
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

#include <FL/Fl_Widget.H>
#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>

void fl_glyph(const Fl_Widget* widget, int t,
	      int x,int y,int w,int h, Fl_Flags f)
{
  Fl_Boxtype box;
  Fl_Color color = widget->get_glyph_color(f);

  // handle special glyphs that don't draw the box:
  switch (t) {

  case FL_GLYPH_CHECK:
    box = widget->text_box();
    if (box != FL_NO_BOX) {
      box->draw(widget, x,y,w,h, f&FL_INACTIVE|FL_FRAME_ONLY);
      box->inset(x,y,w,h);
      fl_color(widget->text_background());
      fl_rectf(x,y,w,h);
      color = fl_inactive(widget->text_color(),f);
    }
    if (f & FL_VALUE) {
      fl_color(color);
      x += 1;
      w = h - 2;
      int d1 = w/3;
      int d2 = w-d1;
      y += (h+d2)/2-d1-2;
      for (int n = 0; n < 3; n++, y++) {
	fl_line(x, y, x+d1, y+d1);
	fl_line(x+d1, y+d1, x+w-1, y+d1-d2+1);
      }
    }
    return;

  case FL_GLYPH_ROUND:
    h = (h+1)&(~1); // even only
    box = widget->text_box();
    if (box != FL_NO_BOX) {
      FL_ROUND_DOWN_BOX->draw(x,y,w,h,widget->text_background(),f&FL_INACTIVE);
      color = fl_inactive(widget->text_color(),f);
    }
    if (f & FL_VALUE) {
      int d = h/4; // box != FL_NO_BOX ? h/4 : 0; //h/5;
      fl_ellipse(x+d, y+d, h-d-d-1, h-d-d-1);
      fl_color(color); fl_fill();
    }
    return;

  case FL_GLYPH_UP:
  case FL_GLYPH_DOWN:
  case FL_GLYPH_LEFT:
  case FL_GLYPH_RIGHT:
    // these glyphs do not have a box
    break;

  case FL_GLYPH_CHOICE: {
    // make the box much smaller:
    int H = h/3;
    y += (h-H)/2;
    h = H;
  } // and fall through to default case to draw the box:
  default: {
    Fl_Boxtype box = widget->box();
    box->draw(widget,x,y,w,h,f);
    box->inset(x,y,w,h);
    }
  }

  // to draw the shape inactive, draw it twice to get the engraved look:
  int i = 0;
  if( f & FL_INACTIVE && !(f & FL_SELECTED)) 
	  i = 1;
  for ( /*i*/ ; i >= 0; i--) {
    fl_color(i ? Fl_Color(FL_LIGHT3) : color);

    int w1 = (w+2)/3; int x1,y1;
    switch(t) {

    case FL_GLYPH_UP_BUTTON:
    case FL_GLYPH_UP:
      x1 = x+(w-1)/2-w1+i;
      y1 = y+(h-w1-1)/2+i;
      fl_vertex(x1, y1+w1);
      fl_vertex(x1+2*w1, y1+w1);
      fl_vertex(x1+w1, y1);
      fl_fill_stroke(fl_color());
      break;

    case FL_GLYPH_DOWN_BUTTON:
    case FL_GLYPH_DOWN:
      x1 = x+(w-1)/2-w1+i;
      y1 = y+(h-w1)/2+i;
      fl_vertex(x1, y1);
      fl_vertex(x1+w1, y1+w1);
      fl_vertex(x1+2*w1, y1);
      fl_fill_stroke(fl_color());
      break;

    case FL_GLYPH_LEFT_BUTTON:
    case FL_GLYPH_LEFT:
      x1 = x+(w-w1-1)/2+i;
      y1 = y+(h-1)/2-w1+i;
      fl_vertex(x1, y1+w1);
      fl_vertex(x1+w1, y1+2*w1);
      fl_vertex(x1+w1, y1);
      fl_fill_stroke(fl_color());
      break;

    case FL_GLYPH_RIGHT_BUTTON:
    case FL_GLYPH_RIGHT:
      x1 = x+(w-w1)/2+i;
      y1 = y+(h-1)/2-w1+i;
      fl_vertex(x1, y1);
      fl_vertex(x1+w1, y1+w1);
      fl_vertex(x1, y1+2*w1);
      fl_fill_stroke(fl_color());
      break;

    case FL_GLYPH_VNSLIDER:
      y1 = (h-4)/2;
      FL_THIN_DOWN_BOX->draw(x, y+y1, w, h-2*y1, widget->selection_color());
      break;

    case FL_GLYPH_HNSLIDER:
      x1 = (w-4)/2;
      FL_THIN_DOWN_BOX->draw(x+x1, y, w-2*x1, h, widget->selection_color());
      break;
    }
  }
}

//
// End of "$Id: fl_glyph.cxx,v 1.26 2001/03/11 16:14:30 spitzak Exp $".
//
