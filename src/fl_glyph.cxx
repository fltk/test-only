//
// "$Id: fl_glyph.cxx,v 1.29 2002/01/23 08:46:01 spitzak Exp $"
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

#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Style.h>
#include <fltk/fl_draw.h>

void fl_glyph(const Fl_Widget* widget, int glyph,
	      int x,int y,int w,int h, Fl_Flags flags)
{
  // handle special glyphs that don't draw the box:
  switch (glyph) {

  case FL_GLYPH_CHECK: {
    Fl_Boxtype box = widget->button_box();
    box->draw(x, y, w, h, widget->color(), flags&FL_INACTIVE|FL_VALUE);
    box->inset(x, y, w, h);
    if (flags & FL_VALUE) {
      fl_color(fl_inactive(widget->text_color(),flags));
      x += 1;
      w = h - 2;
      int d1 = w/3;
      int d2 = w-d1;
      y += (h+d2)/2-d1-2;
      for (int n = 0; n < 3; n++, y++) {
	fl_line(x, y, x+d1, y+d1);
	fl_line(x+d1, y+d1, x+w-1, y+d1-d2+1);
      }
    }}
    return;

  case FL_GLYPH_ROUND:
    h = (h+1)&(~1); // even only
    FL_ROUND_DOWN_BOX->draw(x, y, w, h, widget->color(), flags&FL_INACTIVE);
    if (flags & FL_VALUE) {
      int d = h/4;
      fl_ellipse(x+d, y+d, h-d-d-1, h-d-d-1);
      fl_color(fl_inactive(widget->text_color(),flags)); fl_fill();
    }
    return;

  case FL_GLYPH_UP:
  case FL_GLYPH_DOWN:
  case FL_GLYPH_LEFT:
  case FL_GLYPH_RIGHT:
    // these glyphs do not have a box
    break;

  default: {
    Fl_Boxtype box = widget->button_box();
    if (box != FL_NO_BOX) {
      Fl_Color color;
      if (flags & FL_SELECTED) color = widget->selection_color();
      else if (flags & FL_HIGHLIGHT && (color = widget->highlight_color())) ;
      else color = widget->button_color();
      box->draw(x,y,w,h, color, flags);
      box->inset(x,y,w,h);
    }}
  }

  Fl_Color color;
  if (flags & FL_SELECTED)
    color = widget->selection_text_color();
  else if (flags&FL_HIGHLIGHT && (color = widget->highlight_label_color()))
    ;
  else
    color = widget->text_color();
  // to draw the shape inactive, draw it twice to get the engraved look:
  int i = 0;
  if (flags & FL_INACTIVE) {
    i = 1;
    color = fl_inactive(color);
  }
  for ( /*i*/ ; i >= 0; i--) {
    fl_color(i ? Fl_Color(FL_LIGHT3) : color);

    int w1 = (w+2)/3; int x1,y1;
    switch(glyph) {

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
// End of "$Id: fl_glyph.cxx,v 1.29 2002/01/23 08:46:01 spitzak Exp $".
//
