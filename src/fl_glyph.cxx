//
// "$Id: fl_glyph.cxx,v 1.37 2003/09/03 06:08:07 spitzak Exp $"
//
// Glyph drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/Widget.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
using namespace fltk;

void Widget::default_glyph(const Widget* widget, int glyph,
			   int x,int y,int w,int h, Flags flags)
{
  Color bg, fg;
  if ((flags & HIGHLIGHT) && (bg = widget->highlight_color())) {
    fg = widget->highlight_textcolor();
  } else if (flags & SELECTED) {
    bg = widget->selection_color();
    fg = widget->selection_textcolor();
  } else {
    bg = widget->buttoncolor();
    fg = widget->textcolor();
  }

  // handle special glyphs that don't draw the box:
  switch (glyph) {
  case GLYPH_UP:
  case GLYPH_DOWN:
  case GLYPH_LEFT:
  case GLYPH_RIGHT:
    // these glyphs do not have a box
    break;
  default: {
    Box* box = widget->buttonbox();
    box->draw(x,y,w,h, bg, flags);
    box->inset(x,y,w,h);
    }
  }

  // to draw the shape inactive, draw it twice to get the engraved look:
  int i = 0;
  if (flags & INACTIVE) {
    i = 1;
    fg = inactive(fg);
  }
  for ( /*i*/ ; i >= 0; i--) {
    Color color = i ? Color(GRAY99) : fg;
    setcolor(color);

    int w1 = (w+2)/3; int x1,y1;
    switch(glyph) {

    case GLYPH_UP_BUTTON:
    case GLYPH_UP:
      x1 = x+(w-1)/2-w1+i;
      y1 = y+(h-w1-1)/2+i;
      addvertex(x1, y1+w1);
      addvertex(x1+2*w1, y1+w1);
      addvertex(x1+w1, y1);
      fillstrokepath(color);
      break;

    case GLYPH_DOWN_BUTTON:
    case GLYPH_DOWN:
      x1 = x+(w-1)/2-w1+i;
      y1 = y+(h-w1)/2+i;
      addvertex(x1, y1);
      addvertex(x1+w1, y1+w1);
      addvertex(x1+2*w1, y1);
      fillstrokepath(color);
      break;

    case GLYPH_LEFT_BUTTON:
    case GLYPH_LEFT:
      x1 = x+(w-w1-1)/2+i;
      y1 = y+(h-1)/2-w1+i;
      addvertex(x1, y1+w1);
      addvertex(x1+w1, y1+2*w1);
      addvertex(x1+w1, y1);
      fillstrokepath(color);
      break;

    case GLYPH_RIGHT_BUTTON:
    case GLYPH_RIGHT:
      x1 = x+(w-w1)/2+i;
      y1 = y+(h-1)/2-w1+i;
      addvertex(x1, y1);
      addvertex(x1+w1, y1+w1);
      addvertex(x1, y1+2*w1);
      fillstrokepath(color);
      break;

    }
  }
}

//
// End of "$Id: fl_glyph.cxx,v 1.37 2003/09/03 06:08:07 spitzak Exp $".
//
