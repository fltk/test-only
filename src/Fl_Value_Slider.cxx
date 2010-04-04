//
// "$Id$"
//
// Value slider widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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

#include <fltk3/run.h>
#include <fltk3/ValueSlider.h>
#include <fltk3/draw.h>
#include <math.h>

/**
  Creates a new fltk3::ValueSlider widget using the given
  position, size, and label string. The default boxtype is fltk3::DOWN_BOX.
*/
fltk3::ValueSlider::ValueSlider(int X, int Y, int W, int H, const char*l)
: fltk3::Slider(X,Y,W,H,l) {
  step(1,100);
  textfont_ = fltk3::HELVETICA;
  textsize_ = 10;
  textcolor_ = fltk3::FOREGROUND_COLOR;
}

void fltk3::ValueSlider::draw() {
  int sxx = x(), syy = y(), sww = w(), shh = h();
  int bxx = x(), byy = y(), bww = w(), bhh = h();
  if (horizontal()) {
    bww = 35; sxx += 35; sww -= 35;
  } else {
    syy += 25; bhh = 25; shh -= 25;
  }
  if (damage()&FL_DAMAGE_ALL) draw_box(box(),sxx,syy,sww,shh,color());
  fltk3::Slider::draw(sxx+fltk3::box_dx(box()),
		  syy+fltk3::box_dy(box()),
		  sww-fltk3::box_dw(box()),
		  shh-fltk3::box_dh(box()));
  draw_box(box(),bxx,byy,bww,bhh,color());
  char buf[128];
  format(buf);
  fl_font(textfont(), textsize());
  fl_color(active_r() ? textcolor() : fl_inactive(textcolor()));
  fl_draw(buf, bxx, byy, bww, bhh, fltk3::ALIGN_CLIP);
}

int fltk3::ValueSlider::handle(int event) {
  if (event == fltk3::PUSH && fltk3::visible_focus()) {
    fltk3::focus(this);
    redraw();
  }
  int sxx = x(), syy = y(), sww = w(), shh = h();
  if (horizontal()) {
    sxx += 35; sww -= 35;
  } else {
    syy += 25; shh -= 25;
  }
  return fltk3::Slider::handle(event,
			   sxx+fltk3::box_dx(box()),
			   syy+fltk3::box_dy(box()),
			   sww-fltk3::box_dw(box()),
			   shh-fltk3::box_dh(box()));
}

//
// End of "$Id$".
//
