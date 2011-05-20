//
// "$Id$"
//
// Value output widget for the Fast Light Tool Kit (FLTK).
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

// Fltk widget for drag-adjusting a floating point value.
// This is much lighter than Fl_Value_Input because it has no text editor
// If step() is zero then it can be used to display a floating-point value

#include <fltk3/run.h>
#include <fltk3/ValueOutput.h>
#include <fltk3/draw.h>

void fltk3::ValueOutput::draw() {
  fltk3::Boxtype b = box() ? box() : fltk3::DOWN_BOX;
  int X = x()+fltk3::box_dx(b);
  int Y = y()+fltk3::box_dy(b);
  int W = w()-fltk3::box_dw(b);
  int H = h()-fltk3::box_dh(b);
  if (damage()&~fltk3::DAMAGE_CHILD)
    draw_box(b, color());
  else {
    fltk3::color(color());
    fltk3::rectf(X, Y, W, H);
  }
  char buf[128];
  format(buf);
  fltk3::color(active_r() ? textcolor() : fltk3::inactive(textcolor()));
  fltk3::font(textfont(), textsize());
  fltk3::draw(buf,X,Y,W,H,fltk3::ALIGN_LEFT);
}

int fltk3::ValueOutput::handle(int event) {
  if (!step()) return 0;
  double v;
  int delta;
  int mx = fltk3::event_x();
  static int ix, drag;
  switch (event) {
  case fltk3::PUSH:
    ix = mx;
    drag = fltk3::event_button();
    handle_push();
    return 1;
  case fltk3::DRAG:
    delta = fltk3::event_x()-ix;
    if (delta > 5) delta -= 5;
    else if (delta < -5) delta += 5;
    else delta = 0;
    switch (drag) {
    case 3: v = increment(previous_value(),delta*100); break;
    case 2: v = increment(previous_value(),delta*10); break;
    default:v = increment(previous_value(),delta); break;
    }
    v = round(v);
    handle_drag(soft()?softclamp(v):clamp(v));;
    return 1;
  case fltk3::RELEASE:
    handle_release();
    return 1;
  case fltk3::ENTER :
  case fltk3::LEAVE :
    return 1;
  default:
    return 0;
  }
}

/**
  Creates a new fltk3::ValueOutput widget using the given
  position, size, and label string. The default boxtype is fltk3::NO_BOX.
  <P> Inherited destructor destroys the Valuator.
*/
fltk3::ValueOutput::ValueOutput(int X, int Y, int W, int H,const char *l)
: fltk3::Valuator(X,Y,W,H,l) {
  box(fltk3::NO_BOX);
  align(fltk3::ALIGN_LEFT);
  textfont_ = fltk3::HELVETICA;
  textsize_ = fltk3::NORMAL_SIZE;
  textcolor_ = fltk3::FOREGROUND_COLOR;
  soft_ = 0;
}

//
// End of "$Id$".
//
