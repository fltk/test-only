//
// "$Id$"
//
// Lighted button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Subclass of fltk3::Button where the "box" indicates whether it is
// pushed or not, and the "down box" is drawn small and square on
// the left to indicate the current state.

// The default down_box of zero draws a rectangle designed to look
// just like Flame's buttons.

#include <fltk3/run.h>
#include <fltk3/LightButton.h>
#include <fltk3/draw.h>
#include "flstring.h"

void fltk3::LightButton::draw() {
  FLTK3_OBJECT_VCALLS_WRAPPER(draw(), Draw)
  if (box()) draw_box(this==fltk3::pushed() ? fltk3::down(box()) : box(), color());
  fltk3::Color col = value() ? (active_r() ? selection_color() :
                            fltk3::inactive(selection_color())) : color();
  int W;
  int dx, dy;

  W  = labelsize();
  dx = fltk3::box_dx(box()) + 2;
  dy = (h() - W) / 2;
  // if (dy < 0) dy = 0;         // neg. offset o.k. for vertical centering

  if (down_box()) {
    // draw other down_box() styles:
    switch (down_box()) {
      case fltk3::DOWN_BOX :
      case fltk3::UP_BOX :
      case fltk3::PLASTIC_DOWN_BOX :
      case fltk3::PLASTIC_UP_BOX :
        // Check box...
        draw_box(down_box(), x()+dx, y()+dy, W, W, fltk3::BACKGROUND2_COLOR);
	if (value()) {
	  if (fltk3::scheme() && !strcmp(fltk3::scheme(), "gtk+")) {
	    fltk3::color(fltk3::SELECTION_COLOR);
	  } else {
	    fltk3::color(col);
	  }
	  int tx = x() + dx + 3;
	  int tw = W - 6;
	  int d1 = tw/3;
	  int d2 = tw-d1;
	  int ty = y() + dy + (W+d2)/2-d1-2;
	  for (int n = 0; n < 3; n++, ty++) {
	    fltk3::line(tx, ty, tx+d1, ty+d1);
	    fltk3::line(tx+d1, ty+d1, tx+tw-1, ty+d1-d2+1);
	  }
	}
        break;
      case fltk3::ROUND_DOWN_BOX :
      case fltk3::ROUND_UP_BOX :
        // Radio button...
        draw_box(down_box(), x()+dx, y()+dy, W, W, fltk3::BACKGROUND2_COLOR);
	if (value()) {
	  int tW = (W - fltk3::box_dw(down_box())) / 2 + 1;
	  if ((W - tW) & 1) tW++; // Make sure difference is even to center
	  int tdx = dx + (W - tW) / 2;
	  int tdy = dy + (W - tW) / 2;

	  if (fltk3::scheme() && !strcmp(fltk3::scheme(), "gtk+")) {
	    fltk3::color(fltk3::SELECTION_COLOR);
	    tW --;
	    fltk3::pie(x() + tdx - 1, y() + tdy - 1, tW + 3, tW + 3, 0.0, 360.0);
	    fltk3::arc(x() + tdx - 1, y() + tdy - 1, tW + 3, tW + 3, 0.0, 360.0);
	    fltk3::color(fltk3::color_average(fltk3::WHITE, fltk3::SELECTION_COLOR, 0.2f));
	  } else fltk3::color(col);

	  switch (tW) {
	    // Larger circles draw fine...
	    default :
              fltk3::pie(x() + tdx, y() + tdy, tW, tW, 0.0, 360.0);
	      break;

            // Small circles don't draw well on many systems...
	    case 6 :
	      fltk3::rectf(x() + tdx + 2, y() + tdy, tW - 4, tW);
	      fltk3::rectf(x() + tdx + 1, y() + tdy + 1, tW - 2, tW - 2);
	      fltk3::rectf(x() + tdx, y() + tdy + 2, tW, tW - 4);
	      break;

	    case 5 :
	    case 4 :
	    case 3 :
	      fltk3::rectf(x() + tdx + 1, y() + tdy, tW - 2, tW);
	      fltk3::rectf(x() + tdx, y() + tdy + 1, tW, tW - 2);
	      break;

	    case 2 :
	    case 1 :
	      fltk3::rectf(x() + tdx, y() + tdy, tW, tW);
	      break;
	  }

	  if (fltk3::scheme() && !strcmp(fltk3::scheme(), "gtk+")) {
	    fltk3::color(fltk3::color_average(fltk3::WHITE, fltk3::SELECTION_COLOR, 0.5));
	    fltk3::arc(x() + tdx, y() + tdy, tW + 1, tW + 1, 60.0, 180.0);
	  }
	}
        break;
      default :
        draw_box(down_box(), x()+dx, y()+dy, W, W, col);
        break;
    }
  } else {
    // if down_box() is zero, draw light button style:
    int hh = h()-2*dy - 2;
    int ww = W/2+1;
    int xx = dx;
    if (w()<ww+2*xx) xx = (w()-ww)/2;
    if (fltk3::scheme() && !strcmp(fltk3::scheme(), "plastic")) {
      col = active_r() ? selection_color() : fltk3::inactive(selection_color());
      fltk3::color(value() ? col : fltk3::color_average(col, fltk3::BLACK, 0.5f));
      fltk3::pie(x()+xx, y()+dy+1, ww, hh, 0, 360);
    } else {
      draw_box(fltk3::THIN_DOWN_BOX, x()+xx, y()+dy+1, ww, hh, col);
    }
    dx = (ww + 2 * dx - W) / 2;
  }
  draw_label(x()+W+2*dx, y(), w()-W-2*dx, h());
  if (fltk3::focus() == this) draw_focus();
}

int fltk3::LightButton::handle(int event) {
  FLTK3_OBJECT_VCALLS_WRAPPER_RET(int, handle(event), Handle)
  switch (event) {
  case fltk3::RELEASE:
    if (box()) redraw();
  default:
    return fltk3::Button::handle(event);
  }
}

/**
  Creates a new fltk3::LightButton widget using the given
  position, size, and label string.
  <P>The destructor deletes the check button.
*/
fltk3::LightButton::LightButton(int X, int Y, int W, int H, const char* l)
: fltk3::Button(X, Y, W, H, l) {
  type(fltk3::TOGGLE_BUTTON);
  selection_color(fltk3::YELLOW);
  align(fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE);
}

//
// End of "$Id$".
//
